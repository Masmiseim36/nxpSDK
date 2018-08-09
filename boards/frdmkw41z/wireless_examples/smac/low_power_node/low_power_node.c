/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "low_power_node.h"
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
#define SelfNotificationEvent() ((void)OSA_EventSet(gTaskEvent, gLDPSelf_EVENT_c))
#define OpModeRX                (1)
#define OpModeTX                (2)
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
osaEventId_t            gTaskEvent;
osaEventFlags_t         gTaskEventFlags;

channels_t        testChannel;
uint8_t           testPower;

LPDStates_t       lpdState;
LPDManualStates_t lpdmState;
LPDLppsStates_t   lpdlState;

bool_t evDataFromUART;
bool_t evTestParameters;
uint8_t gu8UartData;

bool_t shortCutsEnabled;

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t gau8RxDataBuffer[gMaxSmacSDULength_c  + sizeof(txPacket_t)];                         
static uint8_t gau8TxDataBuffer[gMaxSmacSDULength_c  + sizeof(rxPacket_t)];                        

static txPacket_t * gAppTxPacket;
static rxPacket_t * gAppRxPacket;

static uint8_t mAppSer;
static tmrTimerID_t AppDelayTmr;
static bool_t timePassed;

static uint8_t u8TxCount;
static bool_t  evKeyPressed;
static bool_t  bTxDone;
static bool_t  bRxDone;
static uint8_t lpd_lpps_config;
static bool_t  bPrevModeIsVlpr;
static bool_t  bWakeUpLLWU;
static int8_t  subMode;
static ptr_set_mcu_mode pPwrp_set_mcu_mode_no_vlls;
static ptr_set_mcu_mode_vlls pPwrp_set_mcu_mode_vlls;
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define gUART_RX_EVENT_c         (1<<0)
#define gMcps_Cnf_EVENT_c        (1<<1)
#define gMcps_Ind_EVENT_c        (1<<2)
#define gLDPSelf_EVENT_c         (1<<3)
#define gTimePassed_EVENT_c      (1<<4)
#define gKeyPressed_EVENT_c      (1<<5)

#define gEventsAll_c	(gUART_RX_EVENT_c | gMcps_Cnf_EVENT_c | gMcps_Ind_EVENT_c | \
gLDPSelf_EVENT_c | gTimePassed_EVENT_c | gKeyPressed_EVENT_c)

#define FlaggedDelay_ms(a)       TMR_StartSingleShotTimer(AppDelayTmr, a, DelayTimeElapsed, NULL)
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

static void UartRxCallBack(void * param);
static void HandleEvents(int32_t evSignals);
static bool_t stringComp(uint8_t * au8leftString, uint8_t * au8RightString, uint8_t bytesToCompare);

