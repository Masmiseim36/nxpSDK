/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
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

#include "wireless_messenger.h"
#include "SMAC_Config.h"
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
#define SelfNotificationEvent()                         ((void)OSA_EventSet(gTaskEvent, gWMSelf_EVENT_c))

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
#if gSmacUseSecurity_c
static uint8_t gau8RxDataBuffer[gMaxSmacSDULength_c  + sizeof(rxPacket_t) + 16];                         
#else
static uint8_t gau8RxDataBuffer[gMaxSmacSDULength_c  + sizeof(rxPacket_t)];
#endif
static uint8_t gau8TxDataBuffer[gMaxSmacSDULength_c  + sizeof(txPacket_t)];                        

static txPacket_t * gAppTxPacket;
static rxPacket_t * gAppRxPacket;

osaEventId_t          gTaskEvent;
osaEventFlags_t       gTaskEventFlags;

uint8_t gu8UartData;
                                              
channels_t       testChannel;
uint8_t          testPower;
address_size_t*  pIDConfig;
address_size_t   shortSourceID;
address_size_t   shortDestinationID;
address_size_t   shortPanID;
                                                                                           
bool_t evDataFromUART;
bool_t evTestParameters;

bool_t shortCutsEnabled;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t mAppSer;
           
static bool_t bCCAFailed;
static bool_t bACKFailed;
static bool_t bTxDone;
static bool_t bRxDone;

