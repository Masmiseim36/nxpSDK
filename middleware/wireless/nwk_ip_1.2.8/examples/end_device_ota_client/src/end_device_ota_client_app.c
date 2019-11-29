/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       end_device_ota_client_app.c
\brief      This is a public source file for the end device ota client demo application.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include <string.h>

/* FSL Framework */
#include "shell.h"
#include "Keyboard.h"
#include "RNG_Interface.h"
#if gLpmIncluded_d
#include "PWR_Interface.h"
#endif

/* Network */
#include "ip_if_management.h"
#include "event_manager.h"

/* Application */
#include "end_device_ota_client_app.h"
#include "shell_ip.h"
#include "thread_utils.h"
#include "thread_network.h"
#include "thread_app_callbacks.h"
#include "app_init.h"
#include "app_stack_config.h"
#include "app_thread_config.h"
#include "app_led.h"
#include "app_temp_sensor.h"
#include "coap.h"
#include "app_socket_utils.h"
#if THR_ENABLE_EVENT_MONITORING
#include "app_event_monitoring.h"
#endif
#if THR_ENABLE_MGMT_DIAGNOSTICS
#include "thread_mgmt.h"
#include "thci.h"
#endif
#if UDP_ECHO_PROTOCOL
#include "app_echo_udp.h"
#endif
#if gEnableOTAClient_d
#include "app_ota.h"
#endif

/*==================================================================================================
Private macros
==================================================================================================*/
#ifndef APP_MSG_QUEUE_SIZE
    #define APP_MSG_QUEUE_SIZE                  20
#endif

#if (THREAD_USE_SHELL == FALSE)
    #define shell_write(a)
    #define shell_refresh()
    #define shell_printf(a,...)
#endif

#define gThrDefaultInstanceId_c                 0

#define APP_LED_URI_PATH                        "/led"
#define APP_TEMP_URI_PATH                       "/temp"
#define APP_SINK_URI_PATH                       "/sink"

#define APP_DEFAULT_DEST_ADDR                   in6addr_realmlocal_allthreadnodes

#define APP_SW_WAKE_UP_TIMEOUT                  5000 /* miliseconds */
/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static instanceId_t mThrInstanceId = gInvalidInstanceId_c;    /*!< Thread Instance ID */

static bool_t mJoiningIsAppInitiated = FALSE;

/*==================================================================================================
Private prototypes
==================================================================================================*/
static void App_HandleKeyboard(uint8_t *param);
static void App_UpdateStateLeds(appDeviceState_t deviceState);
static void APP_JoinEventsHandler(thrEvCode_t evCode);

static void APP_SwWakeUpCb(void *pParam);
static void APP_InitCoapDemo(void);
static void APP_ReportTemp(uint8_t *pParam);
#ifndef FRDM_KW41Z
static void APP_SendDataSinkCreate(uint8_t *pParam);
static void APP_SendDataSinkRelease(uint8_t *pParam);
#endif
#if gKBD_KeysCount_c > 1
static void APP_SendLedRgbOn(uint8_t *pParam);
static void APP_SendLedRgbOff(uint8_t *pParam);
static void APP_SendLedFlash(uint8_t *pParam);
static void APP_SendLedColorWheel(uint8_t *pParam);
#endif
static void APP_LocalDataSinkRelease(uint8_t *pParam);
static void APP_ProcessLedCmd(uint8_t *pCommand, uint8_t dataLen);
static void APP_CoapGenericCallback(coapSessionStatus_t sessionStatus, uint8_t *pData, coapSession_t *pSession, uint32_t dataLen);
static void APP_CoapLedCb(coapSessionStatus_t sessionStatus, uint8_t *pData, coapSession_t *pSession, uint32_t dataLen);
static void APP_CoapTempCb(coapSessionStatus_t sessionStatus, uint8_t *pData, coapSession_t *pSession, uint32_t dataLen);
static void APP_CoapSinkCb(coapSessionStatus_t sessionStatus, uint8_t *pData, coapSession_t *pSession, uint32_t dataLen);
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
const coapUriPath_t gAPP_LED_URI_PATH  = {SizeOfString(APP_LED_URI_PATH), (uint8_t *)APP_LED_URI_PATH};
const coapUriPath_t gAPP_TEMP_URI_PATH = {SizeOfString(APP_TEMP_URI_PATH), (uint8_t *)APP_TEMP_URI_PATH};
const coapUriPath_t gAPP_SINK_URI_PATH = {SizeOfString(APP_SINK_URI_PATH), (uint8_t *)APP_SINK_URI_PATH};

/* Application state/mode */
appDeviceState_t gAppDeviceState[THR_MAX_INSTANCES];
appDeviceMode_t gAppDeviceMode[THR_MAX_INSTANCES];

/* Pointer application task message queue */
taskMsgQueue_t *mpAppThreadMsgQueue;

/* Flag used to stop the attaching retries */
bool_t gbRetryInterrupt = TRUE;

/* CoAP instance */
uint8_t mAppCoapInstId = THR_ALL_FFs8;
/* Destination address for CoAP commands */
ipAddr_t gCoapDestAddress;
/* Timer used to keep the device in Up state for an amount of time */
tmrTimerID_t gAppSwWakeUpTimer = gTmrInvalidTimerID_c;

