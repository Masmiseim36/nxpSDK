/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* OTA Programming Demo Server application.
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

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "otap_server.h"
#include "OtaMessages.h"

/* Drv */
#include "LED.h"
#include "Keyboard.h"
#include "Eeprom.h"

/* Fwk */
#include "SecLib.h"
#include "FsciInterface.h"
#include "SerialManager.h"
#include "RNG_Interface.h"
#include "MemManager.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "OtaSupport.h"

/* 802.15.4 */
#include "PhyInterface.h"
#include "MacInterface.h"

/* KSDK */
#include "board.h"
#include "fsl_os_abstraction.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

#define gDefaultImageChunkLength_c 200 /* [bytes] */


/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static uint8_t App_StartScan(macScanType_t scanType);
static void    App_HandleScanEdConfirm(nwkMessage_t *pMsg);
static uint8_t App_StartCoordinator(void);
static uint8_t App_HandleMlmeInput(nwkMessage_t *pMsg);
static uint8_t App_SendAssociateResponse(nwkMessage_t *pMsgIn);
static void    App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn);
static uint8_t App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType);
static void    App_HandleKeys(key_event_t events);

otaResult_t SetOtapMode(uint8_t* buffer, uint16_t len);
otaResult_t QueryImageReqConfirm(uint8_t* buffer, uint16_t len);
otaResult_t ProcessImageChunk(uint8_t* buffer, uint16_t len);

static bool_t  sendImageNotify(uint16_t Address);
static void sendQueryNextImageResponse(uint16_t Address);
static void sendImageBlockResponse(uint16_t Address, uint8_t status);
static void sendUpgradeEndResponse(uint16_t Address);
static void ProcessMessage(mcpsToNwkMessage_t *pMsgIn, uint16_t addr);

void App_init( void );
void AppThread (uint32_t argument);
resultType_t MLME_NWK_SapHandler (nwkMessage_t* pMsg, instanceId_t instanceId);
resultType_t MCPS_NWK_SapHandler (mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);
extern void Mac_SetExtendedAddress(uint8_t *pAddr, instanceId_t instanceId);

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
/* The Coordinator's addressing information */
static uint64_t mExtendedAddress  = mMacExtendedAddress_c;
static uint16_t mShortAddress = mDefaultValueOfShortAddress_c;
static uint16_t mPanId = mDefaultValueOfPanId_c;

/* The current logical channel (frequency band) */
static uint8_t mLogicalChannel;

/* These byte arrays stores an associated
   devices long and short addresses. */
static uint16_t mDeviceShortAddress;
static uint64_t mDeviceLongAddress;

/* Data request packet for sending data OTA */
static nwkToMcpsMessage_t *mpPacket = NULL;
/* The MSDU handle is a unique data packet identifier */
static uint8_t mMsduHandle = 0;

/* Application input queues */
anchor_t mMlmeNwkInputQueue;
anchor_t mMcpsNwkInputQueue;

/* OTAP variables */
static basicImageHeader_t mOtapHeader;
static uint8_t  mDataBuffer[gImageDataPacketMaxSize_c];
static uint8_t  mDataLen;
static uint16_t mCurrentDeviceId = 0xFFFF;
static uint8_t  mMaxDataSize;
static uint32_t mFileOffset = 0;
bool_t gOtaNewImageAvailable = FALSE;
uint32_t gImageOffset;

static instanceId_t macInstance;
osaEventId_t        mAppEvent;

static const gFsciSerialConfig_t mFsciSerials[] = {
    /* Baudrate,            interface type,   channel No, virtual interface */
    {gUARTBaudRate115200_c, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE,           0}
};

/* The current state of the applications state machine */
static uint8_t gState;

otaServer_AppCB_t mOTAcb = {
    NULL,
    SetOtapMode,
    QueryImageReqConfirm,
    ProcessImageChunk,
    NULL,
    NULL,
    NULL,
};

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize 
*         the system
*
* \param[in]  argument
*
* \return  None.
*
* \pre
*
* \post
*
* \remarks
*
********************************************************************************** */
void main_task(uint32_t param)
{
    static uint8_t initialized = FALSE;
    
    if( !initialized )
    {
        initialized = TRUE;  
        hardware_init();
        MEM_Init();
        TMR_Init();
        LED_Init();
        SecLib_Init();
        SerialManager_Init();
        Phy_Init();
        RNG_Init(); /* RNG must be initialized after the PHY is Initialized */
        MAC_Init();
        
        /* Bind to MAC layer */
        macInstance = BindToMAC( (instanceId_t)0 );
        Mac_RegisterSapHandlers( MCPS_NWK_SapHandler, MLME_NWK_SapHandler, macInstance );
        
        FSCI_Init( (void*)mFsciSerials );    
        OTA_RegisterToFsci(0, &mOTAcb);
        
        App_init();
    }
    /* Call application task */
    AppThread( param );
}

