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

#include "board.h"
#include "fsl_gpio.h"
#include "fsl_ssd1963.h"
#include "fsl_dbi_flexio_edma.h"
#include "fsl_flexio_mculcd.h"
#include "fsl_i2c.h"
#include "fsl_ft5406.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Port Me. Start */
#define BOARD_SSD1963_XTAL_FREQ     10000000U
#define BOARD_SSD1963_PCLK_FREQ     30000000U
#define BOARD_SSD1963_HSW           48U
#define BOARD_SSD1963_HFP           40U
#define BOARD_SSD1963_HBP           0U
#define BOARD_SSD1963_VSW           3U
#define BOARD_SSD1963_VFP           13U
#define BOARD_SSD1963_VBP           18U
#define BOARD_SSD1963_POLARITY_FLAG 0U

/* Macros for FlexIO interfacing the LCD */
#define BOARD_FLEXIO              FLEXIO0
#define BOARD_FLEXIO_CLOCK_FREQ   CLOCK_GetCoreSysClkFreq()
#define BOARD_FLEXIO_BAUDRATE_BPS 160000000U

/* Macros for FlexIO shifter, timer, and pins. */
#define BOARD_FLEXIO_WR_PIN           7
#define BOARD_FLEXIO_RD_PIN           8
#define BOARD_FLEXIO_DATA_PIN_START   16
#define BOARD_FLEXIO_TX_START_SHIFTER 0
#define BOARD_FLEXIO_RX_START_SHIFTER 0
#define BOARD_FLEXIO_TX_END_SHIFTER   7
#define BOARD_FLEXIO_RX_END_SHIFTER   7
#define BOARD_FLEXIO_TIMER            0

/* Macros for the touch touch controller. */
#define BOARD_TOUCH_I2C            I2C1
#define BOARD_TOUCH_I2C_CLOCK_FREQ CLOCK_GetBusClkFreq()
#define BOARD_TOUCH_I2C_BAUDRATE   100000U
/* Port Me. End */

#define DEMO_MS_TO_TICK(ms) ((ms * configTICK_RATE_HZ / 1000) + 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);

static void DEMO_InitLcdClock(void);

static status_t DEMO_InitLcdController(void);

static void DEMO_FlushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

static void DEMO_InitTouch(void);

static bool DEMO_ReadTouch(lv_indev_drv_t *drv, lv_indev_data_t *data);

static void DEMO_SetCSPin(bool set);

static void DEMO_SetRSPin(bool set);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static ft5406_handle_t touchHandle;
#if defined(FSL_RTOS_FREE_RTOS)
static SemaphoreHandle_t s_memWriteDone;
#else
static volatile bool s_memWriteDone;
#endif

/* SSD1963 LCD controller handle. */
ssd1963_handle_t lcdHandle;
/* DBI XFER handle. */
dbi_flexio_edma_xfer_handle_t g_dbiFlexioXferHandle;
/* The FlexIO MCU LCD device. */
FLEXIO_MCULCD_Type flexioLcdDev = {
    .flexioBase          = BOARD_FLEXIO,
    .busType             = kFLEXIO_MCULCD_8080,
    .dataPinStartIndex   = BOARD_FLEXIO_DATA_PIN_START,
    .ENWRPinIndex        = BOARD_FLEXIO_WR_PIN,
    .RDPinIndex          = BOARD_FLEXIO_RD_PIN,
    .txShifterStartIndex = BOARD_FLEXIO_TX_START_SHIFTER,
    .txShifterEndIndex   = BOARD_FLEXIO_TX_END_SHIFTER,
    .rxShifterStartIndex = BOARD_FLEXIO_RX_START_SHIFTER,
    .rxShifterEndIndex   = BOARD_FLEXIO_RX_END_SHIFTER,
    .timerIndex          = BOARD_FLEXIO_TIMER,
    .setCSPin            = DEMO_SetCSPin,
    .setRSPin            = DEMO_SetRSPin,
    .setRDWRPin          = NULL /* Not used in 8080 mode. */
};

