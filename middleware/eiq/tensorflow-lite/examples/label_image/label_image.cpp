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

#include "display_support.h"
#include "camera_support.h"
#include "fsl_pxp.h"

#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"

#include "timer.h"
#include "image.h"

#include "stopwatch_image.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/string_util.h"

#include "mobilenet_v1_0.25_128_quant_model.h"

#include "get_top_n.h"
#include "bitmap_helpers.h"
#include "labels.h"
#include "label_image.h"

#define LOG(x) std::cout

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_FRAME_BUFFER_COUNT 4
/* Pixel format RGB565, bytesPerPixel is 2. */
#define APP_BPP 2

#if (FRAME_BUFFER_ALIGN > DEMO_CAMERA_BUFFER_ALIGN)
#define DEMO_FRAME_BUFFER_ALIGN FRAME_BUFFER_ALIGN
#else
#define CAMERA_FRAME_BUFFER_ALIGN DEMO_CAMERA_BUFFER_ALIGN
#endif

/* PXP */
#define ROTATE_DISPLAY kPXP_Rotate180
#define APP_PXP PXP

#define APP_LCD_BUFFER_COUNT 2

#define APP_IMG_WIDTH DEMO_PANEL_WIDTH
#define APP_IMG_HEIGHT DEMO_PANEL_HEIGHT

/* PS input buffer is square. */
#if APP_IMG_WIDTH > APP_IMG_HEIGHT
#define APP_PS_SIZE APP_IMG_WIDTH
#else
#define APP_PS_SIZE APP_IMG_HEIGHT
#endif

#define APP_PS_ULC_X 0U
#define APP_PS_ULC_Y 0U
#define APP_PS_LRC_X (APP_IMG_WIDTH -1U)
#define APP_PS_LRC_Y (APP_IMG_HEIGHT- 1U)

#define APP_RED 0xF100U
#define APP_GREEN 0x07E0U
#define APP_BLUE 0x001FU
#define APP_WHITE 0xFFFFU
#define APP_PXP_PS_FORMAT kPXP_PsPixelFormatRGB565
#define APP_PXP_AS_FORMAT kPXP_AsPixelFormatRGB565
#define APP_PXP_OUT_FORMAT kPXP_OutputPixelFormatRGB565
#define APP_DC_FORMAT kVIDEO_PixelFormatRGB565

/* Tresholds */
#define DETECTION_TRESHOLD 25

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_BufferSwitchOffCallback(void *param, void *switchOffBuffer);
static void APP_CSIFullBufferReady(camera_receiver_handle_t *handle,
                                   status_t status, void *userData);
static void APP_Rotate(uint32_t input_buffer, uint32_t output_buffer);
static void APP_InitPxp(void);
static void APP_InitCamera(void);
static void APP_InitDisplay(void);
static void APP_CsiRgb565Start(void);
static void APP_CsiRgb565Refresh(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if !defined(__ARMCC_VERSION)
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint16_t s_frameBuffer[APP_FRAME_BUFFER_COUNT][DEMO_PANEL_HEIGHT][DEMO_PANEL_WIDTH],
    CAMERA_FRAME_BUFFER_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN(
    static uint16_t s_lcdBuf[APP_LCD_BUFFER_COUNT][DEMO_PANEL_HEIGHT][DEMO_PANEL_WIDTH],
    CAMERA_FRAME_BUFFER_ALIGN);

#else
AT_NONCACHEABLE_SECTION_ALIGN_INIT(
    static uint16_t s_frameBuffer[APP_FRAME_BUFFER_COUNT][DEMO_PANEL_HEIGHT][DEMO_PANEL_WIDTH],
    CAMERA_FRAME_BUFFER_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN_INIT(
    static uint16_t s_lcdBuf[APP_LCD_BUFFER_COUNT][DEMO_PANEL_HEIGHT][DEMO_PANEL_WIDTH],
    CAMERA_FRAME_BUFFER_ALIGN);
#endif

/*
 * When new frame buffer sent to display, it might not be shown immediately.
 * Application could use callback to get new frame shown notification, at the
 * same time, when this flag is set, application could write to the older
 * frame buffer.
 */
static volatile bool s_newFrameShown = false;
static dc_fb_info_t fbInfo;

int Rec_w = 156;
int Rec_h = 156;
int Rec_x = 176;
int Rec_y = 72;

static volatile bool g_isCamDataExtracted = false;
static uint16_t *pExtract = NULL;

static uint32_t cameraReceivedFrameAddr;
static uint8_t curLcdBufferIdx = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Rotate image PXP.
 * param input_buffer pointer to source image buffer.
 * param output_buffer pointer to output buffer for storing result.
 */
static void APP_Rotate(uint32_t input_buffer, uint32_t output_buffer)
{
  APP_PXP->PS_BUF = input_buffer;
  APP_PXP->OUT_BUF = output_buffer;
  /* Prepare next buffer for LCD. */
  PXP_SetRotateConfig(APP_PXP, kPXP_RotateOutputBuffer, ROTATE_DISPLAY, kPXP_FlipDisable);

  PXP_Start(APP_PXP);

  /* Wait for process complete. */
  while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(APP_PXP)))
  {
  }

  PXP_ClearStatusFlags(APP_PXP, kPXP_CompleteFlag);
}

