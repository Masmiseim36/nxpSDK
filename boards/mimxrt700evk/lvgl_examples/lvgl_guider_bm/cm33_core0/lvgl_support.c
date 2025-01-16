/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl_support.h"
#include "display_support.h"
#include "lvgl.h"
#if defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif
#include "board.h"

#include "fsl_gpio.h"
#include "fsl_power.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#if (DEMO_PANEL == DEMO_PANEL_RM67162)
#include "fsl_ft3267.h"
#elif (DEMO_PANEL == DEMO_PANEL_CO5300)
#include "fsl_tma525b.h"
#elif (DEMO_PANEL == DEMO_PANEL_TFT_PROTO_5) || (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
#include "fsl_lpi2c.h"
#include "fsl_ft5406_rt.h"
#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
       (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))
#include "fsl_gt911.h"
#endif

#if LV_USE_DRAW_VGLITE
#include "vg_lite.h"
#include "vglite_support.h"
#include "lv_vglite_utils.h"
#endif

#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * DBI mode panel (or named command mode panel) supports partial refresh and full refresh,
 * DPI mode panel (or named video mode panel) only supports full refresh.
 */
#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055MHD091 == DEMO_PANEL) || (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL))
#define DEMO_DISPLAY_USE_PARTIAL_REFRESH 0
#else
#define DEMO_DISPLAY_USE_PARTIAL_REFRESH 1
#endif

#ifndef DEMO_USE_ROTATE
/* Use rotate for portrait panel. */
#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))
#define DEMO_USE_ROTATE 0
#else
#define DEMO_USE_ROTATE 0
#endif
#endif

#if DEMO_USE_ROTATE
#define ROTATED_FB_WIDTH  DEMO_FB_HEIGHT
#define ROTATED_FB_HEIGHT DEMO_FB_WIDTH
#else
#define ROTATED_FB_WIDTH  DEMO_FB_WIDTH
#define ROTATED_FB_HEIGHT DEMO_FB_HEIGHT
#endif

#if DEMO_USE_ROTATE
#define LVGL_BUFFER_ADDR 0x28800000U
#endif

#define DEMO_FB_SIZE (DEMO_BUFFER_STRIDE_BYTE * DEMO_FB_HEIGHT)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p);

static void DEMO_InitTouch(void);

static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data);

static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer);

#if ((DEMO_PANEL == DEMO_PANEL_RM67162) || (DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || (DEMO_PANEL_RK055MHD091 == DEMO_PANEL) || \
     (DEMO_PANEL == DEMO_PANEL_CO5300))
void BOARD_PullMIPIPanelTouchResetPin(bool pullUp);
#endif

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))
static void BOARD_ConfigMIPIPanelTouchIntPin(gt911_int_pin_mode_t mode);
#endif

static void DEMO_WaitBufferSwitchOff(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
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

#if (DEMO_PANEL == DEMO_PANEL_RM67162)
static ft3267_handle_t s_touchHandle;
static volatile bool s_touchEvent = false;

static const ft3267_config_t s_touchConfig = {
    .I2C_ReceiveFunc  = BOARD_MIPIPanelTouch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullMIPIPanelTouchResetPin,
    .pullPowerPinFunc = NULL,
    .timeDelayMsFunc  = VIDEO_DelayMs,
};

#elif (DEMO_PANEL == DEMO_PANEL_CO5300)
static tma525b_handle_t s_touchHandle;
static volatile bool s_touchEvent = false;

static const tma525b_config_t s_touchConfig = {
    .I2C_SendFunc  = BOARD_MIPIPanelTouch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_MIPIPanelTouch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullMIPIPanelTouchResetPin,
    .pullPowerPinFunc = NULL,
    .timeDelayMsFunc  = VIDEO_DelayMs,
};

#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
       (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))
/* GT911 I2C address depends on the int pin state during initialization.
 * On this board, the touch panel int pin is forced to input, so the I2C address
 * could not be configured, driver select the one which works.
 */
