/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* LED implementation file for ARM CORTEX-M4 processor
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */
#include "LED.h"
#include "fsl_common.h"
#include "TimersManager.h"
#include "GPIO_Adapter.h"
#include "gpio_pins.h"
#include "pin_mux.h"
#include "fsl_os_abstraction.h"

#ifndef BOARD_USE_PWM_FOR_RGB_LED
#define BOARD_USE_PWM_FOR_RGB_LED 0
#endif

#if BOARD_USE_PWM_FOR_RGB_LED
#include "TMR_Adapter.h"
#endif

#if defined(gLEDSupported_d) && (gLEDSupported_d > 0U)

/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
******************************************************************************/
#if gLedRgbEnabled_d
#define PWM_MAX_DUTY_CYCLE_c            LED_MAX_RGB_VALUE_c    
#define PWM_INIT_VAL_c                  0x00U     /* init value */
#define PWM_FREQ                        190000u  /* 187500u */
#endif

#if BOARD_USE_PWM_FOR_RGB_LED && !(defined(gLpmIncluded_d)&&(gLpmIncluded_d > 0))

/* RGB LED - 3 PWM signals for controlling color */
/* RGB LED SET value */
#define RGB_SET_LED_RED(x)              PWM_SetChnCountVal(gRedLedPwmTimerInstance_c, gRedLedPwmTimerChannel_c, (x))
#define RGB_SET_LED_GREEN(x)            PWM_SetChnCountVal(gGreenLedPwmTimerInstance_c, gGreenLedPwmTimerChannel_c, (x))
#define RGB_SET_LED_BLUE(x)             PWM_SetChnCountVal(gBlueLedPwmTimerInstance_c, gBlueLedPwmTimerChannel_c, (x))

#endif



#if ((!BOARD_USE_PWM_FOR_RGB_LED) || (BOARD_USE_PWM_FOR_RGB_LED && (defined(gLpmIncluded_d)&&(gLpmIncluded_d > 0))))
#define gRGB_Threashold_c               60U
#define RGB_SET_LED_RED(x)              ((x) > gRGB_Threashold_c)?(LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gRedLedIdx_c], gLedOn_c)):(LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gRedLedIdx_c], gLedOff_c))
#define RGB_SET_LED_GREEN(x)            ((x) > gRGB_Threashold_c)?(LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gGreenLedIdx_c], gLedOn_c)):(LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gGreenLedIdx_c], gLedOff_c))
#define RGB_SET_LED_BLUE(x)             ((x) > gRGB_Threashold_c)?(LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gBlueLedIdx_c], gLedOn_c)):(LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gBlueLedIdx_c], gLedOff_c))
#endif


#define gLedId_LastActiveId_c           (0x01U << (gLEDsOnTargetBoardCnt_c-1U))
/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/

/*
* Name: LED_DimMode_t
* Description: RGB LED dimming modes
*/
typedef enum LED_DimMode_tag {
  gLedDimIn_c,
  gLedDimOut_c,
  gLedDimMax_c
} LED_DimMode_t;

/*
* Name: LED_RgbDimInfo_t
* Description: RGB LED dimming information structure
*/
typedef struct LED_RgbDimInfo_tag
{
  LED_DimMode_t mode;     /* dim in/out */
  uint8_t interval;       /* in seconds */
  uint16_t steps;         /* dimming steps */
  uint16_t value[3];      /* current values for RGB LED */
  uint16_t increment[3];  /* increment/decrement values for RGB LED */
  bool_t ongoing;         /* TRUE if RGB LED dimming is ongoing */
} LED_RgbDimInfo_t;

/******************************************************************************
*******************************************************************************
* Public memory definitions
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private memory definitions
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private function prototypes
*******************************************************************************
******************************************************************************/
static void LED_GpioSet(const gpioOutputPinConfig_t* pOutputConfig, LED_OpMode_t operation);
static void LED_HwInit(void);
#if gTMR_Enabled_d
static void LED_FlashTimeout(void *param);
#endif
#if gLedRgbEnabled_d
static void LED_RgbLedInit(void);
static void LED_ToggleRgbLed(LED_t LEDNr);
#endif

#if gLedColorWheelEnabled_d && gLedRgbEnabled_d
static void LED_ColorWheelSet(LED_t LEDNr);
#else
    #if gLEDBlipEnabled_d
    static void LED_DecrementBlip(void);
    #endif
#endif
    
