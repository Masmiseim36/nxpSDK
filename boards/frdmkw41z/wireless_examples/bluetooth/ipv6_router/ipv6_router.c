/*! *********************************************************************************
* \addtogroup IPv6 Router
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the IPv6 Router application
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"

/* Framework / Drivers */
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

#include "ble_service_discovery.h"

/* Nwk IP Manager*/
#include "nwk_ip_manager.h"
#include "nwk_ip_init.h"
#include "ble_abs_types.h"

/* 6LoBLE */
#include "sixloble_interface.h"


#include "shell_nwk_ip.h"
#include "ApplMain.h"
#include "ipv6_router.h"

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

typedef enum appEvent_tag{
    mAppEvt_PeerConnected_c,
    mAppEvt_ServiceDiscoveryComplete_c,
    mAppEvt_ServiceDiscoveryFailed_c,	
    mAppEvt_CbConnectionComplete_c,
    mAppEvt_CbConnectionFailed_c,
    mAppEvt_EncryptionComplete_c,
}appEvent_t;

typedef enum appState_tag{
    mAppIdle_c,
    mAppServiceDisc_c,
    mAppCbConnecting_c,
    mAppRunning_c,
}appState_t;

typedef struct appCustomInfo_tag
{
    uint8_t info;
    /* Add persistent information here */
}appCustomInfo_t;

typedef struct appPeerInfo_tag
{
    deviceId_t          deviceId;
    uint16_t            channelId;
    bleDeviceAddress_t  deviceAddress;
    bleAddressType_t    addrType;       
    appCustomInfo_t     customInfo;
    bool_t              isBonded;
    appState_t          appState;
}appPeerInfo_t;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#if gAppUseBonding_d
static bool_t mRestoringBondedLink = FALSE;
#endif

static bool_t   mScanningOn = FALSE;

/* Profile Data*/
static ipspConfig_t mIpspRouterConfig = {gGattDbInvalidHandle_d, gIpsp_Router_c, mAppLeCbInitialCredits_c};

/* Application specific data*/
static appPeerInfo_t maPeerInformation[gAppMaxConnections_c];
static uint8_t mcActiveConnections = 0;

static bleDeviceAddress_t   maBleDeviceAddress;
static bleAddressType_t     mBleAddrType;    

static instanceId_t mSixLoBleInstanceId;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

static void BleApp_Config();

/* Host Stack callbacks */
static void BleApp_ScanningCallback 
(
    gapScanningEvent_t* pScanningEvent
);

static void BleApp_ConnectionCallback 
(
    deviceId_t peerDeviceId, 
    gapConnectionEvent_t* pConnectionEvent
);

static void BleApp_ServiceDiscoveryCallback
(
	deviceId_t deviceId,
	servDiscEvent_t* pEvent
);


static void BleApp_IpspDataCallback(deviceId_t deviceId, uint16_t channelId, uint8_t* pPacket, uint16_t packetLength);
static void BleApp_IpspControlCallback(l2capControlMessageType_t  messageType, void* pMessage);

static void BleApp_StateMachineHandler
(
    deviceId_t peerDeviceId,
    uint8_t event
);

static bool_t CheckScanEvent(gapScannedDevice_t* pData);

static void BleApp_BleDataReq(bleDataReq_t * pParam, uint32_t upperLayerInstanceId);

static uint32_t   BleApp_GetMaxMsduSize(bleDataReq_t *pParam);
static void   BleApp_GetBleAddress(bleAddr_t aOutAddress);
static bool_t BleApp_GetPeerBleAddress(deviceId_t, bleAddr_t aOutAddress);

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
    shell_init("BLE IPv6 Router>");
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
    if (!mScanningOn)
    {
    	shell_write("\r\nScanning...\r\n");
        App_StartScanning(&gScanParams, BleApp_ScanningCallback, TRUE);
    }
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    switch (events) {
    	case gKBD_EventPressPB1_c:
            BleApp_Start();
            break;
        
        case gKBD_EventPressPB2_c:
        case gKBD_EventLongPB1_c:
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
            /* Configure as GAP Central */
            BleConnManager_GapCentralConfig();
        }
        break;    
        case gPublicAddressRead_c:
        {
            FLib_MemCpy(maBleDeviceAddress, pGenericEvent->eventData.aAddress, sizeof(bleDeviceAddress_t));
            mBleAddrType = gBleAddrTypePublic_c;
            BleApp_Config();
        }
        break;    
        default: 
            break;
    }
}
    