static gt911_handle_t s_touchHandle;
static const gt911_config_t s_touchConfig = {
    .I2C_SendFunc     = BOARD_MIPIPanelTouch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_MIPIPanelTouch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullMIPIPanelTouchResetPin,
    .intPinFunc       = BOARD_ConfigMIPIPanelTouchIntPin,
    .timeDelayMsFunc  = VIDEO_DelayMs,
    .touchPointNum    = 1,
    .i2cAddrMode      = kGT911_I2cAddrAny,
    .intTrigMode      = kGT911_IntRisingEdge,
};
static int s_touchResolutionX;
static int s_touchResolutionY;
#endif

static dc_fb_info_t fbInfo;

/*******************************************************************************
 * Code
 ******************************************************************************/

void lv_port_pre_init(void)
{
}

void lv_port_disp_init(void)
{
    lv_display_t * disp_drv;

    memset((void *)DEMO_BUFFER0_ADDR, 0, DEMO_FB_SIZE);
    memset((void *)DEMO_BUFFER1_ADDR, 0, DEMO_FB_SIZE);

    status_t status;

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

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
    fbInfo.pixelFormat = DEMO_BUFFER_PIXEL_FORMAT;
    fbInfo.width       = DEMO_FB_WIDTH;
    fbInfo.height      = DEMO_FB_HEIGHT;
    fbInfo.startX      = DEMO_BUFFER_START_X;
    fbInfo.startY      = DEMO_BUFFER_START_Y;
    fbInfo.strideBytes = DEMO_BUFFER_STRIDE_BYTE;
    g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    disp_drv = lv_display_create(ROTATED_FB_WIDTH, ROTATED_FB_HEIGHT);
    g_dc.ops->setCallback(&g_dc, 0, DEMO_BufferSwitchOffCallback, disp_drv);

#if defined(SDK_OS_FREE_RTOS)
    s_transferDone = xSemaphoreCreateBinary();
    if (NULL == s_transferDone)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }
#else
    s_transferDone        = false;
#endif

#if DEMO_USE_ROTATE
    /* s_frameBuffer[1] is first shown in the panel, s_frameBuffer[0] is inactive. */
    s_inactiveFrameBuffer = (void *)DEMO_BUFFER0_ADDR;
#endif

    /* Clear initial frame. */
    /* lvgl starts render in frame buffer 0, so show frame buffer 1 first. */

    memset((void *)DEMO_BUFFER1_ADDR, 0, DEMO_BUFFER_STRIDE_BYTE * DEMO_FB_HEIGHT);
    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)DEMO_BUFFER1_ADDR);

    /* Wait for frame buffer sent to display controller video memory. */
    if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
    {
        DEMO_WaitBufferSwitchOff();
    }

    g_dc.ops->enableLayer(&g_dc, 0);

    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    /*Set up the functions to access to your display*/
    lv_display_set_flush_cb(disp_drv, DEMO_FlushDisplay);

    /*
     * Two buffers screen sized buffer for double buffering.
     * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments.
     */
#if DEMO_DISPLAY_USE_PARTIAL_REFRESH
    /* In partial refresh mode, buffer 0 is lvgl working area, it should be larger than
     * 1/10 of full screen buffer, in this examples it is full screen size. Buffer 1 is a
     * full screen buffer. LVGL updated areas will first be merge and updated in buffer 1,
     * then the dirty region in buffer 1 will be sent to LCD controller at the right time.
     */
    lv_display_set_buffers_with_stride(disp_drv, (void *)DEMO_BUFFER0_ADDR, NULL, DEMO_FB_SIZE, DEMO_BUFFER_STRIDE_BYTE, LV_DISPLAY_RENDER_MODE_PARTIAL);

#else /* DEMO_DISPLAY_USE_PARTIAL_REFRESH */
#if DEMO_USE_ROTATE
    memset(s_lvglBuffer, 0, sizeof(s_lvglBuffer));
    lv_display_set_buffers(disp_drv, s_lvglBuffer[0], NULL, DEMO_FB_SIZE, LV_DISPLAY_RENDER_MODE_FULL);
