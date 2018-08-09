/*! *********************************************************************************
* \addtogroup BLE OTAP Client L2CAP CoC
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the source file for the BLE OTAP Client L2CAP CoC application
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
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "Panic.h"
#include "PWR_Interface.h"
#include "OtaSupport.h"

/* BLE Host Stack */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_database.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_handles.h"
#include "l2ca_cb_interface.h"
#include "l2ca_types.h"

/* Profile / Services */
#include "battery_interface.h"
#include "device_info_interface.h"
#include "otap_interface.h"


/* Connection Manager */
#include "ble_conn_manager.h"

#include "board.h"
#include "ApplMain.h"
#include "otap_client_l2cap_credit.h"

/************************************************************************************
*************************************************************************************
* Extern functions
*************************************************************************************
************************************************************************************/
extern void ResetMCU(void);

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

#define mAppLeCbInitialCredits_c        (32768)
#define mBatteryLevelReportInterval_c   (10)        /* battery level report interval in seconds  */

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
#if gAppUseBonding_d
    whiteListAdvState_c,
#endif
    advState_c,
}advType_t;

typedef struct advState_tag
{
    bool_t      advOn;
    advType_t   advType;
} advState_t;

typedef enum otapClientState_tag
{
    mOtapClientStateIdle_c                  = 0x00,
    mOtapClientStateDownloadingImage_c      = 0x01,
    mOtapClientStateImageDownloadComplete_c = 0x02,
} otapClientState_t;

/*! Structure containing the OTAP Client functional data. */
typedef struct otapClientAppData_tag
{
    otapClientState_t           state;
    const uint8_t               currentImgId[gOtap_ImageIdFieldSize_c];         /*!< Id of the currently running image on the OTAP Client */
    const uint8_t               currentImgVer[gOtap_ImageVersionFieldSize_c];   /*!< Version of the currently running image on the OTAP Client */
    deviceId_t                  peerOtapServer;                                 /*!< Device id of the OTAP Server a new image is being downloaded from. */
    uint8_t                     imgId[gOtap_ImageIdFieldSize_c];                /*!< Id of the image being downloaded from the OTAP Server */
    uint8_t                     imgVer[gOtap_ImageVersionFieldSize_c];          /*!< Version of the image being downloaded from the OTAP Server */
    uint32_t                    imgSize;                                        /*!< Size of the image file being downloaded from the OTAP Server */
    uint16_t                    imgComputedCrc;                                 /*!< Computed 16 bit CRC of the image file used in this implementation. */
    uint16_t                    imgReceivedCrc;                                 /*!< Received 16 bit CRC of the image file used in this implementation. */
    uint8_t                     imgSectorBitmap[gBootData_SectorsBitmap_Size_c];    /*!< Flash sector bitmap for the recieved image for the current implementation. */
    uint32_t                    currentPos;                                     /*!< Current position of the file being downloaded. */
    uint16_t                    chunkSize;                                      /*!< Current chunk size for the image file transfer. */
    uint16_t                    chunkSeqNum;                                    /*!< Current chunk sequence number for the block being transferred. */
    uint16_t                    totalBlockChunks;                               /*!< Total number of chunks for the block being transferred. */
    uint32_t                    totalBlockSize;                                 /*!< Total size of the block which was requested. may be smaller than totalBlockChunks * chunkSize. */
    const otapTransferMethod_t  transferMethod;                                 /*!< Currently used transfer method for the OTAP Image File */
    uint16_t                    l2capChannelOrPsm;                              /*!< L2CAP Channel or PSM used for the transfer of the image file: channel 0x0004 for ATT, application specific PSM for CoC. */
    bool_t                      serverWrittenCccd;                              /*!< The OTAP Server has written the CCCD to receive commands from the OTAp Client. */
    otapCmdIdt_t                lastCmdSentToOtapServer;                        /*!< The last command sent to the OTAP Server for which an Indication is expected. */
    uint16_t                    negotiatedMaxAttChunkSize;                      /*!< The negotiated maximum ATT chunk size based on the negotiated ATT MTU between the OTAP Server and the OTAP Client. */
    uint16_t                    negotiatedMaxL2CapChunkSize;                    /*!< The negotiated maximum L2CAP chunk size based on the negotiated L2CAP MTU between the OTAP Server and the OTAP Client. */
    bool_t                      l2capPsmConnected;                              /*!< Flag which is set to true if an L2CAP PSM connection is currently established with a peer device. */
    uint16_t                    l2capPsmChannelId;                              /*!< Channel Id for an L2CAP PSM connection currently established with a peer device. */
} otapClientAppData_t;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

static deviceId_t  mPeerDeviceId = gInvalidDeviceId_c;

/* Adv Parmeters */
static advState_t  mAdvState;
static tmrTimerID_t appTimerId;
#if gAppUseBonding_d
static bool_t   mSendDataAfterEncStart = FALSE;
#endif
/* Service Data */
static basConfig_t basServiceConfig = {service_battery, 0};
static disConfig_t disServiceConfig = {service_device_info};
static otapClientConfig_t otapServiceConfig = {service_otap};
static uint16_t otapWriteNotifHandles[] = {value_otap_control_point,
                                           value_otap_data};

/* Application Data */

/*! OTAP Client data structure.
 *  Contains current image information and state informations
 *  regarding the image download procedure. */
static otapClientAppData_t     otapClientData = 
{
    .state = mOtapClientStateIdle_c,
    .currentImgId = {0x00, 0x00},     // Current Running Image Id - should be 0x0000
    .currentImgVer = {0x01, 0x00, 0x00,    // Build Version
                      0x41,                // Stack Version
                      0x11, 0x11, 0x11,    // Hardware Id
                      0x01                 // Manufacturer Id
                     },               // Current Image Version
    .peerOtapServer = gInvalidDeviceId_c,
    .imgId = {0x00, 0x00},
    .imgVer = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .imgSize = 0,
    .imgComputedCrc = 0,
    .imgReceivedCrc = 0,
    .imgSectorBitmap = {0x00},
    .currentPos = 0,
    .chunkSize = 0,
    .chunkSeqNum = 0,
    .totalBlockChunks = 0,
    .totalBlockSize = 0,
    .transferMethod = gOtapTransferMethodL2capCoC_c,   // Transfer method L2CAP Credit based
    .l2capChannelOrPsm = gOtap_L2capLePsm_c,   // The L2CAP Otap PSM
    .lastCmdSentToOtapServer = gOtapCmdIdNoCommand_c,
    .negotiatedMaxAttChunkSize = gAttDefaultMtu_c - gOtap_AttCommandMtuDataChunkOverhead_c,
    .negotiatedMaxL2CapChunkSize = gOtap_ImageChunkDataSizeL2capCoc_c,
    .l2capPsmConnected = FALSE,
    .l2capPsmChannelId = 0,
};

static tmrTimerID_t mBatteryMeasurementTimerId;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Gatt and Att callbacks */
static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);

/* L2CAP LE PSM callbacks */
static void BleApp_L2capPsmDataCallback (deviceId_t deviceId, uint16_t lePsm, uint8_t* pPacket, uint16_t packetLength);
static void BleApp_L2capPsmControlCallback (l2capControlMessageType_t  messageType, void* pMessage);

static void BleApp_Config();

static void BleApp_CccdWritten (deviceId_t deviceId, uint16_t handle, gattCccdFlags_t cccd);
static void BleApp_AttributeWritten (deviceId_t deviceId, uint16_t handle, uint16_t length, uint8_t* pValue);
static void BleApp_AttributeWrittenWithoutResponse (deviceId_t deviceId, uint16_t handle, uint16_t length, uint8_t* pValue);
static void BleApp_HandleValueConfirmation (deviceId_t deviceId);

static void OtapClient_HandlePsmConnectionComplete (l2caLeCbConnectionComplete_t *pConnComplete);
static void OtapClient_HandlePsmDisconnection (l2caLeCbDisconnection_t * pCbDisconnect);

static void BatteryMeasurementTimerCallback (void *);

static void BleApp_Advertise (void);

