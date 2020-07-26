/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "littlevgl_support.h"
#include "lvgl.h"
#if defined(FSL_RTOS_FREE_RTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#include "fsl_elcdif.h"
#if LV_USE_GPU
#include "fsl_pxp.h"
#endif
#include "fsl_lpi2c.h"
#include "fsl_gpio.h"
#include "fsl_ft5406_rt.h"
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
#define LCD_HSW 41
#define LCD_HFP 4
#define LCD_HBP 8
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 2
#define LCD_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)
#define LCD_LCDIF_DATA_BUS kELCDIF_DataBus16Bit

/* Back light. */
#define LCD_BL_GPIO     GPIO2
#define LCD_BL_GPIO_PIN 31

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);

static void DEMO_InitLcdClock(void);

static void DEMO_InitLcdBackLight(void);

static void DEMO_FlushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

static void DEMO_InitTouch(void);

static bool DEMO_ReadTouch(lv_indev_drv_t *drv, lv_indev_data_t *data);

#if LV_USE_GPU
static void DEMO_InitPxp(void);

static void DEMO_GPU_Fill(
    lv_disp_drv_t *disp_drv, lv_color_t *dest_buf, lv_coord_t dest_width, const lv_area_t *fill_area, lv_color_t color);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_framePending;
#if defined(FSL_RTOS_FREE_RTOS)
static SemaphoreHandle_t s_frameSema;
#endif
static ft5406_rt_handle_t touchHandle;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_frameBuffer[2][LCD_WIDTH * LCD_HEIGHT * LCD_FB_BYTE_PER_PIXEL], 64);

#if LV_USE_GPU
#if defined(FSL_RTOS_FREE_RTOS)
static SemaphoreHandle_t s_pxpIdle;
#else
static volatile bool s_pxpIdle;
#endif
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void lv_port_pre_init(void)
{
}

void lv_port_disp_init(void)
{
    static lv_disp_buf_t disp_buf;

    lv_disp_buf_init(&disp_buf, s_frameBuffer[0], s_frameBuffer[1], LCD_WIDTH * LCD_HEIGHT);

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

#if LV_USE_GPU
    DEMO_InitPxp();
#endif

    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    lv_disp_drv_t disp_drv;      /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = DEMO_FlushDisplay;

#if LV_USE_GPU
    disp_drv.gpu_fill_cb = DEMO_GPU_Fill;
#endif

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

void LCDIF_IRQHandler(void)
{
#if defined(FSL_RTOS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;
#endif

    uint32_t intStatus = ELCDIF_GetInterruptStatus(LCDIF);

    ELCDIF_ClearInterruptStatus(LCDIF, intStatus);

    if (s_framePending)
    {
        if (intStatus & kELCDIF_CurFrameDone)
        {
            s_framePending = false;

#if defined(FSL_RTOS_FREE_RTOS)
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
        /* littlevgl starts render in frame buffer 0, so show frame buffer 1 first. */
        .bufferAddr  = (uint32_t)s_frameBuffer[1],
        .pixelFormat = kELCDIF_PixelFormatRGB565,
        .dataBus     = LCD_LCDIF_DATA_BUS,
    };

    /* Clear frame buffer. */
    memset((void *)s_frameBuffer, 0, sizeof(s_frameBuffer));

#if defined(FSL_RTOS_FREE_RTOS)
    s_frameSema = xSemaphoreCreateBinary();
    if (NULL == s_frameSema)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }
#endif

    /* No frame pending. */
    s_framePending = false;
#if defined(FSL_RTOS_FREE_RTOS)
    NVIC_SetPriority(LCDIF_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
#endif

    DEMO_InitLcdClock();

    ELCDIF_RgbModeInit(LCDIF, &config);

    ELCDIF_EnableInterrupts(LCDIF, kELCDIF_CurFrameDoneInterruptEnable);
    NVIC_EnableIRQ(LCDIF_IRQn);
    ELCDIF_RgbModeStart(LCDIF);

    DEMO_InitLcdBackLight();
}

#if LV_USE_GPU
static void DEMO_InitPxp(void)
{
#if defined(FSL_RTOS_FREE_RTOS)
    s_pxpIdle = xSemaphoreCreateBinary();
    if (NULL == s_pxpIdle)
    {
        PRINTF("PXP semaphore create failed\r\n");
        assert(0);
    }

    NVIC_SetPriority(PXP_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
#endif

    PXP_Init(PXP);

    /* Disable CSC1, it is enabled by default. */
    PXP_EnableCsc1(PXP, false);

    PXP_EnableInterrupts(PXP, kPXP_CompleteInterruptEnable);

    NVIC_EnableIRQ(PXP_IRQn);
}

void PXP_IRQHandler(void)
{
#if defined(FSL_RTOS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;
#endif

    if (kPXP_CompleteFlag & PXP_GetStatusFlags(PXP))
    {
        PXP_ClearStatusFlags(PXP, kPXP_CompleteFlag);

#if defined(FSL_RTOS_FREE_RTOS)
        xSemaphoreGiveFromISR(s_pxpIdle, &taskAwake);

        portYIELD_FROM_ISR(taskAwake);
#else
        s_pxpIdle = true;
#endif
    }
}

static uint32_t DEMO_LVGLColorToXRGB8888(lv_color_t color)
{
    return ((uint32_t)color.ch.blue << 3U) | ((uint32_t)color.ch.green << (8U + 2U)) |
           ((uint32_t)color.ch.red << (16U + 3U));
}

static void DEMO_GPU_Fill(
    lv_disp_drv_t *disp_drv, lv_color_t *dest_buf, lv_coord_t dest_width, const lv_area_t *fill_area, lv_color_t color)
{
    /*
     * To fill an area with const color, the process surface and alpha surface
     * are disabled, the output buffer buffer is configured to the fill area,
     * and the background color is set as the color to fill.
     */

    pxp_output_buffer_config_t outputConfig = {
        .pixelFormat    = kPXP_OutputPixelFormatRGB565,
        .interlacedMode = kPXP_OutputProgressive,
        .buffer0Addr    = (uint32_t)(dest_buf + dest_width * fill_area->y1 + fill_area->x1),
        .buffer1Addr    = NULL,
        .pitchBytes     = dest_width * sizeof(lv_color_t),
        .width          = fill_area->x2 - fill_area->x1 + 1,
        .height         = fill_area->y2 - fill_area->y1 + 1,
    };

    PXP_SetOutputBufferConfig(PXP, &outputConfig);

    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    /* Disable PS. */
    PXP_SetProcessSurfacePosition(PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    PXP_SetProcessSurfaceBackGroundColor(PXP, DEMO_LVGLColorToXRGB8888(color));

#if !defined(FSL_RTOS_FREE_RTOS)
    s_pxpIdle = false;
#endif

    PXP_Start(PXP);

#if defined(FSL_RTOS_FREE_RTOS)
    if (xSemaphoreTake(s_pxpIdle, portMAX_DELAY) != pdTRUE)
    {
        PRINTF("PXP semaphore wait failed\r\n");
        while (1)
            ;
    }
#else
    while (false == s_pxpIdle)
    {
    }
#endif
}
#endif

static void DEMO_FlushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)color_p);

    s_framePending = true;

#if defined(FSL_RTOS_FREE_RTOS)
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
    lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad */
    DEMO_InitTouch();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = DEMO_ReadTouch;
    lv_indev_drv_register(&indev_drv);
}

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
static bool DEMO_ReadTouch(lv_indev_drv_t *drv, lv_indev_data_t *data)
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

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}
