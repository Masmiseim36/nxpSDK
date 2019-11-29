/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_INIT_H             
#define _APP_INIT_H

/*!=================================================================================================
\file       app_init.h
\brief      This is the header file for the initial system startup module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* Network Utils*/
#include "network_utils.h"
#include "LED.h"
#include "board.h"
#include "thread_types.h"

#if DNS_ENABLED
#include "dns_client.h"
#endif

#if MDNS_ENABLED
#include "mdns.h"
#endif

/*==================================================================================================
Public macros
==================================================================================================*/

/* LowPower is desable  gLpmIncluded_d = 0*/
#ifndef gLpmIncluded_d
  #define gLpmIncluded_d 0
#endif


/* app device state: */
typedef enum appDeviceState_tag{
    /* configuration states */
    gDeviceState_FactoryDefault_c,
    gDeviceState_JoiningOrAttaching_c,
    gDeviceState_NwkOperationPending_c,
    gDeviceState_Leader_c,
    gDeviceState_NwkConnected_c,
    gDeviceState_NwkFailure_c,
    gDeviceState_ActiveRouter_c,
    /* application states */
    gDeviceState_AppLedOn_c,
    gDeviceState_AppLedOff_c,
    gDeviceState_AppLedFlash_c,
    gDeviceState_AppLedToggle_c,
    gDeviceState_AppLedRgb_c,
    gDeviceState_AppLedColorWheel_c,
}appDeviceState_t;

/* app device mode: */
typedef enum appDeviceMode_tag{
    gDeviceMode_Configuration_c,
    gDeviceMode_Application_c
}appDeviceMode_t;

#define APP_GetState(thrInst)           gAppDeviceState[thrInst]
#define APP_SetState(thrInst, state)    gAppDeviceState[thrInst] = (state); 
#define APP_GetMode(thrInst)            gAppDeviceMode[thrInst]
#define APP_SetMode(thrInst, state)     gAppDeviceMode[thrInst] = (state);        



/* Tx Activity LED: */
#if (USB_KW24 || FRDM_KW41Z || USB_KW41)
    /* gUSBKW24D512Dongle */
    #define LED_TX_ACTIVITY                     LED1                                           
#elif (FRDM_KL46Z || FRDM_KW24 || FRDM_K64F)
    /* FRDM_KL46Z, FRDM_KW24, FRDM_K64F */
    #define LED_TX_ACTIVITY                     LED4
#else 
/* Twr-KW24D512, Twr-KW21D256 */
    #define LED_TX_ACTIVITY                     LED2                                                                                                    
#endif /* USB_KW24 || FRDM_KW41Z || USB_KW41 */

#define AppTxLedActivityOn()                LED_Operate(LED_TX_ACTIVITY, gLedOn_c) 
#define AppTxLedActivityOff()               LED_Operate(LED_TX_ACTIVITY, gLedOff_c)    
/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern taskMsgQueue_t appThreadMsgQueue;
extern void (* pfAppKeyboardHandler)(uint8_t*);

extern appDeviceState_t  gAppDeviceState[];
extern appDeviceMode_t  gAppDeviceMode[];      

extern void ResetMCU(void);
extern uint32_t APP_GetResetMcuTimeout(void);
extern void APP_ResetMcuOnTimeout(uint32_t timeoutMs, bool_t resetToFactory);

/*==================================================================================================
Public function prototypes
==================================================================================================*/

/*================================================================================================*/
#endif  /* _USER_APP_H */