/* OTAP Client functions */
/* Commands received from the OTAP Server */
static void OtapClient_HandleDataChunk (deviceId_t deviceId, uint16_t length, uint8_t* pData);
static void OtapClient_HandleNewImageNotification (deviceId_t deviceId, uint16_t length, uint8_t* pValue);
static void OtapClient_HandleNewImageInfoResponse (deviceId_t deviceId, uint16_t length, uint8_t* pValue);
static void OtapClient_HandleErrorNotification (deviceId_t deviceId, uint16_t length, uint8_t* pValue);
/* Confirmations of commands sent to the OTAP Server */
static void OtapClient_HandleNewImageInfoRequestConfirmation (deviceId_t deviceId);
static void OtapClient_HandleImageBlockRequestConfirmation (deviceId_t deviceId);
static void OtapClient_HandleImageTransferCompleteConfirmation (deviceId_t deviceId);
static void OtapClient_HandleErrorNotificationConfirmation (deviceId_t deviceId);
static void OtapClient_HandleStopImageTransferConfirmation (deviceId_t deviceId);
/* Connection and Disconnection events */
static void OtapClient_HandleConnectionEvent (deviceId_t deviceId);
static void OtapClient_HandleDisconnectionEvent (deviceId_t deviceId);
/* Otap Client operations */
static void OtapClient_ContinueImageDownload (deviceId_t deviceId);
static void OtapClient_ResetBlockTransferParameters (deviceId_t deviceId);
static void OtapClient_HandleServerCommError (deviceId_t deviceId);
static bool_t OtapClient_IsRemoteImageNewer (uint8_t* pRemoteImgId, uint8_t* pRemoteImgVer);
static otapStatus_t OtapClient_IsImageFileHeaderValid (bleOtaImageFileHeader_t* imgFileHeader);

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

    /* Initialize application specific peripher drivers here. */
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    Led1On();
    
    if (mPeerDeviceId == gInvalidDeviceId_c)
    {
        /* Device is not connected and not advertising*/
        if (!mAdvState.advOn)
        {
#if gAppUseBonding_d
            if (gcBondedDevices > 0)
            {
                mAdvState.advType = whiteListAdvState_c;
            }
            else
            {
#endif
                mAdvState.advType = advState_c;
#if gAppUseBonding_d
            }
#endif
            BleApp_Advertise();
        }
    }
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    BleApp_Start();
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
    /* Configure as GAP peripheral */
    BleConnManager_GapPeripheralConfig();
    
    /* Register OTAP L2CAP PSM */
    L2ca_RegisterLePsm (gOtap_L2capLePsm_c,
                        gOtapCmdImageChunkCocMaxLength_c);  /*!< The negotiated MTU must be higher than the biggest data chunk that will be sent fragmented */
    
    /* Register stack callbacks */
    App_RegisterLeCbCallbacks(BleApp_L2capPsmDataCallback, BleApp_L2capPsmControlCallback);
    App_RegisterGattServerCallback (BleApp_GattServerCallback);
    GattServer_RegisterHandlesForWriteNotifications (sizeof(otapWriteNotifHandles)/sizeof(otapWriteNotifHandles[0]),
                                                     otapWriteNotifHandles);

    mAdvState.advOn = FALSE;

    /* Start services */
    OtapCS_Start(&otapServiceConfig);
    
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    Bas_Start(&basServiceConfig);
    Dis_Start(&disServiceConfig);

    /* Allocate aplication timer */
    appTimerId = TMR_AllocateTimer();  
    mBatteryMeasurementTimerId = TMR_AllocateTimer();
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will satrt after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    switch (mAdvState.advType)
    {
#if gAppUseBonding_d
        case whiteListAdvState_c:
        {
            gAdvParams.filterPolicy = gProcessWhiteListOnly_c;
        }
        break;
#endif
        case advState_c:
        {
            gAdvParams.filterPolicy = gProcessAll_c;
        }
        break;
    }
    
    /* Set advertising parameters*/
    Gap_SetAdvertisingParameters(&gAdvParams);
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

            if(mAdvState.advOn)
            {
                LED_StopFlashingAllLeds();
                Led1Flashing();
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            Led2On();
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
#if gAppUseBonding_d
            bool_t isBonded = FALSE;
            
            mSendDataAfterEncStart = FALSE;
            if (gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded) &&
                TRUE == isBonded) 
            {
                mSendDataAfterEncStart = TRUE;
            }
#endif  
            
            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;
            TMR_StopTimer(appTimerId);         
            
	        /* Subscribe client*/
            mPeerDeviceId = peerDeviceId;
	        Bas_Subscribe(peerDeviceId);		
	        OtapCS_Subscribe(peerDeviceId);

			/* UI */            
            LED_StopFlashingAllLeds();
            Led1On();        
            
            /* Device does not need to sleep until some information is exchanged with the peer. */
            PWR_DisallowDeviceToSleep();
            
            OtapClient_HandleConnectionEvent (peerDeviceId);

            /* Start battery measurements */
            TMR_StartLowPowerTimer(mBatteryMeasurementTimerId, gTmrLowPowerIntervalMillisTimer_c,
                       TmrSeconds(mBatteryLevelReportInterval_c), BatteryMeasurementTimerCallback, NULL);            
        }
        break;
        
        case gConnEvtDisconnected_c:
        {
            /* Unsubscribe client */
            mPeerDeviceId = gInvalidDeviceId_c;
            Bas_Unsubscribe();
            OtapCS_Unsubscribe();
            
            /* UI */
            LED_StopFlashingAllLeds();
            Led1Flashing();
            Led2Flashing();
            Led3Flashing();
            Led4Flashing();
            
            /* Restart advertising*/
            BleApp_Start();

            OtapClient_HandleDisconnectionEvent (peerDeviceId);
        }
        break;
		
#if gAppUsePairing_d
        case gConnEvtEncryptionChanged_c:
        {
            if (mSendDataAfterEncStart)
            {
                /* Application handles encryption changes here. */
            }
        }
        break;
#endif /* gAppUsePairing_d */

    default:
        break;
    }
}

static void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent)
{
    switch (pServerEvent->eventType)
    {
    case gEvtCharacteristicCccdWritten_c:
    {
            
        BleApp_CccdWritten (deviceId,
                            pServerEvent->eventData.charCccdWrittenEvent.handle,
                            pServerEvent->eventData.charCccdWrittenEvent.newCccd);
    }
    break;
        
    case gEvtAttributeWritten_c:
    {
        BleApp_AttributeWritten (deviceId,
                                 pServerEvent->eventData.attributeWrittenEvent.handle,
                                 pServerEvent->eventData.attributeWrittenEvent.cValueLength,
                                 pServerEvent->eventData.attributeWrittenEvent.aValue);
    }
    break;
        
    case gEvtAttributeWrittenWithoutResponse_c:
    {
        BleApp_AttributeWrittenWithoutResponse (deviceId,
                                                pServerEvent->eventData.attributeWrittenEvent.handle,
                                                pServerEvent->eventData.attributeWrittenEvent.cValueLength,
                                                pServerEvent->eventData.attributeWrittenEvent.aValue);
    }
    break;
        
    case gEvtHandleValueConfirmation_c:
    {
        BleApp_HandleValueConfirmation (deviceId);
    }
    break;
        
    default:
    break;
    }
}

static void BleApp_L2capPsmDataCallback (deviceId_t     deviceId,
                                         uint16_t       lePsm,
                                         uint8_t*       pPacket,
                                         uint16_t       packetLength)
{
    OtapClient_HandleDataChunk (deviceId,
                                packetLength,
                                pPacket);
}

static void BleApp_L2capPsmControlCallback(l2capControlMessageType_t    messageType,
                                           void*                        pMessage)
{
    switch (messageType)
    {
        case gL2ca_LePsmConnectRequest_c:
        {
            l2caLeCbConnectionRequest_t *pConnReq = (l2caLeCbConnectionRequest_t *)pMessage;
            
            /* This message is unexpected on the OTAP Client, the OTAP Client sends L2CAP PSM connection
             * requests and expects L2CAP PSM connection responses.
             * Disconnect the peer. */
            Gap_Disconnect (pConnReq->deviceId);
            
            break;
        }
        case gL2ca_LePsmConnectionComplete_c:
        {
            l2caLeCbConnectionComplete_t *pConnComplete = (l2caLeCbConnectionComplete_t *)pMessage;
                
            /* Call the application PSM connection complete handler. */
            OtapClient_HandlePsmConnectionComplete (pConnComplete);
            
            break;
        }
        case gL2ca_LePsmDisconnectNotification_c:
        {
            l2caLeCbDisconnection_t *pCbDisconnect = (l2caLeCbDisconnection_t *)pMessage;

            /* Call the application PSM disconnection handler. */
            OtapClient_HandlePsmDisconnection (pCbDisconnect);
            
            break;
        }
        case gL2ca_NoPeerCredits_c:
        {
            l2caLeCbNoPeerCredits_t *pCbNoPeerCredits = (l2caLeCbNoPeerCredits_t *)pMessage;
            L2ca_SendLeCredit (pCbNoPeerCredits->deviceId,
                               otapClientData.l2capPsmChannelId,
                               mAppLeCbInitialCredits_c);
            break;
        }
        case gL2ca_LocalCreditsNotification_c:
        {
            //l2caLeCbLocalCreditsNotification_t *pMsg = (l2caLeCbLocalCreditsNotification_t *)pMessage;

            break;
        }
        default:
            break;
    }
}


static void BleApp_CccdWritten (deviceId_t deviceId, uint16_t handle, gattCccdFlags_t cccd)
{
    otapCommand_t otapCommand;
    bleResult_t   bleResult;
    
    /*! Check if the OTAP control point CCCD was written. */
    if (handle == cccd_otap_control_point)
    {
        otapClientData.serverWrittenCccd = TRUE;
        switch (otapClientData.state)
        {
        case mOtapClientStateDownloadingImage_c:
        case mOtapClientStateIdle_c:
            /*! If the state is Idle try to send a New Image Info Request Command to the OTAP Server. */
            otapCommand.cmdId = gOtapCmdIdNewImageInfoRequest_c;
            FLib_MemCpy (otapCommand.cmd.newImgInfoReq.currentImageId,
                         (uint8_t*)otapClientData.currentImgId,
                         gOtap_ImageIdFieldSize_c);
            FLib_MemCpy (otapCommand.cmd.newImgInfoReq.currentImageVersion,
                         (uint8_t*)otapClientData.currentImgVer,
                         gOtap_ImageVersionFieldSize_c);
            
            bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                        (void*)(&otapCommand),
                                                        cmdIdToCmdLengthTable[gOtapCmdIdNewImageInfoRequest_c]);
            if (gBleSuccess_c == bleResult)
            {
                otapClientData.lastCmdSentToOtapServer = gOtapCmdIdNewImageInfoRequest_c;
            }
            else
            {
                OtapClient_HandleServerCommError (deviceId);
            }
            break;
            
        case mOtapClientStateImageDownloadComplete_c:
            /*! Simply ignore the situation if the image download is complete. */
            break;
            
        default:
            /*! Ignore. */
            break;
        }
    }
}

