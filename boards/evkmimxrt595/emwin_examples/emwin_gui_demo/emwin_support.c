/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "GUI.h"
#include "WM.h"
#include "emwin_support.h"
#include "GUIDRV_Lin.h"
#include "board.h"

#include "fsl_debug_console.h"

#if (DEMO_PANEL == DEMO_PANEL_RM67162)
#include "fsl_ft3267.h"
#elif (DEMO_PANEL == DEMO_PANEL_TFT_PROTO_5)
#include "fsl_i2c.h"
#include "fsl_ft5406.h"
#else
#include "fsl_gt911.h"
#endif

/*
**      Define everything necessary for different color depths
*/

/* Memory address definitions */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_gui_memory[GUI_NUMBYTES * LCD_BYTES_PER_PIXEL], FRAME_BUFFER_ALIGN);

#define GUI_MEMORY_ADDR ((uint32_t)s_gui_memory)
#define VRAM_ADDR       ((uint32_t)DEMO_BUFFER0_ADDR)

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
    fbInfo.startX      = DEMO_BUFFER_START_X;
    fbInfo.startY      = DEMO_BUFFER_START_Y;
    fbInfo.strideBytes = DEMO_BUFFER_STRIDE_BYTE;
    g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

    g_dc.ops->setCallback(&g_dc, 0, DEMO_BufferSwitchOffCallback, NULL);
}

/*******************************************************************************
 * Implementation of communication with the touch controller
 ******************************************************************************/
#if ((DEMO_PANEL == DEMO_PANEL_RM67162) || (DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))
static void BOARD_PullTouchPanelResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PORT, BOARD_MIPI_PANEL_TOUCH_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PORT, BOARD_MIPI_PANEL_TOUCH_RST_PIN, 0);
    }
}
#endif

#if (DEMO_PANEL == DEMO_PANEL_RM67162)
static void BOARD_TimeDelay(uint32_t ms);

static ft3267_handle_t s_touchHandle;
static volatile bool s_touchEvent = false;

static const ft3267_config_t s_touchConfig = {
    .I2C_ReceiveFunc  = BOARD_MIPIPanelTouch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullTouchPanelResetPin,
    .pullPowerPinFunc = NULL,
    .timeDelayMsFunc  = BOARD_TimeDelay,
};

static void BOARD_TimeDelay(uint32_t ms)
{
    SDK_DelayAtLeastUs(ms * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_TouchIntHandler(void)
{
    s_touchEvent = true;
}

static void BOARD_Touch_Init(void)
{
    status_t status;

    const gpio_pin_config_t resetPinConfig  = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0};
    const gpio_pin_config_t intPinConfig    = {.pinDirection = kGPIO_DigitalInput, .outputLogic = 0};
    gpio_interrupt_config_t intPinIntConfig = {kGPIO_PinIntEnableEdge, kGPIO_PinIntEnableLowOrFall};

    GPIO_PinInit(GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PORT, BOARD_MIPI_PANEL_TOUCH_RST_PIN, &resetPinConfig);

    status = FT3267_Init(&s_touchHandle, &s_touchConfig);

    if (kStatus_Success != status)
    {
        PRINTF("Touch IC initialization failed\r\n");
        assert(false);
    }

    GPIO_PinInit(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, BOARD_MIPI_PANEL_TOUCH_INT_PIN, &intPinConfig);
    GPIO_SetPinInterruptConfig(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, BOARD_MIPI_PANEL_TOUCH_INT_PIN, &intPinIntConfig);
    GPIO_PinEnableInterrupt(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, BOARD_MIPI_PANEL_TOUCH_INT_PIN, kGPIO_InterruptA);
}

void BOARD_Touch_Deinit(void)
{
    FT3267_Deinit(&s_touchHandle);
}

int BOARD_Touch_Poll(void)
{
    int ret = 0;
    touch_event_t touch_event;
    int touch_x;
    int touch_y;
    GUI_PID_STATE pid_state;

    if (s_touchEvent)
    {
        if (kStatus_Success != FT3267_GetSingleTouch(&s_touchHandle, &touch_event, &touch_x, &touch_y))
        {
            ret = 0;
        }
        else if (touch_event != kTouch_Reserved)
        {
            pid_state.x       = LCD_WIDTH - touch_x;
            pid_state.y       = LCD_HEIGHT - touch_y;
            pid_state.Pressed = ((touch_event == kTouch_Down) || (touch_event == kTouch_Contact));
            pid_state.Layer   = 0;
            GUI_TOUCH_StoreStateEx(&pid_state);

            ret = 1;
        }

        s_touchEvent = false;
    }

    return ret;
}

#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
       (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))
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
        GPIO->DIRCLR[BOARD_MIPI_PANEL_TOUCH_INT_PORT] = (1U << BOARD_MIPI_PANEL_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, BOARD_MIPI_PANEL_TOUCH_INT_PIN, 1);
        }
        else
        {
            GPIO_PinWrite(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, BOARD_MIPI_PANEL_TOUCH_INT_PIN, 0);
        }

        GPIO->DIRSET[BOARD_MIPI_PANEL_TOUCH_INT_PORT] = (1U << BOARD_MIPI_PANEL_TOUCH_INT_PIN);
    }
}

static void BOARD_Touch_Init(void)
{
    status_t status;
    const gpio_pin_config_t resetPinConfig = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0};

    GPIO_PinInit(GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PORT, BOARD_MIPI_PANEL_TOUCH_RST_PIN, &resetPinConfig);
    GPIO_PinInit(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, BOARD_MIPI_PANEL_TOUCH_INT_PIN, &resetPinConfig);

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
#else
static ft5406_handle_t touch_handle;

static void BOARD_Touch_Init(void)
{
    status_t status;
    i2c_master_config_t masterConfig;

    I2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Bps = 100000;

    /* Initialize the I2C master peripheral */
    I2C_MasterInit(BOARD_SSD1963_TOUCH_I2C_BASEADDR, &masterConfig, BOARD_SSD1963_TOUCH_I2C_CLOCK_FREQ);

    /* Initialize touch panel controller */
    status = FT5406_Init(&touch_handle, BOARD_SSD1963_TOUCH_I2C_BASEADDR);
    if (status != kStatus_Success)
    {
        PRINTF("Touch panel init failed\n");
    }
}

void BOARD_Touch_Deinit(void)
{
    FT5406_Denit(&touch_handle);
}

int BOARD_Touch_Poll(void)
{
    touch_event_t touch_event;
    int touch_x;
    int touch_y;
    static GUI_PID_STATE pid_state;
    static int isTouched;

    if (kStatus_Success != FT5406_GetSingleTouch(&touch_handle, &touch_event, &touch_x, &touch_y))
    {
        return 0;
    }
    else
    {
        if ((touch_event == kTouch_Contact))
        {
            pid_state.x       = touch_x;
            pid_state.y       = touch_y;
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
#endif

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