#if gTMR_Enabled_d && gLedRgbEnabled_d && gRgbLedDimmingEnabled_d
static void LED_DimmingTimeout(void *param);
static bool_t LED_StartRgbDimming(LED_DimMode_t mode, uint8_t interval);
#endif

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/
typedef struct ledColor_tag
{
  uint8_t redValue;
  uint8_t greenValue;
  uint8_t blueValue;
}ledColor_t;
/******************************************************************************
*******************************************************************************
* Private Memory Declarations
*******************************************************************************
******************************************************************************/
/* Serial Mode: indicates if the LED module is in serial mode or not */
static bool_t mfLedInSerialMode;
/* Flashing Mode: indicates how many LEDs are in flashing mode */
static uint8_t mLedFlashingLEDs = 0;  /* flashing LEDs */
static uint8_t mLedStartFlashingPosition = LED1;
#if gTMR_Enabled_d
/* LED timer ID */
static tmrTimerID_t mLEDTimerID = gTmrInvalidTimerID_c;
#endif
#if gLedRgbEnabled_d
/* last positive RGB values */
static uint8_t mLedRgbLastValues[3] = {0, 0, 0};
#if gTMR_Enabled_d && gRgbLedDimmingEnabled_d
static tmrTimerID_t mRGBLedTimerID = gTmrInvalidTimerID_c;
#endif /* gTMR_Enabled_d */
#endif
#if (gLEDBlipEnabled_d) && (!gLedColorWheelEnabled_d || !gLedRgbEnabled_d)
/* array that stores the LEDs blip information*/
static uint8_t mLedBlips[gLEDsOnTargetBoardCnt_c];
#endif

#if gLedColorWheelEnabled_d && gLedRgbEnabled_d
static uint8_t gColorWheelIdx = 0x00;
#if (defined(BOARD_USE_PWM_FOR_RGB_LED)&&(BOARD_USE_PWM_FOR_RGB_LED>0U)) && !(defined(gLpmIncluded_d)&&(gLpmIncluded_d > 0))
static const ledColor_t gColorWheel[] = {   {255, 0, 0}, /* red */ {255, 128, 0}, /* orange */ {255, 255, 0}, /* yellow */
                                    {128, 255, 0}, /* yellow green */ {0, 255, 0}, /* green */ { 0, 255, 255}, /* blue green */
                                    {0, 0, 255}, /* blue */ { 102, 102, 255}, /* blue violet */{255, 0, 255}, /* violet */
                                    {102, 0, 204}, /* mauves */{ 102, 0, 102}, /* mauves pink */{255, 51, 153}, /* pink */
                                };
#else
extern const ledColor_t gColorWheel[];
const ledColor_t gColorWheel[] = { {255, 0, 0}, /* red */   {255, 255, 0}, /* yellow */
                                 {0, 255, 0}, /* green */ { 0, 255, 255}, /* blue green */
                                 {0, 0, 255}, /* blue */  {255, 0, 255}, /* violet */
                                 {255, 255, 255}, /* white */
                               };
#endif
#endif

#if gTMR_Enabled_d && gLedRgbEnabled_d && gRgbLedDimmingEnabled_d
/*
 * Name: mRbgDimInfo
 * Description: RGB dimming info structure. Contains dimming related information.
 */
static LED_RgbDimInfo_t mRbgDimInfo;
#endif

/******************************************************************************
******************************************************************************
* Public functions
******************************************************************************
*****************************************************************************/

/******************************************************************************
* Name: LED_Init
* Description: Initialize the LED module
* Parameters: -
* Return: -
******************************************************************************/
void LED_Init
(
    void
)
{
    LED_HwInit();

    /* allocate a timer for use in flashing LEDs */
#if gTMR_Enabled_d
    mLEDTimerID = TMR_AllocateTimer();
#endif
    
#if gLedRgbEnabled_d && gRgbLedDimmingEnabled_d && gTMR_Enabled_d
    /* allocate a timer for use in RGB dimming */
    mRGBLedTimerID = TMR_AllocateTimer();
    mRbgDimInfo.ongoing = FALSE;
    mRbgDimInfo.interval = gRgbLedDimDefaultInterval_c;    
#endif /* gLedRgbEnabled_d && gRgbLedDimmingEnabled_d && gTMR_Enabled_d */
}

/******************************************************************************
* Name: Un-initialize the LED module
* Description: turns off all the LEDs and disables clock gating for LED port
* Parameters: -
* Return: -
******************************************************************************/
void LED_UnInit
(
void
)
{

#if gTMR_Enabled_d
    /* free the timer used for flashing mode */
    (void)TMR_FreeTimer(mLEDTimerID);
#endif
    
#if gTMR_Enabled_d && gLedRgbEnabled_d && gRgbLedDimmingEnabled_d    
    (void)TMR_FreeTimer(mRGBLedTimerID);
#endif
    
    /* turn off LED */
    LED_TurnOffAllLeds();
}

