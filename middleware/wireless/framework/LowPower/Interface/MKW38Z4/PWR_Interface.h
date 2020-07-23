/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _PWR_INTERFACE_H_
#define _PWR_INTERFACE_H_

/*****************************************************************************
 *                               INCLUDED HEADERS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the headers that this module needs to include.    *
 * Note that it is not a good practice to include header files into header   *
 * files, so use this section only if there is no other better solution.     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#include "EmbeddedTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                             PRIVATE MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...                                              *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *                             PUBLIC MACROS                                 *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...                                              *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#ifndef gAllowDeviceToSleep_c
#define gAllowDeviceToSleep_c 0
#endif

#define gPWRLib_ResetStatus_WAKEUP  ((uint16_t)1U<<0)
#define gPWRLib_ResetStatus_LVD     ((uint16_t)1U<<1)
#define gPWRLib_ResetStatus_LOC     ((uint16_t)1U<<2)
#define gPWRLib_ResetStatus_LOL     ((uint16_t)1U<<3)
#define gPWRLib_ResetStatus_COP     ((uint16_t)1U<<5)
#define gPWRLib_ResetStatus_PIN     ((uint16_t)1U<<6)
#define gPWRLib_ResetStatus_POR     ((uint16_t)1U<<7)
#define gPWRLib_ResetStatus_LOCKUP  ((uint16_t)1U<<9)
#define gPWRLib_ResetStatus_SW      ((uint16_t)1U<<10)
#define gPWRLib_ResetStatus_MDM_AP  ((uint16_t)1U<<11)
#define gPWRLib_ResetStatus_SACKERR ((uint16_t)1U<<13)

/*****************************************************************************
 *                        PUBLIC TYPE DEFINITIONS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the data types definitions: stuctures, unions,    *
 * enumerations, typedefs ...                                                *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*
 * Name: PWRLib_WakeupReason_t
 * Description: Wakeup reason UNION definitions
 */
typedef  union
{
    uint16_t AllBits;
    struct
    {
        uint8_t FromReset        :1;  /* Comming from Reset */
        uint8_t FromPSwitch      :1;  /* Wakeup by DCDC from Pswitch Pin */
        uint8_t FromKeyBoard     :1;  /* Wakeup by TSI/Push button interrupt */
        uint8_t FromLPTMR        :1;  /* Wakeup by LPTMR timer interrupt */
        uint8_t FromRadio        :1;  /* Wakeup by RTC timer interrupt */
        uint8_t FromBLE_LLTimer  :1;  /* Wakeup by BLE_LL Timer */
        uint8_t DeepSleepTimeout :1;  /* DeepSleep timer ran out */
        uint8_t SleepTimeout     :1;  /* Sleep timer ran out */
        uint8_t FromSerial       :1;  /* Wakeup by Serial RX */
    } Bits;
} PWRLib_WakeupReason_t;

/*
 * Name: PWRLib_LVD_VoltageLevel_t
 * Description: Reported voltage levels. LEVEL_BEST is the highest, corespondig to > 3.0V
 */
typedef enum
{
    PWR_ABOVE_LEVEL_3_0V,  /* > 3.0 V */
    PWR_BELOW_LEVEL_3_0V,
    PWR_BELOW_LEVEL_2_9V,
    PWR_BELOW_LEVEL_2_8V,
    PWR_BELOW_LEVEL_2_7V,
    PWR_BELOW_LEVEL_2_56V,
    PWR_BELOW_LEVEL_2_1V,
    PWR_BELOW_LEVEL_2_0V,
    PWR_BELOW_LEVEL_1_9V,
    PWR_BELOW_LEVEL_1_8V,
    PWR_LEVEL_CRITICAL  /* < 1.6 V */
} PWRLib_LVD_VoltageLevel_t;

/*
 * NAME: pfPWRCallBack_t
 * DESCRIPTION: PWR callback function
 */
typedef void ( *pfPWRCallBack_t ) ( void);

/*
 *  Defines the Application power states so the Lowpower module
 * can match to the most appropriate device lowpower mode for this mode
 */
