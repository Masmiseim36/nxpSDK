/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* OTA Programming Demo Client application.
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
#include "otap_client.h"
#include "OtaMessages.h"

/* Drv */
#include "LED.h"
#include "Keyboard.h"
#include "Eeprom.h"

/* Fwk */
#include "SecLib.h"
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

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static void    App_CommRxCallBack(void *param);
static uint8_t App_StartScan(macScanType_t scanType);
static uint8_t App_HandleScanActiveConfirm(nwkMessage_t *pMsg);
static uint8_t App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType);
static uint8_t App_SendAssociateRequest(void);
static uint8_t App_HandleAssociateConfirm(nwkMessage_t *pMsg);
static uint8_t App_HandleMlmeInput(nwkMessage_t *pMsg);
static void    App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn);
static void    AppPollWaitTimeout(void *param);
static void    App_HandleKeys( key_event_t event );

static void ProcessMessage(mcpsToNwkMessage_t *pMsgIn);
static void sendQueryNextImageRequest(void);
static void sendImageBlockRequest(void);
static void sendUpgradeEndRequest(void);
static void OtaTimeoutCallback(void *param);
static void OtaWaitTimeoutCallback(void *param);

void App_init (void);
void AppThread (uint32_t argument);
resultType_t MLME_NWK_SapHandler (nwkMessage_t* pMsg, instanceId_t instanceId);
resultType_t MCPS_NWK_SapHandler (mcpsToNwkMessage_t* pMsg, instanceId_t instanceId);

extern void Mac_SetExtendedAddress(uint8_t *pAddr, instanceId_t instanceId);
extern void ResetMCU(void);

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

/* Information about the PAN we are part of */
static panDescriptor_t mCoordInfo;

static uint64_t mExtendedAddress  = mMacExtendedAddress_c;
/* This is either the short address assigned by the PAN coordinator
   during association, or our own extended MAC address. */
static uint8_t maMyAddress[8];
/* The devices address mode. If 2, then maMyAddress contains the short
   address assigned by the PAN coordinator. If 3, then maMyAddress is
   equal to the extended address. */
static addrModeType_t mAddrMode;

/* Data request packet for sending data to the coordinator */
static nwkToMcpsMessage_t *mpPacket;
/* The MSDU handle is a unique data packet identifier */
static uint8_t mMsduHandle;

/* Signals that an MLME-Poll request is pending, and that we must wait for
   the MLME-Poll confirm message before sending the next poll request. */
static bool_t mWaitPollConfirm;
/* Time between MLME-Poll requests */
static uint16_t mPollInterval;

/* Application input queues */
anchor_t mMlmeNwkInputQueue;
anchor_t mMcpsNwkInputQueue;

static tmrTimerID_t mTimer_c = gTmrInvalidTimerID_c;

/* OTAP variables */
static fullImageHeader_t mOtapHeader;
static tmrTimerID_t  mOtapTimer = gTmrInvalidTimerID_c;
static tmrTimerID_t  mOtapWaitTimer = gTmrInvalidTimerID_c;
static uint32_t      mFileOffset;
static otaStatus_t   mOtapStatus;
static uint8_t       mBitmapSector[gBootData_SectorsBitmap_Size_c];
static bool_t        mClearToSend = TRUE;
static uint32_t      mReceivedCRC;
static uint16_t      mComputedCRC;

static instanceId_t macInstance;
static uint8_t      interfaceId;
osaEventId_t        mAppEvent;