/******************************************************************************
* Name: LED_Operate
* Description: Basic LED operation: ON, OFF, TOGGLE.
* Param(s): -
* Return: -
******************************************************************************/
void LED_Operate
(
    LED_t led,
    LED_OpMode_t operation
)
{

#if gLedRgbEnabled_d
    if((led & LED_RGB) == LED_RGB)
    {
        switch(operation)
        {
            case gLedOn_c:
                LED_SetRgbLed(LED_RGB, PWM_MAX_DUTY_CYCLE_c, PWM_MAX_DUTY_CYCLE_c, PWM_MAX_DUTY_CYCLE_c);
                break;
            case gLedOff_c:
                LED_SetRgbLed(LED_RGB, 0U, 0U, 0U);
                break;
            case gLedToggle_c:
                LED_ToggleRgbLed(LED_RGB);
                break;
            default:
            	/*MISRA rule 16.4*/
                break;
        }
        
        led &= ~(uint8_t)(LED_RGB);
    }
#endif /* gLedRgbEnabled_d */

#if gLEDsOnTargetBoardCnt_c > 0
    if((bool)(led & LED1))
    {
      LED_GpioSet(ledPins, operation);
      
    }
#endif
#if gLEDsOnTargetBoardCnt_c > 1
    if((bool)(led & LED2))
    {
      LED_GpioSet(ledPins + 1, operation);
    }
#endif
#if gLEDsOnTargetBoardCnt_c > 2
    if((bool)(led & LED3))
    {
        LED_GpioSet(ledPins + 2, operation);
    }
#endif
#if gLEDsOnTargetBoardCnt_c > 3
    if((bool)(led & LED4))
    {
        LED_GpioSet(ledPins + 3, operation);
    }
#endif
}

/******************************************************************************
* Name: LED_TurnOnLed
* Description: Turns ON the specified LED(s)
* Parameters: LEDNr - LED number(s) to be turned ON
* Return:
******************************************************************************/
void LED_TurnOnLed
(
    LED_t LEDNr
)
{
    LED_Operate(LEDNr, gLedOn_c);  /* turn ON LEDs */
}

/******************************************************************************
* Name: LED_TurnOffLed
* Description: Turns OFF the specified LED(s)
* Parameters: LEDNr - LED number(s) to be turned ON
* Return:
******************************************************************************/
void LED_TurnOffLed
(
    LED_t LEDNr
)
{
    LED_Operate(LEDNr, gLedOff_c);  /* turn OFF LEDs */
}

/******************************************************************************
* Name: LED_ToggleLed
* Description: Toggles the specified LED(s)
* Parameters:  LEDNr - LED number(s) to be toggled
* Return: -
******************************************************************************/
void LED_ToggleLed
(
    LED_t LEDNr
)
{
    LED_Operate(LEDNr, gLedToggle_c);  /* toggle LEDs */
}

/******************************************************************************
* Name: LED_TurnOffAllLeds
* Description: Turns OFF all LEDs
* Parameters: -
* Return: -
******************************************************************************/
void LED_TurnOffAllLeds
(
void
)
{
    LED_TurnOffLed(LED_ALL);
}

/******************************************************************************
* Name: LED_TurnOnAllLeds
* Description: Turns ON all LEDs
* Parameters: -
* Return: -
******************************************************************************/
void LED_TurnOnAllLeds
(
void
)
{
    LED_TurnOnLed(LED_ALL);
}

/******************************************************************************
* Name: LED_StopFlashingAllLeds
* Description: Stops flashing and turns OFF all LEDs
* Parameters: -
* Return: -
******************************************************************************/
void LED_StopFlashingAllLeds(void)
{
    LED_SetLed(LED_ALL, (uint8_t)gLedOff_c);
}

#if (gLEDBlipEnabled_d) && (!gLedColorWheelEnabled_d || !gLedRgbEnabled_d)
/******************************************************************************
* Name: LED_StartBlip
* Description: Set up for blinking one or more LEDs once
* Parameters: [IN] LED_t LEDNr - LED Number (may be an OR of the list)
* Return: -
******************************************************************************/
void LED_StartBlip
(
    LED_t LEDNr
)
{
    uint8_t iLedIndex;

    /* set up for blinking one or more LEDs once */
    for(iLedIndex = 0; iLedIndex < gLEDsOnTargetBoardCnt_c; ++iLedIndex) 
    {
        if((bool)(LEDNr & (1u << iLedIndex)))
        {
            mLedBlips[iLedIndex] = 2;   /* blink on, then back off */
        }
    }

    /* start flashing */
    LED_StartFlash(LEDNr);
}
#endif

/******************************************************************************
* Name: LED_StopFlash
* Description: Stop an LED from flashing.
* Parameters: [IN] LED_t LEDNr - LED Number (may be an OR of the list)
* Return: -
******************************************************************************/
void LED_StopFlash
(
    LED_t LEDNr
)
{
    /* leave stopped LEDs in the off state */
    LED_TurnOffLed(LEDNr);

    /* stop flashing on one or more LEDs */
    mLedFlashingLEDs &= (uint8_t)(~LEDNr);

#if gTMR_Enabled_d
    /* if ALL LEDs have stopped flashing, then stop timer */
    if(mLedFlashingLEDs == 0u)
    {
        (void)TMR_StopTimer(mLEDTimerID);
    }
#endif
}