SDK_ALIGN(static uint8_t s_frameBuffer[2][LCD_VIRTUAL_BUF_SIZE * LCD_FB_BYTE_PER_PIXEL], 4);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void DEMO_SetCSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, (uint8_t)set);
}

static void DEMO_SetRSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, (uint8_t)set);
}

static void DEMO_DbiMemoryDoneCallback(status_t status, void *userData)
{
#if defined(FSL_RTOS_FREE_RTOS)
    BaseType_t taskAwake = pdFALSE;

    xSemaphoreGiveFromISR(s_memWriteDone, &taskAwake);

    portYIELD_FROM_ISR(taskAwake);
#else
    s_memWriteDone = true;
#endif
}

/* Clear the LCD controller video memory content. */
static void DEMO_ClearLcd(void)
{
    int32_t leftLinesToClear = LCD_HEIGHT;
    int32_t curLinesToClear;
    int32_t startLine = 0;

    while (leftLinesToClear > 0)
    {
        curLinesToClear =
            (leftLinesToClear > (2 * LCD_VIRTUAL_BUF_HEIGHT)) ? (2 * LCD_VIRTUAL_BUF_HEIGHT) : leftLinesToClear;

        SSD1963_SelectArea(&lcdHandle, 0, startLine, LCD_WIDTH - 1, startLine + curLinesToClear - 1);

#if !defined(FSL_RTOS_FREE_RTOS)
        s_memWriteDone = false;
#endif

        SSD1963_WriteMemory(&lcdHandle, (const uint8_t *)s_frameBuffer,
                            curLinesToClear * LCD_WIDTH * LCD_FB_BYTE_PER_PIXEL);

#if defined(FSL_RTOS_FREE_RTOS)
        if (xSemaphoreTake(s_memWriteDone, portMAX_DELAY) != pdTRUE)
        {
            PRINTF("Wait semaphore error: s_memWriteDone\r\n");
            assert(0);
        }
#else
        while (false == s_memWriteDone)
        {
        }
#endif

        startLine += curLinesToClear;
        leftLinesToClear -= curLinesToClear;
    }
}

status_t DEMO_InitLcdController(void)
{
    status_t status;

    flexio_mculcd_config_t flexioMcuLcdConfig;

    const ssd1963_config_t ssd1963Config = {.pclkFreq_Hz    = BOARD_SSD1963_PCLK_FREQ,
                                            .pixelInterface = kSSD1963_RGB565,
                                            .panelDataWidth = kSSD1963_PanelData24Bit,
                                            .polarityFlags  = BOARD_SSD1963_POLARITY_FLAG,
                                            .panelWidth     = LCD_WIDTH,
                                            .panelHeight    = LCD_HEIGHT,
                                            .hsw            = BOARD_SSD1963_HSW,
                                            .hfp            = BOARD_SSD1963_HFP,
                                            .hbp            = BOARD_SSD1963_HBP,
                                            .vsw            = BOARD_SSD1963_VSW,
                                            .vfp            = BOARD_SSD1963_VFP,
                                            .vbp            = BOARD_SSD1963_VBP};

    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

    /* Set SSD1963 CS, RS, and reset pin to output. */
    GPIO_PinInit(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, &pinConfig);
    GPIO_PinInit(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, &pinConfig);
    GPIO_PinInit(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, &pinConfig);

    /* Initialize the flexio MCU LCD. */
    /*
     * flexioMcuLcdConfig.enable = true;
     * flexioMcuLcdConfig.enableInDoze = false;
     * flexioMcuLcdConfig.enableInDebug = true;
     * flexioMcuLcdConfig.enableFastAccess = true;
     * flexioMcuLcdConfig.baudRate_Bps = 96000000U;
     */
    FLEXIO_MCULCD_GetDefaultConfig(&flexioMcuLcdConfig);
    flexioMcuLcdConfig.baudRate_Bps = BOARD_FLEXIO_BAUDRATE_BPS;

    status = FLEXIO_MCULCD_Init(&flexioLcdDev, &flexioMcuLcdConfig, BOARD_FLEXIO_CLOCK_FREQ);

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Create the DBI XFER handle. Because DMA transfer is not used, so don't
     * need to create DMA handle.
     */
    status = DBI_FLEXIO_EDMA_CreateXferHandle(&g_dbiFlexioXferHandle, &flexioLcdDev, NULL, NULL);

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Reset the SSD1963 LCD controller. */
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 0);
#if defined(FSL_RTOS_FREE_RTOS)
    vTaskDelay(DEMO_MS_TO_TICK(1)); /* Delay at least 10ns. */