static void BleApp_AttributeWritten(deviceId_t  deviceId,
                                    uint16_t    handle,
                                    uint16_t    length,
                                    uint8_t*    pValue)
{
    bleResult_t bleResult;
    otapCommand_t otapCommand;
    
    /* Only the OTAP Control Point attribute is expected to be written using the
     * ATT Write Command. */
    if (handle == value_otap_control_point)
    {
        /*! Handle all OTAP Server to Client Commands Here. */
        switch(((otapCommand_t*)pValue)->cmdId)
        {
        case gOtapCmdIdNewImageNotification_c:
            bleResult = GattServer_SendAttributeWrittenStatus (deviceId,
                                                               value_otap_control_point,
                                                               gAttErrCodeNoError_c);
            if (gBleSuccess_c == bleResult)
            {
                OtapClient_HandleNewImageNotification (deviceId,
                                                       length,
                                                       pValue);
            }
            else
            {
                /*! A BLE error has occurred - Disconnect */
                Gap_Disconnect (deviceId);
            }
            break;
        case gOtapCmdIdNewImageInfoResponse_c:
            bleResult = GattServer_SendAttributeWrittenStatus (deviceId,
                                                               value_otap_control_point,
                                                               gAttErrCodeNoError_c);
            if (gBleSuccess_c == bleResult)
            {
                OtapClient_HandleNewImageInfoResponse (deviceId,
                                                       length,
                                                       pValue);
            }
            else
            {
                /*! A BLE error has occurred - Disconnect */
                Gap_Disconnect (deviceId);
            }
            break;
        case gOtapCmdIdErrorNotification_c:
            bleResult = GattServer_SendAttributeWrittenStatus (deviceId,
                                                               value_otap_control_point,
                                                               gAttErrCodeNoError_c);
            if (gBleSuccess_c == bleResult)
            {
                OtapClient_HandleErrorNotification (deviceId,
                                                    length,
                                                    pValue);
            }
            else
            {
                /*! A BLE error has occurred - Disconnect */
                Gap_Disconnect (deviceId);
            }
            break;
            
        default:
            otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
            otapCommand.cmd.errNotif.cmdId = pValue[0];
            otapCommand.cmd.errNotif.errStatus = gOtapStatusUnexpectedCommand_c;
    
            bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                        (void*)(&otapCommand),
                                                        cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
            if (gBleSuccess_c == bleResult)
            {
                otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
            }
            else
            {
                OtapClient_HandleServerCommError (deviceId);
            }
            break;
        };
    }
    else
    {
        /*! A GATT Server is trying to GATT Write an unknown attribute value.
         *  This should not happen. Disconnect the link. */
        Gap_Disconnect (deviceId);
    }
}

static void BleApp_AttributeWrittenWithoutResponse (deviceId_t deviceId,
                                                    uint16_t handle,
                                                    uint16_t length,
                                                    uint8_t* pValue)
{
    otapCommand_t otapCommand;
    otapStatus_t otapStatus = gOtapStatusSuccess_c;
    bleResult_t bleResult;
    
    /* Only the OTAP Data attribute is expected to be written using the
     * ATT Write Without Response Command. */
    if (handle == value_otap_data)
    {
        if (otapClientData.state == mOtapClientStateDownloadingImage_c)
        {
            if (otapClientData.transferMethod == gOtapTransferMethodAtt_c)
            {
                if (((otapCommand_t*)pValue)->cmdId == gOtapCmdIdImageChunk_c)
                {
                    OtapClient_HandleDataChunk (deviceId,
                                                length,
                                                pValue);
                }
                else
                {
                    /* If the OTAP Client received an unexpected command on the data channel send an error to the OTAP Server. */
                    otapStatus = gOtapStatusUnexpectedCmdOnDataChannel_c;
                }
            }
            else
            {
                /* If the OTAP Client is not expecting image file chunks via ATT send an error to the OTAP Server. */
                otapStatus = gOtapStatusUnexpectedTransferMethod_c;
            }
        }
        else
        {
            /* If the OTAP Client is not expecting image file chunks send an error to the OTAP Server. */
            otapStatus = gOtapStatusImageDataNotExpected_c;
        }
        
        if (otapStatus != gOtapStatusSuccess_c)
        {
            otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
            otapCommand.cmd.errNotif.cmdId = pValue[0];
            otapCommand.cmd.errNotif.errStatus = otapStatus;
    
            bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                        (void*)(&otapCommand),
                                                        cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
            if (gBleSuccess_c == bleResult)
            {
                otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
            }
            else
            {
                OtapClient_HandleServerCommError (deviceId);
            }
        }
    }
}

static void BleApp_HandleValueConfirmation (deviceId_t deviceId)
{
    otapCommand_t otapCommand;
    bleResult_t   bleResult;
    
    /*! Check for which command sent to the OTAP Server the confirmation has been received. */
    switch (otapClientData.lastCmdSentToOtapServer)
    {
    case gOtapCmdIdNewImageInfoRequest_c:
        OtapClient_HandleNewImageInfoRequestConfirmation (deviceId);
        break;
        
    case gOtapCmdIdImageBlockRequest_c:
        OtapClient_HandleImageBlockRequestConfirmation (deviceId);
        break;
        
    case gOtapCmdIdImageTransferComplete_c:
        OtapClient_HandleImageTransferCompleteConfirmation (deviceId);
        break;
        
    case gOtapCmdIdErrorNotification_c:
        OtapClient_HandleErrorNotificationConfirmation (deviceId);
        break;
        
    case gOtapCmdIdStopImageTransfer_c:
        OtapClient_HandleStopImageTransferConfirmation (deviceId);
        break;
        
    default:
        otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
        otapCommand.cmd.errNotif.cmdId = gOtapCmdIdNoCommand_c;
        otapCommand.cmd.errNotif.errStatus = gOtapStatusUnexpectedCommand_c;

        bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                    (void*)(&otapCommand),
                                                    cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
        if (gBleSuccess_c == bleResult)
        {
            otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
        }
        else
        {
            OtapClient_HandleServerCommError (deviceId);
        }
        break;
    };
}

static void OtapClient_HandlePsmConnectionComplete (l2caLeCbConnectionComplete_t *pConnComplete)
{
    if (pConnComplete->result == gSuccessful_c)
    {
        otapClientData.l2capPsmConnected = TRUE;
        otapClientData.l2capPsmChannelId = pConnComplete->cId;
        
        if (pConnComplete->peerMtu > gOtap_l2capCmdMtuDataChunkOverhead_c)
        {
            otapClientData.negotiatedMaxL2CapChunkSize = pConnComplete->peerMtu - gOtap_l2capCmdMtuDataChunkOverhead_c;
        }
        
        /* If the connection is successful reset the image download parameters to safe values and
         * try to continue the image download. */
        otapClientData.chunkSize = 0;
        otapClientData.chunkSeqNum = 0;
        otapClientData.totalBlockChunks = 0;
        otapClientData.totalBlockSize = 0;
        OtapClient_ContinueImageDownload (pConnComplete->deviceId);
    }
    else
    {
        otapClientData.l2capPsmConnected = FALSE;
        /* If the connection failed try to reconnect. */
        L2ca_ConnectLePsm (gOtap_L2capLePsm_c,
                           pConnComplete->deviceId,
                           mAppLeCbInitialCredits_c);
    }
}

static void OtapClient_HandlePsmDisconnection (l2caLeCbDisconnection_t * pCbDisconnect)
{
    otapCommand_t otapCommand;
    bleResult_t   bleResult = gBleSuccess_c;
    
    otapClientData.l2capPsmConnected = FALSE;
    
    /* Stop the image transfer if a downlaod is in progress
     * and try to reconnect. */
    if ((otapClientData.state == mOtapClientStateDownloadingImage_c) &&
        (otapClientData.transferMethod == gOtapTransferMethodL2capCoC_c))
    {
        otapCommand.cmdId = gOtapCmdIdStopImageTransfer_c;
        FLib_MemCpy (otapCommand.cmd.stopImgTransf.imageId,
                     (uint8_t*)otapClientData.imgId,
                     gOtap_ImageIdFieldSize_c);

        bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                    (void*)(&otapCommand),
                                                    cmdIdToCmdLengthTable[gOtapCmdIdStopImageTransfer_c]);
        if (gBleSuccess_c == bleResult)
        {
            otapClientData.lastCmdSentToOtapServer = gOtapCmdIdStopImageTransfer_c;
        }
        else
        {
            /*! A BLE error has occurred - Disconnect */
            Gap_Disconnect (pCbDisconnect->deviceId);
        }
    }
    
    if (gBleSuccess_c == bleResult)
    {
        L2ca_ConnectLePsm (gOtap_L2capLePsm_c,
                           pCbDisconnect->deviceId,
                           mAppLeCbInitialCredits_c);
    }
}