static void SerialUIStateMachine(void);
static void DelayTimeElapsed(void*);
static void ShortCutsParser(uint8_t u8UartData);
static void PrintTestParameters(bool_t bEraseLine);
static bool_t LPPSModeScenario(void);
static bool_t ManualLPMScenario(void);
static void KBD_EventHandler(uint8_t ev);
static void SetMCUConfig(uint8_t mode);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
extern void ResetMCU(void);
/*New section added*/
void InitProject(void);
void InitSmac(void);

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
    shortCutsEnabled                  = FALSE; 
    evDataFromUART                    = FALSE; 
    bPrevModeIsVlpr                   = FALSE;
    bWakeUpLLWU                       = FALSE;
    evKeyPressed                      = FALSE;
    subMode                           = -1;
    pPwrp_set_mcu_mode_no_vlls        = NULL;
    pPwrp_set_mcu_mode_vlls           = NULL;
    u8TxCount                         = 0;
    timePassed                        = FALSE;    
    lpdState                          = gLPDInitState_c;
    lpdmState                         = gLPDMStateInit_c;
    lpdlState                         = gLPDLppsInit_c;
    lpd_lpps_config                   = OpModeTX;
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
        break;
    case gMlmeCcaCnf_c:
        break;
    case gMlmeTimeoutInd_c:
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
    if(evSignals & gLDPSelf_EVENT_c)
    {
    }
    if(evSignals & gKeyPressed_EVENT_c)
    {
        evKeyPressed = TRUE;
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
        
        LED_StartSerialFlash(LED1);
        
        KBD_Init(KBD_EventHandler);
        
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
    AppDelayTmr  = TMR_AllocateTimer();             //Allocate a timer for inserting delays whenever needed
    
    gAppTxPacket = (txPacket_t*)gau8TxDataBuffer;   //Map TX packet to buffer
    gAppRxPacket = (rxPacket_t*)gau8RxDataBuffer;   //Map Rx packet to buffer     
    gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
    
    Serial_InitInterface( &mAppSer, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE); //Get handle of uart interface
    Serial_SetBaudRate (mAppSer, gUARTBaudRate115200_c);   //Set 115200 as default baud
    Serial_SetRxCallBack(mAppSer, UartRxCallBack, NULL);   //Set Receive callback for uart 
    //Initialise SMAC
    InitSmac();
    //Tell SMAC who to call when it needs to pass a message to the application thread.
    Smac_RegisterSapHandlers((SMAC_APP_MCPS_SapHandler_t)smacToAppMcpsSap,(SMAC_APP_MLME_SapHandler_t)smacToAppMlmeSap,0);
    
    InitProject();
    
    SMACFillHeader(&(gAppTxPacket->smacHeader), gBroadcastAddress_c);                   //Destination Address is set in InitProject;
    
    (void)MLMEPAOutputAdjust(testPower);
    (void)MLMESetChannelRequest(testChannel);
    
    //prepare TX packet for Listen Mode Test.
    FLib_MemCpy(gAppTxPacket->smacPdu.smacPdu, "Wake Up Message", 15);
    gAppTxPacket->u8DataLength = 15;
    
    //prepare RX packet for Listen Mode Test.
    gAppRxPacket->u8MaxDataLength = gMaxSmacSDULength_c;
    
    //initialize the custom platform power mode manager
    PWRP_Init();
    /*disable fiterfail + rx watermark*/
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_FILTERFAIL_MSK_MASK;
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_RX_WMRK_MSK_MASK;
    /*enable duty cycle for LPPS*/
    XCVR_MISC->LPPS_CTRL |= (XCVR_CTRL_LPPS_CTRL_LPPS_DCOC_DIG_ALLOW(1)  |
                             XCVR_CTRL_LPPS_CTRL_LPPS_RX_DIG_ALLOW(1)    |
                             XCVR_CTRL_LPPS_CTRL_LPPS_SY_LO_BUF_ALLOW(1) |
                             XCVR_CTRL_LPPS_CTRL_LPPS_SY_LO_ALLOW(1)     |
                             XCVR_CTRL_LPPS_CTRL_LPPS_PDET_ALLOW(1)      |
                             XCVR_CTRL_LPPS_CTRL_LPPS_DCOC_ALLOW(1)      |
                             XCVR_CTRL_LPPS_CTRL_LPPS_ADC_ALLOW(1)       |
                             XCVR_CTRL_LPPS_CTRL_LPPS_BBA_ALLOW(1)       |
                             XCVR_CTRL_LPPS_CTRL_LPPS_TZA_ALLOW(1)
                             );
    
    
}

/************************************************************************************
*
* Low Power Demo State Machine
*
************************************************************************************/
void SerialUIStateMachine(void)
{
    if(gLPDSelectOption_c == lpdState && evTestParameters)
    {
        (void)MLMESetChannelRequest(testChannel);                                   
        (void)MLMEPAOutputAdjust(testPower);
        PrintTestParameters(TRUE);
        evTestParameters = FALSE;
    }
    switch(lpdState)
    {
    case gLPDInitState_c:
        shortCutsEnabled = TRUE;
        PrintMenu(cu8MainMenu, mAppSer);
        PrintTestParameters(FALSE);
        lpdState = gLPDSelectOption_c;
        break;
    case gLPDSelectOption_c:
        if(evDataFromUART)
        {
            evDataFromUART = FALSE;
            if(gu8UartData == '1')
            {
                shortCutsEnabled = FALSE;
                lpdState = gLPDManual_c;
                SelfNotificationEvent();
            }
            else if(gu8UartData == '2')
            {
                shortCutsEnabled = FALSE;
                lpdState = gLPDLpps_c ;
                SelfNotificationEvent();
            }
            else if(gu8UartData == '!')
            {
                ResetMCU();
            }
        }
        break;
    case gLPDManual_c:
        if(ManualLPMScenario())
        {
            lpdState = gLPDInitState_c;
            SelfNotificationEvent();
        }
        break;
    case gLPDLpps_c:
        if(LPPSModeScenario())
        {
            lpdState = gLPDInitState_c;
            SelfNotificationEvent();
        }
        break;
    default:
        break;
    }
}