/* The current state of the applications state machine */
static uint8_t gState;

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
extern  uint8_t *MacPhyInit_ReadExtAddress(void);
void DeepSleepWakeupStackProc(void);

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
    /* Allow sending a poll request */
    mWaitPollConfirm = FALSE;

    /* Initialize the poll interval */
    mPollInterval = mDefaultValueOfPollIntervalSlow_c;

    /* Initialize the MAC 802.15.4 extended address */
    Mac_SetExtendedAddress( (uint8_t*)&mExtendedAddress, macInstance );
    
    mTimer_c = TMR_AllocateTimer();
    mOtapWaitTimer = TMR_AllocateTimer();
    mOtapTimer = TMR_AllocateTimer();
    /* register keyboard callback function */
    KBD_Init(App_HandleKeys);
    
    /* Initialize the UART so that we can print out status messages */
    Serial_InitInterface(&interfaceId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    Serial_SetBaudRate(interfaceId, gUARTBaudRate115200_c);
    Serial_SetRxCallBack(interfaceId, App_CommRxCallBack, NULL);
    
    /* Prepare input queues.*/
    MSG_InitQueue( &mMlmeNwkInputQueue );
    MSG_InitQueue( &mMcpsNwkInputQueue );
    
    /*signal app ready*/
    LED_StartSerialFlash(LED1);
    
    Serial_Print(interfaceId, "\n\rPress any switch on board to start running the application.\n\r", gAllowToBlock_d);  
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
void AppThread(uint32_t argument)
{
    osaEventFlags_t ev;
    /* Pointer for storing the messages from MLME, MCPS, and ASP. */
    void *pMsgIn;
    /* Stores the status code returned by some functions. */
    uint8_t rc;
    
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
                rc = App_WaitMsg(pMsgIn, gMlmeBeaconNotifyInd_c);
                if(rc == errorNoError)
                {
                    /* ALWAYS free the beacon frame contained in the beacon notify indication.*/
                    /* ALSO the application can use the beacon payload.*/
                    MSG_Free(((nwkMessage_t *)pMsgIn)->msgData.beaconNotifyInd.pBufferRoot);
                    ((nwkMessage_t *)pMsgIn)->msgData.beaconNotifyInd.pBufferRoot = NULL;
                    Serial_Print(interfaceId,"Received an MLME-Beacon Notify Indication\n\r", gAllowToBlock_d);
                }
            }
        }
        
        /* The application state machine */
        switch(gState)
        {
        case stateInit:
            /* Print a welcome message to the serial terminal interface */
            Serial_Print(interfaceId,"\n\rOTA Programming Demo Client is initialized and ready.\n\r\n\r", gAllowToBlock_d);
            /* Goto Active Scan state. */
            gState = stateScanActiveStart;
            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
            break;
            
        case stateScanActiveStart:
            /* Start the Active scan, and goto wait for confirm state. */
            Serial_Print(interfaceId,"Start scanning for a PAN coordinator\n\r", gAllowToBlock_d);
            rc = App_StartScan(gScanModeActive_c);
            if(rc == errorNoError)
            {
                gState = stateScanActiveWaitConfirm;
            }
            break;
            
        case stateScanActiveWaitConfirm:
            /* Stay in this state until the Scan confirm message
            arrives, and then goto the associate state. */
            if (ev & gAppEvtMessageFromMLME_c)
            {
                if (pMsgIn)
                {
                    rc = App_WaitMsg(pMsgIn, gMlmeScanCnf_c);
                    if(rc == errorNoError)
                    {
                        rc = App_HandleScanActiveConfirm(pMsgIn);
                        if(rc == errorNoError)
                        {
                            Serial_Print(interfaceId,"Found a coordinator with the following properties:\n\r", gAllowToBlock_d);
                            Serial_Print(interfaceId,"--------------------------------------------------", gAllowToBlock_d);
                            Serial_Print(interfaceId,"\n\rAddress..........0x", gAllowToBlock_d); Serial_PrintHex(interfaceId,(uint8_t*)&mCoordInfo.coordAddress, mCoordInfo.coordAddrMode == gAddrModeShortAddress_c ? 2 : 8, 0);
                            Serial_Print(interfaceId,"\n\rPAN ID...........0x", gAllowToBlock_d); Serial_PrintHex(interfaceId,(uint8_t*)&mCoordInfo.coordPanId, 2, 0);
                            Serial_Print(interfaceId,"\n\rLogical Channel..0x", gAllowToBlock_d); Serial_PrintHex(interfaceId,(uint8_t*)&mCoordInfo.logicalChannel, 1, 0);
                            Serial_Print(interfaceId,"\n\rBeacon Spec......0x", gAllowToBlock_d); Serial_PrintHex(interfaceId,(uint8_t*)&mCoordInfo.superframeSpec, 2, 0);
                            Serial_Print(interfaceId,"\n\rLink Quality.....0x", gAllowToBlock_d); Serial_PrintHex(interfaceId,(uint8_t*)&mCoordInfo.linkQuality, 1, 0);
                            Serial_Print(interfaceId,"\n\r\n\r", gAllowToBlock_d);
                            
                            gState = stateAssociate;
                            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                        }
                        else
                        {
                            Serial_Print(interfaceId,"Scan did not find a suitable coordinator\n\r", gAllowToBlock_d);
                        }
                    }
                }
            }
            break;
            
        case stateAssociate:
            /* Associate to the PAN coordinator */
            Serial_Print(interfaceId,"Associating to PAN coordinator on channel 0x", gAllowToBlock_d);
            Serial_PrintHex(interfaceId,&(mCoordInfo.logicalChannel), 1, gPrtHexNewLine_c);
            rc = App_SendAssociateRequest();
            if(rc == errorNoError)
                gState = stateAssociateWaitConfirm;
            break;
            
        case stateAssociateWaitConfirm:
            /* Stay in this state until the Associate confirm message
            arrives, and then goto the Listen state. */
            if (ev & gAppEvtMessageFromMLME_c)
            {
                if (pMsgIn)
                {
                    rc = App_WaitMsg(pMsgIn, gMlmeAssociateCnf_c);
                    if(rc == errorNoError)
                    {
                        rc = App_HandleAssociateConfirm(pMsgIn);
                        if (rc == errorNoError)
                        {
                            Serial_Print(interfaceId,"Association successful.\n\r", gAllowToBlock_d);
                            Serial_Print(interfaceId,"We were assigned the short address 0x", gAllowToBlock_d);
                            Serial_PrintHex(interfaceId,maMyAddress, mAddrMode == gAddrModeShortAddress_c ? 2 : 8, 0);
                            Serial_Print(interfaceId,"\n\r\n\rPress any switch to query for a new image.\n\r\n\r", gAllowToBlock_d);
                            
                            /* Startup the timer */
                            TMR_StartSingleShotTimer(mTimer_c, mPollInterval, AppPollWaitTimeout, NULL);
                            /* Go to the listen state */
                            gState = stateListen;
                            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                        }
                        else
                        {
                            Serial_Print(interfaceId,"\n\rAssociate Confirm wasn't successful... \n\r\n\r", gAllowToBlock_d);
                            gState = stateScanActiveStart;
                            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                        }
                    }
                }
            }
            break;
            
        case stateListen:
            if (ev & gAppEvtMessageFromMLME_c)
            {
                if (pMsgIn)
                {
                    /* Process it */
                    rc = App_HandleMlmeInput(pMsgIn);
                }
            }
            
            /* Send an ImageRequest to the coordinator */
            if (ev & gAppEvtQueryNextImageRequest_c)
            {
                sendQueryNextImageRequest();
                Serial_Print(interfaceId,"\n\r> Sent Query Next Image req", gAllowToBlock_d);
            }
            
            /* Send an ImageBlockRequest */
            if (ev & gAppEvtImageBlockRequest_c)
            {
                sendImageBlockRequest();
            }
            
            /* Send an UpgradeEndRequest */
            if (ev & gAppEvtUpgradeEndRequest_c)
            {
                sendUpgradeEndRequest();
            }
            
            break;
        }
        
        if (pMsgIn)
        {
            /* Messages must always be freed. */
            MSG_Free(pMsgIn);
            pMsgIn = NULL;
        }
        
        /* Handle received packet from the coordinator */
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