typedef enum
{
    PWR_APP_STATE_NO_ACTIVITY,                /*  Lowpower with no RAM retention: VLLS0, VLLS1 or Pswitch mode                          */
    PWR_APP_STATE_NO_ACTIVITY_RAM_RET,        /*  Lowpower with RAM retention: VLLS2/3 or LLS2/3                                        */
    PWR_APP_STATE_ADV,                        /*  Lowpower when BLE stack is advertising only, VLLS or LLS with reduce wakeup time      */
    PWR_APP_STATE_SCAN,                       /*  Lowpower when BLE stack is scanning or advertising, LLS                               */
    PWR_APP_STATE_CONN,                       /*  Lowpower when BLE stack is connected mode, and/or scanning or advertising             */
    PWR_APP_STATE_INVALID,
} PWR_AppState;

/*****************************************************************************
 *                        PUBLIC VARIABLES                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the data types definitions: stuctures, unions,    *
 * enumerations, typedefs ...                                                *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/* Keep for backward compatibiltiy, the Upper layer shall now call
        PWR_GetWakeUpReason()  function                                     */
extern volatile PWRLib_WakeupReason_t PWRLib_MCU_WakeupReason;

/*****************************************************************************
 *                            PUBLIC FUNCTIONS                               *
 *---------------------------------------------------------------------------*
 * Add to this section all the global functions prototype preceded           *
 * by the keyword 'extern'                                                   *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*---------------------------------------------------------------------------
 * Name: PWR_Init
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void PWR_Init(void);

/*---------------------------------------------------------------------------
 * Name: PWR_GetSystemResetStatus
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern uint16_t PWR_GetSystemResetStatus(void);

/*---------------------------------------------------------------------------
 * Name: PWR_ChangeDeepSleepMode
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern bool_t PWR_ChangeDeepSleepMode(uint8_t dsMode);

/*---------------------------------------------------------------------------
 * Name: PWR_GetDeepSleepMode
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint8_t PWR_GetDeepSleepMode(void);

/*---------------------------------------------------------------------------
 * Name: PWR_AllowDeviceToSleep
 * Description: - This function is used to reset the global variable which
 *                permits(on SET)/restricts(On RESET)the device to enter
 *                low power state.
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void PWR_AllowDeviceToSleep(void);

/*---------------------------------------------------------------------------
 * Name: PWR_DisallowDeviceToSleep
 * Description: - This function is used to reset the global variable which
 *                permits(on SET)/restricts(On RESET)the device to enter
 *                low power state.
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void PWR_DisallowDeviceToSleep(void);

/*---------------------------------------------------------------------------
 * Name: PWR_DisallowXcvrToSleep
 * Description: - This function is used to reset the global variable which
 *                permits(on SET)/restricts(On RESET)the device to enter
 *                low power state.
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_DisallowXcvrToSleep(void);

/*---------------------------------------------------------------------------
 * Name: PWR_AllowXcvrToSleep
 * Description: - This function is used to reset the global variable which
 *                permits(on SET)/restricts(On RESET)the device to enter
 *                low power state.
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_AllowXcvrToSleep(void);

/*---------------------------------------------------------------------------
 * Name: PWR_CheckIfDeviceCanGoToSleep
 * Description: - This function is used to  Check  the Permission flag to go to
 *                low power mode
 * Parameters: -
 * Return: - TRUE  : If the device is allowed to go to the LPM else FALSE
 *---------------------------------------------------------------------------*/
extern bool_t PWR_CheckIfDeviceCanGoToSleep(void);

