/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"
#include "fsl_xcvr.h"

#include "Phy.h"
#include "PhyInterface.h"
#include "MpmInterface.h"
#include "AspInterface.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "Flash_Adapter.h"

#if gFsciIncluded_c
#include "FsciInterface.h"
#include "FsciAspCommands.h"
#endif

#ifdef gSmacSupported
#include "SMAC_Interface.h"
#endif

#if gAspCapability_d

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define mFAD_THR_ResetValue         0x82
#define mANT_AGC_CTRL_ResetValue    0x40
#define mASP_MinTxIntervalMS_d      (5)


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
enum {
  gDftNormal_c          = 0,
  gDftTxNoMod_Carrier_c = 1,
  gDftTxPattern_c       = 4, //should be 6,
  gDftTxRandom_c        = 7,
  gDftTxPnChipData_c    = 8,
  gDftTxExternalSrc_c   = 9
};


/*! *********************************************************************************
*************************************************************************************
* Private functions prototype
*************************************************************************************
********************************************************************************** */
static void ASP_PRBS9_Load ( void );
static void ASP_TxInterval ( uint32_t param );

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
static uint32_t mAsp_TxIntervalMs = mASP_MinTxIntervalMS_d;
static phyTimeTimerId_t mAsp_TxTimer = gInvalidTimerId_c;
static const uint8_t mAsp_Prbs9Packet[65] =
{
    0x42,
    0xff,0xc1,0xfb,0xe8,0x4c,0x90,0x72,0x8b,0xe7,0xb3,0x51,0x89,0x63,0xab,0x23,0x23,  
    0x02,0x84,0x18,0x72,0xaa,0x61,0x2f,0x3b,0x51,0xa8,0xe5,0x37,0x49,0xfb,0xc9,0xca,
    0x0c,0x18,0x53,0x2c,0xfd,0x45,0xe3,0x9a,0xe6,0xf1,0x5d,0xb0,0xb6,0x1b,0xb4,0xbe,
    0x2a,0x50,0xea,0xe9,0x0e,0x9c,0x4b,0x5e,0x57,0x24,0xcc,0xa1,0xb7,0x59,0xb8,0x87
};

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  Initialize the ASP module
*
* \param[in]  phyInstance The instance of the PHY
* \param[in]  interfaceId The FSCI interface used
*
********************************************************************************** */
void ASP_Init( instanceId_t phyInstance )
{
}

