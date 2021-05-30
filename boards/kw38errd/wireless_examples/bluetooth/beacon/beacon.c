/*! *********************************************************************************
* \addtogroup Beacon
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Beacon application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"
#include "Panic.h"
#include "SecLib.h"
#include "fsl_device_registers.h"

/* BLE Host Stack */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_database.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1)
#include "dynamic_gatt_database.h"
#else
#include "gatt_db_handles.h"
#endif /* MULTICORE_APPLICATION_CORE */

/* Application */
#include "ApplMain.h"
#include "beacon.h"
#include "ble_conn_manager.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1)
#include "erpc_host.h"
#include "dynamic_gatt_database.h"
#endif /* MULTICORE_APPLICATION_CORE */

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
#if defined(gBeaconAE_c) && (gBeaconAE_c)
typedef enum 
{
  mAppState_NoAdv_c,
  mAppState_LegacyAdv_c,
  mAppState_ExtAdv_c,
  mAppState_PeriodicAdv_c,
  mAppState_Legacy_ExtAdv_c
}mAppTargetState_t;
#endif
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static bleDeviceAddress_t maBleDeviceAddress;

/* Adv Parameters */
static bool_t mAdvertisingOn = FALSE;

#if defined(gBeaconAE_c) && (gBeaconAE_c)
static bool_t mExtAdvertisingOn = FALSE;
static bool_t mPeriodicAdvOn = FALSE;
mAppTargetState_t mAppTargetState = mAppState_NoAdv_c;
#endif /*gBeaconAE_c */


/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_Config(void);
static void BleApp_Advertise(void);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
    uint8_t buffer[16] = {0};
    uint8_t hash[SHA256_HASH_SIZE];
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1)
    /* Init eRPC host */
    init_erpc_host();
#endif /* MULTICORE_APPLICATION_CORE */
#ifdef FSL_FEATURE_FLASH_ADDR_OF_VENDOR_BD_ADDR
    FLib_MemCpy(&buffer[0], (uint8_t*)FSL_FEATURE_FLASH_ADDR_OF_VENDOR_BD_ADDR, 6);
    FLib_MemCopy32Unaligned(&buffer[7], 0);
    FLib_MemCopy16Unaligned(&buffer[11], 0);
#else /* FSL_FEATURE_FLASH_ADDR_OF_VENDOR_BD_ADDR */
    /* Initialize sha buffer with values from SIM_UID */
    uint8_t len;
    BOARD_GetMCUUid(buffer, &len);
#endif /* FSL_FEATURE_FLASH_ADDR_OF_VENDOR_BD_ADDR */

    SHA256_Hash(buffer, 16, hash);

    /* Updated UUID value from advertising data with the hashed value */
    FLib_MemCpy(&gAppAdvertisingData.aAdStructures[1].aData[gAdvUuidOffset_c], hash, 16);
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    if (!mAdvertisingOn)
    {
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    #if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
        #if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
            (void)PWR_ChangeBlackBoxDeepSleepMode(gAppDeepSleepMode_c);
        #endif
    #else
            (void)PWR_ChangeDeepSleepMode(gAppDeepSleepMode_c);
    #endif
#endif
        BleApp_Advertise();
    }
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
#if defined(gBeaconAE_c) && (gBeaconAE_c)
void BleApp_HandleKeys(key_event_t events)
{
    switch (events)
    {
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
        case gKBD_EventPB1_c:
#endif
        case gKBD_EventPB2_c:
        {
            /* First press starts legacy advertising */
            if ((mAdvertisingOn == FALSE) && (mExtAdvertisingOn == FALSE) && (mPeriodicAdvOn == FALSE))
            {
                mAppTargetState = mAppState_LegacyAdv_c;
                BleApp_Start();
            }
            /* Second press starts extended advertising  and stops legacy advertising */
            else if ((mAdvertisingOn == TRUE) && (mExtAdvertisingOn == FALSE) && (mPeriodicAdvOn == FALSE))
            {
                mAppTargetState = mAppState_ExtAdv_c;   
                if (gBleSuccess_c != Gap_StopAdvertising())
                {
                    panic(0, 0, 0, 0);
                }
            }     
            /* Third press starts extended advertising without data and periodic advertising */
            else if ((mAdvertisingOn == FALSE) && (mExtAdvertisingOn == TRUE) && (mPeriodicAdvOn == FALSE))
            {
                mAppTargetState = mAppState_PeriodicAdv_c;
                if(gBleSuccess_c != Gap_StopExtAdvertising(gExtAdvParams.handle))
                {
                    panic(0, 0, 0, 0);
                }
            }
            /* Fourth press starts legacy advertising and extended advertising */
            else if ((mAdvertisingOn == FALSE) && (mExtAdvertisingOn == TRUE) && (mPeriodicAdvOn == TRUE))
            {
                mAppTargetState = mAppState_Legacy_ExtAdv_c;
                (void)Gap_StopPeriodicAdvertising(gExtAdvParams.handle);
            }
            /* Fifth press stops extended advertising and legacy advertising*/
            else if ((mAdvertisingOn == TRUE) && (mExtAdvertisingOn == TRUE) && (mPeriodicAdvOn == FALSE))  
            {
                mAppTargetState = mAppState_NoAdv_c;
                if(gBleSuccess_c != Gap_StopExtAdvertising(gExtAdvParams.handle))
                {
                    panic(0, 0, 0, 0);
                }
            }
        }
        break;
        
        default:
        {
            ; /* No action required */
        }
        break;
    }
}