void BleApp_BleDataReq(bleDataReq_t * pParam, uint32_t upperLayerInstanceId)
{
    uint8_t i;
    bleDeviceAddress_t address;
    
    for (i=0; i<gAppMaxConnections_c; i++)
    {
        if (maPeerInformation[i].deviceId != gInvalidDeviceId_c)
        {
            BleApp_GetPeerBleAddress(i, address);
            
            if (FLib_MemCmp(address, pParam->dstAddr, sizeof(bleDeviceAddress_t)) || pParam->isMCast)
            {
                Ipsp_SendData(maPeerInformation[i].deviceId, 
                              maPeerInformation[i].channelId, 
                              pParam->pSdu, 
                              pParam->sduLength);
            }
        }
    }
}


uint32_t BleApp_GetMaxMsduSize(bleDataReq_t *pParam)
{
    return gL2capMtu_d;
}

static void BleApp_GetBleAddress(bleAddr_t aOutAddress)
{
    FLib_MemCpyReverseOrder(aOutAddress, maBleDeviceAddress, sizeof(bleDeviceAddress_t));
    
    /* Mitigate the flip of the universal/local bit */
    if (mBleAddrType == gBleAddrTypeRandom_c)
    {
        aOutAddress[0] ^= 0x02;
    }
}

static bool_t BleApp_GetPeerBleAddress(deviceId_t deviceId, bleAddr_t aOutAddress)
{
    FLib_MemCpyReverseOrder(aOutAddress, 
                            maPeerInformation[deviceId].deviceAddress, 
                            sizeof(bleDeviceAddress_t));
    
    /* Mitigate the flip of the universal/local bit */
    if (mBleAddrType == gBleAddrTypeRandom_c)
    {
        aOutAddress[0] ^= 0x02;
    }
    
    /* Send status of more peer addresses pending */
    return (deviceId < (mcActiveConnections - 1));
    
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
    /* Register for callbacks*/
    App_RegisterGattClientProcedureCallback(BleServDisc_SignalGattClientEvent);
    BleServDisc_RegisterCallback(BleApp_ServiceDiscoveryCallback);
	
    /* Register IPSP PSM */
    L2ca_RegisterLePsm(gL2ca_Le_Psm_Ipsp_c, gL2capMtu_d);  
    
    /* Initialize Network IP Manager module */
    NwkIpManager_Init();

    /* Start as an IPv6 Router */
    NwkIpManager_StartRouter();
    
    /* UI */
    shell_write("\r\nPress SW4 to connect to a IPSP Node!\r\n");
}


/*! *********************************************************************************
* \brief        Handles BLE Scanning callback from host stack.
*
* \param[in]    pScanningEvent    Pointer to gapScanningEvent_t.
********************************************************************************** */
static void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
    switch (pScanningEvent->eventType)
    {
        case gDeviceScanned_c:
        {
            if (CheckScanEvent(&pScanningEvent->eventData.scannedDevice))
            {        
                gConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
                FLib_MemCpy(gConnReqParams.peerAddress, 
                            pScanningEvent->eventData.scannedDevice.aAddress,
                            sizeof(bleDeviceAddress_t));
#if gAppUsePrivacy_d
                gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
#endif
                /* UI */
                shell_write("\r\nConnecting...");

                /* Connect to discovered device */
                App_Connect(&gConnReqParams, BleApp_ConnectionCallback);
                
                Gap_StopScanning();
            }
        }        
        break;
        case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;
            
            /* Node starts scanning */
            if (mScanningOn)
            { 
                LED_StopFlashingAllLeds();
                Led1Flashing();
            }
            /* Node is not scanning */
            else
            {                
                Led1Flashing();
                Led2Flashing();
                Led3Flashing();
            	Led4Flashing();
            }
        }
        break;
        case gScanCommandFailed_c:
        {
            panic(0,0,0,0);
            break;
        }
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
	BleConnManager_GapCentralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            /* UI */
            LED_StopFlashingAllLeds();
            Led1On();
            shell_write("\r\nConnected! Device Id: ");
            shell_writeDec(peerDeviceId);

            maPeerInformation[peerDeviceId].appState = mAppIdle_c;                        
            maPeerInformation[peerDeviceId].deviceId = peerDeviceId;
            maPeerInformation[peerDeviceId].isBonded = FALSE;    
            
            FLib_MemCpy(maPeerInformation[peerDeviceId].deviceAddress, 
                        pConnectionEvent->eventData.connectedEvent.peerAddress, 
                        sizeof(bleDeviceAddress_t));
            maPeerInformation[peerDeviceId].addrType = pConnectionEvent->eventData.connectedEvent.peerAddressType;
                    			