/*****************************************************************************
* Initialization function for the App Task. This is called during
* initialization and should contain any application specific initialization
* (ie. hardware initialization/setup, table initialization, power up
* notificaiton.
*
* Interface assumptions: None
*
* Return value: None
*
*****************************************************************************/
void App_init( void )
{
    mAppEvent = OSA_EventCreate(TRUE);
    /* The initial application state */
    gState = stateInit;
    /* Initialize the MAC 802.15.4 extended address */
    Mac_SetExtendedAddress( (uint8_t*)&mExtendedAddress, macInstance );
    /* register keyboard callback function */
    KBD_Init(App_HandleKeys);
    /* Prepare input queues.*/
    MSG_InitQueue(&mMlmeNwkInputQueue); 
    MSG_InitQueue(&mMcpsNwkInputQueue);
    
    /*signal app ready*/
    LED_StartSerialFlash(LED1);
}

/*****************************************************************************
* Mac Application Task event processor.  This function is called to
* process all events for the task. Events include timers, messages and any
* other user defined events
*
* Interface assumptions: None
*
* Return value: None
*****************************************************************************/
void AppThread (uint32_t argument)
{ 
    osaEventFlags_t ev;
    /* Pointer for storing the messages from MLME, MCPS, and ASP. */
    void *pMsgIn;
    /* Stores the status code returned by some functions. */
    uint8_t ret;  
    
    while(1)
    {
        OSA_EventWait(mAppEvent, osaEventFlagsAll_c, FALSE, osaWaitForever_c, &ev);

        if( !gUseRtos_c && !ev)
        {
            break;
        }

        pMsgIn = NULL;
        
        /* Dequeue the MLME message */
        if (ev & gAppEvtMessageFromMLME_c)
        {
            /* Get the message from MLME */
            pMsgIn = MSG_DeQueue(&mMlmeNwkInputQueue);
            
            /* Any time a beacon might arrive. Always handle the beacon frame first */
            if (pMsgIn)
            {
                ret = App_WaitMsg(pMsgIn, gMlmeBeaconNotifyInd_c);
                if(ret == errorNoError)
                {
                    /* ALWAYS free the beacon frame contained in the beacon notify indication.*/
                    /* ALSO the application can use the beacon payload.*/
                    MSG_Free(((nwkMessage_t *)pMsgIn)->msgData.beaconNotifyInd.pBufferRoot);
                }
            }
        }
        /* The application state machine */
        switch(gState)
        {
        case stateInit:
            /* Goto Energy Detection state. */
            gState = stateScanEdStart;
            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
            break;
            
        case stateScanEdStart:
            /* Start the Energy Detection scan, and goto wait for confirm state. */
            ret = App_StartScan(gScanModeED_c);
            if(ret == errorNoError)
            {
                gState = stateScanEdWaitConfirm;
            }
            break;
            
        case stateScanEdWaitConfirm:
            /* Stay in this state until the MLME Scan confirm message arrives,
            and has been processed. Then goto Start Coordinator state. */
            if (ev & gAppEvtMessageFromMLME_c)
            {
                if (pMsgIn)
                {
                    ret = App_WaitMsg(pMsgIn, gMlmeScanCnf_c);
                    if(ret == errorNoError)
                    {
                        /* Process the ED scan confirm. The logical
                        channel is selected by this function. */
                        App_HandleScanEdConfirm(pMsgIn);
                        /* Go to the Start Coordinator state */
                        gState = stateStartCoordinator;
                        OSA_EventSet(mAppEvent, gAppEvtStartCoordinator_c);
                    }
                }
            }
            break;
            
        case stateStartCoordinator:
            if (ev & gAppEvtStartCoordinator_c)
            {
                /* Start up as a PAN Coordinator on the selected channel. */
                ret = App_StartCoordinator();
                if(ret == errorNoError)
                {
                    /* If the Start request was sent successfully to
                    the MLME, then goto Wait for confirm state. */
                    gState = stateStartCoordinatorWaitConfirm;
                }
            }
            break;
            
        case stateStartCoordinatorWaitConfirm:
            /* Stay in this state until the Start confirm message
            arrives, and then goto the Listen state. */
            if (ev & gAppEvtMessageFromMLME_c)
            {
                if (pMsgIn)
                {
                    ret = App_WaitMsg(pMsgIn, gMlmeStartCnf_c);
                    if(ret == errorNoError)
                    {
                        gState = stateListen;
                        LED_TurnOffAllLeds();
                        OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                    }
                }
            }
            break;
            
        case stateListen:
            /* Stay in this state forever.*/
            if (ev & gAppEvtMessageFromMLME_c)
            {
                /* Get the message from MLME */
                if (pMsgIn)
                {
                    /* Process it */
                    ret = App_HandleMlmeInput(pMsgIn);
                    /* Messages from the MLME must always be freed. */
                }
            }
            
            /* OTAP events */
            if (ev & gAppEvtImageNotify_c)
            {
                /* This event is received when a new image is available. */
                (void)sendImageNotify(1);
            }
            
            if (ev & gAppEvtImageChunkRequestEvent_c)
            {
                /* Request a new image chunk */
                OTA_ImageChunkReq(gImageOffset, gDefaultImageChunkLength_c, 0xFFFF);
                
            }
            
            if (ev & gAppEvtQueryImageRequestEvent_c)
            {
                /* Query for all types of images and all types of devices */
                OTA_QueryImageReq(0xFFFF, 0xFFFF, 0xFFFF, 0xFFFFFFFF);
            }
            
            if (ev & gAppEvtImageBlockResponseEvent_c)
            {
                /* Send an Image Block OTA */
                sendImageBlockResponse(mCurrentDeviceId, gOtaStatusSuccess_c);
            }
            
            break;
        }
        
        if (pMsgIn)
        {
            /* Messages must always be freed. */
            MSG_Free(pMsgIn);
            pMsgIn = NULL;
        }
        
        if (ev & gAppEvtMessageFromMCPS_c)
        {
            /* Get the message from MCPS */
            pMsgIn = MSG_DeQueue(&mMcpsNwkInputQueue);
            if (pMsgIn)
            {
                /* Process it */
                App_HandleMcpsInput(pMsgIn);
                /* Messages from the MCPS must always be freed. */
                MSG_Free(pMsgIn);
                pMsgIn = NULL;
            }
        }
        
        /* Check for pending messages in the Queue */
        if(MSG_Pending(&mMcpsNwkInputQueue))
            OSA_EventSet(mAppEvent, gAppEvtMessageFromMCPS_c);
        if(MSG_Pending(&mMlmeNwkInputQueue))
            OSA_EventSet(mAppEvent, gAppEvtMessageFromMLME_c);
        
        if( !gUseRtos_c )
        {
            break;
        } 
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/******************************************************************************
* The App_StartScan(scanType) function will start the scan process of the
* specified type in the MAC. This is accomplished by allocating a MAC message,
* which is then assigned the desired scan parameters and sent to the MLME
* service access point.
* The function may return either of the following values:
*   errorNoError:          The Scan message was sent successfully.
*   errorInvalidParameter: The MLME service access point rejected the
*                          message due to an invalid parameter.
*   errorAllocFailed:      A message buffer could not be allocated.
*
******************************************************************************/
static uint8_t App_StartScan(macScanType_t scanType)
{
    mlmeMessage_t *pMsg;
    mlmeScanReq_t *pScanReq;
    
    /* Allocate a message for the MLME (We should check for NULL). */
    pMsg = MSG_AllocType(mlmeMessage_t);
    if(pMsg != NULL)
    {
        /* This is a MLME-SCAN.req command */
        pMsg->msgType = gMlmeScanReq_c;
        /* Create the Scan request message data. */
        pScanReq = &pMsg->msgData.scanReq;
        /* gScanModeED_c, gScanModeActive_c, gScanModePassive_c, or gScanModeOrphan_c */
        pScanReq->scanType = scanType;
        
#ifdef gPHY_802_15_4g_d
        pScanReq->channelPage = gChannelPageId9_c;
        pScanReq->scanChannels[0] = mDefaultValueOfChannel_c;
#else
        pScanReq->scanChannels = mDefaultValueOfChannel_c;
#endif
        /* Duration per channel 0-14 (dc). T[sec] = (16*960*((2^dc)+1))/1000000.
        A scan duration of 3 on 16 channels approximately takes 2 secs. */
        pScanReq->scanDuration = 3;
        pScanReq->securityLevel = gMacSecurityNone_c;
        
        /* Send the Scan request to the MLME. */
        if( NWK_MLME_SapHandler( pMsg, macInstance ) == gSuccess_c )
        {
            return errorNoError;
        }
        else
        {
            return errorInvalidParameter;
        }
    }
    else
    {
        return errorAllocFailed;
    }
}

/******************************************************************************
* The App_HandleScanEdConfirm(nwkMessage_t *pMsg) function will handle the
* ED scan confirm message received from the MLME when the ED scan has completed.
* The message contains the ED scan result list. This function will search the
* list in order to select the logical channel with the least energy. The
* selected channel is stored in the global variable called 'mLogicalChannel'.
*
******************************************************************************/
static void App_HandleScanEdConfirm(nwkMessage_t *pMsg)
{
    uint8_t n, minEnergy;
    uint8_t *pEdList;
    uint8_t ChannelMask;
    
    
    /* Get a pointer to the energy detect results */
    pEdList = pMsg->msgData.scanCnf.resList.pEnergyDetectList;
    
    /* Set the minimum energy to a large value */
    minEnergy = 0xFF;
    
    /* Select default channel */
    mLogicalChannel = 11;
    
    /* Search for the channel with least energy */
    for(n=0; n<16; n++)
    {
        ChannelMask = n + 11;
        if((pEdList[n] < minEnergy)&&((uint8_t)((mDefaultValueOfChannel_c>>ChannelMask) & 0x1)))
        {
            minEnergy = pEdList[n];
            /* Channel numbering is 11 to 26 both inclusive */
            mLogicalChannel = n + 11;
        }
    }
    
    /* The list of detected energies must be freed. */
    MSG_Free(pEdList);
}

/******************************************************************************
* The App_StartScan(scanType) function will start the scan process of the
* specified type in the MAC. This is accomplished by allocating a MAC message,
* which is then assigned the desired scan parameters and sent to the MLME
* service access point. The MAC PIB attributes "macShortAddress", and
* "macAssociatePermit" are modified.
*
* The function may return either of the following values:
*   errorNoError:          The Scan message was sent successfully.
*   errorInvalidParameter: The MLME service access point rejected the
*                          message due to an invalid parameter.
*   errorAllocFailed:      A message buffer could not be allocated.
*
******************************************************************************/
static uint8_t App_StartCoordinator(void)
{
    /* Message for the MLME will be allocated and attached to this pointer */
    mlmeMessage_t *pMsg;
    
    /* Allocate a message for the MLME (We should check for NULL). */
    pMsg = MSG_AllocType(mlmeMessage_t);
    if(pMsg != NULL)
    {
        /* Pointer which is used for easy access inside the allocated message */
        mlmeStartReq_t *pStartReq;
        
        /* Boolean value that will be written to the MAC PIB */
        uint8_t boolFlag, val;
        
        /* Set-up MAC PIB attributes. Please note that Set, Get,
        and Reset messages are not freed by the MLME. */
        
        /* We must always set the short address to something
       else than 0xFFFF before starting a PAN. */
        pMsg->msgType = gMlmeSetReq_c;
        pMsg->msgData.setReq.pibAttribute = gMPibShortAddress_c;
        pMsg->msgData.setReq.pibAttributeValue = (uint8_t *)&mShortAddress;
        (void)NWK_MLME_SapHandler( pMsg, macInstance );
        
        /* Modify Transaction Persistence Time */
        pMsg->msgType = gMlmeSetReq_c;
        pMsg->msgData.setReq.pibAttribute = gMPibNBSuperFrameInterval_c;
        val = 0;
        pMsg->msgData.setReq.pibAttributeValue = &val;
        (void)NWK_MLME_SapHandler( pMsg, macInstance );
        
        pMsg->msgType = gMlmeSetReq_c;
        pMsg->msgData.setReq.pibAttribute = gMPibTransactionPersistenceTime_c;
        val = 0xC0;
        pMsg->msgData.setReq.pibAttributeValue = &val;
        (void)NWK_MLME_SapHandler( pMsg, macInstance );
        
        /* We must set the Association Permit flag to TRUE
        in order to allow devices to associate to us. */
        pMsg->msgType = gMlmeSetReq_c;
        pMsg->msgData.setReq.pibAttribute = gMPibAssociationPermit_c;
        boolFlag = TRUE;
        pMsg->msgData.setReq.pibAttributeValue = &boolFlag;
        (void)NWK_MLME_SapHandler( pMsg, macInstance );
        
        /* This is a MLME-START.req command */
        pMsg->msgType = gMlmeStartReq_c;
        
        /* Create the Start request message data. */
        pStartReq = &pMsg->msgData.startReq;
        /* PAN ID - LSB, MSB. The example shows a PAN ID of 0xBEEF. */
        FLib_MemCpy(&pStartReq->panId, &mPanId, 2);
        /* Logical Channel - the default of 11 will be overridden */
        pStartReq->logicalChannel = mLogicalChannel;
        /* Beacon Order - 0xF = turn off beacons */
        pStartReq->beaconOrder = 0x0F;
        /* Superframe Order - 0xF = turn off beacons */
        pStartReq->superframeOrder = 0x0F;
        /* Be a PAN coordinator */
        pStartReq->panCoordinator = TRUE;
        /* Dont use battery life extension */
        pStartReq->batteryLifeExtension = FALSE;
        /* This is not a Realignment command */
        pStartReq->coordRealignment = FALSE;
        /* Dont use security */
        pStartReq->coordRealignSecurityLevel = gMacSecurityNone_c;
        pStartReq->beaconSecurityLevel = gMacSecurityNone_c;
        
        /* Send the Start request to the MLME. */
        if(NWK_MLME_SapHandler( pMsg, macInstance ) == gSuccess_c)
        {
            return errorNoError;
        }
        else
        {
            /* One or more parameters in the Start Request message were invalid. */
            return errorInvalidParameter;
        }
    }
    else
    {
        /* Allocation of a message buffer failed. */
        return errorAllocFailed;
    }
}

/******************************************************************************
* The App_SendAssociateResponse(nwkMessage_t *pMsgIn) will create the response
* message to an Associate Indication (device sends an Associate Request to its
* MAC. The request is transmitted to the coordinator where it is converted into
* an Associate Indication). This function will extract the devices long address,
* and various other flags from the incoming indication message for building the
* response message.
*
* The function may return either of the following values:
*   errorNoError:          The Associate Response message was sent successfully.
*   errorInvalidParameter: The MLME service access point rejected the
*                          message due to an invalid parameter.
*   errorAllocFailed:      A message buffer could not be allocated.
*
******************************************************************************/
static uint8_t App_SendAssociateResponse(nwkMessage_t *pMsgIn)
{
    mlmeMessage_t *pMsg;
    mlmeAssociateRes_t *pAssocRes;
    
    /* Allocate a message for the MLME */
    pMsg = MSG_AllocType(mlmeMessage_t);
    if(pMsg != NULL)
    {
        /* This is a MLME-ASSOCIATE.res command */
        pMsg->msgType = gMlmeAssociateRes_c;
        
        /* Create the Associate response message data. */
        pAssocRes = &pMsg->msgData.associateRes;
        
        /* Assign a short address to the device. If a device do not want to use
        short addresses at all in the PAN, a short address of 0xFFFE must
        be assigned to it. */
        if(pMsgIn->msgData.associateInd.capabilityInfo & gCapInfoAllocAddr_c)
        {
            /* Assign a unique short address less than 0xfffe if the device requests so. */
            pAssocRes->assocShortAddress = 0x0001;
        }
        else
        {
            /* A short address of 0xfffe means that the device is granted access to
            the PAN (Associate successful) but that long addressing is used.*/
            pAssocRes->assocShortAddress = 0xFFFE;
        }
        /* Get the 64 bit address of the device requesting association. */
        FLib_MemCpy(&pAssocRes->deviceAddress, &pMsgIn->msgData.associateInd.deviceAddress, 8);
        /* Association granted. May also be gPanAtCapacity_c or gPanAccessDenied_c. */
        pAssocRes->status = gSuccess_c;
        /* Do not use security */
        pAssocRes->securityLevel = gMacSecurityNone_c;
        
        /* Save device info. */
        FLib_MemCpy(&mDeviceShortAddress, &pAssocRes->assocShortAddress, 2);
        FLib_MemCpy(&mDeviceLongAddress,  &pAssocRes->deviceAddress,     8);
        
        /* Send the Associate Response to the MLME. */
        if(NWK_MLME_SapHandler( pMsg, macInstance ) == gSuccess_c)
        {
            return errorNoError;
        }
        else
        {
            /* One or more parameters in the message were invalid. */
            return errorInvalidParameter;
        }
    }
    else
    {
        /* Allocation of a message buffer failed. */
        return errorAllocFailed;
    }
}

/******************************************************************************
* The App_HandleMlmeInput(nwkMessage_t *pMsg) function will handle various
* messages from the MLME, e.g. (Dis)Associate Indication.
*
* The function may return either of the following values:
*   errorNoError:   The message was processed.
*   errorNoMessage: The message pointer is NULL.
******************************************************************************/
static uint8_t App_HandleMlmeInput(nwkMessage_t *pMsg)
{
    if(pMsg == NULL)
        return errorNoMessage;
    
    /* Handle the incoming message. The type determines the sort of processing.*/
    switch(pMsg->msgType) {
    case gMlmeAssociateInd_c:
        /* A device sent us an Associate Request. We must send back a response.  */
        return App_SendAssociateResponse(pMsg);
        break;
        
    case gMlmeCommStatusInd_c:
        /* Sent by the MLME after the Association Response has been transmitted. */
        break;

    default:
        break;
    }
    return errorNoError;
}

/******************************************************************************
* The App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn) function will handle
* messages from the MCPS, e.g. Data Confirm, and Data Indication.
*
******************************************************************************/
static void App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn)
{
    switch(pMsgIn->msgType)
    {
    case gMcpsDataCnf_c:
        /* The MCPS-Data confirm is sent by the MAC to the network
        or application layer when data has been sent. */
        break;
        
    case gMcpsDataInd_c:
        /* The MCPS-Data indication is sent by the MAC to the network
        or application layer when data has been received. */
        ProcessMessage(pMsgIn, pMsgIn->msgData.dataInd.srcAddr);
        break;

    default:
        break;
    }
}

/******************************************************************************
* The App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType) function does not, as
* the name implies, wait for a message, thus blocking the execution of the
* state machine. Instead the function analyzes the supplied message to determine
* whether or not the message is of the expected type.
* The function may return either of the following values:
*   errorNoError: The message was of the expected type.
*   errorNoMessage: The message pointer is NULL.
*   errorWrongConfirm: The message is not of the expected type.
*
******************************************************************************/
static uint8_t App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType)
{
    /* Do we have a message? If not, the exit with error code */
    if(pMsg == NULL)
        return errorNoMessage;
    
    /* Is it the expected message type? If not then exit with error code */
    if(pMsg->msgType != msgType)
        return errorWrongConfirm;
    
    /* Found the expected message. Return with success code */
    return errorNoError;
}

