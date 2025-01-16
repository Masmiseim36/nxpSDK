/*
 * Copyright 2019-2021, 2023 NXP
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
#include "fsl_video_common.h"
#include "fsl_elcdif.h"
#include "fsl_lpi2c.h"
#include "fsl_gpio.h"
#include "fsl_cache.h"
#if (LVGL_PANEL == LVGL_PANEL_RK043FN66HS)
#include "fsl_gt911.h"
#else
#include "fsl_ft5406_rt.h"
#endif
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Macros for the touch touch controller. */
#define TOUCH_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define TOUCH_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define TOUCH_LPI2C_CLOCK_SOURCE_DIVIDER (5U)

#define TOUCH_I2C_CLOCK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (TOUCH_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define TOUCH_I2C_BAUDRATE   100000U

/* Macros for panel. */
#if (LVGL_PANEL == LVGL_PANEL_RK043FN66HS)

#define LCD_HSW 4
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_VSW 4
#define LCD_VFP 8
#define LCD_VBP 12

#else

#define LCD_HSW 41
#define LCD_HFP 4
#define LCD_HBP 8
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 2

#endif

#define LCD_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

#if (LV_COLOR_DEPTH == 16)
#define LCD_FB_BYTE_PER_PIXEL 2
#define LCDIF_PIXEL_FORMAT    kELCDIF_PixelFormatRGB565
#define LCD_LCDIF_DATA_BUS    kELCDIF_DataBus16Bit
#elif ((LV_COLOR_DEPTH == 8) || (LV_COLOR_DEPTH == 1))
#define LCD_FB_BYTE_PER_PIXEL 1
#define LCDIF_PIXEL_FORMAT    kELCDIF_PixelFormatRAW8
#define LCD_LCDIF_DATA_BUS    kELCDIF_DataBus8Bit
#elif (LV_COLOR_DEPTH == 32)
#define LCD_FB_BYTE_PER_PIXEL 4
#define LCDIF_PIXEL_FORMAT    kELCDIF_PixelFormatXRGB8888
#define LCD_LCDIF_DATA_BUS    kELCDIF_DataBus16Bit
#else
#error Color Depth not supported
#endif

/* Back light. */
#define LCD_BL_GPIO     GPIO2
#define LCD_BL_GPIO_PIN 31

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

#if (LV_ATTRIBUTE_MEM_ALIGN_SIZE > DEMO_FB_ALIGN)
#undef DEMO_FB_ALIGN
#define DEMO_FB_ALIGN LV_ATTRIBUTE_MEM_ALIGN_SIZE
#endif

#define DEMO_FB_SIZE (((LCD_WIDTH * LCD_HEIGHT * LCD_FB_BYTE_PER_PIXEL) + DEMO_FB_ALIGN - 1) & ~(DEMO_FB_ALIGN - 1))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);

static void DEMO_InitLcdClock(void);

static void DEMO_InitLcdBackLight(void);

static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p);

static void DEMO_InitTouch(void);

static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data);

#if (LVGL_PANEL == LVGL_PANEL_RK043FN66HS)
static void BOARD_PullTouchResetPin(bool pullUp);

static void BOARD_ConfigTouchIntPin(gt911_int_pin_mode_t mode);
#endif

#if (LV_COLOR_DEPTH == 8)
/*
 * To support 8 color depth with this board, color palette is
 * used to map 256 color to 2^16 color.
 */
static void DEMO_SetLcdColorPalette(void);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_framePending;
#if defined(SDK_OS_FREE_RTOS)
static SemaphoreHandle_t s_frameSema;
#endif

#if (LVGL_PANEL == LVGL_PANEL_RK043FN66HS)
static gt911_handle_t s_touchHandle;
static const gt911_config_t s_touchConfig = {
    .I2C_SendFunc     = BOARD_Touch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_Touch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullTouchResetPin,
    .intPinFunc       = BOARD_ConfigTouchIntPin,
    .timeDelayMsFunc  = VIDEO_DelayMs,
    .touchPointNum    = 1,
    .i2cAddrMode      = kGT911_I2cAddrMode0,
    .intTrigMode      = kGT911_IntRisingEdge,
};
static int s_touchResolutionX;
static int s_touchResolutionY;
#else
static ft5406_rt_handle_t touchHandle;
#endif

SDK_ALIGN(static uint8_t s_frameBuffer[2][DEMO_FB_SIZE], DEMO_FB_ALIGN);

/*******************************************************************************
 * Code
 ******************************************************************************/

void lv_port_disp_init(void)
{
    lv_display_t * disp_drv; /*Descriptor of a display driver*/

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

    disp_drv = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

    lv_display_set_buffers(disp_drv, s_frameBuffer[0], s_frameBuffer[1], DEMO_FB_SIZE, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(disp_drv, DEMO_FlushDisplay);
}

void LCDIF_IRQHandler(void)
{
#if defined(SDK_OS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;
#endif

    uint32_t intStatus = ELCDIF_GetInterruptStatus(LCDIF);

    ELCDIF_ClearInterruptStatus(LCDIF, intStatus);

    if (s_framePending)
    {
        if (intStatus & kELCDIF_CurFrameDone)
        {
            s_framePending = false;

#if defined(SDK_OS_FREE_RTOS)
            xSemaphoreGiveFromISR(s_frameSema, &taskAwake);

            portYIELD_FROM_ISR(taskAwake);
#endif
        }
    }
    SDK_ISR_EXIT_BARRIER;
}

static void DEMO_InitLcdClock(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31,
        .postDivider = 8,
        .numerator   = 0,
        .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}

static void DEMO_InitLcdBackLight(void)
{
    const gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        1,
        kGPIO_NoIntmode,
    };

    /* Backlight. */
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}

