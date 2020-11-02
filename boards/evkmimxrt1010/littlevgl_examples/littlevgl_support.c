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

#include "fsl_lpspi.h"
#include "fsl_lpi2c.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
#include "fsl_ili9341.h"
#include "fsl_ft6x06.h"
#include "fsl_lpspi_cmsis.h"
#include "fsl_lpi2c_cmsis.h"
#include "fsl_cache.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPSPI1_Freq CLOCK_GetFreq(kCLOCK_SysPllClk) / LPSPI_CLOCK_SOURCE_DIVIDER
#define LPI2C1_Freq CLOCK_GetFreq(kCLOCK_OscClk) / LPI2C_CLOCK_SOURCE_DIVIDER
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);
static void DEMO_InitTouch(void);
static bool DEMO_ReadTouch(lv_indev_drv_t *drv, lv_indev_data_t *data);
static void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static ft6x06_handle_t touch_handle;
static volatile uint32_t spi_event;
static volatile bool spi_event_received;
static ft6x06_handle_t touch_handle;
SDK_ALIGN(static uint8_t s_frameBuffer[1][LCD_VIRTUAL_BUF_SIZE * LCD_FB_BYTE_PER_PIXEL], 4);
/*******************************************************************************
 * Code
 ******************************************************************************/
void lv_port_pre_init(void)
{
}

/* Get LPSPI1_CLK_Freq (88MHz).  */
uint32_t LPSPI1_GetFreq(void)
{
    return LPSPI1_Freq;
}

/* Get LPI2C1_CLK_Freq (12MHz).  */
uint32_t LPI2C1_GetFreq(void)
{
    return LPI2C1_Freq;
}

static void SPI_MasterSignalEvent(uint32_t event)
{
    spi_event          = event;
    spi_event_received = true;
}

static uint32_t SPI_WaitEvent(void)
{
    uint32_t event;

    while (!spi_event_received)
    {
#if defined(FSL_RTOS_FREE_RTOS)
        vTaskDelay(1);
#endif
    }
    event              = spi_event;
    spi_event_received = false;

    return event;
}

static void APP_pfWrite8_A0(uint8_t Data)
{
    GPIO_PortClear(BOARD_LCD_DC_GPIO, 1u << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(&Data, 1);
    SPI_WaitEvent();
}

static void APP_pfWrite8_A1(uint8_t Data)
{
    GPIO_PortSet(BOARD_LCD_DC_GPIO, 1u << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(&Data, 1);
    SPI_WaitEvent();
}

static void APP_pfWriteM8_A1(uint8_t *pData, int NumItems)
{
    GPIO_PortSet(BOARD_LCD_DC_GPIO, 1u << BOARD_LCD_DC_GPIO_PIN);
    BOARD_LCD_SPI.Send(pData, NumItems);
    SPI_WaitEvent();
}

static void DEMO_InitLcd(void)
{
    /* Define the init structure for the data/command output pin */
    gpio_pin_config_t dc_config = {
        kGPIO_DigitalOutput,
        1,
        kGPIO_NoIntmode,
    };

    /* Init data/command GPIO output . */
    GPIO_PinInit(BOARD_LCD_DC_GPIO, BOARD_LCD_DC_GPIO_PIN, &dc_config);

    /* SPI master init */
    BOARD_LCD_SPI.Initialize(SPI_MasterSignalEvent);
    BOARD_LCD_SPI.PowerControl(ARM_POWER_FULL);
    BOARD_LCD_SPI.Control(ARM_SPI_MODE_MASTER, BOARD_LCD_SPI_BAUDRATE);

    FT9341_Init(APP_pfWrite8_A1, APP_pfWrite8_A0);
    APP_pfWrite8_A0(ILI9341_CMD_MAC);
    APP_pfWrite8_A1(0x28);
}

void lv_port_disp_init(void)
{
    static lv_disp_buf_t disp_buf;

    lv_disp_buf_init(&disp_buf, s_frameBuffer[0], NULL, LCD_VIRTUAL_BUF_SIZE);

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    lv_disp_drv_t disp_drv;      /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = my_disp_flush;

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

static void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_coord_t x1 = area->x1;
    lv_coord_t y1 = area->y1;
    lv_coord_t x2 = area->x2;
    lv_coord_t y2 = area->y2;

    uint8_t data[4];
    uint32_t send_size               = (x2 - x1 + 1) * (y2 - y1 + 1) * 2;
    /*Column addresses*/
    APP_pfWrite8_A0(ILI9341_CMD_COLADDR);
    data[0] = (x1 >> 8) & 0xFF;
    data[1] = x1 & 0xFF;
    data[2] = (x2 >> 8) & 0xFF;
    data[3] = x2 & 0xFF;
    APP_pfWriteM8_A1(data, 4);

    /*Page addresses*/
    APP_pfWrite8_A0(ILI9341_CMD_PAGEADDR);
    data[0] = (y1 >> 8) & 0xFF;
    data[1] = y1 & 0xFF;
    data[2] = (y2 >> 8) & 0xFF;
    data[3] = y2 & 0xFF;
    APP_pfWriteM8_A1(data, 4);

    /*Memory write*/
    APP_pfWrite8_A0(ILI9341_CMD_GRAM);

    APP_pfWriteM8_A1((void*)color_p, send_size);

    lv_disp_flush_ready(disp_drv);
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

void BOARD_Touch_InterfaceDeinit(void)
{
    BOARD_TOUCH_I2C.PowerControl(ARM_POWER_OFF);
    BOARD_TOUCH_I2C.Uninitialize();
}

static void I2C_MasterSignalEvent(uint32_t event)
{
    /* Notify touch driver about status of the IO operation */
    FT6X06_EventHandler(&touch_handle, event);
}

/*Initialize your touchpad*/
static void DEMO_InitTouch(void)
{
    status_t status;

    NVIC_SetPriority(BOARD_TOUCH_I2C_IRQ, 0);

    /*Init I2C1 */
    BOARD_TOUCH_I2C.Initialize(I2C_MasterSignalEvent);
    BOARD_TOUCH_I2C.PowerControl(ARM_POWER_FULL);
    BOARD_TOUCH_I2C.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);

    status = FT6X06_Init(&touch_handle, &BOARD_TOUCH_I2C);

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

    if (kStatus_Success == FT6X06_GetSingleTouch(&touch_handle, &touch_event, &touch_x, &touch_y))
    {
        if ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact))
        {
            data->state = LV_INDEV_STATE_PR;
        }
    }

    /*Set the last pressed coordinates*/
    data->point.x = LCD_WIDTH - touch_y;
    data->point.y = touch_x;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}
