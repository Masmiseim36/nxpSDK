/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* MyWirelessApp Demo Coordinator application.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "mwa_coordinator.h"

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
static uint8_t App_StartScan(macScanType_t scanType, uint8_t appInstance);
static void    App_HandleScanEdConfirm(nwkMessage_t *pMsg);
static uint8_t App_StartCoordinator( uint8_t appInstance );
static uint8_t App_HandleMlmeInput(nwkMessage_t *pMsg, uint8_t appInstance);
static uint8_t App_SendAssociateResponse(nwkMessage_t *pMsgIn, uint8_t appInstance);
static void    App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn, uint8_t appInstance);
static void    App_TransmitUartData(void);
static uint8_t App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType);
static void    App_HandleKeys(uint8_t events);

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
/* The short address and PAN ID of the coordinator*/
static const uint16_t mShortAddress = mDefaultValueOfShortAddress_c;
static const uint16_t mPanId = mDefaultValueOfPanId_c;

/* The current logical channel (frequency band) */
static uint8_t mLogicalChannel;

/* These byte arrays stores an associated
   devices long and short addresses. */
static uint16_t mDeviceShortAddress = 0xFFFF;
static uint64_t mDeviceLongAddress = 0xFFFFFFFFFFFFFFFF;

/* Data request packet for sending UART input to the coordinator */
static nwkToMcpsMessage_t *mpPacket;

/* The MSDU handle is a unique data packet identifier */
static uint8_t mMsduHandle;

/* Number of pending data packets */
static uint8_t mcPendingPackets;

/* Application input queues */
static anchor_t mMlmeNwkInputQueue;
static anchor_t mMcpsNwkInputQueue;

static const uint64_t mExtendedAddress = mMacExtendedAddress_c;
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
* notification.
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
    /* Reset number of pending packets */
    mcPendingPackets = 0;
    
    /* Prepare input queues.*/
    MSG_InitQueue(&mMlmeNwkInputQueue); 
    MSG_InitQueue(&mMcpsNwkInputQueue);
    
    /* Initialize the MAC 802.15.4 extended address */
    Mac_SetExtendedAddress( (uint8_t*)&mExtendedAddress, macInstance );

    /* register keyboard callback function */
    KBD_Init(App_HandleKeys);
    
    /* Initialize the serial terminal interface so that we can print out status messages */
    Serial_InitInterface(&interfaceId, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    Serial_SetBaudRate(interfaceId, gUARTBaudRate115200_c);
    Serial_SetRxCallBack(interfaceId, UartRxCallBack, NULL);
    
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
        if( ev & gAppEvtMessageFromMLME_c )
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
                    Serial_Print(interfaceId, "Received an MLME-Beacon Notify Indication\n\r", gAllowToBlock_d);
                }
            }
        }
        
        /* The application state machine */
        switch(gState)
        {
        case stateInit:
            /* Print a welcome message to the UART */
          Serial_Print(interfaceId,"MyWirelessApp Demo Coordinator application is initialized and ready.\n\r\n\r", gAllowToBlock_d);
          /* Goto Energy Detection state. */
          gState = stateScanEdStart;
          OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
          break;
          
      case stateScanEdStart:
          /* Start the Energy Detection scan, and goto wait for confirm state. */
          Serial_Print(interfaceId,"Initiating the Energy Detection Scan\n\r", gAllowToBlock_d);

          ret = App_StartScan(gScanModeED_c, 0);
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
              Serial_Print(interfaceId,"\n\rStarting as PAN coordinator on channel 0x", gAllowToBlock_d);
              Serial_PrintHex(interfaceId,&mLogicalChannel, 1, gPrtHexNoFormat_c);
              Serial_Print(interfaceId,".\n\r", gAllowToBlock_d);

              ret = App_StartCoordinator(0);
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
                      Serial_Print(interfaceId,"Started the coordinator with PAN ID 0x", gAllowToBlock_d);
                      Serial_PrintHex(interfaceId,(uint8_t *)&mPanId, 2, gPrtHexNoFormat_c);
                      Serial_Print(interfaceId,", and short address 0x", gAllowToBlock_d);
                      Serial_PrintHex(interfaceId,(uint8_t *)&mShortAddress, 2, gPrtHexNoFormat_c);
                      Serial_Print(interfaceId,".\n\rReady to send and receive data over the UART.\n\r\n\r", gAllowToBlock_d);
                      
                      gState = stateListen;
                      OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                  }
              }
          }
          break; 
          
      case stateListen:
          /* Stay in this state forever. 
          Transmit the data received on UART */
          if (ev & gAppEvtMessageFromMLME_c)
          {
              /* Get the message from MLME */
              if (pMsgIn)
              {      
                  /* Process it */
                  ret = App_HandleMlmeInput(pMsgIn, 0);
                  /* Messages from the MLME must always be freed. */
              }
          }
          
          if (ev & gAppEvtRxFromUart_c)
          {      
              /* get byte from UART */
              App_TransmitUartData();
          }
          break;
      }/* switch(gState) */
      
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
              App_HandleMcpsInput(pMsgIn, 0);
              /* Messages from the MCPS must always be freed. */
              MSG_Free(pMsgIn);
              pMsgIn = NULL;
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
  }/* while(1) */
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
    
    if( stateListen == gState )
    {
        OSA_EventSet(mAppEvent, gAppEvtRxFromUart_c);
        return;
    }
    
    if( gState == stateInit )
    {
        LED_StopFlashingAllLeds();
        OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
    }

    do
    {
        Serial_GetByteFromRxBuffer(interfaceId, &pressedKey, &count);
    }while(count);
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
static uint8_t App_StartScan(macScanType_t scanType, uint8_t appInstance)
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
    /* ChannelsToScan */