/*!
 * @brief Initializes PXP.
 */
static void APP_InitPxp(void)
{
  PXP_Init(APP_PXP);

  /* PS configure. */
  const pxp_ps_buffer_config_t psBufferConfig = {
    .pixelFormat = APP_PXP_PS_FORMAT,
    .swapByte    = false,
    .bufferAddr  = 0U,
    .bufferAddrU = 0U,
    .bufferAddrV = 0U,
    .pitchBytes  = APP_PS_SIZE * APP_BPP,
  };

  PXP_SetProcessSurfaceBackGroundColor(APP_PXP, 0U);

  PXP_SetProcessSurfaceBufferConfig(APP_PXP, &psBufferConfig);
  PXP_SetProcessSurfacePosition(APP_PXP, APP_PS_ULC_X, APP_PS_ULC_Y, APP_PS_LRC_X, APP_PS_LRC_Y);

  /* Disable AS. */
  PXP_SetAlphaSurfacePosition(APP_PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

  pxp_output_buffer_config_t outputBufferConfig;
  /* Output config. */
  outputBufferConfig.pixelFormat    = APP_PXP_OUT_FORMAT;
  outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
  outputBufferConfig.buffer0Addr    = 0U;
  outputBufferConfig.buffer1Addr    = 0U;
  outputBufferConfig.pitchBytes     = APP_IMG_WIDTH * APP_BPP;
  outputBufferConfig.width          = APP_IMG_WIDTH;
  outputBufferConfig.height         = APP_IMG_HEIGHT;

  PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

  /* Disable CSC1, it is enabled by default. */
  PXP_EnableCsc1(APP_PXP, false);
}

/*!
 * @brief Initializes camera.
 */
static void APP_InitCamera(void)
{
  const camera_config_t cameraConfig = {
    .pixelFormat   = kVIDEO_PixelFormatRGB565,
    .bytesPerPixel = APP_BPP,
    .resolution    = FSL_VIDEO_RESOLUTION(DEMO_CAMERA_WIDTH, DEMO_CAMERA_HEIGHT),
    /* Set the camera buffer stride according to panel, so that if
     * camera resoution is smaller than display, it can still be shown
     * correct in the screen.
     */
    .frameBufferLinePitch_Bytes = DEMO_PANEL_WIDTH * APP_BPP,
    .interface                  = kCAMERA_InterfaceGatedClock,
    .controlFlags               = DEMO_CAMERA_CONTROL_FLAGS,
    .framePerSec                = 30,
  };

  memset(s_frameBuffer, 0, sizeof(s_frameBuffer));

  BOARD_InitCameraResource();

  CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, APP_CSIFullBufferReady, NULL);

  if (kStatus_Success != CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig))
  {
    PRINTF("Camera device initialization failed\r\n");
    for (;;) {}
  }

  CAMERA_DEVICE_Start(&cameraDevice);

  /* Submit the empty frame buffers to buffer queue. */
  for (uint32_t i = 0; i < APP_FRAME_BUFFER_COUNT; i++)
  {
     CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_frameBuffer[i]));
  }
}

/*!
 * @brief Initializes LCD.
 */
static void APP_InitDisplay(void)
{
  status_t status;
  
  BOARD_PrepareDisplayController();

  status = g_dc.ops->init(&g_dc);
  if (kStatus_Success != status)
  {
    PRINTF("Display initialization failed\r\n");
    assert(0);
  }

  g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
  fbInfo.pixelFormat = kVIDEO_PixelFormatRGB565;
  fbInfo.width       = DEMO_PANEL_WIDTH;
  fbInfo.height      = DEMO_PANEL_HEIGHT;
  fbInfo.strideBytes = DEMO_PANEL_WIDTH * APP_BPP;
  g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

  g_dc.ops->setCallback(&g_dc, 0, APP_BufferSwitchOffCallback, NULL);
}

/*!
 * @brief Start CSI processing.
 */
