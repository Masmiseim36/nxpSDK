/*
 * Copyright 2019-2021, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl_support_board.h"
#include "lvgl_support.h"
#include "lvgl.h"
#if defined(SDK_OS_FREE_RTOS)
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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitLcd(void);
static void DEMO_InitTouch(void);
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data);
static void my_disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static ft6x06_handle_t touch_handle;
static volatile uint32_t spi_event;
static volatile bool spi_event_received;
static ft6x06_handle_t touch_handle;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_frameBuffer[1][LCD_VIRTUAL_BUF_SIZE * LCD_FB_BYTE_PER_PIXEL], 4);
/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t LPSPI_GetFreq(void)
{
    return LPSPI_FREQ;
}

uint32_t LPI2C_GetFreq(void)
{
    return LPI2C_FREQ;
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
#if defined(SDK_OS_FREE_RTOS)
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
    BOARD_LCD_SPI.Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_DATA_BITS(8), BOARD_LCD_SPI_BAUDRATE);

    FT9341_Init(APP_pfWrite8_A1, APP_pfWrite8_A0);
    APP_pfWrite8_A0(ILI9341_CMD_MAC);
    APP_pfWrite8_A1(0x28);
}

void lv_port_disp_init(void)
{
    lv_display_t * disp_drv; /*Descriptor of a display driver*/
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    DEMO_InitLcd();

    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    disp_drv = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

    lv_display_set_buffers(disp_drv, s_frameBuffer[0], NULL, LCD_VIRTUAL_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp_drv, my_disp_flush);
}

static void my_disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *color_p)
{
    lv_coord_t x1 = area->x1;
    lv_coord_t y1 = area->y1;
    lv_coord_t x2 = area->x2;
    lv_coord_t y2 = area->y2;

    uint8_t data[4];
    uint32_t send_size = (x2 - x1 + 1) * (y2 - y1 + 1) * 2;

    /* Swap the 2 bytes of RGB565 color */
    lv_draw_sw_rgb565_swap(s_frameBuffer[0], (LCD_VIRTUAL_BUF_SIZE));

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

    APP_pfWriteM8_A1((void *)color_p, send_size);

    lv_disp_flush_ready(disp_drv);
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
static void DEMO_ReadTouch(lv_indev_t *drv, lv_indev_data_t *data)
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
}
