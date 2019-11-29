/*! *********************************************************************************
* \addtogroup IPv6 Node
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the IPv6 Node application
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "EmbeddedTypes.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "Panic.h"
#include "shell.h"

/* BLE Host Stack */
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_handles.h"
#include "l2ca_cb_interface.h"
#include "l2ca_types.h"

/* Profile / Services */
#include "ipsp_interface.h"

/* Connection Manager */
#include "ble_conn_manager.h"

/* Nwk IP Manager*/
#include "nwk_ip_manager.h"
#include "nwk_ip_init.h"
#include "ble_abs_types.h"

/* 6LoBLE */
#include "sixloble_interface.h"


#include "shell_nwk_ip.h"
#include "ApplMain.h"
#include "ipv6_node.h"

#include "stdlib.h"
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mAppLeCbInitialCredits_c  50
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
#if gAppUseBonding_d
    fastWhiteListAdvState_c,
#endif
    fastAdvState_c,
    slowAdvState_c
}advType_t;

typedef struct advState_tag{
    bool_t      advOn;
    advType_t   advType;
}advState_t;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* Adv State */
static advState_t  mAdvState;
static deviceId_t  mPeerDeviceId = gInvalidDeviceId_c;
static uint16_t    mChannelId;


/* Profile Data*/
static ipspConfig_t mIpspNodeConfig = {service_ipss, gIpsp_Node_c, mAppLeCbInitialCredits_c};

/* Application specific data*/
static tmrTimerID_t mAdvTimerId;

static bleDeviceAddress_t   maBleDeviceAddress;
static bleAddressType_t     mBleAddrType; 
static bleAddressType_t     mPeerAddressType;
static bleDeviceAddress_t   mPeerAddress;            

static instanceId_t mSixLoBleInstanceId;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void BleApp_Config();
static void BleApp_Advertise(void);

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_IpspDataCallback(deviceId_t deviceId, uint16_t channelId, uint8_t* pPacket, uint16_t packetLength);
static void BleApp_IpspControlCallback(l2capControlMessageType_t  messageType, void* pMessage);

/* Timer Callbacks */
static void AdvertisingTimerCallback (void *);

int8_t BleApp_SendIpspData(uint8_t argc, char * argv[]);
int8_t BleApp_SendIpspTestData(uint8_t argc, char * argv[]);

void BleApp_BleDataReq(bleDataReq_t * pParam, uint32_t upperLayerInstanceId);

uint32_t   BleApp_GetMaxMsduSize(bleDataReq_t *pParam);

void   BleApp_GetBleAddress(bleAddr_t aOutAddress);
bool_t   BleApp_GetPeerBleAddress(deviceId_t deviceId, bleAddr_t aOutAddress);

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

static const bleAbsRequests_t bleAbsReq =
{
    .BLE_DataReq = BleApp_BleDataReq,
    .GetMaxSduSize = BleApp_GetMaxMsduSize,
    .GetBleAddress = BleApp_GetBleAddress,
    .GetPeerBleAddress = BleApp_GetPeerBleAddress    
};

/*! The default block size used in block-wise transfer
    Possible values are 16, 32, 64, 128, 256, 512, 1024 */
