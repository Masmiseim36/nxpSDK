/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* MyStarNetwork Demo Coordinator application.
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
#include "msn_coordinator.h"
#include "Purger.h"

/* Drv */
#include "LED.h"
#include "Keyboard.h"

/* Fwk */
#include "SecLib.h"
#include "RNG_Interface.h"
#include "SerialManager.h"
#include "TimersManager.h"
#include "FunctionLib.h"

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

/* Default size of data payload in MCPS-Data.request.
   The length has no real meaning since MCPS-Data.requests
   are always sent in one-size buffers big enough to hold
   a maximum length data frame. */
#define mDefaultValueOfDataLen_c               20

/* Maximum number of outstanding packets */
#define mMaxPendingDataPackets_c               4

/* Number of time intervals that a packet can remain in the 
   indirect queue. This can be adjusted based on application needs. */
#define mExpireInterval_c                      3

/* Default time interval in miliseconds */
#define mTimeInterval_c                        ( mDefaultValueOfTimeInterval_c * 1000 )

#define gMessageMarkCR_c   0x0D

/* Character that triggers sending of a Device Info Message on the serial terminal interface */
#define gMessageMarkSendDevInfo    0x01

#ifndef mDeviceInfo
#define mDeviceInfo "Kinetis_MKW2x Coordinator"
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static void    App_CommRxCallBack(void*);
static uint8_t App_StartScan(macScanType_t scanType);
static uint8_t App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType);
static uint8_t App_StartCoordinator(void);
static uint8_t App_HandleMlmeInput(nwkMessage_t *pMsg);
static uint8_t App_SendAssociateResponse(nwkMessage_t *pMsgIn);
static void    App_HandleScanEdConfirm(nwkMessage_t *pMsg);
static void    App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn);
static void    App_TransmitData(uint8_t * pData, uint8_t length);
static void    App_CommSendDeviceInfo(void);
static void    App_UpdateLEDs(void);
static void    App_RemoveDevice(uint8_t shortAddrHigh, uint8_t shortAddrLow);
static void    App_IntervalTimeoutHandler(void*);
static void    App_HandleKeys(key_event_t events);

void App_init( void );
void AppThread (uint32_t argument);
resultType_t MLME_NWK_SapHandler (nwkMessage_t* pMsg, instanceId_t instanceId);
resultType_t MCPS_NWK_SapHandler (mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);
extern void Mac_SetExtendedAddress(uint8_t *pAddr, instanceId_t instanceId);


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************** 
 * The following initializations can be modified by the user to
 * better fit the needs, without affecting the way that the app
 * is working. 
 **************************************************************/
 
static const uint16_t mShortAddress = mDefaultValueOfShortAddress_c;
static const uint16_t mPanId = mDefaultValueOfPanId_c;
static tmrTimerID_t mSoftTimerId_c = gTmrInvalidTimerID_c;


/************************************************************** 
 * The following initializations should not be modified, as 
 * they are essential to the correct functioning of the 
 * application. 
 **************************************************************/

/* This is a bit map having on the last 4 bits the OR-ed short addresses
   assigned to end devices associated to the coordinator. 
   The addresses generated are: 1, 2, 4 and 8. When this value is 0x0F,
   no more devices can associate. */
static uint8_t mAddressesMap = 0x00;   

/* Reserved address for the device that is currently associating */
static uint8_t mReservedAddress = 0x00;

/* Data request packet for sending data to the coordinator */
static nwkToMcpsMessage_t *mpPacket = NULL;

/* The MSDU handle is a unique data packet identifier */
static uint8_t mMsduHandle = 0;

/* Number of pending data packets */
static uint8_t mcPendingPackets = 0;

/* Current code to be displayed on the end devices LED. Also
   used as internal time for the application - to track how much
   time a packet remains in the indirect queue. */
static uint8_t mCounterLEDs = 0;

/* Flag signalling that another time interval passed */
static uint8_t mCounterLEDsModified = FALSE;

/* Indication that a time interval passed and the counter was not
   sent to devices. */
static uint8_t mPacketDropped = 0;

/* The current logical channel (frequency band) */
static uint8_t mLogicalChannel;

/* Application input queues */
static anchor_t mMlmeNwkInputQueue;
static anchor_t mMcpsNwkInputQueue;