/******************************************************************************
* Name: LED_SetHex
* Description: Sets a specified hex value on the LEDs
* Parameters: [IN] hexValue - the value to be set on LEDs
* Return: -
******************************************************************************/
void LED_SetHex
(
uint8_t hexValue
)
{
    LED_SetLed(LED_ALL, (uint8_t)gLedOff_c);
    LED_SetLed(hexValue, (uint8_t)gLedOn_c);
}
/******************************************************************************
* Name: LED_SetLed
* Description: This function can set the specified LED(s) in one of the
*              following states: On, Off, Toggle, Flashing or StopFlash
* Parameters: [IN] LEDNr - LED(s) to
*             [IN] LedState_t state - one of the possible states listed above
* Return:
******************************************************************************/
void LED_SetLed
(
    LED_t LEDNr,
    LedState_t state
)
{
    /* turning off flashing same as off state */
    if(state == (uint8_t)gLedStopFlashing_c)
    {
        state = (uint8_t)gLedOff_c;
    }

    #if gLedColorWheelEnabled_d && gLedRgbEnabled_d
    if(LED_RGB == LEDNr)
    {
        gColorWheelIdx = 0x00;
    }
    #endif   
    
    /* turn off serial lights if in serial mode */
    LED_StopSerialFlash();

    /* flash LED */
    if(state == (uint8_t)gLedFlashing_c)
    {
        LED_StartFlash(LEDNr);
    }
#if (gLEDBlipEnabled_d) && (!gLedColorWheelEnabled_d || !gLedRgbEnabled_d)
    /* blink LEDs */
    else if(state == (uint8_t)gLedBlip_c)
    {
        LED_StartBlip(LEDNr);
    }
#endif
    else
    {
        /* on, off or toggle will stop flashing on the LED. */
        if((bool)(mLedFlashingLEDs & LEDNr))
        {
            LED_StopFlash(LEDNr);
        }
        /* Select the operation to be done on the port */
        if(state == (uint8_t)gLedOn_c)
        {
             LED_TurnOnLed(LEDNr);
        }
        if(state == (uint8_t)gLedOff_c)
        {
             LED_TurnOffLed(LEDNr);
        }
        if(state == (uint8_t)gLedToggle_c)
        {
             LED_ToggleLed(LEDNr);
        }
    }
}

/******************************************************************************
* Name: LED_SetRgbLed
* Description: This function is used to control the RGB LED
* Param(s): [in]    LEDNr - led identifier
            [in]    redValue - red value  from 0 to PWM_MODULE_MAX_DUTY_CYCLE_c
            [in]    greenValue - green value from 0 to PWM_MODULE_MAX_DUTY_CYCLE_c
            [in]    blueValue - blue value from 0 to PWM_MODULE_MAX_DUTY_CYCLE_c
* Return: - None
******************************************************************************/
void LED_SetRgbLed
(
    LED_t LEDNr,
    uint16_t redValue,
    uint16_t greenValue,
    uint16_t blueValue
)
{
#if gLedRgbEnabled_d
   if(LEDNr == LED_RGB)
   {
        /* max value = PWM_MAX_DUTY_CYCLE_c */
        if((redValue <= PWM_MAX_DUTY_CYCLE_c) &&
           (greenValue <= PWM_MAX_DUTY_CYCLE_c) &&
           (blueValue <= PWM_MAX_DUTY_CYCLE_c)) 
        {
            /* keep last values */
            if((bool)redValue || (bool)greenValue || (bool)blueValue)
            {
                mLedRgbLastValues[0] = (uint8_t)redValue;
                mLedRgbLastValues[1] = (uint8_t)greenValue;
                mLedRgbLastValues[2] = (uint8_t)blueValue;
            }

#if gLED_InvertedMode_d && BOARD_USE_PWM_FOR_RGB_LED
            redValue   = PWM_MAX_DUTY_CYCLE_c - redValue;
            greenValue = PWM_MAX_DUTY_CYCLE_c - greenValue;
            blueValue  = PWM_MAX_DUTY_CYCLE_c - blueValue;
#endif

            /* set color */
            RGB_SET_LED_RED(redValue);
            RGB_SET_LED_GREEN(greenValue);
            RGB_SET_LED_BLUE(blueValue);          
        }
   }
#else
   LEDNr=LEDNr;
   redValue=redValue;
   greenValue=greenValue;
   blueValue=blueValue;
#endif
}

/******************************************************************************
* Name: LED_RgbDimOut
* Description: Dim out the RGB LED
* Param(s): None    
* Return: TRUE if no dimming is ongoing and the command has been accepted,
*         FALSE otherwise
******************************************************************************/
bool_t LED_RgbDimOut(void)
{
#if gTMR_Enabled_d && gLedRgbEnabled_d && gRgbLedDimmingEnabled_d  
  return LED_StartRgbDimming(gLedDimOut_c, mRbgDimInfo.interval);
#else
  return FALSE;
#endif
}

