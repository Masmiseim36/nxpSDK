/*! *********************************************************************************
* \addtogroup Extended Advertising Peripheral
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2020 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the extended advertising peripheral application
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
#include "MemManager.h"
#include "Panic.h"
#include "SerialManager.h"

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_handles.h"

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "temperature_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "adv_ext_peripheral.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mNumberOfAdvHandles_c    (2U)
#define mInvalidAdvHandle      (0xffU)

#ifndef mAE_PeripheralDebug_c  
#define mAE_PeripheralDebug_c   (0)
#endif

#define mChangePerAdvDataOption_c              (6U)
#define mChangeNonConnNonScannAdvDataOption_c  (5U)
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

typedef enum 
{
    madvHandle_Available_c,
    madvHandle_InUse_c,
}advHandleStatus_t;

typedef enum 
{
    mAdvStatus_Off_c,
    mAdvStatus_On_c
}advStatus_t;

typedef enum 
{
    mLegacyAdvIndex_c = 0,
    mExtAdvScannIndex_c,
    mExtAdvConnIndex_c,
    mExtAdvNonConnNonScanIndex_c,
    mPeriodicAdvIndex_c,
    mAdvIndexMax_c
}advIndex_t;

typedef enum
{
    mExtAdvSeq_Idle_c,
    mExtAdvSeq_Start_c,
    mExtAdvSeq_Stop_c,
    mExtAdvSeq_ChangeExtAdvData_c,
    mExtAdvSeq_ChangePeriodicData_c
}extAdvSequence_t;

typedef enum {gApiReq_Success_c, gApiReq_Denied_c }apiReqStatus_t;
typedef enum {mSS_PrintMenu_c, mSS_WaitOption_c, mSS_HandleOption_c}switch2Status_t;

typedef struct appPeerDevice_tag
{
    deviceId_t deviceId;
    tmrTimerID_t timerId;
}appPeerDevice_t;
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* Adv State */
static extAdvSequence_t mExtAdvSequence; 
static advStatus_t maAdvStatus[mAdvIndexMax_c];

static advIndex_t mExtAdvAPIOwner = mAdvIndexMax_c;

/* Service Data*/
static bool_t           basValidClientList[gAppMaxConnections_c] = {FALSE};
static basConfig_t      basServiceConfig = {(uint16_t)service_battery, 0, basValidClientList, gAppMaxConnections_c};
static disConfig_t      disServiceConfig = {(uint16_t)service_device_info};

static tmsConfig_t tmsServiceConfig = {(uint16_t)service_temperature, 0};

/* Application specific data*/
static appPeerDevice_t maPeerDeviceId[gAppMaxConnections_c];

static uint8_t gAppSerMgrIf;
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
static uint8_t mAppDSM = cPWR_DeepSleepMode;
#endif
static switch2Status_t switch2Status = mSS_PrintMenu_c;
static uint8_t menuOption = 0; 
static const char* legacyAdvStrings[] = {"\n\r 1. Start Legacy Advertising",\
                                         "\n\r 1. Stop Legacy Advertising",
                                         "\n\rConnected"};
static const char* extAdvScannStrings[] = {"\n\r 2. Start Extended Scannable Advertising",\
                                           "\n\r 2. Stop Extended Scannable Advertising"};
static const char* extAdvConnStrings[] = {"\n\r 3. Start Extended Connectable Advertising",\
                                          "\n\r 3. Stop Extended Connectable Advertising",
                                          "\n\rConnected"};

static const char* extAdvNonConnNonScanStrings[] = {"\n\r 4. Start Extended Non Connectable Non Scannable Advertising",\
        "\n\r 4. Stop Extended Non Connectable Non Scannable Advertising \n\r 6. Change Data for Non Connectable Non Scannable Advertising"};

static const char* periodicAdvStrings[] = {"\n\r 5. Start Periodic Advertising",\
        "\n\r 5. Stop Periodic Advertising \n\r 7. Change Data for Periodic Advertising"};

static const char** maMenu[]= {legacyAdvStrings, extAdvScannStrings, extAdvConnStrings, extAdvNonConnNonScanStrings, periodicAdvStrings};
static char* menuOptions[] ={"\r 1","\r 2", "\r 3", "\r 4", "\r 5", "\r 6", "\r 7"};
static advHandleStatus_t maAdvHandle[mNumberOfAdvHandles_c] = {madvHandle_Available_c, madvHandle_Available_c};
static advIndex_t maLastAdvIndexForThisHandle[mNumberOfAdvHandles_c]={ mAdvIndexMax_c, mAdvIndexMax_c};
static gapExtAdvertisingParameters_t* maPExtAdvParam[] = {&gExtAdvParamsLegacy, &gExtAdvParamsScannable, &gExtAdvParamsConnectable, &gExtAdvParamsNonConnNonScann }; 

static gapAdvertisingData_t* maPExtAdvData[] = {&gAppAdvertisingData , NULL, &gAppExtAdvDataConnectable, &gAppExtAdvDataId1NonConnNonScan }; 
static gapAdvertisingData_t* maPExtScanData[] = {&gAppScanRspData , &gAppExtAdvDataScannable, NULL, &gAppScanRspData }; 
static gapAdvertisingData_t* maPPeriodicAdvData[] = {&gAppExtAdvDataId1Periodic, &gAppExtAdvDataId2Periodic};
static gapAdvertisingData_t* maPExtAdvDataNonConnNonScan[] = {&gAppExtAdvDataId1NonConnNonScan, &gAppExtAdvDataId2NonConnNonScan};
static uint8_t mPeriodicAdvDataIndex = 0;
static uint8_t mExtAdvDataNCNSIndex = 0;