/*****************************************************************************
* App_CommRxCallBack
*
* This callback is triggered when a new byte is received over the serial terminal interface
*
*****************************************************************************/
static void App_CommRxCallBack(void* param)
{}

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

  Serial_Print(interfaceId,"Sending the MLME-Scan Request message to the MAC...", gAllowToBlock_d);

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
    if(NWK_MLME_SapHandler( pMsg, macInstance ) == gSuccess_c)
    {
      Serial_Print(interfaceId,"Done\n\r", gAllowToBlock_d);
      return errorNoError;
    }
    else
    {
      Serial_Print(interfaceId,"Invalid parameter!\n\r", gAllowToBlock_d);
      return errorInvalidParameter;
    }
  }
  else
  {
    /* Allocation of a message buffer failed. */
    Serial_Print(interfaceId,"Allocation failed!\n\r", gAllowToBlock_d);
    return errorAllocFailed;
  }
}

/******************************************************************************
* The App_HandleScanActiveConfirm(nwkMessage_t *pMsg) function will handle the
* Active Scan confirm message received from the MLME when the Active scan has
* completed. The message contains a list of PAN descriptors. Based on link
* quality inforamtion in the pan descriptors the nearest coordinator is chosen.
* The corresponding pan descriptor is stored in the global variable mCoordInfo.
*
* The function may return either of the following values:
*   errorNoError:       A suitable pan descriptor was found.
*   errorNoScanResults: No scan results were present in the confirm message.
*
******************************************************************************/
static uint8_t App_HandleScanActiveConfirm(nwkMessage_t *pMsg)
{
  void    *pBlock;
  uint8_t panDescListSize = pMsg->msgData.scanCnf.resultListSize;
  uint8_t rc = errorNoScanResults;
  uint8_t j;
  uint8_t bestLinkQuality = 0;  
  
  panDescriptorBlock_t *pDescBlock = pMsg->msgData.scanCnf.resList.pPanDescriptorBlockList;  
  panDescriptor_t *pPanDesc;    
  
  /* Check if the scan resulted in any coordinator responses. */  
  if (panDescListSize > 0)
  {    
    /* Check all PAN descriptors. */
    while (NULL != pDescBlock)
    {
      for (j = 0; j < pDescBlock->panDescriptorCount; j++)
      {            
        pPanDesc = &pDescBlock->panDescriptorList[j];

        /* Only attempt to associate if the coordinator
           accepts associations and is non-beacon. */
        if( ( pPanDesc->superframeSpec.associationPermit ) && 
            ( pPanDesc->superframeSpec.beaconOrder == 0x0F) )
        {        
          
          /* Find the nearest coordinator using the link quality measure. */
          if(pPanDesc->linkQuality > bestLinkQuality)
          {
            /* Save the information of the coordinator candidate. If we
               find a better candiate, the information will be replaced. */
            FLib_MemCpy(&mCoordInfo, pPanDesc, sizeof(panDescriptor_t));
            bestLinkQuality = pPanDesc->linkQuality;
            rc = errorNoError;
          }
        }      
      }
      
      /* Free current block */
      pBlock = pDescBlock;
      pDescBlock = pDescBlock->pNext;              
      MSG_Free(pBlock);
    }
  }
  
  if (pDescBlock)
      MSG_Free(pDescBlock);
  
  return rc;
}

