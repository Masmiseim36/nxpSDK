/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "GUI.h"
#include "WM.h"
#include "emwin_support.h"
#include "GUIDRV_Lin.h"
#include "board.h"
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
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_gui_memory[GUI_NUMBYTES], 4U);

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

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        assert(0);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &fbInfo);
    fbInfo.pixelFormat = DEMO_BUFFER_PIXEL_FORMAT;
    fbInfo.width       = DEMO_PANEL_WIDTH;
    fbInfo.height      = DEMO_PANEL_HEIGHT;
#if (DEMO_PANEL == DEMO_PANEL_TFT_PROTO_5 || DEMO_PANEL == DEMO_PANEL_CO5300)
    fbInfo.strideBytes = DEMO_BUFFER_STRIDE_BYTE;
#elif (DEMO_PANEL == DEMO_PANEL_RM67162)
    fbInfo.strideBytes = DEMO_BUFFER_WIDTH * DEMO_BUFFER_BYTE_PER_PIXEL;
#endif
    g_dc.ops->setLayerConfig(&g_dc, 0, &fbInfo);

    g_dc.ops->setCallback(&g_dc, 0, DEMO_BufferSwitchOffCallback, NULL);
}

/*******************************************************************************
 * Implementation of communication with the touch controller
 ******************************************************************************/
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
static void BOARD_ConfigMIPIPanelTouchIntPin(gt911_int_pin_mode_t mode);
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
int BOARD_Touch_Poll(void)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;
    static int isTouched;
    GUI_PID_STATE pid_state;

    pid_state.Layer = 0;

    if (s_touchEvent)
    {
#if (DEMO_PANEL == DEMO_PANEL_RM67162)
        if (kStatus_Success == FT3267_GetSingleTouch(&s_touchHandle, &touch_event, &touch_x, &touch_y))
#else
        if (kStatus_Success == TMA525B_GetSingleTouch(&s_touchHandle, &touch_event, &touch_x, &touch_y))
#endif
        {
            if (touch_event != kTouch_Reserved)
            {
                pid_state.x = LCD_WIDTH - touch_x;
                pid_state.y = LCD_HEIGHT - touch_y;

            }
            if ((touch_event == kTouch_Contact) || (touch_event == kTouch_Down))
            {
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
        else
        {
            return 0;
        }

        s_touchEvent = false;
    }

    return 1;
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

void DEMO_Touch_Deinit(void)
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
int BOARD_Touch_Poll(void)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;
    GUI_PID_STATE pid_state;
    static int isTouched;
    pid_state.Layer = 0;

    if (kStatus_Success != FT5406_RT_GetSingleTouch(&touch_handle, &touch_event, &touch_x, &touch_y))
    {
        return 0;
    }
    else
    {
        if (touch_event != kTouch_Reserved)
        {
            pid_state.x       = touch_x;
            pid_state.y       = touch_y;
        }
        if ((touch_event == kTouch_Contact) || (touch_event == kTouch_Down))
        {
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
    memset((void *)DEMO_BUFFER0_ADDR, 0, VRAM_SIZE);
    memset((void *)DEMO_BUFFER1_ADDR, 0, VRAM_SIZE);
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    BOARD_PrepareDisplayController();
    DEMO_InitTouch();
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