#ifdef gPHY_802_15_4g_d
    pScanReq->channelPage = gChannelPageId9_c;
    pScanReq->scanChannels[0] = mDefaultValueOfChannel_c;
    FLib_MemSet(pScanReq->scanChannels+1, 0, sizeof(channelMask_t)-sizeof(uint32_t));
#else
    pScanReq->scanChannels = mDefaultValueOfChannel_c;
#endif

    /* Duration per channel 0-14 (dc). T[sec] = (16*960*((2^dc)+1))/1000000.
       A scan duration of 3 on 16 channels approximately takes 2 secs. */
    pScanReq->scanDuration = 3;
    /* Don't use security */
    pScanReq->securityLevel = gMacSecurityNone_c;
    
    /* Send the Scan request to the MLME. */
    if( NWK_MLME_SapHandler( pMsg, macInstance ) == gSuccess_c )
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
    Serial_Print(interfaceId,"Message allocation failed!\n\r", gAllowToBlock_d);
    return errorAllocFailed;
  }
}


/******************************************************************************
* The App_HandleScanEdConfirm(nwkMessage_t *pMsg) function will handle the
* ED scan confirm message received from the MLME when the ED scan has completed.
* The message contains the ED scan result list. This function will search the
* list in order to select the logical channel with the least energy. The
* selected channel is stored in the global variable called 'maLogicalChannel'.
*
******************************************************************************/
static void App_HandleScanEdConfirm(nwkMessage_t *pMsg)
{  
  uint8_t n, minEnergy;
  uint8_t *pEdList;
  uint32_t chMask = mDefaultValueOfChannel_c;
  uint8_t idx;
#ifndef gPHY_802_15_4g_d
  uint8_t Channel;
#endif

  Serial_Print(interfaceId,"Received the MLME-Scan Confirm message from the MAC\n\r", gAllowToBlock_d);
    
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
          if( (chMask & (1 << n)) )
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
      /* Channel numbering is 11 to 26 both inclusive */
      Channel = n + 11;
      if( (chMask & (1 << Channel)) )
      {
          if( pEdList[idx] < minEnergy )
          {
              minEnergy = pEdList[idx];
              mLogicalChannel = Channel;
          }
          idx++;
      }
  }
#endif /* gPHY_802_15_4g_d */     

  chMask &= ~(1 << mLogicalChannel);
  
  /* Print out the result of the ED scan */
  Serial_Print(interfaceId,"ED scan returned the following results:\n\r  [", gAllowToBlock_d);
