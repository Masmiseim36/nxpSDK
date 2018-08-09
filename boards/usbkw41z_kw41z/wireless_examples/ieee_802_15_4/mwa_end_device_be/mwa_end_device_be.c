/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* MyWirelessApp Demo End Device application.
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
#include "mwa_end_device_be.h"

/* Drv */
#include "LED.h"
#include "Keyboard.h"

/* Fwk */
#include "SecLib.h"
#include "SerialManager.h"
#include "RNG_Interface.h"
#include "MemManager.h"
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

/* If there are too many pending packets to be send over the air, */
/* receive mMaxKeysToReceive_c chars. */
/* The chars will be send over the air when there are no pending packets*/
#define mMaxKeysToReceive_c 32

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

/* Forward declarations of helper functions */
static void    UartRxCallBack(void*);
static uint8_t App_StartScan(macScanType_t scanType);
static uint8_t App_HandleScanPassiveConfirm(nwkMessage_t *pMsg);
static uint8_t App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType);
static uint8_t App_SendSyncRequest(void);
static uint8_t App_SendAssociateRequest(void);
static uint8_t App_HandleAssociateConfirm(nwkMessage_t *pMsg);
static uint8_t App_HandleMlmeInput(nwkMessage_t *pMsg);
static void    App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn);
static void    App_TransmitUartData(void);
static void    App_HandleKeys( key_event_t events );

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
/* Information about the PAN we are part of */
static panDescriptor_t mCoordInfo;

/* This is either the short address assigned by the PAN coordinator
   during association, or our own extended MAC address. */
static uint8_t maMyAddress[8];
/* The devices address mode. If 2, then maMyAddress contains the short
   address assigned by the PAN coordinator. If 3, then maMyAddress is
   equal to the extended address. */
static addrModeType_t mAddrMode;

/* Data request packet for sending UART input to the coordinator */
static nwkToMcpsMessage_t *mpPacket;

/* The MSDU handle is a unique data packet identifier */
static uint8_t mMsduHandle;

/* Number of pending data packets */
static uint8_t mcPendingPackets;

/* Application input queues */
static anchor_t mMlmeNwkInputQueue;
static anchor_t mMcpsNwkInputQueue;

