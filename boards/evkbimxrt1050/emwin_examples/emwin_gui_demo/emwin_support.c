/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "GUI.h"
#include "WM.h"
#include "emwin_support.h"
#include "GUIDRV_Lin.h"

#include "fsl_debug_console.h"
#include "fsl_elcdif.h"
#include "fsl_lpi2c.h"
#include "fsl_ft5406_rt.h"

/*
**      Define everything necessary for different color depths
*/
#if LCD_BITS_PER_PIXEL == 8 /* Palette definition for LUT */
static uint32_t lutData[ELCDIF_LUT_ENTRY_NUM];
static const LCD_COLOR _aColors_256[] = {
    0x000000, 0x000033, 0x000066, 0x000099, 0x0000CC, 0x0000FF, 0x003300, 0x003333, 0x003366, 0x003399, 0x0033CC,
    0x0033FF, 0x006600, 0x006633, 0x006666, 0x006699, 0x0066CC, 0x0066FF, 0x009900, 0x009933, 0x009966, 0x009999,
    0x0099CC, 0x0099FF, 0x00CC00, 0x00CC33, 0x00CC66, 0x00CC99, 0x00CCCC, 0x00CCFF, 0x00FF00, 0x00FF33, 0x00FF66,
    0x00FF99, 0x00FFCC, 0x00FFFF, 0x330000, 0x330033, 0x330066, 0x330099, 0x3300CC, 0x3300FF, 0x333300, 0x333333,
    0x333366, 0x333399, 0x3333CC, 0x3333FF, 0x336600, 0x336633, 0x336666, 0x336699, 0x3366CC, 0x3366FF, 0x339900,
    0x339933, 0x339966, 0x339999, 0x3399CC, 0x3399FF, 0x33CC00, 0x33CC33, 0x33CC66, 0x33CC99, 0x33CCCC, 0x33CCFF,
    0x33FF00, 0x33FF33, 0x33FF66, 0x33FF99, 0x33FFCC, 0x33FFFF, 0x660000, 0x660033, 0x660066, 0x660099, 0x6600CC,
    0x6600FF, 0x663300, 0x663333, 0x663366, 0x663399, 0x6633CC, 0x6633FF, 0x666600, 0x666633, 0x666666, 0x666699,
    0x6666CC, 0x6666FF, 0x669900, 0x669933, 0x669966, 0x669999, 0x6699CC, 0x6699FF, 0x66CC00, 0x66CC33, 0x66CC66,
    0x66CC99, 0x66CCCC, 0x66CCFF, 0x66FF00, 0x66FF33, 0x66FF66, 0x66FF99, 0x66FFCC, 0x66FFFF, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x111111, 0x222222, 0x333333, 0x444444, 0x555555, 0x666666, 0x777777, 0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
    0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x990000, 0x990033, 0x990066, 0x990099, 0x9900CC, 0x9900FF,
    0x993300, 0x993333, 0x993366, 0x993399, 0x9933CC, 0x9933FF, 0x996600, 0x996633, 0x996666, 0x996699, 0x9966CC,
    0x9966FF, 0x999900, 0x999933, 0x999966, 0x999999, 0x9999CC, 0x9999FF, 0x99CC00, 0x99CC33, 0x99CC66, 0x99CC99,
    0x99CCCC, 0x99CCFF, 0x99FF00, 0x99FF33, 0x99FF66, 0x99FF99, 0x99FFCC, 0x99FFFF, 0xCC0000, 0xCC0033, 0xCC0066,
    0xCC0099, 0xCC00CC, 0xCC00FF, 0xCC3300, 0xCC3333, 0xCC3366, 0xCC3399, 0xCC33CC, 0xCC33FF, 0xCC6600, 0xCC6633,
    0xCC6666, 0xCC6699, 0xCC66CC, 0xCC66FF, 0xCC9900, 0xCC9933, 0xCC9966, 0xCC9999, 0xCC99CC, 0xCC99FF, 0xCCCC00,
    0xCCCC33, 0xCCCC66, 0xCCCC99, 0xCCCCCC, 0xCCCCFF, 0xCCFF00, 0xCCFF33, 0xCCFF66, 0xCCFF99, 0xCCFFCC, 0xCCFFFF,
    0xFF0000, 0xFF0033, 0xFF0066, 0xFF0099, 0xFF00CC, 0xFF00FF, 0xFF3300, 0xFF3333, 0xFF3366, 0xFF3399, 0xFF33CC,
    0xFF33FF, 0xFF6600, 0xFF6633, 0xFF6666, 0xFF6699, 0xFF66CC, 0xFF66FF, 0xFF9900, 0xFF9933, 0xFF9966, 0xFF9999,
    0xFF99CC, 0xFF99FF, 0xFFCC00, 0xFFCC33, 0xFFCC66, 0xFFCC99, 0xFFCCCC, 0xFFCCFF, 0xFFFF00, 0xFFFF33, 0xFFFF66,
    0xFFFF99, 0xFFFFCC, 0xFFFFFF

};
static const LCD_PHYSPALETTE _aPalette_256 = {ARRAY_SIZE(_aColors_256), _aColors_256};
#endif
#if LCD_BITS_PER_PIXEL < 32 /* Buffer definitions for emwin and LCD framebuffer */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_gui_memory[GUI_NUMBYTES * LCD_BYTES_PER_PIXEL], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_vram_buffer[VRAM_SIZE * GUI_BUFFERS * LCD_BYTES_PER_PIXEL], FRAME_BUFFER_ALIGN);
#else
uint32_t s_gui_memory[(GUI_NUMBYTES)];
uint32_t s_vram_buffer[VRAM_SIZE * GUI_BUFFERS];
#endif

