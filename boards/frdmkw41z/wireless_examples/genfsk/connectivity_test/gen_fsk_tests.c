/*!
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This is a source file for the main application.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "MemManager.h"
#include "RNG_Interface.h"
#include "TimersManager.h"
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
#include "PWR_Interface.h"
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */

#include "genfsk_interface.h"
#include "gen_fsk_tests_states.h"
#include "gen_fsk_tests.h"
#include "xcvr_test_fsk.h"
#include "menus.h"

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
/*serial interface id*/
uint8_t mAppSerId;
/*timers manager app timer id*/
uint8_t mAppTmrId;
/*GENFSK instance id*/
uint8_t mAppGenfskId;
/*configuration params*/
ct_config_params_t gaConfigParams[5];

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define gPerOpcode1 (0xCA)
#define gPerOpcode2 (0xFE)

#define gRangeOpcode1 (0xAB)
#define gRangeOpcode2 (0xCD)
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
/* PER RX handler */
static bool_t CT_PacketErrorRateRx(ct_event_t evType, void* pAssociatedValue, bool_t bReInit);
/* PER TX handler */
static bool_t CT_PacketErrorRateTx(ct_event_t evType, void* pAssociatedValue, bool_t bReInit);
/* Range RX handler */
static bool_t CT_RangeRx(ct_event_t evType, void* pAssociatedValue, bool_t bReInit);
/* Range RX handler */
static bool_t CT_RangeTx(ct_event_t evType, void* pAssociatedValue, bool_t bReInit);

/* Shortcuts enable/disable depending on test state */
static void CT_EnableShortcuts(bool_t bEnable);
/* Handler that applies latest values for channel and tx power and prints shortcuts values */
static bool_t CT_ApplyPrintConfigParams(void);
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/* buffers for interaction with Generic FSK */
static uint8_t* gRxBuffer;
static uint8_t* gTxBuffer;

/* Generic FSK packets to get formatted data*/
static GENFSK_packet_t gRxPacket;
static GENFSK_packet_t gTxPacket;

/* control if shortcuts are enabled or disabled*/
static bool_t   bEnableShortcuts = FALSE;

/*hook to notify app thread*/
static pHookAppNotification pNotifyAppThread = NULL;
/*hook to notify app thread*/
static pTmrHookNotification pTmrCallback = NULL;

/*packet configuration*/
static GENFSK_packet_config_t pktConfig = 
{
    .preambleSizeBytes = 0, /*1 byte of preamble*/
    .packetType = gGenfskFormattedPacket,
    .lengthSizeBits = gGenFskDefaultLengthFieldSize_c,
    .lengthBitOrder = gGenfskLengthBitLsbFirst,
    /*sync address bytes = size + 1*/
    .syncAddrSizeBytes = gGenFskDefaultSyncAddrSize_c,
    .lengthAdjBytes = 3, /*length field not including CRC so adjust by crc len*/
    .h0SizeBits = gGenFskDefaultH0FieldSize_c,
    .h1SizeBits = gGenFskDefaultH1FieldSize_c,
    .h0Match = gGenFskDefaultH0Value_c, /*match field containing zeros*/
    .h0Mask = gGenFskDefaultH0Mask_c,
    .h1Match = gGenFskDefaultH1Value_c,
    .h1Mask = gGenFskDefaultH1Mask_c
};

/*CRC configuration*/
static GENFSK_crc_config_t crcConfig =
{
    .crcEnable = gGenfskCrcEnable,
    .crcSize = 3,
    .crcStartByte = 4,
    .crcRefIn = gGenfskCrcInputNoRef,
    .crcRefOut = gGenfskCrcOutputNoRef,
    .crcByteOrder = gGenfskCrcLSByteFirst,
    .crcSeed = 0x00555555,
    .crcPoly = 0x0000065B,
    .crcXorOut = 0
};

/*whitener configuration*/
static GENFSK_whitener_config_t whitenConfig = 
{
    .whitenEnable = gGenfskWhitenEnable,
    .whitenStart = gWhitenStartWhiteningAtH0,
    .whitenEnd = gWhitenEndAtEndOfCrc,
    .whitenB4Crc = gCrcB4Whiten,
    .whitenPolyType = gGaloisPolyType,
    .whitenRefIn = gGenfskWhitenInputNoRef,
    .whitenPayloadReinit = gGenfskWhitenNoPayloadReinit,
    .whitenSize = 7,
    .whitenInit = 0x53,
    .whitenPoly = 0x11, /*x^7 + x^4 + 1! x^7 is never set*/
    .whitenSizeThr = 0,
    .manchesterEn = gGenfskManchesterDisable,
    .manchesterStart = gGenfskManchesterStartAtPayload,
    .manchesterInv = gGenfskManchesterNoInv,
};

/*radio configuration*/
static GENFSK_radio_config_t radioConfig = 
{
    .radioMode = gGenfskGfskBt0p5h0p5,
    .dataRate = gGenfskDR1Mbps
};

/*bit processing configuration*/

/*network / sync address configuration*/
static GENFSK_nwk_addr_match_t ntwkAddr = 
{
    .nwkAddrSizeBytes = gGenFskDefaultSyncAddrSize_c,
    .nwkAddrThrBits = 0,
    .nwkAddr = gGenFskDefaultSyncAddress_c,
}; 

/**********************************************************************************/
void CT_GenFskInit(pHookAppNotification pFunc, pTmrHookNotification pTmrFunc)
{
    bEnableShortcuts = TRUE;

    /*configure hook*/
    pNotifyAppThread = pFunc;
    
    /*configure timer callback*/
    pTmrCallback = pTmrFunc;
    
    /* populate shortcut array */
    gaConfigParams[0].paramType = gParamTypeString_c;
    FLib_MemCpy(gaConfigParams[0].paramName, "Mode", 5);
    FLib_MemCpy(gaConfigParams[0].paramValue.stringValue, "RX", 3);
    if(gDefaultMode_c == gModeTx_c)
    {
        gaConfigParams[0].paramValue.stringValue[0] = 'T';
    }
    gaConfigParams[1].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[1].paramName, "Channel", 8);
    gaConfigParams[1].paramValue.decValue = gGenFskDefaultChannel_c;
    
    gaConfigParams[2].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[2].paramName, "Power", 6);
    gaConfigParams[2].paramValue.decValue = gGenFskDefaultTxPowerLevel_c;
    
    gaConfigParams[3].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[3].paramName, "Payload", 8);
    gaConfigParams[3].paramValue.decValue = gGenFskDefaultPayloadLen_c;
    
    gaConfigParams[4].paramType = gParamTypeMaxType_c;
    /* allocate once to use for the entire application */
    gRxBuffer  = MEM_BufferAlloc(gGenFskDefaultMaxBufferSize_c + 
                                 crcConfig.crcSize);
    gTxBuffer  = MEM_BufferAlloc(gGenFskDefaultMaxBufferSize_c);
    
    gRxPacket.payload = (uint8_t*)MEM_BufferAlloc(gGenFskMaxPayloadLen_c  + 
                                                       crcConfig.crcSize);
    gTxPacket.payload = (uint8_t*)MEM_BufferAlloc(gGenFskMaxPayloadLen_c);
    
    /*prepare the part of the tx packet that is common for all tests*/
    gTxPacket.addr = gGenFskDefaultSyncAddress_c;
    gTxPacket.header.h0Field = gGenFskDefaultH0Value_c;
    gTxPacket.header.h1Field = gGenFskDefaultH1Value_c;
    
    /*set bitrate*/
    GENFSK_RadioConfig(mAppGenfskId, &radioConfig);
    /*set packet config*/
    GENFSK_SetPacketConfig(mAppGenfskId, &pktConfig);
    /*set whitener config*/
    GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenConfig);
    /*set crc config*/
    GENFSK_SetCrcConfig(mAppGenfskId, &crcConfig);
    
    /*set network address at location 0 and enable it*/
    GENFSK_SetNetworkAddress(mAppGenfskId, 0, &ntwkAddr);
    GENFSK_EnableNetworkAddress(mAppGenfskId, 0);
    
    /*set tx power level*/
    GENFSK_SetTxPowerLevel(mAppGenfskId, gGenFskDefaultTxPowerLevel_c);
    /*set channel: Freq = 2360MHz + ChannNumber*1MHz*/
    GENFSK_SetChannelNumber(mAppGenfskId, gGenFskDefaultChannel_c);
    
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
    PWR_ChangeDeepSleepMode(7);
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
}

