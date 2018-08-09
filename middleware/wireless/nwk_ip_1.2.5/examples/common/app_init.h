/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
extern void (* pfAppKeyboardHandler)(void*);

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