/* Memory address definitions */
#define GUI_MEMORY_ADDR ((uint32_t)s_gui_memory)
#define VRAM_ADDR ((uint32_t)s_vram_buffer)

static volatile int32_t s_LCDpendingBuffer = -1;

/*******************************************************************************
 * Implementation of PortAPI for emWin LCD driver
 ******************************************************************************/

/* Enable interrupt. */
void BOARD_EnableLcdInterrupt(void)
{
    EnableIRQ(LCDIF_IRQn);
}

void APP_LCDIF_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = ELCDIF_GetInterruptStatus(APP_ELCDIF);

    ELCDIF_ClearInterruptStatus(APP_ELCDIF, intStatus);

    if (intStatus & kELCDIF_CurFrameDone)
    {
        if (s_LCDpendingBuffer >= 0)
        {
            /* Send a confirmation that the given buffer is visible */
            GUI_MULTIBUF_Confirm(s_LCDpendingBuffer);
            s_LCDpendingBuffer = -1;
        }
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void LCDIF_IRQHandler(void)
{
    APP_LCDIF_IRQHandler();
}

void APP_ELCDIF_Init(void)
{
    const elcdif_rgb_mode_config_t config = {
        .panelWidth = APP_IMG_WIDTH,
        .panelHeight = APP_IMG_HEIGHT,
        .hsw = APP_HSW,
        .hfp = APP_HFP,
        .hbp = APP_HBP,
        .vsw = APP_VSW,
        .vfp = APP_VFP,
        .vbp = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr = VRAM_ADDR,
        .pixelFormat = ELCDIF_PIXEL_FORMAT,
        .dataBus = APP_LCDIF_DATA_BUS,
    };
    ELCDIF_RgbModeInit(APP_ELCDIF, &config);

#if (LCD_BITS_PER_PIXEL == 8)
    /* Load the LUT data. */
    ELCDIF_UpdateLut(APP_ELCDIF, kELCDIF_Lut0, 0, lutData, ELCDIF_LUT_ENTRY_NUM);
    ELCDIF_EnableLut(APP_ELCDIF, true);
#endif
    BOARD_EnableLcdInterrupt();
    ELCDIF_EnableInterrupts(APP_ELCDIF, kELCDIF_CurFrameDoneInterruptEnable);
    NVIC_EnableIRQ(LCDIF_IRQn);
    ELCDIF_RgbModeStart(APP_ELCDIF);
}

/*******************************************************************************
 * Implementation of communication with the touch controller
 ******************************************************************************/

/* Touch driver handle. */
static ft5406_rt_handle_t touchHandle;

static void BOARD_Touch_Init(void)
{
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
    masterConfig.baudRate_Hz = BOARD_TOUCH_I2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(BOARD_TOUCH_I2C, &masterConfig, BOARD_TOUCH_I2C_CLOCK_FREQ);

    /* Initialize the touch handle. */
    FT5406_RT_Init(&touchHandle, BOARD_TOUCH_I2C);
}

void BOARD_Touch_Deinit(void)
{
    LPI2C_MasterDeinit(BOARD_TOUCH_I2C);
}

int BOARD_Touch_Poll(void)
{
    touch_event_t touch_event;
    int touch_x;
    int touch_y;
    GUI_PID_STATE pid_state;

    if (kStatus_Success != FT5406_RT_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
    {
        return 0;
    }
    else if (touch_event != kTouch_Reserved)
    {
        pid_state.x = touch_y;
        pid_state.y = touch_x;
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
    GUI_MULTIBUF_Config(GUI_BUFFERS);
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
    LCD_SetSizeEx(0, LCD_WIDTH, LCD_HEIGHT);
    LCD_SetVSizeEx(0, LCD_WIDTH, LCD_HEIGHT);
    LCD_SetVRAMAddrEx(0, (void *)VRAM_ADDR);
#if (LCD_BITS_PER_PIXEL == 8)
    LCD_SetLUT(&_aPalette_256);
#endif
    BOARD_Touch_Init();
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void *p)
{
    uint32_t addr;
#if (LCD_BITS_PER_PIXEL == 8)
    uint16_t colorR, colorG, colorB;
    uint32_t color;
#endif
    int result = 0;
    LCD_X_SHOWBUFFER_INFO *pData;
    switch (Cmd)
    {
        case LCD_X_INITCONTROLLER:
        {
            APP_ELCDIF_Init();
            break;
        }
        case LCD_X_SHOWBUFFER:
        {
            pData = (LCD_X_SHOWBUFFER_INFO *)p;
            /* Calculate address of the given buffer */
            addr = VRAM_ADDR + VRAM_SIZE * pData->Index;
            /* Make the given buffer visible */
            ELCDIF_SetNextBufferAddr(APP_ELCDIF, addr);
            //
            // Remember buffer index to be used by ISR
            //
            s_LCDpendingBuffer = pData->Index;
            while (s_LCDpendingBuffer >= 0)
                ;
            return 0;
        }
#if (LCD_BITS_PER_PIXEL == 8)
        case LCD_X_SETLUTENTRY:
        {
            //
            // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
            //
            LCD_X_SETLUTENTRY_INFO *pData;
            pData = (LCD_X_SETLUTENTRY_INFO *)p;
            //
            // Call hardware routine to write a LUT entry to the controller
            //
            color = pData->Color;
            colorB = (color & 0xFF0000) >> 16;
            colorG = (color & 0x00FF00) >> 8;
            colorR = (color & 0x0000FF);
            /* 16-bit bus */
            lutData[pData->Pos] = ((colorR >> 3) << 11) | ((colorG >> 2) << 5) | ((colorB >> 3) << 0);
            return 0;
        }
#endif
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

void *emWin_memcpy(void *pDst, const void *pSrc, long size)
{
    return memcpy(pDst, pSrc, size);
}