/******************************************************************************
* The App_SendAssociateRequest(void) will create an Associate Request message
* and send it to the coordinator it wishes to associate to. The function uses
* information gained about the coordinator during the scan procedure.
*
* The function may return either of the following values:
*   errorNoError:          The Associate Request message was sent successfully.
*   errorInvalidParameter: The MLME service access point rejected the
*                          message due to an invalid parameter.
*   errorAllocFailed:      A message buffer could not be allocated.
*
******************************************************************************/
static uint8_t App_SendAssociateRequest(void)
{
  mlmeMessage_t *pMsg;
  mlmeAssociateReq_t *pAssocReq;

  Serial_Print(interfaceId,"Sending MLME-Associate Request to the MAC...", gAllowToBlock_d);

  /* Allocate a message for the MLME message. */
  pMsg = MSG_AllocType(mlmeMessage_t);
  if(pMsg != NULL)
  {
    /* This is a MLME-ASSOCIATE.req command. */
    pMsg->msgType = gMlmeAssociateReq_c;

    /* Create the Associate request message data. */
    pAssocReq = &pMsg->msgData.associateReq;

    /* Use the coordinator info we got from the Active Scan. */
    FLib_MemCpy(&pAssocReq->coordAddress, &mCoordInfo.coordAddress, 8);
    FLib_MemCpy(&pAssocReq->coordPanId,   &mCoordInfo.coordPanId, 2);
    pAssocReq->coordAddrMode      = mCoordInfo.coordAddrMode;
    pAssocReq->logicalChannel     = mCoordInfo.logicalChannel;
    pAssocReq->securityLevel = gMacSecurityNone_c;

    /* We want the coordinator to assign a short address to us. */
    pAssocReq->capabilityInfo     = gCapInfoAllocAddr_c;

    /* Send the Associate Request to the MLME. */
    if(NWK_MLME_SapHandler( pMsg, macInstance ) == gSuccess_c)
    {
      Serial_Print(interfaceId,"Done\n\r", gAllowToBlock_d);
      return errorNoError;
    }
    else
    {
      /* One or more parameters in the message were invalid. */
      Serial_Print(interfaceId,"Invalid parameter!\n\r", gAllowToBlock_d);
      return errorInvalidParameter;
    }
  }
  else
  {
    /* Allocation of a message buffer failed. */
    Serial_Print(interfaceId,"Message allocation failed!\n\r", gAllowToBlock_d);
    return errorAllocFailed;
  }
}

/******************************************************************************
* The App_HandleAssociateConfirm(nwkMessage_t *pMsg) function will handle the
* Associate confirm message received from the MLME when the Association
* procedure has completed. The message contains the short address that the
* coordinator has assigned to us. This address is 0xfffe if we did not specify
* the gCapInfoAllocAddr_c flag in the capability info field of the Associate
* request. The address and address mode are saved in global variables. They
* will be used in the next demo application when sending data.
*
******************************************************************************/
static uint8_t App_HandleAssociateConfirm(nwkMessage_t *pMsg)
{
  /* If the coordinator assigns a short address of 0xfffe then,
     that means we must use our own extended address in all
     communications with the coordinator. Otherwise, we use
     the short address assigned to us. */
  if ( pMsg->msgData.associateCnf.status == gSuccess_c)
  {

    if( pMsg->msgData.associateCnf.assocShortAddress >= 0xFFFE )
    {
      mAddrMode = gAddrModeExtendedAddress_c;
      FLib_MemCpy(maMyAddress, &mExtendedAddress, 8);
    }
    else
    {
      mAddrMode = gAddrModeShortAddress_c;
      FLib_MemCpy(maMyAddress, &pMsg->msgData.associateCnf.assocShortAddress, 2);
    }
    return gSuccess_c;
  }
  else
  {
    return pMsg->msgData.associateCnf.status;
  }
}

