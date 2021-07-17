/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "touch.h"
#include "display_support.h"
#if defined(FSL_RTOS_FREE_RTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif
#include "board.h"


#include "fsl_gpio.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

#if (DEMO_PANEL == DEMO_PANEL_RM67162)
#include "fsl_ft3267.h"
#elif (DEMO_PANEL == DEMO_PANEL_TFT_PROTO_5)
#include "fsl_i2c.h"
#include "fsl_ft5406.h"
#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))
#include "fsl_gt911.h"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if ((DEMO_PANEL == DEMO_PANEL_RM67162) || (DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))
void BOARD_PullMIPIPanelTouchResetPin(bool pullUp);
#endif

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))
static void BOARD_ConfigMIPIPanelTouchIntPin(gt911_int_pin_mode_t mode);
#endif


/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (DEMO_PANEL == DEMO_PANEL_RM67162)
static ft3267_handle_t s_touchHandle;
static volatile bool s_touchEvent = false;

static const ft3267_config_t s_touchConfig = {
    .I2C_ReceiveFunc  = BOARD_MIPIPanelTouch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullMIPIPanelTouchResetPin,
    .pullPowerPinFunc = NULL,
    .timeDelayMsFunc  = VIDEO_DelayMs,
};

#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))
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


/*******************************************************************************
 * Code
 ******************************************************************************/
#if ((DEMO_PANEL == DEMO_PANEL_RM67162) || (DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))
void BOARD_PullMIPIPanelTouchResetPin(bool pullUp)
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
void BOARD_TouchIntHandler(void)
{
    s_touchEvent = true;
}

/*Initialize your touchpad*/
int BOARD_Touch_Init(void)
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
	 return kStatus_Success;
}

void BOARD_Touch_DeInit(void)
{
	FT3267_Deinit(&s_touchHandle);
}



/* Will be called by the library to read the touchpad */
int BOARD_Touch_Poll(touch_poll_state_t *output)
{
    status_t status;
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    if(s_touchEvent == true) {
    	s_touchEvent = false;
    	status = FT3267_GetSingleTouch(&s_touchHandle, &touch_event, &touch_x, &touch_y);
    	if (kStatus_Success != status)
    		return status;

    	if (touch_event == kTouch_Reserved)
    		return 0;

    	/*Set the last pressed coordinates*/
    	output->x       = touch_x;
    	output->y       = touch_y;
    	output->pressed = (touch_event == kTouch_Down || touch_event == kTouch_Contact);
        return kStatus_Success;
    }


    return kStatus_Timeout;
}

#elif ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))

void BOARD_ConfigMIPIPanelTouchIntPin(gt911_int_pin_mode_t mode)
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

/*Initialize your touchpad*/
int BOARD_Touch_Init(void)
{
    status_t status;

    const gpio_pin_config_t resetPinConfig = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0};

    GPIO_PinInit(GPIO, BOARD_MIPI_PANEL_TOUCH_RST_PORT, BOARD_MIPI_PANEL_TOUCH_RST_PIN, &resetPinConfig);
    GPIO_PinInit(GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PORT, BOARD_MIPI_PANEL_TOUCH_INT_PIN, &resetPinConfig);

    status = GT911_Init(&s_touchHandle, &s_touchConfig);

    if (kStatus_Success != status)
    {
        PRINTF("Touch IC initialization failed\r\n");
        return status;
    }

    GT911_GetResolution(&s_touchHandle, &s_touchResolutionX, &s_touchResolutionY);
    return kStatus_Success;
}

void BOARD_Touch_DeInit() {
	GT911_Deinit(&s_touchHandle);
}

/* Will be called by the library to read the touchpad */
int BOARD_Touch_Poll(touch_poll_state_t *output)
{
    static int touch_x = 0;
    static int touch_y = 0;

    if (kStatus_Success == GT911_GetSingleTouch(&s_touchHandle, &touch_x, &touch_y))
    {
        output->x       = touch_x;
        output->y       = touch_y;
        output->pressed = 1;
    }
    else
    {
        output->x       = touch_x;
        output->y       = touch_y;
        output->pressed = 0;
    }

    /*Return `false` because we are not buffering and no more data to read*/
    return kStatus_Success;
}

#else

static ft5406_handle_t touch_handle;

/*Initialize your touchpad*/
int BOARD_Touch_Init(void)
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
        return status;
    }
    return kStatus_Success;
}

void BOARD_Touch_DeInit(void)
{
	 FT5406_Denit(&touch_handle);
}

/* Will be called by the library to read the touchpad */
int BOARD_Touch_Poll(touch_poll_state_t *output)
{
    touch_event_t touch_event;
    static int touch_x = 0;
    static int touch_y = 0;

    FT5406_GetSingleTouch(&touch_handle, &touch_event, &touch_x, &touch_y);

    /*Set the last pressed coordinates*/
    output->x       = touch_x;
    output->y       = touch_y;
    output->pressed = (touch_event == kTouch_Down || touch_event == kTouch_Contact);

    return kStatus_Success;
}
#endif