/******************************************************************************
* Name: LED_RgbDimIn
* Description: Dim in the RGB LED
* Param(s): None    
* Return: TRUE if no dimming is ongoing and the command has been accepted,
*         FALSE otherwise
******************************************************************************/
bool_t LED_RgbDimIn(void)
{
#if gTMR_Enabled_d && gLedRgbEnabled_d && gRgbLedDimmingEnabled_d
  return LED_StartRgbDimming(gLedDimIn_c, mRbgDimInfo.interval);
#else
  return FALSE;  
#endif
}

/******************************************************************************
* Name: LED_RgbSetDimInterval
* Description: Dim (fade) in the RGB LED
* Param(s): dim_interval - the dimming interval
* Return: None
******************************************************************************/
void LED_RgbSetDimInterval(uint8_t dim_interval)
{
#if gTMR_Enabled_d && gLedRgbEnabled_d && gRgbLedDimmingEnabled_d  
    if(dim_interval <= gRgbLedDimMaxInterval_c)
    {
        mRbgDimInfo.interval = dim_interval;
    }    
#else
    (void)dim_interval;
#endif  
}

 /******************************************************************************
* Name: LED_StartFlashWithPeriod
* Description: Starts flashing one or more LEDs
* Parameters: [IN] LED_t LEDNr          - LED Number (may be an OR of the list)
              [IN] uint16_t periodMs    - LED flashing period in milliseconds
* Return: -
******************************************************************************/
void LED_StartFlashWithPeriod
(
    LED_t LEDNr,
    uint16_t periodMs
)
{
    /* indicate which LEDs are flashing */
    mLedFlashingLEDs |= LEDNr;

#if gTMR_Enabled_d
    /* start the timer */
    if(!TMR_IsTimerActive(mLEDTimerID))
    {
        (void)TMR_StartIntervalTimer(mLEDTimerID, (uint32_t)periodMs, (pfTmrCallBack_t)LED_FlashTimeout, NULL);
    }
#else
    #warning "The TIMER component is not enabled and therefore the LED flashing function is disabled"
#endif
}
/******************************************************************************
* Name: LED_StartSerialFlashWithPeriod
* Description: starts serial flashing LEDs
* Parameters: [IN] LEDStartPosition - LED start position
*             [IN] periodMs - flashing period in milliseconds
* Return: -
******************************************************************************/
void LED_StartSerialFlashWithPeriod
(
    uint8_t ledStartPosition,
    uint16_t periodMs
)
{
    /* indicate going into flashing mode (must be done first) */
    LED_StartFlashWithPeriod(ledStartPosition, periodMs);

#if gLedRgbEnabled_d
    if(ledStartPosition > gLedId_LastActiveId_c)
    {
        ledStartPosition = LED_RGB;
    }
#else
    if(ledStartPosition > gLedId_LastActiveId_c)
    {
        ledStartPosition = LED1;
    }
#endif
    mLedStartFlashingPosition   = ledStartPosition;

    /* set up for serial lights */
    LED_TurnOffAllLeds();
    LED_TurnOnLed(mLedStartFlashingPosition);
    mLedFlashingLEDs = mLedStartFlashingPosition | (uint8_t)((uint16_t)mLedStartFlashingPosition << 1U); /* toggle these to on first flash */

    /* indicate in serial flash mode */
    mfLedInSerialMode = TRUE;
}
/******************************************************************************
* Name: LED_StopSerialFlash
* Description: Stop serial flashing mode, if serial flash mode is active.
*              Turns all LEDs off.
* Param(s): -
* Return: -
******************************************************************************/
void LED_StopSerialFlash
(
    void
)
{
    if(mfLedInSerialMode)
    {
        /* stop flashing timer */
        (void)TMR_StopTimer(mLEDTimerID);

        /* stop flashing on one or more LEDs */
        mLedFlashingLEDs = 0;
        mfLedInSerialMode = FALSE;

        /* leave stopped LEDs in the off state */
        LED_TurnOffAllLeds();

        mLedStartFlashingPosition = LED1;
    }
}
/******************************************************************************
* Name: LED_StartColorWheel
* Description: starts color wheel
* Parameters: [IN] LEDNr - LED Number (may be an OR of the list)
*             [IN] periodMs - period to the next color in milliseconds
* Return: -
******************************************************************************/
void LED_StartColorWheel
(
    LED_t LEDNr,
    uint16_t periodMs
)
{
#if gLedColorWheelEnabled_d && gLedRgbEnabled_d
    LED_ColorWheelSet(LEDNr);
    LED_StartFlashWithPeriod(LEDNr, periodMs);
#else
    LEDNr=LEDNr;
    periodMs=periodMs;
#endif

}
/******************************************************************************
*******************************************************************************
* Private functions
*******************************************************************************
******************************************************************************/