#else
    SDK_DelayAtLeastUs(1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, 1);
#if defined(FSL_RTOS_FREE_RTOS)
    vTaskDelay(DEMO_MS_TO_TICK(5)); /* Delay at 5ms. */
#else
    SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif

    status =
        SSD1963_Init(&lcdHandle, &ssd1963Config, &g_dbiFlexioEdmaXferOps, &g_dbiFlexioXferHandle, BOARD_SSD1963_XTAL_FREQ);

    if (status == kStatus_Success)
    {
        SSD1963_SetMemoryDoneCallback(&lcdHandle, DEMO_DbiMemoryDoneCallback, NULL);

        /* Clear the SSD1963 video ram. */
        DEMO_ClearLcd();

        SSD1963_StartDisplay(&lcdHandle);

        SSD1963_SetBackLight(&lcdHandle, 128);
    }
    else
    {
        PRINTF("LCD controller initialization failed.\r\n");
    }

    return status;
}

static void DEMO_InitLcdClock(void)
{
    /* Use system clock as FlexIO clock source. */
    CLOCK_SetFlexio0Clock(0U);
}

static void DEMO_InitLcd(void)
{
#if defined(FSL_RTOS_FREE_RTOS)
    s_memWriteDone = xSemaphoreCreateBinary();
    if (NULL == s_memWriteDone)
    {
        PRINTF("Frame semaphore create failed\r\n");
        assert(0);
    }
#else
    s_memWriteDone = false;
#endif

    DEMO_InitLcdClock();
    DEMO_InitLcdController();
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void DEMO_FlushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_coord_t x1 = area->x1;
    lv_coord_t y1 = area->y1;
    lv_coord_t x2 = area->x2;
    lv_coord_t y2 = area->y2;

    int32_t length = (x2 - x1 + 1) * (y2 - y1 + 1) * LCD_FB_BYTE_PER_PIXEL;

    SSD1963_SelectArea(&lcdHandle, x1, y1, x2, y2);

#if !defined(FSL_RTOS_FREE_RTOS)
    s_memWriteDone = false;
#endif

    SSD1963_WriteMemory(&lcdHandle, (const uint8_t *)color_p, length);

#if defined(FSL_RTOS_FREE_RTOS)
    if (xSemaphoreTake(s_memWriteDone, portMAX_DELAY) != pdTRUE)
    {
        PRINTF("Wait semaphore error: s_memWriteDone\r\n");
        assert(0);
    }
#else
    while (!s_memWriteDone)
    {
    }
#endif

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

void lv_port_pre_init(void)
{
}

void lv_port_disp_init(void)
{
    static lv_disp_buf_t disp_buf;

    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));
    lv_disp_buf_init(&disp_buf, (void *)s_frameBuffer[0], (void *)s_frameBuffer[1], LCD_VIRTUAL_BUF_SIZE);

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

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

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
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
    i2c_master_config_t masterConfig;

    /*
     * masterConfig.enableMaster = true;
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.enableTimeout = false;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Bps = BOARD_TOUCH_I2C_BAUDRATE;

    /* Initialize the I2C master peripheral */
    I2C_MasterInit(BOARD_TOUCH_I2C, &masterConfig, BOARD_TOUCH_I2C_CLOCK_FREQ);

    /* Initialize the touch handle. */
    FT5406_Init(&touchHandle, BOARD_TOUCH_I2C);
}

/* Will be called by the library to read the touchpad */
static bool DEMO_ReadTouch(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    data->state = LV_INDEV_STATE_REL;

    if (kStatus_Success == FT5406_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
    {
        if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
        {
            data->state = LV_INDEV_STATE_PR;
        }
    }

    /*Set the last pressed coordinates*/
    data->point.y = touch_y;
    data->point.x = touch_x;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}
