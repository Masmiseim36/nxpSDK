/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
   Copyright 2018-2019 NXP. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

/* File modified by NXP. Changes are described in file
   /middleware/eiq/tensorflow-lite/readme.txt in section "Release notes" */

#include "board.h"

#include "fsl_sai.h"
#include "fsl_codec_common.h"

#include "fsl_wm8960.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_codec_adapter.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/string_util.h"

#include "timer.h"
#include "get_top_n.h"
#include "ds_cnn_s_model.h"
#include "kws_mfcc.h"

#include "commands.h"


#define LOG(x) std::cout
#define TF_QUANTIZED
#define LOG(x) std::cout

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI SAI1
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_SAI_IRQ SAI1_IRQn
#define SAI_TxIRQHandler SAI1_IRQHandler

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* I2C instance and clock */
#define DEMO_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define OVER_SAMPLE_RATE (384U)
#define BUFFER_SIZE (1024U)
/* 16Khz * 2 bytes */
#define AUDIO_NUM (16U * 2)
/* Two buffers are switching for simultaneously reading and writing. */
#define BUFFER_NUMBER (2U * AUDIO_NUM)
#define BUFFER_TOTAL_SIZE (BUFFER_SIZE * BUFFER_NUMBER)
#define STRIDE_SIZE (STRIDE_MS * AUDIO_NUM)
#define DETECTION_TRESHOLD 30

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)
/* demo audio master clock */
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
#define DEMO_AUDIO_MASTER_CLOCK OVER_SAMPLE_RATE *DEMO_AUDIO_SAMPLE_RATE
#else
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#endif
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (1U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth16bits

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool dataReady = false;
sai_transfer_t xferRx;
sai_transfer_t xferTx;

uint8_t codecHandleBuffer[CODEC_HANDLE_SIZE];
codec_handle_t *codecHandle = reinterpret_cast<codec_handle_t*>(codecHandleBuffer);
wm8960_config_t wm8960Config;
codec_config_t boardCodecConfig;

static uint8_t indexRx = 0U, indexTx = 0U, indexStart = 0U;

/*!
 * @brief AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
clock_audio_pll_config_t audioPllConfig;

#if !defined(__ARMCC_VERSION)
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_TOTAL_SIZE], 4);
#else
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint8_t audioBuff[BUFFER_TOTAL_SIZE], 4);
#endif

sai_handle_t txHandle;
sai_handle_t rxHandle;

extern codec_config_t boardCodecConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initializes audio - PPL configuration and wm8960 configuration.
 *
 * @param Enable Sai Mclk output flag. Set tru for output otherwise input is used
 */
void audio_init()
{
  audioPllConfig.loopDivider = 32;  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
  audioPllConfig.postDivider = 1;   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
  audioPllConfig.numerator = 77;    /* 30 bit numerator of fractional loop divider. */
  audioPllConfig.denominator = 100; /* 30 bit denominator of fractional loop divider */

  wm8960Config.i2cConfig.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE;
  wm8960Config.i2cConfig.codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ;
  wm8960Config.route = kWM8960_RoutePlaybackandRecord;
  wm8960Config.rightInputSource = kWM8960_InputDifferentialMicInput2;
  wm8960Config.playSource = kWM8960_PlaySourceDAC;
  wm8960Config.slaveAddress = WM8960_I2C_ADDR;
  wm8960Config.bus = kWM8960_BusI2S;
  wm8960Config.format.mclk_HZ = 6144000U;
  wm8960Config.format.sampleRate = kWM8960_AudioSampleRate16KHz;
  wm8960Config.format.bitWidth = kWM8960_AudioBitWidth16bit;
  wm8960Config.master_slave = false;

  boardCodecConfig.codecDevType = kCODEC_WM8960;
  boardCodecConfig.codecDevConfig = &wm8960Config;
}

/*!
 * @brief Enables Sai output Mclk output
 *
 * @param Enables Sai Mclk output flag. Set tru for output otherwise input is used
 */
void BOARD_EnableSaiMclkOutput(bool enable)
{
  if (enable)
  {
    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
  }
  else
  {
    IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
  }
}

/*!
 * @brief RX callback
 *
 * @param pointer to I2S base address
 * @param pointer to sai edma handler
 * @param status
 * @param pointer to user data
 */
static void rx_callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
  if (kStatus_SAI_RxError == status)
  {
    LOG(INFO) << "SAI_Rx failed!\r\n";
    return;
  }
  else
  {
    xferRx.data = audioBuff + indexRx * BUFFER_SIZE;
    xferRx.dataSize = BUFFER_SIZE;
    if (kStatus_Success == SAI_TransferReceiveNonBlocking(DEMO_SAI, &rxHandle, &xferRx))
    {
      indexTx = indexRx;
      indexRx++;
    }

    if ((indexRx % AUDIO_NUM) == 0U)
    {
      indexStart = indexRx - AUDIO_NUM;
      dataReady = true;
    }

    if (indexRx == BUFFER_NUMBER)
    {
      indexRx = 0U;
    }
  }
}