const uint32_t gCoapBlockSize = 64;
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
extern void ResetMCU(void);

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
	/* Initialize the Network IP Stack Task */
    NwkIpManager_TaskInit();

	/* UI */
    shell_init("BLE IPv6 Node>");
    SHELLComm_Init();

    TurnOnLeds();
    TurnOffLeds();    
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    (void)gCoapBlockSize;
    /* Device is not connected and not advertising*/
    if (!mAdvState.advOn)
    {  
#if gAppUseBonding_d
        if (gcBondedDevices > 0)
        {
            mAdvState.advType = fastWhiteListAdvState_c;
        }
        else
        {
#endif
            mAdvState.advType = fastAdvState_c;
#if gAppUseBonding_d
        }
#endif

        BleApp_Advertise();
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
        case gKBD_EventPressPB1_c:
        {
            BleApp_Start();
            break;
        }

        case gKBD_EventLongPB1_c:
        {
            Gap_Disconnect(mPeerDeviceId);
        }
        break;
        case gKBD_EventPressPB2_c:
        case gKBD_EventLongPB2_c:
        default:
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
    /* Call BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);
    
    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:    
        {
            /* Configure as GAP peripheral */
            BleConnManager_GapPeripheralConfig();
        }
        break;    
        case gPublicAddressRead_c:
        {
            FLib_MemCpy(maBleDeviceAddress, pGenericEvent->eventData.aAddress, sizeof(bleDeviceAddress_t));
            mBleAddrType = gBleAddrTypePublic_c;
            BleApp_Config();
        }
        break;
        
        case gAdvertisingParametersSetupComplete_c:
        {
            App_StartAdvertising(BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
        }
        break;         

        default: 
            break;
    }
}
    
void BleApp_BleDataReq(bleDataReq_t * pParam, uint32_t upperLayerInstanceId)
{
    /* The device is a node. Send directly to the peer */
    Ipsp_SendData(mPeerDeviceId, mChannelId, pParam->pSdu, pParam->sduLength);
}


uint32_t BleApp_GetMaxMsduSize(bleDataReq_t *pParam)
{
    return gL2capMtu_d;
}

void BleApp_GetBleAddress(bleAddr_t aOutAddress)
{
    FLib_MemCpyReverseOrder(aOutAddress, maBleDeviceAddress, sizeof(bleDeviceAddress_t));
    
    /* Mitigate the flip of the universal/local bit */
    if (mBleAddrType == gBleAddrTypeRandom_c)
    {
        aOutAddress[0] ^= 0x02;
    }
}

bool_t BleApp_GetPeerBleAddress(deviceId_t deviceId, bleAddr_t aOutAddress)
{
    (void) deviceId;
    FLib_MemCpyReverseOrder(aOutAddress, mPeerAddress, sizeof(bleDeviceAddress_t));
    
    /* Mitigate the flip of the universal/local bit */
    if (mPeerAddressType == gBleAddrTypeRandom_c)
    {
        aOutAddress[0] ^= 0x02;
    }
    
    return FALSE;
}

bleAbsRequests_t* BleApp_RegisterBleCallbacks(instanceId_t instanceId)
{
    mSixLoBleInstanceId = instanceId;
    L2ca_RegisterLeCbCallbacks(BleApp_IpspDataCallback, BleApp_IpspControlCallback); 
    return (void *)&bleAbsReq;
}