/*****************************************************************************
* Handles all key events for this device.
* Interface assumptions: None
* Return value: None
*****************************************************************************/
static void App_HandleKeys(key_event_t events)
{
    switch ( events ) 
    { 
    case gKBD_EventSW1_c:
    case gKBD_EventSW2_c:
    case gKBD_EventSW3_c:
    case gKBD_EventSW4_c:
#if gTsiSupported_d
    case gKBD_EventSW5_c:    
    case gKBD_EventSW6_c:    
#endif
    case gKBD_EventLongSW1_c:
    case gKBD_EventLongSW2_c:
    case gKBD_EventLongSW3_c:
    case gKBD_EventLongSW4_c:
#if gTsiSupported_d
    case gKBD_EventLongSW5_c:
    case gKBD_EventLongSW6_c:       
#endif
        
        if(gState == stateInit)
        {
            LED_StopFlashingAllLeds();
            LED_TurnOnAllLeds();
            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
        }
        else 
        {
            if (gOtaNewImageAvailable)
            {
                OSA_EventSet(mAppEvent, gAppEvtImageNotify_c);
            }
        }
        break;
    }
}

/******************************************************************************
* The following functions are called by the MAC to put messages into the
* Application's queue. They need to be defined even if they are not used
* in order to avoid linker errors.
******************************************************************************/