/*! *********************************************************************************
* \brief  ASP SAP handler.
*
* \param[in]  pMsg        Pointer to the request message
* \param[in]  instanceId  The instance of the PHY
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t APP_ASP_SapHandler(AppToAspMessage_t *pMsg, instanceId_t phyInstance)
{
    AspStatus_t status = gAspSuccess_c;
#if gFsciIncluded_c
    FSCI_Monitor( gFSCI_AspSapId_c,
                  pMsg,
                  NULL,
                  fsciGetAspInterfaceId(phyInstance) );
#endif
    switch( pMsg->msgType )
    {
    case aspMsgTypeGetTimeReq_c:
        Asp_GetTimeReq((uint64_t*)&pMsg->msgData.aspGetTimeReq.time);
        break;
    case aspMsgTypeSetXtalTrimReq_c:
        if(gXcvrSuccess_c == XCVR_SetXtalTrim(pMsg->msgData.aspXtalTrim.trim))
        {
            NV_ReadHWParameters(&gHardwareParameters);
            if(gHardwareParameters.xtalTrim != pMsg->msgData.aspXtalTrim.trim)
            {
              gHardwareParameters.xtalTrim = pMsg->msgData.aspXtalTrim.trim;
              NV_WriteHWParameters(&gHardwareParameters);
            }
        }
        else
        {
            status = gAspInvalidParameter_c;
        }
        break;
    case aspMsgTypeGetXtalTrimReq_c:
        *((uint8_t*)&status) = XCVR_GetXtalTrim(); /* remove compiler warning */
        break;
    case aspMsgTypeXcvrWriteReq_c:
        status = Asp_XcvrWriteReq( pMsg->msgData.aspXcvrData.mode,
                                   pMsg->msgData.aspXcvrData.addr,
                                   pMsg->msgData.aspXcvrData.len,
                                   pMsg->msgData.aspXcvrData.data);
        break;
    case aspMsgTypeXcvrReadReq_c:
        status = Asp_XcvrReadReq( pMsg->msgData.aspXcvrData.mode,
                                  pMsg->msgData.aspXcvrData.addr,
                                  pMsg->msgData.aspXcvrData.len,
                                  pMsg->msgData.aspXcvrData.data);
        break;
    case aspMsgTypeSetFADState_c:
        status = Asp_SetFADState(pMsg->msgData.aspFADState);
        break;
    case aspMsgTypeSetFADThreshold_c:
        status = Asp_SetFADThreshold(pMsg->msgData.aspFADThreshold);
        break;
    case aspMsgTypeSetANTXState_c:
        status = Asp_SetANTXState(pMsg->msgData.aspANTXState);
        break;
    case aspMsgTypeGetANTXState_c:
        *((uint8_t*)&status) = Asp_GetANTXState();
        break;
    case aspMsgTypeSetPowerLevel_c:
        status = Asp_SetPowerLevel(pMsg->msgData.aspSetPowerLevelReq.powerLevel);
        break;
    case aspMsgTypeGetPowerLevel_c:
        *((uint8_t*)&status) = Asp_GetPowerLevel(); /* remove compiler warning */
        break;
    case aspMsgTypeTelecSetFreq_c:
        status = ASP_TelecSetFreq(pMsg->msgData.aspTelecsetFreq.channel);
        break;
    case aspMsgTypeTelecSendRawData_c:
        status = ASP_TelecSendRawData((uint8_t*)&pMsg->msgData.aspTelecSendRawData);
        break;
    case aspMsgTypeTelecTest_c:
        status = ASP_TelecTest(pMsg->msgData.aspTelecTest.mode);
        break;
    case aspMsgTypeSetLQIMode_c:
        status = Asp_SetLQIMode(pMsg->msgData.aspLQIMode);
        break;
    case aspMsgTypeGetRSSILevel_c:
        *((uint8_t*)&status) = Asp_GetRSSILevel(); /* remove compiler warning */
        break;
    case aspMsgTypeSetTxInterval_c:
        if( pMsg->msgData.aspSetTxInterval.intervalMs >= mASP_MinTxIntervalMS_d )
        {
            mAsp_TxIntervalMs = pMsg->msgData.aspSetTxInterval.intervalMs;
        }
        else
        {
            status = gAspInvalidParameter_c;
        }
        break;
#if gMpmIncluded_d
    case aspMsgTypeSetMpmConfig_c:
        {
            mpmConfig_t cfg = {
                .autoMode = pMsg->msgData.MpmConfig.autoMode,
                .dwellTime = pMsg->msgData.MpmConfig.dwellTime,
                .activeMAC = pMsg->msgData.MpmConfig.activeMAC
            };

            MPM_SetConfig(&cfg);
        }
        break;
    case aspMsgTypeGetMpmConfig_c:
        {
            mpmConfig_t cfg;

            MPM_GetConfig(&cfg);
            pMsg->msgData.MpmConfig.autoMode = cfg.autoMode;
            pMsg->msgData.MpmConfig.dwellTime = cfg.dwellTime;
            pMsg->msgData.MpmConfig.activeMAC = cfg.activeMAC;
        }
        break;
#endif
    default:
        status = gAspInvalidRequest_c; /* OR gAspInvalidParameter_c */
        break;
    }
#if gFsciIncluded_c
    FSCI_Monitor( gFSCI_AspSapId_c,
                  pMsg,
                  (void*)&status,
                  fsciGetAspInterfaceId(phyInstance) );
#endif
    return status;
}

/*! *********************************************************************************
* \brief  Returns the current PHY time
*
* \param[in]  time  location where the PHY time will be stored
*
********************************************************************************** */
void Asp_GetTimeReq(uint64_t *time)
{
    PhyTimeReadClock( time );
}

/*! *********************************************************************************
* \brief  Write XCVR registers
*
* \param[in]  mode   ZLL/XCVR access
* \param[in]  addr   address
* \param[in]  len    number of bytes to write
* \param[in]  pData  data o be written
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t Asp_XcvrWriteReq (uint8_t mode, uint16_t addr, uint8_t len, uint8_t* pData)
{
    if (mode)
    {
        /* Indirect access: XCVR regs */
        FLib_MemCpy((uint8_t*)XCVR_RX_DIG + addr, pData, len);
    }
    else
    {
        /* Direct access: ZLL regs */
        FLib_MemCpy((uint8_t*)ZLL + addr, pData, len);
    }

    return gAspSuccess_c;
}