#else /* defined(gBeaconAE_c) && (gBeaconAE_c) */
void BleApp_HandleKeys(key_event_t events)
{
    switch (events)
    {
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
        case gKBD_EventPB1_c:
#endif
        case gKBD_EventPB2_c:
        {
            /* First press starts legacy advertising */
            if (mAdvertisingOn == FALSE)
            {
                BleApp_Start();
            }
            else
            {
                if (gBleSuccess_c != Gap_StopAdvertising())
                {
                    panic(0, 0, 0, 0);
                }
            }
        }
        break;
        default:
        {
            ; /* No action required */
        }
        break;
    }
}
#endif /* defined(gBeaconAE_c) && (gBeaconAE_c) */

/*! *********************************************************************************
* \brief        Handles gap generic events.
*
* \param[in]    pGenericEvent    Gap Generic event structure.
********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    switch (pGenericEvent->eventType)
    {
        /* Host Stack initialization is complete */
        case gInitializationComplete_c:
        {
            /* Stack configuration */
            BleApp_Config();
        }
        break;

        case gPublicAddressRead_c:
        {
            /* Use address read from the controller */
            FLib_MemCpyReverseOrder(maBleDeviceAddress, pGenericEvent->eventData.aAddress, sizeof(bleDeviceAddress_t));
        }
        break;

        case gAdvertisingParametersSetupComplete_c:
        {
            if(gBleSuccess_c != Gap_SetAdvertisingData(&gAppAdvertisingData, NULL))
            {
                panic(0, 0, 0, 0);
            }
        }
        break;

        case gAdvertisingDataSetupComplete_c:
        {
            if (gBleSuccess_c != App_StartAdvertising(BleApp_AdvertisingCallback, NULL))
            {
                panic(0, 0, 0, 0);
            }
        }
        break;

#if defined(gBeaconAE_c) && (gBeaconAE_c)
        case gExtAdvertisingParametersSetupComplete_c:
        {
            gapAdvertisingData_t* pExtAdvData;
            if(mAppTargetState == mAppState_PeriodicAdv_c)
            {
                pExtAdvData = &gAppExtAdvertisingNoData;
            }
            else
            {
                pExtAdvData = &gAppExtAdvertisingData;
            }
            if(gBleSuccess_c != Gap_SetExtAdvertisingData(gExtAdvParams.handle, pExtAdvData, NULL))
            {
                panic(0, 0, 0, 0);
            }
        }
        break;

        case gExtAdvertisingDataSetupComplete_c:
        {
            if(gBleSuccess_c != App_StartExtAdvertising(BleApp_AdvertisingCallback, NULL, gExtAdvParams.handle, gBleExtAdvNoDuration_c, gBleExtAdvNoMaxEvents_c))
            {
                panic(0, 0, 0, 0);
            }
        }
        break;
        
        case gExtAdvertisingSetRemoveComplete_c:
        {
            if((mAppTargetState == mAppState_PeriodicAdv_c) || (mAppTargetState == mAppState_Legacy_ExtAdv_c))
            {
                if (gBleSuccess_c != Gap_SetExtAdvertisingParameters(&gExtAdvParams))
                {
                    panic(0, 0, 0, 0);
                }
            }
            else if((mAppTargetState == mAppState_NoAdv_c) && (mAdvertisingOn == TRUE))
            {
                if (gBleSuccess_c != Gap_StopAdvertising())
                {
                    panic(0, 0, 0, 0);
                }
            }
        } 
        break;

        case gPeriodicAdvParamSetupComplete_c:
        {
            if(gBleSuccess_c != Gap_SetPeriodicAdvertisingData(gExtAdvParams.handle, &gAppExtAdvertisingData))
            {
                panic(0, 0, 0, 0);
            }
        }
        break;

        case gPeriodicAdvDataSetupComplete_c:
        {
            if(gBleSuccess_c != Gap_StartPeriodicAdvertising(gExtAdvParams.handle))
            {
                panic(0, 0, 0, 0);
            }
        }
        break;