static const uint64_t mExtendedAddress  = mMacExtendedAddress_c;
static instanceId_t   macInstance;
static uint8_t        interfaceId;
osaEventId_t          mAppEvent;



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
void App_init(void)
{
  mAppEvent = OSA_EventCreate(TRUE);
  /* The initial application state */
  gState = stateInit;
  /* Reset number of pending packets */
  mcPendingPackets = 0;

  /* Initialize the MAC 802.15.4 extended address */
  Mac_SetExtendedAddress( (uint8_t*)&mExtendedAddress, macInstance );

  /* register keyboard callback function */
  KBD_Init(App_HandleKeys);

  /* Initialize the UART so that we can print out status messages */
  Serial_InitInterface(&interfaceId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
  Serial_SetBaudRate(interfaceId, gUARTBaudRate115200_c);
  Serial_SetRxCallBack(interfaceId, UartRxCallBack, NULL);

  /* Prepare input queues.*/
  MSG_InitQueue(&mMlmeNwkInputQueue);
  MSG_InitQueue(&mMcpsNwkInputQueue);

  /*signal app ready*/
  LED_StartSerialFlash(LED1);

  Serial_Print(interfaceId, "\n\rPress any switch on board to start running the application.\n\r", gAllowToBlock_d);  
}

/*****************************************************************************
*Mac Application Task event processor.  This function is called to
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
    void *pMsgIn = NULL;
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
        if( ev & gAppEvtMessageFromMLME_c )
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
                    Serial_Print(interfaceId, "Received an MLME-Beacon Notify Indication\n\r", gAllowToBlock_d);
                }
            }
        }
        
        /* The application state machine */
        switch(gState)
        {
        case stateInit:
            /* Print a welcome message to the UART */
            Serial_Print(interfaceId, "MyWirelessApp Demo Beacon Enabled End Device application is initialized and ready.\n\r", gAllowToBlock_d);   
            /* Goto Active Scan state. */
            gState = stateScanPassiveStart;
            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
            break;
            
        case stateScanPassiveStart:
            /* Start the Active scan, and goto wait for confirm state. */
            Serial_Print(interfaceId, "\n\rStart scanning for a PAN coordinator\n\r", gAllowToBlock_d);
            
            rc = App_StartScan(gScanModePassive_c);
            if(rc == errorNoError)
            {
                gState = stateScanPassiveWaitConfirm;
            }
            break;
            
        case stateScanPassiveWaitConfirm:
            /* Stay in this state until the Scan confirm message
            arrives, and then goto the associate state. */
            if (ev & gAppEvtMessageFromMLME_c)
            {
                if (pMsgIn)
                {                     
                    rc = App_WaitMsg(pMsgIn, gMlmeScanCnf_c);
                    if(rc == errorNoError)
                    {
                        rc = App_HandleScanPassiveConfirm(pMsgIn);
                        if(rc == errorNoError)
                        {
                            Serial_Print(interfaceId, "Found a coordinator with the following properties:\n\r", gAllowToBlock_d);
                            Serial_Print(interfaceId, "----------------------------------------------------", gAllowToBlock_d);
                            Serial_Print(interfaceId, "\n\rAddress...........0x", gAllowToBlock_d); Serial_PrintHex(interfaceId, (uint8_t*)&mCoordInfo.coordAddress, mCoordInfo.coordAddrMode == gAddrModeShortAddress_c ? 2 : 8, gPrtHexNoFormat_c);
                            Serial_Print(interfaceId, "\n\rPAN ID............0x", gAllowToBlock_d); Serial_PrintHex(interfaceId, (uint8_t*)&mCoordInfo.coordPanId, 2, gPrtHexNoFormat_c);
                            Serial_Print(interfaceId, "\n\rLogical Channel...0x", gAllowToBlock_d); Serial_PrintHex(interfaceId, &mCoordInfo.logicalChannel, 1, gPrtHexNoFormat_c);
                            Serial_Print(interfaceId, "\n\rBeacon Spec.......0x", gAllowToBlock_d); Serial_PrintHex(interfaceId, (uint8_t*)&mCoordInfo.superframeSpec, 2, gPrtHexNoFormat_c);
                            Serial_Print(interfaceId, "\n\rLink Quality......0x", gAllowToBlock_d); Serial_PrintHex(interfaceId, &mCoordInfo.linkQuality, 1, gPrtHexNoFormat_c);
                            Serial_Print(interfaceId, "\n\r\n\r", gAllowToBlock_d);
                            
                            gState = stateAssociate;
                            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                        }
                        else
                        {
                            Serial_Print(interfaceId, "Scan did not find a suitable coordinator\n\r", gAllowToBlock_d);
                            
                            /* Restart the Active scan */
                            gState = stateScanPassiveStart;
                            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                        }
                    }
                }
            }
            break;
            
        case stateAssociate:
            /* Associate to the PAN coordinator */
            Serial_Print(interfaceId, "Associating to PAN coordinator on channel 0x", gAllowToBlock_d);
            Serial_PrintHex(interfaceId, &(mCoordInfo.logicalChannel), 1, gPrtHexNewLine_c);
            
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
                            Serial_Print(interfaceId, "Successfully associated with the coordinator.\n\r", gAllowToBlock_d);
                            Serial_Print(interfaceId, "We were assigned the short address 0x", gAllowToBlock_d);
                            Serial_PrintHex(interfaceId, maMyAddress, mAddrMode == gAddrModeShortAddress_c ? 2 : 8, gPrtHexNoFormat_c);
                            Serial_Print(interfaceId, "\n\r\n\rReady to send and receive data over the UART.\n\r\n\r", gAllowToBlock_d);
                            
                            /* Go to the listen state */
                            gState = stateListen;
                            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c); 
                        }        
                        else 
                        {
                            Serial_Print(interfaceId, "\n\rAssociate Confirm wasn't successful... \n\r\n\r", gAllowToBlock_d);
                            gState = stateScanPassiveStart;
                            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                        }
                    }
                }
            }
            break; 

        case stateListen:
            /* Transmit to coordinator data received from UART. */
            if (ev & gAppEvtMessageFromMLME_c)
            {  
                if (pMsgIn)
                {  
                    /* Process it */
                    rc = App_HandleMlmeInput(pMsgIn);
                }
            } 

            if (ev & gAppEvtRxFromUart_c)
            {      
                /* get byte from UART */
                App_TransmitUartData();
            }  
            break;
        }

        if (pMsgIn)
        {
            /* Messages must always be freed. */ 
            MSG_Free(pMsgIn);
        }

        /* Handle MCPS confirms and transmit data from UART */
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
            }
        }
        
        /* Check for pending messages in the Queue */ 
        if( MSG_Pending(&mMcpsNwkInputQueue) )
            OSA_EventSet(mAppEvent, gAppEvtMessageFromMCPS_c);
        if( MSG_Pending(&mMlmeNwkInputQueue) )
            OSA_EventSet(mAppEvent, gAppEvtMessageFromMLME_c);

        if( !gUseRtos_c )
        {
            break;
        }
    } /* while(1) */
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*****************************************************************************
* UartRxCallBack
*
* This callback is triggered when a new byte is received over the UART
*
*****************************************************************************/
static void UartRxCallBack(void *pData) 
{
  uint8_t pressedKey;
  uint16_t count;
  
  if(stateListen == gState){
    OSA_EventSet(mAppEvent, gAppEvtRxFromUart_c);
  }else{
    Serial_GetByteFromRxBuffer(interfaceId, &pressedKey, &count);
  }
}

