/*
 * Copyright 2019-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl_support.h"
#include "lvgl.h"
#if defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif
#include "board.h"

#include "fsl_gpio.h"
#include "fsl_cache.h"
#include "fsl_debug_console.h"
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
#include "fsl_lpi2c.h"
#include "fsl_ft5406_rt.h"
#else
#include "fsl_gt911.h"
#endif

#if LV_USE_DRAW_VGLITE
#include "vg_lite.h"
#include "vglite_support.h"
#endif

#if LV_USE_PXP
#if LV_USE_ROTATE_PXP
#include "src/draw/nxp/pxp/lv_draw_pxp.h"
#include "src/display/lv_display_private.h"
#endif
#endif

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_LCDIFV2)
#include "fsl_lcdifv2.h"
#else
#include "fsl_elcdif.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Rotate panel or not. */
#ifndef DEMO_USE_ROTATE
#if LV_USE_PXP
#if LV_USE_ROTATE_PXP
#define DEMO_USE_ROTATE 1
#else
#define DEMO_USE_ROTATE 0
#endif
#else
#define DEMO_USE_ROTATE 0
#endif
#endif

/* Cache line size. */
#ifndef FSL_FEATURE_L2CACHE_LINESIZE_BYTE
#define FSL_FEATURE_L2CACHE_LINESIZE_BYTE 0
#endif
#ifndef FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
#define FSL_FEATURE_L1DCACHE_LINESIZE_BYTE 0
#endif

#if (FSL_FEATURE_L2CACHE_LINESIZE_BYTE > FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define DEMO_CACHE_LINE_SIZE FSL_FEATURE_L2CACHE_LINESIZE_BYTE
#else
#define DEMO_CACHE_LINE_SIZE FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
#endif

#if (DEMO_CACHE_LINE_SIZE > FRAME_BUFFER_ALIGN)
#define DEMO_FB_ALIGN DEMO_CACHE_LINE_SIZE
#else
#define DEMO_FB_ALIGN FRAME_BUFFER_ALIGN
#endif

/* Make the frame buffer cacheline size aligned. */

#if DEMO_USE_ROTATE
#define ROTATED_FB_WIDTH  DEMO_FB_HEIGHT
#define ROTATED_FB_HEIGHT DEMO_FB_WIDTH
#else
#define ROTATED_FB_WIDTH  DEMO_FB_WIDTH
#define ROTATED_FB_HEIGHT DEMO_FB_HEIGHT
#endif

#define DEMO_FB_SIZE DEMO_FB_STRIDE(DEMO_FB_WIDTH) * DEMO_FB_HEIGHT

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p);

static void DEMO_InitTouch(void);

static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data);

static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer);

#if (DEMO_PANEL_RASPI_7INCH != DEMO_PANEL)
static void BOARD_PullMIPIPanelTouchResetPin(bool pullUp);

static void BOARD_ConfigMIPIPanelTouchIntPin(gt911_int_pin_mode_t mode);
#endif
static void DEMO_WaitBufferSwitchOff(void);

#if (LV_COLOR_DEPTH == 8)
/*
 * To support 8 color depth and 1 color depth with this board, color palette is
 * used to map 256 color to 2^16 color.
 */
static void DEMO_SetLcdColorPalette(void);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
SDK_ALIGN(static uint8_t s_frameBuffer[2][DEMO_FB_SIZE], DEMO_FB_ALIGN);
#if DEMO_USE_ROTATE
SDK_ALIGN(static uint8_t s_lvglBuffer[1][DEMO_FB_SIZE], DEMO_FB_ALIGN);
#endif

#if defined(SDK_OS_FREE_RTOS)
static SemaphoreHandle_t s_transferDone;
#else
static volatile bool s_transferDone;
#endif

#if DEMO_USE_ROTATE
/*
 * When rotate is used, LVGL stack draws in one buffer (s_lvglBuffer), and LCD
 * driver uses two buffers (s_frameBuffer) to remove tearing effect.
 */
static void *volatile s_inactiveFrameBuffer;
#endif

#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
static ft5406_rt_handle_t s_touchHandle;
#else
static gt911_handle_t s_touchHandle;