extern bool_t gEnable802154TxLed;

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     void APP_Init(void)
\brief  This function is used to initialize application.

***************************************************************************************************/
void APP_Init
(
    void
)
{
    /* Initialize pointer to application task message queue */
    mpAppThreadMsgQueue = &appThreadMsgQueue;

    /* Initialize main thread message queue */
    ListInit(&appThreadMsgQueue.msgQueue,APP_MSG_QUEUE_SIZE);

    /* Set default device mode/state */
    APP_SetState(gThrDefaultInstanceId_c, gDeviceState_FactoryDefault_c);
    APP_SetMode(gThrDefaultInstanceId_c, gDeviceMode_Configuration_c);

    /* Initialize keyboard handler */
    pfAppKeyboardHandler = App_HandleKeyboard;

    /* Use one instance ID for application */
    mThrInstanceId = gThrDefaultInstanceId_c;

#if THR_ENABLE_EVENT_MONITORING
    /* Initialize event monitoring */
    APP_InitEventMonitor(mThrInstanceId);
#endif

    if(gThrStatus_Success_c == THR_StartInstance(mThrInstanceId, pStackCfg[0]))
    {
        /* Initialize CoAP demo */
        APP_InitCoapDemo();

#if USE_TEMPERATURE_SENSOR
        /* Initialize Temperature sensor/ADC module*/
        APP_InitADC(ADC_0);
#endif

#if THREAD_USE_THCI && THR_ENABLE_MGMT_DIAGNOSTICS
        (void)MgmtDiagnostic_RegisterAppCb(THCI_MgmtDiagnosticAppCb);
#endif

#if THREAD_USE_SHELL && SOCK_DEMO
        /* Initialize use sockets - used from shell */
        APP_InitUserSockets(mpAppThreadMsgQueue);
#endif

#if gEnableOTAClient_d
        OtaClientInit(mThrInstanceId, mpAppThreadMsgQueue);
#endif

#if APP_AUTOSTART
       if(!THR_GetAttr_IsDevConnected(mThrInstanceId))
       {
          mJoiningIsAppInitiated = TRUE;

          if(THR_NwkJoin(mThrInstanceId, THR_APP_JOIN_DISCOVERY_METHOD) != gThrStatus_Success_c)
          {
              /* User can treat join failure according to their application */
          }
       }
#endif
    }
}

/*!*************************************************************************************************
\fn     void App_Handler(void)
\brief  Application Handler. In this configuration is called on the task with the lowest priority
***************************************************************************************************/
void APP_Handler
(
    void
)
{
    bool_t handleMsg = TRUE;

    while(handleMsg == TRUE)
    {
        handleMsg = NWKU_MsgHandler(&appThreadMsgQueue);
        /* For BareMetal break the while(1) after 1 run */
        if(!gUseRtos_c && MSG_Pending(&appThreadMsgQueue.msgQueue))
        {
            (void)OSA_EventSet(appThreadMsgQueue.taskEventId, NWKU_GENERIC_MSG_EVENT);
            break;
        }
    }
}

/*!*************************************************************************************************
\fn     void APP_NwkScanHandler(void *param)
\brief  This function is used to handle network scan results in asynchronous mode.

\param  [in]    param    Pointer to stack event
***************************************************************************************************/
void APP_NwkScanHandler
(
    void *param
)
{
    thrEvmParams_t *pEventParams = (thrEvmParams_t *)param;
    thrNwkScanResults_t *pScanResults = &pEventParams->pEventData->nwkScanCnf;

    /* Handle the network scan result here */
    if(pScanResults)
    {
#if THREAD_USE_SHELL
        SHELL_NwkScanPrint(pScanResults);
#endif
        MEM_BufferFree(pScanResults);
    }
    /* Free Event Buffer */
    MEM_BufferFree(pEventParams);
}

