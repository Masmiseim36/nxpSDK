/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_LED_H             
#define _APP_LED_H

/*!=================================================================================================
\file       app_led.h
\brief      This is the header file for the application led configuration
==================================================================================================*/
   
/*==================================================================================================
Include Files
==================================================================================================*/
#include "app_init.h"
#include "LED.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#if gLedRgbEnabled_d
    #define gConfigLedsOnTargetBoard_c          gLEDsOnTargetBoardDefault_c - 2 
#else
    #define gConfigLedsOnTargetBoard_c          gLEDsOnTargetBoardDefault_c
#endif

#define gLedId_Undefined_c                      THR_ALL_FFs8
#define gLedState_Undefined_c                   THR_ALL_FFs8
#define gLedSetup_DefaultFlashPeriod_c          mLEDInterval_c

/* LED setup bitmap */
#define gLedSetup_On_c                          (0x01<<7)
#define gLedSetup_Flashing_c                    (0x01<<6)
#define gLedSetup_Red_c                         (0x01<<5)
#define gLedSetup_Green_c                       (0x01<<4)
#define gLedSetup_Blue_c                        (0x01<<3)
#define gLedSetup_Chasing_c                     (0x01<<2)
#define gLedSetup_Rgb_c                         (0x01<<1)
#define gLedSetup_FlashPeriod_c                 (0x01<<0)

/* RGB led - color wheel*/
#define gLedSetupColorWheel_c                   (gLedSetup_Flashing_c | gLedSetup_Chasing_c)

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/


/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\public
\fn  void Led_SetState(appDeviceMode_t devModeId, appDeviceState_t devStateId)
\brief set LEDs based on device modeId/stateId

\param [in]   devModeId                 device mode id
\param [in]   devStateId                device state id
 ***************************************************************************************************/
void Led_SetState(appDeviceMode_t devModeId, appDeviceState_t devStateId);

#if gLedRgbEnabled_d
/*!*************************************************************************************************
\public
\fn  void Led_UpdateRgbState(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
\brief set RGB LEDs based on input values for red, green and blue

\param [in]    redValue      Red value
\param [in]    greenValue    Green value
\param [in]    blueValue     Blue value
 ***************************************************************************************************/
void Led_UpdateRgbState(uint8_t redValue, uint8_t greenValue, uint8_t blueValue);
#endif

/*!*************************************************************************************************
\fn     void Led_MacTxOn(void)
\brief  Function used to set LED On (80215.4 tx activity)
***************************************************************************************************/
void Led_MacTxOn(void);

/*!*************************************************************************************************
\fn     void Led_MacTxOff(void)
\brief  Function used to set LED Off (80215.4 tx activity)
***************************************************************************************************/
void Led_MacTxOff(void);

/*!*************************************************************************************************
\fn     void Led_Identify(bool_t state)
\brief  This function is used to start the board identification by flashing the TX LED.

\param  [in]    state      Variable used to toggle the identification process
***************************************************************************************************/
void Led_Identify(bool_t state);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /*  _APP_LED_H */