#if gAppUseBonding_d            
            Gap_CheckIfBonded(peerDeviceId, &maPeerInformation[peerDeviceId].isBonded);
            
            if ((maPeerInformation[peerDeviceId].isBonded) &&
                (gBleSuccess_c == Gap_LoadCustomPeerInformation(peerDeviceId,
                    (void*) &maPeerInformation[peerDeviceId].customInfo, 0, sizeof (appCustomInfo_t))))
            {
                mRestoringBondedLink = TRUE;
                /* Restored custom connection information. Encrypt link */
                Gap_EncryptLink(peerDeviceId);
            }
#endif
            BleApp_StateMachineHandler(peerDeviceId, mAppEvt_PeerConnected_c);
            
            mcActiveConnections++;
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            maPeerInformation[peerDeviceId].deviceId = gInvalidDeviceId_c;
            maPeerInformation[peerDeviceId].appState = mAppIdle_c;               
            maPeerInformation[peerDeviceId].isBonded = FALSE;            
            /* UI */
            LED_TurnOffAllLeds();
            LED_StartFlash(LED_ALL);
            shell_write("\r\nDisconnected! Device Id: ");
            shell_writeDec(peerDeviceId);
            
            mcActiveConnections = (mcActiveConnections > 0)?
              mcActiveConnections - 1:
              0;
        }
        break;

#if gAppUsePairing_d		
        case gConnEvtEncryptionChanged_c:
        {
            if (pConnectionEvent->eventData.encryptionChangedEvent.newEncryptionState)
            {
                mRestoringBondedLink = FALSE;
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_EncryptionComplete_c);
            }
        }
        break;
#endif /* gAppUsePairing_d */

    default:
        break;
    }
}

