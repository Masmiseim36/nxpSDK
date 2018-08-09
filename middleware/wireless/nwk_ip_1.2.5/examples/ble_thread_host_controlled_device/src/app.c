/*! *********************************************************************************
* \addtogroup FSCI BLE application
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file app.c
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "LED.h"
#include "Keyboard.h"
#include "SerialManager.h"
#include "FsciInterface.h"
#include "Messaging.h"
#include "Panic.h"
#include "RNG_Interface.h"
/* BLE Host Stack */
#include "fsci_ble_interface.h"

#include "ble_init.h"

/*  Application */
#include "app.h"
#include "ApplMain.h"

#if gHkb_d
#include "fsl_port_hal.h"
#endif

extern anchor_t mHostAppInputQueue;
extern anchor_t mAppCbInputQueue;
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if 0
/* FSCI Interface Configuration structure */
static const gFsciSerialConfig_t fsciConfigStruct[] = {
    /* Baudrate,                        interface type,                  channel No,                virtual interface */
    {APP_SERIAL_INTERFACE_SPEED, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE,           0}  
};
#endif
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void APP_GeneratePseudoRandomNoSeed(void);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void BleApp_Init(void)
{
    /* Init serial manager */
    //SerialManager_Init();  
    
    /* Init FSCI */
    //FSCI_Init((void*)fsciConfigStruct);
     APP_GeneratePseudoRandomNoSeed();
    
    /* Register BLE handlers in FSCI */
    fsciBleRegister(1);
    
     /* Prepare application input queue.*/
    MSG_InitQueue(&mHostAppInputQueue); 

    /* Prepare callback input queue.*/
    MSG_InitQueue(&mAppCbInputQueue);
        
    /* BLE Host Stack Init */
    if (Ble_Initialize(App_GenericCallback) != gBleSuccess_c)
    {
        panic(0,0,0,0);
        return;
    }
    
}    


void BleApp_HandleKeys(key_event_t events)
{
    (void)events;
}


void BleApp_GenericCallback(gapGenericEvent_t* pGenericEvent)
{
}

/*!*************************************************************************************************
\fn     APP_GeneratePseudoRandomNoSeed
\brief  Generate pseudo rnd. no. seed
\param  [in]
\return         None
***************************************************************************************************/
static void APP_GeneratePseudoRandomNoSeed(void)
{
    uint8_t pseudoRNGSeed[20] = {0};
    
    RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[0])));
    RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[4])));
    RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[8])));
    RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[12])));
    RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[16])));
    RNG_SetPseudoRandomNoSeed(pseudoRNGSeed);
}
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* @}
********************************************************************************** */