#else
    lv_display_set_buffers_with_stride(disp_drv, (void *)DEMO_BUFFER0_ADDR, (void *)DEMO_BUFFER1_ADDR, DEMO_FB_SIZE, DEMO_BUFFER_STRIDE_BYTE, LV_DISPLAY_RENDER_MODE_FULL);
#endif /* DEMO_USE_ROTATE */

#endif /* DEMO_DISPLAY_USE_PARTIAL_REFRESH */

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
#if DEMO_DISPLAY_USE_PARTIAL_REFRESH
    lv_display_t *disp_drv = (lv_display_t *)param;

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready(disp_drv);
#endif

#if defined(SDK_OS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;

    xSemaphoreGiveFromISR(s_transferDone, &taskAwake);
    portYIELD_FROM_ISR(taskAwake);
#else
    s_transferDone        = true;
#endif

#if DEMO_USE_ROTATE
    s_inactiveFrameBuffer = switchOffBuffer;
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

#if DEMO_DISPLAY_USE_PARTIAL_REFRESH
#if LV_USE_DRAW_VGLITE
static void copy_area(const lv_area_t *area, uint8_t *color_p, uint8_t *fb, uint32_t fbStrideBytes, uint8_t align_bytes)
{
    uint32_t y;
    uint32_t areaWidth = lv_area_get_width(area);

    fb += (area->y1 * fbStrideBytes + area->x1 * DEMO_BUFFER_BYTE_PER_PIXEL);

    for (y = area->y1; y <= area->y2; y++)
    {
        memcpy(fb, color_p, areaWidth * DEMO_BUFFER_BYTE_PER_PIXEL);
        fb += fbStrideBytes;
        /* Round up to get correct value to match alignment */
        color_p += LV_ROUND_UP((areaWidth * DEMO_BUFFER_BYTE_PER_PIXEL), align_bytes);
    }
}
#else
static void copy_area(const lv_area_t *area, uint8_t *color_p, uint8_t *fb, uint32_t fbStrideBytes)
{
    uint32_t y;
    uint32_t areaWidth = lv_area_get_width(area);
    uint32_t value;

    fb += (area->y1 * fbStrideBytes + area->x1 * DEMO_BUFFER_BYTE_PER_PIXEL);

    for (y = area->y1; y <= area->y2; y++)
    {
        memcpy(fb, color_p, areaWidth * DEMO_BUFFER_BYTE_PER_PIXEL);
        fb += fbStrideBytes;
        /* Round up to get correct value to match alignment */
        value = (areaWidth * DEMO_BUFFER_BYTE_PER_PIXEL) / LV_DRAW_BUF_STRIDE_ALIGN + ((((areaWidth * DEMO_BUFFER_BYTE_PER_PIXEL) % LV_DRAW_BUF_STRIDE_ALIGN) != 0) ? 1 :0);
        color_p += value * LV_DRAW_BUF_STRIDE_ALIGN;
    }
}
#endif

static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
    static lv_area_t damaged;
    static bool first_flush = true;

    uint8_t *fb = (uint8_t *)DEMO_BUFFER1_ADDR;
    uint8_t cf = lv_display_get_color_format(disp_drv);

#if LV_USE_DRAW_VGLITE
    uint8_t align_bytes = vglite_get_stride_alignment(cf);
    copy_area(area, color_p, fb, DEMO_BUFFER_STRIDE_BYTE, align_bytes);
#else
    copy_area(area, color_p, fb, DEMO_BUFFER_STRIDE_BYTE);