/******************************************************************************
* The App_SendSyncRequest() function will try to resyncronize with
* the coordinator by sending a Sync reuqest message
*
* The function may return either of the following values:
*   errorNoError:          The message was sent successfully.
*   errorInvalidParameter: The MLME service access point rejected the
*                          message due to an invalid parameter.
*   errorAllocFailed:      A message buffer could not be allocated.
*
******************************************************************************/
static uint8_t App_SendSyncRequest(void)
{
    mlmeMessage_t *pMsgOut = MSG_AllocType(mlmeMessage_t);

    if(pMsgOut != NULL)
    {
      pMsgOut->msgType = gMlmeSyncReq_c;
      pMsgOut->msgData.syncReq.logicalChannel = mCoordInfo.logicalChannel;
      pMsgOut->msgData.syncReq.trackBeacon = TRUE;

      if(NWK_MLME_SapHandler( pMsgOut, macInstance ) == gSuccess_c)
      {
        Serial_Print(interfaceId, "Sync Request sent!\n\r", gAllowToBlock_d);
        return errorNoError;
      }
      else
      {
        Serial_Print(interfaceId, "Invalid Sync Request parameters.\n\r", gAllowToBlock_d);
        return errorInvalidParameter;
      }
    }
    else
    {
        Serial_Print(interfaceId, "Sync Request Alloc error.\n\r", gAllowToBlock_d);
        return errorAllocFailed;
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

  Serial_Print(interfaceId, "Sending the MLME-Scan Request message to the MAC...", gAllowToBlock_d);

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
    pScanReq->scanDuration = mDefaultValueOfScanDuration_c;
    pScanReq->securityLevel = gMacSecurityNone_c;
    
    /* Send the Scan request to the MLME. */
    if(NWK_MLME_SapHandler( pMsg, macInstance ) == gSuccess_c)
    {
      Serial_Print(interfaceId, "Done\n\r", gAllowToBlock_d);
      return errorNoError;
    }
    else
    {
      Serial_Print(interfaceId, "Invalid parameter!\n\r", gAllowToBlock_d);
      return errorInvalidParameter;
    }
  }
  else
  {
    /* Allocation of a message buffer failed. */
    Serial_Print(interfaceId, "Message allocation failed!\n\r", gAllowToBlock_d);
    return errorAllocFailed;
  }
}