/****************************************************************************
*****************************Packet error rate*******************************
****************************************************************************/
bool_t CT_PacketErrorRate(ct_event_t evType, void* pAssociatedValue)
{
    bool_t bReturnFromSM = FALSE;
    static uint8_t currentTest = 0;
    
    if(gaConfigParams[0].paramValue.stringValue[0] == 'R')
    {
        bReturnFromSM = CT_PacketErrorRateRx(evType, pAssociatedValue, (currentTest != 'R'));
        currentTest = bReturnFromSM? 0 : 'R';
    }
    else
    {
        bReturnFromSM = CT_PacketErrorRateTx(evType, pAssociatedValue, currentTest != 'T');
        currentTest = bReturnFromSM? 0 : 'T';
    }
    return bReturnFromSM;
}

/*! *********************************************************************************
* \brief  Handles the Packet error rate RX test
********************************************************************************** */
static bool_t CT_PacketErrorRateRx(ct_event_t evType, 
                                   void* pAssociatedValue, 
                                   bool_t bReInit)
{
    static ct_per_rx_states_t perRxState = gPerRxStateInit_c; 
    static int32_t  i32RssiSum;
    static uint16_t u16ReceivedPackets;
    static uint16_t u16PacketsIndex;  
    static uint16_t u16TotalPackets;
    
    int8_t   i8AverageRssi = 0;
    ct_rx_indication_t* pIndicationInfo = NULL;
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
    uint32_t u32SleepDuration = 0;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
    
    uint8_t u8UartData = 0xFF;

    uint8_t* pRxBuffer = NULL;
    bool_t bRestartRx = FALSE;
    bool_t bReturnFromSM = FALSE;
     
    (void)CT_ApplyPrintConfigParams();
    
    if(bReInit) /* Reset the state machine */
    {
        perRxState = gPerRxStateInit_c;
    }
    if(gCtEvtUart_c == evType) /* if UART event extract the character */
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }
    switch(perRxState)
    {
    case gPerRxStateInit_c:
        CT_EnableShortcuts(TRUE);
        /*print per rx menu and shortcut keys*/
        PrintMenu(cu8ShortCutsBar, mAppSerId);
        PrintMenu(cu8PerRxTestMenu, mAppSerId);
        PrintTestParams(gaConfigParams, FALSE, mAppSerId);
        
        u16TotalPackets = 0;
        u16ReceivedPackets = 0;
        u16PacketsIndex = 0;
        i32RssiSum = 0;
        perRxState = gPerRxStateWaitStartTest_c;
        break;
    case gPerRxStateWaitStartTest_c:
        if(gCtEvtUart_c == evType)
        {
            if(' ' == u8UartData)
            {
                Serial_Print(mAppSerId, cu8PerRxMessages[0], gAllowToBlock_d);
                
                if(gGenfskSuccess_c != 
                   GENFSK_StartRx(mAppGenfskId,
                                  gRxBuffer, 
                                  gGenFskDefaultMaxBufferSize_c + 
                                      crcConfig.crcSize,
                                      0,
                                      0)
                   )
                {
                    GENFSK_AbortAll();
                    Serial_Print(mAppSerId, cu8PerRxMessages[1], gAllowToBlock_d);
                    perRxState = gPerRxStateIdle_c; 
                }
                else
                {
                    CT_EnableShortcuts(FALSE);  
                    perRxState = gPerRxStateStartTest_c;
                }
            }
            else if('p' == u8UartData)
            { 
                perRxState = gPerRxStateInit_c;
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gPerRxStateStartTest_c:
        if(gCtEvtRxDone_c == evType || 
           gCtEvtRxFailed_c == evType ||
               gCtEvtSeqTimeout_c == evType) /*check if RX related events are fired */
        {
            if (gCtEvtRxDone_c == evType) /*if rx succesfull, get packet information */
            {
                pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
                pRxBuffer = pIndicationInfo->pBuffer; /*same as gRxBuffer*/
                
                /*map rx buffer to generic fsk packet*/
                GENFSK_ByteArrayToPacket(mAppGenfskId, pRxBuffer, &gRxPacket);
                if(gRxPacket.payload[4] == gPerOpcode1 && 
                   gRxPacket.payload[5] == gPerOpcode2) /* check if packet payload is PER type */
                {
                    u16TotalPackets = ((uint16_t)gRxPacket.payload[0] <<8) + gRxPacket.payload[1];
                    u16PacketsIndex = ((uint16_t)gRxPacket.payload[2] <<8) + gRxPacket.payload[3];
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))                    
                    u32SleepDuration = ((uint16_t)gRxPacket.payload[6] <<24) + ((uint16_t)gRxPacket.payload[7] <<16) +
                                        ((uint16_t)gRxPacket.payload[8] <<8) + gRxPacket.payload[9];
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
                    u16ReceivedPackets++;
                    i32RssiSum += (int8_t)(pIndicationInfo->rssi);
                    
                    /* print statistics */
                    PrintPerRxInfo(u16PacketsIndex, u16ReceivedPackets, pIndicationInfo, mAppSerId);
                    
                    if(u16PacketsIndex == u16TotalPackets) /* received last packet */
                    { 
                        Serial_Print(mAppSerId, cu8PerRxMessages[3], gAllowToBlock_d);
                        perRxState = gPerRxStateIdle_c;
                        pNotifyAppThread(); 
                    }
                    else
                    {
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
                        /*RX will be restarted on WakeUp event.*/
                        PWR_GENFSK_EnterDSM(u32SleepDuration - 10); /*wake-up ~10ms earlier*/
                        PWR_AllowDeviceToSleep();
#else
                        bRestartRx = TRUE;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
                    }
                } 
                else
                {
                    bRestartRx = TRUE;
                }
            }
            else
            {
                bRestartRx = TRUE;
            }
            if(bRestartRx) /*restart RX immediately with no timeout*/
            {
                if(gGenfskSuccess_c != 
                   GENFSK_StartRx(mAppGenfskId,
                                  gRxBuffer, 
                                  gGenFskDefaultMaxBufferSize_c + 
                                      crcConfig.crcSize,
                                      0,
                                      0)
                   )
                {
                    GENFSK_AbortAll();
                    Serial_Print(mAppSerId, cu8PerRxMessages[1], gAllowToBlock_d);
                    perRxState = gPerRxStateIdle_c; 
                }
            }  
        }
        if(gCtEvtUart_c == evType)
        {
            if(' ' == u8UartData)
            {
                GENFSK_AbortAll();
                Serial_Print(mAppSerId, cu8PerRxMessages[2], gAllowToBlock_d);
                pNotifyAppThread(); /* send gCtEvtSelfEvent_c event */
                perRxState = gPerRxStateIdle_c;
            } 
        }     
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8PerRxMessages[2], gAllowToBlock_d);
            pNotifyAppThread(); /* send gCtEvtSelfEvent_c event */
            perRxState = gPerRxStateIdle_c;
        }
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))         
        if(gCtEvtWakeUp_c == evType)
        {
            if(gGenfskSuccess_c != 
               GENFSK_StartRx(mAppGenfskId,
                              gRxBuffer, 
                              gGenFskDefaultMaxBufferSize_c + 
                                  crcConfig.crcSize,
                                  0,
                                  0)
                   )
            {
                GENFSK_AbortAll();
                Serial_Print(mAppSerId, cu8PerRxMessages[1], gAllowToBlock_d);
                perRxState = gPerRxStateIdle_c; 
            }
        }
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
        break;
    case gPerRxStateIdle_c:
        if((gCtEvtUart_c == evType) && 
           ('\r' == u8UartData)) /* reinit state machine (for error cases)*/
        {
            perRxState = gPerRxStateInit_c;
            pNotifyAppThread();
        }
        if(gCtEvtSelfEvent_c == evType) /* print statistics for PER test */
        {
                                
            i8AverageRssi = u16ReceivedPackets > 0 ? 
                i32RssiSum/u16ReceivedPackets: 0;
                
            PrintPerRxFinal(u16ReceivedPackets, 
                            u16TotalPackets, 
                            i8AverageRssi, 
                            mAppSerId);
            Serial_Print(mAppSerId, 
                         cu8PerRxMessages[4], 
                         gAllowToBlock_d);
        }
        break;
    default:
        break;
    }
    return bReturnFromSM;      
}