/************************************************************************************
*
* LPPS Test State Machine
*
************************************************************************************/
static bool_t LPPSModeScenario(void)
{
    bool_t bBackFlag = FALSE;
    uint8_t u8Print[2];
    bool_t bBackToSleep = FALSE;
    switch(lpdlState)
    {
    case gLPDLppsInit_c:
        PrintMenu(cu8LPPSMenu, mAppSer);
        Serial_Print(mAppSer, "\r\n -Press [ENTER] to start \r\n -Press [p] to return to previous menu\r\n", gAllowToBlock_d);
        lpdlState = gLPDLppsConfigRole_c;
        break;
    case gLPDLppsConfigRole_c:
        if(evDataFromUART)
        {
            evDataFromUART = FALSE;
            if(gu8UartData == '\r')
            {
                uint32_t rfOscEn = (RSIM->CONTROL & RSIM_CONTROL_RF_OSC_EN_MASK);
                RSIM->CONTROL |= RSIM_CONTROL_RF_OSC_EN(3);
                if(OpModeTX == lpd_lpps_config)
                {
                    u8TxCount = 0;
                    PWRP_UpdateLPTMRCount(2000);
                    
                    (void)MLMERXDisableRequest();
                    (void)MLMETXDisableRequest();
                    
                    PWRP_SetWakeUpSource(LPTMR_WAKE_UP);
                    Enter_Stop();
                    (void)MCPSDataRequest(gAppTxPacket);
                    lpdlState = gLPDLppsTX_c;
                }
                else
                {
                    lpdlState = gLPDLppsRX_c;
                    (void)MLMERXDisableRequest();
                    (void)MLMETXDisableRequest();
                    //enable LPPS
                    XCVR_MISC->LPPS_CTRL |= XCVR_CTRL_LPPS_CTRL_LPPS_ENABLE_MASK;          
                    bRxDone = FALSE;
                    (void)MLMERXEnableRequest(gAppRxPacket, 0);
                    
                    PWRP_SetWakeUpSource(UART_WAKE_UP);
                    Enter_Stop();
 
                    if(PWRP_GetWakeUpReason() == UART_WAKE_UP)
                    {
                        Serial_Print(mAppSer, "\r\n\t - Test stopped. Press [ENTER] to continue\r\n", gAllowToBlock_d);
                        lpdlState = gLPDLppsRXEnd_c;
                    }
                    else
                    {
                        SelfNotificationEvent();
                    }
                    
                }
                RSIM->CONTROL |= rfOscEn;
            }
            else if(gu8UartData == 'p')
            {
                lpdlState = gLPDLppsInit_c;
                bBackFlag = TRUE;
            }
        }
        break;
    case gLPDLppsRX_c:
        if(bRxDone)
        {
            bRxDone = FALSE;
            if(stringComp(gAppRxPacket->smacPdu.smacPdu, (uint8_t*)"Wake Up Message", 14))
            {
                Serial_Print(mAppSer, "\r\n\t Wake up packet received. Press [ENTER] to continue\r\n", gAllowToBlock_d);
                lpdlState = gLPDLppsRXEnd_c;
            }
            else
            {
                bBackToSleep = TRUE;
            }
        }
        else
        {
            /*here because woke up from phy but no packet received*/
            (void)MLMERXDisableRequest();
            bBackToSleep = TRUE;
        }
        if(bBackToSleep)
        {
            bBackToSleep = FALSE;
            (void)MLMERXEnableRequest(gAppRxPacket, 0);
            
            PWRP_SetWakeUpSource(UART_WAKE_UP);
            Enter_Stop();
            if(PWRP_GetWakeUpReason() == UART_WAKE_UP)
            {
                Serial_Print(mAppSer, "\r\n\t - Test stopped. Press [ENTER] to continue\r\n", gAllowToBlock_d);
                lpdlState = gLPDLppsRXEnd_c;
            }
            else
            {
                SelfNotificationEvent();
            }
        }
        break;
    case gLPDLppsRXEnd_c:
        if(evDataFromUART && gu8UartData == '\r')
        {
            evDataFromUART = FALSE;
            
            (void)MLMERXDisableRequest();
            bRxDone = FALSE;
            //disable LPPS
            XCVR_MISC->LPPS_CTRL  &= ~XCVR_CTRL_LPPS_CTRL_LPPS_ENABLE_MASK;
            lpdlState = gLPDLppsInit_c;
            bBackFlag = TRUE;
        }
        break;
    case gLPDLppsTX_c:
        if(bTxDone)
        {
            bTxDone = FALSE;
            ++u8TxCount;
            
            Serial_Print(mAppSer, "\r\n\t - Sent packet number ", gAllowToBlock_d);
            u8Print[0] = u8TxCount + '0';
            u8Print[1] = '\0';
            Serial_Print(mAppSer, (char*)u8Print, gAllowToBlock_d);
            
            if(u8TxCount < 5)
            {
                FlaggedDelay_ms(4);
            }
            else
            {
                Serial_Print(mAppSer, "\r\n Test finished. Press [ENTER] to continue\r\n", gAllowToBlock_d);
                lpdlState = gLPDLppsTXEnd_c;
            }
        }
        if(timePassed)
        {
            timePassed = FALSE;
            
            PWRP_SetWakeUpSource(LPTMR_WAKE_UP);
            Enter_Stop();
            
            (void)MCPSDataRequest(gAppTxPacket);
        }
        break;
    case gLPDLppsTXEnd_c:
        if(evDataFromUART && gu8UartData == '\r')
        {
            evDataFromUART = FALSE;
            lpdlState = gLPDLppsInit_c;
            bBackFlag = TRUE;
        }
        break;
    }
    return bBackFlag;
}