/*!*************************************************************************************************
\fn     void Stack_to_APP_Handler(void *param)
\brief  This function is used to handle stack events in asynchronous mode.

\param  [in]    param    Pointer to stack event
***************************************************************************************************/
void Stack_to_APP_Handler
(
    void *param
)
{
    thrEvmParams_t *pEventParams = (thrEvmParams_t *)param;

    /* Uncomment for multicast address */
    //ipAddr_t mCastGroup = MCAST_3EAD_ADDRESS;

    switch(pEventParams->code)
    {
        case gThrEv_GeneralInd_ResetToFactoryDefault_c:
            App_UpdateStateLeds(gDeviceState_FactoryDefault_c);
            break;

        case gThrEv_GeneralInd_InstanceRestoreStarted_c:
        case gThrEv_GeneralInd_ConnectingStarted_c:
            APP_SetMode(mThrInstanceId, gDeviceMode_Configuration_c);
            App_UpdateStateLeds(gDeviceState_JoiningOrAttaching_c);
            gEnable802154TxLed = FALSE;
            break;

        case gThrEv_NwkJoinCnf_Success_c:
        case gThrEv_NwkJoinCnf_Failed_c:
            APP_JoinEventsHandler(pEventParams->code);
            break;

        case gThrEv_GeneralInd_Connected_c:
            App_UpdateStateLeds(gDeviceState_NwkConnected_c);
            /* Set application CoAP destination to all nodes on connected network */
            THR_GetIP6Addr(mThrInstanceId, gAllThreadNodes_c, &gCoapDestAddress, NULL);
            APP_SetMode(mThrInstanceId, gDeviceMode_Application_c);
            /* Enable LED for 80215.4 tx activity */
            gEnable802154TxLed = TRUE;
#if UDP_ECHO_PROTOCOL
            ECHO_ProtocolInit(mpAppThreadMsgQueue);
#endif
#if gEnableOTAClient_d
            //(void)NWKU_SendMsg(OtaClient_StartServerDiscovery, NULL, mpAppThreadMsgQueue);
#endif
            /* Uncomment to register multicast address */
            //IP_IF_AddMulticastGroup6(gIpIfSlp0_c, &mCastGroup);
            break;

        case gThrEv_GeneralInd_ConnectingFailed_c:
        case gThrEv_GeneralInd_Disconnected_c:
            APP_SetMode(mThrInstanceId, gDeviceMode_Configuration_c);
            App_UpdateStateLeds(gDeviceState_NwkFailure_c);
            break;

#if gLpmIncluded_d
        case gThrEv_GeneralInd_AllowDeviceToSleep_c:
            PWR_AllowDeviceToSleep();
            break;

        case gThrEv_GeneralInd_DisallowDeviceToSleep_c:
            PWR_DisallowDeviceToSleep();
            break;
#endif
        default:
            break;
    }

    /* Free event buffer */
    MEM_BufferFree(pEventParams->pEventData);
    MEM_BufferFree(pEventParams);
}

/*!*************************************************************************************************
\fn     void APP_Commissioning_Handler(void *param)
\brief  This function is used to handle Commissioning events in synchronous mode.

\param  [in]    param    Pointer to Commissioning event
***************************************************************************************************/
void APP_Commissioning_Handler
(
    void *param
)
{
    thrEvmParams_t *pEventParams = (thrEvmParams_t *)param;

    switch(pEventParams->code)
    {
        /* Joiner Events */
        case gThrEv_MeshCop_JoinerDiscoveryStarted_c:
            break;
        case gThrEv_MeshCop_JoinerDiscoveryFailed_c:
            break;
        case gThrEv_MeshCop_JoinerDiscoveryFailedFiltered_c:
            break;
        case gThrEv_MeshCop_JoinerDiscoverySuccess_c:
            break;
        case gThrEv_MeshCop_JoinerDtlsSessionStarted_c:
            App_UpdateStateLeds(gDeviceState_JoiningOrAttaching_c);
            break;
        case gThrEv_MeshCop_JoinerDtlsError_c:
        case gThrEv_MeshCop_JoinerError_c:
            App_UpdateStateLeds(gDeviceState_FactoryDefault_c);
            break;
        case gThrEv_MeshCop_JoinerAccepted_c:
            break;
    }

    /* Free event buffer */
    MEM_BufferFree(pEventParams);
}

/*!*************************************************************************************************
\fn     void App_SedWakeUpFromKeyBoard(void)
\brief  This is a callback fuction called when the device is waked from keyboard.
***************************************************************************************************/
void App_SedWakeUpFromKeyBoard
(
    void
)
{
    /* Keep the device Up for 5 seconds - using a timer */
    if(gAppSwWakeUpTimer == gTmrInvalidTimerID_c)
    {
        gAppSwWakeUpTimer = TMR_AllocateTimer();
    }

    if(gAppSwWakeUpTimer != gTmrInvalidTimerID_c)
    {
        TMR_StartTimer(gAppSwWakeUpTimer, gTmrSingleShotTimer_c, APP_SW_WAKE_UP_TIMEOUT, APP_SwWakeUpCb, NULL);
    }
}
/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\private
\fn     static void APP_InitCoapDemo(void)
\brief  Initialize CoAP demo.
***************************************************************************************************/
static void APP_InitCoapDemo
(
    void
)
{
    coapRegCbParams_t cbParams[] =  {{APP_CoapLedCb,  (coapUriPath_t *)&gAPP_LED_URI_PATH},
                                     {APP_CoapTempCb, (coapUriPath_t *)&gAPP_TEMP_URI_PATH},
                                     {APP_CoapSinkCb, (coapUriPath_t *)&gAPP_SINK_URI_PATH}};
    /* Register Services in COAP */
    sockaddrStorage_t coapParams = {0};
    
    NWKU_SetSockAddrInfo(&coapParams, NULL, AF_INET6, COAP_DEFAULT_PORT, gNoIPv6FlowInfo_c, gIpIfSlp0_c);

    mAppCoapInstId = COAP_CreateInstance(NULL, &coapParams, (coapRegCbParams_t *)cbParams,
                                         NumberOfElements(cbParams));
}