static const gt911_config_t s_touchConfig = {
    .I2C_SendFunc     = BOARD_MIPIPanelTouch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_MIPIPanelTouch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullMIPIPanelTouchResetPin,
    .intPinFunc       = BOARD_ConfigMIPIPanelTouchIntPin,
    .timeDelayMsFunc  = VIDEO_DelayMs,
    .touchPointNum    = 1,
    .i2cAddrMode      = kGT911_I2cAddrMode0,
    .intTrigMode      = kGT911_IntRisingEdge,
};

static int s_touchResolutionX;
static int s_touchResolutionY;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (LV_COLOR_DEPTH == 8)
static void DEMO_SetLcdColorPalette(void)
{
    /* For 8 bit format , LVGL uses the luminance of a color. */
    uint32_t palette[256];

    for (uint32_t i = 0; i < 256U; i++)
    {
        palette[i] = (i << 16U) | (i << 8U) | (i << 0U);
    }

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_ELCDIF)
    ELCDIF_UpdateLut(LCDIF, kELCDIF_Lut0, 0, palette, 256);
    ELCDIF_EnableLut(LCDIF, true);
#else
    LCDIFV2_SetLut(LCDIFV2, 0, palette, 256, false);
#endif
}
#endif

void lv_port_disp_init(void)
{
    lv_display_t * disp_drv; /*Descriptor of a display driver*/

    status_t status;
    dc_fb_info_t fbInfo;

#if LV_USE_DRAW_VGLITE
    /* Initialize GPU. */
    BOARD_PrepareVGLiteController();
#endif

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    BOARD_PrepareDisplayController();

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        assert(0);
    }

#if (LV_COLOR_DEPTH == 8)
    DEMO_SetLcdColorPalette();
#endif

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
    fbInfo.pixelFormat = DEMO_BUFFER_PIXEL_FORMAT;
    fbInfo.width       = DEMO_FB_WIDTH;
    fbInfo.height      = DEMO_FB_HEIGHT;
    fbInfo.startX      = DEMO_BUFFER_START_X;
    fbInfo.startY      = DEMO_BUFFER_START_Y;
    fbInfo.strideBytes = DEMO_FB_STRIDE(DEMO_FB_WIDTH);
    g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

    g_dc.ops->setCallback(&g_dc, 0, DEMO_BufferSwitchOffCallback, NULL);

#if defined(SDK_OS_FREE_RTOS)
    s_transferDone = xSemaphoreCreateBinary();
    if (NULL == s_transferDone)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }
#else
    s_transferDone = false;
#endif

#if DEMO_USE_ROTATE
    /* s_frameBuffer[1] is first shown in the panel, s_frameBuffer[0] is inactive. */
    s_inactiveFrameBuffer = (void *)s_frameBuffer[0];
#endif

    /* lvgl starts render in frame buffer 0, so show frame buffer 1 first. */
    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_frameBuffer[1]);

    /* Wait for frame buffer sent to display controller video memory. */
    if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
    {
        DEMO_WaitBufferSwitchOff();
    }

    g_dc.ops->enableLayer(&g_dc, 0);

    disp_drv = lv_display_create(ROTATED_FB_WIDTH, ROTATED_FB_HEIGHT);

    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));
#if DEMO_USE_ROTATE
    memset(s_lvglBuffer, 0, sizeof(s_lvglBuffer));
    lv_display_set_buffers_with_stride(disp_drv, (void *)s_lvglBuffer[0], NULL, DEMO_FB_SIZE, DEMO_FB_STRIDE(ROTATED_FB_WIDTH), LV_DISPLAY_RENDER_MODE_FULL);
#else
    lv_display_set_buffers_with_stride(disp_drv, (void *)s_frameBuffer[0], (void *)s_frameBuffer[1], DEMO_FB_SIZE, DEMO_FB_STRIDE(ROTATED_FB_WIDTH), LV_DISPLAY_RENDER_MODE_FULL);
#endif

    lv_display_set_flush_cb(disp_drv, DEMO_FlushDisplay);

