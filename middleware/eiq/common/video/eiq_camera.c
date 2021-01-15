/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>

#include "eiq_camera.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"


static EIQ_Camera_t camera;
static uint32_t cameraReceivedFrameAddr;
static EIQ_IBufferAddrUpdater_t readyCallback = NULL;

#if !defined(__ARMCC_VERSION)
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_buffer[DEMO_CAMERA_BUFFER_COUNT][DEMO_CAMERA_HEIGHT][DEMO_CAMERA_WIDTH*DEMO_CAMERA_BUFFER_BPP],
    DEMO_CAMERA_BUFFER_ALIGN);
#else
AT_NONCACHEABLE_SECTION_ALIGN_INIT(
    static uint8_t s_buffer[DEMO_CAMERA_BUFFER_COUNT][DEMO_CAMERA_HEIGHT][DEMO_CAMERA_WIDTH*DEMO_CAMERA_BUFFER_BPP],
    DEMO_CAMERA_BUFFER_ALIGN);
#endif


static void start(void){
  CAMERA_RECEIVER_Start(&cameraReceiver);

  /* Wait to get the full frame buffer to show. */
  while (kStatus_Success !=
    CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &cameraReceivedFrameAddr)) {}

}

static Dims_t getResolution(){
  Dims_t dims;
  dims.width = DEMO_CAMERA_WIDTH;
  dims.height = DEMO_CAMERA_HEIGHT;
  return dims;
}

static void notify(void){
  CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, cameraReceivedFrameAddr);
  //start();
  CAMERA_RECEIVER_Start(&cameraReceiver);
}

static void setReadyCallback(EIQ_IBufferAddrUpdater_t updater){
  readyCallback = updater;
}

static void bufferReady(camera_receiver_handle_t *handle,
                        status_t status, void *userData)
{
  if(readyCallback != NULL &&
     CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver,
                                   &cameraReceivedFrameAddr) == kStatus_Success)
  {
    readyCallback(cameraReceivedFrameAddr);
  }
}

/*!
 * @brief Initializes camera.
 */
static void init(void)
{
  camera_config_t cameraConfig;
  memset(&cameraConfig, 0, sizeof(cameraConfig));
  memset(s_buffer, 0, sizeof(s_buffer));

  BOARD_InitCameraResource();

  cameraConfig.pixelFormat                = DEMO_CAMERA_PIXEL_FORMAT;
  cameraConfig.bytesPerPixel              = DEMO_CAMERA_BUFFER_BPP;
  cameraConfig.resolution =
      FSL_VIDEO_RESOLUTION(DEMO_CAMERA_WIDTH, DEMO_CAMERA_HEIGHT);
    /* Set the camera buffer stride according to panel, so that if
     * camera resoution is smaller than display, it can still be shown
     * correct in the screen.
     */
  cameraConfig.frameBufferLinePitch_Bytes = DEMO_CAMERA_WIDTH * DEMO_CAMERA_BUFFER_BPP,
  cameraConfig.interface                  = kCAMERA_InterfaceGatedClock,
  cameraConfig.controlFlags               = DEMO_CAMERA_CONTROL_FLAGS,
  cameraConfig.framePerSec                = DEMO_CAMERA_FRAME_RATE,

  CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, bufferReady, NULL);

#if ( DEMO_CAMERA == DEMO_CAMERA_RM68191 || DEMO_CAMERA == DEMO_CAMERA_RM68200 )

  BOARD_InitMipiCsi();

  cameraConfig.pixelFormat   = kVIDEO_PixelFormatYUYV;
  cameraConfig.bytesPerPixel = 2;
  cameraConfig.resolution    = FSL_VIDEO_RESOLUTION(DEMO_CAMERA_WIDTH, DEMO_CAMERA_HEIGHT);
  cameraConfig.interface     = kCAMERA_InterfaceMIPI;
  cameraConfig.controlFlags  = DEMO_CAMERA_CONTROL_FLAGS;
  cameraConfig.framePerSec   = DEMO_CAMERA_FRAME_RATE;
  cameraConfig.csiLanes      = DEMO_CAMERA_MIPI_CSI_LANE;
  CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

#else

  if (kStatus_Success != CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig))
  {
    printf("Camera device initialization failed\r\n");
    for (;;) {}
  }


#endif

  CAMERA_DEVICE_Start(&cameraDevice);

  /* Submit the empty frame buffers to buffer queue. */
  for (uint32_t i = 0; i < DEMO_CAMERA_BUFFER_COUNT; i++)
  {
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_buffer[i]));
  }

}

EIQ_Camera_t* EIQ_CameraInit(){
  camera.base.getResolution = getResolution;
  camera.base.notify = notify;
  camera.base.start = start;
  camera.setReadyCallback = setReadyCallback;

  // Board CSI pin inicialization.
  BOARD_EarlyPrepareCamera();
  BOARD_InitCSIPins();
  init();

  return &camera;
}