/******************************************************************************
* The App_HandleMlmeInput(nwkMessage_t *pMsg) function will handle various
* messages from the MLME, e.g. poll confirm.
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
  case gMlmePollCnf_c:
    if(pMsg->msgData.pollCnf.status != gSuccess_c)
    {
      /* The Poll Confirm status was not successful. Usually this happens if
         no data was available at the coordinator. In this case we start
         polling at a lower rate to conserve power. */
      mPollInterval = mDefaultValueOfPollIntervalSlow_c;

      /* If we get to this point, then no data was available, and we
         allow a new poll request. Otherwise, we wait for the data
         indication before allowing the next poll request. */
      mWaitPollConfirm = FALSE;
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
  switch(pMsgIn->msgType)
  {
    /* The MCPS-Data confirm is sent by the MAC to the network
       or application layer when data has been sent. */
  case gMcpsDataCnf_c:
    mClearToSend = TRUE;
    break;

  case gMcpsDataInd_c:
    /* Since we received data, the coordinator might have more to send. We
       reduce the polling interval to raise the throughput while data is
       available. */
    mPollInterval = mDefaultValueOfPollIntervalFast_c;

    /* Allow another MLME-Poll request. */
    mWaitPollConfirm = FALSE;

    /* Process the received message */
    ProcessMessage(pMsgIn);
    break;

  default:
    break;
  }
}

/******************************************************************************
* The App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType) function does not, as
* the name implies, wait for a message, thus blocking the execution of the
* state machine. Instead the function analyzes the supplied message to
* determine whether or not the message is of the expected type.
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


/******************************************************************************
* The AppPollWaitTimeout() function will check if it is time to send out an
* MLME-Poll request in order to receive data from the coordinator. If its time,
* and we are permitted then a poll request is created and sent.
*
* The function uses the coordinator information gained during the Active Scan
* for building the MLME-Poll Request message. The message is sent to the MLME
* service access point in the MAC.
******************************************************************************/
static void AppPollWaitTimeout(void *param)
{
  /* Just to avoid the compiler warning */
   (void)param;
   
   TMR_StopTimer(mTimer_c);

  /* Check if we are permitted, and if it is time to send a poll request.
     The poll interval is adjusted dynamically to the current band-width
     requirements. */
  if( mWaitPollConfirm == FALSE )
  {
    /* This is an MLME-POLL.req command. */
    mlmeMessage_t *pMlmeMsg = MSG_AllocType(mlmeMessage_t);

    if( pMlmeMsg )
    {
      /* Create the Poll Request message data. */
      pMlmeMsg->msgType = gMlmePollReq_c;
      /* Use the coordinator information we got from the Active Scan. */
      FLib_MemCpy(&pMlmeMsg->msgData.pollReq.coordAddress, &mCoordInfo.coordAddress, 8);
      FLib_MemCpy(&pMlmeMsg->msgData.pollReq.coordPanId, &mCoordInfo.coordPanId, 2);
      pMlmeMsg->msgData.pollReq.coordAddrMode = mCoordInfo.coordAddrMode;
      pMlmeMsg->msgData.pollReq.securityLevel = gMacSecurityNone_c;

      /* Send the Poll Request to the MLME. */
      if(NWK_MLME_SapHandler( pMlmeMsg, macInstance ) == gSuccess_c)
      {
        /* Do not allow another Poll request before the confirm is received. */
        mWaitPollConfirm = TRUE;
      }
    }
  }
 /* Restart timer. */
 TMR_StartSingleShotTimer(mTimer_c, mPollInterval, AppPollWaitTimeout, NULL);
}