static void OtapClient_HandleDataChunk (deviceId_t deviceId, uint16_t length, uint8_t* pData)
{
    otapCommand_t otapCommand;
    bleResult_t   bleResult;
    otapStatus_t otapStatus = gOtapStatusSuccess_c;
    
    otapCmdImgChunkCoc_t* pDataChunk = (otapCmdImgChunkCoc_t*)(&((otapCommand_t*)pData)->cmd); //use the CoC Data Chunk type but observe the length
    uint16_t dataLen = length - gOtap_CmdIdFieldSize_c - gOtap_ChunkSeqNumberSize_c; // len
    
    /* Variables for the local image file parsing state machine. */
    static uint32_t currentImgElemRcvdLen = 0; /*!< Contains the number of received bytes for th current image element (header or othe sub element).
                                                         *   This is needed because the */
    static bleOtaImageFileHeader_t imgFileHeader;   /*!< Saved image file header. */
    static uint32_t elementEnd = 0;                 /*!< Current image file element expected end. */
    static subElementHeader_t subElemHdr;
    
    if (deviceId == otapClientData.peerOtapServer)
    {
        /* Check if the command length is as expected. */
        if ((length > (gOtap_CmdIdFieldSize_c + gOtap_ChunkSeqNumberSize_c)) &&
            (((otapClientData.transferMethod == gOtapTransferMethodAtt_c) && (length <= gOtapCmdImageChunkAttMaxLength_c)) ||
             ((otapClientData.transferMethod == gOtapTransferMethodL2capCoC_c) && (length <= gOtapCmdImageChunkCocMaxLength_c))
            )
           )
        {
            /* Check if the chunk (sequence number) is as expected */
            if ((pDataChunk->seqNumber == otapClientData.chunkSeqNum) &&
                (pDataChunk->seqNumber < otapClientData.totalBlockChunks))
            {
                /*  Check if the data length is as expected. */
                if (((dataLen == otapClientData.chunkSize) && ((pDataChunk->seqNumber < (otapClientData.totalBlockChunks - 1)) || (otapClientData.totalBlockSize % otapClientData.chunkSize == 0))) ||
                    ((dataLen < otapClientData.chunkSize) && (pDataChunk->seqNumber == (otapClientData.totalBlockChunks - 1)) && (dataLen == otapClientData.totalBlockSize % otapClientData.chunkSize))
                   )
                {
                    /* Do more checks here if necessary. */
                }
                else
                {
                    otapStatus = gOtapStatusUnexpectedDataLength_c;
                }
            }
            else
            {
                otapStatus = gOtapStatusUnexpectedSequenceNumber_c;
            }
        }
        else
        {
            otapStatus = gOtapStatusInvalidCommandLength_c;
        }
    }
    else
    {
        otapStatus = gOtapStatusUnexpectedOtapPeer_c;
    }

    /*! If all checks were successful then parse the current data chunk, else send an error notification. */
    if (otapStatus == gOtapStatusSuccess_c)
    {
        pData = (uint8_t*)(&pDataChunk->data);
        
        /* If the Current position is 0 then reset the received length for the current image element
         * and the current image CRC to the initialization value which is 0.
         * The current position should be 0 only at the start of the image file transfer. */
        if (otapClientData.currentPos == 0)
        {
            currentImgElemRcvdLen = 0; 
            otapClientData.imgComputedCrc = 0;
        }
        
        /* Parse all the bytes in the data payload. */
        while (dataLen)
        {
            /* Wait for the header to arrive and check it's contents
             * then handle the elements of the image. */
            if (otapClientData.currentPos < sizeof(bleOtaImageFileHeader_t))
            {
                if ((otapClientData.currentPos + dataLen) >= sizeof(bleOtaImageFileHeader_t))
                {
                    uint16_t residualHeaderLen = sizeof(bleOtaImageFileHeader_t) - otapClientData.currentPos;
                    
                    /* There is enough information in the data payload to complete the header. */
                    FLib_MemCpy ((uint8_t*)(&imgFileHeader) + otapClientData.currentPos, pData, residualHeaderLen);
                    otapClientData.currentPos += residualHeaderLen;
                    pData += residualHeaderLen;
                    dataLen -= residualHeaderLen;
                    
                    /* Check header contents, and if it is not valid return and error and reset the image download position. */
                    otapStatus = OtapClient_IsImageFileHeaderValid (&imgFileHeader);
                    if (otapStatus != gOtapStatusSuccess_c)
                    {
                        otapClientData.currentPos = 0;
                        break;
                    }
                    
                    /* If the header is valid then update the CRC over the header part of the image. */
                    otapClientData.imgComputedCrc = OTA_CrcCompute ((uint8_t*)(&imgFileHeader),
                                                                    sizeof(bleOtaImageFileHeader_t),
                                                                    otapClientData.imgComputedCrc);
                    
                    currentImgElemRcvdLen = 0;
                    
                    /* If the remaining data length is not 0 then the loop will continue with the parsing of the next element. */
                }
                else
                {
                    /* Not enough data to complete the header.
                     * Copy all the data into the temporary header and
                     * increment the current image position. */
                    FLib_MemCpy((uint8_t*)(&imgFileHeader) + otapClientData.currentPos, pData, dataLen);
                    otapClientData.currentPos += dataLen;
                    dataLen = 0;
                }
            }
            else
            {
                /* The parsing has reached the sub-elements portion of the image. 
                 * Wait for each sub-element tag to arrive or parse it if it is known. */
                if (currentImgElemRcvdLen < sizeof(subElementHeader_t))
                {
                    if ((currentImgElemRcvdLen + dataLen) >= sizeof(subElementHeader_t))
                    {
                        uint16_t residualSubElemHdrLen = sizeof(subElementHeader_t) - currentImgElemRcvdLen;
                        
                        /* There is enough information in the data payload to complete the sub-element header. */
                        FLib_MemCpy ((uint8_t*)(&subElemHdr) + currentImgElemRcvdLen, pData, residualSubElemHdrLen);
                        otapClientData.currentPos += residualSubElemHdrLen;
                        currentImgElemRcvdLen += residualSubElemHdrLen;
                        pData += residualSubElemHdrLen;
                        dataLen -= residualSubElemHdrLen;
                        
                        /* Update the CRC over the sub-element header only if it is not the CRC Sub-Element header. */
                        if (subElemHdr.tagId != gBleOtaSubElemTagIdImageFileCrc_c)
                        {
                            otapClientData.imgComputedCrc = OTA_CrcCompute ((uint8_t*)(&subElemHdr),
                                                                            sizeof(subElementHeader_t),
                                                                            otapClientData.imgComputedCrc);
                        }
                        
                        elementEnd = otapClientData.currentPos + subElemHdr.dataLen;
                        
                        /* If the remaining data length is not 0 then the loop will
                        continue with the parsing of the sub-element. */
                    }
                    else
                    {
                        /* Not enough data to complete the sub-element header.
                         * Copy all the data into the temporary sub-element header
                         * and increment the current image position. */
                        FLib_MemCpy ((uint8_t*)(&subElemHdr) + currentImgElemRcvdLen, pData, dataLen);
                        otapClientData.currentPos += dataLen;
                        currentImgElemRcvdLen += dataLen;
                        dataLen = 0;
                    }
                }
                else
                {
                    uint32_t    elementChunkLength = 0;
                    
                    /* Make sure we do not pass the current element boundary. */
                    if ((otapClientData.currentPos + dataLen) >= elementEnd)
                    {
                        elementChunkLength = elementEnd - otapClientData.currentPos;
                    }
                    else
                    {
                        elementChunkLength = dataLen;
                    }
                    
                    /* Handle sub-element payload. */
                    switch (subElemHdr.tagId)
                    {
                    case gBleOtaSubElemTagIdUpgradeImage_c:
                        /* Immediately after receiving the header check if the image sub-element length is valid
                         * by trying to start the image upgrade procedure. */
                        if (currentImgElemRcvdLen == sizeof(subElementHeader_t))
                        {
                            if (gOtaSucess_c != OTA_StartImage(subElemHdr.dataLen))
                            {
                                /* The sub-element length is invalid, set an error status and reset
                                 * the image file download process. */
                                otapStatus = gOtapStatusImageSizeTooLarge_c;
                                otapClientData.currentPos = 0;
                                break;
                            }
                        }
                        
                        /* Upgrade Image Tag - compute the CRC and try to push the chunk to the storage. */
                        otapClientData.imgComputedCrc = OTA_CrcCompute (pData,
                                                                        elementChunkLength,
                                                                        otapClientData.imgComputedCrc);
                        if (gOtaSucess_c != OTA_PushImageChunk (pData, elementChunkLength, NULL, NULL))
                        {
                            otapStatus = gOtapStatusImageStorageError_c;
                            otapClientData.currentPos = 0;
                            OTA_CancelImage();
                            break;
                        }
                        break;
                        
                    case gBleOtaSubElemTagIdSectorBitmap_c:
                        /* Immediately after receiving the header check if the sub-element length is valid. */
                        if (currentImgElemRcvdLen == sizeof(subElementHeader_t))
                        {
                            if (subElemHdr.dataLen != sizeof(otapClientData.imgSectorBitmap))
                            {
                                /* The sub-element length is invalid, set an error status and reset
                                 * the image file download process. */
                                otapStatus = gOtapStatusInvalidSubElementLength_c;
                                otapClientData.currentPos = 0;
                                OTA_CancelImage();
                                break;
                            }
                        }
                        
                        /* Sector Bitmap Tag - Compute the CRC and copy the received bitmap to the buffer. */
                        otapClientData.imgComputedCrc = OTA_CrcCompute (pData,
                                                                        elementChunkLength,
                                                                        otapClientData.imgComputedCrc);
                        
                        FLib_MemCpy ((uint8_t*)otapClientData.imgSectorBitmap + (currentImgElemRcvdLen - sizeof(subElementHeader_t)),
                                     pData,
                                     elementChunkLength);
                        break;
                        
                    case gBleOtaSubElemTagIdImageFileCrc_c:
                        /* Immediately after receiving the header check if the sub-element length is valid. */
                        if (currentImgElemRcvdLen == sizeof(subElementHeader_t))
                        {
                            if (subElemHdr.dataLen != sizeof(otapClientData.imgReceivedCrc))
                            {
                                /* The sub-element length is invalid, set an error status and reset
                                 * the image file download process. */
                                otapStatus = gOtapStatusInvalidSubElementLength_c;
                                otapClientData.currentPos = 0;
                                OTA_CancelImage();
                                break;
                            }
                        }
                        
                        /* CRC Tag - Just copy the received CRC to the buffer. */
                        FLib_MemCpy ((uint8_t*)(&otapClientData.imgReceivedCrc) + (currentImgElemRcvdLen - sizeof(subElementHeader_t)),
                                     pData,
                                     elementChunkLength);
                        break;
                        
                    default:
                        /* Unknown sub-element type, just compute the CRC over it. */
                        otapClientData.imgComputedCrc = OTA_CrcCompute (pData,
                                                                        elementChunkLength,
                                                                        otapClientData.imgComputedCrc);
                        break;
                    };
                    
                    if (otapStatus != gOtapStatusSuccess_c)
                    {
                        /* If an error has occurred then break the loop. */
                        break;
                    }
                    
                    otapClientData.currentPos += elementChunkLength;
                    currentImgElemRcvdLen += elementChunkLength;
                    pData += elementChunkLength;
                    dataLen -= elementChunkLength;
                    
                    /* If this element has been completely received then reset the current element
                     * received length to trigger the reception of the next sub-element. */
                    if (otapClientData.currentPos >= elementEnd)
                    {
                        currentImgElemRcvdLen = 0;
                    }
                }
            }
        } /* while (dataLen) */
    }
    
    if (otapStatus == gOtapStatusSuccess_c)
    {
        /* If the chunk has been successfully processed increase the expected sequence number. */
        otapClientData.chunkSeqNum += 1;
        
        /* Check if the block and/or image transfer is complete */
        if (otapClientData.chunkSeqNum >= otapClientData.totalBlockChunks)
        {
            /* If the image transfer is complete check the image CRC then
             * commit the image and set the bootloader flags. */
            if (otapClientData.currentPos >= otapClientData.imgSize)
            {
                if (otapClientData.imgComputedCrc != otapClientData.imgReceivedCrc)
                {
                    otapStatus = gOtapStatusInvalidImageCrc_c;
                    otapClientData.currentPos = 0;
                    OTA_CancelImage();
                }
                else if (gOtaSucess_c != OTA_CommitImage(otapClientData.imgSectorBitmap))
                {
                    otapStatus = gOtapStatusImageStorageError_c;
                    otapClientData.currentPos = 0;
                    OTA_CancelImage();
                }
                else
                {
                    /* The new image was successfully committed, set the bootloader new image flags,
                     * set the image transfer state as downloaded and send an image transfer complete
                     * message to the peer. */
                    OTA_SetNewImageFlag ();
                    otapClientData.state = mOtapClientStateImageDownloadComplete_c;
                    
                    otapCommand.cmdId = gOtapCmdIdImageTransferComplete_c;
                    FLib_MemCpy((uint8_t*)otapCommand.cmd.imgTransComplete.imageId, otapClientData.imgId, sizeof(otapCommand.cmd.imgTransComplete.imageId));
                    otapCommand.cmd.imgTransComplete.status = gOtapStatusSuccess_c;
                    
                    bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                                (void*)(&otapCommand),
                                                                cmdIdToCmdLengthTable[gOtapCmdIdImageTransferComplete_c]);
                    if (gBleSuccess_c == bleResult)
                    {
                        otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
                    }
                    else
                    {
                        /*! A BLE error has occurred - Trigger the bootloader and reset now.
                         *  Do not wait for the Image Transfer Complete Confirmation. */
                        OtapClient_HandleServerCommError (deviceId);
                        OTA_SetNewImageFlag ();
                        ResetMCU ();
                    }
                }
            }
            else
            {
                /* If just the current block is complete ask for another block. */
                OtapClient_ContinueImageDownload (deviceId);
            }
        }
    }
    
    if (otapStatus != gOtapStatusSuccess_c)
    {
        otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
        otapCommand.cmd.errNotif.cmdId = gOtapCmdIdImageChunk_c;
        otapCommand.cmd.errNotif.errStatus = otapStatus;

        bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                    (void*)(&otapCommand),
                                                    cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
        
        if (gBleSuccess_c == bleResult)
        {
            otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
        }
        else
        {
            OtapClient_HandleServerCommError (deviceId);
        }
    }
}