static void APP_CsiRgb565Start(void)
{
  CAMERA_RECEIVER_Start(&cameraReceiver);

  /* Wait to get the full frame buffer to show. */
  while (kStatus_Success !=
    CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &cameraReceivedFrameAddr)) {}

  APP_Rotate(cameraReceivedFrameAddr, (uint32_t)s_lcdBuf[curLcdBufferIdx]);

  s_newFrameShown = false;
  g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_lcdBuf[curLcdBufferIdx]);

  /* For the DBI interface display, application must wait for the first
     frame buffer sent to the panel. */
  if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
  {
    while (s_newFrameShown == false) {}
  }

  s_newFrameShown = true;

  g_dc.ops->enableLayer(&g_dc, 0);
}

/*!
 * @brief Process camera buffer and send it to LCD.
  */
static void APP_CsiRgb565Refresh()
{
  /* Wait to get the full frame buffer to show. */
  while (kStatus_Success !=
    CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &cameraReceivedFrameAddr)) {}

  curLcdBufferIdx ^= 1U;
  APP_Rotate(cameraReceivedFrameAddr, (uint32_t)s_lcdBuf[curLcdBufferIdx]);

  /* Check if camera buffer is extracted for new inference. */
  if (!g_isCamDataExtracted)
  {
    /* Extract image from camera. */
    ExtractImage(pExtract, Rec_x, Rec_y, Rec_w, Rec_h, (uint16_t *)s_lcdBuf[curLcdBufferIdx]);
    /* Draw red rectangle. */
    DrawRect((uint16_t*)s_lcdBuf[curLcdBufferIdx],Rec_x, Rec_y, Rec_w, Rec_h, 255, 0, 0);
    g_isCamDataExtracted= true;
  }
  else
  {
    /* Draw white rectangle. */
    DrawRect((uint16_t*)s_lcdBuf[curLcdBufferIdx], Rec_x, Rec_y, Rec_w, Rec_h, 255, 255, 255);
  }

  s_newFrameShown = false;
  g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_lcdBuf[curLcdBufferIdx]);
}

/*!
 * @brief Set new empty buffer for CSI.
 */
static void APP_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
  s_newFrameShown = true;
  CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)cameraReceivedFrameAddr);
}

static void APP_CSIFullBufferReady(camera_receiver_handle_t *handle,
                                   status_t status, void *userData)
{
  if (s_newFrameShown)
  {
    APP_CsiRgb565Refresh();
  }
}

/*!
 * @brief Resize image. Input dimension from the input tensor metadata
 * assuming one input only, dimensions could be read from
 * interpreter->tensor(input)->dims. Where size of the picture
 * is 128*128*3. This size is the same as input size of the model.
 * Resize is done by scaling image. Input size of the model
 * could be check using:
 * TfLiteIntArray* dims = interpreter->tensor(input)->dims;
 *   int height = dims->data[1];
 *   int width = dims->data[2];
 *   int channels = dims->data[3];
 *
 * @param reference to interpreter
 * @param Pointer to input data buffer.
 * @param Original height size of image.
 * @param Original width size of image.
 * @param Wanted height size of image.
 * @param Wanted width size of image.
 * @param Image channels number.
 * @param Configuration structure, contains as std, mean, verbose option
 */
void ResizeImage(TfLiteTensor* tensor, uint8_t* in, const int image_height,
    const int image_width, const int wanted_height, const int wanted_width,
    const int image_channels, tflite::label_image::Settings* s)
{
  switch (tensor->type)
  {
    case kTfLiteFloat32:
      s->input_floating = true;
      tflite::label_image::Resize<float>(reinterpret_cast<float*>(
          tensor->data.raw), in, image_height, image_width, image_channels,
          wanted_height, wanted_width, image_channels, s);
      break;
    case kTfLiteUInt8:
      tflite::label_image::Resize<uint8_t>(
          reinterpret_cast<uint8_t*>(tensor->data.raw), in,
          image_height, image_width, image_channels, wanted_height,
          wanted_width, image_channels, s);
    break;
    default:
      LOG(FATAL) << "cannot handle input type " << tensor->type << " yet";
  }
}

/*!
 * @brief Initialize parameters for inference
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
  model = tflite::FlatBufferModel::BuildFromBuffer(mobilenet_model, mobilenet_model_len);
  if (!model)
  {
    LOG(FATAL) << "Failed to load model.\r\n";
    return;
  }

  tflite::ops::builtin::BuiltinOpResolver resolver;

  tflite::InterpreterBuilder(*model, resolver)(&interpreter);
  if (!interpreter)
  {
    LOG(FATAL) << "Failed to construct interpreter.\r\n";
    return;
  }

  int input = interpreter->inputs()[0];

  const std::vector<int> inputs = interpreter->inputs();
  const std::vector<int> outputs = interpreter->outputs();

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
 * @param reference to interpreter
 * @param pointer to labels string array
 * @param reference to flat buffer model
 * @param pointer to input tensor
 */
