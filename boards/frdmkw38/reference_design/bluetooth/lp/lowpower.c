/*! *********************************************************************************
* \addtogroup LowPowerRefDes
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the lowpower slave reference design application
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
#include "TimersManager.h"
#include "FunctionLib.h"
#include "MemManager.h"
#include "Panic.h"
#include "DCDC.h"
#include "GPIO_Adapter.h"
#include "gpio_pins.h"

#include "PWR_Interface.h"
#include "PWR_Configuration.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_handles.h"
#include "gatt_db_app_interface.h"

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "temperature_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "lowpower.h"

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
#include "SerialManager.h"
#endif

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

#define LP_ADV_USER_REQ_NONE            0
#define LP_ADV_USER_REQ_START           1
#define LP_ADV_USER_REQ_STOP            2

#define LP_TEMP_SERVICE_IDX       0U

#define _unused(x) ((void)(x))

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
#define BLEAPP_SERIALINIT              BleApp_SerialInit
#define BLEAPP_WRITE                   BleApp_SerialWrite
#define BLEAPP_WRITEDEC                BleApp_SerialWriteDec
#else
#define BLEAPP_SERIALINIT(...)
#define BLEAPP_WRITE(...)
#define BLEAPP_WRITEDEC(...)
#endif

#if defined(gAppStateNoActivityRamRetention_d) && (gAppStateNoActivityRamRetention_d == 1)
#define LP_SLAVE_APP_STATE_NO_ACTIVITY         PWR_APP_STATE_NO_ACTIVITY_RAM_RET
#define LP_SLAVE_APP_MESSAGE_NO_ACTIVITY       "\r\nGoing into lowpower (RAMRET)\r\n"
#else
#define LP_SLAVE_APP_STATE_NO_ACTIVITY         PWR_APP_STATE_NO_ACTIVITY
#define LP_SLAVE_APP_MESSAGE_NO_ACTIVITY       "\r\nGoing into lowpower (RAMOFF)\r\n"
#endif

#define LP_SLAVE_ADV_START_EVENT                appEvt_AdvertiseStarted_c

/* Debug Macros - stub if not defined */
#ifndef APP_DBG_LOG
#define APP_DBG_LOG(...)
#endif

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

typedef enum appEvent_tag
{
    appEvt_AdvertiseStarted_c,
    appEvt_AdvertiseStopped_c,
    appEvt_PeerConnected_c,
    appEvt_PeerDisconnected_c,
    appEvt_PairingComplete_c,
} appEvent_t;

typedef enum appSate_tag
{
    appIdle_c,
    appAdvertising_c,
    appConnected_c
} appState_t;

typedef struct appPeerInfo_tag
{
    deviceId_t          deviceId;
    bool_t              isPaired;
    bool_t              tempNotifSent;
    bool_t              battNotifSent;
    tmrTimerID_t        timeoutTimer;
} appPeerInfo_t;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* Application specific data*/
static appPeerInfo_t    appPeerInformation[gAppMaxConnections_c];
static appState_t       appState;
static bool_t           appAdvOn = FALSE;
static tmrTimerID_t     appAdvTimerId;
static uint8_t          appAdvUserRequest;
static uint8_t          appConnectionNumber = 0U;

#if defined(gAppAdvOnWakeUp) && (gAppAdvOnWakeUp == 1)
/* gAppAdvertisingInterval in number of slots (0.625ms) to be converted in ms    */
static const uint32_t   appAdvIntervalMs = ((uint32_t)gAppAdvertisingInterval * 500U)/800U;
#endif
#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1)
static bool_t           appWaitForControllerPrivacy = FALSE;
#endif

/* Service Data*/
static bool_t           basValidClientList[gAppMaxConnections_c] = {FALSE};
static basConfig_t      basServiceConfig = {(uint16_t)service_battery, 0, basValidClientList, gAppMaxConnections_c};
static disConfig_t      disServiceConfig = {(uint16_t)service_device_info};
static tmsConfig_t      tmsServiceConfig = {(uint16_t)service_temperature, 0};

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
static uint8_t          gLpSerMgrIf;
#if defined(gAppSerialManagerKeepActiveMs_c) && (gAppSerialManagerKeepActiveMs_c > 0)
static tmrTimerID_t     appSerialTimerId;
#endif
#endif

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)     \
      && defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
/* Timer to disable lowpower on connection establishement until 32Khz is running  */
static tmrTimerID_t appRamoffWait32kInConnectedMode;
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

/* App Private Functions */
static void BleApp_Config(void);
static void BleApp_Start(void);
static void BleApp_Advertise(void);
static void BleApp_StopAdvertise(void);
static void BleApp_InitPeerInformation(void);
static void BleApp_ResetPeerDeviceInformation(deviceId_t peerDeviceId);
static void BleApp_HandleConnection(deviceId_t peerDeviceId);
static void BleApp_HandleDisconnection(deviceId_t peerDeviceId);
#if defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U)
static void BleApp_HandlePairingComplete(deviceId_t peerDeviceId);
#endif
static bool BleApp_HandleLowpowerState(void);
static void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event);
static bleResult_t BleApp_SendBattery(deviceId_t peerDeviceId);
static bleResult_t BleApp_SendTemperature(deviceId_t peerDeviceId);
static bool_t BleApp_TemperatureNotificationEnabled(void);
static bool_t BleApp_BatteryNotificationEnabled(deviceId_t peerDeviceId);
static void BleApp_SubscribeClientToService(deviceId_t peerDeviceId);
static void BleApp_UnsubscribeClientFromService(deviceId_t peerDeviceId);
static void BleApp_PrintLePhyEvent(gapPhyEvent_t* pPhyEvent);

