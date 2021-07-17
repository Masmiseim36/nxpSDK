/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "GUI.h"
#include "WM.h"
#include "emwin_support.h"
#include "GUIDRV_Lin.h"
#include "board.h"
#include "fsl_gt911.h"
#include "fsl_debug_console.h"

/*
**      Define everything necessary for different color depths
*/

/* Memory address definitions */
/* The GUI memory should be 4 byte aligned, otherwise there is
 * unaligned access fault. Because emwin library stores data to
 * the memory using `strd` (store double world), the memory
 * must be 4-byte aligned.
 */
SDK_ALIGN(uint8_t s_gui_memory[GUI_NUMBYTES], 4U);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_vram_buffer[VRAM_SIZE * GUI_BUFFERS], FRAME_BUFFER_ALIGN);

#define GUI_MEMORY_ADDR ((uint32_t)s_gui_memory)
#define VRAM_ADDR       ((uint32_t)s_vram_buffer)

static volatile int32_t s_LCDpendingBuffer = -1;

/*******************************************************************************
 * Implementation of PortAPI for emWin LCD driver
 ******************************************************************************/

void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    if (s_LCDpendingBuffer >= 0)
    {
        /* Send a confirmation that the given buffer is visible */
        GUI_MULTIBUF_Confirm(s_LCDpendingBuffer);
        s_LCDpendingBuffer = -1;
    }
    else if (s_LCDpendingBuffer == -2)
    {
        /* This is the initial buffer to display */
        s_LCDpendingBuffer = -1;
    }
}

void APP_InitDisplay(void)
{
    status_t status;
    dc_fb_info_t fbInfo;

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
    fbInfo.width       = DEMO_BUFFER_WIDTH;
    fbInfo.height      = DEMO_BUFFER_HEIGHT;
    fbInfo.strideBytes = DEMO_BUFFER_STRIDE_BYTE;
    g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

    g_dc.ops->setCallback(&g_dc, 0, DEMO_BufferSwitchOffCallback, NULL);
}

/*******************************************************************************
 * Implementation of communication with the touch controller
 ******************************************************************************/
static void BOARD_PullTouchPanelResetPin(bool pullUp)
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

static void BOARD_ConfigMIPIPanelTouchIntPin(gt911_int_pin_mode_t mode);

static gt911_handle_t s_touchHandle;
static const gt911_config_t s_touchConfig = {
    .I2C_SendFunc     = BOARD_MIPIPanelTouch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_MIPIPanelTouch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullTouchPanelResetPin,
    .intPinFunc       = BOARD_ConfigMIPIPanelTouchIntPin,
    .timeDelayMsFunc  = VIDEO_DelayMs,
    .touchPointNum    = 1,
    .i2cAddrMode      = kGT911_I2cAddrAny,
    .intTrigMode      = kGT911_IntRisingEdge,
};
static int s_touchResolutionX;
static int s_touchResolutionY;

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

static void BOARD_Touch_Init(void)
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

void BOARD_Touch_Deinit(void)
{
    GT911_Deinit(&s_touchHandle);
}

int BOARD_Touch_Poll(void)
{
    int ret;
    static int touch_x              = 0;
    static int touch_y              = 0;
    static bool isPressedPreviously = false;
    GUI_PID_STATE pid_state;

    pid_state.Layer = 0;

    if (kStatus_Success == GT911_GetSingleTouch(&s_touchHandle, &touch_x, &touch_y))
    {
        touch_x             = touch_x * LCD_WIDTH / s_touchResolutionX;
        touch_y             = touch_y * LCD_HEIGHT / s_touchResolutionY;
        pid_state.Pressed   = true;
        isPressedPreviously = true;
        ret                 = 1;
    }
    else
    {
        if (isPressedPreviously)
        {
            pid_state.Pressed   = false;
            isPressedPreviously = false;
            ret                 = 1;
        }
        else
        {
            ret = 0;
        }
    }

    if (ret == 1)
    {
        pid_state.x = touch_x;
        pid_state.y = touch_y;
        GUI_TOUCH_StoreStateEx(&pid_state);
    }

    return ret;
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
    BOARD_Touch_Init();
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void *p)
{
    uint32_t addr;
    int result = 0;
    LCD_X_SHOWBUFFER_INFO *pData;
    switch (Cmd)
    {
        case LCD_X_INITCONTROLLER:
        {
            APP_InitDisplay();
            break;
        }

        case LCD_X_ON:
        {
            /*
             * The frame buffer is already cleared by GUI stack, so send it
             * directly.
             */
            s_LCDpendingBuffer = -2;
            g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)VRAM_ADDR);

            /* Wait for frame buffer sent to display controller video memory. */
            if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
            {
                while (s_LCDpendingBuffer == -2)
                {
                }
            }

            g_dc.ops->enableLayer(&g_dc, 0);

            break;
        }

        case LCD_X_SHOWBUFFER:
        {
            pData              = (LCD_X_SHOWBUFFER_INFO *)p;
            s_LCDpendingBuffer = pData->Index;
            /* Calculate address of the given buffer */
            addr = VRAM_ADDR + VRAM_SIZE * pData->Index;
            /* Make the given buffer visible */
            g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)addr);

            while (s_LCDpendingBuffer >= 0)
            {
            }
            return 0;
        }
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