/******************************************************************************
* Name: LED_GpioSet
* Description: GPIO LED operation: ON, OFF, TOGGLE.
* Param(s): -
* Return: -
******************************************************************************/
static void LED_GpioSet
(
    const gpioOutputPinConfig_t* pOutputConfig,
    LED_OpMode_t operation
)
{
    switch(operation)
    {
        case gLedOn_c:
#if gLED_InvertedMode_d
            GpioSetPinOutput(pOutputConfig);
#else
            GpioClearPinOutput(pOutputConfig);
#endif
            break;
        case gLedOff_c:
#if gLED_InvertedMode_d
            GpioClearPinOutput(pOutputConfig);
#else
            GpioSetPinOutput(pOutputConfig);
#endif
            break;
        case gLedToggle_c:
            GpioTogglePinOutput(pOutputConfig);
            break;
        default:
        	/*MISRA rule 16.4*/
            break;
    }
}


#if gLedRgbEnabled_d
/******************************************************************************
* Name: LED_ToggleRgbLed
* Description: This function is used to toggle the RGB LED
* Param(s): [in]    LEDNr - led identifier
* Return: - None
******************************************************************************/
static void LED_ToggleRgbLed
(
   LED_t LEDNr
)
{
    uint8_t redValue = 0;
    uint8_t greenValue = 0;
    uint8_t blueValue = 0;
    assert( LED_RGB == LEDNr );

#if BOARD_USE_PWM_FOR_RGB_LED && !(defined(gLpmIncluded_d)&&(gLpmIncluded_d > 0))
    redValue   = (uint8_t)PWM_GetChnCountVal(gRedLedPwmTimerInstance_c, gRedLedPwmTimerChannel_c);
    greenValue = (uint8_t)PWM_GetChnCountVal(gGreenLedPwmTimerInstance_c, gGreenLedPwmTimerChannel_c);
    blueValue  = (uint8_t)PWM_GetChnCountVal(gBlueLedPwmTimerInstance_c, gBlueLedPwmTimerChannel_c);
#else
    redValue   = ((0U == GpioReadOutputPin((const gpioOutputPinConfig_t*)&ledPins[gRedLedIdx_c])) ? 1U : 0U);
    greenValue = ((0U == GpioReadOutputPin((const gpioOutputPinConfig_t*)&ledPins[gGreenLedIdx_c])) ? 1U : 0U);
    blueValue  = ((0U == GpioReadOutputPin((const gpioOutputPinConfig_t*)&ledPins[gBlueLedIdx_c])) ? 1U : 0U);
#endif

#if gLED_InvertedMode_d
#if BOARD_USE_PWM_FOR_RGB_LED
    redValue   = LED_MAX_RGB_VALUE_c - redValue;
    greenValue = LED_MAX_RGB_VALUE_c - greenValue;
    blueValue  = LED_MAX_RGB_VALUE_c - blueValue;
#else
    redValue   = ((0U == redValue) ? 1U : 0U);
    greenValue = ((0U == greenValue) ? 1U : 0U);
    blueValue  = ((0U == blueValue) ? 1U : 0U);
#endif
#endif

    if((bool)redValue || (bool)greenValue || (bool)blueValue)
    {
#if !BOARD_USE_PWM_FOR_RGB_LED
        redValue   += gRGB_Threashold_c;
        greenValue += gRGB_Threashold_c;
        blueValue  += gRGB_Threashold_c;
#endif
        mLedRgbLastValues[0] = redValue;
        mLedRgbLastValues[1] = greenValue;
        mLedRgbLastValues[2] = blueValue;
        LED_SetRgbLed(LEDNr, 0, 0, 0);
    }
    else
    {
        if((!(bool)mLedRgbLastValues[0]) && (!(bool)mLedRgbLastValues[1]) && (!(bool)mLedRgbLastValues[2]))
        {
            mLedRgbLastValues[0] = LED_MAX_RGB_VALUE_c;
            mLedRgbLastValues[1] = LED_MAX_RGB_VALUE_c;
            mLedRgbLastValues[2] = LED_MAX_RGB_VALUE_c;
        }
        LED_SetRgbLed(LEDNr, mLedRgbLastValues[0], mLedRgbLastValues[1], mLedRgbLastValues[2]);
    }
}
#endif

#if gLedRgbEnabled_d
/******************************************************************************
* Name: LED_RgbLedInit
* Description: Initialize the RGB LED.
* Parameters: -
* Return: -
******************************************************************************/
static void LED_RgbLedInit(void)
{
#if BOARD_USE_PWM_FOR_RGB_LED && !(defined(gLpmIncluded_d)&&(gLpmIncluded_d > 0))
  
    tmr_adapter_pwm_param_t pwm_config;
    
    /* Ungate PWM module */
    PWM_Init(gRedLedPwmTimerInstance_c);
    PWM_Init(gGreenLedPwmTimerInstance_c);
    PWM_Init(gBlueLedPwmTimerInstance_c);

    /* PWM pin mux on RGB LED */
    BOARD_InitRGB();

    /*Configure PWM Module */
    pwm_config.initValue = PWM_INIT_VAL_c;
    pwm_config.frequency = PWM_FREQ;
    PWM_StartEdgeAlignedLowTrue(gRedLedPwmTimerInstance_c, &pwm_config, gRedLedPwmTimerChannel_c);
    PWM_StartEdgeAlignedLowTrue(gGreenLedPwmTimerInstance_c, &pwm_config, gGreenLedPwmTimerChannel_c);
    PWM_StartEdgeAlignedLowTrue(gBlueLedPwmTimerInstance_c, &pwm_config, gBlueLedPwmTimerChannel_c);
#else
    return;
#endif
}
#endif