resultType_t MLME_NWK_SapHandler (nwkMessage_t* pMsg, instanceId_t instanceId)
{
    /* Put the incoming MLME message in the applications input queue. */
    MSG_Queue(&mMlmeNwkInputQueue, pMsg);
    OSA_EventSet(mAppEvent, gAppEvtMessageFromMLME_c);
    return gSuccess_c;
}

resultType_t MCPS_NWK_SapHandler (mcpsToNwkMessage_t* pMsg, instanceId_t instanceId)
{
    /* Put the incoming MCPS message in the applications input queue. */
    MSG_Queue(&mMcpsNwkInputQueue, pMsg);
    OSA_EventSet(mAppEvent, gAppEvtMessageFromMCPS_c);
    return gSuccess_c;
}

/******************************************************************************
* The following function is called when a Data Indication is received from MAC.
* The packet is parsed and an event will be sent to the AppTask if .
*
* Return value:
*     None
******************************************************************************/
static void ProcessMessage(mcpsToNwkMessage_t *pMsgIn, uint16_t addr)
{
    switch (pMsgIn->msgData.dataInd.pMsdu[0])
    {
        /* Received a Next Image Request command */
    case QueryNextImageRequest:
        sendQueryNextImageResponse(addr);
        break;
        
        /* Received an Image Block Request command */
    case ImageBlockRequest:
        {
            imageBlockRequestCmd_t *ptr;
            
            // Check if the system is busy with another request
            if (mCurrentDeviceId != 0xFFFF)
                return;
            
            mCurrentDeviceId = addr;
            
            ptr = (imageBlockRequestCmd_t*)(pMsgIn->msgData.dataInd.pMsdu);
            mFileOffset = ptr->fileOffset;
            mMaxDataSize = ptr->maxDataSize;
            
            /* Check if the requested fileOffset is valid */
            if (mFileOffset >= mOtapHeader.totalImageSize)
            {
                sendImageBlockResponse(addr, gOtaStatusNotAuthorized_c);
                return;
            }
            
            if (mFileOffset + mMaxDataSize > mOtapHeader.totalImageSize)
                mDataLen = mOtapHeader.totalImageSize - mFileOffset;
            else
                mDataLen = mMaxDataSize;
            
            if ( gUpgradeMode == gUseExternalMemoryForOtaUpdate_c )
            {
                if ( OTA_ReadExternalMemory(mDataBuffer ,mMaxDataSize, mFileOffset) == gOtaSucess_c )
                    OSA_EventSet(mAppEvent, gAppEvtImageBlockResponseEvent_c);
            }
            else
            {
                /* Request new image chunk over the serial interface */
                OTA_ImageChunkReq(mFileOffset, mMaxDataSize, addr);
            }
            
            break;
        }
        
        /* Received an Upgrade End Request command */
    case UpgradeEndRequest:
        sendUpgradeEndResponse(addr);
        break;
    }
}