static const uint64_t mExtendedAddress  = mMacExtendedAddress_c;
static instanceId_t mMacInstance;
static uint8_t mInterfaceId;
osaEventId_t mAppEvent;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/* The current state of the applications state machine */
uint8_t gState;

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
        mMacInstance = BindToMAC( (instanceId_t)0 );
        Mac_RegisterSapHandlers( MCPS_NWK_SapHandler, MLME_NWK_SapHandler, mMacInstance );
        
        App_init();
    }

    /* Call application task */
    AppThread( param );
}

/*****************************************************************************
* Initialization function for the App Task. This is called during
* initialization and should contain any application specific initialization
* (ie. hardware initialization/setup, table initialization, power up
* notification.
*
* Interface assumptions: None
*
* Return value: None
*
*****************************************************************************/
void App_init(void)
{
    mAppEvent = OSA_EventCreate(TRUE);
    /* The initial application state */
    gState = stateInit;    
    /* Initialize the MAC 802.15.4 extended address */
    Mac_SetExtendedAddress( (uint8_t*)&mExtendedAddress, mMacInstance );

    mSoftTimerId_c = TMR_AllocateTimer();

    /* Register keyboard callback function */
    KBD_Init(App_HandleKeys);
    
    /* Initialize the LPM module */
#if (gLpmIncluded_d == 1)    
    PWRLib_Init();
#endif  
    
    /* Initialize the serial terminal interface so that we can print out status messages */
    Serial_InitInterface(&mInterfaceId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    Serial_SetBaudRate(mInterfaceId, gUARTBaudRate57600_c);
    Serial_SetRxCallBack(mInterfaceId, App_CommRxCallBack, NULL);
    
    /* Initialize purger module. */
    Purger_Init( mExpireInterval_c, App_RemoveDevice);   
    
    /* Prepare input queues.*/
    MSG_InitQueue(&mMlmeNwkInputQueue); 
    MSG_InitQueue(&mMcpsNwkInputQueue); 

    /*signal app ready*/  
    LED_StartSerialFlash(LED1);
    
    Serial_Print(mInterfaceId,"\n\rPress any switch on board to start running the application.\n\r", gAllowToBlock_d);  
}

/*****************************************************************************
* The AppTask(event_t events) function is the applicantion main loop and 
* will process any incoming event. Events include timers, messages and any 
* other user defined events.
*
* Interface assumptions:
*     None
*
* Return value: 
*     None
*****************************************************************************/
void AppThread(uint32_t argument)
{ 
    osaEventFlags_t ev;
    /* Pointer for storing the messages from MLME */
    void *pMsgIn;
    /* Stores the error/success code returned by some functions. */
    uint8_t ret;
    static uint8_t maCommDataBuffer[mDefaultValueOfDataLen_c] = {0};
    static uint8_t mCounter = 0;

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
                    Serial_Print(mInterfaceId,"Received an MLME-Beacon Notify Indication\n\r", gAllowToBlock_d);
                }
            }
        }
        switch(gState)
        {
        case stateInit:
            /* Print a welcome message to the serial terminal interface */
            Serial_Print(mInterfaceId,"\n\rMyStarNetwork Demo-Coordinator application is initialized and ready.\n\r\n\r", gAllowToBlock_d);
            /* Goto Energy Detection state. */
            gState = stateScanEdStart;
            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
            break;
            
        case stateScanEdStart:
            /* Start the Energy Detection scan, and goto wait for confirm state. */
            Serial_Print(mInterfaceId,"Initiating the Energy Detection Scan.\n\r", gAllowToBlock_d);
            
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
                        gState = stateStartCoordinator;
                        OSA_EventSet(mAppEvent, gAppEvtStartCoordinator_c);
                    }
                }      
            }        
            break;
            
        case stateStartCoordinator:
            /* Start up as a PAN Coordinator on the selected channel. */
            if (ev & gAppEvtStartCoordinator_c)
            {
                /* Start up as a PAN Coordinator on the selected channel. */
                Serial_Print(mInterfaceId,"\n\rStarting as PAN coordinator on channel 0x", gAllowToBlock_d);
                Serial_PrintHex(mInterfaceId,&mLogicalChannel, 1, FALSE);
                Serial_Print(mInterfaceId,"\n\r", gAllowToBlock_d);
                Serial_Print(mInterfaceId,"PAN Coordinator started.\n\r", gAllowToBlock_d);
                
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
                        Serial_Print(mInterfaceId,"Started the coordinator with PAN ID 0x", gAllowToBlock_d);
                        Serial_PrintHex(mInterfaceId,(uint8_t *)&mPanId, 2, 0);
                        Serial_Print(mInterfaceId,", and short address 0x", gAllowToBlock_d);
                        Serial_PrintHex(mInterfaceId,(uint8_t *)&mShortAddress, 2, 0);
                        Serial_Print(mInterfaceId,".\n\r\n\rReady to send and receive data over the serial terminal interface.\n\r\n\r", gAllowToBlock_d);
                        
                        TMR_StartIntervalTimer( mSoftTimerId_c, mTimeInterval_c, App_IntervalTimeoutHandler, NULL);
                        
                        gState = stateListen;
                        OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                    }
                }
            }      
            break; 
            
        case stateListen:
            /* Stay in this state forever. 
            Transmit to end device data from serial terminal interface */    
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
            
            if (ev & gAppEvtRxFromComm_c)
            {
                uint16_t count;
                
                (void)Serial_GetByteFromRxBuffer(mInterfaceId, &maCommDataBuffer[mCounter], &count);
		if( count && gMessageMarkSendDevInfo == maCommDataBuffer[mCounter] )
                {
                    App_CommSendDeviceInfo();
		}
		else if((mCounter == (mDefaultValueOfDataLen_c-1))||(maCommDataBuffer[mCounter] == gMessageMarkCR_c))
                {
                    App_TransmitData(maCommDataBuffer, mCounter);
                    mCounter = 0;
		}
                else
                {
                    mCounter++;
		}
            }
            
            break;
        } /* end switch*/
        
        /* Free Mlme messages*/
        if (pMsgIn)
        {
            /* Messages must always be freed. */ 
            MSG_Free(pMsgIn);
            pMsgIn = NULL;
        }
        
        /* Handle MCPS confirms and transmit data from serial terminal interface */
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

        if(!gUseRtos_c)
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