/*!*************************************************************************************************
\private
\fn     static void APP_ConfigModeHandleKeyboard(uint32_t keyEvent)
\brief  This is a handler for KBD module events. Device is in configuration mode.

\param  [in]    keyEvent    The keyboard module event
***************************************************************************************************/
static void APP_ConfigModeHandleKeyboard
(
    uint32_t keyEvent
)
{
    switch(keyEvent)
    {
        case gKBD_EventPB1_c:
#if gKBD_KeysCount_c > 1
        case gKBD_EventPB2_c:
        case gKBD_EventPB3_c:
        case gKBD_EventPB4_c:
#endif
            if((APP_GetState(mThrInstanceId) == gDeviceState_FactoryDefault_c) ||
               (APP_GetState(mThrInstanceId) == gDeviceState_NwkFailure_c))
            {
                App_UpdateStateLeds(gDeviceState_JoiningOrAttaching_c);

                mJoiningIsAppInitiated = TRUE;

                /* join the network */
                if(THR_NwkJoin(mThrInstanceId, THR_APP_JOIN_DISCOVERY_METHOD) != gThrStatus_Success_c)
                {
                    /* User can treat join failure according to their application */
                }
            }
            break;
        case gKBD_EventLongPB1_c:
#if gKBD_KeysCount_c > 1
        case gKBD_EventLongPB2_c:
        case gKBD_EventLongPB3_c:
        case gKBD_EventLongPB4_c:
#endif
            break;
        /* Factory reset */
        case gKBD_EventVeryLongPB1_c:
#if gKBD_KeysCount_c > 1
        case gKBD_EventVeryLongPB2_c:
        case gKBD_EventVeryLongPB3_c:
        case gKBD_EventVeryLongPB4_c:
#endif
            THR_FactoryReset();
            break;
        default:
            break;
    }
}

/*!*************************************************************************************************
\private
\fn     static void APP_AppModeHandleKeyboard(uint32_t keyEvent)
\brief  This is a handler for KBD module events. Device is in application mode.

\param  [in]    keyEvent    The keyboard module event
***************************************************************************************************/
static void APP_AppModeHandleKeyboard
(
    uint32_t keyEvent
)
{
#if gLpmIncluded_d
    /* restart wake up timer */
    App_SedWakeUpFromKeyBoard();
#endif

    switch(keyEvent)
    {
        case gKBD_EventPB1_c:
#ifdef FRDM_KW41Z
            /* Remote led RGB - on */
            (void)NWKU_SendMsg(APP_SendLedRgbOn, NULL, mpAppThreadMsgQueue);
#else
            /* Data sink create */
            (void)NWKU_SendMsg(APP_SendDataSinkCreate, NULL, mpAppThreadMsgQueue);
#endif
            break;
#if gKBD_KeysCount_c > 1
        case gKBD_EventPB2_c:
#ifdef FRDM_KW41Z
            /* Remote led RGB - off */
            (void)NWKU_SendMsg(APP_SendLedRgbOff, NULL, mpAppThreadMsgQueue);
#else
            /* Report temperature */
            (void)NWKU_SendMsg(APP_ReportTemp, NULL, mpAppThreadMsgQueue);
#endif
            break;
        case gKBD_EventPB3_c:
            /* Remote led RGB - on */
            (void)NWKU_SendMsg(APP_SendLedRgbOn, NULL, mpAppThreadMsgQueue);
            break;
        case gKBD_EventPB4_c:
            /* Remote led RGB - off */
            (void)NWKU_SendMsg(APP_SendLedRgbOff, NULL, mpAppThreadMsgQueue);
            break;
#endif
        case gKBD_EventLongPB1_c:
#ifdef FRDM_KW41Z
            /* Report temperature */
            (void)NWKU_SendMsg(APP_ReportTemp, NULL, mpAppThreadMsgQueue);
#else
            /* Remote data sink release */
            (void)NWKU_SendMsg(APP_SendDataSinkRelease, NULL, mpAppThreadMsgQueue);
#endif
            break;
#if gKBD_KeysCount_c > 1
        case gKBD_EventLongPB2_c:
#ifdef FRDM_KW41Z
            /* Remote led flash */
            (void)NWKU_SendMsg(APP_SendLedFlash, NULL, mpAppThreadMsgQueue);
            break;
#else
            /* Local data sink release */
            (void)NWKU_SendMsg(APP_LocalDataSinkRelease, NULL, mpAppThreadMsgQueue);
#endif
            break;
        case gKBD_EventLongPB3_c:
            /* Remote led flash */
            (void)NWKU_SendMsg(APP_SendLedFlash, NULL, mpAppThreadMsgQueue);
            break;
        case gKBD_EventLongPB4_c:
            /* Remote led - color wheel */
            (void)NWKU_SendMsg(APP_SendLedColorWheel, NULL, mpAppThreadMsgQueue);
            break;
#endif
        case gKBD_EventVeryLongPB1_c:
#if gKBD_KeysCount_c > 1
        case gKBD_EventVeryLongPB2_c:
        case gKBD_EventVeryLongPB3_c:
        case gKBD_EventVeryLongPB4_c:
#endif
            /* Factory reset */
            THR_FactoryReset();
            break;
        default:
            break;
    }
}

