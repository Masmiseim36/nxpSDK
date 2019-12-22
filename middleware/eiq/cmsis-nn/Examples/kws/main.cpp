/* ----------------------------------------------------------------------
 * Copyright (C) 2018 Arm Limited or its affiliates. All rights reserved.
 * Copyright 2018 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ----------------------------------------------------------------------*/

/* File modified by NXP. Changes are described in file
   /middleware/eiq/cmsis-nn/readme.txt in section "Release notes" */

/*
 * Description: Example code for running keyword spotting on Cortex-M boards
 */
#include "board.h"

#include "fsl_debug_console.h"
#include "fsl_sai.h"
#include "fsl_codec_common.h"

#include "fsl_wm8960.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_codec_adapter.h"

#include "timer.h"
#include "kws_ds_cnn.h"
#include "commands.h"

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
/* Size of single the buffer. */
#define BUFFER_SIZE (1024U)
/* 16Khz * 2 bytes */
#define AUDIO_NUM (16U * 2)
/* Two buffers are switching for simultaneously reading and writing. */
#define BUFFER_NUMBER (2U * AUDIO_NUM)
#define BUFFER_TOTAL_SIZE (BUFFER_SIZE * BUFFER_NUMBER)
#define STRIDE_SIZE (STRIDE_MS * AUDIO_NUM)
#define DETECTION_TRESHOLD 55

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
sai_transfer_t xferRx = {0};
sai_transfer_t xferTx = {0};

uint8_t codecHandleBuffer[CODEC_HANDLE_SIZE] = {0U};
codec_handle_t *codecHandle = (codec_handle_t *)codecHandleBuffer;
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

sai_handle_t txHandle = {0};
sai_handle_t rxHandle = {0};

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
  wm8960Config.route            = kWM8960_RoutePlaybackandRecord;
  wm8960Config.rightInputSource = kWM8960_InputDifferentialMicInput2;
  wm8960Config.playSource       = kWM8960_PlaySourceDAC;
  wm8960Config.slaveAddress     = WM8960_I2C_ADDR;
  wm8960Config.bus              = kWM8960_BusI2S;
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
  if(enable)
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
    PRINTF("SAI_Rx failed!\r\n");
    return;
  }
  else
  {
    xferRx.data     = audioBuff + indexRx * BUFFER_SIZE;
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
      PRINTF("SAI_Tx failed!\r\n");
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
  sai_transfer_t xfer = {0};
  memset (audioBuff, 0, BUFFER_SIZE * BUFFER_NUMBER);
  SAI_TxSoftwareReset(base, kSAI_ResetTypeSoftware);
  SAI_RxSoftwareReset(base, kSAI_ResetTypeSoftware);

  xfer.dataSize = BUFFER_SIZE;
  xfer.data = audioBuff;

  SAI_TransferSendNonBlocking(base, &txHandle, &xfer);
  SAI_TransferReceiveNonBlocking(base, &rxHandle, &xfer);
}

/*!
 * @brief Runs inference input buffer and print result to console
 *
 * @param pointer to kws
 * @param pointer to input data buffer
 * @param pointer to input data buffer
 * @param output class names array
 */
void run_inference(KWS *kws, int16_t* buf, char output_class[12][8])
{
  int detection_threshold = DETECTION_TRESHOLD; //in percent
  kws->audio_buffer = buf;
  int start = 0;
  int end = 0;
  start = GetTimeInUS();
  kws->extract_features(); //extract mfcc features
  kws->classify(); //classify using dnn
  kws->average_predictions();
  end = GetTimeInUS();
  int max_ind = kws->get_top_class(kws->output);
  if (kws->averaged_output[max_ind] > detection_threshold * 128 / 100)
  {
    PRINTF("----------------------------------------\r\n");
    PRINTF("     Inference time: %d ms\r\n", (end - start) / 1000);
    PRINTF("     Detected: %.10s (%d%%)\r\n", output_class[max_ind], ((int)kws->averaged_output[max_ind] * 100 / 128));
    PRINTF("----------------------------------------\r\n\r\n");
  }
}

/*!
 * @brief Main function
 */
int main(void)
{
  /* Clear audio buffer. */
  memset(audioBuff, 0, BUFFER_TOTAL_SIZE);

  /* (recording_win x frame_shift) is the actual recording window size. */
  int recording_win = 49;
  /* Averaging window for smoothing out the output predictions. */
  int averaging_window_len = 1;

  /* Create new instance for static audio files with averaging window len = 1. */
  KWS_DS_CNN *kws = new KWS_DS_CNN(recording_win, averaging_window_len);

  char output_class[12][8] = {"Silence", "Unknown", "yes", "no", "up", "down", "left", "right", "on", "off", "stop", "go"};

  audio_init();

  sai_transceiver_t config;

  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  CLOCK_InitAudioPll(&audioPllConfig);
  BOARD_InitDebugConsole();

  InitTimer();

  PRINTF("Keyword spotting example using CMSIS-NN.\r\n");
  PRINTF("Detection threshold: %d%%\r\n", DETECTION_TRESHOLD);

  PRINTF("\r\nStatic data processing:\r\n");

  run_inference(kws, (int16_t *)OFF, output_class);
  run_inference(kws, (int16_t *)RIGHT, output_class);

  delete kws;

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
  DEMO_SAI->TCSR &= ~(I2S_TCSR_FRIE_MASK | I2S_TCSR_FEIE_MASK);
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

  PRINTF("\r\nMicrophone data processing:\r\n");

  RecordPlayback(DEMO_SAI);

  /* Averaging window for smoothing out the output predictions. */
  averaging_window_len = 1;
  /* Create new instance for static audio files with averaging window len = 1. */
  kws = new KWS_DS_CNN(recording_win, averaging_window_len);

  while (1)
  {
    while (!dataReady);
    dataReady = false;
    uint8_t *pBuf = audioBuff + indexStart * BUFFER_SIZE;
    run_inference(kws, (int16_t *)pBuf, output_class);
  }
}