/*!
 * @brief TX callback
 *
 * @param pointer to I2S base address
 * @param pointer to sai edma handler
 * @param status
 * @param pointer to user data
 */
static void tx_callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
  if (kStatus_SAI_TxError == status)
  {
    return;
  }
  else
  {
    xferTx.dataSize = BUFFER_SIZE;
    xferTx.data = audioBuff + indexTx * BUFFER_SIZE;

    if (SAI_TransferSendNonBlocking(base, &txHandle, &xferTx) != kStatus_Success)
    {
      LOG(INFO) << "SAI_Tx failed!\r\n";
      return;
    }
  }
}

/*!
 * @brief Starts record playback and sets up RX and TX handlers
 *
 * @param pointer to I2S base address
 */
void RecordPlayback(I2S_Type *base)
{
  sai_transfer_t xfer = {NULL, 0};
  memset(audioBuff, 0, BUFFER_SIZE * BUFFER_NUMBER);
  SAI_TxSoftwareReset(base, kSAI_ResetTypeSoftware);
  SAI_RxSoftwareReset(base, kSAI_ResetTypeSoftware);

  xfer.dataSize = BUFFER_SIZE;
  xfer.data = audioBuff;

  SAI_TransferSendNonBlocking(base, &txHandle, &xfer);
  SAI_TransferReceiveNonBlocking(base, &rxHandle, &xfer);
}

/*!
 * @brief Initialize @parameters for inference
 *
 * @param reference to flat buffer
 * @param reference to interpreter
 * @param pointer to storing input tensor address
 * @param verbose mode flag. Set true for verbose mode
 */
void InferenceInit(std::unique_ptr<tflite::FlatBufferModel> &model,
                   std::unique_ptr<tflite::Interpreter> &interpreter,
                   TfLiteTensor** input_tensor, bool isVerbose)
{
  model = tflite::FlatBufferModel::BuildFromBuffer((const char*)ds_cnn_s_model, ds_cnn_s_model_len);
  if (!model)
  {
    LOG(FATAL) << "\nFailed to load model \r\n";
    return;
  }

  tflite::ops::builtin::BuiltinOpResolver resolver;

  tflite::InterpreterBuilder(*model, resolver)(&interpreter);
  if (!interpreter)
  {
    LOG(FATAL) << "Failed to construct interpreterr\r\n";
    return;
  }

  int input = interpreter->inputs()[0];

  if (interpreter->AllocateTensors() != kTfLiteOk)
  {
    LOG(FATAL) << "Failed to allocate tensors!\r\n";
    return;
  }

  /* Get input dimension from the input tensor metadata
     assuming one input only */
  *input_tensor = interpreter->tensor(input);
  
  if (isVerbose)
  {
    const std::vector<int> inputs = interpreter->inputs();
    const std::vector<int> outputs = interpreter->outputs();

    LOG(INFO) << "input: " << inputs[0] << "\r\n";
    LOG(INFO) << "number of inputs: " << inputs.size() << "\r\n";
    LOG(INFO) << "number of outputs: " << outputs.size() << "\r\n";

    LOG(INFO) << "tensors size: " << interpreter->tensors_size() << "\r\n";
    LOG(INFO) << "nodes size: " << interpreter->nodes_size() << "\r\n";
    LOG(INFO) << "inputs: " << interpreter->inputs().size() << "\r\n";
    LOG(INFO) << "input(0) name: " << interpreter->GetInputName(0) << "\r\n";

    int t_size = interpreter->tensors_size();
    for (int i = 0; i < t_size; i++)
    {
      if (interpreter->tensor(i)->name)
      {
        LOG(INFO) << i << ": " << interpreter->tensor(i)->name << ", "
                  << interpreter->tensor(i)->bytes << ", "
                  << interpreter->tensor(i)->type << ", "
                  << interpreter->tensor(i)->params.scale << ", "
                  << interpreter->tensor(i)->params.zero_point << "\r\n";
      }
    }

    LOG(INFO) << "\r\n";
  }
}

/*!
 * @brief Runs inference input buffer and print result to console
 *
 * @param pointer to kws mfcc class
 * @param reference to flat buffer model
 * @param pointer to labels string array
 * @param reference to interpreter
 * @param pointer to input tensor
 */