/*! *********************************************************************************
* \brief  Handles the Packet error rate TX test
********************************************************************************** */
static bool_t CT_PacketErrorRateTx(ct_event_t evType, 
                                   void* pAssociatedValue, 
                                   bool_t bReInit)
{
    const uint16_t u16TotalPacketsOptions[] = {1,25,100,500,1000,2000,5000,10000,65535};
    
    static ct_per_tx_states_t perTxState = gPerTxStateInit_c;
    static uint32_t miliSecDelay;

    static uint16_t u16TotalPackets;
    static uint16_t u16SentPackets;
    
    uint16_t buffLen = 0;
    bool_t bReturnFromSM = FALSE;

    uint8_t u8Index;
    uint8_t u8UartData = 0xFF;
    
    (void)CT_ApplyPrintConfigParams();
    
    if(bReInit)
    {
        perTxState = gPerTxStateInit_c;
    }
    
    if(gCtEvtUart_c == evType)
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }
    switch(perTxState)
    {
    case gPerTxStateInit_c:
        PrintMenu(cu8ShortCutsBar, mAppSerId);
        PrintMenu(cu8PerTxTestMenu, mAppSerId);
        PrintTestParams(gaConfigParams, FALSE, mAppSerId);
       
        CT_EnableShortcuts(TRUE);          
        
        perTxState = gPerTxStateSelectPacketNum_c;
        miliSecDelay = 0;
        break;
    case gPerTxStateSelectPacketNum_c:
        if(gCtEvtUart_c == evType) /*select number of packets to send*/
        {    
            if((u8UartData >= '0') && (u8UartData <= '8'))
            {
                u16TotalPackets = u16TotalPacketsOptions[u8UartData - '0'];
                CT_EnableShortcuts(FALSE);  
                Serial_Print(mAppSerId, cu8PerTxMessages[0], gAllowToBlock_d);
                perTxState = gPerTxStateInputPacketDelay_c;
            }
            else if('p' == u8UartData) /*go back to previous menu*/
            { 
                perTxState = gPerTxStateInit_c;
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gPerTxStateInputPacketDelay_c:
        if(gCtEvtUart_c == evType) /*select delay*/
        {
            if(u8UartData == '\r')
            {
                /*approximate limitation due to printing data on the Serial Interface*/
                if(miliSecDelay < 7) 
                {
                    Serial_Print(mAppSerId, cu8PerTxMessages[1], gAllowToBlock_d);
                    perTxState = gPerTxStateInit_c;
                    pNotifyAppThread(); /*notify application thread*/
                }
                else
                {
                    perTxState = gPerTxStateStartTest_c;
                    pNotifyAppThread();
                }
            }
            else if((u8UartData >= '0') && (u8UartData <='9'))
            {
                miliSecDelay = miliSecDelay*10 + (u8UartData - '0');
                Serial_PrintDec(mAppSerId, (uint32_t)(u8UartData - '0'));
            }
            else if('p' == u8UartData)
            { 
                perTxState = gPerTxStateInit_c;
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gPerTxStateStartTest_c:
        miliSecDelay *= 1000; /*convert into microseconds*/
        u16SentPackets = 0;
        gTxPacket.header.lengthField = 
            (uint16_t)gaConfigParams[3].paramValue.decValue;
        
        gTxPacket.payload[0] = (u16TotalPackets >> 8);
        gTxPacket.payload[1] = (uint8_t)u16TotalPackets;
        gTxPacket.payload[2] = ((u16SentPackets+1) >> 8);
        gTxPacket.payload[3] = (uint8_t)(u16SentPackets+1);
        gTxPacket.payload[4] = gPerOpcode1;
        gTxPacket.payload[5] = gPerOpcode2;
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
        miliSecDelay /= 1000; /*for low power use miliseconds resolution*/
        gTxPacket.payload[6] = (miliSecDelay >> 24);
        gTxPacket.payload[7] = (miliSecDelay >> 16);
        gTxPacket.payload[8] = (miliSecDelay >> 8);
        gTxPacket.payload[9] = (uint8_t)miliSecDelay;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
        
        if(gGenFskMinPayloadLen_c < gaConfigParams[3].paramValue.decValue)
        {
            for(u8Index = gGenFskMinPayloadLen_c ;
                u8Index < gaConfigParams[3].paramValue.decValue;
                u8Index++)
            {     
                gTxPacket.payload[u8Index] = (u8Index%10)+'0';  /*add some padding*/          
            }
        }
        /*pack everything into a buffer*/
        GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
        /*calculate buffer length*/
        buffLen = gTxPacket.header.lengthField+
                    (gGenFskDefaultHeaderSizeBytes_c)+
                        (gGenFskDefaultSyncAddrSize_c + 1);
                
        if(gGenfskSuccess_c != /*start tx at current time + input delay*/
           GENFSK_StartTx(mAppGenfskId, gTxBuffer, buffLen, 0))
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8PerTxMessages[2], gAllowToBlock_d);
            perTxState = gPerTxStateIdle_c;
        }
        u16SentPackets++;
        Serial_Print(mAppSerId, cu8PerTxMessages[3], gAllowToBlock_d);
        Serial_PrintDec(mAppSerId, (uint32_t)u16TotalPackets);
        
        perTxState = gPerTxStateRunningTest_c;
        break;
    case gPerTxStateRunningTest_c:
        if(gCtEvtTxDone_c == evType)
        {    
            PrintPerTxPackets(u16SentPackets, 
                              (gGenfskSuccess == *((genfskEventStatus_t*)pAssociatedValue)),
                              mAppSerId);
            
            if(u16SentPackets == u16TotalPackets)
            {    
                Serial_Print(mAppSerId, cu8PerTxMessages[4], gAllowToBlock_d);
                perTxState = gPerTxStateIdle_c;
            }
            else
            {
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
                /*next packet is sent on wakeup event*/
                PWR_GENFSK_EnterDSM(miliSecDelay);
                PWR_AllowDeviceToSleep();
#else
                gTxPacket.payload[2] = ((u16SentPackets+1) >> 8);
                gTxPacket.payload[3] = (uint8_t)(u16SentPackets+1);
                /*pack everything into a buffer*/
                GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
                /*calculate buffer length*/
                buffLen = gTxPacket.header.lengthField+
                    (gGenFskDefaultHeaderSizeBytes_c)+
                        (gGenFskDefaultSyncAddrSize_c + 1);
                
                if(gGenfskSuccess_c != 
                   GENFSK_StartTx(mAppGenfskId, gTxBuffer, buffLen, GENFSK_GetTimestamp() + miliSecDelay))
                {
                    GENFSK_AbortAll();
                    Serial_Print(mAppSerId, cu8PerTxMessages[5], gAllowToBlock_d);
                    perTxState = gPerTxStateIdle_c;
                }
                
                u16SentPackets++;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
            }
        }
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
        if(gCtEvtWakeUp_c == evType)
        {
            gTxPacket.payload[2] = ((u16SentPackets+1) >> 8);
            gTxPacket.payload[3] = (uint8_t)(u16SentPackets+1);
            /*pack everything into a buffer*/
            GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
            /*calculate buffer length*/
            buffLen = gTxPacket.header.lengthField+
                (gGenFskDefaultHeaderSizeBytes_c)+
                    (gGenFskDefaultSyncAddrSize_c + 1);
            
            if(gGenfskSuccess_c != 
               GENFSK_StartTx(mAppGenfskId, gTxBuffer, buffLen, 0))
            {
                GENFSK_AbortAll();
                Serial_Print(mAppSerId, cu8PerTxMessages[5], gAllowToBlock_d);
                perTxState = gPerTxStateIdle_c;
            }
            
            u16SentPackets++;
        }
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
        if(gCtEvtUart_c == evType && u8UartData == ' ')
        {
            GENFSK_AbortAll();
            
            Serial_Print(mAppSerId, cu8PerTxMessages[6],gAllowToBlock_d);
            perTxState = gPerTxStateIdle_c;
        }
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            
            Serial_Print(mAppSerId, cu8PerTxMessages[6],gAllowToBlock_d);
            perTxState = gPerTxStateIdle_c;            
        }
        break;	
    case gPerTxStateIdle_c:
        if((gCtEvtUart_c == evType) && ('\r' == u8UartData))
        {
            perTxState = gPerTxStateInit_c;
            pNotifyAppThread();
        }
        break;
    default:
        break;
    }
    
    return bReturnFromSM;      
}