#endif /*gBeaconAE_c */

        case gAdvertisingSetupFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

        /* Internal error has occurred */
        case gInternalError_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
/*! *********************************************************************************
* \brief        Configures BLE Stack after initialization. Usually used for
*               configuring advertising, scanning, white list, services, et al.
*
********************************************************************************** */
static void BleApp_Config(void)
{
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1)
    if (GattDbDynamic_CreateDatabase() != gBleSuccess_c)
    {
        panic(0,0,0,0);
        return;
    }
#endif /* MULTICORE_APPLICATION_CORE */

    /* Read public address from controller */
    (void)Gap_ReadPublicDeviceAddress();

    mAdvertisingOn = FALSE;

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    #if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
        #if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
            (void)PWR_ChangeBlackBoxDeepSleepMode(cPWR_DeepSleepMode);
            PWR_AllowBlackBoxToSleep();
        #endif
        (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
        PWR_AllowDeviceToSleep();
    #else
        (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
        PWR_AllowDeviceToSleep();
    #endif
#endif
        LED_StopFlashingAllLeds();
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    /* Set advertising parameters*/
    (void)Gap_SetAdvertisingParameters(&gAppAdvParams);
}

/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
        /* Advertising state changed */
        case gAdvertisingStateChanged_c:
        {
            mAdvertisingOn = !mAdvertisingOn;
            if(mAdvertisingOn)
            {
                Led2On();
            }
            else
            {
                Led2Off();
#if defined(gBeaconAE_c) && (gBeaconAE_c)
                if(mAppTargetState == mAppState_ExtAdv_c)
                {
                    if (gBleSuccess_c != Gap_SetExtAdvertisingParameters(&gExtAdvParams))
                    {
                        panic(0, 0, 0, 0);
                    }      
                }  
#endif              
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#if defined(gBeaconAE_c) && (gBeaconAE_c)
                if ((mExtAdvertisingOn == FALSE) && (mAppTargetState == mAppState_NoAdv_c))
#endif /* gBeaconAE_c*/
                {
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
#if defined(gErpcLowPowerApiServiceIncluded_c) && (gErpcLowPowerApiServiceIncluded_c)
                    (void)PWR_ChangeBlackBoxDeepSleepMode(cPWR_DeepSleepMode);
#endif
                    (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
#else
                    (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
#endif
                }
#endif /* cPWR_UsePowerDownMode */
            }
        }
        break;

#if defined(gBeaconAE_c) && (gBeaconAE_c)
        case gExtAdvertisingStateChanged_c:
        {
            mExtAdvertisingOn = !mExtAdvertisingOn;
            if(mExtAdvertisingOn)
            {
                Led3On();
                if(mAppTargetState == mAppState_PeriodicAdv_c)
                {
                    if (gBleSuccess_c != Gap_SetPeriodicAdvParameters(&gPeriodicAdvParams))
                    {
                        panic(0, 0, 0, 0);
                    }
                }
                else if(mAppTargetState == mAppState_Legacy_ExtAdv_c)
                {
                    if (gBleSuccess_c !=Gap_StartAdvertising(BleApp_AdvertisingCallback, NULL))
                    {
                        panic(0, 0, 0, 0);
                    }                  
                }
            }
            else
            {
                Led3Off();
                Gap_RemoveAdvSet(gExtAdvParams.handle);
            }
        }
        break;

        case gPeriodicAdvertisingStateChanged_c:
        {
            mPeriodicAdvOn = !mPeriodicAdvOn;
            if(mPeriodicAdvOn)
            {
                Led4On();
            }
            else
            {
                Led4Off();
                if(mAppTargetState == mAppState_Legacy_ExtAdv_c)
                {
                    if(gBleSuccess_c != Gap_StopExtAdvertising(gExtAdvParams.handle))
                    {
                        panic(0, 0, 0, 0);
                    }
                }
            }
        }
        break;
#endif /*gBeaconAE_c */

        /* Advertising command failed */
        case gAdvertisingCommandFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}
/*! *********************************************************************************
* @}
********************************************************************************** */