static void OtapClient_HandleNewImageNotification (deviceId_t deviceId, uint16_t length, uint8_t* pValue)
{
    otapCommand_t otapCommand;
    bleResult_t   bleResult;
    otapStatus_t otapStatus = gOtapStatusSuccess_c;
    otapCommand_t*  pRemoteCmd = (otapCommand_t*)pValue;
    
    /* Check the command length and parameters. */
    if (length != cmdIdToCmdLengthTable[gOtapCmdIdNewImageNotification_c])
    {
        otapStatus = gOtapStatusInvalidCommandLength_c;
    }
    else if (pRemoteCmd->cmd.newImgNotif.imageFileSize <= (sizeof(bleOtaImageFileHeader_t) + sizeof(subElementHeader_t)))
    {
        otapStatus = gOtapStatusInvalidImageFileSize_c;
    }
    else
    {
        switch (otapClientData.state)
        {
        case mOtapClientStateIdle_c:
            if (OtapClient_IsRemoteImageNewer(pRemoteCmd->cmd.newImgNotif.imageId, pRemoteCmd->cmd.newImgNotif.imageVersion))
            {
                /* If a response for a New Image Info Request is expected from the OTAP Server simply ignore the
                 * New Image Notification. */
                if (otapClientData.lastCmdSentToOtapServer != gOtapCmdIdNewImageInfoRequest_c)
                {
                    /* Set up the Client to receive the image file. */
                    otapClientData.peerOtapServer = deviceId;
                    FLib_MemCpy(otapClientData.imgId, pRemoteCmd->cmd.newImgNotif.imageId, gOtap_ImageIdFieldSize_c);
                    FLib_MemCpy(otapClientData.imgVer, pRemoteCmd->cmd.newImgNotif.imageVersion, gOtap_ImageVersionFieldSize_c);
                    otapClientData.imgSize = pRemoteCmd->cmd.newImgNotif.imageFileSize;
                    otapClientData.currentPos = 0;
                    OtapClient_ResetBlockTransferParameters (deviceId);
                    
                    /* Change the Client state to Downloading and trigger the download. */
                    otapClientData.state = mOtapClientStateDownloadingImage_c;
                    OtapClient_ContinueImageDownload (deviceId);
                }
            }
            /* If the remote image is not newer than the current image simply ignore the New Image Notification */
            break;
     
        case mOtapClientStateDownloadingImage_c:
            /*! Check if the image is the one currently being downloaded and if it is continue the download,
             *  else if the image is newer than the current one being downloaded then restart the whole download process. */
            if ((FLib_MemCmp(otapClientData.imgId, pRemoteCmd->cmd.newImgNotif.imageId, gOtap_ImageIdFieldSize_c)) &&
                (FLib_MemCmp(otapClientData.imgVer, pRemoteCmd->cmd.newImgNotif.imageVersion, gOtap_ImageVersionFieldSize_c))
               )
            {
                OtapClient_ResetBlockTransferParameters (deviceId);
                OtapClient_ContinueImageDownload (deviceId);
            }
            else if (OtapClient_IsRemoteImageNewer(pRemoteCmd->cmd.newImgNotif.imageId, pRemoteCmd->cmd.newImgNotif.imageVersion))
            {
                /*! A newer image than the one being donloaded is available, restart the download with the new image. */
                otapClientData.peerOtapServer = deviceId;
                FLib_MemCpy(otapClientData.imgId, pRemoteCmd->cmd.newImgNotif.imageId, gOtap_ImageIdFieldSize_c);
                FLib_MemCpy(otapClientData.imgVer, pRemoteCmd->cmd.newImgNotif.imageVersion, gOtap_ImageVersionFieldSize_c);
                otapClientData.imgSize = pRemoteCmd->cmd.newImgNotif.imageFileSize;
                otapClientData.currentPos = 0;
                OtapClient_ResetBlockTransferParameters (deviceId);
                
                OtapClient_ContinueImageDownload (deviceId);
            }
            break;
        case mOtapClientStateImageDownloadComplete_c:
            /* Simply ignore the message if an image is being downloaded or
             * an image download is complete. */
            break;
            
        default:
            /* Some kind of internal error has occurred. Reset the
             * client state to Idle and act as if the state was Idle. */
            otapClientData.state = mOtapClientStateIdle_c;
            if (OtapClient_IsRemoteImageNewer(pRemoteCmd->cmd.newImgNotif.imageId, pRemoteCmd->cmd.newImgNotif.imageVersion))
            {
                /* If a response for a New Image Info Request is expected from the OTAp Server simply ignore the
                 * New Image Notification. */
                if (otapClientData.lastCmdSentToOtapServer != gOtapCmdIdNewImageInfoRequest_c)
                {
                    /* Set up the Client to receive the image file. */
                    otapClientData.peerOtapServer = deviceId;
                    FLib_MemCpy(otapClientData.imgId, pRemoteCmd->cmd.newImgNotif.imageId, gOtap_ImageIdFieldSize_c);
                    FLib_MemCpy(otapClientData.imgVer, pRemoteCmd->cmd.newImgNotif.imageVersion, gOtap_ImageVersionFieldSize_c);
                    otapClientData.imgSize = pRemoteCmd->cmd.newImgNotif.imageFileSize;
                    otapClientData.currentPos = 0;
                    OtapClient_ResetBlockTransferParameters (deviceId);
                    
                    /* Change the Client state to Downloading and trigger the download. */
                    otapClientData.state = mOtapClientStateDownloadingImage_c;
                    OtapClient_ContinueImageDownload (deviceId);
                }
            }
            /* If the remote image is not newer than the current image simply ignore the New Image Notification */
            break;
        };
    }
        
    if (otapStatus != gOtapStatusSuccess_c)
    {
        otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
        otapCommand.cmd.errNotif.cmdId = pValue[0];
        otapCommand.cmd.errNotif.errStatus = otapStatus;

        bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                    (void*)(&otapCommand),
                                                    cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
        if (gBleSuccess_c == bleResult)
        {
            otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
        }
        else
        {
            OtapClient_HandleServerCommError (deviceId);
        }
    }
}