/******************************************************************************
* The following function is called when an Image Notify must be sent to the
* device.
*
* Return value:
*     None
******************************************************************************/
static bool_t sendImageNotify(uint16_t Address)
{
    imageNotifyCmd_t *ptr;
    
    mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + sizeof(imageNotifyCmd_t));
    
    if ( mpPacket != NULL )
    {
        mpPacket->msgType = gMcpsDataReq_c;
        mpPacket->msgData.dataReq.dstAddr = Address;
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr,  &mShortAddress, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mPanId, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mPanId, 2);
        mpPacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.msduLength  = sizeof(imageNotifyCmd_t);
        mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);
        
        ptr = (imageNotifyCmd_t*)(mpPacket->msgData.dataReq.pMsdu);
        ptr->cmdId          = ImageNotify;
        ptr->payloadType    = gPayloadQJitter_ManId_ImgType_Version;
        ptr->queryJitter    = gQueryJitterAllClients;
        ptr->manufacturerId = mOtapHeader.manufacturerId;
        ptr->imageType      = mOtapHeader.imageType;
        /* Force the image to be saved by ClintApp */
        ptr->fileVersion = 0xFFFFFFFF;
        
        if ( Address == 0xFFFF )
        {
            mpPacket->msgData.dataReq.txOptions = gMacTxOptionsNone_c;
        }
        else
        {
            mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
            mpPacket->msgData.dataReq.txOptions |= gMacTxOptionIndirect_c;
        }
        
        mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
        mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;
        
        (void)NWK_MCPS_SapHandler(mpPacket, macInstance);
        
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************
* The following function is called when a Query Next Image Response must be
* sent to the device.
*
* Return value:
*     None
******************************************************************************/
static void sendQueryNextImageResponse(uint16_t Address)
{
    queryNextImageResponseCmd_t *ptr;
    uint8_t status = gOtaStatusSuccess_c;
    
    if ( gOtaNewImageAvailable == FALSE )
        status = gOtaStatusNoImageAvailable_c;
    
    mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + sizeof(queryNextImageResponseCmd_t));
    
    if ( mpPacket != NULL )
    {
        mpPacket->msgType = gMcpsDataReq_c;
        mpPacket->msgData.dataReq.dstAddr = Address;
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr,  &mShortAddress, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mPanId, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mPanId, 2);
        mpPacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);
        
        ptr = (queryNextImageResponseCmd_t*)(mpPacket->msgData.dataReq.pMsdu);
        ptr->cmdId = QueryNextImageResponse;
        ptr->status = status;
        
        if (status == gOtaStatusSuccess_c)
        {
            ptr->manufacturerId = mOtapHeader.manufacturerId;
            ptr->imageType      = mOtapHeader.imageType;
            ptr->imageSize      = mOtapHeader.totalImageSize;
            ptr->fileVersion    = mOtapHeader.fileVersion;
            mpPacket->msgData.dataReq.msduLength = sizeof(queryNextImageResponseCmd_t);
        }
        else
            mpPacket->msgData.dataReq.msduLength = sizeof(shortResponseCmd_t);
        
        mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
        mpPacket->msgData.dataReq.txOptions |= gMacTxOptionIndirect_c;
        
        mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
        mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;
        
        (void)NWK_MCPS_SapHandler(mpPacket, macInstance);
    }
}