#endif

    if (first_flush == true)
    {
        lv_area_copy(&damaged, area);
    }
    else
    {
        _lv_area_join(&damaged, &damaged, area);
    }

    if (lv_disp_flush_is_last(disp_drv))
    {
        /*
         * Before new frame flushing, clear previous frame flush done status.
         */
#if !defined(SDK_OS_FREE_RTOS)
        s_transferDone = false;
#else
        (void)xSemaphoreTake(s_transferDone, 0);
#endif

        first_flush = true;

        if ((g_dc.ops->getProperty(&g_dc) & (uint32_t)kDC_FB_TwoDimensionMemoryWrite) == 0U)
        {
            /*
             * Change refresh region size.
             * For partial refresh, the whole line of the damaged area will be sent
             * to the panel, because low level data send functions does not support
             * sending non-continuous memory.
             */
            fbInfo.height = lv_area_get_height(&damaged);
            fbInfo.startY = damaged.y1;

            g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

            g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)(fb + fbInfo.startY * fbInfo.strideBytes));
        }
        else
        {
            /* Change refresh region size. Only updated part of each line is sent to the panel. */
            fbInfo.height = lv_area_get_height(&damaged);
            fbInfo.startY = damaged.y1;
            fbInfo.width  = lv_area_get_width(&damaged);
            fbInfo.startX = damaged.x1;

            g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

            g_dc.ops->setFrameBuffer(
                &g_dc, 0,
                (void *)(fb + fbInfo.startY * fbInfo.strideBytes + fbInfo.startX * DEMO_BUFFER_BYTE_PER_PIXEL));
        }
    }
    else
    {
        first_flush = false;

        /* IMPORTANT!!!
         * Inform the graphics library that you are ready with the flushing*/
        lv_display_flush_ready(disp_drv);
    }
}
#else
static void DEMO_FlushDisplay(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
#if DEMO_USE_ROTATE

    /*
     * Work flow:
     *
     * 1. Wait for the available inactive frame buffer to draw.
     * 2. Draw the ratated frame to inactive buffer.
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

    /* Use CPU to rotate the panel. */
    lv_draw_sw_rotate(color_p, inactiveFrameBuffer,
                      ROTATED_FB_WIDTH, ROTATED_FB_HEIGHT,
                      ROTATED_FB_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL,
                      DEMO_FB_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL,
                      LV_DISPLAY_ROTATION_270, disp_drv->color_format);

    g_dc.ops->setFrameBuffer(&g_dc, 0, inactiveFrameBuffer);

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready(disp_drv);

#else  /* DEMO_USE_ROTATE */

    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)color_p);

    DEMO_WaitBufferSwitchOff();

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready(disp_drv);
#endif /* DEMO_USE_ROTATE */
}
#endif

void lv_port_indev_init(void)
{
    /*Initialize your touchpad */
    DEMO_InitTouch();

    /*Register a touchpad input device*/
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, DEMO_ReadTouch);
}

#if ((DEMO_PANEL == DEMO_PANEL_RM67162) || (DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || (DEMO_PANEL_RK055MHD091 == DEMO_PANEL) || \
     (DEMO_PANEL == DEMO_PANEL_CO5300))
void BOARD_PullMIPIPanelTouchResetPin(bool pullUp)
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
#endif

#if (DEMO_PANEL == DEMO_PANEL_RM67162) || (DEMO_PANEL == DEMO_PANEL_CO5300)

void BOARD_TouchIntHandler(void)
{
    s_touchEvent = true;
}

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;

    const gpio_pin_config_t resetPinConfig  = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0};
    const gpio_pin_config_t intPinConfig    = {.pinDirection = kGPIO_DigitalInput, .outputLogic = 0};
    GPIO_PinInit(BOARD_MIPI_PANEL_TOUCH_RST_GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PIN, &resetPinConfig);
#if (DEMO_PANEL == DEMO_PANEL_RM67162)
    status = FT3267_Init(&s_touchHandle, &s_touchConfig);
#else
    status = TMA525B_Init(&s_touchHandle, &s_touchConfig);