static void OtapClient_HandleNewImageInfoResponse (deviceId_t deviceId, uint16_t length, uint8_t* pValue)
{
    otapCommand_t otapCommand;
    bleResult_t   bleResult;
    otapStatus_t otapStatus = gOtapStatusSuccess_c;
    otapCommand_t*  pRemoteCmd = (otapCommand_t*)pValue;
    
    /* Check the command length and parameters. */
    if (length != cmdIdToCmdLengthTable[gOtapCmdIdNewImageInfoResponse_c])
    {
        otapStatus = gOtapStatusInvalidCommandLength_c;
    }
    else if (pRemoteCmd->cmd.newImgInfoRes.imageFileSize <= (sizeof(bleOtaImageFileHeader_t) + sizeof(subElementHeader_t)))
    {
        otapStatus = gOtapStatusInvalidImageFileSize_c;
    }
    else
    {
        switch (otapClientData.state)
        {
        case mOtapClientStateIdle_c:
            if (OtapClient_IsRemoteImageNewer(pRemoteCmd->cmd.newImgInfoRes.imageId, pRemoteCmd->cmd.newImgInfoRes.imageVersion))
            {
                /* Set up the Client to receive the image file. */
                otapClientData.peerOtapServer = deviceId;
                FLib_MemCpy(otapClientData.imgId, pRemoteCmd->cmd.newImgInfoRes.imageId, gOtap_ImageIdFieldSize_c);
                FLib_MemCpy(otapClientData.imgVer, pRemoteCmd->cmd.newImgInfoRes.imageVersion, gOtap_ImageVersionFieldSize_c);
                otapClientData.imgSize = pRemoteCmd->cmd.newImgInfoRes.imageFileSize;
                otapClientData.currentPos = 0;
                OtapClient_ResetBlockTransferParameters (deviceId);
                
                /* Change the Client state to Downloading and trigger the download. */
                otapClientData.state = mOtapClientStateDownloadingImage_c;
                OtapClient_ContinueImageDownload (deviceId);
            }
            /* If the remote image is not newer than the current image simply ignore the New Image Info Response */
            break;
     
        case mOtapClientStateDownloadingImage_c:
            /*! Check if the image is the one currently being downloaded and if it is continue the download,
             *  else if the image is newer than the current one being downloaded then restart the whole download process. */
            if ((FLib_MemCmp(otapClientData.imgId, pRemoteCmd->cmd.newImgNotif.imageId, gOtap_ImageIdFieldSize_c)) &&
                (FLib_MemCmp(otapClientData.imgVer, pRemoteCmd->cmd.newImgNotif.imageVersion, gOtap_ImageVersionFieldSize_c))
               )
            {
                OtapClient_ResetBlockTransferParameters (deviceId);
                OtapClient_ContinueImageDownload (deviceId);
            }
            else if (OtapClient_IsRemoteImageNewer(pRemoteCmd->cmd.newImgNotif.imageId, pRemoteCmd->cmd.newImgNotif.imageVersion))
            {
                /*! A newer image than the one being donloaded is available, restart the download with the new image. */
                otapClientData.peerOtapServer = deviceId;
                FLib_MemCpy(otapClientData.imgId, pRemoteCmd->cmd.newImgNotif.imageId, gOtap_ImageIdFieldSize_c);
                FLib_MemCpy(otapClientData.imgVer, pRemoteCmd->cmd.newImgNotif.imageVersion, gOtap_ImageVersionFieldSize_c);
                otapClientData.imgSize = pRemoteCmd->cmd.newImgNotif.imageFileSize;
                otapClientData.currentPos = 0;
                OtapClient_ResetBlockTransferParameters (deviceId);
                
                OtapClient_ContinueImageDownload (deviceId);
            }
            break;
                 
        case mOtapClientStateImageDownloadComplete_c:
            /* Simply ignore the message if an image is being downloaded or
             * an image download is complete. */
            break;
            
        default:
            /* Some kind of internal error has occurred. Reset the
             * client state to Idle and act as if the state was Idle. */
            otapClientData.state = mOtapClientStateIdle_c;
            if (OtapClient_IsRemoteImageNewer(pRemoteCmd->cmd.newImgInfoRes.imageId, pRemoteCmd->cmd.newImgInfoRes.imageVersion))
            {
                /* Set up the Client to receive the image file. */
                otapClientData.peerOtapServer = deviceId;
                FLib_MemCpy(otapClientData.imgId, pRemoteCmd->cmd.newImgInfoRes.imageId, gOtap_ImageIdFieldSize_c);
                FLib_MemCpy(otapClientData.imgVer, pRemoteCmd->cmd.newImgInfoRes.imageVersion, gOtap_ImageVersionFieldSize_c);
                otapClientData.imgSize = pRemoteCmd->cmd.newImgInfoRes.imageFileSize;
                otapClientData.currentPos = 0;
                OtapClient_ResetBlockTransferParameters (deviceId);
                
                /* Change the Client state to Downloading and trigger the download. */
                otapClientData.state = mOtapClientStateDownloadingImage_c;
                OtapClient_ContinueImageDownload (deviceId);
            }
            /* If the remote image is not newer than the current image simply ignore the New Image Info Response */
            break;
        };
    }
        
    if (otapStatus != gOtapStatusSuccess_c)
    {
        otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
        otapCommand.cmd.errNotif.cmdId = gOtapCmdIdNewImageInfoResponse_c;
        otapCommand.cmd.errNotif.errStatus = otapStatus;

        bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                    (void*)(&otapCommand),
                                                    cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
        if (gBleSuccess_c == bleResult)
        {
            otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
        }
        else
        {
            OtapClient_HandleServerCommError (deviceId);
        }
    }
}

static void OtapClient_HandleErrorNotification (deviceId_t deviceId, uint16_t length, uint8_t* pValue)
{
    otapCommand_t otapCommand;
    bleResult_t   bleResult;
    otapStatus_t otapStatus = gOtapStatusSuccess_c;
    otapCommand_t*  pRemoteCmd = (otapCommand_t*)pValue;
    
    /* Check the command length and parameters. */
    if (length == cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c])
    {
        /*! Handle remote error statuses here. */
        if (pRemoteCmd->cmd.errNotif.errStatus < gOtapNumberOfStatuses_c)
        {
            /* Handle all errors in the same way, disconnect to restart the download process. */
            Gap_Disconnect (deviceId);
        }
        else
        {
            otapStatus = gOtapStatusInvalidCommandParameter_c;
        }
    }
    else
    {
        otapStatus = gOtapStatusInvalidCommandLength_c;
    }
        
    if (otapStatus != gOtapStatusSuccess_c)
    {
        otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
        otapCommand.cmd.errNotif.cmdId = gOtapCmdIdNewImageInfoResponse_c;
        otapCommand.cmd.errNotif.errStatus = otapStatus;

        bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                    (void*)(&otapCommand),
                                                    cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
        if (gBleSuccess_c == bleResult)
        {
            otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
        }
        else
        {
            OtapClient_HandleServerCommError (deviceId);
        }
    }
}

