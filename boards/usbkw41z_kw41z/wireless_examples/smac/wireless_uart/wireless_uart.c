/*!
* \file
*
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
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

#include "wireless_uart.h"
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#define SelfNotificationEvent()                         ((void)OSA_EventSet(gTaskEvent, gWUSelf_EVENT_c))


/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
osaEventId_t          gTaskEvent;
osaEventFlags_t       gTaskEventFlags; 

bool_t evDataFromUART;
bool_t haveBytes;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t mAppSer;
static uint8_t timePassed;       
#ifdef VERBOSE
static bool_t bCCAFailed;
static bool_t bACKFailed;
#endif
static bool_t bTxDone;
static bool_t bRxDone;
static bool_t evKeyPressed;
static bool_t bTxInProgress;
static uint8_t gau8TxDataBuffer[gMaxSmacSDULength_c  + sizeof(rxPacket_t)];  
static txPacket_t *gAppTxPacket;
static rxPacket_t *gAppRxPacket;
static txContextConfig_t txConfigContext;
static tmrTimerID_t AppDelayTmr;
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define gUART_RX_EVENT_c         (1<<0)
#define gMcps_Cnf_EVENT_c        (1<<1)
#define gMcps_Ind_EVENT_c        (1<<2)
#define gMlme_EdCnf_EVENT_c      (1<<3)
#define gMlme_CcaCnf_EVENT_c     (1<<4)
#define gMlme_TimeoutInd_EVENT_c (1<<5)
#define gWUSelf_EVENT_c          (1<<6)
#define gTimePassed_EVENT_c      (1<<7)
#define gKeyPressed_EVENT_c      (1<<8)  
#define gEventsAll               (gUART_RX_EVENT_c | gMcps_Cnf_EVENT_c | gMcps_Ind_EVENT_c | \
                                  gMlme_EdCnf_EVENT_c | gMlme_CcaCnf_EVENT_c | gMlme_TimeoutInd_EVENT_c | \
                                    gWUSelf_EVENT_c | gTimePassed_EVENT_c | gKeyPressed_EVENT_c)

#define FlaggedDelay_ms(a)       TMR_StartSingleShotTimer(AppDelayTmr, a, DelayTimeElapsed, NULL)

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static void DelayTimeElapsed(void*);
void UartRxCallBack(void * param);
static void HandleEvents(int32_t evSignals);
void MainThread(void const * taskParam);
void WU_Task(uint32_t params);
static void SerialUIStateMachine(void);
static void keyPressedHandler(uint8_t events);
static void UartTxCallBack(void* param);
#ifdef VERBOSE
static void PrintMenu(char * const pu8Menu[], uint8_t port);
#endif
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
extern void ResetMCU(void);
/*New section added*/
void InitProject(void);
void InitApp(void);
extern void InitSmac(void);
/************************************************************************************
*
* InitProject
*
************************************************************************************/

void InitProject(void)
{   
  /*Global Data init*/
  evDataFromUART                    = FALSE; 
#ifdef VERBOSE
  bACKFailed                        = FALSE;
  bCCAFailed                        = FALSE;
#endif
  bTxInProgress                     = FALSE;
  haveBytes                         = FALSE;
  bTxDone                           = FALSE;
  evKeyPressed                      = FALSE;
  txConfigContext.autoAck           = FALSE;
  txConfigContext.ccaBeforeTx       = FALSE;
  txConfigContext.retryCountAckFail = 0;
  txConfigContext.retryCountCCAFail = 0;
}
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
//(Management) Sap handler for managing timeout indication and ED confirm

smacErrors_t smacToAppMlmeSap(smacToAppMlmeMessage_t* pMsg, instanceId_t instance)
{
  switch(pMsg->msgType)
  {
  case gMlmeEdCnf_c:
     (void)OSA_EventSet(gTaskEvent, gMlme_EdCnf_EVENT_c);
    break;
  case gMlmeCcaCnf_c:
    (void)OSA_EventSet(gTaskEvent, gMlme_CcaCnf_EVENT_c);
    break;
  case gMlmeTimeoutInd_c:
     (void)OSA_EventSet(gTaskEvent, gMlme_TimeoutInd_EVENT_c);
     break;
  default:
    break;
  }
  MEM_BufferFree(pMsg);
  return gErrorNoError_c;
}
//(Data) Sap handler for managing data confirm and data indication
smacErrors_t smacToAppMcpsSap(smacToAppDataMessage_t* pMsg, instanceId_t instance)
{
  switch(pMsg->msgType)
  {
  case gMcpsDataInd_c:
    if(pMsg->msgData.dataInd.pRxPacket->rxStatus == rxSuccessStatus_c)
    {
      (void)OSA_EventSet(gTaskEvent, gMcps_Ind_EVENT_c);
    }
    break;
  case gMcpsDataCnf_c:
#ifdef VERBOSE
    if(pMsg->msgData.dataCnf.status == gErrorChannelBusy_c)
    {
      bCCAFailed = TRUE;
    }
    if(pMsg->msgData.dataCnf.status == gErrorNoAck_c)
    {
      bACKFailed = TRUE;
    }
#endif
    (void)OSA_EventSet(gTaskEvent, gMcps_Cnf_EVENT_c);
    break;
  default:
    break;
  }
  MEM_BufferFree(pMsg);
  return gErrorNoError_c;
}