/*!*************************************************************************************************
\private
\fn     static void App_HandleKeyboard(uint8_t *param)
\brief  This is a handler for KBD module events.

\param  [in]    param    The keyboard module event
***************************************************************************************************/
static void App_HandleKeyboard
(
    uint8_t *param
)
{
    uint32_t events = (uint32_t)(param);

    if(APP_GetMode(mThrInstanceId) == gDeviceMode_Configuration_c)
    {
        /* Device is in configuration mode */
        APP_ConfigModeHandleKeyboard(events);
    }
    else
    {
        /* Device is in application mode */
        APP_AppModeHandleKeyboard(events);
    }
}

/*!*************************************************************************************************
\private
\fn     static void App_UpdateLedState(appDeviceState_t deviceState)
\brief  Called when Application state and LEDs must be updated.

\param  [in]    deviceState    The current device state
***************************************************************************************************/
static void App_UpdateStateLeds
(
    appDeviceState_t deviceState
)
{
    APP_SetState(mThrInstanceId, deviceState);
    Led_SetState(APP_GetMode(mThrInstanceId), APP_GetState(mThrInstanceId));
}

/*!*************************************************************************************************
\private
\fn     static void APP_JoinEventsHandler(thrEvCode_t evCode)
\brief  This function is used to the handle join failed event.

\param  [in]    evCode    Event code
***************************************************************************************************/
static void APP_JoinEventsHandler
(
    thrEvCode_t evCode
)
{
    if(mJoiningIsAppInitiated)
    {
        if(evCode == gThrEv_NwkJoinCnf_Failed_c)
        {
            if(gbRetryInterrupt)
            {
                /* Retry to join the network */
                if(THR_NwkJoin(mThrInstanceId, THR_APP_JOIN_DISCOVERY_METHOD) != gThrStatus_Success_c)
                {
                    /* User can treat join failure according to their application */
                }
                return;
            }
            mJoiningIsAppInitiated = FALSE;
        }
        else if(evCode == gThrEv_NwkJoinCnf_Success_c)
        {
            mJoiningIsAppInitiated = FALSE;
        }
    }
}

/*!*************************************************************************************************
\fn     static void APP_SwWakeUpCb(void *pParam)
\brief  This is a callback fuction called when the device is waked from keyboard.
        After this callback the device will enter in low power

\param  [in]    pParam    Not used
***************************************************************************************************/
static void APP_SwWakeUpCb
(
    void *pParam
)
{
    TMR_StopTimer(gAppSwWakeUpTimer);
#if gLpmIncluded_d
    PWR_AllowDeviceToSleep();
#endif
}

/*==================================================================================================
  Coap Demo functions:
==================================================================================================*/
/*!*************************************************************************************************
\private
\fn     static void APP_CoapGenericCallback(coapSessionStatus_t sessionStatus, uint8_t *pData,
                                            coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the generic callback function for CoAP message.

\param  [in]    sessionStatus   Status for CoAP session
\param  [in]    pData           Pointer to CoAP message payload
\param  [in]    pSession        Pointer to CoAP session
\param  [in]    dataLen         Length of CoAP payload
***************************************************************************************************/
static void APP_CoapGenericCallback
(
    coapSessionStatus_t sessionStatus,
    uint8_t *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    /* If no ACK was received, try again */
    if(sessionStatus == gCoapFailure_c)
    {
        if(FLib_MemCmp(pSession->pUriPath->pUriPath, (coapUriPath_t *)&gAPP_TEMP_URI_PATH.pUriPath,
                       pSession->pUriPath->length))
        {
            (void)NWKU_SendMsg(APP_ReportTemp, NULL, mpAppThreadMsgQueue);
        }
    }
    /* Process data, if any, for sessionStatus != gCoapFailure_c */
    else
    {
        if (pSession->msgType == gCoapNonConfirmable_c)
        {
            //Process data
        }
        else if ((pSession->msgType == gCoapConfirmable_c) && (sessionStatus == gCoapSuccess_c))
        {
            //Process data
        }
    }
}

/*!*************************************************************************************************
\private
\fn     static void APP_ReportTemp(uint8_t *pParam)
\brief  This open a socket and report the temperature to gCoapDestAddress.

\param  [in]    pParam    Not used
***************************************************************************************************/
static void APP_ReportTemp
(
    uint8_t *pParam
)
{
    coapSession_t *pSession = NULL;
    /* Get Temperature */
    uint8_t *pTempString = App_GetTempDataString();
    uint32_t ackPloadSize;

    if(!IP_IF_IsMyAddr(gIpIfSlp0_c, &gCoapDestAddress))
    {
        pSession = COAP_OpenSession(mAppCoapInstId);

        if(NULL != pSession)
        {
            coapMsgTypesAndCodes_t coapMessageType = gCoapMsgTypeNonPost_c;

            pSession->pCallback = NULL;
            FLib_MemCpy(&pSession->remoteAddrStorage.ss_addr, &gCoapDestAddress, sizeof(ipAddr_t));
            ackPloadSize = strlen((char *)pTempString);
            pSession->pUriPath = (coapUriPath_t *)&gAPP_TEMP_URI_PATH;

            if(!IP6_IsMulticastAddr(&gCoapDestAddress))
            {
                coapMessageType = gCoapMsgTypeConPost_c;
                COAP_SetCallback(pSession, APP_CoapGenericCallback);
            }

            COAP_Send(pSession, coapMessageType, pTempString, ackPloadSize);
        }
    }
    /* Print temperature in shell */
    shell_write("\r");
    shell_write((char *)pTempString);
    shell_refresh();
    MEM_BufferFree(pTempString);
}