static void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent)
{
    switch(pEvent->eventType)
    {
        case gServiceDiscovered_c:
        {
            BleApp_StateMachineHandler(peerDeviceId, mAppEvt_ServiceDiscoveryComplete_c);
        }
        break;

        case gDiscoveryFinished_c:
        {
            if (!pEvent->eventData.success)
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_ServiceDiscoveryFailed_c);
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
            BleApp_GetPeerBleAddress(deviceId, pBleDataInd->srcAddr);
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
    Ipsp_ControlCallback(&mIpspRouterConfig, messageType, pMessage);

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
                
                maPeerInformation[pConnComplete->deviceId].channelId = pConnComplete->cId;
                
                /* Call App State Machine */
                BleApp_StateMachineHandler(pConnComplete->deviceId, mAppEvt_CbConnectionComplete_c);
            }
            else
            {
                /* UI */
#if gAppIpspDebugLog_d               
                shell_write("\r\nLE Credit Based Connection Failed with Reason: \r\n");
                shell_writeHex((uint8_t *)&pConnComplete->result, sizeof(l2caLeCbConnectionRequestResult_t));
#endif                
                switch (pConnComplete->result)
                {
#if gAppUsePairing_d                  
                    case gInsufficientAuthentication_c:
                    case gInsufficientAuthorization_c:
                    case gInsufficientEncryptionKeySize_c:
                    case gInsufficientEncryption_c:
                    {
                            shell_write("\r\nPairing...\r\n");
                            Gap_Pair(pConnComplete->deviceId, &gPairingParameters);
                    }
                    break;
#endif                    
                    default:
                    {
                        /* Call App State Machine */
                        BleApp_StateMachineHandler(pConnComplete->deviceId, mAppEvt_CbConnectionFailed_c);                      
                    }
                    break;
                }
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

static bool_t MatchDataInAdvElementList(gapAdStructure_t *pElement, uint16_t data)
{ 
    uint8_t i;
    
    for (i=0; i < pElement->length; i+=sizeof(uint16_t))
    {
        if (FLib_MemCmp(&data, &pElement->aData[i], sizeof(uint16_t)))
        {
            return TRUE;
        } 
    }
    
    return FALSE;
}

static bool_t CheckScanEvent(gapScannedDevice_t* pData)
{
    uint8_t index = 0;
    uint8_t name[10];
    uint8_t nameLength;
    bool_t foundMatch = FALSE;
    
    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;
        
        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1];
        adElement.aData = &pData->data[index + 2];
      
         /* Search for Temperature Custom Service */
        if ((adElement.adType == gAdIncomplete16bitServiceList_c) ||
          (adElement.adType == gAdComplete16bitServiceList_c))
        {
            foundMatch = MatchDataInAdvElementList(&adElement, gBleSig_IpsService_d);
        }
        
        if ((adElement.adType == gAdShortenedLocalName_c) ||
          (adElement.adType == gAdCompleteLocalName_c))
        {
            nameLength = MIN(adElement.length, 10);
            FLib_MemCpy(name, adElement.aData, nameLength);
        }
        
        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);
    }
    
    if (foundMatch)
    {
        /* UI */
        shell_write("\r\nFound device: ");
        shell_writeN((char*)name, nameLength-1);
        shell_write(" 0x");
        shell_writeHexLe(pData->aAddress, 6);
    }
    return foundMatch;
}

static void BleApp_StateMachineHandler(deviceId_t peerDeviceId, uint8_t event)
{
    switch (maPeerInformation[peerDeviceId].appState)
    {
        case mAppIdle_c:
        {
            if (event == mAppEvt_PeerConnected_c)
            {
                uint16_t uuid = gBleSig_IpsService_d;

                /* UI */
                shell_write("\r\nDiscovering IPSS...");

                /* Moving to Service Discovery State*/
                maPeerInformation[peerDeviceId].appState = mAppServiceDisc_c;

                /* Start Service Discovery*/
                BleServDisc_FindService(peerDeviceId, gBleUuidType16_c, (bleUuid_t*) &uuid);
            }
        }
        break;

        case mAppServiceDisc_c:
        {
            if (event == mAppEvt_ServiceDiscoveryComplete_c)
            {
                /* UI */
                shell_write("\r\nInitiating LE Credit Based Connection request...");
                
                /* Moving to Credit Based Connecting State */
                maPeerInformation[peerDeviceId].appState = mAppCbConnecting_c;

#if gAppUseBonding_d                
                if (!mRestoringBondedLink)
#endif                  
                {
                    /* Initiate IPSP Connection */
                    Ipsp_Connect(&mIpspRouterConfig, peerDeviceId);
                }
            }
            else if (event == mAppEvt_ServiceDiscoveryFailed_c)
            {
                Gap_Disconnect(peerDeviceId);
            }
        }
        break;

        case mAppCbConnecting_c:
        {
            if (event == mAppEvt_CbConnectionComplete_c)
            {
                /* Moving to Running State */
                maPeerInformation[peerDeviceId].appState = mAppRunning_c;
            }
            else if(event == mAppEvt_EncryptionComplete_c)
            {
                /* Retry initiating LE CB Connection */
                Ipsp_Connect(&mIpspRouterConfig, peerDeviceId);
            }
        }
        break;

        case mAppRunning_c:
        {
          
        }
        break;
    }
}

/*! *********************************************************************************
* @}
********************************************************************************** */