/******************************************************************************
* The App_HandleScanPassiveConfirm(nwkMessage_t *pMsg) function will handle the
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
static uint8_t App_HandleScanPassiveConfirm(nwkMessage_t *pMsg)
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
        if( pPanDesc->superframeSpec.associationPermit )
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
  
  if(rc == errorNoError)
  {
    /* If we have found a beaconing coodinator we must setup the MAC to
       synchronize to the beacon frames. This requires us to set the
       PAN ID attribute of the MAC PIB to the PAN ID of the coordinator.
       Furthermore, if we want to take advantage of the automatic
       polling feature we must set the Auto Request MAC PIB attribute. */
    if((mCoordInfo.superframeSpec.beaconOrder) < 0xF)
    {
      mlmeMessage_t *pMsgOut = MSG_AllocType(mlmeMessage_t);
      if(pMsgOut != NULL)
      {
        uint8_t value = TRUE;
        uint8_t mBeaconOrder;
        uint8_t mSuperFrameOrder;
        /* Set MAC PIB auto request to TRUE. In this way the device will
           automatically poll for data if the pending address list of the
           beacon frame contains our address. */
        pMsgOut->msgType = gMlmeSetReq_c;
        pMsgOut->msgData.setReq.pibAttribute = gMPibAutoRequest_c;
        pMsgOut->msgData.setReq.pibAttributeValue = &value;
        /* Get/Set/Reset Request messages are NOT freed by the MLME. */
        (void)NWK_MLME_SapHandler( pMsgOut, macInstance );

        /* Since we are going to receive data from the coordinator
           using automatic polling we must synchronize to the beacon
           and keep tracking it. Before synchronizing it is required
           that the MAC PIB PAN ID, and the MAC PIB coordinator
           address is set. */
        pMsgOut->msgType = gMlmeSetReq_c;
        pMsgOut->msgData.setReq.pibAttribute = gMPibPanId_c;
        pMsgOut->msgData.setReq.pibAttributeValue = &mCoordInfo.coordPanId;
        /* Get/Set/Reset Request messages are NOT freed by the MLME. */
        (void)NWK_MLME_SapHandler( pMsgOut, macInstance );

        /* Set coordinator address PIB attribute according the the
           address mode of the coordinator (short or long address). */
        pMsgOut->msgType = gMlmeSetReq_c;
        pMsgOut->msgData.setReq.pibAttribute =
          mCoordInfo.coordAddrMode == gAddrModeShortAddress_c ? gMPibCoordShortAddress_c :
                                                                gMPibCoordExtendedAddress_c;
        pMsgOut->msgData.setReq.pibAttributeValue = &mCoordInfo.coordAddress;
        /* Get/Set/Reset Request messages are NOT freed by the MLME. */
        (void)NWK_MLME_SapHandler( pMsgOut, macInstance );


        /* Set macBeaconOrder PIB attribute according to the
           value found in beacon.*/
        pMsgOut->msgType = gMlmeSetReq_c;
        mBeaconOrder = mCoordInfo.superframeSpec.beaconOrder;
        pMsgOut->msgData.setReq.pibAttribute = gMPibBeaconOrder_c;
        pMsgOut->msgData.setReq.pibAttributeValue = &mBeaconOrder;
        /* Get/Set/Reset Request messages are NOT freed by the MLME. */
        (void)NWK_MLME_SapHandler( pMsgOut, macInstance );

        /* Set macSuperFrameOrder PIB attribute according to the
           value found in beacon.*/
        pMsgOut->msgType = gMlmeSetReq_c;
        mSuperFrameOrder = mCoordInfo.superframeSpec.superframeOrder;
        pMsgOut->msgData.setReq.pibAttribute = gMPibSuperframeOrder_c;
        pMsgOut->msgData.setReq.pibAttributeValue = &mSuperFrameOrder;
        /* Get/Set/Reset Request messages are NOT freed by the MLME. */
        (void)NWK_MLME_SapHandler( pMsgOut, macInstance );

        /* Now send the MLME-Sync Request. We choose to let the MAC track
           the beacons on the logical channel obtained by the passive scan.*/
        pMsgOut->msgType = gMlmeSyncReq_c;
        pMsgOut->msgData.syncReq.trackBeacon = TRUE;
        pMsgOut->msgData.syncReq.logicalChannel = mCoordInfo.logicalChannel;
        (void)NWK_MLME_SapHandler( pMsgOut, macInstance );
      }
    }
  }
  else   if (pDescBlock)
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

  Serial_Print(interfaceId, "Sending the MLME-Associate Request message to the MAC...", gAllowToBlock_d);
  
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
    pAssocReq->securityLevel      = gMacSecurityNone_c;
#ifdef gPHY_802_15_4g_d
    pAssocReq->channelPage = gChannelPageId9_c;
#else
    pAssocReq->channelPage = gDefaultChannelPageId_c;