#if (LV_COLOR_DEPTH == 8)
static void DEMO_SetLcdColorPalette(void)
{
    /* For 8 bit format , LVGL uses the luminance of a color. */
    uint32_t palette[256];

    for (uint32_t i = 0; i < 256U; i++)
    {
        palette[i] = (i << 16U) | (i << 8U) | (i << 0U);
    }

    ELCDIF_UpdateLut(LCDIF, kELCDIF_Lut0, 0, palette, 256);
    ELCDIF_EnableLut(LCDIF, true);
}
#endif

static void DEMO_InitLcd(void)
{
    /* Initialize the display. */
    const elcdif_rgb_mode_config_t config = {
        .panelWidth    = LCD_WIDTH,
        .panelHeight   = LCD_HEIGHT,
        .hsw           = LCD_HSW,
        .hfp           = LCD_HFP,
        .hbp           = LCD_HBP,
        .vsw           = LCD_VSW,
        .vfp           = LCD_VFP,
        .vbp           = LCD_VBP,
        .polarityFlags = LCD_POL_FLAGS,
        /* lvgl starts render in frame buffer 0, so show frame buffer 1 first. */
        .bufferAddr  = (uint32_t)s_frameBuffer[1],
        .pixelFormat = LCDIF_PIXEL_FORMAT,
        .dataBus     = LCD_LCDIF_DATA_BUS,
    };

    /* Clear frame buffer. */
    memset((void *)s_frameBuffer, 0, sizeof(s_frameBuffer));

#if defined(SDK_OS_FREE_RTOS)
    s_frameSema = xSemaphoreCreateBinary();
    if (NULL == s_frameSema)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }
#endif

    /* No frame pending. */
    s_framePending = false;
#if defined(SDK_OS_FREE_RTOS)
    NVIC_SetPriority(LCDIF_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
#endif

    DEMO_InitLcdClock();

    ELCDIF_RgbModeInit(LCDIF, &config);

#if (LV_COLOR_DEPTH == 8)
    DEMO_SetLcdColorPalette();
#endif

    ELCDIF_EnableInterrupts(LCDIF, kELCDIF_CurFrameDoneInterruptEnable);
    NVIC_EnableIRQ(LCDIF_IRQn);
    ELCDIF_RgbModeStart(LCDIF);

    DEMO_InitLcdBackLight();
}

#if LV_USE_DRAW_PXP
void DEMO_CleanInvalidateCacheByAddr(void * addr, int32_t dsize)
{
    SCB_CleanInvalidateDCache_by_Addr(addr, dsize);
}
#endif

static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
    DCACHE_CleanInvalidateByRange((uint32_t)color_p, DEMO_FB_SIZE);

    ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)color_p);

    s_framePending = true;

#if defined(SDK_OS_FREE_RTOS)
    if (xSemaphoreTake(s_frameSema, portMAX_DELAY) == pdTRUE)
    {
        /* IMPORTANT!!!
         * Inform the graphics library that you are ready with the flushing*/
        lv_disp_flush_ready(disp_drv);
    }
    else
    {
        PRINTF("Display flush failed\r\n");
        assert(0);
    }
#else
    while (s_framePending)
    {
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
#endif
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

#if (LVGL_PANEL == LVGL_PANEL_RK043FN66HS)
static void BOARD_PullTouchResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, 0);
    }
}

static void BOARD_ConfigTouchIntPin(gt911_int_pin_mode_t mode)
{
    if (mode == kGT911_IntPinInput)
    {
        BOARD_TOUCH_INT_GPIO->GDIR &= ~(1UL << BOARD_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, 0);
        }
        else
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, 1);
        }

        BOARD_TOUCH_INT_GPIO->GDIR |= (1UL << BOARD_TOUCH_INT_PIN);
    }
}

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;

    const gpio_pin_config_t resetPinConfig = {
        .direction = kGPIO_DigitalOutput, .outputLogic = 0, .interruptMode = kGPIO_NoIntmode};
    GPIO_PinInit(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, &resetPinConfig);
    GPIO_PinInit(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, &resetPinConfig);

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, TOUCH_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, TOUCH_LPI2C_CLOCK_SOURCE_DIVIDER);

    BOARD_LPI2C_Init(TOUCH_I2C, TOUCH_I2C_CLOCK_FREQ);

    status = GT911_Init(&s_touchHandle, &s_touchConfig);

    if (kStatus_Success != status)
    {
        PRINTF("Touch IC initialization failed\r\n");
        assert(false);
    }

    GT911_GetResolution(&s_touchHandle, &s_touchResolutionX, &s_touchResolutionY);
}

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    static int touch_x = 0;
    static int touch_y = 0;

    if (kStatus_Success == GT911_GetSingleTouch(&s_touchHandle, &touch_x, &touch_y))
    {
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = touch_x * LCD_WIDTH / s_touchResolutionX;
    data->point.y = touch_y * LCD_HEIGHT / s_touchResolutionY;
}
#else
/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;

    lpi2c_master_config_t masterConfig = {0};

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, TOUCH_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, TOUCH_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = TOUCH_I2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(TOUCH_I2C, &masterConfig, TOUCH_I2C_CLOCK_FREQ);

    /* Initialize touch panel controller */
    status = FT5406_RT_Init(&touchHandle, TOUCH_I2C);
    if (status != kStatus_Success)
    {
        PRINTF("Touch panel init failed\n");
        assert(0);
    }
}

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    data->state = LV_INDEV_STATE_REL;

    if (kStatus_Success == FT5406_RT_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
    {
        if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
        {
            data->state = LV_INDEV_STATE_PR;
        }
    }

    /*Set the last pressed coordinates*/
    data->point.x = touch_y;
    data->point.y = touch_x;
}
#endif
