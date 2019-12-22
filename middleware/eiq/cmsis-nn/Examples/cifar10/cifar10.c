/* ----------------------------------------------------------------------
* Copyright (C) 2010-2018 Arm Limited. All rights reserved.
* Copyright 2018-2019 NXP. All rights reserved.
*
*
* Project:       CMSIS NN Library
* Title:         arm_nnexamples_cifar10.cpp
*
* Description:   Convolutional Neural Network Example
*
* Target Processor: Cortex-M4/Cortex-M7
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of Arm LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------- */

/* File modified by NXP. Changes are described in file
   /middleware/eiq/cmsis-nn/readme.txt in section "Release notes" */

#include "display_support.h"
#include "camera_support.h"
#include "fsl_pxp.h"

#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"

#include "timer.h"
#include "image.h"
#include "ship.h"

#include "parameter.h"
#include "weights.h"
#include "arm_nnfunctions.h"

#include <cmsis_compiler.h>

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
#define DETECTION_TRESHOLD 60

#define INPUT_MEAN_SHIFT {125,123,114}
#define INPUT_RIGHT_SHIFT {8,8,8}

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

/* conv1_wt, conv2_wt, conv3_wt are convolution layer weight matrices */
/* conv1_bias, conv2_bias, conv3_bias are convolution layer bias arrays */
static const q7_t __ALIGNED(4) conv1_wt[CONV1_IN_CH * CONV1_KER_DIM * CONV1_KER_DIM * CONV1_OUT_CH] = CONV1_WT;
static const q7_t __ALIGNED(4) conv1_bias[CONV1_OUT_CH] = CONV1_BIAS;

static const q7_t __ALIGNED(4) conv2_wt[CONV2_IN_CH * CONV2_KER_DIM * CONV2_KER_DIM * CONV2_OUT_CH] = CONV2_WT;
static const q7_t __ALIGNED(4) conv2_bias[CONV2_OUT_CH] = CONV2_BIAS;

static const q7_t __ALIGNED(4) conv3_wt[CONV3_IN_CH * CONV3_KER_DIM * CONV3_KER_DIM * CONV3_OUT_CH] = CONV3_WT;
static const q7_t __ALIGNED(4) conv3_bias[CONV3_OUT_CH] = CONV3_BIAS;

/* ip1_wt, ip1_bias point to fully-connected layer weights and biases */
static const q7_t __ALIGNED(4) ip1_wt[IP1_IN_DIM * IP1_OUT_DIM] = IP1_WT;
static const q7_t __ALIGNED(4) ip1_bias[IP1_OUT_DIM] = IP1_BIAS;

/* output_data points to the classification output */
static q7_t __ALIGNED(4) output_data[IP1_OUT_DIM];

/* col_buffer is a buffer to store the im2col output */
static q7_t __ALIGNED(4) col_buffer[6400];

/* scratch_buffer is used to store the activation data (intermediate layer outputs) */
static q7_t __ALIGNED(4) scratch_buffer[40960];