#endif

    /* We want the coordinator to assign a short address to us. */
    pAssocReq->capabilityInfo     = gCapInfoAllocAddr_c;
      
    /* Send the Associate Request to the MLME. */
    if(NWK_MLME_SapHandler( pMsg, macInstance ) == gSuccess_c)
    {
      Serial_Print(interfaceId, "Done\n\r", gAllowToBlock_d);
      return errorNoError;
    }
    else
    {
      /* One or more parameters in the message were invalid. */
      Serial_Print(interfaceId, "Invalid parameter!\n\r", gAllowToBlock_d);
      return errorInvalidParameter;
    }
  }
  else
  {
    /* Allocation of a message buffer failed. */
    Serial_Print(interfaceId, "Message allocation failed!\n\r", gAllowToBlock_d);
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

	  if( pMsg->msgData.associateCnf.assocShortAddress >= 0xFFFE)
	  {
	    mAddrMode = gAddrModeExtendedAddress_c;
	    FLib_MemCpy(maMyAddress, (void*)&mExtendedAddress, 8);
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
  case gMlmeBeaconNotifyInd_c:
    /* Always free pBufferRoot */
    MSG_Free(((nwkMessage_t *)pMsg)->msgData.beaconNotifyInd.pBufferRoot);
    break;

  case gMlmeSyncLossInd_c:
      Serial_Print(interfaceId,"\n\rSynchronization lost!\n\r", gAllowToBlock_d);
      /* Resync with the coordinator */
      if ( App_SendSyncRequest() != errorNoError)
      {
        Serial_Print(interfaceId,"Error : Send Sync Request.\n\r", gAllowToBlock_d);
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
    if(mcPendingPackets)
      mcPendingPackets--;
    break;

  case gMcpsDataInd_c:
    /* Copy the received data to the UART. */
    Serial_SyncWrite(interfaceId, pMsgIn->msgData.dataInd.pMsdu, pMsgIn->msgData.dataInd.msduLength);
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
* The App_TransmitUartData() function will perform (single/multi buffered)
* data transmissions of data received by the UART. Data could also come from
* other sources such as sensors etc. This is completely determined by the
* application. The constant mDefaultValueOfMaxPendingDataPackets_c determine the maximum
* number of packets pending for transmission in the MAC. A global variable
* is incremented each time a data packet is sent to the MCPS, and decremented
* when the corresponding MCPS-Data Confirm message is received. If the counter
* reaches the defined maximum no more data buffers are allocated until the
* counter is decreased below the maximum number of pending packets.
*
* The function uses the coordinator information gained during the Active Scan,
* and the short address assigned to us by coordinator, for building an MCPS-
* Data Request message. The message is sent to the MCPS service access point
* in the MAC.
******************************************************************************/
static void App_TransmitUartData(void)
{   
    uint16_t count;
    
    /* Count bytes receive over the serial interface */
    Serial_RxBufferByteCount(interfaceId, &count);
    
    if( 0 == count )
    {
        return;
    }
    
    /* Limit data transfer size */
    if( count > mMaxKeysToReceive_c )
    {
        count = mMaxKeysToReceive_c;
    }

    /* Use multi buffering for increased TX performance. It does not really
    have any effect at low UART baud rates, but serves as an
    example of how the throughput may be improved in a real-world
    application where the data rate is of concern. */
    if( (mcPendingPackets < mDefaultValueOfMaxPendingDataPackets_c) && (mpPacket == NULL) ) 
    {
        /* If the maximum number of pending data buffes is below maximum limit 
        and we do not have a data buffer already then allocate one. */
        mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + gMaxPHYPacketSize_c);
    }

    if(mpPacket != NULL)
    {
        /* Data is available in the SerialManager's receive buffer. Now create an
        MCPS-Data Request message containing the data. */
        mpPacket->msgType = gMcpsDataReq_c;
        mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&mpPacket->msgData.dataReq.pMsdu) + 
                                          sizeof(mpPacket->msgData.dataReq.pMsdu);
        Serial_Read(interfaceId, mpPacket->msgData.dataReq.pMsdu, count, &count);
        /* Create the header using coordinator information gained during 
        the scan procedure. Also use the short address we were assigned
        by the coordinator during association. */
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, &mCoordInfo.coordAddress, 8);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, &maMyAddress, 8);
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, &mCoordInfo.coordPanId, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, &mCoordInfo.coordPanId, 2);
        mpPacket->msgData.dataReq.dstAddrMode = mCoordInfo.coordAddrMode;
        mpPacket->msgData.dataReq.srcAddrMode = mAddrMode;
        mpPacket->msgData.dataReq.msduLength = count;
        /* Request MAC level acknowledgement of the data packet */
        mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
        /* Give the data packet a handle. The handle is
        returned in the MCPS-Data Confirm message. */
        mpPacket->msgData.dataReq.msduHandle = mMsduHandle++;
        /* Don't use security */
        mpPacket->msgData.dataReq.securityLevel = gMacSecurityNone_c;
        
        /* Send the Data Request to the MCPS */
        (void)NWK_MCPS_SapHandler(mpPacket, macInstance);
        
        /* Prepare for another data buffer */
        mpPacket = NULL;
        mcPendingPackets++;
    }
    
    /* If the data wasn't send over the air because there are too many pending packets,
    or new data has beed received, try to send it later   */
    Serial_RxBufferByteCount(interfaceId, &count);
    
    if( count )
    {
        OSA_EventSet(mAppEvent, gAppEvtRxFromUart_c);
    }

}

/*****************************************************************************
* Handles all key events for this device.
* Interface assumptions: None
* Return value: None
*****************************************************************************/
static void App_HandleKeys
  (
  key_event_t events  /*IN: Events from keyboard modul */
  )
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
        if(gState == stateInit)
        {
            LED_StopFlashingAllLeds();

            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
        }
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