#if LV_USE_DRAW_VGLITE
    if (vg_lite_init(DEFAULT_VG_LITE_TW_WIDTH, DEFAULT_VG_LITE_TW_HEIGHT) != VG_LITE_SUCCESS)
    {
        PRINTF("VGLite init error. STOP.");
        vg_lite_close();
        while (1)
            ;
    }

    if (vg_lite_set_command_buffer_size(VG_LITE_COMMAND_BUFFER_SIZE) != VG_LITE_SUCCESS)
    {
        PRINTF("VGLite set command buffer. STOP.");
        vg_lite_close();
        while (1)
            ;
    }
#endif
}

static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
#if defined(SDK_OS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;

    xSemaphoreGiveFromISR(s_transferDone, &taskAwake);
    portYIELD_FROM_ISR(taskAwake);
#else
    s_transferDone = true;
#endif

#if DEMO_USE_ROTATE
    s_inactiveFrameBuffer = switchOffBuffer;
#endif
}

void DEMO_CleanInvalidateCacheByAddr(void * addr, int32_t dsize)
{
#if __CORTEX_M == 4
    L1CACHE_CleanInvalidateSystemCacheByRange((uint32_t)addr, dsize);
#else
    SCB_CleanInvalidateDCache_by_Addr(addr, dsize);
#endif
}


static void DEMO_WaitBufferSwitchOff(void)
{
#if defined(SDK_OS_FREE_RTOS)
    if (xSemaphoreTake(s_transferDone, portMAX_DELAY) != pdTRUE)
    {
        PRINTF("Display flush failed\r\n");
        assert(0);
    }
#else
    while (false == s_transferDone)
    {
    }
    s_transferDone = false;
#endif
}

static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
#if DEMO_USE_ROTATE

    /*
     * Work flow:
     *
     * 1. Wait for the available inactive frame buffer to draw.
     * 2. Draw the rotated frame to inactive buffer.
     * 3. Pass inactive to LCD controller to show.
     */

    static bool firstFlush = true;

    /* Only wait for the first time. */
    if (firstFlush)
    {
        firstFlush = false;
    }
    else
    {
        /* Wait frame buffer. */
        DEMO_WaitBufferSwitchOff();
    }

    /* Copy buffer. */
    void *inactiveFrameBuffer = s_inactiveFrameBuffer;

#if __CORTEX_M == 4
    L1CACHE_CleanInvalidateSystemCacheByRange((uint32_t)s_inactiveFrameBuffer, DEMO_FB_SIZE);
#else
    SCB_CleanInvalidateDCache_by_Addr(inactiveFrameBuffer, DEMO_FB_SIZE);
#endif

#if LV_USE_ROTATE_PXP /* Use PXP to rotate the panel. */
    lv_draw_pxp_rotate(color_p, inactiveFrameBuffer,
                       ROTATED_FB_WIDTH, ROTATED_FB_HEIGHT,
                       DEMO_FB_STRIDE(ROTATED_FB_WIDTH),
                       DEMO_FB_STRIDE(ROTATED_FB_HEIGHT),
                       LV_DISPLAY_ROTATION_90,
#if (LV_COLOR_DEPTH == 32)
                        LV_COLOR_FORMAT_XRGB8888
#elif (LV_COLOR_DEPTH == 16)
                        LV_COLOR_FORMAT_RGB565
#else
                        LV_COLOR_FORMAT_RAW
#endif
                        );

#else /* Use CPU to rotate the panel. */
    lv_draw_sw_rotate(color_p, inactiveFrameBuffer,
                       ROTATED_FB_WIDTH, ROTATED_FB_HEIGHT,
                       DEMO_FB_STRIDE(ROTATED_FB_WIDTH),
                       DEMO_FB_STRIDE(ROTATED_FB_HEIGHT),
                       LV_DISPLAY_ROTATION_90,
#if (LV_COLOR_DEPTH == 32)
                        LV_COLOR_FORMAT_XRGB8888
#elif (LV_COLOR_DEPTH == 16)
                        LV_COLOR_FORMAT_RGB565
#else
                        LV_COLOR_FORMAT_RAW
#endif
                        );
#endif

#if __CORTEX_M == 4
    L1CACHE_CleanInvalidateSystemCacheByRange((uint32_t)s_inactiveFrameBuffer, DEMO_FB_SIZE);