/************************************************************************************
*
* HandleEvents: Performs actions based on event signals given as param
*
************************************************************************************/
static void HandleEvents(int32_t evSignals)
{
  if(evSignals & gUART_RX_EVENT_c)
  {
     evDataFromUART = TRUE;
  }
  if(evSignals & gMcps_Cnf_EVENT_c)
  {
    bTxDone = TRUE;
  }
  if(evSignals & gMcps_Ind_EVENT_c)
  {
    bRxDone = TRUE;
  }
  if(evSignals & gMlme_TimeoutInd_EVENT_c)
  {
  }
  if(evSignals & gMlme_EdCnf_EVENT_c)
  {
    
  }
  if(evSignals & gMlme_CcaCnf_EVENT_c)
  {
    
  }
  if(evSignals & gWUSelf_EVENT_c)
  {
  }
  if(evSignals & gTimePassed_EVENT_c)
  {
    timePassed = TRUE;
  }
}

/*************************************************************************/
/*InitApp: Initializes application mdoules and data*/
/*************************************************************************/
void InitApp()
{
  gAppTxPacket = (txPacket_t*)gau8TxDataBuffer;   //Map TX packet to buffer
  gAppRxPacket = (rxPacket_t*)MEM_BufferAlloc(gMaxSmacSDULength_c + sizeof(rxPacket_t));
  
  if(gAppRxPacket == NULL)
    panic(0,0,0,0);
  Serial_InitInterface( &mAppSer, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);//Get handle of serial interface
  Serial_SetBaudRate (mAppSer, gUARTBaudRate115200_c);   //Set 115200 as default baud
  Serial_SetRxCallBack(mAppSer, UartRxCallBack, NULL);   //Set Receive callback for uart 
  //Initialise SMAC
  InitSmac();
  //Tell SMAC who to call when it needs to pass a message to the application thread.
  Smac_RegisterSapHandlers((SMAC_APP_MCPS_SapHandler_t)smacToAppMcpsSap,(SMAC_APP_MLME_SapHandler_t)smacToAppMlmeSap,0);
  InitProject();
  
  SMACFillHeader(&(gAppTxPacket->smacHeader), gDefaultAddress_c);                  
  
  (void)MLMEPAOutputAdjust(gDefaultOutputPower_c);
  (void)MLMESetChannelRequest(gDefaultChannelNumber_c);         
  (void)MLMEConfigureTxContext(&txConfigContext);
  AppDelayTmr = TMR_AllocateTimer();
  gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
  (void)MLMERXEnableRequest(gAppRxPacket, 0);
}

void main_task(uint32_t param)
{
  static bool_t bIsInitialized = FALSE;
  static bool_t bUserInteraction = FALSE;
  //Initialize Memory Manager, Timer Manager and LEDs.
  if( !bIsInitialized )
  {
    hardware_init();
    
    MEM_Init();
    TMR_Init();
    LED_Init();
    //initialize PHY
    Phy_Init();
    //initialize Serial Manager
    SerialManager_Init();
    
    KBD_Init(keyPressedHandler);
    
    LED_StartSerialFlash(LED1);
    
    gTaskEvent = OSA_EventCreate(TRUE);
    InitApp();
    
#ifdef VERBOSE
    /*Prints the Welcome screens in the terminal*/  
    PrintMenu(cu8Logo, mAppSer);
    Serial_Print(mAppSer,"\r\n\r\n Press switch1 button to start application\r\n\r\n",gAllowToBlock_d);
#endif
    bIsInitialized = TRUE;
  }
  if(!bUserInteraction)
  {
    while(1)
    {
      (void)OSA_EventWait(gTaskEvent, gEventsAll, FALSE, osaWaitForever_c ,&gTaskEventFlags);
      if(evKeyPressed)
      {
        LED_StopFlashingAllLeds();
        SelfNotificationEvent();
        evKeyPressed = FALSE;
        bUserInteraction = TRUE;
        break;
      }
      if(gUseRtos_c == 0)
      {
        break;
      }
    }
  }
  if(bUserInteraction)
  {
    while(1)
    {
      (void)OSA_EventWait(gTaskEvent, gEventsAll, FALSE, osaWaitForever_c ,&gTaskEventFlags);
      HandleEvents(gTaskEventFlags);
      SerialUIStateMachine();  
      if (gUseRtos_c == 0)
      {
        break;
      } 
    }
  } 
}

