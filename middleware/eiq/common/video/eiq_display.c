/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>

#include "eiq_display.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

#include "eiq_pxp.h"

static EIQ_Display_t display;
static volatile uint8_t flags = 0;
static volatile uint8_t s_bufferIdx = 1;
static volatile bool s_newFrameShown = false;
static dc_fb_info_t s_fbInfo;
static EIQ_IUpdater_t readyCallback;

#if !defined(__ARMCC_VERSION)
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_buffer[DEMO_LCD_BUFFER_COUNT][DEMO_PANEL_HEIGHT]
                             [DEMO_PANEL_WIDTH * DEMO_LCD_BUFFER_BPP],
                             DEMO_FRAME_BUFFER_ALIGN);
#else
AT_NONCACHEABLE_SECTION_ALIGN_INIT(
    static uint8_t s_buffer[DEMO_LCD_BUFFER_COUNT][DEMO_PANEL_HEIGHT]
                                  [DEMO_PANEL_WIDTH * DEMO_LCD_BUFFER_BPP],
                                  DEMO_FRAME_BUFFER_ALIGN);
#endif

static void start(void)
{
  g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_buffer[s_bufferIdx]);

  /* For the DBI interface display, application must wait for the first
     frame buffer sent to the panel. */
  if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
  {
    while (s_newFrameShown == false) {}
  }

  g_dc.ops->enableLayer(&g_dc, 0);
  s_newFrameShown = true;
}

static Dims_t getResolution()
{
  Dims_t dims;
  dims.width = DEMO_PANEL_WIDTH;
  dims.height = DEMO_PANEL_HEIGHT;
  return dims;
}

static void notify(void)
{
  while(!s_newFrameShown){;}
  s_newFrameShown = false;
  g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_buffer[s_bufferIdx]);
  s_bufferIdx ^= 1U;
}

uint32_t getEmptyBuffer()
{
  return (uint32_t)s_buffer[s_bufferIdx];
}

static void setReadyCallback(EIQ_IUpdater_t updater){
  readyCallback = updater;
}

static void bufferReleased(void *param, void *switchOffBuffer)
{
  s_newFrameShown = true;
  if(readyCallback != NULL){
    readyCallback();
  }
}

/*!
 * @brief Initializes display.
 */
static void init(){
  status_t status;

  BOARD_PrepareDisplayController();

  memset(s_buffer, 0, sizeof(s_buffer));

  status = g_dc.ops->init(&g_dc);
  if (kStatus_Success != status)
  {
    printf("Display initialization failed\r\n");
    assert(0);
  }

  g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &s_fbInfo);
  s_fbInfo.pixelFormat = kVIDEO_PixelFormatRGB565;
  s_fbInfo.width       = DEMO_PANEL_WIDTH;
  s_fbInfo.height      = DEMO_PANEL_HEIGHT;
  s_fbInfo.strideBytes = DEMO_PANEL_WIDTH * DEMO_LCD_BUFFER_BPP;
  g_dc.ops->setLayerConfig(&g_dc, 0, &s_fbInfo);

  g_dc.ops->setCallback(&g_dc, 0, bufferReleased, NULL);

}

EIQ_Display_t* EIQ_DisplayInit(){
  display.base.getResolution = getResolution;
  display.base.notify = notify;
  display.base.start = start;
  display.getEmptyBuffer = getEmptyBuffer;
  display.setReadyCallback = setReadyCallback;

  BOARD_InitLCDPins();
  init();

  return &display;
}