/*****************************************************************************
* App_CommRxCallBack
*
* This callback is triggered when a new byte is received over the serial terminal interface
*
*****************************************************************************/
static void App_CommRxCallBack(void *pData) 
{
    uint8_t pressedKey;
    uint16_t count;
    
    (void)pData;
    if(stateListen == gState)
    {
        OSA_EventSet(mAppEvent, gAppEvtRxFromComm_c);
    }
    else
    {
        do{
            (void)Serial_GetByteFromRxBuffer(mInterfaceId, &pressedKey, &count);
            if(count && gMessageMarkSendDevInfo == pressedKey)
            {
                App_CommSendDeviceInfo();
            }
        }while(count);
    }
}

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

  Serial_Print(mInterfaceId,"Sending the MLME-Scan Request message to the MAC... ", gAllowToBlock_d);

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
    /* ChannelsToScan */
#ifdef gPHY_802_15_4g_d
    pScanReq->channelPage = gChannelPageId9_c;
    pScanReq->scanChannels[0] = mDefaultValueOfChannel_c;
    FLib_MemSet(pScanReq->scanChannels+1, 0, sizeof(channelMask_t)-sizeof(uint32_t));
#else
	pScanReq->scanChannels = mDefaultValueOfChannel_c;
#endif
    /* Duration per channel 0-14 (dc). T[sec] = (16*960*((2^dc)+1))/1000000.
    A scan duration of 5 on 16 channels approximately takes 8 secs. */
    pScanReq->scanDuration = 5;
    pScanReq->securityLevel = gMacSecurityNone_c;

    /* Send the Scan request to the MLME. */
    if(NWK_MLME_SapHandler( pMsg, mMacInstance ) == gSuccess_c)
    {
      Serial_Print(mInterfaceId,"Done.\n\r", gAllowToBlock_d);
      return errorNoError;
    }
    else
    {
      Serial_Print(mInterfaceId,"Invalid parameter.\n\r", gAllowToBlock_d);
      return errorInvalidParameter;
    }
  }
  else
  {
    /* Allocation of a message buffer failed. */
    Serial_Print(mInterfaceId,"Message allocation failed.\n\r", gAllowToBlock_d);
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
  uint32_t n;
  uint8_t *pEdList;
  uint8_t minEnergy; 
  uint8_t idx;
#ifndef gPHY_802_15_4g_d
  uint8_t ChannelMask;
#endif

  Serial_Print(mInterfaceId,"Received the MLME-Scan Confirm message from the MAC.\n\r", gAllowToBlock_d);

  /* Get a pointer to the energy detect results */
  pEdList = pMsg->msgData.scanCnf.resList.pEnergyDetectList;

  /* Set the minimum energy to a large value */
  minEnergy = 0xFF;

#ifdef gPHY_802_15_4g_d
  /* Select default channel */
  mLogicalChannel = 0;
  
  /* Search for the channel with least energy */
  for(idx=0, n=0; n<mDefaultMaxChannel_c; n++)
  {
    if( mDefaultValueOfChannel_c & (1 << n) )
    {
      if( pEdList[idx] < minEnergy )
      {
        minEnergy = pEdList[idx];
        mLogicalChannel = n;
      }
      idx++;
    }
  }
#else  
  /* Select default channel */
  mLogicalChannel = 11;

  /* Search for the channel with least energy */
  for(idx=0, n=0; n<16; n++)
  {
    ChannelMask = n + 11;
    if((uint8_t)((mDefaultValueOfChannel_c>>ChannelMask) & 0x1))
    { 
      if( pEdList[idx] < minEnergy )
      {
        minEnergy = pEdList[idx];
        /* Channel numbering is 11 to 26 both inclusive */
        mLogicalChannel = n + 11;       
      }
      idx++;
    }
  }
#endif /* gPHY_802_15_4g_d */

  /* Print out the result of the ED scan */
  Serial_Print(mInterfaceId,"ED scan returned the following results:\n\r  [ ", gAllowToBlock_d);
#ifdef gPHY_802_15_4g_d  
  Serial_PrintHex(mInterfaceId,pEdList, mDefaultMaxChannel_c, gPrtHexBigEndian_c | gPrtHexSpaces_c);
#else
  Serial_PrintHex(mInterfaceId,pEdList, 16, gPrtHexBigEndian_c | gPrtHexSpaces_c);
#endif
  Serial_Print(mInterfaceId,"].\n\r\n\r", gAllowToBlock_d);

  /* Print out the selected logical channel */
  Serial_Print(mInterfaceId,"Based on the ED scan the logical channel 0x", gAllowToBlock_d);
  Serial_PrintHex(mInterfaceId,&mLogicalChannel, 1, 0);
  Serial_Print(mInterfaceId," was selected.\n\r", gAllowToBlock_d);

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

  Serial_Print(mInterfaceId,"Sending the MLME-Start Request message to the MAC ", gAllowToBlock_d);
  /* Allocate a message for the MLME (We should check for NULL). */
  pMsg = MSG_AllocType(mlmeMessage_t);
  if(pMsg != NULL)
  {
    /* Pointer which is used for easy access inside the allocated message */
    mlmeStartReq_t *pStartReq;
    /* Return value from MSG_send - used for avoiding compiler warnings */
    uint8_t ret = 0;
    /* Boolean value that will be written to the MAC PIB */
    uint8_t boolFlag;

    (void) ret;     // remove compiler warning
    /* Set-up MAC PIB attributes. Please note that Set, Get, and Reset 
       messages are not freed by the MLME. We must always set the short 
       address to something else than 0xFFFF before starting a PAN. 
    */
    pMsg->msgType = gMlmeSetReq_c;
    pMsg->msgData.setReq.pibAttribute = gMPibShortAddress_c;
    pMsg->msgData.setReq.pibAttributeValue = (uint8_t *)&mShortAddress;
    ret = NWK_MLME_SapHandler( pMsg, mMacInstance );

    /* We must set the Association Permit flag to TRUE 
    in order to allow devices to associate to us. */
    pMsg->msgType = gMlmeSetReq_c;
    pMsg->msgData.setReq.pibAttribute = gMPibAssociationPermit_c;
    boolFlag = TRUE;
    pMsg->msgData.setReq.pibAttributeValue = &boolFlag;
    ret = NWK_MLME_SapHandler( pMsg, mMacInstance );

    /* This is a MLME-START.req command */
    pMsg->msgType = gMlmeStartReq_c;
    /* Create the Start request message data. */
    pStartReq = &pMsg->msgData.startReq;
    /* PAN ID - LSB, MSB. The demo shows a PAN ID of 0xAAAA. */
    FLib_MemCpy(&pStartReq->panId, (void*)&mPanId, 2);
    /* Logical Channel - the default of 11 will be overridden */
    pStartReq->logicalChannel = mLogicalChannel;
#ifdef gPHY_802_15_4g_d
    pStartReq->channelPage = gChannelPageId9_c;
#endif
    
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
    if(NWK_MLME_SapHandler( pMsg, mMacInstance ) == gSuccess_c)
    {
      Serial_Print(mInterfaceId,"Done.\n\r", gAllowToBlock_d);
      return errorNoError;
    }
    else
    {
      /* One or more parameters in the Start Request message were invalid. */
      Serial_Print(mInterfaceId,"Invalid parameter.\n\r", gAllowToBlock_d);
      return errorInvalidParameter;
    }
  }
  else
  {
    /* Allocation of a message buffer failed. */
    Serial_Print(mInterfaceId,"Message allocation failed.\n\r", gAllowToBlock_d);
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
  uint8_t selectedAddress = 0x01;
  resultType_t requestResolution = gSuccess_c;

  if(mReservedAddress)
  {
    Serial_Print(mInterfaceId,"Busy associating another device. Ignoring MLME-Associate Indication.\n\r", gAllowToBlock_d);
    return errorNoError;
  }

  Serial_Print(mInterfaceId,"Sending the MLME-Associate Response message to the MAC ", gAllowToBlock_d);
  /* Allocate a message for the MLME */
  pMsg = MSG_AllocType(mlmeMessage_t);
  if(pMsg != NULL)
  {
    /* This is a MLME-ASSOCIATE.res command */
    pMsg->msgType = gMlmeAssociateRes_c;

    /* Create the Associate response message data. */
    pAssocRes = &pMsg->msgData.associateRes;

    /* See if we still have space. */
    if(0x0F > mAddressesMap)
    {
      /* We can assign 1, 2, 4 and 8 as a short address. Check the map and determine
      the first free address. */
      while((selectedAddress & mAddressesMap) != 0)
      {
        selectedAddress = selectedAddress << 1;
      }

      pAssocRes->assocShortAddress = selectedAddress;

      /* Association granted.*/      
      requestResolution = gSuccess_c;
    }
    else
    {
      /* Signal that we do not have a valid short address. */
      pAssocRes->assocShortAddress = 0xFFFE;
      requestResolution = gPanAtCapacity_c;
    }

    /* Association  resolution. */
    pAssocRes->status = requestResolution;

    /* Do not use security */
    pAssocRes->securityLevel = gMacSecurityNone_c;

    /* Get the 64 bit address of the device requesting association. */
    FLib_MemCpy(&pAssocRes->deviceAddress, &pMsgIn->msgData.associateInd.deviceAddress, 8);

    /* Send the Associate Response to the MLME. */
    if(NWK_MLME_SapHandler( pMsg, mMacInstance ) == gSuccess_c)
    {
      if(gSuccess_c == requestResolution)
      {
        mReservedAddress |= selectedAddress;
        Serial_Print(mInterfaceId,"Done.\n\r", gAllowToBlock_d);
      }
      else
      {
        Serial_Print(mInterfaceId,"PAN at capacity.\n\r", gAllowToBlock_d);
      }
      return errorNoError;
    }
    else
    {
    /* One or more parameters in the message were invalid. */
    Serial_Print(mInterfaceId,"Invalid parameter.\n\r", gAllowToBlock_d);
    return errorInvalidParameter;
    }
  }
  else
  {
    /* Allocation of a message buffer failed. */
    Serial_Print(mInterfaceId,"Message allocation failed.\n\r", gAllowToBlock_d);
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
  {    
    return errorNoMessage;
  }

  /* Handle the incoming message. The type determines the sort of processing.*/
  switch(pMsg->msgType) 
  {
  case gMlmeAssociateInd_c:
    Serial_Print(mInterfaceId,"Received an MLME-Associate Indication from the MAC.\n\r", gAllowToBlock_d);
    /* A device sent us an Associate Request. We must send back a response.  */
    return App_SendAssociateResponse(pMsg);
    break;

  case gMlmeCommStatusInd_c:
    /* Sent by the MLME after the Association Response has been transmitted. */
    Serial_Print(mInterfaceId,"Received an MLME-Comm-Status Indication from the MAC.\n\r", gAllowToBlock_d);
	switch(pMsg->msgData.commStatusInd.status)
	{
	case gSuccess_c:
	  /*Device successfully associated. Storing reserved address.*/
      Serial_Print(mInterfaceId,"Device associated with address 00", gAllowToBlock_d);
      Serial_PrintHex(mInterfaceId,&mReservedAddress, 1, 0);
      Serial_Print(mInterfaceId,".\n\r", gAllowToBlock_d);
      mAddressesMap |= mReservedAddress;
      mReservedAddress = 0x00;
      App_UpdateLEDs();
      break;
      
	case gTransactionExpired_c:
	  /*Association response expired. Device not associated. Dropping reserved address.*/
	  Serial_Print(mInterfaceId,"Association response expired.\n\r", gAllowToBlock_d);
      Serial_Print(mInterfaceId,"Device not associated.\n\r", gAllowToBlock_d);
	  mReservedAddress = 0x00;
	  break;
	  
	case  gNoAck_c:
	  /*ACK not received for Association Response. Device not associated. Dropping reserved address.*/
	  Serial_Print(mInterfaceId,"ACK not received for Association Response.\n\r", gAllowToBlock_d);
	  Serial_Print(mInterfaceId,"Device not associated.\n\r", gAllowToBlock_d);
	  mReservedAddress = 0x00;
	  break;
	  
	default:
	  Serial_Print(mInterfaceId,"Unknown error occured.\n\r", gAllowToBlock_d);
	  Serial_Print(mInterfaceId,"Device not associated.\n\r", gAllowToBlock_d);
	  mReservedAddress = 0x00;
	}
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
  uint8_t ret = 0;
  
  (void) ret;       // remove compiler warning
  switch(pMsgIn->msgType)
  {
    /* The MCPS-Data confirm is sent by the MAC to the network 
    or application layer when data has been sent. */
    case gMcpsDataCnf_c:
      if(mcPendingPackets)
      {        
        mcPendingPackets--;
      }
      /* If successful remove the packet from the purger tracklist or else we let the purger disassociate the end device. */
      if(pMsgIn->msgData.dataCnf.status == gSuccess_c)
          ret = Purger_Remove(pMsgIn->msgData.dataCnf.msduHandle);
      break;

    case gMcpsDataInd_c: //MyStarNetwork
      /* The MCPS-Data indication is sent by the MAC to the network 
      or application layer when data has been received. We simply 
      format the message and copy it to the serial terminal interface. */
    	
      /* First we check to see what type of message has been received
       * A standard message from a key-press or
       * a message with sensor data.
       * The sensor data message starts wita a 0xFF.
       */
      Serial_Print(mInterfaceId,"\n\rDevice address: ", gAllowToBlock_d);
      Serial_PrintHex(mInterfaceId,(uint8_t*)&pMsgIn->msgData.dataInd.srcAddr, 2, 0);
      
      if (pMsgIn->msgData.dataInd.pMsdu[0] == 0xFF)
      {
        // Sensor data message
    	  Serial_Print(mInterfaceId," Ctrl info:", gNoBlock_d);      
    	  Serial_PrintHex(mInterfaceId,(uint8_t*)&pMsgIn->msgData.dataInd.pMsdu[1], 1, 0);
    	  
    	  Serial_Print(mInterfaceId," On/Off:", gNoBlock_d);      
    	  Serial_PrintHex(mInterfaceId,(uint8_t*)&pMsgIn->msgData.dataInd.pMsdu[2], 1, 0);
    	  
    	  Serial_Print(mInterfaceId," Temperature:", gNoBlock_d);
    	  Serial_PrintHex(mInterfaceId,(uint8_t*)&pMsgIn->msgData.dataInd.pMsdu[3], 1, 0);
    	  
    	  Serial_Print(mInterfaceId," Accelerometer:", gNoBlock_d);
    	  Serial_PrintHex(mInterfaceId,(uint8_t*)&pMsgIn->msgData.dataInd.pMsdu[4], 3, gPrtHexCommas_c);
    	  
    	  Serial_Print(mInterfaceId," Pressure:", gNoBlock_d);
    	  Serial_PrintHex(mInterfaceId,(uint8_t*)&pMsgIn->msgData.dataInd.pMsdu[7], 1, 0);
    	  
    	  Serial_Print(mInterfaceId," Battery level:", gNoBlock_d);
    	  Serial_PrintHex(mInterfaceId,(uint8_t*)&pMsgIn->msgData.dataInd.pMsdu[8], 1, 0);
      }
      else
      {
    	// Keyboard triggered standard message from ED
    	Serial_Print(mInterfaceId,":", gAllowToBlock_d);
    	Serial_PrintHex(mInterfaceId,(uint8_t*)&pMsgIn->msgData.dataInd.mpduLinkQuality, 1, 0);
        Serial_Print(mInterfaceId,":", gAllowToBlock_d);
        Serial_SyncWrite(mInterfaceId, pMsgIn->msgData.dataInd.pMsdu, pMsgIn->msgData.dataInd.msduLength);
      }
      break;

    case gMcpsPurgeCnf_c:
      /* We have confirmation that the packet was removed from the indirect queue,
      so decrement the number of pending packets. */
      if(mcPendingPackets)
      mcPendingPackets--;
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
  {    
    return errorNoMessage;
  }
  /* Is it the expected message type? If not then exit with error code */
  if(pMsg->msgType != msgType)
  {    
    return errorWrongConfirm;
  }

  /* Found the expected message. Return with success code */
  return errorNoError;
}

/******************************************************************************
* The App_TransmitData() function will perform data transmissions of data 
* received by the serial terminal interface and the counter to be displayed on the board LED's.Data 
* could also come from other sources such as sensors etc. This is completely
* determined by the application. The constant mMaxPendingDataPackets_c determine 
* the maximum number of packets pending for transmission in the MAC. A global 
* variable is incremented each time a data packet is sent to the MCPS, and 
* decremented when the corresponding MCPS-Data Confirm message is received. We
* set this constant to the maximum number of devices supported by this aplication
* and transmit a packet to all connected devices at the same time. Because of 
* this, we send a packet only when the number of pending packets is zero.
*
* The function uses the address map, that was updated when a device associates,
* to build MCPS-Data Request messages, one for every associated device. The 
* messages are sent to the MCPS service access point in the MAC.
******************************************************************************/
static void App_TransmitData(uint8_t * pData, uint8_t length)
{
  uint8_t morePackets = 1;
  uint8_t deviceAddress = 0x01;
  uint8_t ret = 0;

  (void) ret;       // remove compiler warning
  /* We transmit only if at least one device is associated. */
  if(mAddressesMap == 0)
  {    
    return;
  }

  /* Send packets only if we can send info to all End Devices */
  if(mcPendingPackets > 0)
  {
    if(mPacketDropped > 0) 
    {
      Serial_Print(mInterfaceId,"Packet dropped.\n\r", gAllowToBlock_d);
      mPacketDropped = 0;
    }
    return;
  }

  if(mCounterLEDsModified) 
  {
    mCounterLEDsModified = FALSE;
    mPacketDropped = FALSE;
  }
  
  /* For every device associated, if there is still room in the queue
  allocate and send a packet */
  if(length != 0) 
  {
    /* Find the first associated end device. We have at least one.*/
    while((deviceAddress & mAddressesMap) == 0)
    {
      deviceAddress = deviceAddress << 1;
    }
    /* Transmit packets to the devices */
    do
    {
      morePackets = 0;
      if (mcPendingPackets < mMaxPendingDataPackets_c && NULL == mpPacket)
      {        
        mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + gMaxPHYPacketSize_c);
        
        if (mpPacket != NULL)
        {
          /* Create an MCPS-Data Request message containing the data. */
          mpPacket->msgType = gMcpsDataReq_c;
          /* Copy data to be sent to packet */
          mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);
          FLib_MemCpy(mpPacket->msgData.dataReq.pMsdu, (void *)pData, length);
          /* Create the header using device information stored when creating 
          the association response. In this simple example the use of short
          addresses is hardcoded. In a real world application we must be
          flexible, and use the address mode required by the given situation. */
          mpPacket->msgData.dataReq.dstAddr = deviceAddress;   

          FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr,  (void*)&mShortAddress, 2);
          FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, (void*)&mPanId, 2);
          FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, (void*)&mPanId, 2);
          mpPacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
          mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
          mpPacket->msgData.dataReq.msduLength = length;
          /* Request MAC level acknowledgement, and 
          indirect transmission of the data packet */
          mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
          mpPacket->msgData.dataReq.txOptions |= gMacTxOptionIndirect_c;
          /* Give the data packet a handle. The handle is
          returned in the MCPS-Data Confirm message. */
          mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
          mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;

          /* Add the packet to tracking list in the purger module. */
          ret = Purger_Track(mpPacket->msgData.dataReq.msduHandle, 0, deviceAddress, mCounterLEDs);          
          /* Send the Data Request to the MCPS */
          (void)NWK_MCPS_SapHandler(mpPacket, mMacInstance);
          /* Prepare for another data buffer */
          mpPacket = NULL;
          mcPendingPackets++;
          /* Move to next associated device */
          do
          {
            deviceAddress = deviceAddress << 1;
          }
          while(((deviceAddress & mAddressesMap) == 0) && (deviceAddress < 0x10));
          
          /* Continue if we haven't passed the last possible device address. */
          morePackets = deviceAddress < 0x10;
        }
        else
        {
          Serial_Print(mInterfaceId,"Packet NULL.\n\r", gAllowToBlock_d);
        }
      }
    }
    while (morePackets);
  }
}