/*!*************************************************************************************************
\fn     void APP_CriticalExitCb(uint32_t location, uint32_t param)
\brief  If the stack is in a deadlock situation, it calls APP_CriticalExitCb.

\param  [in]  location  Address where the Panic occurred
\param  [in]  param     Parameter with extra debug information
***************************************************************************************************/
void APP_CriticalExitCb
(
    uint32_t location,
    uint32_t param
)
{
   panic(0, location, param, 0);
   ResetMCU();
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
static void BleApp_Config()
{

    /* Register IPSP PSM */
    L2ca_RegisterLePsm(gL2ca_Le_Psm_Ipsp_c, gL2capMtu_d);
    
    /* Setup Advertising and scanning data */
    Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);

    mAdvState.advOn = FALSE;
    
    NwkIpManager_Init();
    
    /* Allocate aplication timer */
    mAdvTimerId = TMR_AllocateTimer();
    /* UI */
    shell_write("\r\nPress SW4 to start advertising!\r\n");
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will satrt after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    uint32_t timeout = 0;

    switch (mAdvState.advType)
    {
#if gAppUseBonding_d
        case fastWhiteListAdvState_c:
        {
            gAdvParams.minInterval = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessWhiteListOnly_c;
            timeout = gFastConnWhiteListAdvTime_c;
        }
        break;
#endif
        case fastAdvState_c:
        {
            gAdvParams.minInterval = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            timeout = gFastConnAdvTime_c - gFastConnWhiteListAdvTime_c;
        }
        break;

        case slowAdvState_c:
        {
            gAdvParams.minInterval = gReducedPowerMinAdvInterval_c;
            gAdvParams.maxInterval = gReducedPowerMinAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            timeout = gReducedPowerAdvTime_c;
        }
        break;
    }

    /* UI */
    shell_write("\r\nAdvertising...");
    
    /* Set advertising parameters*/
    Gap_SetAdvertisingParameters(&gAdvParams);

    /* Start advertising timer */
    TMR_StartLowPowerTimer(mAdvTimerId,gTmrLowPowerSecondTimer_c,
               TmrSeconds(timeout), AdvertisingTimerCallback, NULL);
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
        case gAdvertisingStateChanged_c:
        {
            mAdvState.advOn = !mAdvState.advOn;
            LED_StopFlashingAllLeds();
            Led1Flashing();

            if(!mAdvState.advOn)
            {
                Led2Flashing();
                Led3Flashing();
                Led4Flashing();
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
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
            mPeerDeviceId = peerDeviceId;

            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;         
            
            FLib_MemCpy(mPeerAddress, 
                        pConnectionEvent->eventData.connectedEvent.peerAddress, 
                        sizeof(bleDeviceAddress_t));
            mPeerAddressType = pConnectionEvent->eventData.connectedEvent.peerAddressType;
        
            /* UI */
            LED_StopFlashingAllLeds();
            Led1On();
            shell_write("\r\nConnected!\r\n");
            
            /* Stop Advertising Timer*/
            TMR_StopTimer(mAdvTimerId);
            
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            mPeerDeviceId = gInvalidDeviceId_c;

            if (pConnectionEvent->eventData.disconnectedEvent.reason == gHciConnectionTimeout_c)
            {
                /* Link loss detected*/
                BleApp_Start();
            }
            else
            {
              /* Connection was terminated by peer or application */
                BleApp_Start();
            }
        }
        break;
    default:
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles IPSP Data Plane callback from host stack.
*
* \param[in]    deviceId        Peer device ID.
* \param[in]    pPacket            Pointer to data.
* \param[in]    packetLength    Packet length.
********************************************************************************** */
static void BleApp_IpspDataCallback(deviceId_t deviceId, uint16_t channelId, uint8_t* pPacket, uint16_t packetLength)
{
    bleDataInd_t* pBleDataInd = NULL;
    
    pBleDataInd = MEM_BufferAlloc(sizeof(bleDataInd_t));

    /* Create the BLE Data indication */
    if (pBleDataInd != NULL)
    {
        pBleDataInd->pSdu = MEM_BufferAlloc(packetLength);
        
        if (pBleDataInd->pSdu)
        {
            pBleDataInd->instanceId = mSixLoBleInstanceId;
            BleApp_GetBleAddress(pBleDataInd->dstAddr);
            BleApp_GetPeerBleAddress(0, pBleDataInd->srcAddr);
            pBleDataInd->sduLength = packetLength;
            FLib_MemCpy(pBleDataInd->pSdu, pPacket, packetLength);
            
            /* Send message to 6lo */
            SixLoBle_DataIndCB(pBleDataInd);
        }
        else
        {
            MEM_BufferFree(pBleDataInd);
        }
    }  
  
    /* UI */
#if gAppIpspDebugLog_d    
    shell_write("\r\nReceived IPSP SDU: ");
    shell_write("\r\n   Device Id: ");
    shell_writeDec(deviceId);
    shell_write("   Channel Id: ");
    shell_writeDec(channelId);
    shell_write("   SDU Length: ");
    shell_writeDec(packetLength);
    shell_write("\r\n   SDU : ");
    shell_writeHex(pPacket, packetLength);
    SHELL_NEWLINE();
    shell_refresh();
#endif                
}

/*! *********************************************************************************
* \brief        Handles IPSP Control Plane callback from host stack.
*
* \param[in]    messageType     L2CAP Control Message Type.
* \param[in]    pMessage        Pointer to message.
********************************************************************************** */
static void BleApp_IpspControlCallback(l2capControlMessageType_t  messageType, void* pMessage)
{
    Ipsp_ControlCallback(&mIpspNodeConfig, messageType, pMessage);

    switch (messageType)
    {
        case gL2ca_LePsmConnectionComplete_c:
        {
            l2caLeCbConnectionComplete_t *pConnComplete = (l2caLeCbConnectionComplete_t *)pMessage;
            
            if (pConnComplete->result == gSuccessful_c)
            {
                /* UI */
                shell_write("\r\nLE Credit Based Connection Complete!");
                shell_write("\r\n   Device Id: ");
                shell_writeDec(pConnComplete->deviceId);
                shell_write("\r\n   Channel Id: ");
                shell_writeDec(pConnComplete->cId);
                shell_write("\r\n   Peer MTU: ");
                shell_writeDec(pConnComplete->peerMtu);
                shell_write("\r\n   Peer MPS: ");
                shell_writeDec(pConnComplete->peerMps);
                shell_write("\r\n   Credits: ");
                shell_writeDec(pConnComplete->initialCredits);
                SHELL_NEWLINE();
                shell_refresh();
                
                mChannelId = pConnComplete->cId;
                
                NwkIpManager_StartNode();
            }
            else
            {
                /* UI */
#if gAppIpspDebugLog_d  
                shell_write("\r\nLE Credit Based Connection Failed with Reason: \r\n");
                shell_writeHex((uint8_t *)&pConnComplete->result, sizeof(l2caLeCbConnectionRequestResult_t));
#endif
            }
            break;
        }
        case gL2ca_LePsmDisconnectNotification_c:
        {
            l2caLeCbDisconnection_t *pDisconnect = (l2caLeCbDisconnection_t *)pMessage;
            
            Gap_Disconnect(pDisconnect->deviceId);
            break;
        }
        case gL2ca_NoPeerCredits_c:
        {
            l2caLeCbNoPeerCredits_t *pNoCredits = (l2caLeCbNoPeerCredits_t *)pMessage;
            
            /* UI */
#if gAppIpspDebugLog_d            
            shell_write("\r\nNo credits available! Sending credits to peer device...\r\n");
            SHELL_NEWLINE();
            shell_refresh();
#endif            
            L2ca_SendLeCredit(pNoCredits->deviceId,
                              pNoCredits->cId,
                              mAppLeCbInitialCredits_c);
            break;
        }
        case gL2ca_LocalCreditsNotification_c:
        {
#if gAppIpspDebugLog_d          
            l2caLeCbLocalCreditsNotification_t *pMsg = (l2caLeCbLocalCreditsNotification_t *)pMessage;

            shell_write("\r\n   Device Id: ");
            shell_writeDec(pMsg->deviceId);
            shell_write(" Channel Id: ");
            shell_writeDec(pMsg->cId);
            shell_write(" Available LE credits: ");
            shell_writeDec(pMsg->localCredits);
            SHELL_NEWLINE();
            shell_refresh();
#endif            
            break;
        }
        default:
            break;
    }
}


/*! *********************************************************************************
* \brief        Handles advertising timer callback.
*
* \param[in]    pParam        Calback parameters.
********************************************************************************** */
static void AdvertisingTimerCallback(void * pParam)
{
    /* Stop and restart advertising with new parameters */
    Gap_StopAdvertising();

    switch (mAdvState.advType)
    {
#if gAppUseBonding_d
        case fastWhiteListAdvState_c:
        {
            mAdvState.advType = fastAdvState_c;
        }
        break;
#endif
        case fastAdvState_c:
        {
            mAdvState.advType = slowAdvState_c;
        }
        break;

        default:
        break;
    }
    BleApp_Advertise();
}

/*! *********************************************************************************
* @}
********************************************************************************** */