/*! *********************************************************************************
* \brief  Read XCVR registers
*
* \param[in]  mode   Direct/Indirect access
* \param[in]  addr   XCVR address
* \param[in]  len    number of bytes to read
* \param[in]  pData  location where data will be stored
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t Asp_XcvrReadReq  (uint8_t mode, uint16_t addr, uint8_t len, uint8_t* pData)
{
    if (mode)
    {
        /* Indirect access: XCVR regs */
        FLib_MemCpy(pData, (uint8_t*)XCVR_RX_DIG + addr, len);
    }
    else
    {
        /* Direct access: ZLL regs */
        FLib_MemCpy(pData, (uint8_t*)ZLL + addr, len);
    }

    return gAspSuccess_c;
}

/*! *********************************************************************************
* \brief  Set Tx output power level
*
* \param[in]  powerLevel   The new power level: 0x03-0x1F (see documentation for details)
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t Asp_SetPowerLevel( uint8_t powerLevel )
{
    AspStatus_t res = gAspSuccess_c;

    if( gPhySuccess_c != PhyPlmeSetPwrLevelRequest(powerLevel) )
    {
        res = gAspInvalidParameter_c;
    }

    return res;
}

/*! *********************************************************************************
* \brief  Read the current Tx power level
*
* \return  power level
*
********************************************************************************** */
uint8_t Asp_GetPowerLevel(void)
{
    return PhyPlmeGetPwrLevelRequest();
}

/*! *********************************************************************************
* \brief  Set the state of Active Promiscuous functionality
*
* \param[in]  state  new state 
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t Asp_SetActivePromState(bool_t state)
{
    PhySetActivePromiscuous(state);
    return gAspSuccess_c;
}

/*! *********************************************************************************
* \brief  Set the state of Fast Antenna Diversity functionality
*
* \param[in]  state  new state 
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t Asp_SetFADState(bool_t state)
{
    AspStatus_t status = gAspSuccess_c;

    if( gPhySuccess_c != PhyPlmeSetFADStateRequest(state) )
    {
        status = gAspDenied_c;
    }

    return status;
}

/*! *********************************************************************************
* \brief  Set the Fast Antenna Diversity threshold
*
* \param[in]  threshold 
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t Asp_SetFADThreshold(uint8_t thresholdFAD)
{
    AspStatus_t status = gAspSuccess_c;

    if( gPhySuccess_c != PhyPlmeSetFADThresholdRequest(thresholdFAD) )
    {
        status = gAspDenied_c;
    }
    return status;
}

/*! *********************************************************************************
* \brief  Set the ANTX functionality
*
* \param[in]  state 
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t Asp_SetANTXState(bool_t state)
{
    AspStatus_t status = gAspSuccess_c;
    
    if( gPhySuccess_c != PhyPlmeSetANTXStateRequest(state) )
    {
        status = gAspDenied_c;
    }
    return status;
}

/*! *********************************************************************************
* \brief  Get the ANTX functionality
*
* \return  current state
*
********************************************************************************** */
uint8_t Asp_GetANTXState(void)
{
  return PhyPlmeGetANTXStateRequest();
}

/*! *********************************************************************************
* \brief  Set the ANTX pad state
*
* \param[in]  antAB_on 
* \param[in]  rxtxSwitch_on 
*
* \return  status
*
********************************************************************************** */
uint8_t Asp_SetANTPadStateRequest(bool_t antAB_on, bool_t rxtxSwitch_on)
{
    return PhyPlmeSetANTPadStateRequest(antAB_on, rxtxSwitch_on);
}

/*! *********************************************************************************
* \brief  Set the ANTX pad strength
*
* \param[in]  hiStrength 
*
* \return  status
*
********************************************************************************** */
uint8_t Asp_SetANTPadStrengthRequest(bool_t hiStrength)
{
    return 0;
}

/*! *********************************************************************************
* \brief  Set the ANTX inverted pads
*
* \param[in]  invAntA  invert Ant_A pad
* \param[in]  invAntB  invert Ant_B pad
* \param[in]  invTx    invert Tx pad
* \param[in]  invRx    invert Rx pad
*
* \return  status
*
********************************************************************************** */
uint8_t Asp_SetANTPadInvertedRequest(bool_t invAntA, bool_t invAntB, bool_t invTx, bool_t invRx)
{
    return PhyPlmeSetANTPadInvertedRequest(invAntA, invAntB, invTx, invRx);
}

