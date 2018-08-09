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
#include "mwa_end_device.h"

/* Fwk */
#include "LED.h"
#include "Keyboard.h"
#include "SecLib.h"
#include "SerialManager.h"
#include "RNG_Interface.h"
#include "Panic.h"
#include "MemManager.h"
#include "TimersManager.h"
#include "FunctionLib.h"

#if mEnterLowPowerWhenIdle_c
  #include "PWR_Interface.h"
#endif

#if gNvmTestActive_d
  #include "NVM_Interface.h"
#endif

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

#define mAppStackSize_c 700
#define mAppTaskPrio_c  4

#if gNvmTestActive_d

  /* Current IDs for each data set. */
  #define mCoordInfo_ID_c          0x0010
  #define maMyAddress_ID_c         0x0011
  #define mAddrMode_ID_c           0x0012
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
/* Forward declarations of helper functions */
static void    UartRxCallBack(void*);
static uint8_t App_StartScan(macScanType_t scanType);
static uint8_t App_HandleScanActiveConfirm(nwkMessage_t *pMsg);
static uint8_t App_WaitMsg(nwkMessage_t *pMsg, uint8_t msgType);
static uint8_t App_SendAssociateRequest(void);
static uint8_t App_HandleAssociateConfirm(nwkMessage_t *pMsg);
static uint8_t App_HandleMlmeInput(nwkMessage_t *pMsg);
static void    App_HandleMcpsInput(mcpsToNwkMessage_t *pMsgIn);
static void    App_TransmitUartData(void);
static void    AppPollWaitTimeout(void *);
static void    App_HandleKeys( key_event_t events );

void App_init( void );
void AppThread (osaTaskParam_t argument);
void App_Idle_Task(uint32_t argument);
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

OSA_TASK_DEFINE( AppThread, mAppTaskPrio_c, 1, mAppStackSize_c, FALSE );

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

/* Signals that an MLME-Poll request is pending, and that we must wait for 
   the MLME-Poll confirm message before sending the next poll request. */
static bool_t mWaitPollConfirm;

/* Time between MLME-Poll requests */
static uint16_t mPollInterval;

/* Application input queues */
static anchor_t mMlmeNwkInputQueue;
static anchor_t mMcpsNwkInputQueue;

static tmrTimerID_t mTimer_c = gTmrInvalidTimerID_c;

static const uint64_t mExtendedAddress  = mMacExtendedAddress_c;
static instanceId_t   macInstance;
static uint8_t        interfaceId;
osaEventId_t          mAppEvent;
osaTaskId_t           mAppTaskHandler;

#if gNvmTestActive_d

static uint16_t timeoutCounter = 0;

/*
 * Datasets used by the NVM test application 
 */

/* This data set contains application variables to be preserved across resets */
NVM_RegisterDataSet(&mCoordInfo,          1,   sizeof(panDescriptor_t), mCoordInfo_ID_c, gNVM_MirroredInRam_c);
NVM_RegisterDataSet(&maMyAddress,         1,   8, maMyAddress_ID_c, gNVM_MirroredInRam_c);
NVM_RegisterDataSet(&mAddrMode,           1,   sizeof(addrModeType_t), mAddrMode_ID_c, gNVM_MirroredInRam_c);
#endif


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
#if mEnterLowPowerWhenIdle_c
        PWR_Init();
        PWR_DisallowDeviceToSleep();
#endif
#if gNvmTestActive_d
        NvModuleInit();
#endif
        
        /* Bind to MAC layer */
        macInstance = BindToMAC( (instanceId_t)0 );
        Mac_RegisterSapHandlers( MCPS_NWK_SapHandler, MLME_NWK_SapHandler, macInstance );

        App_init();

        /* Create application task */
        mAppTaskHandler = OSA_TaskCreate(OSA_TASK(AppThread), NULL);
        if( NULL == mAppTaskHandler )
        {
            panic(0,0,0,0);
            return;
        }
    }

    /* Call application Idle task */
    App_Idle_Task( param );
}

/*! *********************************************************************************
* \brief  This is the application's Idle task.
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
void App_Idle_Task(uint32_t argument)
{
#if mEnterLowPowerWhenIdle_c
    PWRLib_WakeupReason_t wakeupReason;
#endif

    while(1)
    {
#if gNvmTestActive_d
        /* Process NV Storage save-on-idle and save-on-count requests. */
        NvIdle();
