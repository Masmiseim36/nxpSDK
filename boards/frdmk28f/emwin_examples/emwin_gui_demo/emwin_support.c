/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "GUI.h"
#include "WM.h"
#include "GUIDRV_FlexColor.h"
#include "emwin_support.h"

#include "fsl_debug_console.h"
#include "fsl_gpio.h"

#include "fsl_ssd1963.h"
#include "fsl_dmamux.h"

#include "fsl_port.h"
#include "fsl_i2c.h"
#include "fsl_ft5406.h"

#ifndef GUI_MEMORY_ADDR
static uint32_t s_gui_memory[(GUI_NUMBYTES + 3) / 4]; /* needs to be word aligned */
#define GUI_MEMORY_ADDR ((uint32_t)s_gui_memory)
#endif

/*******************************************************************************
 * Implementation of PortAPI for emWin LCD driver
 ******************************************************************************/

/* SSD1963 LCD controller handle. */
ssd1963_handle_t lcdHandle;
/* The FlexIO MCU LCD device. */
FLEXIO_MCULCD_Type flexioLcdDev = {
    .flexioBase = BOARD_FLEXIO,
    .busType = kFLEXIO_MCULCD_8080,
    .dataPinStartIndex = BOARD_FLEXIO_DATA_PIN_START,
    .ENWRPinIndex = BOARD_FLEXIO_WR_PIN,
    .RDPinIndex = BOARD_FLEXIO_RD_PIN,
    .txShifterStartIndex = BOARD_FLEXIO_TX_START_SHIFTER,
    .txShifterEndIndex = BOARD_FLEXIO_TX_END_SHIFTER,
    .rxShifterStartIndex = BOARD_FLEXIO_RX_START_SHIFTER,
    .rxShifterEndIndex = BOARD_FLEXIO_RX_END_SHIFTER,
    .timerIndex = BOARD_FLEXIO_TIMER,
    .setCSPin = BOARD_SetCSPin,
    .setRSPin = BOARD_SetRSPin,
    .setRDWRPin = NULL /* Not used in 8080 mode. */
};

status_t BOARD_LCD_Init(void)
{
    status_t status;

    /* FlexIO MCU LCD EDMA handle. */
    static edma_handle_t rxEdmaHandle;
    static edma_handle_t txEdmaHandle;

    flexio_mculcd_config_t flexioMcuLcdConfig;

    const ssd1963_config_t ssd1963Config = {.pclkFreq_Hz = BOARD_SSD1963_PCLK_FREQ,
                                            .pixelInterface = kSSD1963_PixelInterface16Bit565,
                                            .panelDataWidth = kSSD1963_PanelData24Bit,
                                            .polarityFlags = BOARD_SSD1963_POLARITY_FLAG,
                                            .panelWidth = LCD_WIDTH,
                                            .panelHeight = LCD_HEIGHT,
                                            .hsw = BOARD_SSD1963_HSW,
                                            .hfp = BOARD_SSD1963_HFP,
                                            .hbp = BOARD_SSD1963_HBP,
                                            .vsw = BOARD_SSD1963_VSW,
                                            .vfp = BOARD_SSD1963_VFP,
                                            .vbp = BOARD_SSD1963_VBP};

    edma_config_t edmaConfig;

    /*
     * edmaConfig.enableContinuousLinkMode = false;
     * edmaConfig.enableHaltOnError = true;
     * edmaConfig.enableRoundRobinArbitration = false;
     * edmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(BOARD_FLEXIO_DMA, &edmaConfig);

    DMAMUX_Init(BOARD_FLEXIO_DMAMUX);

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

    DMAMUX_SetSource(BOARD_FLEXIO_DMAMUX, BOARD_FLEXIO_RX_DMA_CHANNEL, BOARD_FLEXIO_RX_DMA_REQUEST);
    DMAMUX_SetSource(BOARD_FLEXIO_DMAMUX, BOARD_FLEXIO_TX_DMA_CHANNEL, BOARD_FLEXIO_TX_DMA_REQUEST);
    DMAMUX_EnableChannel(BOARD_FLEXIO_DMAMUX, BOARD_FLEXIO_RX_DMA_CHANNEL);
    DMAMUX_EnableChannel(BOARD_FLEXIO_DMAMUX, BOARD_FLEXIO_TX_DMA_CHANNEL);

    EDMA_CreateHandle(&rxEdmaHandle, BOARD_FLEXIO_DMA, BOARD_FLEXIO_RX_DMA_CHANNEL);
    EDMA_CreateHandle(&txEdmaHandle, BOARD_FLEXIO_DMA, BOARD_FLEXIO_TX_DMA_CHANNEL);

    /* Reset the SSD1963 LCD controller. */
    GPIO_PinWrite(BOARD_SSD1963_RST_GPIO, BOARD_SSD1963_RST_PIN, 0);
    GUI_X_Delay(1); /* Delay at least 10ns. */
    GPIO_PinWrite(BOARD_SSD1963_RST_GPIO, BOARD_SSD1963_RST_PIN, 1);
    GUI_X_Delay(5); /* Delay at 5ms. */

    status =
        SSD1963_Init(&lcdHandle, &ssd1963Config, &flexioLcdDev, &txEdmaHandle, &rxEdmaHandle, BOARD_SSD1963_XTAL_FREQ);

    return status;
}