#ifndef FRDM_KW41Z
/*!*************************************************************************************************
\private
\fn     static nwkStatus_t APP_SendDataSinkCommand(uint8_t *pCommand, uint8_t dataLen)
\brief  This function is used to send a Data Sink command to APP_DEFAULT_DEST_ADDR.

\param  [in]    pCommand       Pointer to command data
\param  [in]    dataLen        Data length

\return         nwkStatus_t    Status of the command
***************************************************************************************************/
static nwkStatus_t APP_SendDataSinkCommand
(
    uint8_t *pCommand,
    uint8_t dataLen
)
{
    nwkStatus_t status = gNwkStatusFail_c;
    coapSession_t *pSession = COAP_OpenSession(mAppCoapInstId);

    if(pSession)
    {
        ipAddr_t coapDestAddress = APP_DEFAULT_DEST_ADDR;

        pSession->pCallback = NULL;
        FLib_MemCpy(&pSession->remoteAddrStorage.ss_addr, &coapDestAddress, sizeof(ipAddr_t));
        pSession->pUriPath = (coapUriPath_t*)&gAPP_SINK_URI_PATH;
        status = COAP_Send(pSession, gCoapMsgTypeNonPost_c, pCommand, dataLen);
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static void APP_SendDataSinkCreate(uint8_t *pParam)
\brief  This function is used to send a Data Sink Create command to APP_DEFAULT_DEST_ADDR.

\param  [in]    pParam    Not used
***************************************************************************************************/
static void APP_SendDataSinkCreate
(
    uint8_t *pParam
)
{
    uint8_t aCommand[] = {"create"};

    /* Send command over the air */
    if(APP_SendDataSinkCommand(aCommand, sizeof(aCommand)) == gNwkStatusSuccess_c)
    {
        /* Local data sink create */
        (void)THR_GetIP6Addr(mThrInstanceId, gMLEIDAddr_c, &gCoapDestAddress, NULL);
    }
}

/*!*************************************************************************************************
\private
\fn     static void APP_SendDataSinkRelease(void *pParam)
\brief  This function is used to send a Data Sink Release command to APP_DEFAULT_DEST_ADDR.

\param  [in]    pParam    Pointer to stack event
***************************************************************************************************/
static void APP_SendDataSinkRelease
(
    uint8_t *pParam
)
{
    uint8_t aCommand[] = {"release"};

    /* Send command over the air */
    if(APP_SendDataSinkCommand(aCommand, sizeof(aCommand)) == gNwkStatusSuccess_c)
    {
        /* Local data sink release */
        APP_LocalDataSinkRelease(pParam);
    }
}
#endif

#if gKBD_KeysCount_c > 1
/*!*************************************************************************************************
\private
\fn     static void APP_SendLedCommand(uint8_t *pCommand, uint8_t dataLen)
\brief  This function is used to send a Led command to gCoapDestAddress.

\param  [in]    pCommand    Pointer to command data
\param  [in]    dataLen     Data length
***************************************************************************************************/
static void APP_SendLedCommand
(
    uint8_t *pCommand,
    uint8_t dataLen
)
{
    if(!IP_IF_IsMyAddr(gIpIfSlp0_c, &gCoapDestAddress))
    {
        coapSession_t *pSession = COAP_OpenSession(mAppCoapInstId);

        if(pSession)
        {
            coapMsgTypesAndCodes_t coapMessageType = gCoapMsgTypeNonPost_c;

            pSession->pCallback = NULL;
            FLib_MemCpy(&pSession->remoteAddrStorage.ss_addr, &gCoapDestAddress, sizeof(ipAddr_t));
            pSession->pUriPath = (coapUriPath_t *)&gAPP_LED_URI_PATH;

            if(!IP6_IsMulticastAddr(&gCoapDestAddress))
            {
                coapMessageType = gCoapMsgTypeConPost_c;
                COAP_SetCallback(pSession, APP_CoapGenericCallback);
            }
            else
            {
                APP_ProcessLedCmd(pCommand, dataLen);
            }
            COAP_Send(pSession, coapMessageType, pCommand, dataLen);
        }
    }
    else
    {
        APP_ProcessLedCmd(pCommand, dataLen);
    }
}

/*!*************************************************************************************************
\private
\fn     static void APP_SendLedRgbOn(void *pParam)
\brief  This function is used to send a Led RGB On command over the air.

\param  [in]    pParam    Not used
***************************************************************************************************/
static void APP_SendLedRgbOn
(
    uint8_t *pParam
)
{
    uint8_t aCommand[] = {"rgb r000 g000 b000"};
    uint8_t redValue, greenValue, blueValue;

    /* Red value on: 0x01 - 0xFF */
    redValue = (uint8_t)NWKU_GetRandomNoFromInterval(0x01, THR_ALL_FFs8);

    /* Green value on: 0x01 - 0xFF */
    greenValue = (uint8_t)NWKU_GetRandomNoFromInterval(0x01, THR_ALL_FFs8);

    /* Blue value on: 0x01 - 0xFF */
    blueValue = (uint8_t)NWKU_GetRandomNoFromInterval(0x01, THR_ALL_FFs8);

    NWKU_PrintDec(redValue, aCommand + 5, 3, TRUE);     //aCommand + strlen("rgb r")
    NWKU_PrintDec(greenValue, aCommand + 10, 3, TRUE);  //aCommand + strlen("rgb r000 g")
    NWKU_PrintDec(blueValue, aCommand + 15, 3, TRUE);   //aCommand + strlen("rgb r000 g000 b")

    APP_SendLedCommand(aCommand, sizeof(aCommand));
}

/*!*************************************************************************************************
\private
\fn     static void APP_SendLedRgbOff(uint8_t *pParam)
\brief  This function is used to send a Led RGB Off command over the air.

\param  [in]    pParam    Not used
***************************************************************************************************/
static void APP_SendLedRgbOff
(
    uint8_t *pParam
)
{
    uint8_t aCommand[] = {"rgb r000 g000 b000"};

    APP_SendLedCommand(aCommand, sizeof(aCommand));
}

/*!*************************************************************************************************
\private
\fn     static void APP_SendLedFlash(uint8_t *pParam)
\brief  This function is used to send a Led flash command over the air.

\param  [in]    pParam    Not used
***************************************************************************************************/
static void APP_SendLedFlash
(
    uint8_t *pParam
)
{
    uint8_t aCommand[] = {"flash"};

    APP_SendLedCommand(aCommand, sizeof(aCommand));
}

/*!*************************************************************************************************
\private
\fn     static void APP_SendLedColorWheel(uint8_t *pParam)
\brief  This function is used to send a Led color wheel command over the air.

\param  [in]    pParam    Not used
***************************************************************************************************/
static void APP_SendLedColorWheel
(
    uint8_t *pParam
)
{
    uint8_t aCommand[] = {"color wheel"};

    APP_SendLedCommand(aCommand, sizeof(aCommand));
}
#endif

/*!*************************************************************************************************
\private
\fn     static void APP_LocalDataSinkRelease(uint8_t *pParam)
\brief  This function is used to restore the default destination address for CoAP messages.

\param  [in]    pParam    Not used
***************************************************************************************************/
static void APP_LocalDataSinkRelease
(
    uint8_t *pParam
)
{
    ipAddr_t defaultDestAddress = APP_DEFAULT_DEST_ADDR;

    FLib_MemCpy(&gCoapDestAddress, &defaultDestAddress, sizeof(ipAddr_t));
    (void)pParam;
}

/*!*************************************************************************************************
\private
\fn     static void APP_CoapLedCb(coapSessionStatus_t sessionStatus, uint8_t *pData,
                                  coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for CoAP LED message.
\brief  It performs the required operations and sends back a CoAP ACK message.

\param  [in]    sessionStatus   Status for CoAP session
\param  [in]    pData           Pointer to CoAP message payload
\param  [in]    pSession        Pointer to CoAP session
\param  [in]    dataLen         Length of CoAP payload
***************************************************************************************************/
static void APP_CoapLedCb
(
    coapSessionStatus_t sessionStatus,
    uint8_t *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    /* Process the command only if it is a POST method */
    if((pData) && (sessionStatus == gCoapSuccess_c) && (pSession->code == gCoapPOST_c))
    {
        APP_ProcessLedCmd(pData, dataLen);
    }

    /* Send the reply if the status is Success or Duplicate */
    if((gCoapFailure_c != sessionStatus) && (gCoapConfirmable_c == pSession->msgType))
    {
        /* Send CoAP ACK */
        COAP_Send(pSession, gCoapMsgTypeAckSuccessChanged_c, NULL, 0);
    }
}

/*!*************************************************************************************************
\private
\fn     static void APP_ProcessLedCmd(uint8_t *pCommand, uint8_t dataLen)
\brief  This function is used to process a LED command (on, off, flash, toggle, rgb, color wheel).

\param  [in]    pCommand    Pointer to command data
\param  [in]    dataLen     Data length
***************************************************************************************************/
static void APP_ProcessLedCmd
(
    uint8_t *pCommand,
    uint8_t dataLen
)
{
    /* Set mode state */
    APP_SetMode(mThrInstanceId, gDeviceMode_Application_c);

    /* Process command */
    if(FLib_MemCmp(pCommand, "on", 2))
    {
        App_UpdateStateLeds(gDeviceState_AppLedOn_c);
    }
    else if(FLib_MemCmp(pCommand, "off", 3))
    {
        App_UpdateStateLeds(gDeviceState_AppLedOff_c);
    }
    else if(FLib_MemCmp(pCommand, "toggle", 6))
    {
        App_UpdateStateLeds(gDeviceState_AppLedToggle_c);
    }
    else if(FLib_MemCmp(pCommand, "flash", 5))
    {
        App_UpdateStateLeds(gDeviceState_AppLedFlash_c);
    }
    else if(FLib_MemCmp(pCommand, "rgb", 3))
    {
        char* p = (char *)pCommand + strlen("rgb");
        uint8_t redValue = 0, greenValue = 0, blueValue = 0;
        appDeviceState_t appState = gDeviceState_AppLedRgb_c;

        dataLen -= strlen("rgb");

        while(dataLen != 0)
        {
            if(*p == 'r')
            {
                p++;
                dataLen--;
                redValue = NWKU_atoi(p);
            }

            if(*p == 'g')
            {
                p++;
                dataLen--;
                greenValue = NWKU_atoi(p);
            }

            if(*p == 'b')
            {
                p++;
                dataLen--;
                blueValue = NWKU_atoi(p);
            }

            dataLen--;
            p++;
        }

        /* Update RGB values */
#if gLedRgbEnabled_d
        Led_UpdateRgbState(redValue, greenValue, blueValue);
#else
        appState = gDeviceState_AppLedOff_c;
        if(redValue || greenValue || blueValue)
        {
            appState = gDeviceState_AppLedOn_c;
        }
#endif
        App_UpdateStateLeds(appState);
    }
    else if(FLib_MemCmp(pCommand, "color wheel", 11))
    {
#if gLedRgbEnabled_d
        App_UpdateStateLeds(gDeviceState_AppLedColorWheel_c);
#else
        App_UpdateStateLeds(gDeviceState_AppLedFlash_c);
#endif
    }
}

/*!*************************************************************************************************
\private
\fn     static void APP_CoapTempCb(coapSessionStatus_t sessionStatus, uint8_t *pData,
                                   coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for CoAP temperature message.
\brief  It sends the temperature value in a CoAP ACK message.

\param  [in]    sessionStatus   Status for CoAP session
\param  [in]    pData           Pointer to CoAP message payload
\param  [in]    pSession        Pointer to CoAP session
\param  [in]    dataLen         Length of CoAP payload
***************************************************************************************************/
static void APP_CoapTempCb
(
    coapSessionStatus_t sessionStatus,
    uint8_t *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    uint8_t *pTempString = NULL;
    uint32_t ackPloadSize = 0, maxDisplayedString = 10;

    /* Send CoAP ACK */
    if(gCoapGET_c == pSession->code)
    {
        /* Get Temperature */
        pTempString = App_GetTempDataString();
        ackPloadSize = strlen((char*)pTempString);
    }
    /* Do not parse the message if it is duplicated */
    else if((gCoapPOST_c == pSession->code) && (sessionStatus == gCoapSuccess_c))
    {
        if(NULL != pData)
        {
            char addrStr[INET6_ADDRSTRLEN];
            uint8_t temp[10];

            ntop(AF_INET6, (ipAddr_t*)&pSession->remoteAddrStorage.ss_addr, addrStr, INET6_ADDRSTRLEN);
            shell_write("\r");

            if(0 != dataLen)
            {
                /* Prevent from buffer overload */
                (dataLen >= maxDisplayedString) ? (dataLen = (maxDisplayedString - 1)) : (dataLen);
                temp[dataLen]='\0';
                FLib_MemCpy(temp,pData,dataLen);
                shell_printf((char*)temp);
            }
            shell_printf("\tFrom IPv6 Address: %s\n\r", addrStr);
            shell_refresh();
        }
    }

    if(gCoapConfirmable_c == pSession->msgType)
    {
        if(gCoapGET_c == pSession->code)
        {
            COAP_Send(pSession, gCoapMsgTypeAckSuccessContent_c, pTempString, ackPloadSize);
        }
        else
        {
            COAP_Send(pSession, gCoapMsgTypeAckSuccessChanged_c, NULL, 0);
        }
    }

    if(pTempString)
    {
        MEM_BufferFree(pTempString);
    }
}

/*!*************************************************************************************************
\private
\fn     static void APP_CoapSinkCb(coapSessionStatus_t sessionStatus, uint8_t *pData,
                                   coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for CoAP sink message.

\param  [in]    sessionStatus   Status for CoAP session
\param  [in]    pData           Pointer to CoAP message payload
\param  [in]    pSession        Pointer to CoAP session
\param  [in]    dataLen         Length of CoAP payload
***************************************************************************************************/
static void APP_CoapSinkCb
(
    coapSessionStatus_t sessionStatus,
    uint8_t *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    /* Do not execute the command multiple times, if the received message is duplicated */
    if((pData) && (sessionStatus == gCoapSuccess_c))
    {
        /* Process command */
        if(FLib_MemCmp(pData, "create",6))
        {
            /* Data sink create */
            FLib_MemCpy(&gCoapDestAddress, &pSession->remoteAddrStorage.ss_addr, sizeof(ipAddr_t));
        }

        if(FLib_MemCmp(pData, "release",7))
        {
            /* Data sink release */
            APP_LocalDataSinkRelease(NULL);
        }
    }

    if(gCoapConfirmable_c == pSession->msgType)
    {
        /* Send CoAP ACK */
        COAP_Send(pSession, gCoapMsgTypeAckSuccessChanged_c, NULL, 0);
    }
}
/*==================================================================================================
Private debug functions
==================================================================================================*/