static const char* advTypeStrings[] = {"\n\rLegacy Advertising",\
                                       "\n\rExtended Scanable Advertising",\
                                       "\n\rExtended Connectable Advertising",\
                                       "\n\rExtended Non Connectable Non Scanable Advertising",\
                                       "\n\rExtended Periodic Advertising"}; 

#if mAE_PeripheralDebug_c
static char const* genericCBCKStrings[] = {\
    "gInitializationComplete_c",\
    "gInternalError_c",\
    "gAdvertisingSetupFailed_c",\
    "gAdvertisingParametersSetupComplete_c",\
    "gAdvertisingDataSetupComplete_c",\
    "gWhiteListSizeRead_c",\
    "gDeviceAddedToWhiteList_c",
    "gDeviceRemovedFromWhiteList_c",\
    "gWhiteListCleared_c",\
    "gRandomAddressReady_c",\
    "gCreateConnectionCanceled_c",\
    "gPublicAddressRead_c",\
    "gAdvTxPowerLevelRead_c",\
    "gPrivateResolvableAddressVerified_c",\
    "gRandomAddressSet_c",\
    "gLeScPublicKeyRegenerated_c",\
    "gLeScLocalOobData_c",\
    "gHostPrivacyStateChanged_c",\
    "gControllerPrivacyStateChanged_c",\
    "gControllerTestEvent_c",\
    "gTxPowerLevelSetComplete_c",\
    "gLePhyEvent_c",\
    "gControllerNotificationEvent_c",\
    "gBondCreatedEvent_c",\
    "gChannelMapSet_c",\
    "gExtAdvertisingParametersSetupComplete_c",\
    "gExtAdvertisingDataSetupComplete_c",\
    "gExtAdvertisingSetRemoveComplete_c",\
    "gPeriodicAdvParamSetupComplete_c",\
    "gPeriodicAdvDataSetupComplete_c",\
    "gPeriodicAdvListUpdateComplete_c",\
    "gPeriodicAdvCreateSyncCancelled_c",\
    "gTxEntryAvailable_c"};

static char const* advertisingCBCKStrings[] = {\
    "gAdvertisingStateChanged_c",\
    "gAdvertisingCommandFailed_c",\
    "gExtAdvertisingStateChanged_c",\
    "gAdvertisingSetTerminated_c",\
    "gExtScanNotification_c",\
    "gPeriodicAdvertisingStateChanged_c"};
#endif
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);
static void BleApp_Config(void);

/* Timer Callbacks */
static void DisconnectTimerCallback(void *pParam);

static void BleApp_SendTemperature(deviceId_t deviceId);

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
static void SleepTimeoutSequence(void);
#endif

static void AppPrintLePhyEvent(gapPhyEvent_t* pPhyEvent);
static void Uart_PrintMenu(void *pData);
static void AppPrintString(const char* pBuff);
static void AppPrintDec(uint32_t nb);
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
static void PWR_SetApplicationDeepSleepMode(void);
#else
#define PWR_SetApplicationDeepSleepMode()
#endif
static void Key_HandleOption(void *pData);
static void AppPrintStringCallback(void* pData);
static uint8_t Alloc_AdvHandler(void);
static void Free_AdvHandler(uint8_t advHandle);
static bool_t AllAdvertisingsAreOff(void);
static apiReqStatus_t ExtAdvAPIRequest(advIndex_t advIndex);
static void  FreeExtAdvAPI(void);
static void  EndSequence(void);
static bool_t Add_PeerDevice(deviceId_t deviceId);
static void Remove_PeerDevice(deviceId_t deviceId);
static uint8_t Get_PeerDeviceIndex(deviceId_t deviceId);
static void Disconnect_AllPeerDevices(void);
static uint8_t NumberOf_PeerDevices(void);
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
    /* Initialize application support for drivers */
    BOARD_InitAdc();

#if (defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
    gAppSerMgrIf = BOARD_GetSerialManagerInterface();