/******************************************************************************
* This function will send device info on serial terminal interface
******************************************************************************/
static void App_CommSendDeviceInfo(void)
{
	/* Print device info */
	Serial_Print(mInterfaceId,mDeviceInfo, gAllowToBlock_d);
	Serial_Print(mInterfaceId," Short address:", gAllowToBlock_d);
	Serial_PrintHex(mInterfaceId,(uint8_t*)&mShortAddress, 2, FALSE);
	Serial_Print(mInterfaceId," PAN Id:", gAllowToBlock_d);
	Serial_PrintHex(mInterfaceId,(uint8_t*)&mPanId, 2, FALSE);
	Serial_Print(mInterfaceId," Channel:", gAllowToBlock_d);
	Serial_PrintHex(mInterfaceId,&mLogicalChannel, 1, FALSE);
}

/******************************************************************************
* This function will remove the device specified by it's short address from the 
* map of associated devices. It is called from the purger module, after a 
* pachet addressed to the specified device was not requested in proper time.
******************************************************************************/
static void App_RemoveDevice(uint8_t shortAddrHigh, uint8_t shortAddrLow)
{  
  /* Variable shortAddrHigh not used in this release*/
  uint8_t addHigh = shortAddrHigh; 

  (void) addHigh;        // remove compiler warning
  /* Remove the end device from the associated devices map - perhaps this should be a call
  to an app privided fn.*/
  if(mAddressesMap & shortAddrLow) /* Remove only if present to avoid multiple serial terminal interface messages */
  {
    mAddressesMap &= ~(shortAddrLow);
    App_UpdateLEDs();
    Serial_Print(mInterfaceId,"\n\rDisconnected device: ", gAllowToBlock_d);
    Serial_PrintHex(mInterfaceId,&shortAddrLow, 1, 0);
    Serial_Print(mInterfaceId,".\n\r", gAllowToBlock_d); 
  }
}
  