static WMStates_t gWMState;
static txContextConfig_t txConfigContext;
static ConfigMenuStates_t configMenuState;
static ConsoleMenuStates_t consoleMenuState;
#if gSmacUseSecurity_c
static SecurityMenuStates_t securityMenuState;
uint8_t KEY[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
uint8_t  IV[16]={0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
#endif
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
#define gWMSelf_EVENT_c          (1<<6)
#define gTimePassed_EVENT_c      (1<<7)

#define gEventsAll_c	(gUART_RX_EVENT_c | gMcps_Cnf_EVENT_c | gMcps_Ind_EVENT_c | \
			gMlme_EdCnf_EVENT_c | gMlme_CcaCnf_EVENT_c | gMlme_TimeoutInd_EVENT_c | \
			gWMSelf_EVENT_c | gTimePassed_EVENT_c)

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

void UartRxCallBack(void * param);
static void HandleEvents(int32_t evSignals);
static uint16_t HexString2Dec16(uint8_t * au8String);
//static bool_t stringComp(uint8_t * au8leftString, uint8_t * au8RightString, uint8_t bytesToCompare);

static void SerialUIStateMachine(void);
//static void DelayTimeElapsed();
static void ShortCutsParser(uint8_t u8UartData);
static void PrintTestParameters(bool_t bEraseLine);
static bool_t ConsoleWM();
static bool_t ConfigureWM();
#if gSmacUseSecurity_c
static bool_t SecurityConfig();
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
void InitSmac(void);
void Clear_CurrentLine();

/************************************************************************************
*
* InitProject
*
************************************************************************************/
void InitProject(void)
{   
  /*Global Data init*/
  testChannel                       = gDefaultChannelNumber_c;
  testPower                         = gDefaultOutputPower_c;
  gWMState                          = gWMIdleState_c;
  shortCutsEnabled                  = FALSE; 
  evDataFromUART                    = FALSE; 
  bACKFailed                        = FALSE;
  bCCAFailed                        = FALSE;
  txConfigContext.autoAck           = FALSE;
  txConfigContext.ccaBeforeTx       = FALSE;
  txConfigContext.retryCountAckFail = 0;
  txConfigContext.retryCountCCAFail = 0;
  shortSourceID                     = gNodeAddress_c;
  shortPanID                        = gDefaultPanID_c;
  shortDestinationID                = gBroadcastAddress_c;
}

/************************************************************************************
*************************************************************************************
* SAP functions
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
    if(pMsg->msgData.dataCnf.status == gErrorChannelBusy_c)
    {
      bCCAFailed = TRUE;
    }
    if(pMsg->msgData.dataCnf.status == gErrorNoAck_c)
    {
      bACKFailed = TRUE;
    }
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
* Performs actions based on the event flags given as param
*
************************************************************************************/
static void HandleEvents(int32_t evSignals)
{
  uint16_t u16SerBytesCount = 0;
  
  if(evSignals & gUART_RX_EVENT_c)
  {
    if(gSerial_Success_c == Serial_GetByteFromRxBuffer(mAppSer, &gu8UartData, &u16SerBytesCount))
    {
      if(shortCutsEnabled)
      {
        ShortCutsParser(gu8UartData);  
      }
      else
      {
        evDataFromUART = TRUE;
      }
      Serial_RxBufferByteCount(mAppSer, &u16SerBytesCount);
      if(u16SerBytesCount)
      {
        (void)OSA_EventSet(gTaskEvent, gUART_RX_EVENT_c);
      }
    }
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
  if(evSignals & gWMSelf_EVENT_c)
  {
  }
}
/*************************************************************************
*Main Task: Application entry point*
**************************************************************************/
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

#if gSmacUseSecurity_c
    SecLib_Init();
#endif
	
    LED_StartSerialFlash(LED1);
    
    gTaskEvent = OSA_EventCreate(TRUE);
    InitApp();
    
    /*Prints the Welcome screens in the terminal*/  
    PrintMenu(cu8Logo, mAppSer);
    bIsInitialized = TRUE;
  }
  if(!bUserInteraction)
  {
    while(1)
    {
      (void)OSA_EventWait(gTaskEvent, gEventsAll_c, FALSE, osaWaitForever_c ,&gTaskEventFlags);
      HandleEvents(gTaskEventFlags);
      if(evDataFromUART)
      {
        evDataFromUART = FALSE;
        if(gu8UartData == '\r')
        {
          LED_StopFlashingAllLeds();
          SelfNotificationEvent();
          bUserInteraction = TRUE;
          break;
        }
        else
        {
          PrintMenu(cu8Logo, mAppSer);
        }
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
      (void)OSA_EventWait(gTaskEvent, gEventsAll_c, FALSE, osaWaitForever_c ,&gTaskEventFlags);
      HandleEvents(gTaskEventFlags);
      SerialUIStateMachine();  
      if (gUseRtos_c == 0)
      {
        break;
      } 
    }
  } 
}

/******************************************************************************
*InitApp
******************************************************************************/
void InitApp()
{
//  AppDelayTmr  = TMR_AllocateTimer();             //Allocate a timer for inserting delays whenever needed
  
  gAppTxPacket = (txPacket_t*)gau8TxDataBuffer;   //Map TX packet to buffer
  gAppRxPacket = (rxPacket_t*)gau8RxDataBuffer;   //Map Rx packet to buffer     
#if gSmacUseSecurity_c
  gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c + 16;
#else
  gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
#endif
  
  Serial_InitInterface( &mAppSer, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE); //Get handle of uart interface
  Serial_SetBaudRate (mAppSer, gUARTBaudRate115200_c);   //Set 115200 as default baud
  Serial_SetRxCallBack(mAppSer, UartRxCallBack, NULL);   //Set Receive callback for uart 
  //Initialise SMAC
  InitSmac();
  //Tell SMAC who to call when it needs to pass a message to the application thread.
  Smac_RegisterSapHandlers((SMAC_APP_MCPS_SapHandler_t)smacToAppMcpsSap,(SMAC_APP_MLME_SapHandler_t)smacToAppMlmeSap,0);
  
  InitProject();
  
  SMACFillHeader(&(gAppTxPacket->smacHeader), shortDestinationID);                   //Destination Address is set in InitProject;
  
#ifdef gPHY_802_15_4g_d
  (void)MLMESetPhyMode(gDefaultMode1_c);
#endif
  (void)MLMEPAOutputAdjust(testPower);
  (void)MLMESetChannelRequest(testChannel);
  
#if gSmacUseSecurity_c
  SMAC_SetIVKey(KEY, IV);
#endif
}

/************************************************************************************
*
* Wireless Messenger State Machine
*
************************************************************************************/
void SerialUIStateMachine(void)
{
  if((gWMSelectOption_c == gWMState) && evTestParameters)
  {
    (void)MLMESetChannelRequest(testChannel);                                   
    (void)MLMEPAOutputAdjust(testPower);
    PrintTestParameters(TRUE);
    evTestParameters = FALSE;
  }
  switch(gWMState)
  {
  case gWMIdleState_c:
    PrintMenu(cu8MainMenu, mAppSer);
    PrintTestParameters(FALSE);
    shortCutsEnabled = TRUE;
    gWMState = gWMSelectOption_c;
    break;
  case gWMSelectOption_c:
    if(evDataFromUART)
    {
      if('1' == gu8UartData)
      {
        configMenuState = gConfigMenuInit_c;
        gWMState        = gWMConfiguration_c;
      }
      else if('2' == gu8UartData)
      {
        consoleMenuState  = gConsoleMenuInit_c;
        gWMState          = gWMConsole_c;
      }
#if gSmacUseSecurity_c
      else if('3' == gu8UartData)
      {
        securityMenuState = gSecurityMenuInit_c;
        gWMState          = gWMSec_c;
      }
#endif
      else if( '!' == gu8UartData)
      {
        ResetMCU();
      }
      evDataFromUART = FALSE;
      SelfNotificationEvent();
    }
    break;
  case gWMConfiguration_c:
    if(ConfigureWM())
    {
      gWMState = gWMIdleState_c;
      SelfNotificationEvent();
    }
    break;
  case gWMConsole_c:
    if(ConsoleWM())
    {
      gWMState = gWMIdleState_c;
      SelfNotificationEvent();
    }
    break;
#if gSmacUseSecurity_c
  case gWMSec_c:
    if(SecurityConfig())
    {
      gWMState = gWMIdleState_c;
      SelfNotificationEvent();
    }
    break;
#endif
  default:
    break;
  }
}
#if gSmacUseSecurity_c
bool_t SecurityConfig(void)
{
  bool_t bBackFlag = FALSE;
  static uint8_t ctr = 0;
  static uint8_t idx = 0;
  static uint8_t hexString[3]={0};
  
  switch(securityMenuState)
  {
  case gSecurityMenuInit_c:
    Serial_Print(mAppSer, "\f\r\n\r\n Security parameters configuration \r\n\r\n", gAllowToBlock_d);
    Serial_Print(mAppSer, "KEY[0] = 0x", gAllowToBlock_d);
    securityMenuState = gSecurityMenuInputKEY_c;
    shortCutsEnabled = FALSE;
    break;
  case gSecurityMenuInputKEY_c:
    if(evDataFromUART)
    {
      evDataFromUART = FALSE;
      if(isAsciiHex(gu8UartData))
      {
        uint8_t auxStr[2];
        auxStr[0] = gu8UartData;
        auxStr[1] = 0;
        Serial_Print(mAppSer, (char*)auxStr, gAllowToBlock_d);
        hexString[idx] = gu8UartData;
        idx = (idx+1) & 1;
        ctr = ctr + ((idx+1)&1);
        
        if(idx == 0)
        {
          KEY[ctr-1] = (uint8_t)HexString2Dec16(hexString);
          if(ctr == 16)
          {
            Serial_Print(mAppSer, "\r\nIV[0] = 0x", gAllowToBlock_d);
            ctr = 0;
            securityMenuState = gSecurityMenuInputIV_c;
          }
          else
          {
            Serial_Print(mAppSer, "\r\nKEY[", gAllowToBlock_d);
            Serial_PrintDec(mAppSer, ctr);
            Serial_Print(mAppSer, "] = 0x", gAllowToBlock_d);
          }
        }
      }
    }
    break;
  case gSecurityMenuInputIV_c:
    if(evDataFromUART)
    {
      evDataFromUART = FALSE;
      if(isAsciiHex(gu8UartData))
      {
        uint8_t auxStr[2];
        auxStr[0] = gu8UartData;
        auxStr[1] = 0;
        Serial_Print(mAppSer, (char*)auxStr, gAllowToBlock_d);
        hexString[idx] = gu8UartData;
        idx = (idx+1) & 1;
        ctr = ctr + ((idx+1)&1);
        
        if(idx == 0)
        {
          IV[ctr-1] = (uint8_t)HexString2Dec16(hexString);
          if(ctr == 16)
          {
            ctr = 0;
            bBackFlag = TRUE;
            shortCutsEnabled = TRUE;
            SMAC_SetIVKey(KEY, IV);
          }
          else
          {
            Serial_Print(mAppSer, "\r\nIV[", gAllowToBlock_d);
            Serial_PrintDec(mAppSer, ctr);
            Serial_Print(mAppSer, "] = 0x", gAllowToBlock_d);
          }
        }
      }
    }
    break;
  default:
    break;
  }
  return bBackFlag;
}
#endif
/************************************************************************************
*
* Configures application's runtime parameters (addressing, channel, power, etc)
*
************************************************************************************/
bool_t ConfigureWM()
{
  bool_t bBackFlag = FALSE;
  static uint8_t inputString[] = {'\0','\0','\0','\0','\0'};
  uint8_t tempArray[2];
  static uint8_t inputStringIndex = 0;
  if(evTestParameters)
  {
    (void)MLMESetChannelRequest(testChannel);                                 
    (void)MLMEPAOutputAdjust(testPower);
    if(configMenuState == gConfigMenuSelectOption_c)
    {
      PrintTestParameters(TRUE);
    }
    else
    {
      PrintTestParameters(FALSE);
      Serial_Print(mAppSer, "\r\n", gAllowToBlock_d);     
    }
    evTestParameters = FALSE;
  }
  switch(configMenuState)
  {
  case gConfigMenuInit_c:
    PrintMenu(cu8ShortCutsBar, mAppSer);
    PrintMenu(cu8ConfigMenu, mAppSer); 
    shortCutsEnabled = TRUE;
    FLib_MemCpy(inputString, "\0\0\0\0\0", 5);
    inputStringIndex = 0;
    configMenuState = gConfigMenuSelectOption_c;
    PrintTestParameters(FALSE);
    break;
  case gConfigMenuSelectOption_c:
    if(evDataFromUART)
    {
      switch(gu8UartData)
      {
      case '1':
        configMenuState = gConfigMenuInputPan_c;
        shortCutsEnabled = FALSE;
        SelfNotificationEvent();
        break;
      case '2':
        configMenuState = gConfigMenuInputSrc_c;
        shortCutsEnabled = FALSE;
        SelfNotificationEvent();
        break;
      case '3':
        configMenuState = gConfigMenuInputDst_c;
        shortCutsEnabled = FALSE;
        SelfNotificationEvent();
        break;
      case 'p':
        configMenuState = gConfigMenuInit_c;
        bBackFlag = TRUE;
        SelfNotificationEvent();
        break;
      default:
        break;
      }
      evDataFromUART = FALSE;
    }
    break;
  case gConfigMenuInputPan_c:
    pIDConfig = &shortPanID;
    Serial_Print(mAppSer, "\r\n\r\nPlease input short pan address and press [ENTER]: 0x",gAllowToBlock_d);
    configMenuState = gConfigMenuInputValue_c;
    break;
  case gConfigMenuInputDst_c:
    pIDConfig = &shortDestinationID;
    Serial_Print(mAppSer, "\r\n\r\nPlease input short destination address and press [ENTER]: 0x",gAllowToBlock_d);
    configMenuState = gConfigMenuInputValue_c;
    break;
  case gConfigMenuInputSrc_c:
    pIDConfig = &shortSourceID;
    Serial_Print(mAppSer, "\r\n\r\nPlease input short source address and press [ENTER]: 0x",gAllowToBlock_d);
    configMenuState = gConfigMenuInputValue_c;
    break;
  case gConfigMenuInputValue_c:
    if(evDataFromUART)
    {
      if(gu8UartData == '\r')
      {
        if(inputStringIndex != (2*(sizeof(address_size_t)/sizeof(uint8_t))) )
        {
#if !gUseSMACLegacy_c
          Serial_Print(mAppSer, "\r\n Input value is between 0x0000 and 0xFFFF. Press [ENTER] to continue!\r\n", gAllowToBlock_d);
#else
          Serial_Print(mAppSer, "\r\n Input value is between 0x00 and 0xFF. Press [ENTER] to continue!\r\n", gAllowToBlock_d);
#endif
          configMenuState = gConfigMenuEnd_c;
        }
        else
        {
          *pIDConfig = HexString2Dec16(inputString);
          SMACSetPanID(shortPanID);
          SMACSetShortSrcAddress(shortSourceID);
          SMACFillHeader(&(gAppTxPacket->smacHeader), shortDestinationID);
          Serial_Print(mAppSer, "\r\nValue stored. Press [ENTER] to continue! \r\n", gAllowToBlock_d);              
          configMenuState = gConfigMenuEnd_c;
        }
      }
      else if(isAsciiHex(gu8UartData))
      {
        if(inputStringIndex < (2*(sizeof(address_size_t)/sizeof(uint8_t))))
        {
          inputString[inputStringIndex++] = gu8UartData;
          tempArray[0] = gu8UartData;
          tempArray[1] = '\0';
          Serial_Print(mAppSer,(char*)tempArray,gAllowToBlock_d);
        }
        else
        {
          Serial_Print(mAppSer,"\r\n\r\nAddress length must be ", gAllowToBlock_d);
          Serial_PrintDec(mAppSer, sizeof(address_size_t));
          Serial_Print(mAppSer," byte(s)\r\n\r\n",gAllowToBlock_d);
          Serial_Print(mAppSer,"Press [ENTER] to continue!\r\n",gAllowToBlock_d);
          inputStringIndex = 0;
          configMenuState = gConfigMenuEnd_c;
          SelfNotificationEvent();
        }
      }
      evDataFromUART = FALSE;
    }
  case gConfigMenuEnd_c:
    if(evDataFromUART && gu8UartData == '\r')
    {
      configMenuState = gConfigMenuInit_c;
      evDataFromUART = FALSE;
      SelfNotificationEvent();
    }
  default:
    break;
  }
  return bBackFlag;
}
/************************************************************************************
*
* Manages the console option in the menu
*
************************************************************************************/
bool_t ConsoleWM()
{
  bool_t bBackFlag = FALSE;
  static uint8_t* pPacketPdu;
  static uint8_t u8PacketIndex;
  smacErrors_t err;
  uint8_t tempArray[2];
  
  if(evTestParameters)
  {
    (void)MLMESetChannelRequest(testChannel);                                 
    (void)MLMEPAOutputAdjust(testPower);
    evTestParameters = FALSE;
  }
  
  switch(consoleMenuState)
  {
  case gConsoleMenuInit_c:
    shortCutsEnabled = FALSE;
    Serial_Print(mAppSer, "\r\n\r\nInitializing Wireless Console... Exit with [ESC]\r\n\r\n", gAllowToBlock_d);
#if gSmacUseSecurity_c
    FLib_MemSet(gAppRxPacket->smacPdu.smacPdu, 0, gMaxSmacSDULength_c + 16);
    gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c + 16;
#else
    FLib_MemSet(gAppRxPacket->smacPdu.smacPdu, 0, gMaxSmacSDULength_c);
    gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
#endif
    MLMERXEnableRequest(gAppRxPacket, 0);
    consoleMenuState = gConsoleMenuPrepare_c;
    err = MLMEConfigureTxContext(&txConfigContext);
    if(err == gErrorOutOfRange_c)
    {
      Serial_Print(mAppSer, "\r\n\r\n\tSending failed. Reason: Cannot have more that gMaxRetriesAllowed_c retries\r\n\r\n",gAllowToBlock_d);
      consoleMenuState = gConsoleMenuInit_c;
      bBackFlag = TRUE;
    }
    SelfNotificationEvent();
    break;
  case gConsoleMenuPrepare_c:
    pPacketPdu = gAppTxPacket->smacPdu.smacPdu;
    FLib_MemSet(gAppTxPacket->smacPdu.smacPdu, 0, gMaxSmacSDULength_c);
    u8PacketIndex = 0;
    Serial_Print(mAppSer, "\f\r\n\r\n>", gAllowToBlock_d);
    consoleMenuState = gConsoleMenuWaitEv_c;
    break;
  case gConsoleMenuWaitEv_c:
    if(evDataFromUART)
    {
      if(gu8UartData == 0x1B)
      {
        bBackFlag = TRUE;
        (void)MLMERXDisableRequest();
        consoleMenuState = gConsoleMenuInit_c;
      }
      else if(gu8UartData == '\r' && u8PacketIndex != 0)
      {
        consoleMenuState = gConsoleMenuSendMessage_c;
        SelfNotificationEvent();
      }
      else
      {
        if(gu8UartData == '\b')
        {
          if(u8PacketIndex > 0)
          {
            u8PacketIndex--;
            Serial_Print(mAppSer,"\b \b",gAllowToBlock_d);
          }
        }
        else
        {
          if(!('\r' == gu8UartData && u8PacketIndex == 0))
          {
            pPacketPdu[u8PacketIndex++] = gu8UartData;
            tempArray[0] = gu8UartData;
            tempArray[1] = '\0';
            Serial_Print(mAppSer,(char*)tempArray, gAllowToBlock_d);
          }
          if(u8PacketIndex >= gMaxSmacSDULength_c)
          {
            Serial_Print(mAppSer,"\r\n Error: The message you are trying to send is larger than gMaxSmacSDULength_c\r\n",gAllowToBlock_d);
            consoleMenuState = gConsoleMenuPrepare_c;
            SelfNotificationEvent();
          }
        }
      }
      evDataFromUART = FALSE;
    }
    if(bRxDone)
    {
      Clear_CurrentLine();
#if !gUseSMACLegacy_c
      Serial_Print(mAppSer,"[0x",gAllowToBlock_d);
      Serial_PrintHex(mAppSer, (uint8_t*)(&(gAppRxPacket->smacHeader.srcAddr)), 2, 0);
      Serial_Print(mAppSer,"]: ",gAllowToBlock_d);
#else
      Serial_Print(mAppSer,"[...]: ", gAllowToBlock_d);
#endif
      Serial_Print(mAppSer,(char*)gAppRxPacket->smacPdu.smacPdu, gAllowToBlock_d);
      Serial_Print(mAppSer,"\r\n\r\n>", gAllowToBlock_d);
      Serial_Print(mAppSer,(char*)gAppTxPacket->smacPdu.smacPdu, gAllowToBlock_d);
      bRxDone = FALSE;
#if gSmacUseSecurity_c
      FLib_MemSet(gAppRxPacket->smacPdu.smacPdu, 0, gMaxSmacSDULength_c + 16);
      gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c + 16;
#else
      FLib_MemSet(gAppRxPacket->smacPdu.smacPdu, 0, gMaxSmacSDULength_c);
      gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
#endif
      (void)MLMERXEnableRequest(gAppRxPacket, 0);
    }
    break;
  case gConsoleMenuSendMessage_c:
    if(bRxDone)
    {
      Clear_CurrentLine();
#if !gUseSMACLegacy_c
      Serial_Print(mAppSer,"[0x",gAllowToBlock_d);
      Serial_PrintHex(mAppSer, (uint8_t*)(&(gAppRxPacket->smacHeader.srcAddr)), 2, 0);
      Serial_Print(mAppSer,"]: ",gAllowToBlock_d);
#else
      Serial_Print(mAppSer,"[...]: ", gAllowToBlock_d);
#endif
      Serial_Print(mAppSer,(char*)gAppRxPacket->smacPdu.smacPdu, gAllowToBlock_d);
      Serial_Print(mAppSer,"\r\n\r\n>", gAllowToBlock_d);
      Serial_Print(mAppSer,(char*)gAppTxPacket->smacPdu.smacPdu, gAllowToBlock_d);
      bRxDone = FALSE;
#if gSmacUseSecurity_c
      FLib_MemSet(gAppRxPacket->smacPdu.smacPdu, 0, gMaxSmacSDULength_c + 16);
      gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c + 16;
#else
      FLib_MemSet(gAppRxPacket->smacPdu.smacPdu, 0, gMaxSmacSDULength_c);
      gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
#endif
      (void)MLMERXEnableRequest(gAppRxPacket, 0);
    }
    
    gAppTxPacket->u8DataLength = u8PacketIndex;
    
    (void)MLMERXDisableRequest();
    err = MCPSDataRequest(gAppTxPacket);
    
    if(err != gErrorNoError_c)
    {
      if(err == gErrorOutOfRange_c)
      {
        Serial_Print(mAppSer, "\r\n\r\n\tSending failed. Reason: Packet is either too big or too small in size\r\n\r\n", gAllowToBlock_d);
      }
      else
      {
        Serial_Print(mAppSer, "\r\n\r\n\tSending failed. Reason: SMAC is busy\r\n\r\n", gAllowToBlock_d);
      }
      consoleMenuState = gConsoleMenuPrepare_c;
      SelfNotificationEvent();
    }
    else
    {
      consoleMenuState = gConsoleMenuWaitingConfirmation_c;
    }
    break;
  case gConsoleMenuWaitingConfirmation_c:
    if(bTxDone)
    {
      if(bACKFailed)
      {
        Serial_Print(mAppSer,"\r\n\r\nPacket Sending Failed. Reason: No Ack!\r\n",gAllowToBlock_d);
        bACKFailed = FALSE;
      }
      else if(bCCAFailed)
      {
        Serial_Print(mAppSer,"\r\n\r\nPacket Sending Failed. Reason. Channel Busy!\r\n",gAllowToBlock_d);
        bCCAFailed = FALSE;
      }
      else
      {
        Serial_Print(mAppSer,"\r\n\r\nPacket Sent\r\n",gAllowToBlock_d);
      }
      bTxDone = FALSE;
      consoleMenuState = gConsoleMenuPrepare_c;
#if gSmacUseSecurity_c
      gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c + 16;
#else
      gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
#endif
      (void)MLMERXEnableRequest(gAppRxPacket, 0);
      SelfNotificationEvent();
    }
    break;
  default:
    break;
  }
  return bBackFlag;
}
/************************************************************************************
*
* Prints parameters related menu (shortcut menu)
*
************************************************************************************/
void PrintTestParameters(bool_t bEraseLine)
{
  uint8_t u8Index1;
  
  if(bEraseLine)
  {
    Serial_Print(mAppSer,"\r",gAllowToBlock_d);
    for(u8Index1 = 0; u8Index1 < 110; u8Index1++)
    {
      Serial_Print(mAppSer," ",gAllowToBlock_d);
    }
    Serial_Print(mAppSer,"\r",gAllowToBlock_d);
  }
  
  Serial_Print(mAppSer, " Channel ", gAllowToBlock_d);
  Serial_PrintDec(mAppSer, (uint32_t)testChannel);
  Serial_Print(mAppSer,", Power ", gAllowToBlock_d);
  Serial_PrintDec(mAppSer,(uint32_t)testPower);
  Serial_Print(mAppSer,", CCA Fail Retries ", gAllowToBlock_d);
  Serial_PrintDec(mAppSer, (uint32_t)txConfigContext.retryCountCCAFail);
  Serial_Print(mAppSer,"/",gAllowToBlock_d);
  Serial_PrintDec(mAppSer,(uint32_t)gMaxRetriesAllowed_c);
  Serial_Print(mAppSer,", Ack Fail Retries ", gAllowToBlock_d);
  Serial_PrintDec(mAppSer, (uint32_t)txConfigContext.retryCountAckFail);
  Serial_Print(mAppSer,"/",gAllowToBlock_d);
  Serial_PrintDec(mAppSer,(uint32_t)gMaxRetriesAllowed_c);
  Serial_Print(mAppSer,", Pan: 0x", gAllowToBlock_d);
  Serial_PrintHex(mAppSer, (uint8_t*)&shortPanID, sizeof(address_size_t), gPrtHexNoFormat_c);
  Serial_Print(mAppSer,", Src: 0x", gAllowToBlock_d);
  Serial_PrintHex(mAppSer, (uint8_t*)&shortSourceID, sizeof(address_size_t), gPrtHexNoFormat_c);
  Serial_Print(mAppSer,", Dst: 0x", gAllowToBlock_d);
  Serial_PrintHex(mAppSer, (uint8_t*)&shortDestinationID, sizeof(address_size_t), gPrtHexNoFormat_c);
  Serial_Print(mAppSer," >", gAllowToBlock_d);
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

/************************************************************************************
*
* Performs actions based on shortcut key pressed in the menus where this is allowed
*
************************************************************************************/
void ShortCutsParser(uint8_t u8UartData)
{
  evTestParameters = TRUE;
  evDataFromUART = FALSE;
  switch(u8UartData){
  case 'q': 
    if(gMaxChannel_c == testChannel)
      testChannel = gMinChannel_c;
	else
	  testChannel++;
    break;
  case 'w':
    if(gMinChannel_c == testChannel)
      testChannel = gMaxChannel_c;
    else
      testChannel--;
    break;
  case 'a':
    testPower++;
    if(gMaxOutputPower_c < testPower){
      testPower = gMinOutputPower_c;
    }
    break;
  case 's':
    if(testPower == gMinOutputPower_c)
      testPower = gMaxOutputPower_c;
    else
      testPower--;
    break;
  case 'z':
    if(txConfigContext.retryCountCCAFail >= gMaxRetriesAllowed_c)
    {
      txConfigContext.retryCountCCAFail = 0;
      txConfigContext.ccaBeforeTx = FALSE;
    }
    else
    {
      txConfigContext.retryCountCCAFail++;
      txConfigContext.ccaBeforeTx = TRUE;
    }
    break;
  case 'x':
    if(txConfigContext.retryCountCCAFail > 0)
    {
      txConfigContext.retryCountCCAFail--;
      if( 0 == txConfigContext.retryCountCCAFail)
      {
        txConfigContext.ccaBeforeTx = FALSE;
      }
    }
    else
    {
      txConfigContext.retryCountCCAFail = gMaxRetriesAllowed_c;
      txConfigContext.ccaBeforeTx = TRUE;
    }
    break;
    case 'r':
#if !gUseSMACLegacy_c
    if(txConfigContext.retryCountAckFail >= gMaxRetriesAllowed_c)
    {
      txConfigContext.retryCountAckFail = 0;
      txConfigContext.autoAck           = FALSE;
    }
    else
    {
      txConfigContext.retryCountAckFail++;
      txConfigContext.autoAck = TRUE;
    }
#endif
    break;
    case 't':
#if !gUseSMACLegacy_c
    if(txConfigContext.retryCountAckFail > 0)
    {
      txConfigContext.retryCountAckFail--;
      if( 0 == txConfigContext.retryCountAckFail)
      {
        txConfigContext.autoAck = FALSE;
      }
    }
    else
    {
      txConfigContext.retryCountAckFail = gMaxRetriesAllowed_c;
      txConfigContext.autoAck           = TRUE;
    }
#endif
    break;
  default:
    evDataFromUART = TRUE;
    evTestParameters = FALSE;
    break;
  }
}
/***********************************************************************
*********************Utilities Software********************************
************************************************************************/

uint16_t HexString2Dec16(uint8_t * au8String)
{
  uint8_t u8LocIndex=0;
  uint8_t u8LocIndex2=0;
  uint16_t u16DecValue = 0;
  
  while(au8String[u8LocIndex]){
    u8LocIndex++;
  }
  
  while(u8LocIndex--){
    if((au8String[u8LocIndex] >= '0') && (au8String[u8LocIndex] <= '9'))
      u16DecValue |= ((uint16_t)(au8String[u8LocIndex] - '0'))<<(u8LocIndex2*4);
    else if((au8String[u8LocIndex] >= 'A') && (au8String[u8LocIndex] <= 'F')){
      u16DecValue |= ((uint16_t)(au8String[u8LocIndex] - 'A' + 0x0A))<<(u8LocIndex2*4);    
    }else{
      u16DecValue |= ((uint16_t)(au8String[u8LocIndex] - 'a' + 0x0A))<<(u8LocIndex2*4);        
    }
    u8LocIndex2++;
  }
  
  return u16DecValue;
}

//bool_t stringComp(uint8_t * au8leftString, uint8_t * au8RightString, uint8_t bytesToCompare)
//{
//  do
//  {
//  }while((*au8leftString++ == *au8RightString++) && --bytesToCompare);
//  return(0 == bytesToCompare);
//}

inline void Clear_CurrentLine()
{
  uint8_t index;
  Serial_Print(mAppSer,"\r",gAllowToBlock_d);
  for(index = 0; index  < gMaxSmacSDULength_c; index++)
    Serial_Print(mAppSer," ",gAllowToBlock_d);
  Serial_Print(mAppSer,"\r",gAllowToBlock_d);
}
/***********************************************************************
************************************************************************/