/* App context Callbacks */
static void BleApp_StateMachineCallback(appCallbackParam_t pParam);
static void BleApp_LowpowerStateCallback(appCallbackParam_t pParam);
#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
static void BleApp_PrintAdvMessageCallback(appCallbackParam_t pParam);
#endif

/* Timer Callbacks */
static void AdvertisingTimerCallback(void *pParam);
static void DisconnectTimerCallback(void *pParam);
#if defined(gAppWakeUpTimerAfterNoActivityMs) && (gAppWakeUpTimerAfterNoActivityMs != 0U)
static void WakeUpTimerCallback(void* pParam);
#endif

/* Serial Private Functions */
#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
static void BleApp_SerialInit(void);
static void BleApp_SerialWrite(const char* pString);
static void BleApp_SerialWriteDec(uint32_t nb);
static void BleApp_SerialRxCb( void *params );
#endif

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
    PWRLib_WakeupReason_t  wkup_reason;
    uint16_t               resetStatus;

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    BLEAPP_SERIALINIT();
#endif

    /* The application may want to do specific tasks on wakeup from specific reset, if so, do it here */
    resetStatus = PWR_GetSystemResetStatus();
    if( (resetStatus & gPWRLib_ResetStatus_WAKEUP) && ((resetStatus & gPWRLib_ResetStatus_PIN) != gPWRLib_ResetStatus_PIN) )
    {
        BLEAPP_WRITE("\r\nWake up from RAMOFF\r\n");
    }
    else
    {
        BLEAPP_WRITE("\r\nLP Slave reference design Application !!\r\n");
        BLEAPP_WRITE("Connect with a LP master or Temperature collector device\r\n");
        BLEAPP_WRITE("Type in the terminal to check echo\r\n");
    }

    appState = appIdle_c;
    BleApp_InitPeerInformation();

    /* check the wakeup reason and call the callback if needed.
       The application is required to do that as the the lowpower module in framework only
          calls the callbacks when waking up from a RAM retention lowpower state when all the ressources
          are initialized and safe to call. From a wakeup from a RAM off state (POR, pswitch, VLLS0/1),
          the callbacks can only be called when all the system are initalized , ie when BleApp_Init()
          is called here */
    wkup_reason = PWR_GetWakeUpReason();

    if (wkup_reason.Bits.FromKeyBoard == 1)
    {
        APP_DBG_LOG("Wakeup from KBD");

        /* You may want to call the Handle key callback here */
    }

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    if (wkup_reason.Bits.FromSerial == 1)
    {
        APP_DBG_LOG("Wakeup from Serial");

        /* Call Application callback */
        BleApp_SerialRxCb(NULL);
    }
#endif
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    APP_DBG_LOG("%d", events);
    switch (events)
    {
    case 1:
        /* re-enable lowpower in case it has been disabled(debug purpose) */
        if ( PWR_CheckIfDeviceCanGoToSleep() == FALSE )
        {
            PWR_AllowDeviceToSleep();
            App_PostCallbackMessage(BleApp_LowpowerStateCallback, NULL);
        }
        break;
    case 2:
       {
            /* If SW2 is held and lowpower enabled, this function will disable lowpower
               If lowpower is disable, this function will enable it
               This is useful to prevent flashing issues with CMSIS while using lowpower */
            bool changed;
            changed = BleApp_HandleLowpowerState();
            if ( changed == TRUE )
            {
                break;
            }

            if(!appAdvOn)
            {
#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1)
                /* if privacy has been disabled when going to no activity RAM RET,
                   we need to restart the privacy to restart the 15min counter.
                   no required when comming from no activity RAM OFF */
                (void)BleConnManager_EnablePrivacy();
#endif
                /* send event to State Machine callback to start advertising  */
                App_PostCallbackMessage(BleApp_StateMachineCallback, (void*)LP_SLAVE_ADV_START_EVENT);
            }
            else
            {
#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
                /* In RAMOFF mode, advertising is always on, so after a wakeup with button
                   Advertising callback will not be called, we need to print the information here */
                App_PostCallbackMessage(BleApp_PrintAdvMessageCallback, NULL);
#else
                /* send event to State Machine callback to stop advertising and go to lowpower */
                App_PostCallbackMessage(BleApp_StateMachineCallback, (void*)appEvt_AdvertiseStopped_c);
#endif
            }
        }
    }

    DBG_LOG_DUMP(FALSE);
}

/*! *********************************************************************************
* \brief        Handles BLE generic callback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
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
        {
            (void)Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
        }
        break;

        case gAdvertisingDataSetupComplete_c:
        {
            /* Start advertising if data and parameters were successfully set */
            (void)App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
        }
        break;

#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1)
        case gControllerPrivacyStateChanged_c:
        {
            if(pGenericEvent->eventData.newControllerPrivacyState == TRUE)
            {
                if(appWaitForControllerPrivacy == TRUE)
                {
                    /* Controller Privacy is ready, we can start adv */
                    BleApp_Start();
                    appWaitForControllerPrivacy = FALSE;
                }
            }
        }
        break;
#endif

#if gAppExtAdvEnable_d
        case gExtAdvertisingParametersSetupComplete_c:
        {
            (void)Gap_SetExtAdvertisingData(0,&gAppAdvertisingData, &gAppScanRspData);
        }
        break;

        case gExtAdvertisingDataSetupComplete_c:
        {
            /* Start advertising if data and parameters were successfully set */
            (void)App_StartExtAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback,0,0,0);
        }
        break;