/*---------------------------------------------------------------------------
 * Name: PWR_EnterPowerOff
 * Description: - This function was kept to ensure compatibility with other platforms
 *              - It is not actually a power off. It is mode 4 with no wakeup sources.
 *              - Enable mode 4 if you use it. If mode 4 is disabled it is just an infinite loop.
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void PWR_EnterPowerOff(void);

/*---------------------------------------------------------------------------
 * Name: PWR_EnterLowPower
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern PWRLib_WakeupReason_t PWR_EnterLowPower(void);

/*---------------------------------------------------------------------------
 * Name: PWR_GENFSK_EnterDSM
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
bool_t PWR_GENFSK_EnterDSM(uint32_t dsmDuration);

/*---------------------------------------------------------------------------
 * Name: PWR_BLE_EnterDSM
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_BLE_EnterDSM(uint16_t wakeupInstant);

/*---------------------------------------------------------------------------
 * Name: PWR_BLE_GetReferenceClock
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint16_t PWR_BLE_GetReferenceClock(void);

/*---------------------------------------------------------------------------
 * Name: PWR_BLE_ExitDSM
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_BLE_ExitDSM(void);

/*---------------------------------------------------------------------------
 * Name: PWR_SetDeepSleepTimeInMs
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void PWR_SetDeepSleepTimeInMs(uint32_t deepSleepTimeMs);

/*---------------------------------------------------------------------------
 * Name: PWR_SetDeepSleepTimeInSymbols
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void PWR_SetDeepSleepTimeInSymbols(uint32_t deepSleepTimeSymbols);

/*---------------------------------------------------------------------------
 * Name: PWR_LVD_ReportLevel
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern PWRLib_LVD_VoltageLevel_t PWR_LVD_ReportLevel(void);

#if defined(PWR_FUNCTION_NOT_USED)
/*---------------------------------------------------------------------------
 * Name: PWR_SystemReset
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void PWR_SystemReset(void);
#endif
/*---------------------------------------------------------------------------
 * Name: PWR_RegisterLowPowerEnterCallback
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_RegisterLowPowerEnterCallback(pfPWRCallBack_t lowPowerEnterCallback);

/*---------------------------------------------------------------------------
 * Name: PWR_RegisterLowPowerExitCallback
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_RegisterLowPowerExitCallback(pfPWRCallBack_t lowPowerExitCallback);

/*---------------------------------------------------------------------------
 * Name: PWR_EnterSleep
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_EnterSleep(void);

/*---------------------------------------------------------------------------
 * Name: PWR_GetTotalSleepDurationMS
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint32_t PWR_GetTotalSleepDurationMS(void);

/*---------------------------------------------------------------------------
 * Name: PWR_ResetTotalSleepDuration
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_ResetTotalSleepDuration(void);

/*---------------------------------------------------------------------------
 * Name: PWR_GetWakeupReason
 * Description: - get the last wakeup reason from lowpower - if the wakeup reason is NULL
        the handling of the wakeup source has already been performed
 * Parameters: -
 * Return: - Wakeup reason
 *---------------------------------------------------------------------------*/
PWRLib_WakeupReason_t PWR_GetWakeUpReason(void);

/*---------------------------------------------------------------------------
 * Name: PWR_GetAppState
 * Description: - Provide the application state
 * Parameters: -
 * Return: - application state
 *---------------------------------------------------------------------------*/
PWR_AppState PWR_GetAppState(void);

/*---------------------------------------------------------------------------
 * Name: PWR_SetNewAppState
 * Description: - Provide the application lowpower mode so the lowpower module could
 *                match to the most appropriate lowpower mode
 * Parameters: -  app_state
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_SetNewAppState(PWR_AppState app_state);

/*---------------------------------------------------------------------------
 * Name: PWR_SetWakeupPins
 * Description: - Enable Additional wakeup pins
 * Parameters: -  wakeup pins bitmap from board.h
 *      It is recommended to be called from hardware_init() before lowpower module
 *         initialization PWR_Init()
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_SetWakeupPins(uint32_t wkup_pin_set);

/*---------------------------------------------------------------------------
 * Name: PWR_UnSetWakeupPins
 * Description: - Disable wakeup pins
 * Parameters: -  wakeup pins bitmap from board.h
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_UnSetWakeupPins(uint32_t wkup_pin_unset);

/*---------------------------------------------------------------------------
 * Name: PWR_SetWakeupPinsSerial
 * Description: - Enable Additional wakeup pins from Serial
 *      When wakeup from these pins are detected, the Lowpower module will call
 *        the serial API SerialInterface_SerialIsWakeupSrc()
 *        to call the Application serial RX callback
 *      Can be unset by PWR_UnSetWakeupPins()
 *      Only one serial interface can be registered but serial pins from the serial
 *      peripheral can be source of wakeup. However, in common case, only the RX line
 *      usefull, but an other IO can also be used.
 *   Warning : in order to allow proper detection when waking up from a RAM OFF lowpower mode,
 *      this function shall be called from hardware_init.c before the lowpower module
 *      initialization PWR_Init()
 * Parameters: -  serial wakeup pins bitmap from board.h
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_SetWakeupPinsSerial(uint32_t serial_wkup_pin_set, uint8_t serial_interface);

#ifdef __cplusplus
}
#endif

#endif /* _PWR_INTERFACE_H_ */