/******************************************************************************
* The following function is called when an Image Block Response must be sent
* to the device.
*
* Return value:
*     None
******************************************************************************/
static void sendImageBlockResponse(uint16_t Address, uint8_t status)
{
    imageBlockResponseCmd_t *ptr;
    nwkToMcpsMessage_t *pImagePacket;
    
    pImagePacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + gMaxPHYPacketSize_c);
    
    if ( pImagePacket != NULL )
    {
        pImagePacket->msgType = gMcpsDataReq_c;
        pImagePacket->msgData.dataReq.dstAddr = Address;
        FLib_MemCpy(&pImagePacket->msgData.dataReq.srcAddr,  &mShortAddress, 2);
        FLib_MemCpy(&pImagePacket->msgData.dataReq.dstPanId, &mPanId, 2);
        FLib_MemCpy(&pImagePacket->msgData.dataReq.srcPanId, &mPanId, 2);
        pImagePacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
        pImagePacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        pImagePacket->msgData.dataReq.pMsdu = (uint8_t*)(&(pImagePacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);
        pImagePacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;
        
        ptr = (imageBlockResponseCmd_t*)(pImagePacket->msgData.dataReq.pMsdu);
        ptr->cmdId  = ImageBlockResponse;
        ptr->status = status;
        
        if (ptr->status == gOtaStatusSuccess_c)
        {
            ptr->manufacturerId = mOtapHeader.manufacturerId;
            ptr->imageType      = mOtapHeader.imageType;
            ptr->fileOffset     = mFileOffset;
            ptr->dataSize       = Mac_GetMaxMsduLength(&pImagePacket->msgData.dataReq);
            if( ptr->dataSize > mDataLen )
                ptr->dataSize = mDataLen;
            ptr->fileVersion    = mOtapHeader.fileVersion;
            FLib_MemCpy((uint8_t*)ptr + sizeof(imageBlockResponseCmd_t), mDataBuffer, ptr->dataSize);
            pImagePacket->msgData.dataReq.msduLength = sizeof(imageBlockResponseCmd_t) + ptr->dataSize;
        }
        else
            pImagePacket->msgData.dataReq.msduLength = sizeof(shortResponseCmd_t);
        
        pImagePacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
        pImagePacket->msgData.dataReq.txOptions |= gMacTxOptionIndirect_c;
        
        pImagePacket->msgData.dataReq.msduHandle = mMsduHandle++;
        
        (void)NWK_MCPS_SapHandler(pImagePacket, macInstance);
        
        if (Address == mCurrentDeviceId)
            mCurrentDeviceId = 0xFFFF;
    }
}