#endif

        case gLePhyEvent_c:
        {
            if(pGenericEvent->eventData.phyEvent.phyEventType == gPhyUpdateComplete_c )
            {
                BleApp_PrintLePhyEvent(&pGenericEvent->eventData.phyEvent);
            }
        }
        break;

        case gAdvertisingSetupFailed_c:
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

/* ***********************************************************************************
*************************************************************************************
* Gatt and Att Callbacks
*************************************************************************************
*********************************************************************************** */

/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
        case gAdvertisingStateChanged_c:
#if defined(gAppExtAdvEnable_d) && (gAppExtAdvEnable_d == 1)
        case gExtAdvertisingStateChanged_c:
#endif
        {
            appAdvOn = !appAdvOn;
            switch (appAdvUserRequest)
            {
            case LP_ADV_USER_REQ_START:

                /* An ADV Start has been requested by the user */
                BLEAPP_WRITE("\r\nAdvertising...\r\n");
                APP_DBG_LOG("ADV started");

                /* First time we start the ADV => Start ADV expiration timer */
                (void)TMR_StartLowPowerTimer(appAdvTimerId,
                       gTmrLowPowerSecondTimer_c,
                       TmrSeconds(gAppAdvTimeout_c),
                       AdvertisingTimerCallback, NULL);
#if defined(gAppAdvOnWakeUp) && (gAppAdvOnWakeUp == 1)
                 /* If gAppAdvOnWakeUp is set, advertising interval is higher than
                    10.24 sec and we use Mode 9 to advertise at each wake up.
                    Here we program the max deep sleep time to the advertising interval
                    value so the device will wake up to advertise. */
                 PWR_SetDeepSleepTimeInMs(appAdvIntervalMs);
#endif

                break;

            case LP_ADV_USER_REQ_STOP:

                /* An ADV stop has been requested by the user */
                BLEAPP_WRITE("\r\nAdvertising stopped\r\n");
                APP_DBG_LOG("ADV Stopped");
                (void)TMR_StopTimer(appAdvTimerId);

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
                if( (appConnectionNumber == 0U) && PWR_CheckIfDeviceCanGoToSleep() )
                {
#if defined(gAppAdvOnWakeUp) && (gAppAdvOnWakeUp == 1)
                    /* Restore default max deep sleep time */
                    PWR_SetDeepSleepTimeInMs(cPWR_DeepSleepDurationMs);
#endif
#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1)
                /* When Privacy is enabled, a LPTMR is running to update RPA
                   To be able to shutdown 32khz osc in VLLS1, no LPTMR should run
                   Calling this function will stop the timer and allow power saving */
                (void)BleConnManager_DisablePrivacy();
#endif
#if defined(gAppWakeUpTimerAfterNoActivityMs) && (gAppWakeUpTimerAfterNoActivityMs != 0U)
                    (void)TMR_StartLowPowerTimer(appAdvTimerId,
                       gTmrLowPowerSingleShotMillisTimer_c,
                       TmrMilliseconds(gAppWakeUpTimerAfterNoActivityMs),
                       WakeUpTimerCallback, NULL);
#endif
                    /* go to lowpower right now */
                    BLEAPP_WRITE(LP_SLAVE_APP_MESSAGE_NO_ACTIVITY);
                    PWR_SetNewAppState(LP_SLAVE_APP_STATE_NO_ACTIVITY);
                }
#endif
                break;

            default:
                break;
            }

            appAdvUserRequest = LP_ADV_USER_REQ_NONE;
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            /* Panic UI */
            APP_DBG_LOG("ADV Failed");
            assert(0);
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
        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(peerDeviceId, appEvt_PairingComplete_c);
                assert(appPeerInformation[peerDeviceId].isPaired);
            }
            else
            {
                BLEAPP_WRITE("Device ");
                BLEAPP_WRITEDEC((uint32_t)appPeerInformation[peerDeviceId].deviceId);
                BLEAPP_WRITE(" - Pairing failed\r\n");
            }
        }
        break;

        case gConnEvtConnected_c:
        {
            /* Host stack automatically stops ADV when connection is established */
            appAdvOn = FALSE;

            BleApp_StateMachineHandler(peerDeviceId, appEvt_PeerConnected_c);

            /* If max connection is reached, no need to advertise anymore,
             * we can stop the adv timer */
            if(appConnectionNumber == gAppMaxConnections_c )
            {
#if (gAppMaxConnections_c > 1)
                BLEAPP_WRITE("\r\nMax connection reached\r\n");
#endif
                (void)TMR_StopTimer(appAdvTimerId);
            }
            else
            {
#if !defined(gAppRestartAdvAfterConnect) || (gAppRestartAdvAfterConnect == 0)
                /* continue to advertise */
                App_PostCallbackMessage(BleApp_StateMachineCallback, (void*)LP_SLAVE_ADV_START_EVENT);
#endif
            }
        }
        break;

        case gConnEvtDisconnected_c:
        {
            /* at this stage, connection number cannot be equal to 0 */
            assert(appConnectionNumber);

            BleApp_StateMachineHandler(peerDeviceId, appEvt_PeerDisconnected_c);

#if !defined(gAppRestartAdvAfterConnect) || (gAppRestartAdvAfterConnect == 0)
            if( (appConnectionNumber > 0) && (appConnectionNumber < gAppMaxConnections_c) )
#endif
            {
                /* need to restart advertising in case ADV was stopped
                 * when max connection has been reached */
                BleApp_StateMachineHandler(gInvalidDeviceId_c, LP_SLAVE_ADV_START_EVENT);
            }
        }
        break;

        case gConnEvtEncryptionChanged_c:   /* Fallthrough */
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
            //APP_DBG_LOG("CCCD Written");
            bleResult_t result = gBleInvalidState_c;