/******************************************************************************
* This function will display the lower 4 bits of mAddressesMap, representing 
* associated devices, on the boards's LEDS.
******************************************************************************/
static void App_UpdateLEDs(void)
{
  if(mAddressesMap & 0x01) 
      Led1On() ;
    else Led1Off();
  if(mAddressesMap & 0x02) 
      Led2On();
    else Led2Off();
  if(mAddressesMap & 0x04) 
      Led3On();
    else Led3Off();
  if(mAddressesMap & 0x08) 
      Led4On();
    else Led4Off();
}

/******************************************************************************
* The App_IntervalTimeoutHandler(uint8_t timerID) function is the timer callback
* function. It is called by the timer module after the programmed interval has
* finished.
*
******************************************************************************/
static void App_IntervalTimeoutHandler(void *pData)
{
  uint8_t CounterLed;
  (void)pData;
  mCounterLEDs++;
  /* Was last value sent to devices? */
  if(mCounterLEDsModified)
  {        
    mPacketDropped = TRUE;
  }
  mCounterLEDsModified = TRUE; 
  (void)Purger_Check(mCounterLEDs, mMacInstance, mInterfaceId);
    
  CounterLed = mCounterLEDs & 0x0F;
    
  App_TransmitData(&CounterLed, 1);      

}

/*****************************************************************************
* Function to handle a generic key press. Called for all keys.
*****************************************************************************/
static void App_HandleGenericKey(void)
{
      if(gState == stateInit)
      {
        LED_StopFlashingAllLeds();

        OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);        
      }
}

/*****************************************************************************
* The App_HandleKeys(key_event_t events) function can handle different
* key events. It waits for user to push a button in order to start 
* the application.
*
* Interface assumptions:
*     None
*
* Return value: 
*     None
*****************************************************************************/
static void App_HandleKeys( key_event_t events )
{
#if gKBD_KeysCount_c > 0 
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
        App_HandleGenericKey();
        break;
    default:
        break;  
    }
#endif
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