#endif
    if (kStatus_Success != status)
    {
        PRINTF("Touch IC initialization failed\r\n");
        assert(false);
    }

    GPIO_SetPinInterruptConfig(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN, kGPIO_InterruptRisingEdge);

    GPIO_SetPinInterruptChannel(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN, kGPIO_InterruptOutput0);
    NVIC_SetPriority(BOARD_MIPI_TOUCH_INT_GPIO_IRQ, 1);
    EnableIRQ(GPIO10_IRQn);
    GPIO_PinInit(BOARD_MIPI_PANEL_TOUCH_INT_GPIO,  BOARD_MIPI_PANEL_TOUCH_INT_PIN, &intPinConfig);
}

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    data->state = LV_INDEV_STATE_REL;

    if (s_touchEvent)
    {
#if (DEMO_PANEL == DEMO_PANEL_RM67162)
        if (kStatus_Success == FT3267_GetSingleTouch(&s_touchHandle, &touch_event, &touch_x, &touch_y))
#else
        if (kStatus_Success == TMA525B_GetSingleTouch(&s_touchHandle, &touch_event, &touch_x, &touch_y))
#endif
        {
            if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
            {
                data->state = LV_INDEV_STATE_PR;
                touch_x     = LCD_WIDTH - touch_x;
                touch_y     = LCD_HEIGHT - touch_y;
            }
        }

        s_touchEvent = false;
    }

    /*Set the last pressed coordinates*/
    data->point.x = touch_x;
    data->point.y = touch_y;
}

#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
       (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))

static void BOARD_ConfigMIPIPanelTouchIntPin(gt911_int_pin_mode_t mode)
{
    if (mode == kGT911_IntPinInput)
    {
        BOARD_MIPI_PANEL_TOUCH_INT_GPIO->PDDR &= ~(1UL << BOARD_MIPI_PANEL_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN, 1);
        }
        else
        {
            GPIO_PinWrite(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN, 0);
        }

        BOARD_MIPI_PANEL_TOUCH_INT_GPIO->PDDR |= (1UL << BOARD_MIPI_PANEL_TOUCH_INT_PIN);
    }
}

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;

    const gpio_pin_config_t resetPinConfig = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0};

    GPIO_PinInit(BOARD_MIPI_PANEL_TOUCH_RST_GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PIN, &resetPinConfig);
    GPIO_PinInit(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN, &resetPinConfig);

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
#if DEMO_USE_ROTATE
    data->point.x = DEMO_PANEL_HEIGHT - (touch_y * DEMO_PANEL_HEIGHT / s_touchResolutionY);
    data->point.y = touch_x * DEMO_PANEL_WIDTH / s_touchResolutionX;
#else
    data->point.x = touch_x * DEMO_PANEL_WIDTH / s_touchResolutionX;
    data->point.y = touch_y * DEMO_PANEL_HEIGHT / s_touchResolutionY;
#endif
}

#else

static ft5406_rt_handle_t touch_handle;

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;
    lpi2c_master_config_t masterConfig = {0};

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
    masterConfig.baudRate_Hz = 100000U;

    /* Initialize the I2C master peripheral */
    LPI2C_MasterInit(BOARD_SSD1963_TOUCH_I2C_BASEADDR, &masterConfig, BOARD_SSD1963_TOUCH_I2C_CLOCK_FREQ);

    /* Initialize touch panel controller */
    status = FT5406_RT_Init(&touch_handle, BOARD_SSD1963_TOUCH_I2C_BASEADDR);
    if (status != kStatus_Success)
    {
        PRINTF("Touch panel init failed\n");
    }
}

/* Will be called by the library to read the touchpad */
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    data->state = LV_INDEV_STATE_REL;

    if (kStatus_Success == FT5406_RT_GetSingleTouch(&touch_handle, &touch_event, &touch_x, &touch_y))
    {
        if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
        {
            data->state = LV_INDEV_STATE_PR;
        }
    }

    /*Set the last pressed coordinates*/
    data->point.x = touch_x;
    data->point.y = touch_y;
}
#endif

void DEMO_CleanInvalidateCacheByAddr(void *addr, uint32_t size)
{
    XCACHE_CleanInvalidateCacheByRange((uint32_t)addr, size);
}