#if defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U)

            /* on iOS, this event is sent before bonding is complete
             * so we need to check the device is bonded before sending data
             * but in all cases it's a good habit to check the device is paired
             * before sending data */
            if( appPeerInformation[deviceId].isPaired )
#endif
            {
                /* we check what CCCD has been written */

                if(pServerEvent->eventData.charCccdWrittenEvent.handle == (uint16_t)cccd_temperature)
                {
                    /* check if the peer device is subscribed to the Temperature service */
                    if(appPeerInformation[deviceId].deviceId == LP_TEMP_SERVICE_IDX)
                    {
                        result = BleApp_SendTemperature(deviceId);
                        appPeerInformation[deviceId].tempNotifSent = TRUE;
                    }
                }
                else
                {
                    if(pServerEvent->eventData.charCccdWrittenEvent.handle == (uint16_t)cccd_battery_level)
                    {
                        /* all devices are subscribed to Battery service */
                        result = BleApp_SendBattery(deviceId);
                        appPeerInformation[deviceId].battNotifSent = TRUE;
                    }
                }
#if !defined(gAppDisconnectOnTimeoutOnly_s) || (gAppDisconnectOnTimeoutOnly_s == 0)
                /* Disconnect only if successful */
                if(result == gBleSuccess_c)
                {
                    /* if the device is subscribed to both services, need to check
                       if both notifications have been sent before disconnecting */
                    if(appPeerInformation[deviceId].deviceId == LP_TEMP_SERVICE_IDX)
                    {
                        if( (appPeerInformation[deviceId].tempNotifSent == TRUE) &&\
                            (appPeerInformation[deviceId].battNotifSent == TRUE) )
                        {
                            /* Stop disconnect timer */
                            (void)TMR_StopTimer(appPeerInformation[deviceId].timeoutTimer);

                            /* Disconnect */
                            (void)Gap_Disconnect(appPeerInformation[deviceId].deviceId);
                        }
                    }
                    else
                    {
                        if(appPeerInformation[deviceId].battNotifSent == TRUE)
                        {
                            /* Stop disconnect timer */
                            (void)TMR_StopTimer(appPeerInformation[deviceId].timeoutTimer);

                            /* Disconnect */
                            (void)Gap_Disconnect(appPeerInformation[deviceId].deviceId);
                        }
                    }
                }
#else
                NOT_USED(result);
#endif
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

/* ***********************************************************************************
*************************************************************************************
* App Private Functions
*************************************************************************************
*********************************************************************************** */

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

    /* Start services */
    tmsServiceConfig.initialTemperature = (int16_t)(100 * BOARD_GetTemperature());
    (void)Tms_Start(&tmsServiceConfig);

    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    (void)Bas_Start(&basServiceConfig);
    (void)Dis_Start(&disServiceConfig);

    /* Allocate application timer */
    appAdvTimerId = TMR_AllocateTimer();

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)     \
      && defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
    /* Disable lowpower in RAM off during ADV with Pswitch untill 32Khz is running */
    appRamoffWait32kInConnectedMode = TMR_AllocateTimer();
#endif

#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1)
    if (gcBondedDevices == 0U)
    {
        /* if no devices are bonded, we can start adv immediately */
        BleApp_Start();
    }
    else
    {
        /* will wait event from Controller before starting adv */
        appWaitForControllerPrivacy = TRUE;
        /* allow lowpower while COntroller Privacy gets ready */
        PWR_SetNewAppState(PWR_APP_STATE_ADV);
        PWR_AllowDeviceToSleep();
    }
#else
    BleApp_Start();
#endif

}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
static void BleApp_Start(void)
{
    /* Invoke app command to start advertising */
    BleApp_StateMachineHandler(gInvalidDeviceId_c, LP_SLAVE_ADV_START_EVENT);
    PWR_AllowDeviceToSleep();

    APP_DBG_LOG("");
    DBG_LOG_DUMP(FALSE);
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    bleResult_t result = gBleSuccess_c;

    if(appAdvOn == FALSE)
    {
#if gAppExtAdvEnable_d
        result = Gap_SetExtAdvertisingParameters(&gAppExtAdvParams);
#else
        result = Gap_SetAdvertisingParameters(&gAdvParams);
#endif
    }
    assert(result == gBleSuccess_c);
    _unused( result );
}

/*! *********************************************************************************
* \brief        Requests to stop advertising.
*
********************************************************************************** */
static void BleApp_StopAdvertise(void)
{
    bleResult_t result = gBleSuccess_c;
    appAdvUserRequest = LP_ADV_USER_REQ_STOP;
    /* since recent changes in host stack, ADV callback won't be called if the
     * advertising has been already stopped and Gap_StopAdvertising returns gBleInvalidState_c
     * so if appAdvOn is TRUE but advertising has already been stopped, it means the app doesn't
     * follow correctly the ADV state, so we assert to catch the bug */
    if( appAdvOn == TRUE )
    {
        result = Gap_StopAdvertising();
    }
    assert(result != gBleInvalidState_c);
    _unused( result );
}

/*! *********************************************************************************
* \brief        Init peer device ids.
*
********************************************************************************** */
static void BleApp_InitPeerInformation(void)
{
    for(int i = 0; i < gAppMaxConnections_c; i++)
    {
        appPeerInformation[i].deviceId = gInvalidDeviceId_c;
        appPeerInformation[i].isPaired = false;
        appPeerInformation[i].timeoutTimer = TMR_AllocateTimer();
    }
}

