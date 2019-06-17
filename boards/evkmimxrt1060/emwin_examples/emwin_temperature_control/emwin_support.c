/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "GUI.h"
#include "WM.h"
#include "emwin_support.h"
#include "GUIDRV_Lin.h"

#include "fsl_debug_console.h"
#include "fsl_elcdif.h"
#include "fsl_lpi2c.h"
#include "fsl_ft5406_rt.h"
#include "fsl_gpt.h"

/*
**      Define everything necessary for different color depths
*/
#if LCD_BITS_PER_PIXEL == 8 /* Palette definition for LUT */
static uint32_t lutData[ELCDIF_LUT_ENTRY_NUM];
static const LCD_COLOR _aColors_256[] = {
    0xff000000, 0xff330000, 0xff660000, 0xff990000, 0xffcc0000, 0xffff0000, 0xff003300, 0xff333300, 0xff663300,
    0xff993300, 0xffcc3300, 0xffff3300, 0xff006600, 0xff336600, 0xff666600, 0xff996600, 0xffcc6600, 0xffff6600,
    0xff009900, 0xff339900, 0xff669900, 0xff999900, 0xffcc9900, 0xffff9900, 0xff00cc00, 0xff33cc00, 0xff66cc00,
    0xff99cc00, 0xffcccc00, 0xffffcc00, 0xff00ff00, 0xff33ff00, 0xff66ff00, 0xff99ff00, 0xffccff00, 0xffffff00,
    0xff000033, 0xff330033, 0xff660033, 0xff990033, 0xffcc0033, 0xffff0033, 0xff003333, 0xff333333, 0xff663333,
    0xff993333, 0xffcc3333, 0xffff3333, 0xff006633, 0xff336633, 0xff666633, 0xff996633, 0xffcc6633, 0xffff6633,
    0xff009933, 0xff339933, 0xff669933, 0xff999933, 0xffcc9933, 0xffff9933, 0xff00cc33, 0xff33cc33, 0xff66cc33,
    0xff99cc33, 0xffcccc33, 0xffffcc33, 0xff00ff33, 0xff33ff33, 0xff66ff33, 0xff99ff33, 0xffccff33, 0xffffff33,
    0xff000066, 0xff330066, 0xff660066, 0xff990066, 0xffcc0066, 0xffff0066, 0xff003366, 0xff333366, 0xff663366,
    0xff993366, 0xffcc3366, 0xffff3366, 0xff006666, 0xff336666, 0xff666666, 0xff996666, 0xffcc6666, 0xffff6666,
    0xff009966, 0xff339966, 0xff669966, 0xff999966, 0xffcc9966, 0xffff9966, 0xff00cc66, 0xff33cc66, 0xff66cc66,
    0xff99cc66, 0xffcccc66, 0xffffcc66, 0xff00ff66, 0xff33ff66, 0xff66ff66, 0xff99ff66, 0xffccff66, 0xffffff66,
    0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
    0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff111111, 0xff222222, 0xff333333, 0xff444444, 0xff555555,
    0xff666666, 0xff777777, 0xff888888, 0xff999999, 0xffaaaaaa, 0xffbbbbbb, 0xffcccccc, 0xffdddddd, 0xffeeeeee,
    0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000,
    0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000099, 0xff330099, 0xff660099, 0xff990099, 0xffcc0099,
    0xffff0099, 0xff003399, 0xff333399, 0xff663399, 0xff993399, 0xffcc3399, 0xffff3399, 0xff006699, 0xff336699,
    0xff666699, 0xff996699, 0xffcc6699, 0xffff6699, 0xff009999, 0xff339999, 0xff669999, 0xff999999, 0xffcc9999,
    0xffff9999, 0xff00cc99, 0xff33cc99, 0xff66cc99, 0xff99cc99, 0xffcccc99, 0xffffcc99, 0xff00ff99, 0xff33ff99,
    0xff66ff99, 0xff99ff99, 0xffccff99, 0xffffff99, 0xff0000cc, 0xff3300cc, 0xff6600cc, 0xff9900cc, 0xffcc00cc,
    0xffff00cc, 0xff0033cc, 0xff3333cc, 0xff6633cc, 0xff9933cc, 0xffcc33cc, 0xffff33cc, 0xff0066cc, 0xff3366cc,
    0xff6666cc, 0xff9966cc, 0xffcc66cc, 0xffff66cc, 0xff0099cc, 0xff3399cc, 0xff6699cc, 0xff9999cc, 0xffcc99cc,
    0xffff99cc, 0xff00cccc, 0xff33cccc, 0xff66cccc, 0xff99cccc, 0xffcccccc, 0xffffcccc, 0xff00ffcc, 0xff33ffcc,
    0xff66ffcc, 0xff99ffcc, 0xffccffcc, 0xffffffcc, 0xff0000ff, 0xff3300ff, 0xff6600ff, 0xff9900ff, 0xffcc00ff,
    0xffff00ff, 0xff0033ff, 0xff3333ff, 0xff6633ff, 0xff9933ff, 0xffcc33ff, 0xffff33ff, 0xff0066ff, 0xff3366ff,
    0xff6666ff, 0xff9966ff, 0xffcc66ff, 0xffff66ff, 0xff0099ff, 0xff3399ff, 0xff6699ff, 0xff9999ff, 0xffcc99ff,
    0xffff99ff, 0xff00ccff, 0xff33ccff, 0xff66ccff, 0xff99ccff, 0xffccccff, 0xffffccff, 0xff00ffff, 0xff33ffff,
    0xff66ffff, 0xff99ffff, 0xffccffff, 0xffffffff};
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
#define VRAM_ADDR (SDK_SIZEALIGN(s_vram_buffer, FRAME_BUFFER_ALIGN))

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
        .panelWidth    = APP_IMG_WIDTH,
        .panelHeight   = APP_IMG_HEIGHT,
        .hsw           = APP_HSW,
        .hfp           = APP_HFP,
        .hbp           = APP_HBP,
        .vsw           = APP_VSW,
        .vfp           = APP_VFP,
        .vbp           = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr    = VRAM_ADDR,
        .pixelFormat   = ELCDIF_PIXEL_FORMAT,
        .dataBus       = APP_LCDIF_DATA_BUS,
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
    static GUI_PID_STATE pid_state;
    static int isTouched;

    if (kStatus_Success != FT5406_RT_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y))
    {
        return 0;
    }
    else
    {
        if ((touch_event == kTouch_Contact))
        {
            pid_state.x       = touch_y;
            pid_state.y       = touch_x;
            pid_state.Pressed = 1;
            GUI_TOUCH_StoreStateEx(&pid_state);
            isTouched = 1;
        }
        else if (isTouched && (touch_event == kTouch_Up))
        {
            isTouched         = 0;
            pid_state.Pressed = 0;
            GUI_TOUCH_StoreStateEx(&pid_state);
        }
    }
    return 1;
}

/*********************************************************************
 *
 *       _ClearCache
 */
static void _ClearCache(U32 v)
{
    GUI_USE_PARA(v);
    SCB_CleanInvalidateDCache();
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
    GUI_DCACHE_SetClearCacheHook(_ClearCache);
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void *p)
{
    GUI_USE_PARA(LayerIndex);

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
            color  = pData->Color;
            colorR = (color & 0xFF0000) >> 16;
            colorG = (color & 0x00FF00) >> 8;
            colorB = (color & 0x0000FF);
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
    GUI_X_Delay(1);
}

GUI_TIMER_TIME GUI_X_GetTime(void)
{
    return GPT_GetCurrentTimerCount(EXAMPLE_GPT) / EXAMPLE_GPT_TICK_TO_MS;
}

void GUI_X_Delay(int Period)
{
    volatile uint32_t tNow = GPT_GetCurrentTimerCount(EXAMPLE_GPT);
    while ((GPT_GetCurrentTimerCount(EXAMPLE_GPT) - tNow) < Period * EXAMPLE_GPT_TICK_TO_MS)
        ;
}

void *emWin_memcpy(void *pDst, const void *pSrc, long size)
{
    return memcpy(pDst, pSrc, size);
}