#endif

#if mEnterLowPowerWhenIdle_c
        if( PWR_CheckIfDeviceCanGoToSleep() )
        {
            Serial_Print(interfaceId, "\n\rEntering in deep sleep mode...\n\r", gAllowToBlock_d);
            wakeupReason = PWR_EnterLowPower();
            PWR_DisallowDeviceToSleep();
            Led1On();
            Led2On();
            Led3On();
            Led4On();

            if( wakeupReason.Bits.FromKeyBoard )
            {
                App_HandleKeys( gKBD_EventSW1_c );
            }
        }
        else
        {
            PWR_EnterSleep();
        }
#endif
        if( !gUseRtos_c )
        {
            break;
        }
    }
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
    /* Reset number of pending packets */
    mcPendingPackets = 0;

    /* Allow sending a poll request */  
    mWaitPollConfirm = FALSE;    
    
    /* Initialize the poll interval */
    mPollInterval = mDefaultValueOfPollIntervalSlow_c;
    
    /* Initialize the MAC 802.15.4 extended address */
    Mac_SetExtendedAddress( (uint8_t*)&mExtendedAddress, macInstance );
    
    mTimer_c = TMR_AllocateTimer();
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
#if mEnterLowPowerWhenIdle_c
    if (!PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout)
    {
#endif
        Serial_Print(interfaceId, "\n\rPress any switch on board to start running the application.\n\r", gAllowToBlock_d);  
#if gNvmTestActive_d
        Serial_Print(interfaceId, "Long press switch_1 on board to use MAC data restore from NVM.\n\r", gAllowToBlock_d);  
#endif
#if mEnterLowPowerWhenIdle_c
    }
    else
    {
        Serial_Print(interfaceId, "\n\rWake up from deep sleep.\n\r", gAllowToBlock_d);
        if(gState == stateInit)
        {
            LED_StopFlashingAllLeds();
            OSA_EventSet(mAppEvent, gAppEvtPressedRestoreNvmBut_c);
        }
    }
#endif
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
void AppThread(osaTaskParam_t argument)
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
            Serial_Print(interfaceId, "MyWirelessApp Demo End Device application is initialized and ready.\n\r", gAllowToBlock_d);  
#if gNvmTestActive_d 
            if( ev & gAppEvtPressedRestoreNvmBut_c )
            {
              NvRestoreDataSet(&mCoordInfo, TRUE);
              NvRestoreDataSet(&maMyAddress, TRUE);
              NvRestoreDataSet(&mAddrMode, TRUE);
            }
            
            if ( mCoordInfo.coordAddress && ( mAddrMode != gAddrModeNoAddress_c ) ) 
            {
                mlmeMessage_t msg;
                msg.msgType = gMlmeSetReq_c;
                msg.msgData.setReq.pibAttribute = gMPibShortAddress_c;
                msg.msgData.setReq.pibAttributeValue = (uint8_t *)&maMyAddress[0];
                (void)NWK_MLME_SapHandler( &msg, 0 );
                msg.msgType = gMlmeSetReq_c;
                msg.msgData.setReq.pibAttribute = gMPibCoordShortAddress_c;
                msg.msgData.setReq.pibAttributeValue = (uint64_t *)&mCoordInfo.coordAddress;
                (void)NWK_MLME_SapHandler( &msg, 0 );
                msg.msgType = gMlmeSetReq_c;
                msg.msgData.setReq.pibAttribute = gMPibPanId_c;
                msg.msgData.setReq.pibAttributeValue = (uint16_t *)&mCoordInfo.coordPanId;
                (void)NWK_MLME_SapHandler( &msg, 0 );
                msg.msgType = gMlmeSetReq_c;
                msg.msgData.setReq.pibAttribute = gMPibLogicalChannel_c;
                msg.msgData.setReq.pibAttributeValue = (uint8_t *)&mCoordInfo.logicalChannel;
                (void)NWK_MLME_SapHandler( &msg, 0 );                 
                Serial_Print(interfaceId, "\n\rPIB elements restored from NVM:\n\r", gAllowToBlock_d); 
                Serial_Print(interfaceId, "\n\rAddress...........0x", gAllowToBlock_d); Serial_PrintHex(interfaceId, (uint8_t*)&mCoordInfo.coordAddress, mCoordInfo.coordAddrMode == gAddrModeShortAddress_c ? 2 : 8, gPrtHexNoFormat_c);
                Serial_Print(interfaceId, "\n\rPAN ID............0x", gAllowToBlock_d); Serial_PrintHex(interfaceId, (uint8_t*)&mCoordInfo.coordPanId, 2, gPrtHexNoFormat_c);
                Serial_Print(interfaceId, "\n\rLogical Channel...0x", gAllowToBlock_d); Serial_PrintHex(interfaceId, &mCoordInfo.logicalChannel, 1, gPrtHexNoFormat_c);
                Serial_Print(interfaceId, "\n\r", gAllowToBlock_d); 
                Serial_Print(interfaceId, "End device address restored from NVM: 0x", gAllowToBlock_d);
                Serial_PrintHex(interfaceId, maMyAddress, mAddrMode == gAddrModeShortAddress_c ? 2 : 8, gPrtHexNoFormat_c);
                Serial_Print(interfaceId, "\n\r\n\r", gAllowToBlock_d);
                /* Startup the timer */
                TMR_StartLowPowerTimer(mTimer_c, gTmrSingleShotTimer_c ,mPollInterval, AppPollWaitTimeout, NULL );
                gState = stateListen;
            }
            else
            {
                Serial_Print(interfaceId, "PIB elements were not restored from NVM!\n\r", gAllowToBlock_d); 
#endif
                /* Goto Active Scan state. */
                gState = stateScanActiveStart;
#if gNvmTestActive_d 
            }