static void OtapClient_HandleNewImageInfoRequestConfirmation (deviceId_t deviceId)
{
    /* Clear the last command sent to the OTAP Server for which a Confirmation is expected. */
    otapClientData.lastCmdSentToOtapServer = gOtapCmdIdNoCommand_c;
    
    /* Nothing more to do here. If the New Image Info Request Command has reached
     * the OTAP Server then the OTAP Client expects a New Image Info Response */
}

static void OtapClient_HandleImageBlockRequestConfirmation (deviceId_t deviceId)
{
    /* Clear the last command sent to the OTAP Server for which a Confirmation is expected. */
    otapClientData.lastCmdSentToOtapServer = gOtapCmdIdNoCommand_c;
    
    /* Nothing more to do here. If the Image Block Request Command has reached
     * the OTAP Server then the OTAP Client expects the requested image chunks
     * or an Error Notification. */
}

static void OtapClient_HandleImageTransferCompleteConfirmation (deviceId_t deviceId)
{
    otapCommand_t otapCommand;
    bleResult_t   bleResult;
    
    /* Clear the last command sent to the OTAP Server for which a Confirmation is expected. */
    otapClientData.lastCmdSentToOtapServer = gOtapCmdIdNoCommand_c;
    
    /* If the image transfer was not successful then the image download state should be Idle.
     * If it is, try to trigger a new download.
     * If the Image Transfer Complete Command has reached the OTAP Server and the transfer was succesful 
     * then the OTAP Client will just wait for the restart and the
     * bootloader to flash the new image. */
    if (otapClientData.state == mOtapClientStateIdle_c)
    {
        otapCommand.cmdId = gOtapCmdIdNewImageInfoRequest_c;
        FLib_MemCpy (otapCommand.cmd.newImgInfoReq.currentImageId,
                     (uint8_t*)otapClientData.currentImgId,
                     gOtap_ImageIdFieldSize_c);
        FLib_MemCpy (otapCommand.cmd.newImgInfoReq.currentImageVersion,
                     (uint8_t*)otapClientData.currentImgVer,
                     gOtap_ImageVersionFieldSize_c);
        
        bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                    (void*)(&otapCommand),
                                                    cmdIdToCmdLengthTable[gOtapCmdIdNewImageInfoRequest_c]);
        if (gBleSuccess_c == bleResult)
        {
            otapClientData.lastCmdSentToOtapServer = gOtapCmdIdNewImageInfoRequest_c;
        }
        else
        {
            OtapClient_HandleServerCommError (deviceId);
        }
    }
    else if (otapClientData.state == mOtapClientStateImageDownloadComplete_c)
    {
        /* If the image transfer is complete trigger the bootloader and reset the device. */
        Gap_Disconnect (deviceId);
        OTA_SetNewImageFlag ();
        ResetMCU ();
    }
}

static void OtapClient_HandleErrorNotificationConfirmation (deviceId_t deviceId)
{
    /* Clear the last command sent to the OTAP Server for which a Confirmation is expected. */
    otapClientData.lastCmdSentToOtapServer = gOtapCmdIdNoCommand_c;
    
    /* Reset block download parameters to safe values. */
    OtapClient_ResetBlockTransferParameters (deviceId);
    
    /* If an error has occured try to continue the image download from a "safe" point. */
    OtapClient_ContinueImageDownload (deviceId);
}

static void OtapClient_HandleStopImageTransferConfirmation (deviceId_t deviceId)
{
    /* Clear the last command sent to the OTAP Server for which a Confirmation is expected. */
    otapClientData.lastCmdSentToOtapServer = gOtapCmdIdNoCommand_c;
    
    /* Reset block download parameters to safe values. */
    OtapClient_ResetBlockTransferParameters (deviceId);
    
    /* If an error has occured try to continue the image download from a "safe" point. */
    OtapClient_ContinueImageDownload (deviceId);
}

static void OtapClient_HandleConnectionEvent (deviceId_t deviceId)
{
#if gAppUseBonding_d
    bool_t isBonded = FALSE;
    
    if ((gBleSuccess_c == Gap_CheckIfBonded (mPeerDeviceId, &isBonded)) &&
        (TRUE == isBonded)) 
    {
        otapClientData.serverWrittenCccd = TRUE;
    }
#endif
    
    switch (otapClientData.state)
    {
    case mOtapClientStateIdle_c:
        /*! If the OTAP Server has written the CCCD to receive commands fromt he OTAp Client then send a
         *  new image info request. */
        if (otapClientData.serverWrittenCccd == TRUE)
        {
            otapCommand_t otapCommand;
            bleResult_t   bleResult;
    
            otapCommand.cmdId = gOtapCmdIdNewImageInfoRequest_c;
            FLib_MemCpy (otapCommand.cmd.newImgInfoReq.currentImageId,
                         (uint8_t*)otapClientData.currentImgId,
                         gOtap_ImageIdFieldSize_c);
            FLib_MemCpy (otapCommand.cmd.newImgInfoReq.currentImageVersion,
                         (uint8_t*)otapClientData.currentImgVer,
                         gOtap_ImageVersionFieldSize_c);
            
            bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                        (void*)(&otapCommand),
                                                        cmdIdToCmdLengthTable[gOtapCmdIdNewImageInfoRequest_c]);
            if (gBleSuccess_c == bleResult)
            {
                otapClientData.lastCmdSentToOtapServer = gOtapCmdIdNewImageInfoRequest_c;
                otapClientData.serverWrittenCccd = TRUE;
            }
            else
            {
                OtapClient_HandleServerCommError (deviceId);
            }
        }
        break;
    case  mOtapClientStateDownloadingImage_c:
        /*! If the state is Downloading try to continue the download from where it was left off.
         *  Check if the appropriate server is connected first. */
        if (otapClientData.peerOtapServer == deviceId)
        {
            /* Reset block download parameters to safe values. */
            OtapClient_ResetBlockTransferParameters (deviceId);
            
            if (otapClientData.serverWrittenCccd == TRUE)
            {
                OtapClient_ContinueImageDownload (deviceId);
            }
        }
        break;
    case mOtapClientStateImageDownloadComplete_c:
        /*! If the image download is complete try to set the new image flag
         *  and reset the MCU for the bootloader ot kick in. */
        Gap_Disconnect (deviceId);
        OTA_SetNewImageFlag ();
        ResetMCU ();
        break;
    default:
        /* Some kind of internal error has occurred. Reset the
         * client state to Idle and act as if the state was Idle. */
        otapClientData.state = mOtapClientStateIdle_c;
        break;
    };
}

static void OtapClient_HandleDisconnectionEvent (deviceId_t deviceId)
{
    /* Check if the peer OTAP server was disconnected and if so reset block download
     * parameters to safe values. */
    if (otapClientData.peerOtapServer == deviceId)
    {
        OtapClient_ResetBlockTransferParameters (deviceId);
    }
    
    otapClientData.serverWrittenCccd = FALSE;
    otapClientData.l2capPsmConnected = FALSE;
}