#if (gLEDBlipEnabled_d) && (!gLedColorWheelEnabled_d || !gLedRgbEnabled_d)
/******************************************************************************
* Name: LED_DecrementBlip
* Description: Decrements the blink count
* Param(s): -
* Return: -
******************************************************************************/
static void LED_DecrementBlip(void)
{
    uint8_t iLedIndex;

    for(iLedIndex = 0; iLedIndex < gLEDsOnTargetBoardCnt_c; ++iLedIndex)
    {
        if((bool)mLedBlips[iLedIndex])
        {
            --mLedBlips[iLedIndex];
            if(!(bool)mLedBlips[iLedIndex])
            {
                mLedFlashingLEDs &= ~(1u << iLedIndex);
            }
        }
    }

    /* done, stop the timer */
    if(!(bool)mLedFlashingLEDs)
    {
        (void)TMR_StopTimer(mLEDTimerID);
    }
}
#endif

#if gTMR_Enabled_d
/******************************************************************************
* Name: LED_FlashTimeout
* Description: timer callback function that is called each time the timer
*              expires
* Param(s): [IN] timerId - the timer ID
* Return: -
******************************************************************************/
static void LED_FlashTimeout
(
    void *param
)
{
#if gLedColorWheelEnabled_d && gLedRgbEnabled_d
    if(!(bool)gColorWheelIdx)
    {
        LED_ToggleLed(mLedFlashingLEDs);
    }
    else
    {
        LED_ColorWheelSet(mLedFlashingLEDs & LED_RGB);
        LED_ToggleLed(mLedFlashingLEDs & (~(uint8_t)LED_RGB));
    }
#else

    LED_ToggleLed(mLedFlashingLEDs);
#if gLEDBlipEnabled_d
   /* decrement blips */
   LED_DecrementBlip();
#endif
#endif

    /* if serial lights, move on to next light */
    if(mfLedInSerialMode)
    {
        mLedFlashingLEDs = mLedFlashingLEDs << 1U;
        if((bool)(mLedFlashingLEDs & (gLedId_LastActiveId_c << 1U)))  /* wrap around */
        {
            mLedFlashingLEDs &= LED_ALL;
            mLedFlashingLEDs |= mLedStartFlashingPosition;
        }
    }
}

#if gLedColorWheelEnabled_d && gLedRgbEnabled_d
/******************************************************************************
* Name: LED_ColorWheelSet
* Description: Color wheel callback
* Param(s): -
* Return: -
******************************************************************************/
static void LED_ColorWheelSet( LED_t LEDNr)
{

    if(gColorWheelIdx >= NumberOfElements(gColorWheel))
    {
        gColorWheelIdx = 0;
    }
    LED_SetRgbLed(LEDNr, gColorWheel[gColorWheelIdx].redValue,
                    gColorWheel[gColorWheelIdx].greenValue, gColorWheel[gColorWheelIdx].blueValue);

    gColorWheelIdx++;
}
#endif /* gLedColorWheelEnabled_d && gLedRgbEnabled_d */

#if gLedRgbEnabled_d && gRgbLedDimmingEnabled_d
/******************************************************************************
* Name: LED_DimmingTimeout
* Description: Callback function of the interval timer used by the RGB LED 
*              dimmer. It increases/decreases the RGB values accordingly
*              to dimming information stored in mRbgDimInfo.
* Param(s): [in]    tmrId - timer ID
* Return: - None
******************************************************************************/
static void LED_DimmingTimeout(void *param)
{
  uint16_t redValue = 0;
  uint16_t greenValue = 0;
  uint16_t blueValue = 0;

  if(gLedDimOut_c == mRbgDimInfo.mode) 
  {     
      redValue = mRbgDimInfo.value[0] - mRbgDimInfo.increment[0];
      greenValue = mRbgDimInfo.value[1] - mRbgDimInfo.increment[1];
      blueValue = mRbgDimInfo.value[2] - mRbgDimInfo.increment[2];

  }
  
  if(gLedDimIn_c == mRbgDimInfo.mode)
  {    
      redValue = mRbgDimInfo.value[0] + mRbgDimInfo.increment[0];
      greenValue = mRbgDimInfo.value[1] + mRbgDimInfo.increment[1];
      blueValue = mRbgDimInfo.value[2] + mRbgDimInfo.increment[2];
  }
    
  if(--mRbgDimInfo.steps == 0u)
  {
    if(gLedDimOut_c == mRbgDimInfo.mode)
    {
     /* Turn off the RGB LED (due to potential residual value over PWM channel, 
      * if the initial value is not an integer multiple of dimming steps) 
      */
      LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gRedLedIdx_c], gLedOff_c);
      LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gGreenLedIdx_c], gLedOff_c);
      LED_GpioSet((const gpioOutputPinConfig_t*)&ledPins[gBlueLedIdx_c], gLedOff_c);
    }
    else
    {
      /* Set RGB LED to last values (due to potential residual value over PWM channel, 
      * if the initial value is not an integer multiple of dimming steps) 
      */
      RGB_SET_LED_RED(mLedRgbLastValues[0]); 
      RGB_SET_LED_GREEN(mLedRgbLastValues[1]);
      RGB_SET_LED_BLUE(mLedRgbLastValues[2]);
    }
    
    /* Stop the timer */
    (void)TMR_StopTimer(mRGBLedTimerID);
    
    /* Clear the ongoing flag */
    OSA_InterruptDisable();
    mRbgDimInfo.ongoing = FALSE;
    OSA_InterruptEnable();
  }
  else
  {
    /* store the current RGB LED color */
    mRbgDimInfo.value[0] = redValue; 
    mRbgDimInfo.value[1] = greenValue; 
    mRbgDimInfo.value[2] = blueValue;
    
    /* set the current RGB LED color */
    RGB_SET_LED_RED(redValue); 
    RGB_SET_LED_GREEN(greenValue); 
    RGB_SET_LED_BLUE(blueValue);
  }
}