/*! *********************************************************************************
* \brief        Resets all peer device information
*
********************************************************************************** */
static void BleApp_ResetPeerDeviceInformation(deviceId_t peerDeviceId)
{
    /* Reset all peer device information to default value
       and stop the connection timeout timer
       Should be called at disconnection */
    appPeerInformation[peerDeviceId].tempNotifSent = FALSE;
    appPeerInformation[peerDeviceId].battNotifSent = FALSE;
    appPeerInformation[peerDeviceId].isPaired = FALSE;
    (void)TMR_StopTimer(appPeerInformation[peerDeviceId].timeoutTimer);
    appPeerInformation[peerDeviceId].deviceId = gInvalidDeviceId_c;
}

/*! *********************************************************************************
* \brief        Registers a new connected device and check different parameters
*               associated.
*
********************************************************************************** */
static void BleApp_HandleConnection(deviceId_t peerDeviceId)
{
    appConnectionNumber++;
    /* register device */
    appPeerInformation[peerDeviceId].deviceId = peerDeviceId;
    BleApp_SubscribeClientToService(peerDeviceId);

#if defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U)
    /* if the device has already been bonded, consider it is paired */
    (void)Gap_CheckIfBonded(appPeerInformation[peerDeviceId].deviceId, &appPeerInformation[peerDeviceId].isPaired, NULL);
#endif /* gAppUsePairing_d */

    /* start timeout timer */
    (void)TMR_StartLowPowerTimer(appPeerInformation[peerDeviceId].timeoutTimer,
           gTmrLowPowerSecondTimer_c,
           TmrSeconds(gAppConnectionTimeoutInSecs_c),
           DisconnectTimerCallback, &appPeerInformation[peerDeviceId].deviceId);

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)     \
      && defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
    /* Disable lowpower in RAM off during ADV with Pswitch untill 32Khz is running */
    APP_DBG_LOG("start timer : %d", appRamoffWait32kInConnectedMode);
    (void)TMR_StartTimer(appRamoffWait32kInConnectedMode,
           gTmrSingleShotTimer_c,
           TmrMilliseconds(500),
           NULL, NULL);
#endif

#if (gAppMaxConnections_c > 1)
    BLEAPP_WRITE("Device ");
    BLEAPP_WRITEDEC((uint32_t)appPeerInformation[peerDeviceId].deviceId);
    BLEAPP_WRITE(" - Connected\r\n");
#else
    BLEAPP_WRITE("Connected\r\n");
#endif
}

/*! *********************************************************************************
* \brief        Reverts what has been done in BleApp_HandleConnection()
*
********************************************************************************** */
static void BleApp_HandleDisconnection(deviceId_t peerDeviceId)
{
    appConnectionNumber--;

    /* unregister device */
    BleApp_UnsubscribeClientFromService(peerDeviceId);
    /* reset peer device information */
    BleApp_ResetPeerDeviceInformation(peerDeviceId);

#if (gAppMaxConnections_c > 1)
    BLEAPP_WRITE("Device ");
    BLEAPP_WRITEDEC((uint32_t)peerDeviceId);
    BLEAPP_WRITE(" - Disconnected\r\n");
#else
    BLEAPP_WRITE("Disconnected\r\n");
#endif
}

/*! *********************************************************************************
* \brief        Registers the device as paired.
*
********************************************************************************** */
#if defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U)
static void BleApp_HandlePairingComplete(deviceId_t peerDeviceId)
{
    appPeerInformation[peerDeviceId].isPaired = TRUE;

#if (gAppMaxConnections_c > 1)
    BLEAPP_WRITE("Device ");
    BLEAPP_WRITEDEC((uint32_t)appPeerInformation[peerDeviceId].deviceId);
    BLEAPP_WRITE(" - Paired\r\n");
#else
    BLEAPP_WRITE("Paired\r\n");
#endif
}
#endif

/*! *********************************************************************************
* \brief        Is used to be able to disable lowpower feature at run time with switch
*               buttons.
*               If Lowpower is enabled and SW2 is pressed, Lowpower will be disabled.
*               If Lowpower is disabled, it will be enabled.
*               This function should be used in BleApp_HandleKeys.
*
********************************************************************************** */
static bool BleApp_HandleLowpowerState(void)
{
    uint32_t sw2_level;
    bool     changed = FALSE;

    sw2_level = GpioReadPinInput(&switchPins[0]);

    if( PWR_CheckIfDeviceCanGoToSleep() && (sw2_level == 0U) )
    {
        changed = TRUE;
        PWR_DisallowDeviceToSleep();
        App_PostCallbackMessage(BleApp_LowpowerStateCallback, NULL);
    }

    return changed;
}