#else
    SCB_CleanInvalidateDCache_by_Addr(inactiveFrameBuffer, DEMO_FB_SIZE);
#endif

    g_dc.ops->setFrameBuffer(&g_dc, 0, inactiveFrameBuffer);

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);

#else  /* DEMO_USE_ROTATE */

#if __CORTEX_M == 4
    L1CACHE_CleanInvalidateSystemCacheByRange((uint32_t)color_p, DEMO_FB_SIZE);
#else
    SCB_CleanInvalidateDCache_by_Addr(color_p, DEMO_FB_SIZE);
#endif

    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)color_p);

    DEMO_WaitBufferSwitchOff();

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
#endif /* DEMO_USE_ROTATE */
}

void lv_port_indev_init(void)
{
    /*Initialize your touchpad */
    DEMO_InitTouch();

    /*Register a touchpad input device*/
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, DEMO_ReadTouch);
}
#if (DEMO_PANEL_RASPI_7INCH != DEMO_PANEL)
static void BOARD_PullMIPIPanelTouchResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_PANEL_TOUCH_RST_GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_PANEL_TOUCH_RST_GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PIN, 0);
    }
}

static void BOARD_ConfigMIPIPanelTouchIntPin(gt911_int_pin_mode_t mode)
{
    if (mode == kGT911_IntPinInput)
    {
        BOARD_MIPI_PANEL_TOUCH_INT_GPIO->GDIR &= ~(1UL << BOARD_MIPI_PANEL_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN, 0);
        }
        else
        {
            GPIO_PinWrite(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN, 1);
        }

        BOARD_MIPI_PANEL_TOUCH_INT_GPIO->GDIR |= (1UL << BOARD_MIPI_PANEL_TOUCH_INT_PIN);
    }
}
#endif
/*Initialize your touchpad*/
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
static void DEMO_InitTouch(void)
{
    status_t status;

    /* Initialize touch panel controller */
    status = FT5406_RT_Init(&s_touchHandle, BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR);
    if (status != kStatus_Success)
    {
        PRINTF("Touch IC initialization failed\r\n");
    }
}
#else
static void DEMO_InitTouch(void)
{
    status_t status;

    const gpio_pin_config_t resetPinConfig = {
        .direction = kGPIO_DigitalOutput, .outputLogic = 0, .interruptMode = kGPIO_NoIntmode};
    GPIO_PinInit(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN, &resetPinConfig);
    GPIO_PinInit(BOARD_MIPI_PANEL_TOUCH_RST_GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PIN, &resetPinConfig);

    status = GT911_Init(&s_touchHandle, &s_touchConfig);

    if (kStatus_Success != status)
    {
        PRINTF("Touch IC initialization failed\r\n");
        assert(false);
    }

    GT911_GetResolution(&s_touchHandle, &s_touchResolutionX, &s_touchResolutionY);
}
#endif

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    static int touch_x = 0;
    static int touch_y = 0;

#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
    touch_event_t touch_event;
    if (kStatus_Success == FT5406_RT_GetSingleTouch(&s_touchHandle, &touch_event, &touch_x, &touch_y))
    {
        if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
        {
            data->state = LV_INDEV_STATE_PR;
        }
    }
#else
    if (kStatus_Success == GT911_GetSingleTouch(&s_touchHandle, &touch_x, &touch_y))
    {
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
#endif

#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
#if DEMO_USE_ROTATE
    data->point.x = DEMO_PANEL_HEIGHT - touch_y;
    data->point.y = touch_x;
#else
    data->point.x = touch_x;
    data->point.y = touch_y; 
#endif
#else
    /*Set the last pressed coordinates*/
#if DEMO_USE_ROTATE
    data->point.x = DEMO_PANEL_HEIGHT - (touch_y * DEMO_PANEL_HEIGHT / s_touchResolutionY);
    data->point.y = touch_x * DEMO_PANEL_WIDTH / s_touchResolutionX;
#else
    data->point.x = touch_x * DEMO_PANEL_WIDTH / s_touchResolutionX;
    data->point.y = touch_y * DEMO_PANEL_HEIGHT / s_touchResolutionY;
#endif
#endif
}