/************************************************************************************
*
* Manual Low Power Test State Machine
*
************************************************************************************/
static bool_t ManualLPMScenario(void)
{
    bool_t bBackFlag = FALSE;
    
    switch(lpdmState)
    {
    case gLPDMStateInit_c:
        PrintMenu(cu8LPCSelectMCUPowerModeMenuString, mAppSer);
        lpdmState = gLPDMStateSelectMcuMode_c;
        break;
    case gLPDMStateSelectMcuMode_c:
        if(evDataFromUART)
        {
            evDataFromUART = FALSE;
            if(gu8UartData == 'p')
            {
                lpdmState = gLPDMStateInit_c;
                bBackFlag = TRUE;
                break;
            }
            if(gu8UartData < '0' || gu8UartData > '9')
            {
                break;
            }
            SetMCUConfig(gu8UartData - '0');
            if(bWakeUpLLWU)
            {
                if(subMode == 0 && pPwrp_set_mcu_mode_vlls != NULL)
                {
                    Serial_Print(mAppSer, "\r\nNote: Wake up from VLLS0 can be done only via GPIO" 
                                 "(SW4) since LPO is disabled\r\n",
                                 gAllowToBlock_d);
                    Serial_Print(mAppSer, "\r\n-Press [1] to select GPIO (Any Push Button) as wake up source\r\n",
                                 gAllowToBlock_d);
                }
                else
                {
                    PrintMenu(cu8WakeUpLLWU, mAppSer);
                }
                lpdmState = gLPDMStateSelectWakeUpSource_c;
            }
            else if (!bPrevModeIsVlpr)
            {
                PrintMenu(cu8WakeUpNonLLWU, mAppSer);
                lpdmState = gLPDMStateSelectWakeUpSource_c;
            }
            else
            {
                Serial_Print(mAppSer, "\r\n\r\n Long press any push button to exit VLPR\r\n\r\n", gAllowToBlock_d);
                SelfNotificationEvent();
                
                lpdmState = gLPDMStateEnterLowPower_c;
            }
        }
        break;
    case gLPDMStateSelectWakeUpSource_c:
        if(evDataFromUART)
        {
            bool_t validKey = TRUE;
            evDataFromUART = FALSE;
            if(gu8UartData == '1' && !bWakeUpLLWU)
            {
                PWRP_SetWakeUpSource(UART_WAKE_UP);
            }
            else if( (gu8UartData == '2' && !bWakeUpLLWU) ||
                    (gu8UartData == '1' && bWakeUpLLWU))
            {
                PWRP_SetWakeUpSource(GPIO_WAKE_UP);
            }
            else if( (gu8UartData == '3' && !bWakeUpLLWU) ||
                    (gu8UartData == '2' && bWakeUpLLWU && subMode !=0))
            {
                PWRP_SetWakeUpSource(LPTMR_WAKE_UP);
                PWRP_UpdateLPTMRCount(gLPTMR_MS_COUNT_c);
            }
            else
            {
                validKey = FALSE;
            }
            
            if(validKey)
            {
                lpdmState = gLPDMStateEnterLowPower_c;
                SelfNotificationEvent();
            }
        }
        break;
    case gLPDMStateEnterLowPower_c:
        if(subMode != -1 && pPwrp_set_mcu_mode_vlls != NULL)
        {
            /*never returns*/
            pPwrp_set_mcu_mode_vlls(subMode);
        }
        else
        {
            pPwrp_set_mcu_mode_no_vlls();
            if(!bPrevModeIsVlpr)
            {
                if(PWRP_GetWakeUpReason() != UNKNOWN_WAKE_UP)
                {
                    Serial_Print(mAppSer, "\r\n\t--Back in run mode--\r\n\r\nPress [ENTER] to continue\r\n", gAllowToBlock_d);
                }
                else
                {
                    Serial_Print(mAppSer, "\r\n\t--Back in run mode (other interrupt)--\r\n\r\n"
                                 "Press [ENTER] to continue\r\n", gAllowToBlock_d);
                }
            }
            lpdmState = gLPDMStateHandleExitLowPower_c;
        }
        
        break;
    case gLPDMStateHandleExitLowPower_c:
        if(evDataFromUART)
        {
            evDataFromUART = FALSE;
            if(gu8UartData == '\r' && !bPrevModeIsVlpr)
            {
                lpdmState = gLPDMStateInit_c;
                SelfNotificationEvent();
            }
        }
        if(bPrevModeIsVlpr && evKeyPressed)
        {
            evKeyPressed = FALSE;
            bPrevModeIsVlpr = FALSE;
            
            Enter_RUN();
            Serial_Print(mAppSer, "\r\n\t--Back in run mode--\r\n", gAllowToBlock_d);
            lpdmState = gLPDMStateInit_c;
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
* Sets the appropriate handler for MCU low power mode
*
************************************************************************************/
static void SetMCUConfig(uint8_t mode)
{
    pPwrp_set_mcu_mode_no_vlls = NULL;
    pPwrp_set_mcu_mode_vlls = NULL;
    subMode = -1;
    bWakeUpLLWU = FALSE;
    bPrevModeIsVlpr = FALSE;
    switch(mode)
    {
    case 0:
        pPwrp_set_mcu_mode_no_vlls = Enter_Wait;
        break;
    case 1:
        pPwrp_set_mcu_mode_no_vlls = Enter_Stop;
        break;
    case 2:
        pPwrp_set_mcu_mode_no_vlls = Enter_VLPR;
        bPrevModeIsVlpr = TRUE;
        break;
    case 3:
        pPwrp_set_mcu_mode_no_vlls = Enter_VLPW;
        break;
    case 4:
        pPwrp_set_mcu_mode_no_vlls = Enter_VLPS;
        break;
    case 5:
        pPwrp_set_mcu_mode_no_vlls = Enter_LLS;
        bWakeUpLLWU = TRUE;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        pPwrp_set_mcu_mode_vlls = Enter_VLLS;
        bWakeUpLLWU = TRUE;
        if(mode == 6)
        {
            subMode = 3;
        }
        else if(mode == 7)
        {
            subMode = 2;
        }
        else if(mode == 8)
        {
            subMode = 1;
        }
        else
        {
            subMode = 0;
        }
        break;
    default:
        pPwrp_set_mcu_mode_no_vlls = Enter_LLS;
        bWakeUpLLWU = TRUE;
        break;
    }
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
        for(u8Index1 = 0; u8Index1 < 80; u8Index1++)
        {
            Serial_Print(mAppSer," ",gAllowToBlock_d);
        }
        Serial_Print(mAppSer,"\r",gAllowToBlock_d);
    }
    
    Serial_Print(mAppSer, " Channel ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer, (uint32_t)testChannel);
    Serial_Print(mAppSer,", Power ", gAllowToBlock_d);
    Serial_PrintDec(mAppSer,(uint32_t)testPower);
    Serial_Print(mAppSer,", LPPS Test: ", gAllowToBlock_d);
    if(lpd_lpps_config == OpModeRX)
    {
        Serial_Print(mAppSer,"Rx", gAllowToBlock_d);
    }
    else
    {
        Serial_Print(mAppSer,"Tx", gAllowToBlock_d);
    }
    Serial_Print(mAppSer," >", gAllowToBlock_d);
}
/*****************************************************************************
* KBD_EventHandler function
*
* Interface assumptions:
* This callback is triggered when a push button is pressed
*
* Return Value:
* None
*****************************************************************************/
static void KBD_EventHandler(uint8_t ev)
{
    (void)ev;
    if(bPrevModeIsVlpr)
    {
        OSA_EventSet(gTaskEvent, gKeyPressed_EVENT_c);
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
    case 'r':
        lpd_lpps_config = OpModeRX;
        break;
    case 't':
        lpd_lpps_config = OpModeTX;
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
static void DelayTimeElapsed(void* arg)
{
    timePassed = TRUE;
    OSA_EventSet(gTaskEvent, gTimePassed_EVENT_c);
}
bool_t stringComp(uint8_t * au8leftString, uint8_t * au8RightString, uint8_t bytesToCompare)
{
    do
    {
    }while((*au8leftString++ == *au8RightString++) && --bytesToCompare);
    return(0 == bytesToCompare);
}
/***********************************************************************
************************************************************************/