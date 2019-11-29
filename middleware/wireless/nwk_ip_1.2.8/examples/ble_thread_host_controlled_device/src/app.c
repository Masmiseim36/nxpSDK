/*! *********************************************************************************
* \addtogroup FSCI BLE application
* @{
********************************************************************************** */
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
* \file app.c
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