/*! *********************************************************************************
* \brief        Handles app state machine.
*
* \param[in]    peerDeviceId        peer device's ID.
* \param[in]    event               app event.
********************************************************************************** */
static void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event)
{
    APP_DBG_LOG("State: %d Evt: %d", appState, event);
    switch (appState)
    {
    case appIdle_c:
        {
            switch(event)
            {
            case appEvt_AdvertiseStarted_c:
                {
                    /* Set the App on Advertising state */
                    appState = appAdvertising_c;
                    PWR_SetNewAppState(PWR_APP_STATE_ADV);
                    /* We request an Adv start (processed in Advertising callback) */
                    appAdvUserRequest = LP_ADV_USER_REQ_START;
                    /* Set Adv parameters which will trigger Advertising start */
                    BleApp_Advertise();
                }
                break;
            case appEvt_AdvertiseStopped_c:
                /* in case a second ADV stopped event happened before the state machine has run
                    from host stack callback  -> do nothing               */
                break;
            default:
                {
                    /* should not get another event in this state */
                    assert(0);
                }
            }
        }
        break;

    case appAdvertising_c:
        {
            switch(event)
            {
            case appEvt_PeerConnected_c:
                {
                    /* Set the App on Connected state */
                    PWR_SetNewAppState(PWR_APP_STATE_CONN);
                    appState = appConnected_c;
                    /* This function will register the device to a service,
                     * handle the connection number, etc (see function for more details) */
                    BleApp_HandleConnection(peerDeviceId);
                }
                break;
            case appEvt_AdvertiseStopped_c:
                {
                    /* If we stop advertising we can go back to Idle state */
                    appState = appIdle_c;
                    /* This function will request to stop the Advertising.
                     * Depending on gAppStateNoActivityRamRetention_d, the device will
                     * go to VLLS0/1 (RAMOFF) or VLLS2/3 (RAMRET) */
                    BleApp_StopAdvertise();
                }
                break;
            case appEvt_AdvertiseStarted_c:
                /* in case a second ADV Start event happened before the state machine has run
                    from host stack callback  -> do nothing               */
                break;
            default:
                {
                    /* should not get another event in this state */
                    assert(0);
                }
            }
        }
        break;

    case appConnected_c:
        {
            switch(event)
            {
            case appEvt_PeerConnected_c:
                {
                    BleApp_HandleConnection(peerDeviceId);
                }
                break;
            case appEvt_PeerDisconnected_c:
                {
                    /* this function basically reverts what has been done in
                     * BleApp_HandleConnection */
                    BleApp_HandleDisconnection(peerDeviceId);

                    if(appConnectionNumber == 0U)
                    {
                        /* If there's not connection anymore, we set the app state
                         * back to Idle and request an Advertising stop.
                         * Depending on gAppStateNoActivityRamRetention_d, the device will
                         * go to VLLS0/1 (RAMOFF) or VLLS2/3 (RAMRET) */
                        appState = appIdle_c;
#if !defined(gAppRestartAdvAfterConnect) || (gAppRestartAdvAfterConnect == 0)
                        BleApp_StopAdvertise();
#endif

#if defined(LP_APP_TEST_REMOVE_BONDING)
                        /* test only : remove Bonding for stress test */
                        Gap_RemoveAllBonds();
#endif
                    }
                }
                break;
            case appEvt_AdvertiseStarted_c:
                {
                    appAdvUserRequest = LP_ADV_USER_REQ_START;
                    BleApp_Advertise();
                }
                break;
            case appEvt_AdvertiseStopped_c:
                {
                    BleApp_StopAdvertise();
                }
                break;
#if defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U)
            case appEvt_PairingComplete_c:
                {
                    /* Basically registers the device as paired */
                    BleApp_HandlePairingComplete(peerDeviceId);
                }
                break;
#endif
            default:
                {
                    /* should not get another event in this state */
                    assert(0);
                }
            }
        }
        break;
    default:
        {
            /* should not be another state */
            assert(0);
        }
        break;
    }
    DBG_LOG_DUMP(FALSE);
}

/*! *********************************************************************************
* \brief        Sends battery level over-the-air.
*
********************************************************************************** */
static bleResult_t BleApp_SendBattery(deviceId_t peerDeviceId)
{
    bleResult_t result = gBleInvalidState_c;
    bool_t notif_enable = BleApp_BatteryNotificationEnabled(peerDeviceId);

    APP_DBG_LOG("");

    if( notif_enable )
    {
        basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();

        result = Bas_RecordBatteryMeasurement(&basServiceConfig);

        if( result == gBleSuccess_c )
        {
#if (gAppMaxConnections_c > 1)
            BLEAPP_WRITE("Device ");
            BLEAPP_WRITEDEC((uint32_t)appPeerInformation[peerDeviceId].deviceId);
            BLEAPP_WRITE(" - Battery level sent: ");
#else
            BLEAPP_WRITE("Battery level sent: ");
#endif
            BLEAPP_WRITEDEC((uint32_t)basServiceConfig.batteryLevel);
            BLEAPP_WRITE("%\r\n");
        }
    }
    else
    {
        APP_DBG_LOG("%d - Battery Notifications not enabled", peerDeviceId);
    }
    return result;
}

/*! *********************************************************************************
* \brief        Sends temperature value over-the-air.
*
********************************************************************************** */
static bleResult_t BleApp_SendTemperature(deviceId_t peerDeviceId)
{
    int16_t temperature_val;
    bleResult_t result = gBleInvalidState_c;
    bool_t notif_enable = BleApp_TemperatureNotificationEnabled();

    APP_DBG_LOG("");

    /* make sur notifications are enabled on client side */
    if( notif_enable )
    {
        temperature_val = BOARD_GetTemperature();

        /* Update with initial temperature */
        result = Tms_RecordTemperatureMeasurement((uint16_t)service_temperature,
                                               temperature_val * 100);
        if( result == gBleSuccess_c )
        {
#if (gAppMaxConnections_c > 1)
            BLEAPP_WRITE("Device ");
            BLEAPP_WRITEDEC((uint32_t)appPeerInformation[peerDeviceId].deviceId);
            BLEAPP_WRITE(" - Temperature sent: ");
#else
            BLEAPP_WRITE("Temperature sent: ");
#endif
            BLEAPP_WRITEDEC((uint32_t)temperature_val);
            BLEAPP_WRITE(" C\r\n");
        }
    }
    else
    {
        APP_DBG_LOG("%d - Temperature Notifications not enabled", peerDeviceId);
    }

    return result;
}

