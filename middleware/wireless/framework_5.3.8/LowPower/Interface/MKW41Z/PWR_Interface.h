/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

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

#define gPWRLib_ResetStatus_WAKEUP  (1<<0)
#define gPWRLib_ResetStatus_LVD     (1<<1)
#define gPWRLib_ResetStatus_LOC     (1<<2)
#define gPWRLib_ResetStatus_LOL     (1<<3)
#define gPWRLib_ResetStatus_COP     (1<<5)
#define gPWRLib_ResetStatus_PIN     (1<<6)
#define gPWRLib_ResetStatus_POR     (1<<7)
#define gPWRLib_ResetStatus_LOCKUP  (1<<9)
#define gPWRLib_ResetStatus_SW      (1<<10)
#define gPWRLib_ResetStatus_MDM_AP  (1<<11)
#define gPWRLib_ResetStatus_SACKERR (1<<13)

/* Zigbee stack interface functions */
    
/******************************************************************************
 * Description : Macros to get the current status of the STACK
 * Assumptions : None
 * Inputs      : None
 * Output      : Status from PWRLib_StackPS
 * Errors      : Not handled
 ******************************************************************************/
#define PWRLib_GetCurrentZigbeeStackPowerState      PWRLib_StackPS
    
/******************************************************************************
 * Description : Macro to set the status of the STACK
 * Inputs      : NewStackPS  : New state
 *               StackPS_Running   : Busy
 *               StackPS_Sleep     : Sleep allowed. Duration is appl. specific
 *               StackPS_DeepSleep : DeepSleep allowed
 * Output      : None
 * Errors      : Not handled
 ******************************************************************************/
#define PWRLib_SetCurrentZigbeeStackPowerState(NewStackPS)  PWRLib_StackPS = (NewStackPS);
    
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
    uint8_t AllBits;
    struct 
    {
        uint8_t FromReset        :1;  /* Comming from Reset */
        uint8_t FromPSwitch_UART :1;  /* Wakeup by UART interrupt */
        uint8_t FromKeyBoard     :1;  /* Wakeup by TSI/Push button interrupt */
        uint8_t FromLPTMR        :1;  /* Wakeup by LPTMR timer interrupt */
        uint8_t FromRadio        :1;  /* Wakeup by RTC timer interrupt */
        uint8_t FromBLE_LLTimer  :1;  /* Wakeup by BLE_LL Timer */
        uint8_t DeepSleepTimeout :1;  /* DeepSleep timer ran out */
        uint8_t SleepTimeout     :1;  /* Sleep timer ran out */
    } Bits;
} PWRLib_WakeupReason_t;

/* 
 * Name: PWRLib_StackPS_t
 * Description: Zigbee stack power state
 */
typedef enum
{
    StackPS_Running=122,
    StackPS_Sleep,
    StackPS_DeepSleep
} PWRLib_StackPS_t;

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

/*****************************************************************************
 *                        PUBLIC VARIABLES                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the data types definitions: stuctures, unions,    *
 * enumerations, typedefs ...                                                *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
extern PWRLib_StackPS_t PWRLib_StackPS;
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

/*---------------------------------------------------------------------------
 * Name: PWR_SystemReset
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void PWR_SystemReset(void);

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
    
#ifdef __cplusplus
}
#endif

#endif /* _PWR_INTERFACE_H_ */