/****************************************************************************
*********************************Range test**********************************
****************************************************************************/
bool_t CT_RangeTest(ct_event_t evType, void* pAssociatedValue)
{
        
    static uint8_t currentTest = 0;
    bool_t bReturnFromSM = FALSE;
    
    if(gaConfigParams[0].paramValue.stringValue[0] == 'R')
    {
        bReturnFromSM = CT_RangeRx(evType, pAssociatedValue, (currentTest != 'R'));
        currentTest = bReturnFromSM? 0 : 'R';
    }
    else
    {
        bReturnFromSM = CT_RangeTx(evType, pAssociatedValue, currentTest != 'T');
        currentTest = bReturnFromSM? 0 : 'T';
    }
    return bReturnFromSM;   
}

/*! *********************************************************************************
* \brief  Handles the Range RX test
********************************************************************************** */
static bool_t CT_RangeRx(ct_event_t evType, 
                         void* pAssociatedValue, 
                         bool_t bReInit)
{
    static ct_range_rx_states_t rangeRxState = gRangeRxStateInit_c;
    static int32_t i32RssiSum = 0;
    static uint16_t u16ReceivedPackets = 0;
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
    static uint32_t miliSecDelay;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */

    ct_rx_indication_t* pIndicationInfo = NULL;
    uint8_t u8UartData = 0xFF;
    uint16_t buffLen = 0;
    int8_t i8AverageRssi = 0;
    int8_t i8TempValue = 0;
    bool_t bReturnFromSM = FALSE;
    bool_t bRestartRx = FALSE;
    
    if(bReInit)
    {
        rangeRxState = gRangeRxStateInit_c;
    }
    
    if(gCtEvtUart_c == evType)
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }
    
    (void)CT_ApplyPrintConfigParams();
    
    switch(rangeRxState)
    {
    case gRangeRxStateInit_c:
        i32RssiSum = 0;
        u16ReceivedPackets = 0;
        
        PrintMenu(cu8ShortCutsBar, mAppSerId);
        PrintMenu(cu8RangeRxTestMenu, mAppSerId);
        PrintTestParams(gaConfigParams, FALSE,  mAppSerId);
        
        CT_EnableShortcuts(TRUE);           
        
        gTxPacket.payload[1] = gRangeOpcode1;
        gTxPacket.payload[2] = gRangeOpcode2;
        gTxPacket.payload[3] = gRangeOpcode1 ^ gRangeOpcode2;
        gTxPacket.payload[4] = 255 - gTxPacket.payload[3];
        gTxPacket.payload[5] = 0x00;
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
        miliSecDelay = 100; /*100 ms between packets*/
        gTxPacket.payload[6] = (miliSecDelay >> 24);
        gTxPacket.payload[7] = (miliSecDelay >> 16);
        gTxPacket.payload[8] = (miliSecDelay >> 8);
        gTxPacket.payload[9] = (uint8_t)miliSecDelay;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
        
        rangeRxState = gRangeRxStateStartTest_c;
        break;
    case gRangeRxStateStartTest_c:
        if(gCtEvtUart_c == evType)
        {
            if(' ' == u8UartData)
            {
                CT_EnableShortcuts(FALSE); 
                
                if(gGenfskSuccess_c == 
                   GENFSK_StartRx(mAppGenfskId,
                                  gRxBuffer, 
                                  gGenFskDefaultMaxBufferSize_c + 
                                      crcConfig.crcSize, 
                                  0,
                                  0))
                {
                    Serial_Print(mAppSerId, cu8RangeRxMessages[0], gAllowToBlock_d);
                    rangeRxState = gRangeRxStateRunningTest_c;
                }
                else
                {
                    Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                    rangeRxState = gRangeRxStateIdle_c;
                }
            }
            else if('p' == u8UartData)
            { 
                rangeRxState = gRangeRxStateInit_c;
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gRangeRxStateRunningTest_c:
        if(gCtEvtUart_c == evType && 
           (' ' == u8UartData)) /*stop test in case [SPACE] is pressed*/
        {             
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8RangeRxMessages[2], gAllowToBlock_d);
            
            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            
            PrintAvgRssi(i8AverageRssi, mAppSerId); /*Print average RSSI*/
            
            Serial_Print(mAppSerId, cu8RangeRxMessages[3], gAllowToBlock_d);
            rangeRxState = gRangeRxStateIdle_c;
        }
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8RangeRxMessages[2], gAllowToBlock_d);
            
            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            
            PrintAvgRssi(i8AverageRssi, mAppSerId); /*Print average RSSI*/
            
            Serial_Print(mAppSerId, cu8RangeRxMessages[3], gAllowToBlock_d);
            rangeRxState = gRangeRxStateIdle_c;            
        }
        if(gCtEvtRxDone_c == evType || 
           gCtEvtSeqTimeout_c == evType || 
               gCtEvtRxFailed_c == evType)
        {
            if(gCtEvtRxDone_c == evType) /*if RX is succesfull*/
            {                   
                pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
                GENFSK_ByteArrayToPacket(mAppGenfskId, pIndicationInfo->pBuffer, &gRxPacket);
                /*check if Range Test payload type*/
                if(gRxPacket.payload[1] == gRangeOpcode1 &&
                   gRxPacket.payload[2] == gRangeOpcode2 &&
                       gRxPacket.header.lengthField == gGenFskMinPayloadLen_c) 
                {
                    gTxPacket.header.lengthField = gGenFskMinPayloadLen_c;
                    /*extract RSSI for the received packet and send it back*/
                    gTxPacket.payload[0] = pIndicationInfo->rssi;
                    
                    /*prepare TX packet and calculate buffer length for the resulting buffer*/
                    GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
                    buffLen = gTxPacket.header.lengthField+
                        (gGenFskDefaultHeaderSizeBytes_c)+
                            (gGenFskDefaultSyncAddrSize_c + 1);
                    if(gGenfskSuccess_c != 
                       GENFSK_StartTx(mAppGenfskId,
                                      gTxBuffer, 
                                      buffLen, 
                                      GENFSK_GetTimestamp() + 1000))    
                    {
                        Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                        rangeRxState = gRangeRxStateIdle_c;
                    }
                    else
                    {
                        rangeRxState = gRangeRxStatePrintTestResults_c;
                    }
                }
                else
                {
                    bRestartRx = TRUE;
                }
            }
            else
            {
                bRestartRx = TRUE;
            }
            if(bRestartRx)
            {
                if(gGenfskSuccess_c != 
                   GENFSK_StartRx(mAppGenfskId,
                                  gRxBuffer, 
                                  gGenFskDefaultMaxBufferSize_c + 
                                      crcConfig.crcSize, 
                                  0,
                                  0))
                {
                    Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                    rangeRxState = gRangeRxStateIdle_c;
                } 
            }
        }
        break;
    case gRangeRxStatePrintTestResults_c:
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8RangeRxMessages[2], gAllowToBlock_d);
            
            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            
            PrintAvgRssi(i8AverageRssi, mAppSerId); /*Print average RSSI*/
            
            Serial_Print(mAppSerId, cu8RangeRxMessages[3], gAllowToBlock_d);
            rangeRxState = gRangeRxStateIdle_c;            
        }
        if(gCtEvtTxDone_c == evType)
        {     
            i8TempValue = (int8_t)(gTxPacket.payload[0]);
            i32RssiSum += i8TempValue;
            u16ReceivedPackets++;
            /*after sending the RSSI to TX device succesfully, print it*/
            PrintCrtRssi(i8TempValue, mAppSerId);

#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
            /*next packet is sent on wakeup event*/
            PWR_GENFSK_EnterDSM(miliSecDelay - 10);
            PWR_AllowDeviceToSleep();
#else
            if(gGenfskSuccess_c != 
               GENFSK_StartRx(mAppGenfskId,
                              gRxBuffer, 
                              gGenFskDefaultMaxBufferSize_c + 
                                  crcConfig.crcSize, 
                                  0,
                                  0)) /*go back in reception*/
            {
                Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                rangeRxState = gRangeRxStateIdle_c;
            }
            else
            {
                rangeRxState = gRangeRxStateRunningTest_c;
            }
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
        }
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
        if(gCtEvtWakeUp_c == evType)
        {
            if(gGenfskSuccess_c != 
               GENFSK_StartRx(mAppGenfskId,
                              gRxBuffer, 
                              gGenFskDefaultMaxBufferSize_c + 
                                  crcConfig.crcSize, 
                                  0,
                                  0)) /*go back in reception*/
            {
                Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                rangeRxState = gRangeRxStateIdle_c;
            }
            else
            {
                rangeRxState = gRangeRxStateRunningTest_c;
            }
        }
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
        break;
    case gRangeRxStateIdle_c:
        if((gCtEvtUart_c == evType) && 
           ('\r' == u8UartData))
        {
            rangeRxState = gRangeRxStateInit_c;
            pNotifyAppThread();
        }
        break;
    default:
        break;
    }
    
    return bReturnFromSM;      
}