/*! *********************************************************************************
* \brief        Check temperature notifications are enabled on client side.
*
********************************************************************************** */
static bool_t BleApp_TemperatureNotificationEnabled()
{
    bool_t enable = false;
    uint16_t  handle;
    uint16_t  hCccd;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_Temperature_d);

    result = GattDb_FindCharValueHandleInService((uint16_t)service_temperature,
             gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Get handle of CCCD */
        result = GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd);
        if (result == gBleSuccess_c)
        {
            /* Check if notifications are active */
            (void)Gap_CheckNotificationStatus(appPeerInformation[LP_TEMP_SERVICE_IDX].deviceId, hCccd, &enable);
        }
    }
    return enable;
}

/*! *********************************************************************************
* \brief        Check battery notifications are enabled on client side.
*
********************************************************************************** */
static bool_t BleApp_BatteryNotificationEnabled(deviceId_t peerDeviceId)
{
    bool_t enable = false;
    uint16_t  handle;
    uint16_t  hCccd;
    bleResult_t result;
    bleUuid_t uuid = Uuid16(gBleSig_BatteryLevel_d);

    result = GattDb_FindCharValueHandleInService((uint16_t)service_battery,
             gBleUuidType16_c, &uuid, &handle);

    if (result == gBleSuccess_c)
    {
        /* Get handle of CCCD */
        result = GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd);
        if (result == gBleSuccess_c)
        {
            /* Check if notifications are active */
            (void)Gap_CheckNotificationStatus(appPeerInformation[peerDeviceId].deviceId, hCccd, &enable);
        }
    }
    return enable;
}

/*! *********************************************************************************
* \brief        Subscribe a client to a service.
*
* \param[in]    peerDeviceId        Peer device ID.
********************************************************************************** */
static void BleApp_SubscribeClientToService(deviceId_t peerDeviceId)
{
    /* All peer devices are subscribed to Battery Service but
       Only one can be subscribed to Temperature Service */
    if( appPeerInformation[peerDeviceId].deviceId == LP_TEMP_SERVICE_IDX )
    {
        (void)Tms_Subscribe(appPeerInformation[peerDeviceId].deviceId);
    }

    (void)Bas_Subscribe(&basServiceConfig, appPeerInformation[peerDeviceId].deviceId);
}

/*! *********************************************************************************
* \brief        Unsubscribe a client to a service.
*
* \param[in]    peerDeviceId        Peer device ID.
********************************************************************************** */
static void BleApp_UnsubscribeClientFromService(deviceId_t peerDeviceId)
{
    if( appPeerInformation[peerDeviceId].deviceId == LP_TEMP_SERVICE_IDX )
    {
        (void)Tms_Unsubscribe();
    }

    (void)Bas_Unsubscribe(&basServiceConfig, appPeerInformation[peerDeviceId].deviceId);
}

/*! *********************************************************************************
* \brief        Prints phy event.
*
********************************************************************************** */
static void BleApp_PrintLePhyEvent(gapPhyEvent_t* pPhyEvent)
{
    /* String dictionary corresponding to gapLePhyMode_t */
    static const char* mLePhyModeStrings[] =
    {
        "Invalid\n\r",
        "1M\n\r",
        "2M\n\r",
        "Coded\n\r",
    };
    uint8_t txPhy = (pPhyEvent->txPhy <= gLePhyCoded_c) ? pPhyEvent->txPhy : 0U;
    uint8_t rxPhy = (pPhyEvent->rxPhy <= gLePhyCoded_c) ? pPhyEvent->rxPhy : 0U;
#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    BLEAPP_WRITE("Phy Update Complete.\n\r");
    BLEAPP_WRITE("TxPhy ");
    BLEAPP_WRITE(mLePhyModeStrings[txPhy]);
    BLEAPP_WRITE("RxPhy ");
    BLEAPP_WRITE(mLePhyModeStrings[rxPhy]);
#else
    NOT_USED(txPhy);
    NOT_USED(rxPhy);
    NOT_USED(mLePhyModeStrings);
#endif
}

/* ***********************************************************************************
*************************************************************************************
* App Context Callbacks
*************************************************************************************
*********************************************************************************** */

/*! *********************************************************************************
* \brief        Makes sure BleApp_StateMachineHandler is executed in App task context.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void BleApp_StateMachineCallback(appCallbackParam_t pParam)
{
    uint32_t event = (uint32_t)pParam;
    APP_DBG_LOG("Evt: %d", event);
    BleApp_StateMachineHandler(gInvalidDeviceId_c, (appEvent_t)event);
}

/*! *********************************************************************************
* \brief        Print Lowpower feature state in App task context.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void BleApp_LowpowerStateCallback(appCallbackParam_t pParam)
{
    APP_DBG_LOG("");
    if( PWR_CheckIfDeviceCanGoToSleep() )
    {
        BLEAPP_WRITE("\r\nLowpower enabled\r\n");
    }
    else
    {
        BLEAPP_WRITE("\r\nLowpower disabled\r\n");
    }
}

/*! *********************************************************************************
* \brief        Print Advertising state in App task context.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
static void BleApp_PrintAdvMessageCallback(appCallbackParam_t pParam)
{
    if(appAdvOn == TRUE)
    {
        BLEAPP_WRITE("\r\nAdvertising...\r\n");
        APP_DBG_LOG("ADV started");
    }
    else
    {
        BLEAPP_WRITE("\r\nAdvertising stopped\r\n");
        APP_DBG_LOG("ADV stopped");
    }
    NOT_USED(pParam);
}
#endif

/* ***********************************************************************************
*************************************************************************************
* Timer Callbacks
*************************************************************************************
*********************************************************************************** */