/* image_data - raw uint8 type RGB image in [R, G, B, R, G, B, R, G, B, ... R, G, B] format */
void run_nn(const uint8_t *image_data)
{
  q7_t *buffer1 = scratch_buffer;
  q7_t *buffer2 = buffer1 + 32 * 32 * 32;

  int mean_data[3] = INPUT_MEAN_SHIFT;
  unsigned int scale_data[3] = INPUT_RIGHT_SHIFT;

  /* Input pre-processing */
  for (int i = 0; i < 32 * 32 * 3; i += 3)
  {
    buffer2[i]   = (q7_t)__SSAT(((((int)image_data[i] - mean_data[0]) << 7)
      + (0x1 << (scale_data[0] - 1))) >> scale_data[0], 8);
    buffer2[i + 1] = (q7_t)__SSAT(((((int)image_data[i + 1] - mean_data[1]) << 7)
      + (0x1 << (scale_data[1] - 1))) >> scale_data[1], 8);
    buffer2[i+2] = (q7_t)__SSAT(((((int)image_data[i + 2] - mean_data[2]) << 7)
      + (0x1 << (scale_data[2] - 1))) >> scale_data[2], 8);
  }

  /* conv1 buffer2 -> buffer1 */
  arm_convolve_HWC_q7_RGB(buffer2, CONV1_IN_DIM, CONV1_IN_CH, conv1_wt,
    CONV1_OUT_CH, CONV1_KER_DIM, CONV1_PAD, CONV1_STRIDE, conv1_bias,
    CONV1_BIAS_LSHIFT, CONV1_OUT_RSHIFT, buffer1, CONV1_OUT_DIM,
    (q15_t *) col_buffer, NULL);

  arm_relu_q7(buffer1, CONV1_OUT_DIM * CONV1_OUT_DIM * CONV1_OUT_CH);

  /* pool1 buffer1 -> buffer2 */
  arm_maxpool_q7_HWC(buffer1, CONV1_OUT_DIM, CONV1_OUT_CH, POOL1_KER_DIM,
                     POOL1_PAD, POOL1_STRIDE, POOL1_OUT_DIM, NULL, buffer2);

  /* conv2 buffer2 -> buffer1 */
  arm_convolve_HWC_q7_fast(buffer2, CONV2_IN_DIM, CONV2_IN_CH, conv2_wt,
    CONV2_OUT_CH, CONV2_KER_DIM, CONV2_PAD, CONV2_STRIDE, conv2_bias,
    CONV2_BIAS_LSHIFT, CONV2_OUT_RSHIFT, buffer1, CONV2_OUT_DIM,
    (q15_t *) col_buffer, NULL);

  arm_relu_q7(buffer1, CONV2_OUT_DIM * CONV2_OUT_DIM * CONV2_OUT_CH);

  /* pool2 buffer1 -> buffer2 */
  arm_maxpool_q7_HWC(buffer1, CONV2_OUT_DIM, CONV2_OUT_CH, POOL2_KER_DIM,
                     POOL2_PAD, POOL2_STRIDE, POOL2_OUT_DIM, col_buffer, buffer2);

  /* conv3 buffer2 -> buffer1 */
  arm_convolve_HWC_q7_fast(buffer2, CONV3_IN_DIM, CONV3_IN_CH, conv3_wt,
    CONV3_OUT_CH, CONV3_KER_DIM, CONV3_PAD, CONV3_STRIDE, conv3_bias,
    CONV3_BIAS_LSHIFT, CONV3_OUT_RSHIFT, buffer1, CONV3_OUT_DIM,
    (q15_t *) col_buffer, NULL);

  arm_relu_q7(buffer1, CONV3_OUT_DIM * CONV3_OUT_DIM * CONV3_OUT_CH);

  /* pool3 buffer-> buffer2 */
  arm_maxpool_q7_HWC(buffer1, CONV3_OUT_DIM, CONV3_OUT_CH, POOL3_KER_DIM,
                     POOL3_PAD, POOL3_STRIDE, POOL3_OUT_DIM, col_buffer, buffer2);

  arm_fully_connected_q7_opt(buffer2, ip1_wt, IP1_IN_DIM, IP1_OUT_DIM,
    IP1_BIAS_LSHIFT, IP1_OUT_RSHIFT, ip1_bias, output_data, (q15_t *) buffer1);

  arm_softmax_q7(output_data, 10, output_data);
}

/*!
 * @brief  Run inference. It processed static image if static image is not NULL 
 * otherwise camera input is processed.
 * 
 * param staticImage pointer to address of static image. Use NULL when static image
 * is not required.
 * param staticImageLen size of static image.
 */
void run_inference(const uint8_t *image_data, const char* labels[])
{
  uint32_t start = 0U;
  uint32_t end = 0U;
  q7_t max_value;
  uint32_t max_index;

  start = GetTimeInUS();
  run_nn(image_data);
  end = GetTimeInUS();

  /* Find out the true item with the biggest value */
  arm_max_q7(output_data, 10, &max_value, &max_index);
  int score = (max_value * 100) / 128;

  if (score > DETECTION_TRESHOLD)
  {
    PRINTF("----------------------------------------\r\n");
    PRINTF("     Inference time : %d ms    \r\n", (end - start) / 1000);
    PRINTF("     Detected: %.10s (%d%%)\r\n", labels[max_index], (int)score);
    PRINTF("----------------------------------------\r\n\r\n");
  }
}

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
    while (1) {}
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
 * @brief Main function
 */
int main(void)
{
  const char* labels[10] = {
    "airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", 
    "ship", "truck"
  };

  /* Init board hardware. */
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

  PRINTF("CIFAR-10 object recognition example using CMSIS-NN.\r\n");
  PRINTF("Detection threshold: %d%%.\r\n", DETECTION_TRESHOLD);

  /* Run inference with static ship image. */
  run_inference(ship_image, labels);

  APP_InitCamera();
  APP_InitDisplay();
  APP_InitPxp();
  /* Start CSI transfer */
  APP_CsiRgb565Start();

  pExtract = (uint16_t*)malloc(Rec_w * Rec_h * sizeof(uint16_t));

  uint8_t* data = (uint8_t*)malloc(Rec_w * Rec_h * 3 * sizeof(uint8_t));
  memset(data, 0 , Rec_w * Rec_h * 3);

  Image prev_scale = {
    .width = Rec_w,
    .height = Rec_h,
    .channels = 3,
  };

  Image *after_scale;

  uint8_t wanted_width = 32;
  uint8_t wanted_height = 32;
  double dx = 1.0 * wanted_width / Rec_w;
  double dy = 1.0 * wanted_height / Rec_h;

  after_scale = ImCreate(&prev_scale, dx, dy);

  PRINTF("\r\nCamera data processing:\r\n");

  while (1)
  {
    if (g_isCamDataExtracted)
    {
       CSI2Image(data, Rec_w, Rec_h, pExtract, false);

       /* Resize image to 32x32 */
       prev_scale.imageData = data;
       after_scale = ImScale(&prev_scale, after_scale, dx, dy);

       run_inference(after_scale->imageData, labels);

       g_isCamDataExtracted = false;
    }
  }
}