#else
    SerialManager_Init();
    (void)Serial_InitInterface(&gAppSerMgrIf, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    (void)Serial_SetBaudRate(gAppSerMgrIf, APP_SERIAL_INTERFACE_SPEED);    
#endif

}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(uint8_t mode)
{
    switch(mode)
    {
    case (uint8_t)mLegacyAdvIndex_c:
    case (uint8_t)mExtAdvScannIndex_c:
    case (uint8_t)mExtAdvConnIndex_c:
    case (uint8_t)mExtAdvNonConnNonScanIndex_c:
        if(ExtAdvAPIRequest((advIndex_t)mode) == gApiReq_Denied_c)
        {
            AppPrintString("\n\rAnother Advertising Operation in Progress. Try later...");
        }   
        else
        {
            if (maAdvStatus[mode] == mAdvStatus_Off_c)
            {
                maPExtAdvParam[mode]->handle = Alloc_AdvHandler();
                if(maPExtAdvParam[mode]->handle == mInvalidAdvHandle)
                {
                    AppPrintString("\n\r No Advertising Handle Available. Stop Another Advertising");
                    FreeExtAdvAPI();
                }
                else
                {
                    PWR_SetApplicationDeepSleepMode();             
                    if(((uint8_t)maLastAdvIndexForThisHandle[maPExtAdvParam[mode]->handle] == mode) || (maLastAdvIndexForThisHandle[maPExtAdvParam[mode]->handle] == mAdvIndexMax_c))
                    {
                        if(gBleSuccess_c == Gap_SetExtAdvertisingParameters(maPExtAdvParam[mode]))
                        {
                            mExtAdvSequence = mExtAdvSeq_Start_c;
                        }  
                        else
                        {
                            AppPrintString("\n\rGap_SetExtAdvertisingParameters failed");
                            Free_AdvHandler(maPExtAdvParam[mode]->handle); 
                            FreeExtAdvAPI();
                        }
                    }
                    else
                    {
                        if(gBleSuccess_c == Gap_RemoveAdvSet(maPExtAdvParam[mode]->handle))
                        {
                            mExtAdvSequence = mExtAdvSeq_Start_c;
                        }
                        else
                        {
                            AppPrintString("\n\rGap_RemoveAdvSet failed");
                            Free_AdvHandler(maPExtAdvParam[mode]->handle); 
                            FreeExtAdvAPI();
                        }
                    }
                }
            }
            else if(maAdvStatus[mode] ==  mAdvStatus_On_c)
            {
                if((mode == (uint8_t)mExtAdvNonConnNonScanIndex_c) && (maAdvStatus[mPeriodicAdvIndex_c] == mAdvStatus_On_c) )
                {
                    AppPrintString("\n\rPeriodic Advertising is ON. Stop the Periodic Advertising First");
                    FreeExtAdvAPI();
                }
                else
                {
                    if(gBleSuccess_c == Gap_StopExtAdvertising(maPExtAdvParam[mode]->handle))
                    {
                        mExtAdvSequence = mExtAdvSeq_Stop_c;
                    }
                    else
                    {
                        AppPrintString("\n\rGap_StopExtAdvertising failed");
                        FreeExtAdvAPI();
                    }
                }
            }
            else
            {
                FreeExtAdvAPI();
            }
        }
        break;
        
    case (uint8_t)mPeriodicAdvIndex_c:
        
        if(maAdvStatus[mExtAdvNonConnNonScanIndex_c] !=  mAdvStatus_On_c)
        {
            AppPrintString("\n\rThis Option Requires Extended Non Connectable Non Scanable Advertising to be On   ");
        }
        else
        {
            if(ExtAdvAPIRequest((advIndex_t)mode) == gApiReq_Denied_c)
            {
                AppPrintString("\n\rAnother Advertising Operation in Progress. Try later...");
            } 
            else
            {
                if(maAdvStatus[mode] ==  mAdvStatus_Off_c)
                {
                    gPeriodicAdvParams.handle = gExtAdvParamsNonConnNonScann.handle;
                    if(Gap_SetPeriodicAdvParameters(&gPeriodicAdvParams) == gBleSuccess_c )
                    {
                        mExtAdvSequence = mExtAdvSeq_Start_c;
                    }
                    else
                    {
                        AppPrintString("\n\rGap_SetPeriodicAdvParameters failed");
                        FreeExtAdvAPI();
                    }
                }
                else if(maAdvStatus[mode] ==  mAdvStatus_On_c)
                {
                    if(Gap_StopPeriodicAdvertising(gPeriodicAdvParams.handle) != gBleSuccess_c )
                    {
                        AppPrintString("\n\rGap_StopPeriodicAdvertising failed");
                        FreeExtAdvAPI();
                    }
                    else
                    {
                        mExtAdvSequence = mExtAdvSeq_Stop_c;
                    }
                }
                else
                {
                    FreeExtAdvAPI();
                }
            }
        }
        break;
    case mChangePerAdvDataOption_c:
        if(ExtAdvAPIRequest(mPeriodicAdvIndex_c) == gApiReq_Denied_c)
        {
            AppPrintString("\n\rAnother Advertising Operation in Progress. Try later...");
        } 
        else
        {
            mPeriodicAdvDataIndex ^= 1U;
            if(Gap_StopPeriodicAdvertising(gPeriodicAdvParams.handle) != gBleSuccess_c )
            {
                mPeriodicAdvDataIndex ^= 1U;
                AppPrintString("\n\rGap_StopPeriodicAdvertising Failed");
                FreeExtAdvAPI();
            }
            else
            {
                mExtAdvSequence = mExtAdvSeq_ChangePeriodicData_c;
            }
        }
        break;
        
    case mChangeNonConnNonScannAdvDataOption_c:
        if(ExtAdvAPIRequest(mExtAdvNonConnNonScanIndex_c) == gApiReq_Denied_c)
        {
            AppPrintString("\n\rAnother Advertising Operation in Progress. Try later...");
        } 
        else
        {
            mExtAdvDataNCNSIndex ^= 1U;
            maPExtAdvData[mExtAdvAPIOwner] = maPExtAdvDataNonConnNonScan[mExtAdvDataNCNSIndex];
            
            if(gBleSuccess_c != Gap_StopExtAdvertising(maPExtAdvParam[mExtAdvAPIOwner]->handle))
            {
                AppPrintString("\n\rGap_StopExtAdvertising Failed");
                mExtAdvDataNCNSIndex ^= 1U;
                maPExtAdvData[mExtAdvAPIOwner] = maPExtAdvDataNonConnNonScan[mExtAdvDataNCNSIndex];
                FreeExtAdvAPI();
            }
            else
            {
                mExtAdvSequence = mExtAdvSeq_ChangeExtAdvData_c;
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

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    switch (events)
    {
    case gKBD_EventPressPB1_c:  /* fall-through */
    case gKBD_EventLongPB1_c:   /* fall-through */
        {
            uint8_t bmValidOption = (uint8_t)0x1fU | 
                (((maAdvStatus[mExtAdvNonConnNonScanIndex_c] == mAdvStatus_On_c)? (uint8_t)1:(uint8_t)0)<<5) | 
                    (((maAdvStatus[mPeriodicAdvIndex_c] == mAdvStatus_On_c)? (uint8_t)1:(uint8_t)0)<<6);
            do
            {
                menuOption = (menuOption + 1U)%NumberOfElements(menuOptions);
            }
            while( ((((uint8_t)1)<<menuOption) & bmValidOption) == 0U);
        }
        (void)App_PostCallbackMessage(AppPrintStringCallback, (void*)menuOptions[menuOption]);
        switch2Status = mSS_HandleOption_c;                                                                   
        break;
        
        /* Start the application */
    case gKBD_EventPressPB2_c:
        {           
            if(switch2Status == mSS_PrintMenu_c )
            {
                switch2Status = mSS_WaitOption_c;   
                menuOption = NumberOfElements(menuOptions) - 1U;
                (void)App_PostCallbackMessage(Uart_PrintMenu, NULL);
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)                
                PWR_DisallowDeviceToSleep();
#endif                
            }
            else if(switch2Status == mSS_HandleOption_c )
            {
                switch2Status = mSS_PrintMenu_c;
                (void)App_PostCallbackMessage(Key_HandleOption, &menuOption);
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)                
                PWR_AllowDeviceToSleep();
#endif                
            }
            else
            {
                ;/*misra compliance purposes*/
            }
        }
        break;
        
        /* Disconnect  */
    case gKBD_EventLongPB2_c:
        {
            Disconnect_AllPeerDevices();        
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
* \brief        Handles BLE generic callback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    bleResult_t bleResult;
    
#if mAE_PeripheralDebug_c    
    AppPrintString("\n\rGeneric Callback - ");
    AppPrintString(genericCBCKStrings[pGenericEvent->eventType]);    
#endif
    /* Call BLE Conn Manager */      
    BleConnManager_GenericEvent(pGenericEvent);
    switch (pGenericEvent->eventType)
    {
    case gInitializationComplete_c:
        {
            /* Configure application and start services */
            BleApp_Config();
        }
        break;
    case gAdvertisingParametersSetupComplete_c:
        break;
    case gAdvertisingDataSetupComplete_c:
        break;
    case gExtAdvertisingParametersSetupComplete_c:
        {
            if(gBleSuccess_c != Gap_SetExtAdvertisingData(maPExtAdvParam[mExtAdvAPIOwner]->handle, maPExtAdvData[mExtAdvAPIOwner], maPExtScanData[mExtAdvAPIOwner]))
            {
                AppPrintString("\n\rGap_SetExtAdvertisingData failed");
                EndSequence();
            }
            else
            {
                maLastAdvIndexForThisHandle[maPExtAdvParam[mExtAdvAPIOwner]->handle] = mExtAdvAPIOwner;
            }
        }
        break;
    case gExtAdvertisingDataSetupComplete_c:
        {
            if(gBleSuccess_c != App_StartExtAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback, maPExtAdvParam[mExtAdvAPIOwner]->handle, gBleExtAdvNoDuration_c, gBleExtAdvNoMaxEvents_c))
            {
                AppPrintString("\n\rApp_StartExtAdvertising failed");
                EndSequence();
            }
            else
            {
                maLastAdvIndexForThisHandle[maPExtAdvParam[mExtAdvAPIOwner]->handle] = mExtAdvAPIOwner;
            }
        }
        break;
    case gPeriodicAdvParamSetupComplete_c:
        {
            bleResult = Gap_SetPeriodicAdvertisingData(gPeriodicAdvParams.handle, maPPeriodicAdvData[mPeriodicAdvDataIndex]);
            if(gBleSuccess_c != bleResult)
            {
                AppPrintString("\n\r Gap_SetPeriodicAdvertisingData Failed");
                EndSequence();
            }
        }
        break;
    case gPeriodicAdvDataSetupComplete_c:
        {
            if(gBleSuccess_c != Gap_StartPeriodicAdvertising(gPeriodicAdvParams.handle))
            {
                AppPrintString("\n\r Gap_StartPeriodicAdvertising Failed");
                EndSequence();
            }                
        }
        break;
    case gAdvertisingSetupFailed_c:
        EndSequence();
        break;
    case gExtAdvertisingSetRemoveComplete_c:
        AppPrintString(advTypeStrings[maLastAdvIndexForThisHandle[maPExtAdvParam[mExtAdvAPIOwner]->handle]]); 
        AppPrintString(" Removed" );     
        maLastAdvIndexForThisHandle[maPExtAdvParam[mExtAdvAPIOwner]->handle] = mAdvIndexMax_c;
        if(mExtAdvSequence == mExtAdvSeq_Start_c)
        {
            if(gBleSuccess_c != Gap_SetExtAdvertisingParameters(maPExtAdvParam[mExtAdvAPIOwner]))
            {
                AppPrintString("\n\rGap_SetExtAdvertisingParameters failed");
                EndSequence();
            }
        }
        else
        {
            maAdvStatus[mExtAdvAPIOwner] = mAdvStatus_Off_c;
            EndSequence();
        }
        break;
    case gLePhyEvent_c:
        if(pGenericEvent->eventData.phyEvent.phyEventType == gPhyUpdateComplete_c )
        {
            AppPrintLePhyEvent(&pGenericEvent->eventData.phyEvent);
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
    /* Common GAP configuration */
    BleConnManager_GapCommonConfig();
    
    /* Register for callbacks*/
    (void)App_RegisterGattServerCallback(BleApp_GattServerCallback);
    
    FLib_MemSet (maAdvStatus, (uint8_t)mAdvStatus_Off_c, sizeof(maAdvStatus));
    
    /* Start services */
    tmsServiceConfig.initialTemperature = (int16_t)(100 * BOARD_GetTemperature());
    (void)Tms_Start(&tmsServiceConfig);
    
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_Start(&basServiceConfig);
    (void)Dis_Start(&disServiceConfig);
    
    /* Allocate application timer */
    {
        uint8_t i;
        for( i = 0; i < gAppMaxConnections_c; i++ )
        {
            maPeerDeviceId[i].deviceId = gInvalidDeviceId_c;
            maPeerDeviceId[i].timerId = TMR_AllocateTimer();
            if( maPeerDeviceId[i].timerId == gTmrInvalidTimerID_c )
            {
                panic(0,0,0,0);
            }
        }
    }
    AppPrintString("\n\rExtended Advertising Application - Peripheral");
    AppPrintString("\n\rPress WAKESW to see the menu");
    
    /* Set low power mode */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
    mAppDSM = cPWR_DeepSleepMode;
    PWR_AllowDeviceToSleep();
#else
    LED_StopFlash(LED_ALL);
    Led1On();
    Led2On();
#endif
}


/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
#if mAE_PeripheralDebug_c    
    AppPrintString("\n\rAdvertising Callback - ");
    AppPrintString(advertisingCBCKStrings[pAdvertisingEvent->eventType]);    
#endif    
    switch (pAdvertisingEvent->eventType)
    {
        
    case gAdvertisingStateChanged_c:
        
        break;
        
    case gAdvertisingCommandFailed_c:
        {
            /* Panic UI */
            EndSequence();
            panic(0,0,0,0);
        }
        break;
        
    case gExtAdvertisingStateChanged_c:
        
        if(maAdvStatus[mExtAdvAPIOwner] == mAdvStatus_On_c)
        {
            maAdvStatus[mExtAdvAPIOwner] = mAdvStatus_Off_c;
            AppPrintString(advTypeStrings[mExtAdvAPIOwner]); 
            AppPrintString(" Stopped" );
            if(mExtAdvSequence == mExtAdvSeq_ChangeExtAdvData_c)
            {
                if(gBleSuccess_c != Gap_SetExtAdvertisingData(maPExtAdvParam[mExtAdvAPIOwner]->handle, maPExtAdvData[mExtAdvAPIOwner], maPExtScanData[mExtAdvAPIOwner]))
                {
                    AppPrintString("\n\rGap_SetExtAdvertisingData failed");
                    EndSequence();
                }
            }
            if(mExtAdvSequence == mExtAdvSeq_Stop_c)
            {
                maAdvStatus[mExtAdvAPIOwner] = mAdvStatus_Off_c;
                EndSequence();
            }
        }
        else 
        {
#if !defined(cPWR_UsePowerDownMode) || (cPWR_UsePowerDownMode == 0)
            Led1Flashing();
#endif            
            maAdvStatus[mExtAdvAPIOwner] = mAdvStatus_On_c;
            maLastAdvIndexForThisHandle[maPExtAdvParam[mExtAdvAPIOwner]->handle] = mExtAdvAPIOwner;
            AppPrintString(advTypeStrings[mExtAdvAPIOwner]); 
            AppPrintString(" Started on handle " );
            AppPrintDec((uint32_t)maPExtAdvParam[mExtAdvAPIOwner]->handle);
            if(mExtAdvSequence == mExtAdvSeq_ChangeExtAdvData_c)
            {
                AppPrintString(advTypeStrings[mExtAdvAPIOwner]); 
                AppPrintString(" Data Changed" );    
            }
            EndSequence();
        }
        break;
    case gPeriodicAdvertisingStateChanged_c:    
        
        if(maAdvStatus[mExtAdvAPIOwner] == mAdvStatus_On_c)
        {
            maAdvStatus[mExtAdvAPIOwner] = mAdvStatus_Off_c;
            AppPrintString(advTypeStrings[mExtAdvAPIOwner]); 
            AppPrintString(" Stopped" );    
            if(mExtAdvSequence == mExtAdvSeq_ChangePeriodicData_c)
            {
                if(gBleSuccess_c !=  Gap_SetPeriodicAdvertisingData(gPeriodicAdvParams.handle, maPPeriodicAdvData[mPeriodicAdvDataIndex]))
                {
                    AppPrintString("\n\rGap_SetPeriodicAdvertisingData failed");
                    EndSequence();
                }
            }
            else
            {
                EndSequence();
            }
        }
        else 
        {
            maAdvStatus[mExtAdvAPIOwner] = mAdvStatus_On_c;
            AppPrintString(advTypeStrings[mExtAdvAPIOwner]); 
            AppPrintString(" Started" );    
            if(mExtAdvSequence == mExtAdvSeq_ChangePeriodicData_c)
            {
                AppPrintString(advTypeStrings[mExtAdvAPIOwner]); 
                AppPrintString(" Data Changed" );    
            }
            EndSequence();    
        }            
        break;
    case gAdvertisingSetTerminated_c:
        {
            uint8_t i;
            for( i=0; i < (uint8_t)mPeriodicAdvIndex_c; i++)
            {
                if((maAdvStatus[i] ==  mAdvStatus_On_c) && (maPExtAdvParam[i]->handle == pAdvertisingEvent->eventData.advSetTerminated.handle ))
                {
                    AppPrintString(advTypeStrings[i]); 
                    AppPrintString(" Terminated on handle " );
                    AppPrintDec((uint32_t)maPExtAdvParam[i]->handle);
                    maAdvStatus[i] =  mAdvStatus_Off_c;
                    Free_AdvHandler(maPExtAdvParam[i]->handle);
#if !defined(cPWR_UsePowerDownMode) || (cPWR_UsePowerDownMode == 0)
                    if(AllAdvertisingsAreOff())
                    {
                        StopLed1Flashing();        
                        Led1On();
                    }
#endif                     
                    break;
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

/*! *********************************************************************************
* \brief        Handles BLE Connection callback from host stack.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pConnectionEvent    Pointer to gapConnectionEvent_t.
********************************************************************************** */
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    /* Connection Manager to handle Host Stack interactions */
    BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);
    
    switch (pConnectionEvent->eventType)
    {
    case gConnEvtConnected_c:
        {
            /* Advertising stops when connected */
            /* Subscribe client*/
            (void)Add_PeerDevice(peerDeviceId);
            (void)Bas_Subscribe(&basServiceConfig, peerDeviceId);
            (void)Tms_Subscribe(peerDeviceId);
            (void)Serial_Print(gAppSerMgrIf, "\n\rConnected!\n\r", gAllowToBlock_d);
            /* Set low power mode */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
            (void)PWR_ChangeDeepSleepMode(gAppDeepSleepMode_c);
            mAppDSM = gAppDeepSleepMode_c;
#if (!defined(CPU_MKW37A512VFT4) && !defined(CPU_MKW37Z512VFT4) && !defined(CPU_MKW38A512VFT4) && !defined(CPU_MKW38Z512VFT4) && !defined(CPU_MKW39A512VFT4) && !defined(CPU_MKW39Z512VFT4))
            PWR_AllowDeviceToSleep();
#endif /* CPU_MKW37xxx, CPU_MKW38xxx and CPU_MKW39xxx*/
#else
            /* UI */            
            Led2Flashing();
#endif


        }
        break;
        
    case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            Remove_PeerDevice(peerDeviceId);
            (void)Bas_Unsubscribe(&basServiceConfig, peerDeviceId);
            (void)Tms_Unsubscribe();
            (void)Serial_Print(gAppSerMgrIf, "\n\rDisconnected!\n\r", gAllowToBlock_d);
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
            /* Go to sleep */
            if(AllAdvertisingsAreOff()&& (NumberOf_PeerDevices() == 0U))
            {
                SleepTimeoutSequence();
            }
#else
            if(NumberOf_PeerDevices() == 0U)
            {
                /* UI */                
                StopLed2Flashing();        
                Led2On();                
            }
#endif
            
        }
        break;
        
    case gConnEvtEncryptionChanged_c:   /* Fall-through */
    default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles GATT server callback from host stack.
*
* \param[in]    deviceId        Peer device ID.
* \param[in]    pServerEvent    Pointer to gattServerEvent_t.
********************************************************************************** */
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent)
{
    switch (pServerEvent->eventType)
    {
    case gEvtCharacteristicCccdWritten_c:
        {
            /* Notify the temperature value when CCCD is written */
            BleApp_SendTemperature(deviceId);
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
* \brief        Handles disconnect timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void DisconnectTimerCallback(void* pParam)
{
    /* Terminate connection */
    appPeerDevice_t* pAppPeerDevice = (appPeerDevice_t*)pParam;
    if (pAppPeerDevice->deviceId != gInvalidDeviceId_c)
    {
        (void)Gap_Disconnect(pAppPeerDevice->deviceId);
    }
}

/*! *********************************************************************************
* \brief        Sends temperature value over-the-air.
*
********************************************************************************** */
static void BleApp_SendTemperature(deviceId_t deviceId)
{
    uint8_t peerIndex;
    peerIndex = Get_PeerDeviceIndex(deviceId);
    if(peerIndex < gAppMaxConnections_c)
    {
        (void)TMR_StopTimer(maPeerDeviceId[peerIndex].timerId);
        
        /* Update with initial temperature */
        (void)Tms_RecordTemperatureMeasurement((uint16_t)service_temperature,
                                               (int16_t)(BOARD_GetTemperature() * 100));
         
         /* Start Sleep After Data timer */
         (void)TMR_StartLowPowerTimer(maPeerDeviceId[peerIndex].timerId,
                                      gTmrLowPowerSecondTimer_c,
                                      TmrSeconds(gGoToSleepAfterDataTime_c),
                                      DisconnectTimerCallback, &maPeerDeviceId[peerIndex]);
    }
}

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
static void SleepTimeoutSequence(void)
{
    (void)PWR_ChangeDeepSleepMode(cPWR_DeepSleepMode);
    mAppDSM = cPWR_DeepSleepMode;
}
#endif

/*! *********************************************************************************
* \brief        Prints a string.
*
********************************************************************************** */
static void AppPrintString(const char* pBuff)
{
    (void)Serial_Print(gAppSerMgrIf, pBuff, gAllowToBlock_d);
}
/*! *********************************************************************************
* \brief        Prints a uint32_t value in decimal.
*
********************************************************************************** */
static void AppPrintDec(uint32_t nb)
{
    (void)Serial_PrintDec(gAppSerMgrIf, nb);
}
/*! *********************************************************************************
* \brief        Prints a string from the app task.
*
********************************************************************************** */
static void AppPrintStringCallback(void* pData)
{
    const char* pBuff = (const char*)pData;
    (void)Serial_Print(gAppSerMgrIf, pBuff, gAllowToBlock_d);
}
/*! *********************************************************************************
* \brief        Prints phy event.
*
********************************************************************************** */
static void PrintLePhyEvent(void(*pfPrint)(const char* pChar),gapPhyEvent_t* pPhyEvent)
{
    /* String dictionary corresponding to gapLePhyMode_t */
    static const char* mLePhyModeStrings[] =
    {
        "Invalid\n\r",
        "1M\n\r",
        "2M\n\r",
        "Coded\n\r",
    };
    uint8_t txPhy = (pPhyEvent->txPhy <= (uint8_t)gLePhyCoded_c) ? pPhyEvent->txPhy : (0U);
    uint8_t rxPhy = (pPhyEvent->rxPhy <= (uint8_t)gLePhyCoded_c) ? pPhyEvent->rxPhy : (0U);
    pfPrint("Phy Update Complete.\n\r");
    pfPrint("TxPhy ");
    pfPrint(mLePhyModeStrings[txPhy]);
    pfPrint("RxPhy ");
    pfPrint(mLePhyModeStrings[rxPhy]);
}

/*! *********************************************************************************
* \brief        Prints phy event.
*
********************************************************************************** */
static void AppPrintLePhyEvent(gapPhyEvent_t* pPhyEvent)
{
    PrintLePhyEvent(AppPrintString, pPhyEvent);
}

/*! *********************************************************************************
* \brief        prints application menu.
*
* \param[in]    pData        Parameters.
********************************************************************************** */
static void Uart_PrintMenu(void *pData)
{
    uint8_t i;
    AppPrintString("\n\r Menu");
    for( i = 0 ; i < (uint8_t)mAdvIndexMax_c; i++)
    {
        AppPrintString((maMenu[i])[(uint8_t)maAdvStatus[i]]);
    }
    AppPrintString("\n\rPress OPTSW to choose an option\n\rThen confirm it with the WAKESW\n\r");
}

/*! *********************************************************************************
* \brief        Handles the keyboard option.
*
* \param[in]    pData        Parameters.
********************************************************************************** */
static void Key_HandleOption(void *pData)
{
    uint8_t keyOption = *((uint8_t*)pData);    
    BleApp_Start(keyOption);    
}

/*! *********************************************************************************
* \brief        Set Application deep sleep mode.
*
* \param[in]    pData        Parameters.
********************************************************************************** */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
static void PWR_SetApplicationDeepSleepMode(void)
{
    if(mAppDSM != (uint8_t)gAppDeepSleepMode_c)
    {
        (void)PWR_ChangeDeepSleepMode(gAppDeepSleepMode_c);
        mAppDSM = (uint8_t)gAppDeepSleepMode_c;
    }
}               
#endif                    


/*! *********************************************************************************
* \brief        Allocates the first available advertising handler .
*
* \param[in]    none
********************************************************************************** */
static uint8_t Alloc_AdvHandler(void)
{
    uint8_t i, advHandle;
    advHandle = mInvalidAdvHandle;
    OSA_InterruptDisable();
    for( i=0; i < mNumberOfAdvHandles_c; i++)
    {
        if(maAdvHandle[i] == madvHandle_Available_c)
        {
            maAdvHandle[i] = madvHandle_InUse_c;
            advHandle = i;
            break;
        }
    }
    OSA_InterruptEnable();
    return advHandle;
}

/*! *********************************************************************************
* \brief        Frees the advertising handle received as parameter.
*
* \param[in]    advHandle to free 
********************************************************************************** */
static void Free_AdvHandler(uint8_t advHandle)
{
    OSA_InterruptDisable();
    if( advHandle <  mNumberOfAdvHandles_c)
    {
        if(maAdvHandle[advHandle] == madvHandle_InUse_c )
        {
            maAdvHandle[advHandle] = madvHandle_Available_c;
        }
    }
    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief        Returns whether there is an advertising started or not.
*
* \param[in]    none
********************************************************************************** */
static bool_t AllAdvertisingsAreOff(void)
{
    bool_t result = TRUE;
    uint8_t i;
    for(i = 0; i< (uint8_t)mAdvIndexMax_c; i++)
    {
        if(maAdvStatus[i] != mAdvStatus_Off_c)
        {
            result = FALSE;
            break;
        }
    }
    return result;            
}

/*! *********************************************************************************
* \brief        Function used to get access to the ext adv API.
*
* \param[in]    none
********************************************************************************** */
static apiReqStatus_t ExtAdvAPIRequest(advIndex_t advIndex)
{
    apiReqStatus_t   apiReqStatus = gApiReq_Denied_c;
    if(advIndex < mAdvIndexMax_c)
    {
        OSA_InterruptDisable();
        if(mExtAdvAPIOwner == mAdvIndexMax_c)
        {
            mExtAdvAPIOwner = advIndex;
        }
        if(mExtAdvAPIOwner == advIndex)
        {
            apiReqStatus = gApiReq_Success_c;
        }
        OSA_InterruptEnable();
    }
    return apiReqStatus;
}

/*! *********************************************************************************
* \brief        Function used to release the ext adv API.
*
* \param[in]    none
********************************************************************************** */
static void  FreeExtAdvAPI(void)
{
    mExtAdvAPIOwner = mAdvIndexMax_c;
}

/*! *********************************************************************************
* \brief        Function used to manage the end of a sequence.
*
* \param[in]    none
********************************************************************************** */
static void  EndSequence(void)
{
    mExtAdvSequence = mExtAdvSeq_Idle_c;
    if((mExtAdvAPIOwner < mPeriodicAdvIndex_c) && (maAdvStatus[mExtAdvAPIOwner] == mAdvStatus_Off_c))
    {
        Free_AdvHandler(maPExtAdvParam[mExtAdvAPIOwner]->handle);
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
        /* Go to sleep */
        if(AllAdvertisingsAreOff()&& (NumberOf_PeerDevices() == 0U))
        {
            SleepTimeoutSequence();    
        }
#else
        if(AllAdvertisingsAreOff())
        {
            StopLed1Flashing();        
            Led1On();
        }

#endif                 
    }
    FreeExtAdvAPI();
}

/*! *********************************************************************************
* \brief        Add peer device in the peer array. Return FALSE if there is no room in the array 
*               or the device is already there and TRUE otherwise. 
*              
* \param[in]    deviceId peer device to add
********************************************************************************** */
static bool_t Add_PeerDevice(deviceId_t deviceId)
{
    bool_t result = FALSE;
    uint8_t i;
    uint8_t firstFreeIndex = gAppMaxConnections_c;
    if( gInvalidDeviceId_c != deviceId )
    {
        result = TRUE;
        for( i = 0; i < gAppMaxConnections_c; i++ )
        {
            if(maPeerDeviceId[i].deviceId == gInvalidDeviceId_c)
            {
                if(firstFreeIndex == gAppMaxConnections_c)
                {
                    firstFreeIndex = i;
                }
            }
            else if(maPeerDeviceId[i].deviceId == deviceId)
            {
                result = FALSE;
                break;
            }
            else
            {
                ;
            }
        }
    }
    if(result == TRUE)
    {
        if(firstFreeIndex < gAppMaxConnections_c)
        {
            maPeerDeviceId[firstFreeIndex].deviceId = deviceId;
        }
        else
        {
            result = FALSE;
        }
    }
    return result;     
}

/*! *********************************************************************************
* \brief        Removes peer device from the peer array. 
*              
* \param[in]    deviceId peer device to remove
********************************************************************************** */
static void Remove_PeerDevice(deviceId_t deviceId)
{
    uint8_t i;
    if( gInvalidDeviceId_c != deviceId )
    {
        for( i = 0; i < gAppMaxConnections_c; i++ )
        {
            if(maPeerDeviceId[i].deviceId == deviceId)
            {
                maPeerDeviceId[i].deviceId = gInvalidDeviceId_c;
                break;
            }
        }
    }
}
/*! *********************************************************************************
* \brief        Returns the peer array index of the peer device. 
*              
* \param[in]    deviceId peer device to search for
********************************************************************************** */
static uint8_t Get_PeerDeviceIndex(deviceId_t deviceId)
{
    uint8_t i = gAppMaxConnections_c;
    if( gInvalidDeviceId_c != deviceId )
    {
        for( i = 0; i < gAppMaxConnections_c; i++ )
        {
            if(maPeerDeviceId[i].deviceId == deviceId)
            {
                break;
            }
        }
    }
    return i;
}

/*! *********************************************************************************
* \brief        Disconnects all peer devices in the array 
*              
* \param[in]    none
********************************************************************************** */
static void Disconnect_AllPeerDevices(void)
{
    uint8_t i;
    for( i = 0; i < gAppMaxConnections_c; i++ )
    {
        if(maPeerDeviceId[i].deviceId != gInvalidDeviceId_c)
        {
            (void)Gap_Disconnect(maPeerDeviceId[i].deviceId);
        }
    }
}

/*! *********************************************************************************
* \brief        Returns the number of peer devices in the array 
*              
* \param[in]    none
********************************************************************************** */
static uint8_t NumberOf_PeerDevices(void)
{
    uint8_t i;
    uint8_t peerNo = 0;
    for( i = 0; i < gAppMaxConnections_c; i++ )
    {
        if(maPeerDeviceId[i].deviceId != gInvalidDeviceId_c)
        {
            peerNo++;
        }
    }
 return peerNo;   
}
/*! *********************************************************************************
* @}
********************************************************************************** */