/*! *********************************************************************************
* \brief  Handles the Range TX test
********************************************************************************** */
static bool_t CT_RangeTx(ct_event_t evType, 
                         void* pAssociatedValue, 
                         bool_t bReInit)
{
    static ct_range_tx_states_t rangeTxState = gRangeTxStateInit_c;
    static int32_t i32RssiSum = 0;
    static uint16_t u16ReceivedPackets = 0;
    static uint16_t u16PacketsDropped = 0;
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
    static uint32_t miliSecDelay;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
    
    ct_rx_indication_t* pIndicationInfo;
    int8_t  i8AverageRssi;
    int8_t  i8TempValue;
    uint8_t u8UartData = 0xFF;
    uint16_t buffLen = 0;
    bool_t bRestartRx = FALSE;
    bool_t bReturnFromSM = FALSE;
    
    if(bReInit)
    {
        rangeTxState = gRangeTxStateInit_c;
    }
    
    if(gCtEvtUart_c == evType)
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }
    
    (void)CT_ApplyPrintConfigParams();
    
    switch(rangeTxState)
    {
    case gRangeTxStateInit_c:
        i32RssiSum = 0;
        u16ReceivedPackets = 0;
        u16PacketsDropped = 0;
        
        PrintMenu(cu8ShortCutsBar, mAppSerId);
        PrintMenu(cu8RangeTxTestMenu, mAppSerId);
        PrintTestParams(gaConfigParams, FALSE, mAppSerId);
        
        gTxPacket.payload[1] = gRangeOpcode1;
        gTxPacket.payload[2] = gRangeOpcode2;
        gTxPacket.payload[3] = gRangeOpcode1 ^ gRangeOpcode2;
        gTxPacket.payload[4] = 255 - gTxPacket.payload[3];
        gTxPacket.payload[5] = 0x00;
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
        miliSecDelay = 100; /*100 ms between packets*/
        gTxPacket.payload[6] = (miliSecDelay >> 24);
        gTxPacket.payload[7] = (miliSecDelay >> 16);
        gTxPacket.payload[8] = (miliSecDelay >> 8);
        gTxPacket.payload[9] = (uint8_t)miliSecDelay;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
        
        CT_EnableShortcuts(TRUE);          
        rangeTxState = gRangeTxStateStartTest_c;
        break;
    case gRangeTxStateStartTest_c:
        if(gCtEvtUart_c == evType)
        {
            if(' ' == u8UartData)
            {
                CT_EnableShortcuts(FALSE); 
                /*set first byte of payload*/
                gTxPacket.payload[0] = 0x00;
                /*set payload length*/
                gTxPacket.header.lengthField = gGenFskMinPayloadLen_c;
                /*convert to a ready-to-send buffer*/
                GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
                /*calculate gTxBuffer length*/
                buffLen = gTxPacket.header.lengthField + 
                         (gGenFskDefaultHeaderSizeBytes_c) + 
                         (gGenFskDefaultSyncAddrSize_c + 1);

                if(gGenfskSuccess_c == GENFSK_StartTx(mAppGenfskId,
                                                      gTxBuffer, 
                                                      buffLen,
                                                      0))
                {
                    Serial_Print(mAppSerId, cu8RangeTxMessages[0], gAllowToBlock_d);
                    rangeTxState = gRangeTxStateRunningTest_c;
                }
                else
                {
                    Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                    rangeTxState = gRangeTxStateIdle_c;
                }
            }
            else if('p' == u8UartData)
            { 
                rangeTxState = gRangeTxStateInit_c;
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gRangeTxStateRunningTest_c:
        if(gCtEvtUart_c && 
           (' ' == u8UartData))
        {
            GENFSK_AbortAll();
            
            Serial_Print(mAppSerId, cu8RangeTxMessages[3], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ? 
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            /*print average RSSI for the Range test*/
            PrintAvgRssi(i8AverageRssi, mAppSerId);
            /*print number of dropped packets*/
            PrintRangeTxDroppedPackets(u16PacketsDropped, mAppSerId);
            
            Serial_Print(mAppSerId, cu8RangeTxMessages[4], gAllowToBlock_d);
            
            rangeTxState = gRangeTxStateIdle_c;
        }
        
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            
            Serial_Print(mAppSerId, cu8RangeTxMessages[3], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ? 
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            /*print average RSSI for the Range test*/
            PrintAvgRssi(i8AverageRssi, mAppSerId);
            /*print number of dropped packets*/
            PrintRangeTxDroppedPackets(u16PacketsDropped, mAppSerId);
            
            Serial_Print(mAppSerId, cu8RangeTxMessages[4], gAllowToBlock_d);
            
            rangeTxState = gRangeTxStateIdle_c;            
        }
        
        if(gCtEvtTxDone_c == evType)
        {                                         
            if(gGenfskSuccess_c != 
               GENFSK_StartRx(mAppGenfskId,
                              gRxBuffer, 
                              gGenFskDefaultMaxBufferSize_c, 
                              0, 
                              10000)) /*wait 10 ms for the response*/
            {
                Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                rangeTxState = gRangeTxStateIdle_c;  
            }
            else
            {
                rangeTxState = gRangeTxStatePrintTestResults_c;
            }
        }
        break;
    case gRangeTxStatePrintTestResults_c:
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            
            Serial_Print(mAppSerId, cu8RangeTxMessages[3], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ? 
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            /*print average RSSI for the Range test*/
            PrintAvgRssi(i8AverageRssi, mAppSerId);
            /*print number of dropped packets*/
            PrintRangeTxDroppedPackets(u16PacketsDropped, mAppSerId);
            
            Serial_Print(mAppSerId, cu8RangeTxMessages[4], gAllowToBlock_d);
            
            rangeTxState = gRangeTxStateIdle_c;            
        }
        if(gCtEvtRxDone_c == evType ||
           gCtEvtRxFailed_c == evType ||
               gCtEvtSeqTimeout_c == evType)
        {                                                       
            if(gCtEvtRxDone_c == evType)
            { 
                pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
                GENFSK_ByteArrayToPacket(mAppGenfskId, pIndicationInfo->pBuffer, &gRxPacket);
                
                if(gRxPacket.payload[1] == gRangeOpcode1 &&
                   gRxPacket.payload[2] == gRangeOpcode2 &&
                       gRxPacket.header.lengthField == gGenFskMinPayloadLen_c)
                {
                    i8TempValue = (int8_t)(gRxPacket.payload[0]); 
                    i32RssiSum += i8TempValue;  
                    u16ReceivedPackets++;
                    
                    PrintCrtRssi(i8TempValue, mAppSerId);
                }
                else
                {                                   
                    bRestartRx = TRUE;                        
                }
            }
            else if(gCtEvtSeqTimeout_c == evType)
            {
                u16PacketsDropped++;
                Serial_Print(mAppSerId, cu8RangeTxMessages[2], gAllowToBlock_d);
            }
            else
            {
                bRestartRx = TRUE;
            }
            if(bRestartRx)
            {
                if(gGenfskSuccess_c != 
                   GENFSK_StartRx(mAppGenfskId, 
                                  gRxBuffer, 
                                  gGenFskDefaultMaxBufferSize_c, 
                                  0, 
                                  10000))
                {
                    Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                    rangeTxState = gRangeTxStateIdle_c;  
                }
            }
            else
            {
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
                /*next packet is sent on wakeup event*/
                PWR_GENFSK_EnterDSM(miliSecDelay);
                PWR_AllowDeviceToSleep();
#else
                gTxPacket.payload[0] = 0x00;
                gTxPacket.header.lengthField = gGenFskMinPayloadLen_c;
                GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
                
                buffLen = gTxPacket.header.lengthField + 
                    (gGenFskDefaultHeaderSizeBytes_c) + 
                        (gGenFskDefaultSyncAddrSize_c + 1);
                
                if(gGenfskSuccess_c != GENFSK_StartTx(mAppGenfskId,
                                                      gTxBuffer, 
                                                      buffLen, 
                                                      GENFSK_GetTimestamp() + 100000))
                {
                    Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                    rangeTxState = gRangeTxStateIdle_c;
                }
                else
                {
                    rangeTxState = gRangeTxStateRunningTest_c;
                }
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
            }
        }  
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
        if(gCtEvtWakeUp_c == evType)
        {
            gTxPacket.payload[0] = 0x00;
            gTxPacket.header.lengthField = gGenFskMinPayloadLen_c;
            GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
            
            buffLen = gTxPacket.header.lengthField + 
                (gGenFskDefaultHeaderSizeBytes_c) + 
                    (gGenFskDefaultSyncAddrSize_c + 1);
            
            if(gGenfskSuccess_c != GENFSK_StartTx(mAppGenfskId,
                                                  gTxBuffer, 
                                                  buffLen, 
                                                  0))
            {
                Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                rangeTxState = gRangeTxStateIdle_c;
            }
            else
            {
                rangeTxState = gRangeTxStateRunningTest_c;
            }
        }
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
        break;
    case gRangeTxStateIdle_c:
        if((gCtEvtUart_c == evType) && 
           ('\r' == u8UartData))
        {
            rangeTxState = gRangeTxStateInit_c;
            pNotifyAppThread();
        }
        break;
    default:
        break;
    }
    return bReturnFromSM;      
}


/****************************************************************************
*****************************Continuous Tests********************************
****************************************************************************/

/*! ************************************************************************
* \brief  Handles continuous tests
*************************************************************************** */
bool_t CT_ContinuousTests(ct_event_t evType, void* pAssociatedValue)
{
    static ct_cont_tests_states_t contTestState = gContStateInit_c;
    static ct_cont_tests_states_t prevState = gContStateInvalid_c;
    
    ct_rx_indication_t* pIndicationInfo = NULL;
    uint32_t u32RandomValue = 0;
    uint16_t buffLen = 0;
    uint16_t u16Idx = 0;
    uint8_t u8ByteCount = 0;
    uint8_t u8UartData = 0xFF;
    
    bool_t bRestartRx = FALSE;
    bool_t bReturnFromSM = FALSE;
    
    if(gCtEvtUart_c == evType)
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }
    /*if continuous tx test go through idle so that channel and tx power can
      be changed during test*/
    if((gContStateRunModTxOne_c <= prevState &&
       gContStateRunUnmodTx_c >= prevState) || 
       gContStateRunIdle_c == prevState)
    {
        XcvrFskIdle();
        if(CT_ApplyPrintConfigParams())
        {
            contTestState = prevState;
        }
    }
    else
    {
        (void)CT_ApplyPrintConfigParams();
    }

    
    switch(contTestState)
    {
    case gContStateInit_c:
        XcvrFskIdle();
        PrintMenu(cu8ShortCutsBar, mAppSerId);
        PrintMenu(cu8ContinuousTestMenu, mAppSerId); 
        PrintTestParams(gaConfigParams, FALSE, mAppSerId);
        CT_EnableShortcuts(TRUE);
        contTestState = gContStateSelectTest_c;
        break;
    case gContStateSelectTest_c:
        if(gCtEvtUart_c == evType)
        {
            if('1' <= u8UartData &&
               '8' >= u8UartData)
            {
                contTestState = (ct_cont_tests_states_t)((u8UartData - '1') + 
                                                         gContStateRunIdle_c);
                prevState = contTestState;
                pNotifyAppThread();
            }
            else if('p' == u8UartData)
            {
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
                prevState     = gContStateInvalid_c;
                contTestState = gContStateInit_c;
            }
        }
        break;
    case gContStateRunIdle_c:
        XcvrFskIdle();
        Serial_Print(mAppSerId, cu8ContMessages[7], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;
    case gContStateRunPRBS_c:
        if(gCtEvtTxDone_c == evType ||   /* if packet is sent */
           gCtEvtSelfEvent_c == evType) /* if first run */
        {
            if(gCtEvtSelfEvent_c == evType)
            {
                CT_EnableShortcuts(FALSE);
                Serial_Print(mAppSerId, cu8ContMessages[0], gAllowToBlock_d);
            }
            /*set payload length with user set value*/
            gTxPacket.header.lengthField = gaConfigParams[3].paramValue.decValue;
            /*calculate buffer length*/
            buffLen = gTxPacket.header.lengthField+
                (gGenFskDefaultHeaderSizeBytes_c)+
                    (gGenFskDefaultSyncAddrSize_c + 1);
            /*fill the payload*/
            for(u16Idx = 0; u16Idx < gTxPacket.header.lengthField; u16Idx++)
            {
               if(u8ByteCount == 0)
               {
                   RNG_GetRandomNo(&u32RandomValue); /*get 4 bytes from RNG*/
                   u8ByteCount = 4;
               }
               --u8ByteCount;
               gTxPacket.payload[u16Idx] = 
                   (uint8_t)(u32RandomValue >> (u8ByteCount << 3));
            }
            /*convert the packet to generic fsk formatted byte array*/
            GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
            if(gGenfskSuccess_c != GENFSK_StartTx(mAppGenfskId, gTxBuffer, buffLen, 0))
            {
                Serial_Print(mAppSerId, cu8ContMessages[1], gAllowToBlock_d);
                contTestState = gContStateIdle_c;
            }
        }
        if(gCtEvtUart_c == evType && 
           ' ' == u8UartData)
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8ContMessages[2], gAllowToBlock_d);
            contTestState = gContStateIdle_c; 
        }
        break;
    case gContStateRunModTxOne_c:
        XcvrFskLoadPattern(0xFFFFFFFF);
        XcvrFskModTx();
        Serial_Print(mAppSerId, cu8ContMessages[8], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;
    case gContStateRunModTxZero_c:
        XcvrFskLoadPattern(0x00000000);
        XcvrFskModTx();
        Serial_Print(mAppSerId, cu8ContMessages[9], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;
    case gContStateRunModTxPN_c:
        XcvrFskTxRand();
        Serial_Print(mAppSerId, cu8ContMessages[10], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;
    case gContStateRunUnmodTx_c:
        XcvrFskNoModTx();
        Serial_Print(mAppSerId, cu8ContMessages[11], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;
    case gContStateRunRx_c:
        if(gCtEvtUart_c == evType &&
           (' ' == u8UartData))
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8ContMessages[5], gAllowToBlock_d);
            contTestState = gContStateIdle_c;  
        }
        if(gCtEvtRxDone_c == evType)
        {
            pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
            Serial_Print(mAppSerId, cu8ContMessages[6], gAllowToBlock_d);
            Serial_PrintHex(mAppSerId, 
                            pIndicationInfo->pBuffer, 
                            pIndicationInfo->bufferLength, 
                            gPrtHexBigEndian_c);
            bRestartRx = TRUE;
            
        }
        else if(gCtEvtRxFailed_c == evType ||
                gCtEvtSeqTimeout_c == evType)
        {
            GENFSK_AbortAll();
            bRestartRx = TRUE;
        }
        if(gCtEvtSelfEvent_c == evType)
        {
            CT_EnableShortcuts(FALSE);
            Serial_Print(mAppSerId, cu8ContMessages[3], gAllowToBlock_d);
            bRestartRx = TRUE;
        }
        if(bRestartRx)
        {
            if(gGenfskSuccess_c != 
               GENFSK_StartRx(mAppGenfskId,
                              gRxBuffer, 
                              gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize, 
                              0, 
                              0))
            {
                Serial_Print(mAppSerId, cu8ContMessages[4], gAllowToBlock_d);
                contTestState = gContStateIdle_c;
            }
        }
        break;
    case gContStateRunEd_c:
        if(gCtEvtSelfEvent_c == evType)
        {
            CT_EnableShortcuts(FALSE);
            TMR_StartIntervalTimer(mAppTmrId, 50, pTmrCallback, NULL);
        }
        if(gCtEvtUart_c == evType &&
           (' ' == u8UartData))
        {
            XcvrFskIdle();
            TMR_StopTimer(mAppTmrId);
            Serial_Print(mAppSerId, cu8ContMessages[12], gAllowToBlock_d);
            contTestState = gContStateIdle_c;  
        }
        if(gCtEvtTimerExpired_c == evType)
        {
            PrintCrtRssi((int8_t)XcvrFskGetInstantRssi(), mAppSerId);
        }
        break;
    case gContStateIdle_c:
        if(gCtEvtUart_c == evType &&
           ('\r' == u8UartData))
        {
            contTestState = gContStateInit_c;
            pNotifyAppThread();
        }
    default:
        break;
    }
    
    return bReturnFromSM;    
}

/*! *********************************************************************************
* \brief  Updates the corresponding parameter depending on key pressed
********************************************************************************** */
bool_t CT_UpdateShortcutKeyParam(uint8_t u8PressedKey)
{
    ct_config_params_t* pConfig;
    bool_t bReturnFlag = TRUE;
    switch(u8PressedKey)
    {
    case 't':
        pConfig = &gaConfigParams[0];
        pConfig->paramValue.stringValue[0] = 'T';
        break;
    case 'r':
        pConfig = &gaConfigParams[0];
        pConfig->paramValue.stringValue[0] = 'R';
        break;
    case 'q': 
        pConfig = &gaConfigParams[1];
        if(pConfig->paramValue.decValue == gGenFskMaxChannel_c)
        {
            pConfig->paramValue.decValue = gGenFskMinChannel_c;
        }
        else
        {
            pConfig->paramValue.decValue++;
        }
        break;
    case 'w':
        pConfig = &gaConfigParams[1];
        if(pConfig->paramValue.decValue == gGenFskMinChannel_c)
        {
            pConfig->paramValue.decValue = gGenFskMaxChannel_c;
        }
        else
        {
            pConfig->paramValue.decValue--;
        }
        break;
    case 'a':
        pConfig = &gaConfigParams[2];
        pConfig->paramValue.decValue++;
        if(gGenFskMaxTxPowerLevel_c < pConfig->paramValue.decValue)
        {
            pConfig->paramValue.decValue = gGenFskMinTxPowerLevel_c;
        }
        break;
    case 's':
        pConfig = &gaConfigParams[2];
        if( pConfig->paramValue.decValue == gGenFskMinTxPowerLevel_c)
        {
             pConfig->paramValue.decValue = gGenFskMaxTxPowerLevel_c;
        }
        else
        {
             pConfig->paramValue.decValue--;	
        }
        break;
    case 'n':
        pConfig = &gaConfigParams[3];
        pConfig->paramValue.decValue++;
        if(gGenFskMaxPayloadLen_c < pConfig->paramValue.decValue)
        {
            pConfig->paramValue.decValue = gGenFskMinPayloadLen_c;
        }    
        break;
    case 'm':
        pConfig = &gaConfigParams[3];
        pConfig->paramValue.decValue--;
        if(gGenFskMinPayloadLen_c > pConfig->paramValue.decValue)
        {
            pConfig->paramValue.decValue = gGenFskMaxPayloadLen_c;
        }    
        break;
    default:
        bReturnFlag = FALSE;
        break;
    };
    return bReturnFlag;
}

/*! *********************************************************************************
* \brief  Checks if shortcuts can be used
********************************************************************************** */
bool_t CT_IsShortcutMenuEnabled(void)
{
    return bEnableShortcuts;
}

/*! *********************************************************************************
* \brief  Enables / Disables the shortcut keys
********************************************************************************** */
static void CT_EnableShortcuts(bool_t bEnable)
{
    bEnableShortcuts = bEnable;
}

/*! *********************************************************************************
* \brief  Calls Generic FSK api to set channel and tx power level and prints
*         the shortcuts values
********************************************************************************** */
static bool_t CT_ApplyPrintConfigParams(void)
{
    bool_t bParamsUpdated = FALSE;
    if(CT_IsShortcutMenuEnabled())
    {
        uint8_t crtChannel = GENFSK_GetChannelNumber(mAppGenfskId);
        uint8_t crtPwr     = GENFSK_GetTxPowerLevel(mAppGenfskId);
        if(gaConfigParams[1].paramValue.decValue != crtChannel)
        {
            if(gGenfskSuccess_c != 
               GENFSK_SetChannelNumber(mAppGenfskId, (uint8_t)gaConfigParams[1].paramValue.decValue))
            {
                gaConfigParams[1].paramValue.decValue = crtChannel;
            }
            else
            {
                bParamsUpdated = TRUE;
            }
        }
        if(gaConfigParams[2].paramValue.decValue != crtPwr)
        {
            if(gGenfskSuccess_c !=
               GENFSK_SetTxPowerLevel(mAppGenfskId, gaConfigParams[2].paramValue.decValue))
            {
                gaConfigParams[2].paramValue.decValue = crtPwr;
            }
            else
            {
                bParamsUpdated = TRUE;
            }
        }
        PrintTestParams(gaConfigParams, TRUE, mAppSerId);
    }
    return bParamsUpdated;
}