/******************************************************************************
* The following function is called when a End Response must be sent to the
* device.
*
* Return value:
*     None
******************************************************************************/
static void sendUpgradeEndResponse(uint16_t Address)
{
    upgradeEndResponseCmd_t *ptr;
    
    mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + sizeof(upgradeEndResponseCmd_t));
    
    if ( mpPacket != NULL )
    {
        mpPacket->msgType = gMcpsDataReq_c;
        mpPacket->msgData.dataReq.dstAddr = Address;
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr,  &mShortAddress, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mPanId, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mPanId, 2);
        mpPacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.msduLength = sizeof(upgradeEndResponseCmd_t);
        mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);
        
        ptr = (upgradeEndResponseCmd_t*)(mpPacket->msgData.dataReq.pMsdu);
        ptr->cmdId          = UpgradeEndResponse;
        ptr->manufacturerId = mOtapHeader.manufacturerId;
        ptr->imageType      = mOtapHeader.imageType;
        ptr->currentTime    = 0;
        ptr->upgradeTime    = 0;
        ptr->fileVersion    = mOtapHeader.fileVersion;
        
        mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
        mpPacket->msgData.dataReq.txOptions |= gMacTxOptionIndirect_c;
        
        mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
        mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;
        
        (void)NWK_MCPS_SapHandler(mpPacket, macInstance);
    }
}