/*! *********************************************************************************
* \brief  Set the LQI mode
*
* \param[in]  mode 
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t Asp_SetLQIMode(bool_t mode)
{
    AspStatus_t status = gAspSuccess_c;

    if( gPhySuccess_c != PhyPlmeSetLQIModeRequest(mode) )
    {
        status = gAspDenied_c;
    }
    return status;
}

/*! *********************************************************************************
* \brief  Get the last RSSI level
*
* \return  RSSI
*
********************************************************************************** */
uint8_t Asp_GetRSSILevel(void)
{
  return PhyPlmeGetRSSILevelRequest();
}

/*! *********************************************************************************
* \brief  Set current channel
*
* \param[in]  channel  channel number (11-26)
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t ASP_TelecSetFreq(uint8_t channel)
{
    AspStatus_t status = gAspSuccess_c;
    
    PhyPlmeForceTrxOffRequest();
    
    if( gPhySuccess_c != PhyPlmeSetCurrentChannelRequest(channel,0) )
    {
        status = gAspInvalidParameter_c;
    }

    return status;
}

/*! *********************************************************************************
* \brief  Send a raw data frame OTA
*
* \param[in]  dataPtr  raw data
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t ASP_TelecSendRawData(uint8_t *dataPtr)
{
    AspStatus_t status = gAspSuccess_c;
    uint8_t *pTxBuffer = (uint8_t*)ZLL->PKT_BUFFER_TX;
    uint32_t len;

    /* Validate the length */
    if ((dataPtr[0] + 2) > gMaxPHYPacketSize_c)
    {
        status = gAspTooLong_c;
    }
    else
    {
        /* Force Idle */
        PhyPlmeForceTrxOffRequest();
        XCVR_TX_DIG->CTRL &= ~XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK;
        ZLL->SEQ_CTRL_STS &= ~ZLL_SEQ_CTRL_STS_CONTINUOUS_EN_MASK;

        len = *dataPtr++;
        /* Add FCS length to PSDU Length*/
        *pTxBuffer++ = len + 2;
        /* Load the TX PB: load the PSDU Length byte but not the FCS bytes */
        while (len--)
        {
            *pTxBuffer++ = *dataPtr++;
        }
        /* Program a Tx sequence */
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_XCVSEQ(gTX_c);
    }

    return status;
}

/*! *********************************************************************************
* \brief  Set Telec test mode
*
* \param[in]  mode  Telec test mode
*
* \return  AspStatus_t
*
********************************************************************************** */
AspStatus_t ASP_TelecTest(uint8_t mode)
{
    uint8_t channel;
    AspStatus_t status = gAspSuccess_c;
    static bool_t fracSet = FALSE;

    /* Get current channel number */
    channel = PhyPlmeGetCurrentChannelRequest(0);

    if( fracSet )
    {
        ASP_TelecSetFreq(channel);
        fracSet = FALSE;
    }

    switch( mode )
    {
    case gTestForceIdle_c:  /* ForceIdle() */
        /* Stop Tx interval timer (if started) */
        PhyTime_CancelEvent(mAsp_TxTimer);
#ifdef gSmacSupported
        MLMEPhySoftReset();
#else
        PhyPlmeForceTrxOffRequest();
#endif
        XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK | 
                               XCVR_TX_DIG_CTRL_LFSR_EN_MASK | 
                               XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
        XCVR_TSM->CTRL &= ~XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
        XCVR_MISC->DTEST_CTRL &= ~XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
        ZLL->SEQ_CTRL_STS &= ~ZLL_SEQ_CTRL_STS_CONTINUOUS_EN_MASK;
        break;

    case gTestPulseTxPrbs9_c:  /* Continuously transmit a PRBS9 pattern. */
        XCVR_TX_DIG->CTRL &= ~XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK;
        ASP_PRBS9_Load(); /* Load the TX RAM */
        /* Enable continuous TX mode */
        ZLL->SEQ_CTRL_STS |= ZLL_SEQ_CTRL_STS_CONTINUOUS_EN_MASK;
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_XCVSEQ(gTX_c);
        break;

    case gTestContinuousRx_c:  /* Sets the device into continuous RX mode */
        XCVR_TX_DIG->CTRL &= ~XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK;
        /* Set length of data in DUAL_PAN_DWELL register */
        ZLL->DUAL_PAN_CTRL &= ~ZLL_DUAL_PAN_CTRL_DUAL_PAN_DWELL_MASK;
        ZLL->DUAL_PAN_CTRL |= ZLL_DUAL_PAN_CTRL_DUAL_PAN_DWELL(127);
        /* Enable continuous RX mode */
        ZLL->SEQ_CTRL_STS |= ZLL_SEQ_CTRL_STS_CONTINUOUS_EN_MASK;
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_XCVSEQ(gRX_c);
        break;

    case gTestContinuousTxMod_c:  /* Sets the device to continuously transmit a 10101010 pattern */
        XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                               XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
        XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxPattern_c) |
                             XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK;
        XCVR_TX_DIG->DFT_PATTERN = 0xAAAAAAAAU;
        XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
        XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
        break;

    case gTestContinuousTxNoMod_c: /* Sets the device to continuously transmit an unmodulated CW */
        XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                               XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
        XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxNoMod_Carrier_c);
        XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
        XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
        fracSet = TRUE;
        break;

    case gTestContinuousTx1MbpsPRBS9_c:
        XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                               XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK |
                               XCVR_TX_DIG_CTRL_LFSR_LENGTH_MASK);
        XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxPnChipData_c) |
                             XCVR_TX_DIG_CTRL_LFSR_LENGTH(0) | /* length 9 */
                             XCVR_TX_DIG_CTRL_LFSR_EN_MASK;
        XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
        XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
        break;

    case gTestContinuousTxExternalSrc_c:
        XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                               XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
        XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxExternalSrc_c);
        XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
        XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
        break;

    case gTestContinuousTxModZero_c:
        XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                               XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
        XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxPattern_c) |
                             XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK;
        XCVR_TX_DIG->DFT_PATTERN = 0x00000000;
        XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
        XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
        break;

    case gTestContinuousTxModOne_c:
        XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                               XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
        XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxPattern_c) |
                             XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK;
        XCVR_TX_DIG->DFT_PATTERN = 0xFFFFFFFFU;
        XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
        XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
        break;

    case gTestTxPacketPRBS9_c:
        ASP_TxInterval( (uint32_t)mAsp_Prbs9Packet );
        break;
        
    default:
        status = gAspInvalidParameter_c;
        break;
    }

    return status;
}