static void OtapClient_ContinueImageDownload (deviceId_t deviceId)
{
    otapCommand_t   otapCommand;
    bleResult_t     bleResult;
    uint32_t        bytesToDownload;
    uint32_t        maxBlockSize;

    /* If the Server did not write the CCCD and the image is being downloaded exit immediately.
     * No commands can be exchanged before the CCCD is written. */
    if ((otapClientData.serverWrittenCccd == FALSE) &&
        (otapClientData.state == mOtapClientStateDownloadingImage_c))
    {
        return;
    }
    
    /* Check if the L2CAP OTAP PSM is not connected and the image download is not complete.
	 * If yes try to connect and exit immediatley. */
    if ((otapClientData.l2capPsmConnected == FALSE) &&
        (otapClientData.state != mOtapClientStateImageDownloadComplete_c))
    {
        L2ca_ConnectLePsm (gOtap_L2capLePsm_c,
                           deviceId,
                           mAppLeCbInitialCredits_c);
        return;
    }
    
    switch (otapClientData.state)
    {
    case mOtapClientStateIdle_c:
        /* If the state is Idle do nothing. No need to continue the transfer of an image. */
        break;
    case mOtapClientStateDownloadingImage_c:
        /* If the last received chunk sequence number is equal to the total block
         * chunks or they are both zero then ask for a new block from the server. */
        if (otapClientData.chunkSeqNum == otapClientData.totalBlockChunks)
        {
            /* Ask for another block only if the image transfer was not completed. */
            if (otapClientData.currentPos < otapClientData.imgSize)
            {
                bytesToDownload = otapClientData.imgSize - otapClientData.currentPos;
                
                if (otapClientData.transferMethod == gOtapTransferMethodAtt_c)
                {
                    maxBlockSize = otapClientData.negotiatedMaxAttChunkSize * gOtap_MaxChunksPerBlock_c;
                    otapClientData.l2capChannelOrPsm = gL2capCidAtt_c;
                    otapClientData.chunkSize = otapClientData.negotiatedMaxAttChunkSize;
                }
                else if (otapClientData.transferMethod == gOtapTransferMethodL2capCoC_c)
                {
                    if (otapClientData.l2capChannelOrPsm == gOtap_L2capLePsm_c)
                    {
                        maxBlockSize = otapClientData.negotiatedMaxL2CapChunkSize * gOtap_MaxChunksPerBlock_c;
                        otapClientData.chunkSize = otapClientData.negotiatedMaxL2CapChunkSize;
                    }
                    else
                    {
                        /* If the L2CAP CoC is not valid then some kind of error or missconfiguration has
                         * occurred. Send a proper error notification to the peer and
                         * reset the download state machine to Idle. */
                        otapClientData.state = mOtapClientStateIdle_c;
                        
                        otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
                        otapCommand.cmd.errNotif.cmdId = gOtapCmdIdNoCommand_c;
                        otapCommand.cmd.errNotif.errStatus = gOtapStatusUnexpectedL2capChannelOrPsm_c;

                        bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                                    (void*)(&otapCommand),
                                                                    cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
                        if (gBleSuccess_c == bleResult)
                        {
                            otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
                        }
                        else
                        {
                            OtapClient_HandleServerCommError (deviceId);
                        }
                        
                        return;
                    }
                }
                else
                {
                    /* If the transfer method is not recognized then this image has been missconfigured
                     * or a critical error has occurred. Send a proper error notification to the peer and
                     * reset the download state machien to Idle. */
                    otapClientData.state = mOtapClientStateIdle_c;
                    
                    otapCommand.cmdId = gOtapCmdIdErrorNotification_c;
                    otapCommand.cmd.errNotif.cmdId = gOtapCmdIdNoCommand_c;
                    otapCommand.cmd.errNotif.errStatus = gOtapStatusUnexpectedTransferMethod_c;

                    bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                                (void*)(&otapCommand),
                                                                cmdIdToCmdLengthTable[gOtapCmdIdErrorNotification_c]);
                    if (gBleSuccess_c == bleResult)
                    {
                        otapClientData.lastCmdSentToOtapServer = gOtapCmdIdErrorNotification_c;
                    }
                    else
                    {
                        OtapClient_HandleServerCommError (deviceId);
                    }
                    
                    return;
                }
                
                if (bytesToDownload >= maxBlockSize)
                {
                    /* If there are more bytes to download than the maximum block size then
                     * ask a full block from the server on the selected tansfer method and set up
                     * the client to recieve the chunks.*/
                    otapClientData.chunkSeqNum = 0;
                    otapClientData.totalBlockChunks = gOtap_MaxChunksPerBlock_c;
                    otapClientData.totalBlockSize = maxBlockSize;
                }
                else
                {
                    /* If there are fewer bytes to download than the maximum block size then compute the
                    *  number of chunks expected and set the expected block size to the number of 
                    *  bytes to download. */
                    otapClientData.chunkSeqNum = 0;
                    /* Compute number of full chunks. Integer division. */
                    otapClientData.totalBlockChunks = bytesToDownload / otapClientData.chunkSize;
                    /* Add an extra chunk if the chunk size is not a divisor of the number of bytes to download. */
                    otapClientData.totalBlockChunks += (bytesToDownload % otapClientData.chunkSize) ? 1 : 0;
                    otapClientData.totalBlockSize = bytesToDownload;
                }

                /* Send the Block request Command with the determined parameters. */
                otapCommand.cmdId = gOtapCmdIdImageBlockRequest_c;
                
                FLib_MemCpy(otapCommand.cmd.imgBlockReq.imageId, otapClientData.imgId, gOtap_ImageIdFieldSize_c);
                otapCommand.cmd.imgBlockReq.startPosition = otapClientData.currentPos;
                otapCommand.cmd.imgBlockReq.blockSize = otapClientData.totalBlockSize;
                otapCommand.cmd.imgBlockReq.chunkSize = otapClientData.chunkSize;
                otapCommand.cmd.imgBlockReq.transferMethod = otapClientData.transferMethod;
                otapCommand.cmd.imgBlockReq.l2capChannelOrPsm = otapClientData.l2capChannelOrPsm;
                
                bleResult = OtapCS_SendCommandToOtapServer (service_otap,
                                                            (void*)(&otapCommand),
                                                            cmdIdToCmdLengthTable[gOtapCmdIdImageBlockRequest_c]);
                if (gBleSuccess_c == bleResult)
                {
                    otapClientData.lastCmdSentToOtapServer = gOtapCmdIdImageBlockRequest_c;
                }
                else
                {
                    OtapClient_HandleServerCommError (deviceId);
                }
            }
        }
        break;
    case mOtapClientStateImageDownloadComplete_c:
        /*! If the image download is complete try to set the new image flag
         *  and reset the MCU for the bootloader ot kick in. */
        Gap_Disconnect (deviceId);
        OTA_SetNewImageFlag ();
        ResetMCU ();
        break;
    default:
        /* This code should never be reached in normal running conditions.
        Do nothing here, no need to continue the transfer of an image. */
        break;
    };
}

static void OtapClient_ResetBlockTransferParameters (deviceId_t deviceId)
{
    otapClientData.chunkSize = 0;
    otapClientData.chunkSeqNum = 0;
    otapClientData.totalBlockChunks = 0;
    otapClientData.totalBlockSize = 0;
}

static void OtapClient_HandleServerCommError (deviceId_t deviceId)
{
    /* Disconnect to trigger communication reset. */
    Gap_Disconnect (deviceId);
}

static bool_t OtapClient_IsRemoteImageNewer (uint8_t* pRemoteImgId, uint8_t* pRemoteImgVer)
{
    uint32_t    remoteBuildVer;
    uint32_t    localeBuildVer;
    /* Ignore the Image Id for the moment. */
    /* Check the Manufacturer Id */
    if (pRemoteImgVer[7] != otapClientData.currentImgVer[7])
    {
        return FALSE;
    }
    
    /* Check Hardware Id */
    if (!FLib_MemCmp((void*)(&(pRemoteImgVer[4])), (void*)(&(otapClientData.currentImgVer[4])), 3))
    {
        return FALSE;
    }
    
    /* Check Stack Version */
    if (pRemoteImgVer[3] < otapClientData.currentImgVer[3])
    {
        return FALSE;
    }
    
    /* Check Build Version */
    remoteBuildVer = (uint32_t)pRemoteImgVer[0] + ((uint32_t)(pRemoteImgVer[1]) << 8) + ((uint32_t)(pRemoteImgVer[2]) << 16);
    localeBuildVer = (uint32_t)otapClientData.currentImgVer[0] + ((uint32_t)(otapClientData.currentImgVer[1]) << 8) + ((uint32_t)(otapClientData.currentImgVer[2]) << 16);
    if (remoteBuildVer <= localeBuildVer)
    {
        return FALSE;
    }
    
    return TRUE;
}

static otapStatus_t OtapClient_IsImageFileHeaderValid (bleOtaImageFileHeader_t* imgFileHeader)
{
    if (imgFileHeader->fileIdentifier != gBleOtaFileHeaderIdentifier_c)
    {
        return gOtapStatusUnknownFileIdentifier_c;
    }
    
    if (imgFileHeader->headerVersion != gbleOtapHeaderVersion0100_c)
    {
        return gOtapStatusUnknownHeaderVersion_c;
    }
    
    if (imgFileHeader->headerLength != sizeof(bleOtaImageFileHeader_t))
    {
        return gOtapStatusUnexpectedHeaderLength_c;
    }
    
    if (imgFileHeader->fieldControl != gBleOtaFileHeaderDefaultFieldControl_c)
    {
        return gOtapStatusUnexpectedHeaderFieldControl_c;
    }
    
    if (imgFileHeader->companyId != gBleOtaCompanyIdentifier_c)
    {
        return gOtapStatusUnknownCompanyId_c;
    }
    
    if (FALSE == FLib_MemCmp (imgFileHeader->imageId, otapClientData.imgId, sizeof(imgFileHeader->imageId)))
    {
        return gOtapStatusUnexpectedImageId_c;
    }
    
    if (FALSE == FLib_MemCmp (imgFileHeader->imageVersion, otapClientData.imgVer, sizeof(imgFileHeader->imageVersion)))
    {
        return gOtapStatusUnexpectedImageVersion_c;
    }
    
    if (imgFileHeader->totalImageFileSize != otapClientData.imgSize)
    {
        return gOtapStatusUnexpectedImageFileSize_c;
    }
    
    return gOtapStatusSuccess_c;
}

static void BatteryMeasurementTimerCallback(void * pParam)
{
    basServiceConfig.batteryLevel = BOARD_GetBatteryLevel();
    Bas_RecordBatteryMeasurement(basServiceConfig.serviceHandle, basServiceConfig.batteryLevel);
}


/*! *********************************************************************************
* @}
********************************************************************************** */