static void APP_pfWrite16_A0(U16 Data)
{
    FLEXIO_MCULCD_StartTransfer(lcdHandle.base);
    FLEXIO_MCULCD_WriteCommandBlocking(lcdHandle.base, Data);
    FLEXIO_MCULCD_StopTransfer(lcdHandle.base);
}

static void APP_pfWrite16_A1(U16 Data)
{
    FLEXIO_MCULCD_StartTransfer(lcdHandle.base);
    FLEXIO_MCULCD_WriteDataArrayBlocking(lcdHandle.base, &Data, 2);
    FLEXIO_MCULCD_StopTransfer(lcdHandle.base);
}

static void APP_pfWriteM16_A1(U16 *pData, int NumItems)
{
    FLEXIO_MCULCD_StartTransfer(lcdHandle.base);
    FLEXIO_MCULCD_WriteDataArrayBlocking(lcdHandle.base, pData, NumItems * 2);
    FLEXIO_MCULCD_StopTransfer(lcdHandle.base);
}

static U16 APP_pfRead16_A1(void)
{
    uint16_t Data;
#if defined(BOARD_LCD_READABLE) && (BOARD_LCD_READABLE == 0)
    PRINTF("Warning: LCD does not support read operation, the image may get distorted.\r\n");
    assert(0);
#endif
    FLEXIO_MCULCD_StartTransfer(lcdHandle.base);
    FLEXIO_MCULCD_ReadDataArrayBlocking(lcdHandle.base, &Data, 2);
    FLEXIO_MCULCD_StopTransfer(lcdHandle.base);
    return Data;
}

static void APP_pfReadM16_A1(U16 *pData, int NumItems)
{
#if defined(BOARD_LCD_READABLE) && (BOARD_LCD_READABLE == 0)
    PRINTF("Warning: LCD does not support read operation, the image may get distorted.\r\n");
    assert(0);
#endif
    FLEXIO_MCULCD_StartTransfer(lcdHandle.base);
    FLEXIO_MCULCD_ReadDataArrayBlocking(lcdHandle.base, pData, NumItems * 2);
    FLEXIO_MCULCD_StopTransfer(lcdHandle.base);
}

/*******************************************************************************
 * Implementation of communication with the touch controller
 ******************************************************************************/

/* Touch driver handle. */
static ft5406_handle_t touchHandle;

static void BOARD_Touch_Init(void)
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

void BOARD_Touch_Deinit(void)
{
    I2C_MasterDeinit(BOARD_TOUCH_I2C);
}

int BOARD_Touch_Poll(void)
{
    touch_event_t touch_event;
    int touch_x;
    int touch_y;
    GUI_PID_STATE pid_state;

    if (kStatus_Success != FT5406_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
    {
        return 0;
    }
    else if (touch_event != kTouch_Reserved)
    {
        pid_state.x = touch_x;
        pid_state.y = touch_y;
        pid_state.Pressed = ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact));
        pid_state.Layer = 0;
        GUI_TOUCH_StoreStateEx(&pid_state);
        return 1;
    }
    return 0;
}

/*******************************************************************************
 * Application implemented functions required by emWin library
 ******************************************************************************/
void LCD_X_Config(void)
{
    GUI_DEVICE *pDevice;
    GUI_PORT_API PortAPI;
    CONFIG_FLEXCOLOR Config = {0, 0, 0, 0, 1};

    pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_M565, 0, 0);
    GUIDRV_FlexColor_Config(pDevice, &Config);

    LCD_SetSizeEx(0, LCD_WIDTH, LCD_HEIGHT);
    LCD_SetVSizeEx(0, LCD_WIDTH, LCD_HEIGHT);

    PortAPI.pfWrite16_A0 = APP_pfWrite16_A0;
    PortAPI.pfWrite16_A1 = APP_pfWrite16_A1;
    PortAPI.pfWriteM16_A1 = APP_pfWriteM16_A1;
    PortAPI.pfRead16_A1 = APP_pfRead16_A1;
    PortAPI.pfReadM16_A1 = APP_pfReadM16_A1;
    GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66720, GUIDRV_FLEXCOLOR_M16C0B16);

    BOARD_Touch_Init();
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void *pData)
{
    int result = 0;

    switch (Cmd)
    {
        case LCD_X_INITCONTROLLER:
            BOARD_LCD_Init();
            SSD1963_StartDisplay(&lcdHandle);
            SSD1963_SetBackLight(&lcdHandle, 255);
            break;
        default:
            result = -1;
            break;
    }

    return result;
}

void GUI_X_Config(void)
{
    /* Assign work memory area to emWin */
    GUI_ALLOC_AssignMemory((void *)GUI_MEMORY_ADDR, GUI_NUMBYTES);

    /* Select default font */
    GUI_SetDefaultFont(GUI_FONT_6X8);
}

void GUI_X_Init(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_InitOS(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_Lock(void)
{
}

/* Dummy RTOS stub required by emWin */
void GUI_X_Unlock(void)
{
}

/* Dummy RTOS stub required by emWin */
U32 GUI_X_GetTaskId(void)
{
    return 0;
}

void GUI_X_ExecIdle(void)
{
}

GUI_TIMER_TIME GUI_X_GetTime(void)
{
    return 0;
}

void GUI_X_Delay(int Period)
{
    volatile int i;
    for (; Period > 0; Period--)
    {
        for (i = 15000; i > 0; i--)
            ;
    }
}