/*! *********************************************************************************
* \brief  Transmit a raw data packet at a specific interval
*
* \param[in]  address of the raw data packet
*
********************************************************************************** */
static void ASP_TxInterval ( uint32_t param )
{
    phyTimeEvent_t ev;
    
    /* convert interval to symbols */
    ev.timestamp = ((uint64_t)mAsp_TxIntervalMs * 1000) / 16;
    ev.timestamp += PhyTime_GetTimestamp();
    ev.callback = ASP_TxInterval;
    ev.parameter = param;
    mAsp_TxTimer = PhyTime_ScheduleEvent(&ev);
    
    ASP_TelecSendRawData( (uint8_t*)param );
}

/*! *********************************************************************************
* \brief  Generate and load a PRBS9 pattern into Tx buffer
*
********************************************************************************** */
static void ASP_PRBS9_Load(void)
{
#if 1
    uint32_t i;

    for (i = 0; i < sizeof(mAsp_Prbs9Packet); i++)
    {
        ((uint8_t*)ZLL->PKT_BUFFER_TX)[i] = mAsp_Prbs9Packet[i];
    }
#else
    uint8_t c1;  /* Byte counter */
    uint8_t c2;  /* Bit counter */
    uint16_t t1; /* LFSR */
    uint16_t t2; /* LFSR output */
    uint16_t t3; /* LFSR feedback tap */
    uint8_t t4;  /* Assembled transmit byte */
    uint8_t *pTxBuffer = (uint8_t *)ZLL->PKT_BUFFER_TX;

    pTxBuffer[0] = 64;
    t1 = 0x01FF;                 /* Initialize the LFSR */
    for (c1 = 1; c1 <= 64; c1++) /* Byte counter */
    {
        t4 = 0x00;                 /* Initialize the byte */
        for (c2 = 0; c2 < 8; c2++) /* Bit counter */
        {
            t2 = (t1 & 0x0001); /* LFSR output */
            if (t2 == 0x0001)
            {
                t4 = t4 | 0x80; /* Set/Clear byte based on LFSR output */
            }
            if (c2 != 7)
            {
                t4 = t4 >> 1; /* LSBit will be first bit out of LFSR */
            }
            t3 = ((t1 & 0x0010u) >> 4u); /* LFSR tap */
            t1 = t1 >> 1;                /* Now shift the LFSR */
            if (t2 == t3)                /* Set/Clr the LFSR MSBit */
            {
                t1 = t1 & 0xFEFF;
            }
            else
            {
                t1 = t1 | 0x0100;
            }
        }
        pTxBuffer[c1] = t4;
    }
#endif
}

#endif /* gAspCapability_d */