#endif
            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
            break;
            
        case stateScanActiveStart:
            /* Start the Active scan, and goto wait for confirm state. */
            Serial_Print(interfaceId, "\n\rStart scanning for a PAN coordinator\n\r", gAllowToBlock_d);
            
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
#if gNvmTestActive_d 
                            if ( mCoordInfo.coordAddress && ( mAddrMode != gAddrModeNoAddress_c ) )
                            {
                                FLib_MemSet(&mCoordInfo, 0, sizeof(mCoordInfo));
                                FLib_MemSet(maMyAddress, 0, 8);
                                mAddrMode = gAddrModeNoAddress_c;
                                NvSaveOnIdle(&mCoordInfo, TRUE);
                                NvSaveOnIdle(&maMyAddress, TRUE);
                                NvSaveOnIdle(&mAddrMode, TRUE);
                            }
#endif
                            /* Restart the Active scan */
                            gState = stateScanActiveStart;
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
#if gNvmTestActive_d                             
                            NvSaveOnIdle(&mCoordInfo, TRUE);
                            NvSaveOnIdle(&maMyAddress, TRUE);
                            NvSaveOnIdle(&mAddrMode, TRUE);
#endif 
                            /* Startup the timer */
                            TMR_StartLowPowerTimer(mTimer_c, gTmrSingleShotTimer_c ,mPollInterval, AppPollWaitTimeout, NULL );
                            /* Go to the listen state */
                            gState = stateListen;
                            OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c); 
                        }        
                        else 
                        {
                            Serial_Print(interfaceId, "\n\rAssociate Confirm wasn't successful... \n\r\n\r", gAllowToBlock_d);
#if gNvmTestActive_d 
                            FLib_MemSet(&mCoordInfo, 0, sizeof(mCoordInfo));
                            FLib_MemSet(maMyAddress, 0, 8);
                            mAddrMode = gAddrModeNoAddress_c;
                            NvSaveOnIdle(&mCoordInfo, TRUE);
                            NvSaveOnIdle(&maMyAddress, TRUE);
                            NvSaveOnIdle(&mAddrMode, TRUE);
#endif
                            gState = stateScanActiveStart;
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
#if gNvmTestActive_d  
            if (timeoutCounter >= mDefaultValueOfTimeoutError_c)
            {
                  Serial_Print(interfaceId, "\n\rTimeout - No data received.\n\r\n\r", gAllowToBlock_d);
                  FLib_MemSet(&mCoordInfo, 0, sizeof(mCoordInfo));
                  FLib_MemSet(maMyAddress, 0, 8);
                  mAddrMode = gAddrModeNoAddress_c;
                  NvSaveOnIdle(&mCoordInfo, TRUE);
                  NvSaveOnIdle(&maMyAddress, TRUE);
                  NvSaveOnIdle(&mAddrMode, TRUE);
                  timeoutCounter = 0;
                  OSA_EventSet(mAppEvent, gAppEvtDummyEvent_c);
                  gState = stateInit;
            }
#endif
            break;
        }

        if (pMsgIn)
        {
            /* Messages must always be freed. */ 
            MSG_Free(pMsgIn);
            pMsgIn = NULL;
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
    FLib_MemSet(pScanReq->scanChannels+1, 0, sizeof(channelMask_t)-sizeof(uint32_t));
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

  Serial_Print(interfaceId, "Sending the MLME-Associate Request message to the MAC...", gAllowToBlock_d);
  
  /* Allocate a message for the MLME message. */
  pMsg = MSG_AllocType(mlmeMessage_t);
  if (pMsg != NULL)
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
#if mEnterLowPowerWhenIdle_c
  static uint8_t lowPowerCounter = 0;
#endif
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
#if gNvmTestActive_d      
      if(pMsg->msgData.pollCnf.status == gNoAck_c)
      {
          timeoutCounter++;
      }
      else
      {
          timeoutCounter = 0;
      }
#endif
#if mEnterLowPowerWhenIdle_c
    lowPowerCounter++;
    if ( lowPowerCounter > mDefaultValueOfMlmeHandlersToAllowSleep_c)
    {
        Led1Off();
        Led2Off();
        Led3Off();
        Led4Off();
        lowPowerCounter = 0;
        PWR_AllowDeviceToSleep();
    }
#endif
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
    /* Since we received data, the coordinator might have more to send. We 
       reduce the polling interval to raise the throughput while data is
       available. */
    mPollInterval = mDefaultValueOfPollIntervalFast_c;
    /* Allow another MLME-Poll request. */
    mWaitPollConfirm = FALSE;
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

/******************************************************************************
* The App_ReceiveUartData() function will check if it is time to send out an
* MLME-Poll request in order to receive data from the coordinator. If its time,
* and we are permitted then a poll request is created and sent.
* 
* The function uses the coordinator information gained during the Active Scan
* for building the MLME-Poll Request message. The message is sent to the MLME
* service access point in the MAC.
******************************************************************************/
static void    AppPollWaitTimeout(void *pData)
{ 
   /* Just to avoid the compiler warning */
  (void)pData;
   
  /* Check if we are permitted, and if it is time to send a poll request.
     The poll interval is adjusted dynamically to the current band-width
     requirements. */
  if( !mWaitPollConfirm )
  {
    /* This is an MLME-POLL.req command. */
    mlmeMessage_t *pMlmeMsg = MSG_AllocType(mlmeMessage_t);
    if(pMlmeMsg)
    {
      /* Create the Poll Request message data. */
      pMlmeMsg->msgType = gMlmePollReq_c;
 
      /* Use the coordinator information we got from the Active Scan. */
      FLib_MemCpy(&pMlmeMsg->msgData.pollReq.coordAddress, &mCoordInfo.coordAddress, 8);
      FLib_MemCpy(&pMlmeMsg->msgData.pollReq.coordPanId, &mCoordInfo.coordPanId, 2);
      pMlmeMsg->msgData.pollReq.coordAddrMode = mCoordInfo.coordAddrMode;
      pMlmeMsg->msgData.pollReq.securityLevel = gMacSecurityNone_c;

      /* Send the Poll Request to the MLME. */
      if( NWK_MLME_SapHandler( pMlmeMsg, macInstance ) == gSuccess_c )
      {
        /* Do not allow another Poll request before the confirm is received. */
        mWaitPollConfirm = TRUE; 
      }
    }
  }
 /* Restart timer. */
  TMR_StartLowPowerTimer(mTimer_c, gTmrSingleShotTimer_c ,mPollInterval, AppPollWaitTimeout, NULL );
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
    case gKBD_EventLongSW1_c:
        OSA_EventSet(mAppEvent, gAppEvtPressedRestoreNvmBut_c);
    case gKBD_EventLongSW2_c:
    case gKBD_EventLongSW3_c:
    case gKBD_EventLongSW4_c:
    case gKBD_EventSW1_c:
    case gKBD_EventSW2_c:
    case gKBD_EventSW3_c:
    case gKBD_EventSW4_c:
#if gTsiSupported_d
    case gKBD_EventSW5_c:    
    case gKBD_EventSW6_c:    
#endif
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