/*****************************************************************************
* Handles all key ev for this device.
* Interface assumptions: None
* Return value: None
*****************************************************************************/
static void App_HandleKeys(key_event_t event)
{
#if gKBD_KeysCount_c > 0 
    switch ( event ) 
    { 
    case gKBD_EventSW1_c:
    case gKBD_EventSW2_c:
    case gKBD_EventSW3_c:
    case gKBD_EventSW4_c:
    case gKBD_EventLongSW1_c:
    case gKBD_EventLongSW2_c:
    case gKBD_EventLongSW3_c:
    case gKBD_EventLongSW4_c:

        if(gState == stateInit)
        {
          LED_StopFlashingAllLeds();
          OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
        } 
        else 
        {
          OSA_EventSet(mAppEvent, gAppEvtQueryNextImageRequest_c);
        }
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

/******************************************************************************
* The following function is called when a new imageBlockRespomse is received.
* The function will print the OTA transfer progress on the Console.
*
* Return value:
*     None
******************************************************************************/
static void PrintProgress()
{
  uint8_t progress = 0, i;
  char progressString[33];

  progress = (uint8_t)((mFileOffset*100)/mOtapHeader.totalImageSize);

  Serial_Print(interfaceId, "\r", gAllowToBlock_d);

  /* Write start of Progress Bar */
  progressString[3] = '%';
  progressString[4] = ' ';
  progressString[5] = '[';

  /* Write Progress Bar to string */
  for ( i=6; i<31; i++ )
    if (i<(progress/4)+6)
      progressString[i] = '=';
    else
      progressString[i] = ' ';

  /* Units */
  progressString[2] = '0' + (progress % 10);
  progress /= 10;

  /* Tens */
  if ( (progress % 10 > 0) || progress==10 )
    progressString[1] = '0' + (progress % 10);
  else
    progressString[1] = ' ';

  /* Hundreds */
  progress /= 10;
  if ( progress % 10 > 0 )
    progressString[0] = '0' + (progress % 10);
  else
    progressString[0] = ' ';

  /* Write end of Progress Bar */
  progressString[31] = ']';
  progressString[32] = '\0';

  Serial_Print(interfaceId, progressString, gAllowToBlock_d);
}

/******************************************************************************
* AbortOtaTransfer() function is called when the OTA transfer needs to be
* stopped.
*
* Return value:
*     None
******************************************************************************/
static void AbortOtaTransfer()
{
  mOtapStatus = gOtaStatusAbort_c;
  OTA_CancelImage();
  OSA_EventSet(mAppEvent, gAppEvtUpgradeEndRequest_c);
}

/******************************************************************************
* The following function is called when a Data Indication is received from MAC.
* The packet is parsed and an event will be sent to the AppTask.
*
* Return value:
*     None
******************************************************************************/
static void ProcessMessage(mcpsToNwkMessage_t *pMsgIn)
{
  static subElement_t Tag;
  static uint32_t elementEnd=0;
  static uint16_t offset = 0;

  switch( pMsgIn->msgData.dataInd.pMsdu[0] ) {

  /* Received an Image Notify command */
  case ImageNotify:
  {
      imageNotifyCmd_t *ptr;
      uint32_t actualFileVer = mActualFileVersion;

      Serial_Print(interfaceId,"\n\r> Received an Image Notify", gAllowToBlock_d);

      ptr = (imageNotifyCmd_t*)(pMsgIn->msgData.dataInd.pMsdu);
      if ( ptr->payloadType == gPayloadQJitter_ManId_ImgType_Version )
      {
          if ( (ptr->manufacturerId != gManufacturerCode_d) ||
               !IsManufacturerSpecificImage(ptr->imageType) ||
               ( ptr->fileVersion == actualFileVer ) )
          {
              Serial_Print(interfaceId,"\n\rUpgrade/Downgrade not necessary!", gAllowToBlock_d);
              return;
          }
      }
      OSA_EventSet(mAppEvent, gAppEvtQueryNextImageRequest_c);
  }
  break;

 /* Received a Query Next Image Response command */
  case QueryNextImageResponse:
  {
      queryNextImageResponseCmd_t *ptr;

      TMR_StopTimer(mOtapTimer);
      ptr = (queryNextImageResponseCmd_t*)(pMsgIn->msgData.dataInd.pMsdu);

      if ( ptr->status == gOtaStatusSuccess_c )
      {
          mOtapHeader.totalImageSize = ptr->imageSize;
          Serial_Print(interfaceId,"\n\rOTA transfer progress:\n\r", gAllowToBlock_d);
          OSA_EventSet(mAppEvent, gAppEvtImageBlockRequest_c);
          mComputedCRC = 0;
          offset = 0;
#if (gEepromType_d > gEepromDevice_InternalFlash_c)
          KBD_Deinit();
#endif
          OTA_InitExternalMemory();
      }
      else if ( ptr->status == gOtaStatusNoImageAvailable_c )
          Serial_Print(interfaceId,"\n\rNo image available!", gAllowToBlock_d);
      else
          Serial_Print(interfaceId,"\n\rInvalid image!", gAllowToBlock_d);
      break;
  }

 /* Received an Image Block Response command */
  case ImageBlockResponse:
  {
    uint32_t len;
    imageBlockResponseCmd_t *ptr;
    uint32_t fOffset;
    uint8_t *pData;

    TMR_StopTimer(mOtapTimer);
    ptr = (imageBlockResponseCmd_t*)(pMsgIn->msgData.dataInd.pMsdu);
    pData = (uint8_t*)ptr + sizeof(imageBlockResponseCmd_t);
    len = ptr->dataSize;

    FLib_MemCpy(&fOffset, &ptr->fileOffset, sizeof(fOffset));

    /* Check if requested offset is the same with the received one */
    if ((ptr->status == gOtaStatusWaitForData_c) || (fOffset != mFileOffset))
    {
        TMR_StartSingleShotTimer(mOtapWaitTimer, gOtaWaitTimeout_c, OtaWaitTimeoutCallback, NULL);
        return;
    }

    mOtapStatus = gOtaStatusSuccess_c;

    /* Check received status */
    if (ptr->status != gOtaStatusSuccess_c)
      {
       Serial_Print(interfaceId,"\n\rServer Error!", gAllowToBlock_d);
       AbortOtaTransfer();
       return;
      }

    /* Get Image Header */
    if ( fOffset < sizeof(fullImageHeader_t) ) {
        if ( fOffset + len > sizeof(fullImageHeader_t) )
        {
    	    len = sizeof(fullImageHeader_t) - fOffset;
        }

    	FLib_MemCpy((uint8_t*)&mOtapHeader + fOffset, pData, len);

        if (fOffset + len == sizeof(fullImageHeader_t))
        {
          mComputedCRC = OTA_CrcCompute((uint8_t*)&mOtapHeader, sizeof(fullImageHeader_t), mComputedCRC);
          elementEnd = sizeof(fullImageHeader_t);
        }
    }

    /* Get SubElements */
    else
    {
      uint8_t idx=0;

      /* Read TagId and PayloadLen */
      if (elementEnd == fOffset)
      {
          FLib_MemCpy(&Tag.tagId, pData, sizeof(Tag.tagId));
          FLib_MemCpy(&Tag.dataLen, &pData[sizeof(Tag.tagId)], sizeof(Tag.dataLen));
          elementEnd += Tag.dataLen + sizeof(subElement_t);
          idx += sizeof(subElement_t);
      }

      if ( fOffset + len > elementEnd )
          len = elementEnd - fOffset;

      switch (Tag.tagId) {
        case gUpgradeImageTadId:
            if (idx)
            {
                /* This is the first image chunk we've received.
                   Provide the image size to the Otap component */
                if ( gOtaSucess_c != OTA_StartImage(Tag.dataLen) )
                {
                    Serial_Print(interfaceId,"\n\rOTAP init error!", gAllowToBlock_d);
                    AbortOtaTransfer();
                    return;
                }
    	    }

    	    mComputedCRC = OTA_CrcCompute(pData, len, mComputedCRC);
            if ( OTA_PushImageChunk(&pData[idx], len-idx, NULL, NULL) != gOtaSucess_c )
            {
                Serial_Print(interfaceId,"\n\rExternal Memory Error!", gAllowToBlock_d);
                AbortOtaTransfer();
                return;
            }
            break;

        case gSectorBitmapTagId:
            mComputedCRC = OTA_CrcCompute(pData, len, mComputedCRC);
    	    FLib_MemCpy(&mBitmapSector[offset], &pData[idx], len-idx);
    	    offset += (len-idx);
            break;

        case gCRCTagId:
            FLib_MemCpy(&mReceivedCRC, &pData[idx], sizeof(mReceivedCRC));
            break;

        default:
            /* Unknown SubElement. Just compute CRC. */
            mComputedCRC = OTA_CrcCompute(pData, len, mComputedCRC);
            break;
      }
    }

    mFileOffset += len;

    /* Print Transfer progress */
    PrintProgress();

    /* Verify if this is the last packet */
    if (mOtapHeader.totalImageSize == mFileOffset)
    {
      uint8_t status;

      /* Commit Image */
      status = OTA_CommitImage(mBitmapSector);
      if ( (status != gOtaSucess_c) || (mComputedCRC != (uint16_t)mReceivedCRC))
      {
        Serial_Print(interfaceId,"\n\rCommit Failed!", gAllowToBlock_d);
        AbortOtaTransfer();
        return;
      }

      OSA_EventSet(mAppEvent, gAppEvtUpgradeEndRequest_c);
      return;
    }

    /* Request next file block */
    OSA_EventSet(mAppEvent, gAppEvtImageBlockRequest_c);
    break;
  }

 /* Received an Upgrade End response command */
    case UpgradeEndResponse:
        if ( mOtapStatus == gOtaStatusSuccess_c )
        {
            Serial_Print(interfaceId,"\n\rTransfer Successful!\n\rResetting MCU...", gAllowToBlock_d);
            OTA_SetNewImageFlag();
            ResetMCU();
        }
        else
        {
            Serial_Print(interfaceId,"\n\rTransfer Failed!", gAllowToBlock_d);
        }
        break;
    }
}

/******************************************************************************
* The following function is called when a Image Request must be sent to the
* coordinator.
*
* Return value:
*     None
******************************************************************************/
static void sendQueryNextImageRequest(void)
{
  queryNextImageRequestCmd_t *ptr;

  if ( !mClearToSend )
  {
    OSA_EventSet(mAppEvent, gAppEvtQueryNextImageRequest_c);
    return;
  }

  mFileOffset = 0;
  mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + sizeof(queryNextImageRequestCmd_t));

  if ( mpPacket != NULL )
  {
    mpPacket->msgType = gMcpsDataReq_c;
    FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);
    FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, maMyAddress, 8);
    FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);
    FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
    mpPacket->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
    mpPacket->msgData.dataReq.srcAddrMode = mAddrMode;
    mpPacket->msgData.dataReq.msduLength  = sizeof(queryNextImageRequestCmd_t);
    mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);

    ptr = (queryNextImageRequestCmd_t*)((mpPacket->msgData.dataReq.pMsdu));
    ptr->cmdId          = QueryNextImageRequest;
    ptr->fieldControl.HwVerPresent = TRUE;
    ptr->manufacturerId = gManufacturerCode_d;
    ptr->imageType      = gUpgradeImageTadId;
    ptr->fileVersion = mActualFileVersion;
    ptr->hwVersion   = (HwVersion_d << 8) | HwRevision_d;

    mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
    mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
    mClearToSend = FALSE;
    mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;

    (void)NWK_MCPS_SapHandler(mpPacket, macInstance);

    TMR_StartSingleShotTimer(mOtapTimer, gOtaTimeout_c, OtaTimeoutCallback, NULL);
  }
  else
    Serial_Print(interfaceId,"\n\rAllocation failed!", gAllowToBlock_d);
}