/******************************************************************************
* SetOtapMode - is called by the PC to establish the transfer mode.
*
******************************************************************************/
otaResult_t SetOtapMode(uint8_t* buffer, uint16_t len)
{
    if ( gUpgradeMode == gUseExternalMemoryForOtaUpdate_c )
    {
#if (gEepromType_d > gEepromDevice_InternalFlash_c)
        KBD_Deinit();
#endif
        OTA_InitExternalMemory();
        mFileOffset = 0;
        gImageOffset = 0;
    }
    
    /* Request necessary information for the image */
    OSA_EventSet(mAppEvent, gAppEvtQueryImageRequestEvent_c);
    
    return gOtaSucess_c;
}

/******************************************************************************
 * QueryImageReq - is called to obtain the Small Image Header
 ******************************************************************************/
otaResult_t QueryImageReqConfirm(uint8_t* buffer, uint16_t len)
{
    uint8_t idx = 0;
    
    /* Skip over the first two fields */
    idx += sizeof(uint8_t) + sizeof(uint16_t);
    
    /* Write basic information in the OtapHeader */
    FLib_MemCpy(&mOtapHeader.manufacturerId, &buffer[idx], sizeof(mOtapHeader.manufacturerId));
    idx += sizeof(mOtapHeader.manufacturerId);
    FLib_MemCpy(&mOtapHeader.imageType,      &buffer[idx], sizeof(mOtapHeader.imageType));
    idx += sizeof(mOtapHeader.imageType);
    FLib_MemCpy(&mOtapHeader.fileVersion,    &buffer[idx], sizeof(mOtapHeader.fileVersion));
    idx += sizeof(mOtapHeader.fileVersion);
    FLib_MemCpy(&mOtapHeader.totalImageSize, &buffer[idx], sizeof(mOtapHeader.totalImageSize));
    
    if( gUpgradeMode == gDoNotUseExternalMemoryForOtaUpdate_c )
    {
        gOtaNewImageAvailable = TRUE;
        OSA_EventSet(mAppEvent, gAppEvtImageNotify_c);
    }
    else
    {
        /* Start requesting image chunks */
        OSA_EventSet(mAppEvent, gAppEvtImageChunkRequestEvent_c);
    }
    
    return gOtaSucess_c;
}

/******************************************************************************
 * ProcessImageChunk - It is called from ZTC to inform the Application that
 * a new image chunk has been received
******************************************************************************/
otaResult_t ProcessImageChunk(uint8_t* buffer, uint16_t len)
{
    if( gUpgradeMode == gUseExternalMemoryForOtaUpdate_c )
    {
        if( gImageOffset + len > mOtapHeader.totalImageSize )
            return gOtaInvalidParam_c;
        
        /* Store the Image Chnh into the External Memory */
        if( OTA_WriteExternalMemory(buffer, len, mFileOffset) == gOtaSucess_c )
        {
            gImageOffset += len;
            mFileOffset  += len;
            
            /* Check if this is the last chunk */
            if( gImageOffset == mOtapHeader.totalImageSize )
            {
                gOtaNewImageAvailable = TRUE;
                OSA_EventSet(mAppEvent, gAppEvtImageNotify_c);
                OTA_CancelImage();
            }
            else
            {
                /* Request next Image Chunk */
                OSA_EventSet(mAppEvent, gAppEvtImageChunkRequestEvent_c);
            }
        }
    }
    else
    {
        /* Send ImageBlockResponse OTA */
        mDataLen = len;
        FLib_MemCpy(mDataBuffer, buffer, len);
        OSA_EventSet(mAppEvent, gAppEvtImageBlockResponseEvent_c);
    }
    
    return gOtaSucess_c;
}