#ifdef gPHY_802_15_4g_d
  Serial_PrintHex(interfaceId,pEdList, mDefaultMaxChannel_c, gPrtHexBigEndian_c | gPrtHexSpaces_c);
#else  
  Serial_PrintHex(interfaceId,pEdList, 16, gPrtHexBigEndian_c | gPrtHexSpaces_c);
#endif /* gPHY_802_15_4g_d */
  Serial_Print(interfaceId,"]\n\r\n\r", gAllowToBlock_d);
  
  /* Print out the selected logical channel */
  Serial_Print(interfaceId,"Based on the ED scan the logical channel 0x", gAllowToBlock_d);
  Serial_PrintHex(interfaceId,&mLogicalChannel, 1, gPrtHexNoFormat_c);
  Serial_Print(interfaceId," was selected\n\r", gAllowToBlock_d);

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
static uint8_t App_StartCoordinator( uint8_t appInstance )
{
  /* Message for the MLME will be allocated and attached to this pointer */
  mlmeMessage_t *pMsg;
  /* Value that will be written to the MAC PIB */
  uint8_t value;
  /* Pointer which is used for easy access inside the allocated message */
  mlmeStartReq_t *pStartReq;

  Serial_Print(interfaceId,"Sending the MLME-Start Request message to the MAC...", gAllowToBlock_d);
  
  /* Allocate a message for the MLME (We should check for NULL). */
  pMsg = MSG_AllocType(mlmeMessage_t);
  if(pMsg != NULL)
  {
  
    /* Set-up MAC PIB attributes. Please note that Set, Get,
       and Reset messages are not freed by the MLME. */

    /* Initialize the MAC 802.15.4 extended address */
    pMsg->msgType = gMlmeSetReq_c;
    pMsg->msgData.setReq.pibAttribute = gMPibExtendedAddress_c;
    pMsg->msgData.setReq.pibAttributeValue = (uint8_t *)&mExtendedAddress;
    (void)NWK_MLME_SapHandler( pMsg, macInstance );
      
    /* We must always set the short address to something
       else than 0xFFFF before starting a PAN. */
    pMsg->msgType = gMlmeSetReq_c;
    pMsg->msgData.setReq.pibAttribute = gMPibShortAddress_c;
    pMsg->msgData.setReq.pibAttributeValue = (uint8_t *)&mShortAddress;
    (void)NWK_MLME_SapHandler( pMsg, macInstance );

    /* We must set the Association Permit flag to TRUE
       in order to allow devices to associate to us. */
    pMsg->msgType = gMlmeSetReq_c;
    pMsg->msgData.setReq.pibAttribute = gMPibAssociationPermit_c;
    value = TRUE;
    pMsg->msgData.setReq.pibAttributeValue = &value;
    (void)NWK_MLME_SapHandler( pMsg, macInstance );

    /* This is a MLME-START.req command */
    pMsg->msgType = gMlmeStartReq_c;

    /* Create the Start request message data. */
    pStartReq = &pMsg->msgData.startReq;
    /* PAN ID - LSB, MSB. The example shows a PAN ID of 0xBEEF. */
    FLib_MemCpy(&pStartReq->panId, (void *)&mPanId, 2);
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
    pStartReq->startTime = 0;

    /* Don't use security */
    pStartReq->coordRealignSecurityLevel = gMacSecurityNone_c;
    pStartReq->beaconSecurityLevel       = gMacSecurityNone_c;

    /* Send the Start request to the MLME. */
    if(NWK_MLME_SapHandler( pMsg, macInstance ) != gSuccess_c)
    {
      /* One or more parameters in the Start Request message were invalid. */
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

  Serial_Print(interfaceId,"Done\n\r", gAllowToBlock_d);
  return errorNoError;
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
static uint8_t App_SendAssociateResponse(nwkMessage_t *pMsgIn, uint8_t appInstance)
{
  mlmeMessage_t *pMsg;
  mlmeAssociateRes_t *pAssocRes;
 
  Serial_Print(interfaceId,"Sending the MLME-Associate Response message to the MAC...", gAllowToBlock_d);
 
  /* Allocate a message for the MLME */
  pMsg = MSG_AllocType(mlmeMessage_t);
  if(pMsg != NULL)
  {
    /* This is a MLME-ASSOCIATE.res command */
    pMsg->msgType = gMlmeAssociateRes_c;

    /* Create the Associate response message data. */
    pAssocRes = &pMsg->msgData.associateRes;

    /* Assign a short address to the device. In this example we simply
       choose 0x0001. Though, all devices and coordinators in a PAN must have
       different short addresses. However, if a device do not want to use
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
    if( gSuccess_c == NWK_MLME_SapHandler( pMsg, macInstance ) )
    {
      Serial_Print( interfaceId,"Done\n\r", gAllowToBlock_d );
      return errorNoError;
    }
    else
    {
      /* One or more parameters in the message were invalid. */
      Serial_Print( interfaceId,"Invalid parameter!\n\r", gAllowToBlock_d );
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
* The App_HandleMlmeInput(nwkMessage_t *pMsg) function will handle various
* messages from the MLME, e.g. (Dis)Associate Indication.
*
* The function may return either of the following values:
*   errorNoError:   The message was processed.
*   errorNoMessage: The message pointer is NULL.
******************************************************************************/
static uint8_t App_HandleMlmeInput(nwkMessage_t *pMsg, uint8_t appInstance)
{
  if(pMsg == NULL)
    return errorNoMessage;
  
  /* Handle the incoming message. The type determines the sort of processing.*/
  switch(pMsg->msgType) {
  case gMlmeAssociateInd_c:
    Serial_Print(interfaceId,"Received an MLME-Associate Indication from the MAC\n\r", gAllowToBlock_d);
    /* A device sent us an Associate Request. We must send back a response.  */
    return App_SendAssociateResponse(pMsg, appInstance);
    
  case gMlmeCommStatusInd_c:
    /* Sent by the MLME after the Association Response has been transmitted. */
    Serial_Print(interfaceId,"Received an MLME-Comm-Status Indication from the MAC\n\r", gAllowToBlock_d);
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
static void App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn, uint8_t appInstance)
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
    /* The MCPS-Data indication is sent by the MAC to the network
       or application layer when data has been received. We simply
       copy the received data to the UART. */
    Serial_SyncWrite( interfaceId,pMsgIn->msgData.dataInd.pMsdu, pMsgIn->msgData.dataInd.msduLength );
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
* The function uses the device information, that was stored when the device,
* associated to us, for building an MCPS-Data Request message. The message
* is sent to the MCPS service access point in the MAC.
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
        if( mDeviceShortAddress != 0xFFFF )
        {
            mpPacket = MSG_Alloc(sizeof(nwkToMcpsMessage_t) + count);
        }
    }
    
    if(mpPacket != NULL)
    {
        /* Data is available in the SerialManager's receive buffer. Now create an
        MCPS-Data Request message containing the data. */
        mpPacket->msgType = gMcpsDataReq_c;
        mpPacket->msgData.dataReq.pMsdu = (uint8_t*)(&mpPacket->msgData.dataReq.pMsdu) + 
            sizeof(mpPacket->msgData.dataReq.pMsdu);
        Serial_Read(interfaceId, mpPacket->msgData.dataReq.pMsdu, count, &count);
        /* Create the header using device information stored when creating
        the association response. In this simple example the use of short
        addresses is hardcoded. In a real world application we must be
        flexible, and use the address mode required by the given situation. */
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstAddr, (void*)&mDeviceShortAddress, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcAddr, (void*)&mShortAddress, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.dstPanId, (void*)&mPanId, 2);
        FLib_MemCpy(&mpPacket->msgData.dataReq.srcPanId, (void*)&mPanId, 2);
        mpPacket->msgData.dataReq.dstAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.srcAddrMode = gAddrModeShortAddress_c;
        mpPacket->msgData.dataReq.msduLength = count;
        /* Request MAC level acknowledgement, and
        indirect transmission of the data packet */
        mpPacket->msgData.dataReq.txOptions = gMacTxOptionsAck_c;
        mpPacket->msgData.dataReq.txOptions |= gMacTxOptionIndirect_c;
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
  uint8_t events  /*IN: Events from keyboard modul  */
  )
{
#if gKBD_KeysCount_c > 0 
    switch ( events ) 
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