void RunInference(std::unique_ptr<tflite::Interpreter> &interpreter,
                  const char* labels[],
                  std::unique_ptr<tflite::FlatBufferModel> &model,
                  TfLiteTensor* input_tensor,
                  tflite::label_image::Settings* s)
{
  auto start = GetTimeInUS();

  if (interpreter->Invoke() != kTfLiteOk)
  {
    LOG(FATAL) << "Failed to invoke tflite!\r\n" << std::endl;
  }

  auto end = GetTimeInUS();
  const float threshold = (float)DETECTION_TRESHOLD / 100;

  std::vector<std::pair<float, int>> top_results;
  int output = interpreter->outputs()[0];
  int input = interpreter->inputs()[0];
  TfLiteTensor* output_tensor = interpreter->tensor(output);
  TfLiteIntArray* output_dims = output_tensor->dims;
  /* Assume output dims to be something like (1, 1, ... , size) */
  int output_size = output_dims->data[output_dims->size - 1];

  /* Find best label candidates. */
  switch (interpreter->tensor(output)->type)
  {
    case kTfLiteFloat32:
      GetTopN<float>(interpreter->typed_output_tensor<float>(0), output_size,
        s->number_of_results, threshold, &top_results, true);
      break;
    case kTfLiteUInt8:
      GetTopN<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0),
        output_size, s->number_of_results, threshold, &top_results, false);
      break;
    default:
      LOG(FATAL) << "cannot handle output type "
      << interpreter->tensor(input)->type << " yet";
      return;
  }

  if (!top_results.empty())
  {
    auto result = top_results.front();
    const float confidence = result.first;
    const int index = result.second;
    if (confidence * 100 > DETECTION_TRESHOLD)
    {
      LOG(INFO) << "----------------------------------------\r\n";
      LOG(INFO) << "     Inference time: " << (end - start) / 1000 << " ms\r\n";
      LOG(INFO) << "     Detected: " << std::setw(10) << labels[index] << " (" << (int)(confidence * 100) << "%)\r\n";
      LOG(INFO) << "----------------------------------------\r\n\r\n";
    }
  }
}

/*!
 * @brief Main function
 */
int main(void)
{
  /* Init board hardware */
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_InitDEBUG_UARTPins();
  BOARD_InitSDRAMPins();
  BOARD_EarlyPrepareCamera();
  BOARD_InitCSIPins();
  BOARD_InitLCDPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  NVIC_SetPriorityGrouping(3);
  InitTimer();

  tflite::label_image::Settings s;

  std::unique_ptr<tflite::FlatBufferModel> model;
  std::unique_ptr<tflite::Interpreter> interpreter;
  TfLiteTensor* input_tensor = 0;
  InferenceInit(model, interpreter, &input_tensor, s.verbose);

  LOG(INFO) << "Label image example using a TensorFlow Lite model.\r\n"<< std::endl;
  LOG(INFO) << "Detection threshold: " << DETECTION_TRESHOLD << "%\r\n" << std::endl;

  /* Create 128x128x3 image layout for camera picture processing. */
  const int image_height = input_tensor->dims->data[1];
  const int image_width = input_tensor->dims->data[2];
  const int image_channels = input_tensor->dims->data[3];

  int width = 0;
  int height = 0;
  int channels = 0;
  uint8_t* data = tflite::label_image::ReadBmp(stopwatch_bmp, stopwatch_bmp_len,
      &width, &height,&channels, &s);

  int input = interpreter->inputs()[0];
  /* Resize image to input tensor for correct inference. */
  ResizeImage(interpreter->tensor(input), data, height, width, image_height,
      image_width, image_channels, &s);

  /* Run inference with static Stopwatch bmp image. */
  LOG(INFO) <<std::endl << "Static data processing:\r\n";
  RunInference(interpreter, labels, model, input_tensor, &s);

  /* Clear buffer. */
  memset(data, 0, width*height*channels);

  pExtract = new uint16_t[Rec_w * Rec_h * sizeof(uint16_t)];

  LOG(INFO) << "Camera data processing:\r\n";

  APP_InitCamera();
  APP_InitDisplay();
  APP_InitPxp();
  /* Start CSI transfer */
  APP_CsiRgb565Start();

  while (1)
  {
    if (g_isCamDataExtracted)
    {
      /* Extract image from camera to data buffer. */
      CSI2Image(data, Rec_w, Rec_h, pExtract, true);
      /* Resize image to input tensor size. */
      ResizeImage(interpreter->tensor(input), data, Rec_h, Rec_w, image_height, image_width, image_channels, &s);
      /* Run inference on camera input. */
      RunInference(interpreter, labels, model, input_tensor, &s);

      g_isCamDataExtracted = false;
    }
  }
}