void RunInference(KWS_MFCC *kws, int16_t* buf, const std::string *labels,
                  std::unique_ptr<tflite::FlatBufferModel> &model,
                  std::unique_ptr<tflite::Interpreter> &interpreter,
                  TfLiteTensor* input_tensor)
{
  kws->audio_buffer = buf;
  kws->extract_features();
  float* in = (float*)kws->mfcc_buffer;

  /* Get input dimension from the input tensor metadata
     assuming one input only */
  int input_bytes = input_tensor->bytes;

  /* Experimental min, max range for quantization of the input.*/
  float min= -247.0;
  float max = 30.0;
  for (int i = 0; i < input_bytes; i++)
  {
      input_tensor->data.uint8[i] = (uint8_t)round((255 * 1.0 * (float)(in[i] - min)) / (max - min));
  }

  auto start = GetTimeInUS();
  if (interpreter->Invoke() != kTfLiteOk)
  {
    LOG(FATAL) << "Failed to invoke tflite!\r\n";
    return;
  }
  auto end = GetTimeInUS();

  const float threshold = (float)DETECTION_TRESHOLD /100;

  std::vector<std::pair<float, int>> top_results;

  int output = interpreter->outputs()[0];
  TfLiteTensor* output_tensor = interpreter->tensor(output);
  TfLiteIntArray* output_dims = output_tensor->dims;

  /* Assume output dims to be something like (1, 1, ... , size) */
  auto output_size = output_dims->data[output_dims->size - 1];

  GetTopN<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0),
                     output_size, 1, threshold,
                     &top_results, false);

  if (!top_results.empty())
  {
    auto result = top_results.front();
    const float confidence = result.first;
    const int index = result.second;
    if (confidence * 100 > DETECTION_TRESHOLD)
    {
      LOG(INFO) << "----------------------------------------\r\n";
      LOG(INFO) << "     Inference time:   " << (end - start) / 1000 << " ms\r\n";
      LOG(INFO) << "     Detected: " << std::setw(10) << labels[index] << " (" << (int)(confidence * 100) << "%)\r\n";
      LOG(INFO) << "----------------------------------------\r\n\r\n";
    }
  }
}

/*!
 * @brief Initializes device and run KWS application
 */
int main(void)
{
  /* (recording_win x frame_shift) is the actual recording window size. */	
  int recording_win = 49;
  KWS_MFCC kws_mfcc(recording_win);
  
  const std::string labels[] = {"Silence", "Unknown","yes", "no",
    "up", "down", "left", "right", "on", "off", "stop", "go"};

  audio_init();

  sai_transceiver_t config;

  /* Init board hardware */
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  CLOCK_InitAudioPll(&audioPllConfig);
  BOARD_InitDebugConsole();

  BOARD_InitDebugConsole();

  InitTimer();

  std::unique_ptr<tflite::FlatBufferModel> model;
  std::unique_ptr<tflite::Interpreter> interpreter;
  TfLiteTensor* input_tensor = 0;
  InferenceInit(model, interpreter, &input_tensor, false);

  LOG(INFO) << "Keyword spotting example using a TensorFlow Lite model.\r\n" << std::endl;
  LOG(INFO) << "Detection threshold: " << DETECTION_TRESHOLD << "%\r\n";

  LOG(INFO) << "\r\nStatic data processing:\r\n" << std::endl;

  RunInference(&kws_mfcc, (int16_t*)OFF, labels, model, interpreter, input_tensor);
  RunInference(&kws_mfcc, (int16_t*)RIGHT, labels, model, interpreter, input_tensor);

  /* Clock setting for LPI2C */
  CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

  /* Clock setting for SAI1 */
  CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
  CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

  /* Enable MCLK clock */
  BOARD_EnableSaiMclkOutput(true);

  /* SAI init */
  SAI_Init(DEMO_SAI);
  /* Clear TCSR interrupt flags. */
  DEMO_SAI->TCSR &= ~(I2S_TCSR_FRIE_MASK | I2S_TCSR_FEIE_MASK);
  /* Clear RCSR interrupt flags. */
  DEMO_SAI->RCSR &= ~(I2S_RCSR_FRIE_MASK | I2S_RCSR_FEIE_MASK);
  SAI_TransferTxCreateHandle(DEMO_SAI, &txHandle, tx_callback, NULL);
  SAI_TransferRxCreateHandle(DEMO_SAI, &rxHandle, rx_callback, NULL);

  /* I2S mode configurations */
  SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
  SAI_TransferTxSetConfig(DEMO_SAI, &txHandle, &config);
  config.syncMode = kSAI_ModeSync;
  SAI_TransferRxSetConfig(DEMO_SAI, &rxHandle, &config);

  /* set bit clock divider */
  SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                        DEMO_AUDIO_DATA_CHANNEL);
  SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                        DEMO_AUDIO_DATA_CHANNEL);

  /* master clock configurations */
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
  (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
  mclkConfig.mclkHz          = DEMO_AUDIO_MASTER_CLOCK;
  mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;
#endif
  SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
#endif

  /* Use default setting to init codec */
  CODEC_Init(codecHandle, &boardCodecConfig);

  LOG(INFO) << "\r\nMicrophone data processing:\r\n" << std::endl;

  RecordPlayback(DEMO_SAI);

  while(1)
  {
    while (!dataReady) {}
    dataReady = false;
    uint8_t* pBuf = audioBuff + indexStart * BUFFER_SIZE;
    RunInference(&kws_mfcc, reinterpret_cast<int16_t*>(pBuf), labels, model, interpreter, input_tensor);
  }
}