/******************************************************************************
* The following function is called when a Image Bloch Request must be sent to
* the coordinator.
*
* Return value:
*     None
******************************************************************************/
static void sendImageBlockRequest(void)
{
  imageBlockRequestCmd_t *ptr;

  if ( !mClearToSend )
  {
    OSA_EventSet(mAppEvent, gAppEvtImageBlockRequest_c);
    return;
  }

  mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + sizeof(imageBlockRequestCmd_t));

  if ( mpPacket != NULL )
  {
    mpPacket->msgType = gMcpsDataReq_c;
    FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);
    FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, maMyAddress, 8);
    FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);
    FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
    mpPacket->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
    mpPacket->msgData.dataReq.srcAddrMode = mAddrMode;
    mpPacket->msgData.dataReq.msduLength  = sizeof(imageBlockRequestCmd_t);
    mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);

    ptr = (imageBlockRequestCmd_t*)((mpPacket->msgData.dataReq.pMsdu));
    ptr->cmdId          = ImageBlockRequest;
    ptr->fieldControl.ieeeAddrPresent = FALSE;
    ptr->manufacturerId = gManufacturerCode_d;
    ptr->imageType      = gUpgradeImageTadId;
    ptr->fileVersion    = mOtapHeader.fileVersion;
    ptr->fileOffset     = mFileOffset;
    ptr->maxDataSize    = gImageDataPacketMaxSize_c;

    mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
    mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
    mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;

    (void)NWK_MCPS_SapHandler(mpPacket, macInstance);
    mClearToSend = FALSE;
    
    /* Trigger a MLME-POLL.Request */
    AppPollWaitTimeout(NULL);
    
    TMR_StartSingleShotTimer(mOtapTimer, gOtaTimeout_c, OtaTimeoutCallback, NULL);
  }
  else
    Serial_Print(interfaceId,"\n\rAllocation failed !", gAllowToBlock_d);
}