/******************************************************************************
* Name: LED_StartRgbDimming
* Description: Starts RGB LED dimming
* Param(s): [in]    mode - dimm in/out
*                   interval - dimming interval, in seconds
* Return: TRUE if no dimming is ongoing and the command has been accepted,
*         FALSE otherwise
******************************************************************************/
static bool_t LED_StartRgbDimming(LED_DimMode_t mode, uint8_t interval)
{
  assert(mode <= gLedDimOut_c);
  bool_t status;
  
  /*In this case, increment is 0, it's no meaning to run*/
  if(!(bool)mLedRgbLastValues[0]&&!(bool)mLedRgbLastValues[1]&&!(bool)mLedRgbLastValues[2])
  {
      status = FALSE;
  }
  else
  {
      OSA_InterruptDisable();
      if(FALSE == mRbgDimInfo.ongoing)
      {
          mRbgDimInfo.ongoing = TRUE;
          OSA_InterruptEnable();
          
          /* populate dimming info structure */
          mRbgDimInfo.interval = interval;
          mRbgDimInfo.mode = mode;
          if(mode == gLedDimIn_c)  /*dimming in*/
          {
            mRbgDimInfo.value[0] = 0;
            mRbgDimInfo.value[1] = 0;
            mRbgDimInfo.value[2] = 0;
          }
          else  /*dimming out*/
          {
            mRbgDimInfo.value[0] = mLedRgbLastValues[0];
            mRbgDimInfo.value[1] = mLedRgbLastValues[1];
            mRbgDimInfo.value[2] = mLedRgbLastValues[2];
          }
          mRbgDimInfo.steps = (uint16_t)(mRbgDimInfo.interval * TmrSeconds(1))/(uint16_t)(gRgbLedDimTimerTimeout_c);
          mRbgDimInfo.steps /= 3u; /* adjust the steps number taking into account processing overhead */
          mRbgDimInfo.increment[0] = mLedRgbLastValues[0]/mRbgDimInfo.steps;
          mRbgDimInfo.increment[1] = mLedRgbLastValues[1]/mRbgDimInfo.steps;
          mRbgDimInfo.increment[2] = mLedRgbLastValues[2]/mRbgDimInfo.steps;  

          /* start the timer used for dimming */
          (void)TMR_StartIntervalTimer(mRGBLedTimerID, gRgbLedDimTimerTimeout_c, (pfTmrCallBack_t)LED_DimmingTimeout, NULL);
          
          status = TRUE;              
      }
      else  /*In this case, dimming is onoging*/
      {
        OSA_InterruptEnable();
        status = FALSE;
      } 
  }
  return status;
}
#endif /* gLedRgbEnabled_d && gRgbLedDimmingEnabled_d */
#endif /* gTMR_Enabled_d */
/******************************************************************************
* Name: LED_HwInit
* Description: Initialize the LED hardware module
*              
* Param(s): -
* Return: -
******************************************************************************/
static void LED_HwInit(void)
{
    BOARD_InitLEDs();
    (void)GpioOutputPinInit(ledPins, gLEDsOnTargetBoardCnt_c);
#if (defined(gLedRgbEnabled_d) && (gLedRgbEnabled_d > 0U))
    LED_RgbLedInit();
#endif 
}

/******************************************************************************
* Name: LED_ExitLowPower
* Description: Led Exit low power mode.
*              
* Param(s): -
* Return: -
******************************************************************************/
void LED_ExitLowPower(void)
{
    LED_HwInit();
}
#endif /* defined(gLEDSupported_d) && (gLEDSupported_d > 0U) */