/************************************************************************************
*
* Wireless Uart state machine
*
************************************************************************************/
void SerialUIStateMachine(void)
{
  uint16_t byteCount;
  uint16_t readBytes;
//  If something is received OTA, print it.
  if(bRxDone)
  {
    bRxDone = FALSE;
    //send received buffer to Serial Manager to print it.
    Serial_AsyncWrite(mAppSer, gAppRxPacket->smacPdu.smacPdu, gAppRxPacket->u8DataLength, UartTxCallBack, gAppRxPacket);
    //allocate another buffer to start receptioning Asap
    gAppRxPacket = (rxPacket_t*)MEM_BufferAlloc(gMaxSmacSDULength_c + sizeof(rxPacket_t));
    //problem with Allocator. Application needs to hang.
    if(gAppRxPacket == NULL)
      panic(0,0,0,0);
    //get back in RX
    gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
    MLMERXEnableRequest(gAppRxPacket, 0); 
  }
  //if byte received over UART
  if(evDataFromUART)
  {
    evDataFromUART = FALSE;

    Serial_RxBufferByteCount(mAppSer, &byteCount);
    if(byteCount < gMaxSmacSDULength_c)
    { 
      TMR_StopTimer(AppDelayTmr);
      timePassed = FALSE;
      FlaggedDelay_ms(7);
    }
    //buffer was filled before scheduling a timer
    else if(!timePassed && !TMR_IsTimerActive(AppDelayTmr))
    {
      //simulate a timer event
      OSA_EventSet(gTaskEvent, gTimePassed_EVENT_c);
    }
  }  
  if(timePassed && !bTxInProgress)
  {
    timePassed = FALSE;
    if(gSerial_Success_c != Serial_Read(mAppSer, gAppTxPacket->smacPdu.smacPdu,
                                        gMaxSmacSDULength_c, &readBytes))
    {
      return;
    }
    gAppTxPacket->u8DataLength = readBytes;
    (void)MLMERXDisableRequest();
    (void)MCPSDataRequest(gAppTxPacket);
    bTxInProgress = TRUE;
  }
  //if data send OTA check if bytes are pending to be read from Serial Manager's buffers. If not, go back in RX
  if(bTxDone)
  {
    bTxDone = FALSE;
    bTxInProgress = FALSE;
#ifdef VERBOSE
    if(bCCAFailed == TRUE || bACKFailed == TRUE)
    {
      bCCAFailed = FALSE;
      bACKFailed = FALSE;
      Serial_Print(mAppSer, "(failed)", gAllowToBlock_d);
    }
#endif
    Serial_RxBufferByteCount(mAppSer, &byteCount);
    //if timer fired an event which is not handled,
    //notify the application task so that the timer event will be handled
    if(timePassed)
    {
      SelfNotificationEvent();
    }
    else if(byteCount)
    {
      OSA_EventSet(gTaskEvent, gUART_RX_EVENT_c);
    }
    else
    {
      (void)MLMERXEnableRequest(gAppRxPacket, 0);
    }
  }
}

/*****************************************************************************
* UartRxCallBack function
*
* Interface assumptions:
* This callback is triggered when a new byte is received over the UART
*
* Return Value:
* None
*****************************************************************************/
void UartRxCallBack(void * param) 
{
  (void)OSA_EventSet(gTaskEvent, gUART_RX_EVENT_c);
}
/*****************************************************************************
* UartTxCallBack function
*
* Interface assumptions:
* This callback is triggered when asynchronous request is honored by Serial Manager
*
*Parameters:
* param: in this scenario is pointer to Rx Packet that was previously printed and
*        needs to be freed.
*
* Return Value:
* None
*****************************************************************************/
void UartTxCallBack(void* param)
{
  memStatus_t status = MEM_BufferFree(param);
  if(status!=MEM_SUCCESS_c)
    panic(0,0,0,0);
}
/*****************************************************************************
* keyPressedHandler function
*
* Interface assumptions:
* This callback is triggered when Switch 1 button is pressed.
*
*Parameters:
* events: event flags
*
* Return Value:
* None
*****************************************************************************/

void keyPressedHandler(uint8_t events)
{
  evKeyPressed = TRUE;
  (void)OSA_EventSet(gTaskEvent, gKeyPressed_EVENT_c);
}
/*****************************************************************************
* DelayTimeElapsed function
*
* Interface assumptions:
* This callback is triggered when the application timer fires an event.
*
*Parameters:
* params: callback parameter
*
* Return Value:
* None
*****************************************************************************/

void DelayTimeElapsed(void* param)
{
  (void)param;
  (void)OSA_EventSet(gTaskEvent, gTimePassed_EVENT_c);
}
/****************************************************************************
*******************************Utilities*************************************
****************************************************************************/
#ifdef VERBOSE
void PrintMenu(char * const pu8Menu[], uint8_t port)
{
  uint8_t u8Index = 0;
  (void)port;
  while(pu8Menu[u8Index]){
    Serial_Print(port, pu8Menu[u8Index], gAllowToBlock_d);
    u8Index++;
  }
}
#endif
/***********************************************************************
************************************************************************/