/******************************************************************************
* The following function is called when a End Request must be sent to the
* coordinator.
*
* Return value:
*     None
******************************************************************************/
static void sendUpgradeEndRequest(void)
{
  upgradeEndRequestCmd_t *ptr;

  if ( !mClearToSend )
  {
    OSA_EventSet(mAppEvent, gAppEvtUpgradeEndRequest_c);
    return;
  }

  mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + sizeof(upgradeEndRequestCmd_t));

  if ( mpPacket != NULL )
  {
    mpPacket->msgType = gMcpsDataReq_c;
    FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);
    FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, maMyAddress, 8);
    FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);
    FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
    mpPacket->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
    mpPacket->msgData.dataReq.srcAddrMode = mAddrMode;
    mpPacket->msgData.dataReq.msduLength  = sizeof(upgradeEndRequestCmd_t);
    mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&(mpPacket->msgData.dataReq.pMsdu)) + sizeof(uint8_t*);

    ptr = (upgradeEndRequestCmd_t*)(mpPacket->msgData.dataReq.pMsdu);
    ptr->cmdId          = UpgradeEndRequest;
    ptr->status         = mOtapStatus;
    ptr->manufacturerId = gManufacturerCode_d;;
    ptr->imageType      = gUpgradeImageTadId;
    ptr->fileVersion    = mOtapHeader.fileVersion;

    mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
    mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
    mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;

    (void)NWK_MCPS_SapHandler(mpPacket, macInstance);
    mClearToSend = FALSE;
  }
  else
    Serial_Print(interfaceId,"\n\rAllocation failed !", gAllowToBlock_d);
}

/******************************************************************************
* OtaTimeoutCallback - is called if the server is not responding more then
* gOtaTimeout_c ms.
*
******************************************************************************/
static void OtaTimeoutCallback(void *param)
{
  static uint8_t Retries = 3;
  (void)param;

  if (Retries--)
  {
    sendImageBlockRequest();
    return;
  }

  Retries = 3;
  Serial_Print(interfaceId,"\n\rOTA transfer time-out!", gAllowToBlock_d);
  AbortOtaTransfer();
}

/******************************************************************************
* OtaWaitTimeoutCallback -  is called when an ImageBlockResponse with the
* status OtapWaitForData_c is received or if a wrong FileOffset is received OTA.
* The device will request a new image block after gOtaWaitTimeout_c ms.
*
******************************************************************************/
static void OtaWaitTimeoutCallback(void *param)
{
  (void)param;
  sendImageBlockRequest();
}