/*! *********************************************************************************
* \brief        Handles advertising timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void AdvertisingTimerCallback(void* pParam)
{
    /* Post to main App task to execute a callback, it makes sure the callback will be executed
     * in App context and prevents race conditions */
    App_PostCallbackMessage(BleApp_StateMachineCallback, (void*)appEvt_AdvertiseStopped_c);
}

/*! *********************************************************************************
* \brief        Handles disconnect timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
static void DisconnectTimerCallback(void* pParam)
{
    deviceId_t* peerDeviceId = (deviceId_t*)pParam;
    if( appPeerInformation[*peerDeviceId].deviceId != gInvalidDeviceId_c )
    {
        /* Terminate connection */
        (void)Gap_Disconnect(appPeerInformation[*peerDeviceId].deviceId);
         APP_DBG_LOG("Connection timeout device id %d", appPeerInformation[*peerDeviceId].deviceId);
    }
}

/*! *********************************************************************************
* \brief        Handles wake up after no activity timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
#if defined(gAppWakeUpTimerAfterNoActivityMs) && (gAppWakeUpTimerAfterNoActivityMs != 0U)
static void WakeUpTimerCallback(void* pParam)
{
    APP_DBG_LOG("");
#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1)
    /* If Privacy is used, make sure to reenable it */
    (void)BleConnManager_EnablePrivacy();
#endif
    /* send event to State Machine callback to start advertising  */
    App_PostCallbackMessage(BleApp_StateMachineCallback, (void*)LP_SLAVE_ADV_START_EVENT);
}
#endif

/*! *********************************************************************************
* \brief        Serial routines
*
********************************************************************************** */
#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
static void BleApp_SerialInit(void)
{
    serialStatus_t ret;

    gLpSerMgrIf = BOARD_GetSerialManagerInterface();

    /* Set RX callback
       In case the device could get RX data from the serial prior to this point, Setting the RX callback
          can be called from BOARD_InitHardware() function in hardware_init.c file . However, the application
          shall make sure no BLE API (gap, etc..) is called from the callback before the host stack
          initialization (BleApp_Init() is called)  */
    ret = Serial_SetRxCallBack(gLpSerMgrIf, BleApp_SerialRxCb, NULL);
    assert( ret == gSerial_Success_c );
    NOT_USED(ret);

    /* Allocate application timer - this will be used to keep the device in STOP
       mode during a certain period of time to receive additional bytes*/
#if defined(gAppSerialManagerKeepActiveMs_c) && (gAppSerialManagerKeepActiveMs_c > 0)
    appSerialTimerId = TMR_AllocateTimer();
#endif
}

static void BleApp_SerialWrite(const char* pString)
{
    serialStatus_t ret;

    ret = Serial_Print( gLpSerMgrIf, pString, gAllowToBlock_d /* gAllowToBlock_d or gNoBlock_d*/ );
    assert( ret == gSerial_Success_c );
    NOT_USED(ret);
}

static void BleApp_SerialWriteDec(uint32_t nb)
{
    serialStatus_t ret;

    ret = Serial_PrintDec(gLpSerMgrIf, nb);
    assert( ret == gSerial_Success_c );
    NOT_USED(ret);
}

#if defined(gAppSerialManagerKeepActiveMs_c) && (gAppSerialManagerKeepActiveMs_c > 0)
static void BleApp_SerialTimerCb(void* pParam)
{
    /* Serial Timer no longer running, the lowpower module will allow to go to lowpower
       No thing to do             */
}
#endif

static void BleApp_SerialRxCb( void *params )
{
    BleApp_SerialWrite(">");

    while (true)
    {
        char     ichar[2];
        uint16_t wlen;

        ichar[0] = 0x0;
        ichar[1] = 0x0;

        (void)Serial_Read( gLpSerMgrIf, (uint8_t*)ichar, 1, &wlen );

        if( (wlen == 0u) )
        {
            break;
        }
        if( ichar[0] == 0x0 )
        {
            /* report 'null char' : Likely a wakeup from lowpower, the first byte is lost */
            BleApp_SerialWrite("null char");
            break;
        }

        /* Echo to the host */
        BleApp_SerialWrite((const char*)ichar);
    }

    BleApp_SerialWrite("\r\n");

    /* when gAppSerialManagerKeepActiveMs_c is set, prevent the device
       from going to lowpower immediatly so more bytes can to be received     */
#if defined(gAppSerialManagerKeepActiveMs_c) && (gAppSerialManagerKeepActiveMs_c > 0)
    tmrErrCode_t ret;

    /* Stop and restart Serial Timer every time data is received */
    ret = TMR_StopTimer(appSerialTimerId);
    assert( ret == gTmrSuccess_c );

    ret = TMR_StartTimer(appSerialTimerId,
               gTmrSingleShotTimer_c,
               TmrMilliseconds(gAppSerialManagerKeepActiveMs_c),
               BleApp_SerialTimerCb, NULL);
     assert( ret == gTmrSuccess_c );

     NOT_USED(ret);    /* Avoid warning when assert is not enabled */
#endif
}
#endif

/*! *********************************************************************************
* @}
********************************************************************************** */
