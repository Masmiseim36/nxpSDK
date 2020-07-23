/*

Copyright 2016-2019 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"

#include "MemManager.h"
#include "FunctionLib.h"
#if defined (RADIO_IS_GEN_3P5)
#include "nxp_xcvr_coding_config.h"
#endif
#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
#include "MWS.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief GENFSK Maximum data length after the packet header including the payload and the CRC. */
#define gGENFSK_MaxPacketPayloadAndAdjLength_c     (2047U)

/*! @brief GENFSK Maximum RAW packet length. */
#if defined (RADIO_IS_GEN_3P5)
#define gGENFSK_MaxRawPacketLength_c     ((1UL << 10U) - 1U + gGENFSK_MaxSyncAddressSize_c)   /*1027*/
#else
#define gGENFSK_MaxRawPacketLength_c     ((1U << 5U) - 1U + gGENFSK_MaxSyncAddressSize_c)   /*35*/
#endif /* defined (RADIO_IS_GEN_3P5) */

/*! @brief GENFSK Maximum sync address size. */
#define gGENFSK_MaxSyncAddressSize_c     (4U)

#if !defined (RADIO_IS_GEN_3P5)
/*! @brief GENFSK PB_PARTITION settings for maximum TX packet length. */
#define gGENFSK_PbPartitionMaxTx_c    (1088U)

/*! @brief GENFSK PB_PARTITION settings for maximum RX packet length. */
#define gGENFSK_PbPartitionMaxRx_c    (0U)
#endif

/*! @brief GENFSK default SW overdead */
#define gGENFSK_Overhead_c          (200U) /* [us] */

/*! @brief GENFSK Protocol Engine commands. */
typedef enum _GENFSK_sequence_commands
{
    NO_ACTION = 0U,  /*!< No action. */
    TX_START_NOW = 1U,  /*!< TX start now. */
    TX_START_T1 = 2U, /*!< TX start @T1 Timer Compare Match (EVENT_TMR = T1_CMP). */
    TX_START_T2 = 3U,  /*!< TX start @T2 Timer Compare Match (EVENT_TMR = T2_CMP). */
    TX_CANCEL = 4U,  /*!< TX cancel -- Cancels pending TX events but do not abort a TX-in-progress. */
    RX_START_NOW = 5U,  /*!< RX start now. */
    RX_START_T1 = 6U,  /*!< RX start @T1 Timer Compare Match (EVENT_TMR = T1_CMP). */
    RX_START_T2 = 7U,  /*!< RX start @T2 Timer Compare Match (EVENT_TMR = T2_CMP). */
    RX_STOP_T1 = 8U,  /*!< RX stop @T1 Timer Compare Match (EVENT_TMR = T1_CMP). */
    RX_STOP_T2 = 9U,  /*!< RX stop @T2 Timer Compare Match (EVENT_TMR = T2_CMP). */
    RX_CANCEL = 10U,  /*!< RX cancel -- Cancels pending RX events but do not abort a RX-in-progress. */
    ABORT_ALL = 11U,  /*!< Abort all -- Cancels all pending events and abort any sequence-in-progress. */
#if defined (RADIO_IS_GEN_3P5)
    TR_START_NOW = 12U, /*!< - TR Start Now. */
    TR_START_T1 = 13U, /*!< TR Start @ T1 Timer Compare Match (EVENT_TMR = T1_CMP) */
    TR_START_T2 = 14U, /*!< TR Start @ T2 Timer Compare Match (EVENT_TMR = T2_CMP) */
    TR_CANCEL = 15U,   /*!< TR Cancel -- Cancels pending TR events but do not abort a TR-in-progress */
    CCA_START_NOW = 16U, /*!< CCA Start Now */
    CCA_START_T1 = 17U,  /*!< CCA Start @ T1 Timer Compare Match (EVENT_TMR = T1_CMP) */
    CCA_START_T2 = 18U,  /*!< CCA Start @ T2 Timer Compare Match (EVENT_TMR = T2_CMP) */
    CCA_CANCEL = 19U     /*!< CCA Cancel -- Cancels pending CCA events but do not abort a CCA-in-progress */
#endif
} GENFSK_sequence_commands;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*! @brief Send commands to GENFSK LL. */
GENFSK_STATIC_INLINE void GENFSK_Command(GENFSK_sequence_commands command);

/*! @brief GENFSK RX timeout callback function. */
static void GENFSK_RxTimeoutCallback(void);

/*! @brief GENFSK LL Task. */
static void GENFSK_Task(osaTaskParam_t argument);

#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
static uint32_t MWS_GENFSK_Callback(mwsEvents_t event);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

static OSA_TASK_DEFINE(GENFSK_Task, gGENFSK_TaskPriority_c, 1, gGENFSK_TaskStackSize_c, FALSE);

static bool_t genfskAlreadyInit = FALSE;

/*! @brief GENFSK default radio configuration. */
static const GENFSK_radio_config_t mDefaultRadioConfig =
{
    .radioMode = gGenfskGfskBt0p5h0p5,
    .dataRate = gGenfskDR1Mbps
};

/*! @brief GENFSK default packet configuration. */
static const GENFSK_packet_config_t mDefaultPacketConfig =
{
    .preambleSizeBytes = 0,
    .packetType = gGenfskFormattedPacket,
    .lengthSizeBits = 0,
    .lengthBitOrder = gGenfskLengthBitLsbFirst,
    .syncAddrSizeBytes = 1,
    .lengthAdjBytes = 0,
    .h0SizeBits = 0,
    .h1SizeBits = 0,
    .h0Match = 0,
    .h0Mask = 0,
    .h1Match = 0,
    .h1Mask = 0,
};

/*! @brief GENFSK default mode configuration. */
static const GENFSK_mode_config_t mDefaultModeConfig =
{
    .mode = gGenfskGllMode,
    .stayInRx = 0,
    .turnaroundTime = 200  /* Turnaround time changed to 200 us by default because the fast warmup is not yet functionnal. See Jira CONNRF-485. */
};

/*! @brief GENFSK default CRC configuration. */
static const GENFSK_crc_config_t mDefaultCrcConfig =
{
    .crcEnable = gGenfskCrcEnable,
    .crcRecvInvalid = gGenfskCrcRecvInvalid,
    .crcSize = 2,
    .crcStartByte = 0,
    .crcRefIn = gGenfskCrcInputNoRef,
    .crcRefOut = gGenfskCrcOutputNoRef,
    .crcByteOrder = gGenfskCrcLSByteFirst,
    .crcSeed = 0x0000,
    .crcPoly = 0x1021,
    .crcXorOut = 0
};

/*! @brief GENFSK default Whitener configuration. */
static const GENFSK_whitener_config_t mDefaultWhitenerConfig =
{
    .whitenEnable = gGenfskWhitenEnable,
    .whitenStart = gWhitenStartNoWhitening,
    .whitenEnd = gWhitenEndAtEndOfPayload,
    .whitenB4Crc = gWhitenB4Crc,
    .whitenPolyType = gFibonnaciPolyType,
    .whitenRefIn = gGenfskWhitenInputNoRef,
    .whitenPayloadReinit = gGenfskWhitenNoPayloadReinit,
    .whitenSize = 9,
    .whitenInit = 0x1FF,
    .whitenPoly = 0x21,
    .whitenSizeThr = 0x800,
    .manchesterEn = gGenfskManchesterDisable,
    .manchesterStart = gGenfskManchesterStartAtPayload,
    .manchesterInv = gGenfskManchesterNoInv,
};

/*! @brief GENFSK default network address configuration. */
static const GENFSK_nwk_addr_match_t mDefaultNwkAddrConfig =
{
    .nwkAddrSizeBytes = 1,
    .nwkAddrThrBits = 0,
    .nwkAddr = 0x55555555U,
};

/*! @brief GENFSK local structure initial configuration. */
GENFSK_LocalStruct_t genfskLocal[gGENFSK_InstancesCnt_c];
/*! @brief GENFSK active instance. */
uint8_t mGenfskActiveInstance = gGENFSK_InvalidIdx_c;
/*! @breif GENFSK total number of allocated instances. */
static uint8_t mNumberOfAllocatedInstances = 0;
/*! @brief GENFSK RX timeout timer ID. */
genfskTimerId_t rxTimeoutTimer = gGENFSK_InvalidTimerId_c;

static osaTaskId_t gGenfskTaskId = NULL;
osaEventId_t mGenfskTaskEvent;

#if defined(RADIO_IS_GEN_3P0) && (RADIO_IS_GEN_3P0 != 0) && defined(RF_OSC_26MHZ) && (RF_OSC_26MHZ != 0)
uint8_t gGenfskRxRecycleHdrError;
uint8_t gGenfskRxRecycleCrcError;
#endif

#ifdef RADIO_IS_GEN_3P5
#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
extern uint16_t gRfRadioVer_c;
#else /* (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c) */
/* For the application using coexistence, gRfRadioVer_c is declared in ble_init.c */
GENFSK_STATIC uint16_t gRfRadioVer_c = (uint16_t)(XCVR_RADIO_GEN_INVALID);
#endif /* gGENFSK_NoMwsControl_c */
#endif /* RADIO_IS_GEN_3P5 */

/*******************************************************************************
 * Code
 ******************************************************************************/
GENFSK_STATIC_INLINE void GENFSK_Command(GENFSK_sequence_commands command)
{
#if defined(RADIO_IS_GEN_3P5)
    if (gRfRadioVer_c == (uint16_t)(XCVR_RADIO_GEN_3P5))
    {
        /*Workaround to fix PDM issue: TKT0519094 Wrong pre-set of enc_byte_counter if PREAMBLE_SZ not equals to 0, due to clock being gated.
        Jira ticket: CONNRF-499*/
        RADIO_CTRL->RF_CTRL_OVRD |= RADIO_CTRL_RF_CTRL_OVRD_RBME_CLK_EN_OVRD_MASK;
        /* LCOV_EXCL_START */
        while ( (RADIO_CTRL->RF_CTRL_OVRD & RADIO_CTRL_RF_CTRL_OVRD_RBME_CLK_EN_OVRD_MASK) == 0U )
        {
            ASM_NOP();
        }
        /* LCOV_EXCL_STOP */
        RADIO_CTRL->RF_CTRL_OVRD &= ~RADIO_CTRL_RF_CTRL_OVRD_RBME_CLK_EN_OVRD_MASK;
    }
#endif /* defined(RADIO_IS_GEN_3P5) */
    GENFSK->XCVR_CTRL = (uint32_t)command;
}

#if defined (RADIO_IS_GEN_3P5)
static genfskStatus_t GENFSK_SelectRateConfig(uint8_t instanceId, genfskRadioMode_t radioModeIn, genfskDataRate_t dataRate)
{
    genfskStatus_t status = gGenfskSuccess_c;
    if (dataRate != gGenfskDR1Mbps)
    {
        /* 2Mbps specific configuration is alternate data rate under 1Mbps configuration.
           500Kbps and 250Kbps configurations are the alternate date rates under their configuration structures.
           The selection is made through DATARATE_CONFIG_SEL bit in ENH_FEATURE register:
               bank0 => 1Mbps
               bank1 => 2Mbps, 500K, or 250K (depending upon the configuration structure selected)             */
       genfskLocal[instanceId].genfskRegs.enhFeature |= GENFSK_ENH_FEATURE_DATARATE_CONFIG_SEL_MASK;
    }
    else
    {
        /* For the other data rates, clear the bit to get bank0 selected by default*/
        genfskLocal[instanceId].genfskRegs.enhFeature &= ~GENFSK_ENH_FEATURE_DATARATE_CONFIG_SEL_MASK;
    }

    GENFSK->ENH_FEATURE = genfskLocal[instanceId].genfskRegs.enhFeature;

    return status;
}

static void GENFSK_ChangeMode(uint8_t instanceId, const GENFSK_radio_config_t *radioConfig, const xcvr_config_t *xcvrConfig, const xcvr_coding_config_t *rbmeConfig)
{
    bool_t is_high_power_configured = false;
    /*Due to changes to latest RF driver (PJ_XCVR_GEN3.6_KW37_REV_B0_EARLY_BRING_UP), the default value for LL_CTRL has changed from 3 to 0. */
    RADIO_CTRL->LL_CTRL = RADIO_CTRL_LL_CTRL_ACTIVE_LL(3);
    
    /* As XCVR_ChangeMode() API will restore registers to default, need to remember that
    high power was configured in order to restore it after XCVR_ChangeMode() completes. */
    if (GENFSK_IsHighPowerConfigured() == 0x1U)
    {
        is_high_power_configured = true;
        (void)GENFSK_ConfigurePower(0x0U);
    }
    (void)XCVR_ChangeMode(&xcvrConfig, &rbmeConfig);
    
    /* Restore high power if needed. */
    if (is_high_power_configured)
    {
        (void)GENFSK_ConfigurePower(0x1U);
    }
    
#if defined (RADIO_IS_GEN_3P5)
    /* TODO: remove this after radio init sequence rework as XCVR_MISC->XCVR_CTRL is overwritten during radio init*/
    XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK);
    XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL(GENFSK_LL));
#endif
    (void)XCVR_SetActiveLL(XCVR_ACTIVE_LL_GENFSK);
    (void)GENFSK_SelectRateConfig(instanceId, radioConfig->radioMode, radioConfig->dataRate);

    /*Restore whitener and crc config (RBME parameter) due to XCVR_ChangeMode() API called above*/
    RBME->CRCW_CFG3 = genfskLocal[instanceId].genfskRegs.crcCfg;
    RBME->CRC_INIT = genfskLocal[instanceId].genfskRegs.crcInit;
    RBME->CRC_POLY = genfskLocal[instanceId].genfskRegs.crcPoly;
    RBME->CRC_XOR_OUT = genfskLocal[instanceId].genfskRegs.crcXorOut;
    RBME->WHITEN_CFG = genfskLocal[instanceId].genfskRegs.whitenCfg;
    RBME->WHITEN_POLY = genfskLocal[instanceId].genfskRegs.whitenPoly;
    RBME->WHITEN_SZ_THR = genfskLocal[instanceId].genfskRegs.whitenSzThr;
}
#endif /* defined (RADIO_IS_GEN_3P5) */

GENFSK_STATIC genfskStatus_t GENFSK_GetBusyStatus(GENFSK_LL_state_t genfskState)
{
    genfskStatus_t status;

    /*
     * Convert internal representation of GENFSK state to public enum type.
     * Return GenfskSuccess_c if GENFSK is not busy.
     */
    switch (genfskState)
    {
    case gGENFSK_LL_NoInit:
        status = gGenfskFail_c;
        break;
    case gGENFSK_LL_BusyRx:
        status = gGenfskBusyRx_c;
        break;
    case gGENFSK_LL_BusyTx:
        status = gGenfskBusyTx_c;
        break;
    case gGENFSK_LL_BusyPendingRx:
        status = gGenfskBusyPendingRx_c;
        break;
    case gGENFSK_LL_BusyPendingTx:
        status = gGenfskBusyPendingTx_c;
        break;
#if defined (RADIO_IS_GEN_3P5)
    case gGENFSK_LL_BusyRxTx:
        status = gGenfskBusyRx_c;
        break;
    case gGENFSK_LL_BusyTxRx:
        status = gGenfskBusyTx_c;
        break;
    case gGENFSK_LL_BusyPendingRxTx:
        status = gGenfskBusyPendingRx_c;
        break;
    case gGENFSK_LL_BusyPendingTxRx:
        status = gGenfskBusyPendingTx_c;
        break;
#endif
    case gGENFSK_LL_Idle:
        status = gGenfskSuccess_c;
        break;
    default:
        status = gGenfskFail_c;
        break;
    }

    return status;
}

static void GENFSK_SwitchToInstance(uint8_t instanceId)
{
    uint32_t irqSts;

#if !defined (RADIO_IS_GEN_3P5)
    radio_mode_t radioMode = NUM_RADIO_MODES;
#else
    const xcvr_config_t *xcvrConfig = NULL;
    /* For now, until XCVR implementation is available, RBME is configured manually in GENFSK code */
    const xcvr_coding_config_t *rbmeConfig = &xcvr_ble_uncoded_config;
#endif

    OSA_InterruptDisable();

    irqSts = GENFSK->IRQ_CTRL & gGENFSK_AllIrqFlags;
    irqSts |= genfskLocal[instanceId].genfskRegs.irqCtrl;
    GENFSK->IRQ_CTRL |= irqSts;
    genfskLocal[instanceId].genfskRegs.irqCtrl = irqSts;

    GENFSK->XCVR_CFG &= ~(GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK |
                          GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK |
                          GENFSK_XCVR_CFG_SW_CRC_EN_MASK |
#if defined (RADIO_IS_GEN_3P5)
                          GENFSK_XCVR_CFG_PREAMBLE_SEL_MASK |
#endif
                          GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK);

    /* Restore all registers from Instance's storage */
    GENFSK->XCVR_CFG = genfskLocal[instanceId].genfskRegs.xcvrCfg;
#if defined (RADIO_IS_GEN_3P5)
    GENFSK->CHANNEL_NUM0 = genfskLocal[instanceId].genfskRegs.channelNum;
#else
    GENFSK->CHANNEL_NUM = genfskLocal[instanceId].genfskRegs.channelNum;
#endif
    GENFSK->TX_POWER = genfskLocal[instanceId].genfskRegs.txPower;
    GENFSK->NTW_ADR_CTRL = genfskLocal[instanceId].genfskRegs.ntwAdrCtrl;
    GENFSK->NTW_ADR_0 = genfskLocal[instanceId].genfskRegs.ntwAdr0;
    GENFSK->NTW_ADR_1 = genfskLocal[instanceId].genfskRegs.ntwAdr1;
    GENFSK->NTW_ADR_2 = genfskLocal[instanceId].genfskRegs.ntwAdr2;
    GENFSK->NTW_ADR_3 = genfskLocal[instanceId].genfskRegs.ntwAdr3;
    GENFSK->PACKET_CFG = genfskLocal[instanceId].genfskRegs.packetCfg;
    GENFSK->H0_CFG = genfskLocal[instanceId].genfskRegs.h0Cfg;
    GENFSK->H1_CFG = genfskLocal[instanceId].genfskRegs.h1Cfg;
#if defined (RADIO_IS_GEN_3P5)
    GENFSK->LENGTH_ADJ = genfskLocal[instanceId].genfskRegs.lengthAdj;
    GENFSK->ENH_FEATURE = genfskLocal[instanceId].genfskRegs.enhFeature;
    GENFSK->TURNAROUND_TIME = genfskLocal[instanceId].genfskRegs.turnaroundTime;
    //GENFSK->BITRATE = genfskLocal[instanceId].genfskRegs.bitRate;
#else
    GENFSK->CRC_CFG = genfskLocal[instanceId].genfskRegs.crcCfg;
    GENFSK->CRC_INIT = genfskLocal[instanceId].genfskRegs.crcInit;
    GENFSK->CRC_POLY = genfskLocal[instanceId].genfskRegs.crcPoly;
    GENFSK->CRC_XOR_OUT = genfskLocal[instanceId].genfskRegs.crcXorOut;
    GENFSK->WHITEN_CFG = genfskLocal[instanceId].genfskRegs.whitenCfg;
    GENFSK->WHITEN_POLY = genfskLocal[instanceId].genfskRegs.whitenPoly;
    GENFSK->WHITEN_SZ_THR = genfskLocal[instanceId].genfskRegs.whitenSzThr;
    GENFSK->BITRATE = genfskLocal[instanceId].genfskRegs.bitRate;
#endif

#if !defined (RADIO_IS_GEN_3P5)
        (void)GENFSK_GetXcvrConfig(genfskLocal[instanceId].radioConfig.radioMode, &radioMode);
        (void)XCVR_ChangeMode(radioMode, (data_rate_t)genfskLocal[instanceId].radioConfig.dataRate);
#else
        (void)GENFSK_GetXcvrConfig(genfskLocal[instanceId].radioConfig.radioMode, genfskLocal[instanceId].radioConfig.dataRate, &xcvrConfig);
        GENFSK_ChangeMode(instanceId, &genfskLocal[instanceId].radioConfig, xcvrConfig, rbmeConfig);
#endif

    if (
#ifndef RADIO_IS_GEN_3P5
        (genfskLocal[instanceId].radioConfig.radioMode == gGenfskFsk) ||
#endif /* RADIO_IS_GEN_3P5 */
        (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)
        )
    {
#if 0 // OJE TBD
        XCVR_TX_DIG->GFSK_COEFF1 = ((0U)<<0 | (511U)<<7 | (0U)<<16 | (511U)<<23);
        XCVR_TX_DIG->GFSK_COEFF2 = 0x00000000U;
        XCVR_TX_DIG->GFSK_CTRL |= XCVR_TX_DIG_GFSK_CTRL_GFSK_FLD_MASK;
#endif
    }

    mGenfskActiveInstance = instanceId;

    OSA_InterruptEnable();
}

GENFSK_STATIC void GENFSK_CallEventNotifyCallback(uint32_t eventType, genfskEventStatus_t eventStatus)
{
    /* Call eventNotifyCallback if configured AND event is enabled */
    if ((genfskLocal[mGenfskActiveInstance].eventNotifyCallback != NULL) &&
        ((genfskLocal[mGenfskActiveInstance].enabledEvents & eventType) != 0U))
    {
        genfskLocal[mGenfskActiveInstance].eventNotifyCallback(eventType, eventStatus);
    }
}

static void GENFSK_RxTimeoutCallback(void)
{
    /* Reset the timer ID as it's no more usable (event consumed) */
    rxTimeoutTimer = gGENFSK_InvalidTimerId_c;

    GENFSK_AbortAll();

    GENFSK_CallEventNotifyCallback(gGenfskRxEvent, gGenfskTimeout);
}

genfskStatus_t GENFSK_SetBleWhitenInit(uint8_t instanceId, uint8_t channelNum)
{
    genfskStatus_t status = gGenfskSuccess_c;
    uint8_t channel;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else if ((channelNum < 42U) || (channelNum > (42U+(39U*2U))) || ((channelNum & 0x1U) != 0U))
    {
        /* Consider only BLE channels, ie GENFSK channels from 42 to 120 (included) by step of 2*/
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        if (mGenfskActiveInstance == instanceId)
        {
            uint16_t tempValInit;

            /* Only consider BLE channels which are separated 2 MHz appart of each other*/
            channelNum = (channelNum - 42U) / 2U;

            channel = channelNum;

            /*"RF channel" to  "Channel Index" mapping*/
            if(0U == channelNum)
            {
                channel = 37U;
            }
            else if(12U == channelNum)
            {
                channel = 38U;
            }
            else if (11U >= channelNum)
            {
                channel = channelNum - 1U;
            }
            else if (39U != channelNum)
            {
                channel = channelNum - 2U;
            }
            else
            {
                /*Misra rule 15.7*/
            }

            /* Apply the whitening init as defined in the Core specification Vol 6, Part B, Section 2.2. Note that according to spec
            the channel index bits are reversed but as prior storing the value to WHITEN_CFG, the value needs to be reversed again,
            the final formatting is as follows:*/
            tempValInit = (uint16_t)(channel) | 0x40U;

#if !defined (RADIO_IS_GEN_3P5)
            genfskLocal[instanceId].genfskRegs.whitenCfg &= ~GENFSK_WHITEN_CFG_WHITEN_INIT_MASK;
            genfskLocal[instanceId].genfskRegs.whitenCfg |= GENFSK_WHITEN_CFG_WHITEN_INIT(tempValInit);
            GENFSK->WHITEN_CFG = genfskLocal[instanceId].genfskRegs.whitenCfg;
#else
            genfskLocal[instanceId].genfskRegs.whitenCfg &= ~RBME_WHITEN_CFG_WHITEN_INIT_MASK;
            genfskLocal[instanceId].genfskRegs.whitenCfg |= RBME_WHITEN_CFG_WHITEN_INIT(tempValInit);
            RBME->WHITEN_CFG = genfskLocal[instanceId].genfskRegs.whitenCfg;
#endif
        }
    }

    return status;
}

genfskStatus_t GENFSK_Init(void)
{
    genfskStatus_t status = gGenfskSuccess_c;

    if (TRUE == genfskAlreadyInit)
    {
        status = gGenfskAlreadyInit_c;
    }
    else
    {
        FLib_MemSet(genfskLocal, 0, sizeof(genfskLocal));

        mNumberOfAllocatedInstances = 0;
        rxTimeoutTimer = gGENFSK_InvalidTimerId_c;

        for (uint8_t i = 0; i < gGENFSK_InstancesCnt_c; i++)
        {
            genfskLocal[i].enabledEvents = gGenfskDefaultEnabledEvents;
        }

        mGenfskTaskEvent = OSA_EventCreate(TRUE);

        /* LCOV_EXCL_START */
        if (NULL == mGenfskTaskEvent)
        {
            status = gGenfskFail_c;
        }
        else
        /* LCOV_EXCL_STOP */
        {
            gGenfskTaskId = OSA_TaskCreate(OSA_TASK(GENFSK_Task), NULL);

            /* LCOV_EXCL_START */
            if (NULL == gGenfskTaskId)
            {
                status = gGenfskFail_c;
            }
            else
            /* LCOV_EXCL_STOP */
            {
                genfskAlreadyInit = TRUE;
            }
        }
    }

    return status;
}

genfskStatus_t GENFSK_AllocInstance(uint8_t *pInstanceId, const GENFSK_radio_config_t *radioConfig, const GENFSK_packet_config_t *packetConfig, const GENFSK_bitproc_t *bitProcConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;
    uint8_t instanceId = gGENFSK_InvalidIdx_c;

#if gGENFSK_CheckParams_c == 1
    if (pInstanceId == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    {
        for (uint8_t i = 0; i < gGENFSK_InstancesCnt_c; i++)
        {
            if (genfskLocal[i].genfskState == gGENFSK_LL_NoInit)
            {
                genfskLocal[i].genfskState = gGENFSK_LL_Idle;
                instanceId = i;
                break;
            }
        }

        if (instanceId != gGENFSK_InvalidIdx_c)
        {
            /* Enter critical section. */
            OSA_InterruptDisable();

            /* Set current istance as active if it is the first instance initiated. */
            if (mGenfskActiveInstance == gGENFSK_InvalidIdx_c)
            {
                mGenfskActiveInstance = instanceId;
            }

            if (radioConfig == NULL)
            {
                status =  GENFSK_RadioConfig(instanceId, &mDefaultRadioConfig);
            }
            else
            {
                status = GENFSK_RadioConfig(instanceId, radioConfig);
            }

            if (status == gGenfskSuccess_c)
            {
                if (packetConfig == NULL)
                {
                    status = GENFSK_SetPacketConfig(instanceId, &mDefaultPacketConfig);
                }
                else
                {
                    status = GENFSK_SetPacketConfig(instanceId, packetConfig);
                }
            }

            if (status == gGenfskSuccess_c)
            {
                if (bitProcConfig == NULL)
                {
                     status = GENFSK_SetCrcConfig(instanceId, &mDefaultCrcConfig);
                     status = GENFSK_SetWhitenerConfig(instanceId, &mDefaultWhitenerConfig);
                }
                else
                {
                    if (bitProcConfig->crcConfig == NULL)
                    {
                        status = GENFSK_SetCrcConfig(instanceId, &mDefaultCrcConfig);
                    }
                    else
                    {
                        status = GENFSK_SetCrcConfig(instanceId, bitProcConfig->crcConfig);
                    }

                    if (status == gGenfskSuccess_c)
                    {
                        if (bitProcConfig->whitenerConfig == NULL)
                        {
                            status = GENFSK_SetWhitenerConfig(instanceId, &mDefaultWhitenerConfig);
                        }
                        else
                        {
                            status = GENFSK_SetWhitenerConfig(instanceId, bitProcConfig->whitenerConfig);
                        }
                    }
                }
            }

            if (status == gGenfskSuccess_c)
            {
                status = GENFSK_SetModeConfig(instanceId, &mDefaultModeConfig);
            }

            /* GENFSK instance initialization failed. */
            if (status != gGenfskSuccess_c)
            {
                genfskLocal[instanceId].genfskState = gGENFSK_LL_NoInit;
                instanceId = gGENFSK_InvalidIdx_c;
            }
            else
            {
                mNumberOfAllocatedInstances++;
                GENFSK_UnprotectFromXcvrInterrupt();
            }

            /* Exit critical section. */
            OSA_InterruptEnable();
        }
        else
        {
            status = gGenfskAllocInstanceFailed;
        }

        *pInstanceId = instanceId;
    }

    return status;
}

#ifdef RADIO_IS_GEN_3P5
genfskStatus_t GENFSK_RadioConfigWithRbme(uint8_t instanceId, const GENFSK_radio_config_t *radioConfig, const xcvr_coding_config_t **rbmeConf)
{
    genfskStatus_t status = gGenfskSuccess_c;
    genfskDataRate_t dataRate;

#if (gGenfskPreserveXcvrDacTrimValue_d == 1)
    xcvr_DcocDacTrim_t  mXcvrDacTrim;
    bool_t              trim_invalid;
#endif

    const xcvr_config_t *xcvrConfig = NULL;
    /* For now, until XCVR implementation is available, RBME is configured manually in GENFSK code */
    const xcvr_coding_config_t *rbmeConfig = *rbmeConf;

#if gGENFSK_CheckParams_c == 1
    if(radioConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if ((radioConfig->radioMode != gGenfskGfskBt0p5h0p5) &&
        (radioConfig->radioMode != gGenfskGfskBt0p5h0p32) &&
        (radioConfig->radioMode != gGenfskGfskBt0p5h0p7) &&
        (radioConfig->radioMode != gGenfskGfskBt0p5h1p0) &&
        (radioConfig->radioMode != gGenfskMsk))
    {
           status = gGenfskInvalidParameters_c;
    }
    else if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        dataRate = radioConfig->dataRate;

        status = GENFSK_GetXcvrConfig(radioConfig->radioMode, dataRate, &xcvrConfig);

        if(status == gGenfskSuccess_c)
        {
            FLib_MemCpy(&genfskLocal[instanceId].radioConfig, radioConfig, sizeof(genfskLocal[instanceId].radioConfig));

            if (mGenfskActiveInstance == instanceId)
            {
                /* This is the first initialization */
                if (mNumberOfAllocatedInstances == 0U)
                {
                    xcvrStatus_t xcvr_status;
#if (gGenfskPreserveXcvrDacTrimValue_d == 1)
                    /* Check if XCVR Trim value is valid */
                    trim_invalid = GENFSK_RestoreXcvrDcocDacTrimFromFlash(&mXcvrDacTrim);

                    /* Initialize Radio without trim */
                    xcvr_status = XCVR_InitNoDacTrim(&xcvrConfig, &rbmeConfig);
                    assert( xcvr_status == gXcvrSuccess_c );

                    if(trim_invalid == FALSE)
                    {
                        /* XCVR Trim value is not valid. Calculate now. */
                        uint32_t retry = 0U;
                        do
                        {
                            retry++;
                            xcvr_status = XCVR_CalculateDcocDacTrims(&mXcvrDacTrim);
                        } while ((gXcvrSuccess_c != xcvr_status) && (retry < gGenfskXcvrInitRetryCount_c));

                        if(xcvr_status == gXcvrSuccess_c)
                        {
                            /* store calculated XCVR Trim value */
                            (void)GENFSK_SaveXcvrDcocDacTrimToFlash(&mXcvrDacTrim);
                        }
                    }
                    else
                    {
                        /* XCVR Trim value is valid. Restore it. */
                        xcvr_status = XCVR_SetDcocDacTrims(&mXcvrDacTrim);
                    }
#else
                    xcvr_status = XCVR_Init(&xcvrConfig, &rbmeConfig);
#endif /* gGenfskPreserveXcvrDacTrimValue_d */
                    /* LCOV_EXCL_START */
                    if ( xcvr_status != gXcvrSuccess_c )
                    {
                        status = gGenfskFail_c;
                    }
                    else
                    /* LCOV_EXCL_STOP */
                    {
                        (void)GENFSK_SetXtalTrim(GENFSK_GetSavedXtalTrim());

                        /* Enable the CRC as it is disabled by default after reset */
                        (void)XCVR_SetActiveLL(XCVR_ACTIVE_LL_GENFSK);
                        /*Need to set active GENFSK LL before any GENFSK register access*/
                        (void)GENFSK_SelectRateConfig(instanceId, radioConfig->radioMode, dataRate);
                        RBME->CRCW_CFG |= RBME_CRCW_CFG_CRCW_EN(1);

                        /* Install interrupt. */
                        GENFSK_InstallInterrupt();
                        GENFSK_TimeInit();
#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
                        MWS_Register(gMWS_GENFSK_c, MWS_GENFSK_Callback);
#else /* (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c) */
                        /* If MWS is defined (ble_genfsk_adv app for instance) the gRfRadioVer_c will have been updated in ble_init.c.
                        If MWS is not defined (GENFSK app only), the version has to be retrieved.*/
                        gRfRadioVer_c = XCVR_ReadRadioVer();
#endif /* gGENFSK_NoMwsControl_c */
                    }
                }
                else
                {
                    GENFSK_ChangeMode(instanceId, radioConfig, xcvrConfig, rbmeConfig);
                }

                if (
#ifndef RADIO_IS_GEN_3P5
                 (radioConfig->radioMode == gGenfskFsk) ||
#endif
                  (radioConfig->radioMode == gGenfskMsk)
                 )
                {
#if 0 // OJE TBD
                    /* TODO, Clarify with XCVR team, what COEFF need to be used */
                    XCVR_TX_DIG->GFSK_COEFF1 = ((0U)<<0 | (511U)<<7 | (0U)<<16 | (511U)<<23);
                    XCVR_TX_DIG->GFSK_COEFF2 = 0x00000000U;
                    XCVR_TX_DIG->GFSK_CTRL |= XCVR_TX_DIG_GFSK_CTRL_GFSK_FLD_MASK;
#endif
                }
            }
        }
    }

    return status;
}
#endif /* RADIO_IS_GEN_3P5 */

genfskStatus_t GENFSK_RadioConfig(uint8_t instanceId, const GENFSK_radio_config_t *radioConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;
#if !defined (RADIO_IS_GEN_3P5)
    genfskDataRate_t dataRate;

    radio_mode_t radioMode = NUM_RADIO_MODES;

#if gGENFSK_CheckParams_c == 1
    if(radioConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        dataRate = radioConfig->dataRate;
        status = GENFSK_GetXcvrConfig(radioConfig->radioMode, &radioMode);

        if(status == gGenfskSuccess_c)
        {
            FLib_MemCpy(&genfskLocal[instanceId].radioConfig, radioConfig, sizeof(genfskLocal[instanceId].radioConfig));

            if (mGenfskActiveInstance == instanceId)
            {
                /* This is the first initialization */
                if (mNumberOfAllocatedInstances == 0U)
                {
                    if ( gXcvrSuccess_c != XCVR_Init(radioMode, (data_rate_t)dataRate))
                    {
                        status = gGenfskFail_c;
                    }
                    else
                    {
                        (void)GENFSK_SetXtalTrim(GENFSK_GetSavedXtalTrim());

                        /* Enable the CRC as it is disabled by default after reset */
                        XCVR_MISC->CRCW_CFG |= XCVR_CTRL_CRCW_CFG_CRCW_EN(1);

                        /* Install interrupt. */
                        GENFSK_InstallInterrupt();
                        GENFSK_TimeInit();
#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
                        MWS_Register(gMWS_GENFSK_c, MWS_GENFSK_Callback);
#endif
                    }
                }
                else
                {
                    (void)XCVR_ChangeMode(radioMode, (data_rate_t)dataRate);
                }

                if(status == gGenfskSuccess_c)
                {
                    GENFSK->BITRATE = dataRate;
                    genfskLocal[instanceId].genfskRegs.bitRate = dataRate;
#if 0               /* TODO, Clarify with XCVR team, what COEFF need to be used */
                    if ((radioConfig->radioMode == gGenfskFsk) || (radioConfig->radioMode == gGenfskMsk))
                    {
                        XCVR_TX_DIG->GFSK_COEFF1 = ((0U)<<0 | (511U)<<7 | (0U)<<16 | (511U)<<23);
                        XCVR_TX_DIG->GFSK_COEFF2 = 0x00000000U;
                        XCVR_TX_DIG->GFSK_CTRL |= XCVR_TX_DIG_GFSK_CTRL_GFSK_FLD_MASK;
                    }
#endif
                }
            }
        }
    }
#else
    const xcvr_coding_config_t *rbmeConfig = &xcvr_ble_uncoded_config;
    status = GENFSK_RadioConfigWithRbme(instanceId, radioConfig, &rbmeConfig);
#endif
    return status;
}

genfskStatus_t GENFSK_SetPacketConfig(uint8_t instanceId, const GENFSK_packet_config_t *packetConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (packetConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
#if !defined (RADIO_IS_GEN_3P5)
    else if (packetConfig->preambleSizeBytes > 7U)
#else
    else if (packetConfig->preambleSizeBytes > 511U)
#endif
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((packetConfig->lengthSizeBits > 16U) || (packetConfig->h0SizeBits > 16U) || (packetConfig->h1SizeBits > 16U))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (((packetConfig->lengthSizeBits + packetConfig->h0SizeBits + packetConfig->h1SizeBits) & 0x07U) != 0U)
    {
        status = gGenfskInvalidParameters_c;
    }
#if !defined (RADIO_IS_GEN_3P5)
    else if ((packetConfig->lengthAdjBytes < -31) || (packetConfig->lengthAdjBytes > 31))
#else
    else if ((packetConfig->lengthAdjBytes < -1024) || (packetConfig->lengthAdjBytes > 1023))
#endif
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
#if (gGENFSK_CheckParams_c == 1) || (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
    if (packetConfig->syncAddrSizeBytes > 3U)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        genfskLocal[instanceId].packetType = packetConfig->packetType;
        genfskLocal[instanceId].syncAddrSizeBytes = packetConfig->syncAddrSizeBytes + 1U;  /* Save the sync address bytes = size + 1 */
        genfskLocal[instanceId].headerSizeBytes = (packetConfig->lengthSizeBits + packetConfig->h0SizeBits + packetConfig->h1SizeBits+7U)>>3U;
        genfskLocal[instanceId].adjSizeBytes = packetConfig->lengthAdjBytes;

        /* Save the configuration in local structure. */
        genfskLocal[instanceId].genfskRegs.xcvrCfg &= ~(GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK);
#if defined (RADIO_IS_GEN_3P5)
        genfskLocal[instanceId].genfskRegs.xcvrCfg &= ~(GENFSK_XCVR_CFG_PREAMBLE_SEL_MASK | GENFSK_XCVR_CFG_GEN_PREAMBLE_MASK);  /* other modes than 0 and 1 not supported yet */
        if (packetConfig->preambleBytePattern != 0U)
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg |= GENFSK_XCVR_CFG_PREAMBLE_SEL(0x1U) | GENFSK_XCVR_CFG_GEN_PREAMBLE(packetConfig->preambleBytePattern);
        }
#endif
        genfskLocal[instanceId].genfskRegs.xcvrCfg |= GENFSK_XCVR_CFG_PREAMBLE_SZ(packetConfig->preambleSizeBytes);

        genfskLocal[instanceId].genfskRegs.packetCfg &= (uint32_t)~(uint32_t)(GENFSK_PACKET_CFG_LENGTH_SZ_MASK |
                                                                                  GENFSK_PACKET_CFG_LENGTH_BIT_ORD_MASK |
                                                                                      GENFSK_PACKET_CFG_SYNC_ADDR_SZ_MASK |
#if !defined (RADIO_IS_GEN_3P5)
                                                                                          GENFSK_PACKET_CFG_LENGTH_ADJ_MASK |
#else
                                                                                             GENFSK_PACKET_CFG_AA_PLAYBACK_CNT_MASK |
                                                                                             GENFSK_PACKET_CFG_LL_FETCH_AA_MASK |
#endif
                                                                                                GENFSK_PACKET_CFG_H0_SZ_MASK |
                                                                                                  GENFSK_PACKET_CFG_H1_SZ_MASK);
#if defined (RADIO_IS_GEN_3P5)
        genfskLocal[instanceId].genfskRegs.lengthAdj &= (uint32_t)~(uint32_t)(GENFSK_LENGTH_ADJ_LENGTH_ADJ_MASK);
#endif

        genfskLocal[instanceId].genfskRegs.h0Cfg &= (uint32_t)~(uint32_t)(GENFSK_H0_CFG_H0_MATCH_MASK |
                                                                              GENFSK_H0_CFG_H0_MASK_MASK);

        genfskLocal[instanceId].genfskRegs.h1Cfg &= (uint32_t)~(uint32_t)(GENFSK_H1_CFG_H1_MATCH_MASK |
                                                                             GENFSK_H1_CFG_H1_MASK_MASK);

#if !defined (RADIO_IS_GEN_3P5)
        if ((genfskLocal[instanceId].packetType == gGenfskFormattedPacket) && (genfskLocal[instanceId].radioConfig.radioMode != gGenfskMsk))
#else
        if (genfskLocal[instanceId].packetType == gGenfskFormattedPacket)
#endif
        {
            genfskLocal[instanceId].genfskRegs.packetCfg |= (uint32_t)(GENFSK_PACKET_CFG_LENGTH_SZ(packetConfig->lengthSizeBits) |
                                                                           GENFSK_PACKET_CFG_LENGTH_BIT_ORD(packetConfig->lengthBitOrder) |
                                                                               GENFSK_PACKET_CFG_SYNC_ADDR_SZ(packetConfig->syncAddrSizeBytes) |
#if !defined (RADIO_IS_GEN_3P5)
                                                                                   GENFSK_PACKET_CFG_LENGTH_ADJ(packetConfig->lengthAdjBytes) |
#else
                                                                                     GENFSK_PACKET_CFG_AA_PLAYBACK_CNT(0) | /* Default configuration is AA_PLAYBACK=0 + FETCH_AA=0 */
                                                                                     GENFSK_PACKET_CFG_LL_FETCH_AA(0) |
#endif
                                                                                       GENFSK_PACKET_CFG_H0_SZ(packetConfig->h0SizeBits) |
                                                                                           GENFSK_PACKET_CFG_H1_SZ(packetConfig->h1SizeBits));
#if defined (RADIO_IS_GEN_3P5)
            genfskLocal[instanceId].genfskRegs.lengthAdj |= (uint32_t)(GENFSK_LENGTH_ADJ_LENGTH_ADJ(packetConfig->lengthAdjBytes));
#endif
            genfskLocal[instanceId].genfskRegs.h0Cfg |= (uint32_t)(GENFSK_H0_CFG_H0_MATCH(packetConfig->h0Match) |
                                                                       GENFSK_H0_CFG_H0_MASK(packetConfig->h0Mask));



            genfskLocal[instanceId].genfskRegs.h1Cfg |= (uint32_t)(GENFSK_H1_CFG_H1_MATCH(packetConfig->h1Match) |
                                                                       GENFSK_H1_CFG_H1_MASK(packetConfig->h1Mask));
#if defined (RADIO_IS_GEN_3P5)
            if (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)
            {
                genfskLocal[instanceId].genfskRegs.xcvrCfg |= (GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK |
                                                               GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK);
                genfskLocal[instanceId].whitenEnable = gGenfskWhitenDisable;
            }
#endif
        }
        else
        {
            genfskLocal[instanceId].genfskRegs.packetCfg |= GENFSK_PACKET_CFG_SYNC_ADDR_SZ(packetConfig->syncAddrSizeBytes);
            genfskLocal[instanceId].genfskRegs.xcvrCfg |= (GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK |
                                                               GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK |
                                                                   GENFSK_XCVR_CFG_SW_CRC_EN_MASK);
            genfskLocal[instanceId].crcEnable = gGenfskCrcDisable;
            genfskLocal[instanceId].whitenEnable = gGenfskWhitenDisable;

        }

        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->XCVR_CFG &= ~GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK;
#if defined (RADIO_IS_GEN_3P5)
            GENFSK->XCVR_CFG &= ~(GENFSK_XCVR_CFG_PREAMBLE_SEL_MASK);  /* other modes not supported yet */
#endif
            GENFSK->XCVR_CFG |= (genfskLocal[instanceId].genfskRegs.xcvrCfg &

                                        (GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK |
#if defined (RADIO_IS_GEN_3P5)
                                          GENFSK_XCVR_CFG_PREAMBLE_SEL_MASK | GENFSK_XCVR_CFG_GEN_PREAMBLE_MASK |
#endif
                                             GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK |
                                                 GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK |
                                                     GENFSK_XCVR_CFG_SW_CRC_EN_MASK));
            GENFSK->PACKET_CFG = genfskLocal[instanceId].genfskRegs.packetCfg;
#if defined (RADIO_IS_GEN_3P5)
            GENFSK->LENGTH_ADJ = genfskLocal[instanceId].genfskRegs.lengthAdj;
#endif
            GENFSK->H0_CFG = genfskLocal[instanceId].genfskRegs.h0Cfg;
            GENFSK->H1_CFG = genfskLocal[instanceId].genfskRegs.h1Cfg;
        }
    }

    return status;
}

genfskStatus_t GENFSK_GetPacketConfig(uint8_t instanceId, GENFSK_packet_config_t *packetConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;
    uint32_t temp;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (packetConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else

    {
        packetConfig->preambleSizeBytes = (uint16_t) ((genfskLocal[instanceId].genfskRegs.xcvrCfg & GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK) >> GENFSK_XCVR_CFG_PREAMBLE_SZ_SHIFT);
        packetConfig->lengthSizeBits = (uint8_t) ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_SZ_MASK) >> GENFSK_PACKET_CFG_LENGTH_SZ_SHIFT);
        temp = ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_BIT_ORD_MASK) >> GENFSK_PACKET_CFG_LENGTH_BIT_ORD_SHIFT);
        packetConfig->lengthBitOrder = (genfskPacketCfgLengthBitOrd_t) temp;
        packetConfig->syncAddrSizeBytes = (uint8_t) ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_SYNC_ADDR_SZ_MASK) >> GENFSK_PACKET_CFG_SYNC_ADDR_SZ_SHIFT);
#if defined (RADIO_IS_GEN_3P5)
        temp = ((genfskLocal[instanceId].genfskRegs.lengthAdj & GENFSK_LENGTH_ADJ_LENGTH_ADJ_MASK) >> GENFSK_LENGTH_ADJ_LENGTH_ADJ_SHIFT);
#else
        temp = ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_ADJ_MASK) >> GENFSK_PACKET_CFG_LENGTH_ADJ_SHIFT);
#endif
        packetConfig->lengthAdjBytes = (int16_t) temp;
        packetConfig->h0SizeBits = (uint8_t) ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H0_SZ_MASK) >> GENFSK_PACKET_CFG_H0_SZ_SHIFT);
        packetConfig->h1SizeBits = (uint8_t) ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H1_SZ_MASK) >> GENFSK_PACKET_CFG_H1_SZ_SHIFT);
        packetConfig->h0Match = (uint16_t) ((genfskLocal[instanceId].genfskRegs.h0Cfg & GENFSK_H0_CFG_H0_MATCH_MASK) >> GENFSK_H0_CFG_H0_MATCH_SHIFT);
        packetConfig->h0Mask = (uint16_t) ((genfskLocal[instanceId].genfskRegs.h0Cfg & GENFSK_H0_CFG_H0_MASK_MASK) >> GENFSK_H0_CFG_H0_MASK_SHIFT);
        packetConfig->h1Match = (uint16_t) ((genfskLocal[instanceId].genfskRegs.h1Cfg & GENFSK_H1_CFG_H1_MATCH_MASK) >> GENFSK_H1_CFG_H1_MATCH_SHIFT);
        packetConfig->h1Mask = (uint16_t) ((genfskLocal[instanceId].genfskRegs.h1Cfg & GENFSK_H1_CFG_H1_MASK_MASK) >> GENFSK_H1_CFG_H1_MASK_SHIFT);
        packetConfig->packetType = genfskLocal[instanceId].packetType;
#if defined (RADIO_IS_GEN_3P5)
        packetConfig->preambleBytePattern = (uint8_t) ((genfskLocal[instanceId].genfskRegs.xcvrCfg & GENFSK_XCVR_CFG_GEN_PREAMBLE_MASK) >> GENFSK_XCVR_CFG_GEN_PREAMBLE_SHIFT);
#endif
    }

    return status;
}

genfskStatus_t GENFSK_SetModeConfig(uint8_t instanceId, const GENFSK_mode_config_t *modeConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if defined (RADIO_IS_GEN_3P5)
#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (modeConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (modeConfig->mode > gGenfskGtmMode)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
#if (gGENFSK_CheckParams_c == 1) || (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
    if (modeConfig->stayInRx > 1U)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {

        /* Save the configuration in local structure. */
        genfskLocal[instanceId].genfskRegs.enhFeature &= ~(GENFSK_ENH_FEATURE_GENLL_MODE_MASK |
                                                             GENFSK_ENH_FEATURE_STAY_IN_RX_MASK |
                                                               GENFSK_ENH_FEATURE_SW_BUILD_ACK_MASK |
                                                                 GENFSK_ENH_FEATURE_ACKBUF_SEL_MASK |
                                                                   GENFSK_ENH_FEATURE_AUTOACK_MASK |
                                                                     GENFSK_ENH_FEATURE_RXACKRQD_MASK); /* Any ordinary RX frame follows the Tx frame of TR sequence */

        genfskLocal[instanceId].genfskRegs.enhFeature |= (GENFSK_ENH_FEATURE_GENLL_MODE(modeConfig->mode) |
                                                             GENFSK_ENH_FEATURE_STAY_IN_RX(modeConfig->stayInRx) |
                                                               GENFSK_ENH_FEATURE_ACKBUF_SEL_MASK | /* Auto Ack frame is in Tx buffer RAM */
                                                                 GENFSK_ENH_FEATURE_SW_BUILD_ACK_MASK); /* SW builds ACK */

        genfskLocal[instanceId].genfskRegs.turnaroundTime = modeConfig->turnaroundTime;

        if (modeConfig->mode == gGenfskAntMode)
        {
            genfskLocal[instanceId].genfskRegs.packetCfg |= (uint32_t)GENFSK_PACKET_CFG_AA_PLAYBACK_CNT(1); /* ANT requires AA_PLAYBACK_CNT=1 */
            if (mGenfskActiveInstance == instanceId)
            {
                GENFSK->PACKET_CFG = genfskLocal[instanceId].genfskRegs.packetCfg;
            }
        }

        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->ENH_FEATURE = genfskLocal[instanceId].genfskRegs.enhFeature;
            GENFSK->TURNAROUND_TIME = genfskLocal[instanceId].genfskRegs.turnaroundTime;
        }
    }
#endif

    return status;
}

genfskStatus_t GENFSK_GetModeConfig(uint8_t instanceId, GENFSK_mode_config_t *modeConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if defined (RADIO_IS_GEN_3P5)
#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (modeConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else
    {
        uint32_t temp = (genfskLocal[instanceId].genfskRegs.enhFeature & GENFSK_ENH_FEATURE_GENLL_MODE_MASK) >> GENFSK_ENH_FEATURE_GENLL_MODE_SHIFT;
        modeConfig->mode = (genfskMode_t)(temp);
        modeConfig->stayInRx = (uint8_t) ((genfskLocal[instanceId].genfskRegs.enhFeature & GENFSK_ENH_FEATURE_STAY_IN_RX_MASK) >> GENFSK_ENH_FEATURE_STAY_IN_RX_SHIFT);
        modeConfig->turnaroundTime = genfskLocal[instanceId].genfskRegs.turnaroundTime;
    }
#endif

    return status;
}

genfskStatus_t GENFSK_SetCrcConfig(uint8_t instanceId, const GENFSK_crc_config_t *crcConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (crcConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (crcConfig->crcStartByte > 0xFU)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
#if (gGENFSK_CheckParams_c == 1) || (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
    if (crcConfig->crcSize > 4U)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        genfskLocal[instanceId].crcEnable = crcConfig->crcEnable;
        genfskLocal[instanceId].crcRecvInvalid = crcConfig->crcRecvInvalid;

#if !defined (RADIO_IS_GEN_3P5)
        genfskLocal[instanceId].genfskRegs.crcCfg &= (uint32_t)~(uint32_t)(GENFSK_CRC_CFG_CRC_SZ_MASK |
                                                                               GENFSK_CRC_CFG_CRC_START_BYTE_MASK |
                                                                                   GENFSK_CRC_CFG_CRC_REF_IN_MASK |
                                                                                       GENFSK_CRC_CFG_CRC_REF_OUT_MASK |
                                                                                           GENFSK_CRC_CFG_CRC_BYTE_ORD_MASK);

        genfskLocal[instanceId].genfskRegs.crcCfg |= (uint32_t)(GENFSK_CRC_CFG_CRC_SZ(crcConfig->crcSize) |
                                                                    GENFSK_CRC_CFG_CRC_START_BYTE(crcConfig->crcStartByte) |
                                                                        GENFSK_CRC_CFG_CRC_REF_IN(crcConfig->crcRefIn) |
                                                                            GENFSK_CRC_CFG_CRC_REF_OUT(crcConfig->crcRefOut) |
                                                                                GENFSK_CRC_CFG_CRC_BYTE_ORD(crcConfig->crcByteOrder));
#else
        genfskLocal[instanceId].genfskRegs.crcCfg &= (uint32_t)~(uint32_t)(RBME_CRCW_CFG3_CRC_SZ_MASK |
                                                                               RBME_CRCW_CFG3_CRC_START_BYTE_MASK |
                                                                                   RBME_CRCW_CFG3_CRC_REF_IN_MASK |
                                                                                       RBME_CRCW_CFG3_CRC_REF_OUT_MASK |
                                                                                           RBME_CRCW_CFG3_CRC_BYTE_ORD_MASK);

        genfskLocal[instanceId].genfskRegs.crcCfg |= (uint32_t)(RBME_CRCW_CFG3_CRC_SZ(crcConfig->crcSize) |
                                                                    RBME_CRCW_CFG3_CRC_START_BYTE(crcConfig->crcStartByte) |
                                                                        RBME_CRCW_CFG3_CRC_REF_IN(crcConfig->crcRefIn) |
                                                                            RBME_CRCW_CFG3_CRC_REF_OUT(crcConfig->crcRefOut) |
                                                                                RBME_CRCW_CFG3_CRC_BYTE_ORD(crcConfig->crcByteOrder));
#endif

        /* Left align CRC seed and Poly. */
        genfskLocal[instanceId].genfskRegs.crcInit = crcConfig->crcSeed << ((4U - crcConfig->crcSize) << 3U);
        genfskLocal[instanceId].genfskRegs.crcPoly = crcConfig->crcPoly << ((4U - crcConfig->crcSize) << 3U);
        genfskLocal[instanceId].genfskRegs.crcXorOut = crcConfig->crcXorOut  << ((4U - crcConfig->crcSize) << 3U);
#if !defined (RADIO_IS_GEN_3P5)
        if ((genfskLocal[instanceId].crcEnable == gGenfskCrcEnable) && (genfskLocal[instanceId].packetType == gGenfskFormattedPacket) &&
            (genfskLocal[instanceId].radioConfig.radioMode != gGenfskMsk))
#else
        if ((genfskLocal[instanceId].crcEnable == gGenfskCrcEnable) && (genfskLocal[instanceId].packetType == gGenfskFormattedPacket))
#endif
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg &= ~GENFSK_XCVR_CFG_SW_CRC_EN_MASK;
        }
        else
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg |= GENFSK_XCVR_CFG_SW_CRC_EN_MASK;
            genfskLocal[instanceId].crcEnable = gGenfskCrcDisable;
        }

        if (mGenfskActiveInstance == instanceId)
        {
#if defined (RADIO_IS_GEN_3P5)
            RBME->CRCW_CFG3  = genfskLocal[instanceId].genfskRegs.crcCfg;
            RBME->CRC_INIT = genfskLocal[instanceId].genfskRegs.crcInit;
            RBME->CRC_POLY = genfskLocal[instanceId].genfskRegs.crcPoly;
            RBME->CRC_XOR_OUT = genfskLocal[instanceId].genfskRegs.crcXorOut;
            GENFSK->CRC_CFG |= GENFSK_CRC_CFG_CRC_IGNORE_MASK;
#else
            GENFSK->CRC_CFG = genfskLocal[instanceId].genfskRegs.crcCfg;
            GENFSK->CRC_INIT = genfskLocal[instanceId].genfskRegs.crcInit;
            GENFSK->CRC_POLY = genfskLocal[instanceId].genfskRegs.crcPoly;
            GENFSK->CRC_XOR_OUT = genfskLocal[instanceId].genfskRegs.crcXorOut;
#endif
            GENFSK->XCVR_CFG &= ~GENFSK_XCVR_CFG_SW_CRC_EN_MASK;
            GENFSK->XCVR_CFG |= genfskLocal[instanceId].genfskRegs.xcvrCfg & GENFSK_XCVR_CFG_SW_CRC_EN_MASK;
        }
    }

    return status;
}

genfskStatus_t GENFSK_GetCrcConfig(uint8_t instanceId, GENFSK_crc_config_t *crcConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;
#if defined (RADIO_IS_GEN_3P5)
    uint32_t temp;
#endif
#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (crcConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else
    {
        crcConfig->crcEnable = genfskLocal[instanceId].crcEnable;
        crcConfig->crcRecvInvalid = genfskLocal[instanceId].crcRecvInvalid;
#if !defined (RADIO_IS_GEN_3P5)
        crcConfig->crcSize = (uint8_t) ((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_SZ_MASK) >> GENFSK_CRC_CFG_CRC_SZ_SHIFT);
        crcConfig->crcStartByte = (uint8_t) ((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_START_BYTE_MASK) >> GENFSK_CRC_CFG_CRC_START_BYTE_SHIFT);
        crcConfig->crcRefIn = (genfskCrcCfgCrcRefIn_t)((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_REF_IN_MASK) >> GENFSK_CRC_CFG_CRC_REF_IN_SHIFT);
        crcConfig->crcRefOut = (genfskCrcCfgCrcRefOut_t)((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_REF_OUT_MASK) >> GENFSK_CRC_CFG_CRC_REF_OUT_SHIFT);
        crcConfig->crcByteOrder = (genfskCrcCfgCrcByteOrd_t)((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_BYTE_ORD_MASK) >> GENFSK_CRC_CFG_CRC_BYTE_ORD_SHIFT);
#else
        crcConfig->crcSize = (uint8_t) ((genfskLocal[instanceId].genfskRegs.crcCfg & RBME_CRCW_CFG3_CRC_SZ_MASK) >> RBME_CRCW_CFG3_CRC_SZ_SHIFT);
        crcConfig->crcStartByte = (uint8_t) ((genfskLocal[instanceId].genfskRegs.crcCfg & RBME_CRCW_CFG3_CRC_START_BYTE_MASK) >> RBME_CRCW_CFG3_CRC_START_BYTE_SHIFT);
        temp = ((genfskLocal[instanceId].genfskRegs.crcCfg & RBME_CRCW_CFG3_CRC_REF_IN_MASK) >> RBME_CRCW_CFG3_CRC_REF_IN_SHIFT);
        crcConfig->crcRefIn = (genfskCrcCfgCrcRefIn_t) temp;
        temp = ((genfskLocal[instanceId].genfskRegs.crcCfg & RBME_CRCW_CFG3_CRC_REF_OUT_MASK) >> RBME_CRCW_CFG3_CRC_REF_OUT_SHIFT);
        crcConfig->crcRefOut = (genfskCrcCfgCrcRefOut_t) temp;
        temp = ((genfskLocal[instanceId].genfskRegs.crcCfg & RBME_CRCW_CFG3_CRC_BYTE_ORD_MASK) >> RBME_CRCW_CFG3_CRC_BYTE_ORD_SHIFT);
        crcConfig->crcByteOrder = (genfskCrcCfgCrcByteOrd_t) temp;
#endif
        crcConfig->crcSeed = genfskLocal[instanceId].genfskRegs.crcInit >> ((4U - crcConfig->crcSize) << 3U);
        crcConfig->crcPoly = genfskLocal[instanceId].genfskRegs.crcPoly >> ((4U - crcConfig->crcSize) << 3U);
        crcConfig->crcXorOut = genfskLocal[instanceId].genfskRegs.crcXorOut >> ((4U - crcConfig->crcSize) << 3U);
    }

    return status;
}

genfskStatus_t GENFSK_SetWhitenerConfig(uint8_t instanceId, const GENFSK_whitener_config_t *whitenerConfig)
{
    uint16_t tempValInit;
    uint16_t tempValPoly;
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (whitenerConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (whitenerConfig->whitenSize > 9U)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((whitenerConfig->whitenInit > 0x1FFU) || (whitenerConfig->whitenPoly > 0x1FFU))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (whitenerConfig->whitenSizeThr > 0xFFFU)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
#if (gGENFSK_CheckParams_c == 1) || (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
    if ((whitenerConfig->whitenEnable > gGenfskWhitenDisable) &&
       (whitenerConfig->manchesterEn == gGenfskManchesterEnable))
    {
        /* Whitening and Manchester are mutual exclusive. */
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        genfskLocal[instanceId].whitenEnable = whitenerConfig->whitenEnable;

        /* Reorder the bits in POLY and SEED for whitening. */
        tempValInit = GENFSK_Reverse9Bit(whitenerConfig->whitenInit & 0x1FFU);
        tempValInit >>= 9U - whitenerConfig->whitenSize;
        tempValPoly = whitenerConfig->whitenPoly;
        tempValPoly &= 0xFFFEU;
        tempValPoly = GENFSK_Reverse9Bit(tempValPoly & 0x1FFU);
        tempValPoly >>= 9U - whitenerConfig->whitenSize;

#if !defined (RADIO_IS_GEN_3P5)
        genfskLocal[instanceId].genfskRegs.whitenCfg &= ~(GENFSK_WHITEN_CFG_WHITEN_START_MASK |
                                                              GENFSK_WHITEN_CFG_WHITEN_END_MASK |
                                                                  GENFSK_WHITEN_CFG_WHITEN_B4_CRC_MASK |
                                                                      GENFSK_WHITEN_CFG_WHITEN_POLY_TYPE_MASK |
                                                                          GENFSK_WHITEN_CFG_WHITEN_REF_IN_MASK |
                                                                              GENFSK_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_MASK |
                                                                                  GENFSK_WHITEN_CFG_WHITEN_SIZE_MASK |
                                                                                      GENFSK_WHITEN_CFG_WHITEN_INIT_MASK);

        genfskLocal[instanceId].genfskRegs.whitenCfg |= (GENFSK_WHITEN_CFG_WHITEN_START(whitenerConfig->whitenStart) |
                                                             GENFSK_WHITEN_CFG_WHITEN_END(whitenerConfig->whitenEnd) |
                                                                 GENFSK_WHITEN_CFG_WHITEN_B4_CRC(whitenerConfig->whitenB4Crc) |
                                                                     GENFSK_WHITEN_CFG_WHITEN_POLY_TYPE(whitenerConfig->whitenPolyType) |
                                                                         GENFSK_WHITEN_CFG_WHITEN_REF_IN(whitenerConfig->whitenRefIn) |
                                                                             GENFSK_WHITEN_CFG_WHITEN_PAYLOAD_REINIT(whitenerConfig->whitenPayloadReinit) |
                                                                                 GENFSK_WHITEN_CFG_WHITEN_SIZE(whitenerConfig->whitenSize) |
                                                                                     GENFSK_WHITEN_CFG_WHITEN_INIT(tempValInit));

        genfskLocal[instanceId].genfskRegs.whitenPoly = GENFSK_WHITEN_POLY_WHITEN_POLY(tempValPoly);
        genfskLocal[instanceId].genfskRegs.whitenSzThr &= ~GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
        genfskLocal[instanceId].genfskRegs.whitenSzThr |= GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR(whitenerConfig->whitenSizeThr);

        genfskLocal[instanceId].genfskRegs.whitenCfg &= ~(GENFSK_WHITEN_CFG_MANCHESTER_EN_MASK |
                                                              GENFSK_WHITEN_CFG_MANCHESTER_INV_MASK |
                                                                  GENFSK_WHITEN_CFG_MANCHESTER_START_MASK);
#else
        genfskLocal[instanceId].genfskRegs.whitenCfg &= ~(RBME_WHITEN_CFG_WHITEN_START_MASK |
                                                              RBME_WHITEN_CFG_WHITEN_END_MASK |
                                                                  RBME_WHITEN_CFG_WHITEN_B4_CRC_MASK |
                                                                      RBME_WHITEN_CFG_WHITEN_POLY_TYPE_MASK |
                                                                          RBME_WHITEN_CFG_WHITEN_REF_IN_MASK |
                                                                              RBME_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_MASK |
                                                                                  RBME_WHITEN_CFG_WHITEN_SIZE_MASK |
                                                                                      RBME_WHITEN_CFG_WHITEN_INIT_MASK);

        genfskLocal[instanceId].genfskRegs.whitenCfg |= (RBME_WHITEN_CFG_WHITEN_START(whitenerConfig->whitenStart) |
                                                             RBME_WHITEN_CFG_WHITEN_END(whitenerConfig->whitenEnd) |
                                                                 RBME_WHITEN_CFG_WHITEN_B4_CRC(whitenerConfig->whitenB4Crc) |
                                                                     RBME_WHITEN_CFG_WHITEN_POLY_TYPE(whitenerConfig->whitenPolyType) |
                                                                         RBME_WHITEN_CFG_WHITEN_REF_IN(whitenerConfig->whitenRefIn) |
                                                                             RBME_WHITEN_CFG_WHITEN_PAYLOAD_REINIT(whitenerConfig->whitenPayloadReinit) |
                                                                                 RBME_WHITEN_CFG_WHITEN_SIZE(whitenerConfig->whitenSize) |
                                                                                     RBME_WHITEN_CFG_WHITEN_INIT(tempValInit));

        genfskLocal[instanceId].genfskRegs.whitenPoly = RBME_WHITEN_POLY_WHITEN_POLY(tempValPoly);
        genfskLocal[instanceId].genfskRegs.whitenSzThr &= ~RBME_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
        genfskLocal[instanceId].genfskRegs.whitenSzThr |= RBME_WHITEN_SZ_THR_WHITEN_SZ_THR(whitenerConfig->whitenSizeThr);
#if 0 // OJE TBD
        genfskLocal[instanceId].genfskRegs.whitenCfg &= ~(GENFSK_WHITEN_CFG_MANCHESTER_EN_MASK |
                                                              GENFSK_WHITEN_CFG_MANCHESTER_INV_MASK |
                                                                  GENFSK_WHITEN_CFG_MANCHESTER_START_MASK);
#endif
#endif
        if ((genfskLocal[instanceId].whitenEnable == gGenfskWhitenEnable) && (genfskLocal[instanceId].packetType == gGenfskFormattedPacket) &&
            (genfskLocal[instanceId].radioConfig.radioMode != gGenfskMsk))
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg &= ~(GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK);
#if 0 // OJE TBD
            genfskLocal[instanceId].genfskRegs.whitenCfg |= (GENFSK_WHITEN_CFG_MANCHESTER_EN(whitenerConfig->manchesterEn) |
                                                                 GENFSK_WHITEN_CFG_MANCHESTER_INV(whitenerConfig->manchesterInv) |
                                                                     GENFSK_WHITEN_CFG_MANCHESTER_START(whitenerConfig->manchesterStart));
#endif
        }
        else
        {
            genfskLocal[instanceId].genfskRegs.xcvrCfg |= GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK;
#if 0 // OJE TBD
            genfskLocal[instanceId].genfskRegs.whitenCfg |= (GENFSK_WHITEN_CFG_MANCHESTER_EN(0) |
                                                                 GENFSK_WHITEN_CFG_MANCHESTER_INV(0) |
                                                                     GENFSK_WHITEN_CFG_MANCHESTER_START(0));
#endif
            genfskLocal[instanceId].whitenEnable = gGenfskWhitenDisable;
        }

        if (mGenfskActiveInstance == instanceId)
        {
#if !defined (RADIO_IS_GEN_3P5)
            GENFSK->WHITEN_CFG = genfskLocal[instanceId].genfskRegs.whitenCfg;
            GENFSK->WHITEN_POLY = genfskLocal[instanceId].genfskRegs.whitenPoly;
            GENFSK->WHITEN_SZ_THR &= ~GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
            GENFSK->WHITEN_SZ_THR |= genfskLocal[instanceId].genfskRegs.whitenSzThr & GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
#else
            RBME->WHITEN_CFG = genfskLocal[instanceId].genfskRegs.whitenCfg;
            RBME->WHITEN_POLY = genfskLocal[instanceId].genfskRegs.whitenPoly;
            RBME->WHITEN_SZ_THR &= ~RBME_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
            RBME->WHITEN_SZ_THR |= genfskLocal[instanceId].genfskRegs.whitenSzThr & RBME_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK;
#endif
            GENFSK->XCVR_CFG &= ~(GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK);
            GENFSK->XCVR_CFG |= genfskLocal[instanceId].genfskRegs.xcvrCfg & (GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK);
        }
    }

    return status;
}

genfskStatus_t GENFSK_GetWhitenerConfig(uint8_t instanceId, GENFSK_whitener_config_t *whitenerConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;
#if defined (RADIO_IS_GEN_3P5)
    uint32_t temp;
#endif

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (whitenerConfig == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else
    {
        whitenerConfig->whitenEnable = genfskLocal[instanceId].whitenEnable;
#if !defined (RADIO_IS_GEN_3P5)
        whitenerConfig->whitenStart = (genfskWhitenStart_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_START_MASK) >> GENFSK_WHITEN_CFG_WHITEN_START_SHIFT);
        whitenerConfig->whitenEnd = (genfskWhitenEnd_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_END_MASK) >> GENFSK_WHITEN_CFG_WHITEN_END_SHIFT);
        whitenerConfig->whitenB4Crc = (genfskWhitenB4Crc_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_B4_CRC_MASK) >> GENFSK_WHITEN_CFG_WHITEN_B4_CRC_SHIFT);
        whitenerConfig->whitenPolyType = (genfskWhitenPolyType_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_POLY_TYPE_MASK) >> GENFSK_WHITEN_CFG_WHITEN_POLY_TYPE_SHIFT);
        whitenerConfig->whitenRefIn = (genfskWhitenCfgRefIn_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_REF_IN_MASK) >> GENFSK_WHITEN_CFG_WHITEN_REF_IN_SHIFT);
        whitenerConfig->whitenPayloadReinit = (genfskWhitenCfgPayloadReinit_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_MASK) >> GENFSK_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_SHIFT);
        whitenerConfig->whitenSize = (uint8_t) ((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_SIZE_MASK) >> GENFSK_WHITEN_CFG_WHITEN_SIZE_SHIFT);
        whitenerConfig->whitenInit = ((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_WHITEN_INIT_MASK) >> GENFSK_WHITEN_CFG_WHITEN_INIT_SHIFT);
        whitenerConfig->manchesterEn = (genfskManchesterEn_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_EN_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_EN_SHIFT);
        whitenerConfig->manchesterInv = (genfskManchesterInv_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_INV_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_INV_SHIFT);
        whitenerConfig->manchesterStart = (genfskManchesterStart_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_START_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_START_SHIFT);
        whitenerConfig->whitenPoly = (uint16_t) ((genfskLocal[instanceId].genfskRegs.whitenPoly & GENFSK_WHITEN_POLY_WHITEN_POLY_MASK) >> GENFSK_WHITEN_POLY_WHITEN_POLY_SHIFT);
        whitenerConfig->whitenSizeThr = (uint16_t) ((genfskLocal[instanceId].genfskRegs.whitenSzThr & GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK) >> GENFSK_WHITEN_SZ_THR_WHITEN_SZ_THR_SHIFT);
#else
        temp = ((genfskLocal[instanceId].genfskRegs.whitenCfg & RBME_WHITEN_CFG_WHITEN_START_MASK) >> RBME_WHITEN_CFG_WHITEN_START_SHIFT);
        whitenerConfig->whitenStart = (genfskWhitenStart_t) temp;
        temp = ((genfskLocal[instanceId].genfskRegs.whitenCfg & RBME_WHITEN_CFG_WHITEN_END_MASK) >> RBME_WHITEN_CFG_WHITEN_END_SHIFT);
        whitenerConfig->whitenEnd = (genfskWhitenEnd_t) temp;
        temp = ((genfskLocal[instanceId].genfskRegs.whitenCfg & RBME_WHITEN_CFG_WHITEN_B4_CRC_MASK) >> RBME_WHITEN_CFG_WHITEN_B4_CRC_SHIFT);
        whitenerConfig->whitenB4Crc = (genfskWhitenB4Crc_t) temp;
        temp = ((genfskLocal[instanceId].genfskRegs.whitenCfg & RBME_WHITEN_CFG_WHITEN_POLY_TYPE_MASK) >> RBME_WHITEN_CFG_WHITEN_POLY_TYPE_SHIFT);
        whitenerConfig->whitenPolyType = (genfskWhitenPolyType_t) temp;
        temp = ((genfskLocal[instanceId].genfskRegs.whitenCfg & RBME_WHITEN_CFG_WHITEN_REF_IN_MASK) >> RBME_WHITEN_CFG_WHITEN_REF_IN_SHIFT);
        whitenerConfig->whitenRefIn = (genfskWhitenCfgRefIn_t) temp;
        temp = ((genfskLocal[instanceId].genfskRegs.whitenCfg & RBME_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_MASK) >> RBME_WHITEN_CFG_WHITEN_PAYLOAD_REINIT_SHIFT);
        whitenerConfig->whitenPayloadReinit = (genfskWhitenCfgPayloadReinit_t) temp;
        whitenerConfig->whitenSize = (uint8_t) ((genfskLocal[instanceId].genfskRegs.whitenCfg & RBME_WHITEN_CFG_WHITEN_SIZE_MASK) >> RBME_WHITEN_CFG_WHITEN_SIZE_SHIFT);
        whitenerConfig->whitenInit = (uint16_t) ((genfskLocal[instanceId].genfskRegs.whitenCfg & RBME_WHITEN_CFG_WHITEN_INIT_MASK) >> RBME_WHITEN_CFG_WHITEN_INIT_SHIFT);
        whitenerConfig->whitenPoly = (uint16_t) ((genfskLocal[instanceId].genfskRegs.whitenPoly & RBME_WHITEN_POLY_WHITEN_POLY_MASK) >> RBME_WHITEN_POLY_WHITEN_POLY_SHIFT);
        whitenerConfig->whitenSizeThr = (uint16_t) ((genfskLocal[instanceId].genfskRegs.whitenSzThr & RBME_WHITEN_SZ_THR_WHITEN_SZ_THR_MASK) >> RBME_WHITEN_SZ_THR_WHITEN_SZ_THR_SHIFT);
#if 0 // OJE TBD
        whitenerConfig->manchesterEn = (genfskManchesterEn_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_EN_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_EN_SHIFT);
        whitenerConfig->manchesterInv = (genfskManchesterInv_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_INV_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_INV_SHIFT);
        whitenerConfig->manchesterStart = (genfskManchesterStart_t)((genfskLocal[instanceId].genfskRegs.whitenCfg & GENFSK_WHITEN_CFG_MANCHESTER_START_MASK) >> GENFSK_WHITEN_CFG_MANCHESTER_START_SHIFT);
#else
        whitenerConfig->manchesterEn = (genfskManchesterEn_t)0;
        whitenerConfig->manchesterInv = (genfskManchesterInv_t)0;
        whitenerConfig->manchesterStart = (genfskManchesterStart_t)0;
#endif
#endif
        /* Reorder the bits in POLY and SEED for whitening. */
        whitenerConfig->whitenInit = GENFSK_Reverse9Bit(whitenerConfig->whitenInit & 0x1FFU);
        whitenerConfig->whitenInit >>= 9U - whitenerConfig->whitenSize;
        whitenerConfig->whitenPoly = GENFSK_Reverse9Bit(whitenerConfig->whitenPoly & 0x1FFU);
        whitenerConfig->whitenPoly >>= 9U - whitenerConfig->whitenSize;
        whitenerConfig->whitenPoly |= (0x01U);
    }

    return status;
}

genfskStatus_t GENFSK_FreeInstance(uint8_t instanceId)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();

        GENFSK_ResetToDefaults(instanceId);
        genfskLocal[instanceId].genfskState = gGENFSK_LL_NoInit;
        mNumberOfAllocatedInstances--;

        if (mNumberOfAllocatedInstances == 0U)
        {
            mGenfskActiveInstance = gGENFSK_InvalidIdx_c;
            GENFSK_ProtectFromXcvrInterrupt();
        }
        else
        {
            if (instanceId == mGenfskActiveInstance)
            {
                /* If active instance is current instance then find first free "idle" instance to switch to. */
/* LCOV_EXCL_START */
                /* Exclude the for loop from coverage since it's impossible to have i equals to gGENFSK_InstancesCnt_c.
                 * All of non-active allocated instance is surely in gGENFSK_LL_Idle state */
                for (uint8_t i = 0; i < gGENFSK_InstancesCnt_c; i++)
/* LCOV_EXCL_STOP */
                {
                    if (genfskLocal[i].genfskState == gGENFSK_LL_Idle)
                    {
                        /* Change the active instance */
                        GENFSK_SwitchToInstance(i);
                        break;
                    }
                }
            }
        }

        /* Exit critical section. */
        OSA_InterruptEnable();
    }

    return status;
}

void GENFSK_ResetToDefaults(uint8_t instanceId)
{
    if (instanceId < gGENFSK_InstancesCnt_c)
    {
        if (genfskLocal[instanceId].genfskState != gGENFSK_LL_NoInit)
        {
            /* Enter critical section. */
            OSA_InterruptDisable();

            if (mGenfskActiveInstance == instanceId)
            {
                GENFSK_AbortAll();
            }

            (void)GENFSK_SetModeConfig(instanceId, &mDefaultModeConfig);
            (void)GENFSK_SetPacketConfig(instanceId, &mDefaultPacketConfig);
            (void)GENFSK_SetCrcConfig(instanceId, &mDefaultCrcConfig);
            (void)GENFSK_SetWhitenerConfig(instanceId, &mDefaultWhitenerConfig);

            for (uint8_t i = 0; i < 4U; i++)
            {
                (void)GENFSK_SetNetworkAddress(instanceId, i, &mDefaultNwkAddrConfig);
                (void)GENFSK_DisableNetworkAddress(instanceId, i);
            }

            genfskLocal[instanceId].genfskState = gGENFSK_LL_Idle;

            /* Exit critical section. */
            OSA_InterruptEnable();
        }
    }
}

genfskStatus_t GENFSK_SetNetworkAddress(uint8_t instanceId, uint8_t location, const GENFSK_nwk_addr_match_t *nwkAddressSettings)
{

    GENFSK_nwk_addr_t tempNwkAddress = { 0 };
#if !defined (RADIO_IS_GEN_3P5)
    uint8_t *pTempNwkAddress = NULL;
#endif
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((location > 3U) || (nwkAddressSettings == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((nwkAddressSettings->nwkAddrSizeBytes > 3U) || (nwkAddressSettings->nwkAddrThrBits > 7U ))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        tempNwkAddress = nwkAddressSettings->nwkAddr;

#if !defined (RADIO_IS_GEN_3P5)
        /* Twiddling is done by the HW */
        if (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)
        {
            /* Twiddle network address bytes for MSK. */
            pTempNwkAddress = (uint8_t *)&tempNwkAddress;

            GENFSK_MskPreProcessing(pTempNwkAddress,
                                    pTempNwkAddress,
                                    nwkAddressSettings->nwkAddrSizeBytes + 1U,
                                    ((~pTempNwkAddress[0]) & 0x01U));
        }
#endif
#if defined (RADIO_IS_GEN_3P5)
        genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR_SZ_MASK | GENFSK_NTW_ADR_CTRL_NTW_ADR_THR_MASK);
        genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |=  (GENFSK_NTW_ADR_CTRL_NTW_ADR_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                               GENFSK_NTW_ADR_CTRL_NTW_ADR_THR(nwkAddressSettings->nwkAddrThrBits));
        if (location == 0U)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr0 = tempNwkAddress;
        }
        else if (location == 1U)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr1 = tempNwkAddress;
        }
        else if (location == 2U)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr2 = tempNwkAddress;
        }
        else
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr3 = tempNwkAddress;
        }
#else
        if (location == 0U)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr0 = tempNwkAddress;

            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR0_SZ_MASK |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR0_MASK);

            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (GENFSK_NTW_ADR_CTRL_NTW_ADR0_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR0(nwkAddressSettings->nwkAddrThrBits));
        }
        else if (location == 1U)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr1 = tempNwkAddress;

            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR1_SZ_MASK |
                                                                    GENFSK_NTW_ADR_CTRL_NTW_ADR_THR1_MASK);

            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (GENFSK_NTW_ADR_CTRL_NTW_ADR1_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR1(nwkAddressSettings->nwkAddrThrBits));
        }
        else if (location == 2U)
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr2 = tempNwkAddress;

            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR2_SZ_MASK |
                                                                    GENFSK_NTW_ADR_CTRL_NTW_ADR_THR2_MASK);

            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (GENFSK_NTW_ADR_CTRL_NTW_ADR2_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR2(nwkAddressSettings->nwkAddrThrBits));
        }
        else
        {
            genfskLocal[instanceId].genfskRegs.ntwAdr3 = tempNwkAddress;

            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(GENFSK_NTW_ADR_CTRL_NTW_ADR3_SZ_MASK |
                                                                    GENFSK_NTW_ADR_CTRL_NTW_ADR_THR3_MASK);

            genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (GENFSK_NTW_ADR_CTRL_NTW_ADR3_SZ(nwkAddressSettings->nwkAddrSizeBytes) |
                                                                   GENFSK_NTW_ADR_CTRL_NTW_ADR_THR3(nwkAddressSettings->nwkAddrThrBits));
        }
#endif

        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->NTW_ADR_CTRL = genfskLocal[instanceId].genfskRegs.ntwAdrCtrl;
            GENFSK->NTW_ADR_0 = genfskLocal[instanceId].genfskRegs.ntwAdr0;
            GENFSK->NTW_ADR_1 = genfskLocal[instanceId].genfskRegs.ntwAdr1;
            GENFSK->NTW_ADR_2 = genfskLocal[instanceId].genfskRegs.ntwAdr2;
            GENFSK->NTW_ADR_3 = genfskLocal[instanceId].genfskRegs.ntwAdr3;
        }
    }

    return status;
}

genfskStatus_t GENFSK_GetNetworkAddress(uint8_t instanceId, uint8_t location, GENFSK_nwk_addr_match_t *nwkAddressSettings)
{
#if !defined (RADIO_IS_GEN_3P5)
    GENFSK_nwk_addr_t tempNwkAddress = { 0 };
    uint8_t *pTempNwkAddress = NULL;
#endif
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((location > 3U) || (nwkAddressSettings == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
    {
        status = gGenfskNotInitialized_c;
    }
    else
    {
#if defined (RADIO_IS_GEN_3P5)
        nwkAddressSettings->nwkAddrSizeBytes = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_SZ_SHIFT);
        nwkAddressSettings->nwkAddrThrBits = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR_SHIFT);
        if (location == 0U)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr0;
        }
        else if (location == 1U)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr1;
        }
        else if (location == 2U)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr2;
        }
        else
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr3;
        }
#else
        if (location == 0U)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr0;
            nwkAddressSettings->nwkAddrSizeBytes = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR0_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR0_SZ_SHIFT);
            nwkAddressSettings->nwkAddrThrBits = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR0_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR0_SHIFT);
        }
        else if (location == 1U)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr1;
            nwkAddressSettings->nwkAddrSizeBytes = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR1_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR1_SZ_SHIFT);
            nwkAddressSettings->nwkAddrThrBits = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR1_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR1_SHIFT);
        }
        else if (location == 2U)
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr2;
            nwkAddressSettings->nwkAddrSizeBytes = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR2_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR2_SZ_SHIFT);
            nwkAddressSettings->nwkAddrThrBits = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR2_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR2_SHIFT);
        }
        else
        {
            nwkAddressSettings->nwkAddr = genfskLocal[instanceId].genfskRegs.ntwAdr3;
            nwkAddressSettings->nwkAddrSizeBytes = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR3_SZ_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR3_SZ_SHIFT);
            nwkAddressSettings->nwkAddrThrBits = (uint8_t) ((genfskLocal[instanceId].genfskRegs.ntwAdrCtrl & GENFSK_NTW_ADR_CTRL_NTW_ADR_THR3_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_THR3_SHIFT);
        }
#endif

#if !defined (RADIO_IS_GEN_3P5)
        /* Twiddling is done by the HW */
        if (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)
        {
            tempNwkAddress = nwkAddressSettings->nwkAddr;

            /* Twiddle network address bytes for MSK. */
            pTempNwkAddress = (uint8_t *)&tempNwkAddress;

            GENFSK_MskPostProcessing(pTempNwkAddress,
                                     pTempNwkAddress,
                                     ((uint16_t) nwkAddressSettings->nwkAddrSizeBytes) + 1U,
                                     ((~pTempNwkAddress[0]) & 0x01U),
                                     1U);

            nwkAddressSettings->nwkAddr = tempNwkAddress;
        }
#endif
    }

    return status;
}

genfskStatus_t GENFSK_EnableNetworkAddress(uint8_t instanceId, uint8_t location)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (location > 3U)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        genfskLocal[instanceId].genfskRegs.ntwAdrCtrl |= (uint32_t)((1UL << location) << GENFSK_NTW_ADR_CTRL_NTW_ADR_EN_SHIFT);

        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->NTW_ADR_CTRL = genfskLocal[instanceId].genfskRegs.ntwAdrCtrl;
        }
    }

    return status;
}

genfskStatus_t GENFSK_DisableNetworkAddress(uint8_t instanceId, uint8_t location)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (location > 3U)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        genfskLocal[instanceId].genfskRegs.ntwAdrCtrl &= ~(uint32_t)((1UL << location) << GENFSK_NTW_ADR_CTRL_NTW_ADR_EN_SHIFT);

        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->NTW_ADR_CTRL = genfskLocal[instanceId].genfskRegs.ntwAdrCtrl;
        }
    }

    return status;
}

genfskStatus_t GENFSK_SetChannelNumber(uint8_t instanceId, uint8_t channelNum)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (channelNum > 127U)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        genfskLocal[instanceId].genfskRegs.channelNum = channelNum;

        if (mGenfskActiveInstance == instanceId)
        {
#if defined (RADIO_IS_GEN_3P5)
            GENFSK->CHANNEL_NUM0 = genfskLocal[instanceId].genfskRegs.channelNum;
#else
            GENFSK->CHANNEL_NUM = genfskLocal[instanceId].genfskRegs.channelNum;
#endif
        }
    }

    return status;
}

uint8_t GENFSK_GetChannelNumber(uint8_t instanceId)
{
    uint8_t chanNum = 0xFF;

    if (instanceId < gGENFSK_InstancesCnt_c)
    {
        if (genfskLocal[instanceId].genfskState != gGENFSK_LL_NoInit)
        {
            chanNum = (uint8_t)genfskLocal[instanceId].genfskRegs.channelNum;
        }
    }

    return chanNum;
}

genfskStatus_t GENFSK_SetTxPowerLevel(uint8_t instanceId, uint8_t txPowerLevel)
{
    uint8_t tempTxPower = 0;
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (txPowerLevel > 32U)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_Idle)
    {
        status = GENFSK_GetBusyStatus(genfskLocal[instanceId].genfskState);
    }
    else
    {
        /* txPowerLevel is converted to PA_POWER:
           0 --> 1
           1 --> 2
           2 --> 4
           3 --> 6
           ...
           31 --> 62
           32 --> 62 (for compatibility with previous version of the API)
        */
        if (txPowerLevel == 0U)
        {
            tempTxPower = 1U;
        }
        else
        {
            if (txPowerLevel == 32U)
            {
                txPowerLevel--;
            }
            tempTxPower = txPowerLevel * 2U;
        }

        genfskLocal[instanceId].genfskRegs.txPower = tempTxPower;

        if (mGenfskActiveInstance == instanceId)
        {
            GENFSK->TX_POWER = genfskLocal[instanceId].genfskRegs.txPower;
        }
    }

    return status;
}

uint8_t GENFSK_GetTxPowerLevel(uint8_t instanceId)
{
    uint8_t tempTxPower = 0xFF;

    if (instanceId < gGENFSK_InstancesCnt_c)
    {
        if (genfskLocal[instanceId].genfskState != gGENFSK_LL_NoInit)
        {
            tempTxPower = (uint8_t)genfskLocal[instanceId].genfskRegs.txPower;

            tempTxPower = tempTxPower >> 1;
        }
    }

    return tempTxPower;
}

#if gGENFSK_CheckParams_c == 1
/* Check that parameters provide to GENFSK_StartTx() are correct */
static genfskStatus_t GENFSK_StartTxCheckParams(uint8_t instanceId, const uint8_t *pBuffer, uint16_t bufLengthBytes, bool followedByRx, GENFSK_timestamp_t rxDuration)
{
    genfskStatus_t status;
    uint16_t maxPayloadSize;

    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (pBuffer == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
#if !defined (RADIO_IS_GEN_3P5)
    else if (followedByRx)
    {
        status = gGenfskInvalidParameters_c;
    }
#else
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
    else if ((followedByRx) && (rxDuration == 0ULL))
    {
        status = gGenfskInvalidParameters_c;
    }
#endif
#endif
    else
    {
        /* Check that requested Tx payload size does not exceed maximum supported one */
        if ((genfskLocal[instanceId].packetType == gGenfskRawPacket) || (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk))
        {
            maxPayloadSize = (uint16_t)gGENFSK_MaxRawPacketLength_c + (uint16_t)genfskLocal[instanceId].syncAddrSizeBytes;
        }
        else
        {
            int16_t temp = (int16_t)gGENFSK_MaxPacketPayloadAndAdjLength_c - genfskLocal[instanceId].adjSizeBytes;
            maxPayloadSize = (uint16_t)temp;
            maxPayloadSize += (uint16_t)genfskLocal[instanceId].syncAddrSizeBytes + (uint16_t)genfskLocal[instanceId].headerSizeBytes;
        }

        if (bufLengthBytes > maxPayloadSize)
        {
            status = gGenfskInvalidParameters_c;
        }
        else
        {
            status = gGenfskSuccess_c;
        }
    }

    return status;
}
#endif

genfskStatus_t GENFSK_StartTx(uint8_t instanceId, const uint8_t *pBuffer, uint16_t bufLengthBytes, GENFSK_timestamp_t txStartTime)
{
    /* Call generic Tx function. */
    return GENFSK_StartTxRx(instanceId, pBuffer, bufLengthBytes, txStartTime, FALSE, 0, NULL, 0);
}
                          
genfskStatus_t GENFSK_StartTxRx(uint8_t instanceId, const uint8_t *pBuffer, uint16_t bufLengthBytes, GENFSK_timestamp_t txStartTime, bool followedByRx, GENFSK_timestamp_t rxDuration, uint8_t *pRxBuffer, uint16_t maxBufLengthBytes )
{
#if !defined (RADIO_IS_GEN_3P5)
    uint8_t codedBytes[gGENFSK_MaxSyncAddressSize_c + gGENFSK_MaxRawPacketLength_c];
#endif
    GENFSK_timestamp_t currentTime = 0U;
    GENFSK_timestamp_t tempTime = 0U;
#if defined (RADIO_IS_GEN_3P5)
    GENFSK_TimeEvent_t event;
#endif
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    status = GENFSK_StartTxCheckParams(instanceId, pBuffer, bufLengthBytes, followedByRx, rxDuration);

    if (status == gGenfskSuccess_c)
#endif
    {
        if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
        {
            status = gGenfskNotInitialized_c;
        }
    }

    if (status == gGenfskSuccess_c)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();

#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
        /* Compute sequence duration */
        tempTime = GENFSK_GetTxDuration(instanceId, bufLengthBytes) + gGENFSK_Overhead_c;
#if defined (RADIO_IS_GEN_3P5)
        if (followedByRx)
        {
            tempTime += rxDuration + genfskLocal[mGenfskActiveInstance].genfskRegs.turnaroundTime;
        }
#endif
        if (txStartTime != 0ULL)
        {
            tempTime += (txStartTime - GENFSK_TimeGetTimestamp());
        }
#endif

        /* If GENFSK LL active sequence is Idle start TX. */
        if (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_Idle)
        {
            uint32_t  warmupTime;
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
            if ((tempTime < MWS_GetInactivityDuration(gMWS_GENFSK_c)) &&
                (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE)))
            {
#endif
                /* Change the active instance */
                if (instanceId != mGenfskActiveInstance)
                {
                    GENFSK_SwitchToInstance(instanceId);
                }
#if !defined (RADIO_IS_GEN_3P5)
                if ((genfskLocal[instanceId].packetType == gGenfskRawPacket) ||
                    (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk))
                {
                    GENFSK->PACKET_CFG &= ~(GENFSK_PACKET_CFG_H0_SZ_MASK |
                                            GENFSK_PACKET_CFG_H1_SZ_MASK);
                    GENFSK->PACKET_CFG &= ~(GENFSK_PACKET_CFG_LENGTH_ADJ_MASK);

                    /* Compute the length without sync address. */
                    uint16_t tempLength = bufLengthBytes - genfskLocal[instanceId].syncAddrSizeBytes;

                    /* If the length exceedes the LENGTH_ADJ size, store the remaining 4 bytes in H0 and H1. */
                    if (tempLength  > (gGENFSK_MaxRawPacketLength_c - 4U))
                    {
                        GENFSK->PACKET_CFG |= (GENFSK_PACKET_CFG_H0_SZ(16) | GENFSK_PACKET_CFG_H1_SZ(16));
                        tempLength -= 4U; /* Keep 4 bytes in H0 and H1 */
                    }
                    GENFSK->PACKET_CFG |= GENFSK_PACKET_CFG_LENGTH_ADJ(tempLength);
                }
#else /* !defined (RADIO_IS_GEN_3P5) */
                /* With RADIO_IS_GEN_3P0, gGENFSK_MaxRawPacketLength_c length without the SYNC address was 31 bytes. To increase
                the amount of data by 4 bytes, H0 and H1 fields were used. With RADIO_IS_GEN_3P5,  gGENFSK_MaxRawPacketLength_c
                length without the SYNC address is 1023 bytes. Gaining 4 additionnal bytes with H0 and H1 is no more relevant.
                Only need here is to provide the length information in LENGTH_ADJ. */
                if (genfskLocal[instanceId].packetType == gGenfskRawPacket)
                {
                    /* Compute the length without sync address. */
                    uint16_t tempLength = bufLengthBytes - genfskLocal[instanceId].syncAddrSizeBytes;
                    GENFSK->LENGTH_ADJ &= ~(GENFSK_LENGTH_ADJ_LENGTH_ADJ_MASK);
                    GENFSK->LENGTH_ADJ |= GENFSK_LENGTH_ADJ_LENGTH_ADJ(tempLength);
                }
#endif /* !defined (RADIO_IS_GEN_3P5) */

#if !defined (RADIO_IS_GEN_3P5)
                /* Set the packet buffer partition to maximum TX packet. */
                GENFSK->PB_PARTITION = gGENFSK_PbPartitionMaxTx_c;
#else
                /* If abort all command or TX cancel command has been issued, the whitening, preamble and crc generation
                have been deactivated => Reconfigure the XCVR_CFG register with user defined values. */
                GENFSK->XCVR_CFG = genfskLocal[instanceId].genfskRegs.xcvrCfg;
#endif

#if !defined (RADIO_IS_GEN_3P5)
                if (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)
                {
                    FLib_MemSet(codedBytes, 0, sizeof(codedBytes));
                    FLib_MemCpy(codedBytes, pBuffer, bufLengthBytes);

                    GENFSK_MskPreProcessing(codedBytes,
                                            codedBytes,
                                            (uint8_t)sizeof(codedBytes),
                                            ((~codedBytes[0]) & 0x01U));

                    /* Write packet to packet buffer. */
                    GENFSK_WritePacketBuffer(0, codedBytes, bufLengthBytes);
                }
                else
#endif
                {
                    /* Write packet to packet buffer. */
                    GENFSK_WritePacketBuffer(0, pBuffer, bufLengthBytes);
                }

#if !defined (RADIO_IS_GEN_3P5)
                warmupTime = (GENFSK->XCVR_CFG & GENFSK_XCVR_CFG_TX_WARMUP_MASK) >> GENFSK_XCVR_CFG_TX_WARMUP_SHIFT;
#else
                warmupTime = ((GENFSK->WARMUP_TIME & GENFSK_WARMUP_TIME_TX_WARMUP_MASK) >> GENFSK_WARMUP_TIME_TX_WARMUP_SHIFT);
#endif
                /* Start TX now. */
                if (txStartTime == 0ULL)
                {
#if defined (RADIO_IS_GEN_3P5)
                    if (followedByRx)
                    {
                        genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyTxRx;

                        genfskLocal[instanceId].genfskRxLocal.rxPacketBuffer = pRxBuffer;
                        genfskLocal[instanceId].genfskRxLocal.rxMaxPacketLength = maxBufLengthBytes;

                        /* Enable RX, TX and PLL unlock interrupts. */
                        GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK | GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK |
                                                GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK);
                        if (rxDuration != 0ULL)
                        {
                            currentTime = GENFSK_GetTimestamp();

                            event.timestamp = currentTime + warmupTime + GENFSK_GetTxDuration(instanceId, bufLengthBytes) + genfskLocal[mGenfskActiveInstance].genfskRegs.turnaroundTime + rxDuration + gGENFSK_Overhead_c ;
                            event.callback = GENFSK_RxTimeoutCallback;

                            rxTimeoutTimer = GENFSK_TimeScheduleEvent(&event);
                        }

                        GENFSK_Command(TR_START_NOW);

                    }
                    else
#endif
                    {
                        genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyTx;

                        /* Enable TX and PLL unlock interrupts. */
                        GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);

                        GENFSK_Command(TX_START_NOW);
                    }
                }
                else
                {
                    /* Subtract the TX warmup time from the start time. */
                    tempTime = txStartTime - (GENFSK_timestamp_t)warmupTime;

                    currentTime = GENFSK_GetTimestamp();

                    if (tempTime <= currentTime + gGENFSK_MinSetupTime_c)
                    {
                        status = gGenfskInstantPassed_c;
                    }
                    else if ((tempTime - currentTime) >= gGENFSK_OverflowTimerUnit_c)
                    {
                        /* Timer value overrun the 24bit value. */
                        status = gGenfskInvalidParameters_c;
                    }
                    else
                    {
                        GENFSK->T1_CMP = (uint32_t) ((tempTime & GENFSK_T1_CMP_T1_CMP_MASK) | GENFSK_T1_CMP_T1_CMP_EN_MASK);
#if defined (RADIO_IS_GEN_3P5)
                        if (followedByRx)
                        {
                            genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyPendingTxRx;

                            genfskLocal[instanceId].genfskRxLocal.rxPacketBuffer = pRxBuffer;
                            genfskLocal[instanceId].genfskRxLocal.rxMaxPacketLength = maxBufLengthBytes;

                            /* Enable RX, TX and PLL unlock interrupts. */
                            GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK | GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK |
                                                    GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK);

                            if (rxDuration != 0ULL)
                            {
                                event.timestamp = txStartTime + GENFSK_GetTxDuration(instanceId, bufLengthBytes) + genfskLocal[mGenfskActiveInstance].genfskRegs.turnaroundTime + rxDuration + gGENFSK_Overhead_c ;
                                event.callback = GENFSK_RxTimeoutCallback;

                                rxTimeoutTimer = GENFSK_TimeScheduleEvent(&event);
                            }

                            GENFSK_Command(TR_START_T1);

                        }
                        else
#endif
                        {
                            genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyPendingTx;

                            /* Enable TX and PLL unlock interrupts. */
                            GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);

                            GENFSK_Command(TX_START_T1);
                        }
                    }
                }
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
            }
            else
            {
                status = gGenfskFail_c;
            }
#endif
        }
        else
        {
            status = GENFSK_GetBusyStatus(genfskLocal[mGenfskActiveInstance].genfskState);
        }

        /* Exit critical section. */
        OSA_InterruptEnable();
    }

    return status;
}

genfskStatus_t GENFSK_CancelPendingTx(void)
{
    genfskStatus_t status = gGenfskSuccess_c;

    if (mGenfskActiveInstance != gGENFSK_InvalidIdx_c)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();

#if defined (RADIO_IS_GEN_3P5)
        if ((genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyTxRx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingTxRx))
        {
            GENFSK_Command(TR_CANCEL);
        }
        else
#endif
        {
            GENFSK_Command(TX_CANCEL);
        }

        /* Wait for XCVR to become idle. */
        while ((GENFSK->XCVR_CTRL & GENFSK_XCVR_CTRL_XCVR_BUSY_MASK) != 0U) {};

#if defined (RADIO_IS_GEN_3P5)
        /* Deactivate whitening */
        GENFSK->XCVR_CFG |= (GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK);
#endif

        /* Enable TX and PLL unlock interrupts. */
        GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);

        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;

        /* Exit critical section. */
        OSA_InterruptEnable();
    }

    return status;
}

#if gGENFSK_CheckParams_c == 1
/* Check that parameters provide to GENFSK_StartRx() are correct */
static genfskStatus_t GENFSK_StartRxCheckParams(uint8_t instanceId, uint8_t *pRxBuffer, uint16_t maxBufLengthBytes, GENFSK_timestamp_t rxDuration,
                                                const uint8_t *pTxBuffer, uint16_t txBufLengthBytes)
{
    genfskStatus_t status;

    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (pRxBuffer == NULL)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (((genfskLocal[instanceId].packetType == gGenfskRawPacket) || (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)) &&
              (maxBufLengthBytes > gGENFSK_MaxRawPacketLength_c  + (uint16_t)genfskLocal[instanceId].syncAddrSizeBytes))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (maxBufLengthBytes > gGENFSK_MaxPacketPayloadAndAdjLength_c
                                 + genfskLocal[instanceId].syncAddrSizeBytes
                                 + genfskLocal[instanceId].headerSizeBytes)
    {
        status = gGenfskInvalidParameters_c;
    }
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
    else if (rxDuration == 0ULL)
    {
        /* Continous reception forbidden when using coexistence module */
        status = gGenfskInvalidParameters_c;
    }
#endif
#if !defined (RADIO_IS_GEN_3P5)
    else if ((pTxBuffer != NULL) || (txBufLengthBytes != 0U))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        status = gGenfskSuccess_c;
    }
#else
    else if ((txBufLengthBytes != 0U) && (pTxBuffer == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (txBufLengthBytes != 0U)
    {
        /* Check that requested Tx payload size does not exceed maximum supported one */
        uint16_t maxPayloadSize;
        if ((genfskLocal[instanceId].packetType == gGenfskRawPacket) || (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk))
        {
            maxPayloadSize = (uint16_t)gGENFSK_MaxRawPacketLength_c + (uint16_t)genfskLocal[instanceId].syncAddrSizeBytes;
        }
        else
        {
            int16_t temp = (int16_t)gGENFSK_MaxPacketPayloadAndAdjLength_c - genfskLocal[instanceId].adjSizeBytes;
            maxPayloadSize = (uint16_t)temp;
            maxPayloadSize += (uint16_t)genfskLocal[instanceId].syncAddrSizeBytes + (uint16_t)genfskLocal[instanceId].headerSizeBytes;
        }

        if (txBufLengthBytes > maxPayloadSize)
        {
            status = gGenfskInvalidParameters_c;
        }
        else
        {
            status = gGenfskSuccess_c;
        }
    }
    else
    {
        status = gGenfskSuccess_c;
    }
#endif
    return status;
}
#endif

genfskStatus_t GENFSK_StartRx(uint8_t instanceId, uint8_t *pRxBuffer, uint16_t maxBufLengthBytes, GENFSK_timestamp_t rxStartTime, GENFSK_timestamp_t rxDuration)
{
    /* Call generic Rx function. */
    return GENFSK_StartRxTx(instanceId, pRxBuffer, maxBufLengthBytes, rxStartTime, rxDuration, NULL, 0);
}

genfskStatus_t GENFSK_StartRxTx(uint8_t instanceId, uint8_t *pRxBuffer, uint16_t maxBufLengthBytes, GENFSK_timestamp_t rxStartTime, GENFSK_timestamp_t rxDuration,
                              const uint8_t *pTxBuffer, uint16_t txBufLengthBytes)
{
    GENFSK_TimeEvent_t event;
    GENFSK_timestamp_t currentTime = 0;
    GENFSK_timestamp_t tempTime = 0;
#if defined (RADIO_IS_GEN_3P5)
    bool autoAck = FALSE;
#endif
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    status = GENFSK_StartRxCheckParams(instanceId, pRxBuffer, maxBufLengthBytes, rxDuration, pTxBuffer, txBufLengthBytes);

    if (status == gGenfskSuccess_c)
#endif
    {
        if (genfskLocal[instanceId].genfskState == gGENFSK_LL_NoInit)
        {
            status = gGenfskNotInitialized_c;
        }
    }

    if (status == gGenfskSuccess_c)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();

#if defined (RADIO_IS_GEN_3P5)
        if ((pTxBuffer != NULL) && (txBufLengthBytes != 0U))
        {
            autoAck = TRUE;
        }
#endif
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
        /* Compute sequence duration */
        tempTime = rxDuration + gGENFSK_Overhead_c;
#if defined (RADIO_IS_GEN_3P5)
        if (autoAck)
        {
            tempTime += GENFSK_GetTxDuration(instanceId, txBufLengthBytes) + genfskLocal[instanceId].genfskRegs.turnaroundTime;
        }
#endif
        if (rxStartTime != 0ULL)
        {
            tempTime += (rxStartTime - GENFSK_TimeGetTimestamp());
        }
#endif

        /* If GENFSK LL is idle start RX. */
        if (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_Idle)
        {
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
            if ((tempTime < MWS_GetInactivityDuration(gMWS_GENFSK_c)) &&
                (gMWS_Success_c == MWS_Acquire(gMWS_GENFSK_c, FALSE)))
            {
#endif
                /* Change the active instance */
                if (instanceId != mGenfskActiveInstance)
                {
                    GENFSK_SwitchToInstance(instanceId);
                }
#if !defined (RADIO_IS_GEN_3P5)
                if ((genfskLocal[instanceId].packetType == gGenfskRawPacket) ||
                    (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk))
                {
                    GENFSK->PACKET_CFG &= ~(GENFSK_PACKET_CFG_H0_SZ_MASK |
                                            GENFSK_PACKET_CFG_H1_SZ_MASK);
                    GENFSK->PACKET_CFG &= ~(GENFSK_PACKET_CFG_LENGTH_ADJ_MASK);

                    /* Compute the length without sync address. */
                    uint16_t tempLength = maxBufLengthBytes - genfskLocal[instanceId].syncAddrSizeBytes;

                    /* If the length exceedes the LENGTH_ADJ size, store the remaining 4 bytes in H0 and H1. */
                    if (tempLength  > (gGENFSK_MaxRawPacketLength_c - 4U))
                    {
                        GENFSK->PACKET_CFG |= (GENFSK_PACKET_CFG_H0_SZ(16) |
                                               GENFSK_PACKET_CFG_H1_SZ(16));
                        tempLength -= 4U; /* Keep 4 bytes in H0 and H1 */
                    }
                    GENFSK->PACKET_CFG |= GENFSK_PACKET_CFG_LENGTH_ADJ(tempLength);
                }
#else /* !defined (RADIO_IS_GEN_3P5) */
                /* With RADIO_IS_GEN_3P0, gGENFSK_MaxRawPacketLength_c length without the SYNC address was 31 bytes. To increase
                the amount of data by 4 bytes, H0 and H1 fields were used. With RADIO_IS_GEN_3P5,  gGENFSK_MaxRawPacketLength_c
                length without the SYNC address is 1023 bytes. Gaining 4 additionnal bytes with H0 and H1 is no more relevant.
                Only need here is to provide the length information in LENGTH_ADJ. */
                if (genfskLocal[instanceId].packetType == gGenfskRawPacket)
                {
                    /* Compute the length without sync address. */
                    uint16_t tempLength = maxBufLengthBytes - genfskLocal[instanceId].syncAddrSizeBytes;
                    GENFSK->LENGTH_ADJ &= ~(GENFSK_LENGTH_ADJ_LENGTH_ADJ_MASK);
                    GENFSK->LENGTH_ADJ |= GENFSK_LENGTH_ADJ_LENGTH_ADJ(tempLength);
                }
#endif /* !defined (RADIO_IS_GEN_3P5) */

#if !defined (RADIO_IS_GEN_3P5)
                /* Set the packet buffer partition to maximum RX packet. */
                GENFSK->PB_PARTITION = gGENFSK_PbPartitionMaxRx_c;
#endif

                /* Enable RX interrupts. */
                GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);

#if defined(RADIO_IS_GEN_3P0) && (RADIO_IS_GEN_3P0 != 0) && defined(RF_OSC_26MHZ) && (RF_OSC_26MHZ != 0)
                if ( genfskLocal[mGenfskActiveInstance].radioConfig.dataRate == gGenfskDR2Mbps )
                {
                    /*  When 26MHz RF clock is used with 2mbps data rate, the
                        GENFSK receiver auto recycle sometimes does not trigger
                        the next reception correctly in case of bad h0/h1/length/crc.
                        The workaround consists of disable the rx recycle and
                        recycle the rx manually. */
                    if( (genfskLocal[mGenfskActiveInstance].genfskRegs.whitenSzThr & GENFSK_WHITEN_SZ_THR_REC_BAD_PKT_MASK) == 0 )
                    {
                        /* Disable recycle if H0, H1 or length error */
                        GENFSK->WHITEN_SZ_THR |=  GENFSK_WHITEN_SZ_THR_REC_BAD_PKT_MASK;
                        gGenfskRxRecycleHdrError = 1;
                    }
                    else
                    {
                        gGenfskRxRecycleHdrError = 0;
                    }

                    if( genfskLocal[mGenfskActiveInstance].crcRecvInvalid == gGenfskCrcSupressInvalid )
                    {
                        /* Disable recycle if crc error */
                        GENFSK->IRQ_CTRL = (GENFSK->IRQ_CTRL | GENFSK_IRQ_CTRL_CRC_IGNORE_MASK) & 0xFFFF0000U; /* do not w1c status bits */
                        gGenfskRxRecycleCrcError = 1;
                    }
                    else
                    {
                        gGenfskRxRecycleCrcError = 0;
                    }
                }
#endif

                genfskLocal[instanceId].genfskRxLocal.rxPacketBuffer = pRxBuffer;
                genfskLocal[instanceId].genfskRxLocal.rxMaxPacketLength = maxBufLengthBytes;

#if defined (RADIO_IS_GEN_3P5)
                /* Handle Auto-Ack sequence if required (only available starting fron Gen4 GenLL */
                if (autoAck)
                {
#ifndef RADIO_IS_GEN_3P5
                    /* Twiddling is done by HW */
                    if (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk)

                    {
                        uint8_t codedBytes[gGENFSK_MaxSyncAddressSize_c + gGENFSK_MaxRawPacketLength_c];

                        FLib_MemSet(codedBytes, 0, sizeof(codedBytes));
                        FLib_MemCpy(codedBytes, pTxBuffer, txBufLengthBytes);
                        GENFSK_MskPreProcessing(codedBytes,
                                                codedBytes,
                                                (uint8_t)sizeof(codedBytes),
                                                ((~codedBytes[0]) & 0x01U));
                        /* Write packet to packet buffer. */
                        GENFSK_WritePacketBuffer(0, codedBytes, txBufLengthBytes);
                    }
                    else
#endif
                    {
                        /* Write packet to packet buffer. */
                        GENFSK_WritePacketBuffer(0, pTxBuffer, txBufLengthBytes);
                    }

                    GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK);

                    GENFSK->ENH_FEATURE |= GENFSK_ENH_FEATURE_AUTOACK_MASK;

                    /* Disable fast warmup for autoack sequence. See Jira CONNRF-485. */
                    XCVR_TSM->FAST_CTRL1 &= ~(XCVR_TSM_FAST_CTRL1_FAST_RX2TX_EN_MASK | XCVR_TSM_FAST_CTRL1_FAST_RX_WU_EN_MASK | XCVR_TSM_FAST_CTRL1_FAST_TX_WU_EN_MASK);
                }
                else
                {
                    GENFSK->ENH_FEATURE &= ~GENFSK_ENH_FEATURE_AUTOACK_MASK;
                }

                /* If abort all command or RX cancel command has been issued, the whitening has
                been deactivated => Reconfigure the XCVR_CFG register with user defined values. */
                GENFSK->XCVR_CFG = genfskLocal[instanceId].genfskRegs.xcvrCfg;
#endif

                /* Start RX now. */
                if (rxStartTime == 0ULL)
                {
#if defined (RADIO_IS_GEN_3P5)
                    if (autoAck)
                    {
                        genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyRxTx;
                    }
                    else
#endif
                    {
                        genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyRx;
                    }

                    if (rxDuration != 0ULL)
                    {
                        event.timestamp = GENFSK_GetTimestamp() + rxDuration;
                        event.callback = GENFSK_RxTimeoutCallback;

                        rxTimeoutTimer = GENFSK_TimeScheduleEvent(&event);
                    }

                    /* Start RX immediately. */
                    GENFSK_Command(RX_START_NOW);
                }
                else
                {
                    /* Subtract the RX warmup time from the start time. */
                    uint32_t warmupTime;
#if !defined (RADIO_IS_GEN_3P5)
                    warmupTime =  (GENFSK->XCVR_CFG & GENFSK_XCVR_CFG_RX_WARMUP_MASK) >> GENFSK_XCVR_CFG_RX_WARMUP_SHIFT;
#else
                    warmupTime =  (GENFSK->WARMUP_TIME & GENFSK_WARMUP_TIME_RX_WARMUP_MASK) >> GENFSK_WARMUP_TIME_RX_WARMUP_SHIFT;
#endif
                    tempTime = rxStartTime - (GENFSK_timestamp_t)warmupTime;

                    currentTime = GENFSK_GetTimestamp();

                    if (tempTime <= currentTime + gGENFSK_MinSetupTime_c)
                    {
                        status = gGenfskInstantPassed_c;
                    }
                    else if ((tempTime - currentTime) >= gGENFSK_OverflowTimerUnit_c)
                    {
                        /* The start time absolute value should be less than 24bits. */
                        status = gGenfskInvalidParameters_c;
                    }
                    else
                    {
#if defined (RADIO_IS_GEN_3P5)
                        if (autoAck)
                        {
                            genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyPendingRxTx;
                        }
                        else
#endif
                        {
                            genfskLocal[instanceId].genfskState = gGENFSK_LL_BusyPendingRx;
                        }

                        GENFSK->T1_CMP = (uint32_t) ((tempTime & GENFSK_T1_CMP_T1_CMP_MASK) | GENFSK_T1_CMP_T1_CMP_EN_MASK);

                        if (rxDuration != 0ULL)
                        {
                            event.timestamp = rxStartTime + rxDuration;
                            event.callback = GENFSK_RxTimeoutCallback;

                            rxTimeoutTimer = GENFSK_TimeScheduleEvent(&event);
                        }

                        GENFSK_Command(RX_START_T1);
                    }
                }
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
            }
            else
            {
                status = gGenfskFail_c;
            }
#endif
        }
        else
        {
            status = GENFSK_GetBusyStatus(genfskLocal[mGenfskActiveInstance].genfskState);
        }

        /* Exit critical section. */
        OSA_InterruptEnable();
    }

    return status;
}

genfskStatus_t GENFSK_CancelPendingRx(void)
{
    genfskStatus_t status = gGenfskSuccess_c;

    if (mGenfskActiveInstance != gGENFSK_InvalidIdx_c)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();

        GENFSK_TimeCancelEvent(&rxTimeoutTimer);

        GENFSK_Command(RX_CANCEL);

        /* Wait for XCVR to become idle. */
        while ((GENFSK->XCVR_CTRL & GENFSK_XCVR_CTRL_XCVR_BUSY_MASK) != 0U) {};

#if defined (RADIO_IS_GEN_3P5)
        /* Deactivate whitening */
        GENFSK->XCVR_CFG |= (GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK);
#endif

        /* Disable RX interrupts. */
        GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK | GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK);

        genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength = 0;
        genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer = NULL;
        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;

        /* Exit critical section. */
        OSA_InterruptEnable();
    }

    return status;
}

void GENFSK_AbortAll(void)
{
    if (mGenfskActiveInstance != gGENFSK_InvalidIdx_c)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();

        GENFSK_TimeCancelEvent(&rxTimeoutTimer);

        GENFSK_Command(ABORT_ALL);

        /* Wait for XCVR to become idle. 
        In some cases (abort during TX warmup, see CONNRF-767), you may enter in an infinite loop.
        To workaround the issue, you may want to implement at the caller side a wait loop (waiting for the completion
        of TX warmup): please refer to below MWS_GENFSK_Callback() function (gMWS_Release_c case).*/
        while ((GENFSK->XCVR_CTRL & GENFSK_XCVR_CTRL_XCVR_BUSY_MASK) != 0U) {};

#if defined (RADIO_IS_GEN_3P5)
        /* Deactivate whitening */
        GENFSK->XCVR_CFG |= (GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK | GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK);
#endif

        /* Disable interrupts. */
        GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK |
                                    GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK | GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK);

        genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength = 0;
        genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer = NULL;
        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;

#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
        (void) OSA_EventSet(mGenfskTaskEvent, gGenfskMwsReleaseEventFlag_c);
#endif /* gGENFSK_LLMwsControl_c */
        /* Exit critical section. */
        OSA_InterruptEnable();
    }
}

GENFSK_timestamp_t GENFSK_GetTimestamp(void)
{
    volatile GENFSK_timestamp_t tempTime = 0;

    /* fetches the current value of the timebase for the LL if in proper state. */
    if (TRUE == genfskAlreadyInit)
    {
        tempTime = GENFSK_TimeGetTimestamp();
    }

    return tempTime;
}

genfskStatus_t GENFSK_SetEventMask(uint8_t instanceId, uint32_t eventMask)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    /* Sets the event mask according to genfskEventNotifyCallBack_t definition */
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (eventMask > gGenfskAllEvents)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    {
        genfskLocal[instanceId].enabledEvents = eventMask;
    }
    return status;
}

uint32_t GENFSK_GetEventMask(uint8_t instanceId)
{
    uint32_t eventMask = 0xFF;

    if (instanceId < gGENFSK_InstancesCnt_c)
    {
        eventMask = genfskLocal[instanceId].enabledEvents & ~gGenfskPromiscuousEvent;
    }

    return eventMask;
}

genfskStatus_t GENFSK_RegisterCallbacks(uint8_t instanceId, genfskPacketReceivedCallBack_t packetReceivedCallback, genfskEventNotifyCallBack_t eventCallback)
{
    genfskStatus_t status = gGenfskSuccess_c;

    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();

        genfskLocal[instanceId].packetReceivedcallback = packetReceivedCallback;
        genfskLocal[instanceId].eventNotifyCallback = eventCallback;

        /* Exit critical section. */
        OSA_InterruptEnable();
    }

    return status;
}

genfskStatus_t GENFSK_PacketToByteArray(uint8_t instanceId, const GENFSK_packet_t *pPacket, uint8_t *pBuffer)
{
    uint8_t lengthFieldSize;
    uint8_t h0FieldSize;
    uint8_t h1FieldSize;
    uint8_t syncAddrSize;
    uint64_t tempVar;
    uint16_t tempField;
    uint8_t fieldSizeAdjust;
    uint16_t formattedLength = 0;

    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((pPacket == NULL) || (pBuffer == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    {
        lengthFieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_SZ_MASK) >> GENFSK_PACKET_CFG_LENGTH_SZ_SHIFT);
        h0FieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H0_SZ_MASK) >> GENFSK_PACKET_CFG_H0_SZ_SHIFT);
        h1FieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H1_SZ_MASK) >> GENFSK_PACKET_CFG_H1_SZ_SHIFT);
        syncAddrSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_SYNC_ADDR_SZ_MASK) >> GENFSK_PACKET_CFG_SYNC_ADDR_SZ_SHIFT);

        tempVar = (uint64_t)pPacket->addr << (64U - 8U * (syncAddrSize + 1U));

        for (uint8_t count = syncAddrSize + 1U; count > 0U; count --)
        {
            *pBuffer++ = (uint8_t) ((tempVar >> (64U - 8U * count)) & 0xFFU);
        }

        fieldSizeAdjust = h1FieldSize;
        tempField = (uint16_t)(pPacket->header.h1Field & ((1UL << h1FieldSize) - 1U));
        tempVar = ((uint64_t)tempField << (64U - h1FieldSize));
        fieldSizeAdjust = h1FieldSize + lengthFieldSize;

        /* Formatting the length field with MSBs first */
        if ((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_BIT_ORD_MASK) == GENFSK_PACKET_CFG_LENGTH_BIT_ORD_MASK)
        {
            uint16_t tempLength = pPacket->header.lengthField;
            for (uint8_t i = 0; i < lengthFieldSize; i++)
            {
                formattedLength |= ((tempLength & 0x1U) << (lengthFieldSize - 1U - i));
                tempLength >>= 0x1U;
            }
        }
        else
        {
            formattedLength = pPacket->header.lengthField;
        }

        tempField = (uint16_t)(formattedLength & ((1UL << lengthFieldSize) - 1U));
        tempVar |= ((uint64_t)tempField << (64U - fieldSizeAdjust));
        fieldSizeAdjust = h1FieldSize + lengthFieldSize + h0FieldSize;
        tempField = (uint16_t)(pPacket->header.h0Field & ((1UL << h0FieldSize) - 1U));
        tempVar |= ((uint64_t)tempField << (64U - fieldSizeAdjust));
        fieldSizeAdjust = fieldSizeAdjust / 8U;

        for (uint8_t count = fieldSizeAdjust; count > 0U; count --)
        {
            *pBuffer++ = (uint8_t) ((tempVar >> (64U - 8U * count)) & 0xFFU);
        }

        /* Check mode of operation: payload specified or not */
        if (pPacket->payload != NULL)
        {
            /* Payload is specified: "copy" mode.
             * The API copies the provided payload into the buffer.
             */
            FLib_MemCpy(pBuffer, pPacket->payload, pPacket->header.lengthField);
        }
    }

    return status;
}

genfskStatus_t GENFSK_ByteArrayToPacket(uint8_t instanceId, const uint8_t *pBuffer, GENFSK_packet_t *pPacket)
{
    uint8_t lengthFieldSize;
    uint8_t h0FieldSize;
    uint8_t h1FieldSize;
    uint8_t syncAddrSize;
    uint8_t crcSize;
    uint32_t count = 0;
    uint8_t fieldSizeAdjust = 0;
    uint64_t tempVar = 0;

    genfskStatus_t status = gGenfskSuccess_c;

#if gGENFSK_CheckParams_c == 1
    if (instanceId >= gGENFSK_InstancesCnt_c)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if ((pPacket == NULL) || (pBuffer == NULL) || (pPacket->payload == NULL))
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    {
        lengthFieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_LENGTH_SZ_MASK) >> GENFSK_PACKET_CFG_LENGTH_SZ_SHIFT);
        h0FieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H0_SZ_MASK) >> GENFSK_PACKET_CFG_H0_SZ_SHIFT);
        h1FieldSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_H1_SZ_MASK) >> GENFSK_PACKET_CFG_H1_SZ_SHIFT);
        syncAddrSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.packetCfg & GENFSK_PACKET_CFG_SYNC_ADDR_SZ_MASK) >> GENFSK_PACKET_CFG_SYNC_ADDR_SZ_SHIFT);
#if !defined (RADIO_IS_GEN_3P5)
        crcSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_SZ_MASK) >> GENFSK_CRC_CFG_CRC_SZ_SHIFT);
#else
        crcSize = (uint8_t)((genfskLocal[instanceId].genfskRegs.crcCfg & RBME_CRCW_CFG3_CRC_SZ_MASK) >> RBME_CRCW_CFG3_CRC_SZ_SHIFT);
#endif
        pPacket->addr = 0x00U;

        for (count = 0U; count < ((uint32_t)syncAddrSize + 1U); count++)
        {
            pPacket->addr |= ((uint32_t)(*pBuffer++)) << (8U * count);
        }

        fieldSizeAdjust += lengthFieldSize + h0FieldSize + h1FieldSize;

        for (count = 0; count < ((uint32_t)fieldSizeAdjust) / 8U; count++)
        {
            tempVar |= ((uint64_t)(*pBuffer++)) << (8U * count);
        }

        pPacket->header.h1Field = (uint16_t) ((tempVar >> (lengthFieldSize + h0FieldSize)) & 0xFFU);
        pPacket->header.lengthField = (uint16_t) ((tempVar << (64U - (h0FieldSize + lengthFieldSize))) >> (64U - lengthFieldSize));
        pPacket->header.h0Field = (uint16_t) ((tempVar << (64U - (h0FieldSize))) >> (64U - h0FieldSize));

        if (genfskLocal[instanceId].packetType != gGenfskRawPacket)
        {
            count = (uint32_t)pPacket->header.lengthField + crcSize;
        }
        else
        {
            uint32_t length = 0;
            /* For raw packet the length field in the header is 0. The LENGTH_ADJ register contains
            the length of the payload + crc. */
#if defined (RADIO_IS_GEN_3P5)
            length = GENFSK->LENGTH_ADJ & GENFSK_LENGTH_ADJ_LENGTH_ADJ_MASK;
#else
            length = GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_LENGTH_ADJ_MASK;
#endif
            count = length;
        }
        FLib_MemCpy(pPacket->payload, pBuffer, count);
    }

    return status;
}

/*Functions to be called prior calling GTM STart Rx/Tx functions:
    GENFSK_RadioConfig()
    GENFSK_SetPacketConfig()
    GENFSK_SetNetworkAddress()
    GENFSK_EnableNetworkAddress()
    GENFSK_SetTxPowerLevel()
    GENFSK_SetChannelNumber()
    GENFSK_SetModeConfig()
*/

#if defined (RADIO_IS_GEN_3P5)
static uint32_t GENFSK_GtmGetPacketSize(uint8_t instanceId, uint32_t pdu_length, bool_t packet_size_with_aa)
{
    uint32_t packet_size = 0;

    /* Computes the packet size if packet configuration is retrieved successfully. If not, a zero value is returned. */
    if (genfskLocal[instanceId].genfskState != gGENFSK_LL_NoInit)
    {
        packet_size = pdu_length + (uint32_t)genfskLocal[instanceId].headerSizeBytes;

        if (packet_size_with_aa)
        {
            uint32_t preamble_size = (genfskLocal[instanceId].genfskRegs.xcvrCfg & GENFSK_XCVR_CFG_GEN_PREAMBLE_MASK) >> GENFSK_XCVR_CFG_GEN_PREAMBLE_SHIFT;
            packet_size += ((uint32_t)genfskLocal[instanceId].syncAddrSizeBytes) + (preamble_size + 1UL);
        }
    }
    return packet_size;
}
#endif /* RADIO_IS_GEN_3P5 */

#if defined (RADIO_IS_GEN_3P5)
#define gGENFSK_RxRecycleTimeMargin    50U /* 50 us is required to account for the processing delay in the GENFSK framework
                                              between the time the RX IRQ is issued and the time the RX is handled in the
                                              GENFSK Task ( ie get the number of bytes received and retrieve the data in
                                              the packet RAM )*/
GENFSK_STATIC genfskStatus_t GENFSK_GtmGetIpd(uint8_t instanceId, uint32_t pdu_length, uint32_t* gtm_ipd, uint32_t* recycle_time)
{
    genfskStatus_t status = gGenfskSuccess_c;

    uint32_t min_gtm_ipd;
    uint32_t min_recycle_time;
    uint32_t duration_us;
    
    /* Get the size of the packet including: preamble, access address, H0, length, H1, payload and CRC. */
    uint32_t buffLen = GENFSK_GtmGetPacketSize(instanceId, pdu_length, TRUE);

    if (buffLen != 0U)
    {
        duration_us = GENFSK_GetTxDuration(instanceId, (uint16_t)buffLen);

        uint32_t end_of_seq = XCVR_TSM->END_OF_SEQ;
        /*Compute first minimum TX IPD*/
        uint32_t gtm_ipd_tx;
        uint32_t warmupTime = ((GENFSK->WARMUP_TIME & GENFSK_WARMUP_TIME_TX_WARMUP_MASK) >> GENFSK_WARMUP_TIME_TX_WARMUP_SHIFT);
        uint32_t warmdownTime = ((end_of_seq & XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_SHIFT) -
                                ((end_of_seq & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT);
        gtm_ipd_tx = duration_us + warmupTime + warmdownTime;
        
        /*Compute first minimum RX IPD*/
        uint32_t gtm_ipd_rx;
        warmupTime = ((GENFSK->WARMUP_TIME & GENFSK_WARMUP_TIME_RX_WARMUP_MASK) >> GENFSK_WARMUP_TIME_RX_WARMUP_SHIFT);
        warmdownTime = ((end_of_seq & XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_SHIFT) -
                       ((end_of_seq & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT);
        gtm_ipd_rx = duration_us + warmupTime + warmdownTime;
        
        min_gtm_ipd = (gtm_ipd_rx > gtm_ipd_tx) ? gtm_ipd_rx : gtm_ipd_tx;
          
        min_gtm_ipd += gGENFSK_RxRecycleTimeMargin; /* Add some margin. */
        
        /* If gtm_ipd = 0 (user lets SW compute the value) or if provided value is smaller than minimal IPD value
        then overwrite the current IPD value. */
        if (*gtm_ipd <= min_gtm_ipd)
        {
            *gtm_ipd = min_gtm_ipd;
            /* min_gtm_ipd goes with min recycle time so update recycle time when gtm_ipd = 0. */
            if (recycle_time != NULL)
            {
                *recycle_time = 0;
            }
        }
        
        /* recycle_time is applicable to current context */
        if (recycle_time != NULL)
        {
            min_recycle_time = warmdownTime;  /* gtm_ipd_rx - duration_us - warmupTime */
            if (warmdownTime < gGENFSK_RxRecycleTimeMargin)
            {
                min_recycle_time = gGENFSK_RxRecycleTimeMargin;
            }
            
            /* If recycle_time = 0 (user lets SW compute the value) or if provided value is smaller than minimal recycle value
            then overwrite the current recycle value. */
            if (*recycle_time <= min_recycle_time)
            {
                *recycle_time = min_recycle_time;
            }
        }
    }
    else
    {
        status = gGenfskFail_c;
    }
    return status;
}
#endif /* RADIO_IS_GEN_3P5 */

genfskStatus_t GENFSK_GtmStartRx(uint8_t instanceId, const GENFSK_gtm_rx_config_t* rx_gtm_config)
{
    genfskStatus_t status = gGenfskSuccess_c;
#if defined (RADIO_IS_GEN_3P5)    
    uint32_t sfd2wd_time = 0;
            
    /* Get the PDU length including H0, length, H1, payload and CRC. */
    /* pdu_length passed to GENFSK_GtmGetPacketSize()
    -if LENGTH_ADJ =0, pdu_length = length of (PAYLOAD + CRC)
    -if LENGTH_ADJ!=0, pdu_length = length of PAYLOAD
    */
    uint32_t length = GENFSK_GtmGetPacketSize(instanceId, rx_gtm_config->pdu_length, FALSE);
    uint32_t mode = genfskLocal[instanceId].genfskRegs.enhFeature & GENFSK_ENH_FEATURE_GENLL_MODE_MASK;
    
    if ((length != 0U) && (mode == (uint32_t)gGenfskGtmMode))
    {
        /* The TX duration also applies to RX duration. Add 2 bytes margin. */
        sfd2wd_time = GENFSK_GetTxDuration(instanceId, (uint16_t)(length+2U));
        
        GENFSK->GTM_FIRST_SFD2WD &= ~(GENFSK_GTM_FIRST_SFD2WD_GTM_FIRST_SFD2WD_MASK);
        GENFSK->GTM_FIRST_SFD2WD |= GENFSK_GTM_FIRST_SFD2WD_GTM_FIRST_SFD2WD(sfd2wd_time);
        
        uint32_t gtm_ipd = rx_gtm_config->gtm_ipd;
        uint32_t recycle_time = rx_gtm_config->recycle_time;
        
        /* No need to check here for the returned value. */
        (void) GENFSK_GtmGetIpd(instanceId, rx_gtm_config->pdu_length, &gtm_ipd, &recycle_time);
        /* Program the register RX_RECYCLE_TIME, which means the time between the
        force Rx warmdown and Rx warmup. */
        GENFSK->GTM_RX_RECYCLE_TIME &= ~(GENFSK_GTM_RX_RECYCLE_TIME_GTM_RX_RECYCLE_TIME_MASK);
        GENFSK->GTM_RX_RECYCLE_TIME |= GENFSK_GTM_RX_RECYCLE_TIME_GTM_RX_RECYCLE_TIME(recycle_time);

        /* Program the interpacket duration (register GTM_IPD[19:0], reused from MACLONGADDRS1_MSB[19:0]) */
        GENFSK->GTM_IPD &= ~GENFSK_GTM_IPD_GTM_IPD_MASK;
        GENFSK->GTM_IPD |= GENFSK_GTM_IPD_GTM_IPD(gtm_ipd);

        /* If register GTM_PKT_COUNT_CHECK_DIS is programmed, then 
            The check GTM_PKT_COUNT == GTM_PKT_NUM is disabled */
        GENFSK->GTM_CFG = GENFSK_GTM_CFG_GTM_IPD_CHECK_DIS((uint8_t) rx_gtm_config->ipd_check_dis) |
                            GENFSK_GTM_CFG_GTM_PKT_COUNT_CHECK_DIS((uint8_t)rx_gtm_config->pkt_count_check_dis) |
                            GENFSK_GTM_CFG_GTM_PKT_NUM(rx_gtm_config->packet_count);

        /* Start GTM-RX scan mode. This bit should also clear all GTM packet 
            counters (GTM_GOOD_PKT_COUNT, GTM_BAD_PKT_COUNT and GTM_PKT_COUNT) */
        GENFSK->GTM_CTRL &= ~GENFSK_GTM_CTRL_GTM_IN_RX_MASK; 
        GENFSK->GTM_CTRL |= GENFSK_GTM_CTRL_GTM_IN_RX_MASK; 

        /* The receiver will wait for the first correctly AA/SFD matched packet. This initiates
            the GTM-RX state machine. */
        /* Every packet for which AA/SFD match happens should increment the GTM_PKT_COUNT. */

        /* Once GTM_PKT_COUNT == GTM_PKT_NUM, GTM_IN_RX bit should be deasserted
        and the periodic RX recycling by the GTM state machine should stop. */
    }
    else
#endif /* RADIO_IS_GEN_3P5 */
    {
        status = gGenfskFail_c;
    }

    return status;
}
genfskStatus_t GENFSK_GtmStopRx(void)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if defined (RADIO_IS_GEN_3P5)  
    /* Turn off RX */
    GENFSK->GTM_CTRL &= ~GENFSK_GTM_CTRL_GTM_IN_RX_MASK;
#endif /* RADIO_IS_GEN_3P5 */

    return status;
}
                                 
genfskStatus_t GENFSK_GtmStartTx(uint8_t instanceId, const GENFSK_gtm_tx_config_t* tx_gtm_config)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if defined (RADIO_IS_GEN_3P5) 
#if gGENFSK_CheckParams_c == 1
    if (tx_gtm_config->pdu_type >= genfskGtmPktPayloadMax)
    {
        status = gGenfskInvalidParameters_c;
    }
    else if (tx_gtm_config->packet_count >= GENFSK_GTM_CFG_GTM_PKT_NUM_MASK)
    {
        status = gGenfskInvalidParameters_c;
    }
    else
#endif
    {
        uint32_t mode = genfskLocal[instanceId].genfskRegs.enhFeature & GENFSK_ENH_FEATURE_GENLL_MODE_MASK;
        
        if (mode == (uint32_t)gGenfskGtmMode)
        {
            /* Program a fixed length for the Packet size in octets (PKT_LEN). */
            genfskLocal[instanceId].genfskRegs.enhFeature &= ~(GENFSK_ENH_FEATURE_LENGTH_ACK_MASK);
            /* pdu_length
            -if LENGTH_ADJ=0, pdu_length = length of (PAYLOAD + CRC)
            -if LENGTH_ADJ!=0, pdu_length = length of PAYLOAD
            */
            genfskLocal[instanceId].genfskRegs.enhFeature |= GENFSK_ENH_FEATURE_LENGTH_ACK(tx_gtm_config->pdu_length);
            
            GENFSK->ENH_FEATURE = genfskLocal[instanceId].genfskRegs.enhFeature;
            
            /* Program the type of payload (GTM_PDU) and number of packets to be sent (GTM_PKT_NUM). */
            GENFSK->GTM_CFG &= ~(GENFSK_GTM_CFG_GTM_PDU_TYPE_MASK |
                                GENFSK_GTM_CFG_GTM_PKT_NUM_MASK | 
                                GENFSK_GTM_CFG_GTM_IPD_CHECK_DIS_MASK | 
                                GENFSK_GTM_CFG_GTM_PKT_COUNT_CHECK_DIS_MASK);
            GENFSK->GTM_CFG |=  GENFSK_GTM_CFG_GTM_PDU_TYPE((uint8_t)(tx_gtm_config->pdu_type)) |
                                GENFSK_GTM_CFG_GTM_PKT_NUM(tx_gtm_config->packet_count);
        
            if ((tx_gtm_config->pdu_type == genfskGtm8BitPatternPayload) || 
                (tx_gtm_config->pdu_type == genfskGtm32BitPatternPayload))
            {
                /* Programmable Pattern (from register GTM_PDU, reused from MACSHORTADDRS1*/
                GENFSK->GTM_PDU = tx_gtm_config->pattern;
            }
            
            uint32_t gtm_ipd = tx_gtm_config->gtm_ipd;
            status = GENFSK_GtmGetIpd(instanceId, tx_gtm_config->pdu_length, &gtm_ipd, NULL);
            
            if (status == gGenfskSuccess_c)
            {
                /* Program the interpacket duration (register GTM_IPD[19:0], reused from MACLONGADDRS1_MSB[19:0]). */
                GENFSK->GTM_IPD &= ~GENFSK_GTM_IPD_GTM_IPD_MASK;
                GENFSK->GTM_IPD |= GENFSK_GTM_IPD_GTM_IPD(gtm_ipd);

                /* Program the delay time between 1st Tx warmup time and GTM_IN_TX asserted 
                (register GTM_RX_RECYCLE_TIME[19:0], reused from MACLONGADDRS0_MSB[19:0];
                This register has different meaning in GTM RX mode) */
                GENFSK->GTM_RX_RECYCLE_TIME &= ~(GENFSK_GTM_RX_RECYCLE_TIME_GTM_RX_RECYCLE_TIME_MASK);
                GENFSK->GTM_RX_RECYCLE_TIME |= GENFSK_GTM_RX_RECYCLE_TIME_GTM_RX_RECYCLE_TIME(tx_gtm_config->warmup_time_delay);
            
                /* Turn on TX */
                GENFSK->GTM_CTRL |= GENFSK_GTM_CTRL_GTM_IN_TX_MASK;
            }
            else
            {
                status = gGenfskFail_c;
            }
        }
        else
        {
            status = gGenfskFail_c;
        }
    }
#else
    status = gGenfskFail_c;
#endif /* RADIO_IS_GEN_3P5 */
    return status;
}
genfskStatus_t GENFSK_GtmStopTx(uint8_t instanceId)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if defined (RADIO_IS_GEN_3P5)
    /* Restore */
    genfskLocal[instanceId].genfskRegs.enhFeature &= ~(GENFSK_ENH_FEATURE_LENGTH_ACK_MASK);
    
    /* Turn off TX */
    GENFSK->GTM_CTRL &= ~GENFSK_GTM_CTRL_GTM_IN_TX_MASK; 
#else
    status = gGenfskFail_c;
#endif /* RADIO_IS_GEN_3P5 */
    
    return status;
  
}
                                                           
genfskStatus_t GENFSK_GtmReadPacketCount(GENFSK_gtm_packet_count_t *pkt_results)
{
    genfskStatus_t status = gGenfskSuccess_c;

#if defined (RADIO_IS_GEN_3P5)
    /* Read results from GTM module */
    pkt_results->packet_count = (uint16_t)(GENFSK->GTM_PKT_CNT & GENFSK_GTM_PKT_CNT_GTM_PKT_COUNT_MASK);
    pkt_results->good_packet_count = (uint16_t)(GENFSK->GTM_GOOD_CNT & GENFSK_GTM_GOOD_CNT_GTM_GOOD_PKT_COUNT_MASK);
    pkt_results->bad_packet_count = (uint16_t)(GENFSK->GTM_BAD_CNT & GENFSK_GTM_BAD_CNT_GTM_BAD_PKT_COUNT_MASK);
    
    /* When the expected number of packets have been sent/received, is_activity_enabled = FALSE. */
    pkt_results->is_activity_enabled = ((GENFSK->GTM_CTRL & (GENFSK_GTM_CTRL_GTM_IN_TX_MASK|GENFSK_GTM_CTRL_GTM_IN_RX_MASK)) > 0U ? TRUE: FALSE); 
#else
    status = gGenfskFail_c;
#endif /* RADIO_IS_GEN_3P5 */
    
    return status;
}

/* Copy Rx data to rxPacketBuffer (app buffer) and notify app by calling the registered packetReceivedcallback */
GENFSK_STATIC void GENFSK_TaskRxCopyAndPassData(uint16_t byteCount, bool_t crcValid)
{
    uint64_t tempTime,tmp;
    uint8_t rssi;

    GENFSK_ReadPacketBuffer(0, genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, byteCount);

    rssi = (uint8_t)((GENFSK->XCVR_STS & GENFSK_XCVR_STS_RSSI_MASK) >> GENFSK_XCVR_STS_RSSI_SHIFT);
    tmp = gGenfskTimerOverflow;
    tempTime = tmp | GENFSK->TIMESTAMP;
#ifndef RADIO_IS_GEN_3P5
    /* Twiddling is done by the HW */
    if (genfskLocal[mGenfskActiveInstance].radioConfig.radioMode == gGenfskMsk)
    {
        GENFSK_MskPostProcessing(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                 genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer,
                                 byteCount, (uint8_t) (!((genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer[0] & 1U)==1U)), 1U);

    }
#endif

    /* Packet received OK. */
    if (genfskLocal[mGenfskActiveInstance].packetReceivedcallback != NULL)
    {
        genfskLocal[mGenfskActiveInstance].packetReceivedcallback(genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxPacketBuffer, byteCount, tempTime, rssi, (uint8_t) crcValid);
    }
}

GENFSK_STATIC void GENFSK_TaskRxWithoutCrc(uint16_t byteCount)
{
    if (byteCount <= genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength)
    {
        GENFSK_TaskRxCopyAndPassData(byteCount, FALSE);
    }
    else
    {
        /* Allocated length is smaller than the received length. */
        GENFSK_CallEventNotifyCallback(gGenfskRxEvent, gGenfskRxAllocLengthFail);
    }
}

GENFSK_STATIC void GENFSK_TaskRxWithCrc(uint16_t byteCount)
{
    if ((GENFSK->IRQ_CTRL & GENFSK_IRQ_CTRL_CRC_VALID_MASK) != 0U) /* CRC is valid */
    {
        if (byteCount <= genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength)
        {
            GENFSK_TaskRxCopyAndPassData(byteCount, TRUE);
        }
        else
        {
            /* Packet received OK but allocated length is smaller than the received length. */
            GENFSK_CallEventNotifyCallback(gGenfskRxEvent, gGenfskRxAllocLengthFail);
        }
    }
    else /* CRC is invalid */
    {
        if ((byteCount > genfskLocal[mGenfskActiveInstance].genfskRxLocal.rxMaxPacketLength) ||
            (genfskLocal[mGenfskActiveInstance].crcRecvInvalid == gGenfskCrcSupressInvalid))
        {
            /* Allocated length is smaller than the received length or CRC error reception disabled*/
            GENFSK_CallEventNotifyCallback(gGenfskRxEvent, gGenfskCRCInvalid);
        }
        else
        {
            GENFSK_TaskRxCopyAndPassData(byteCount, FALSE);
        }
    }
}

/* RX event handling. */
static void GENFSK_TaskRx(void)
{
    uint16_t byteCount;

    byteCount = (uint16_t) ((GENFSK->RX_WATERMARK & GENFSK_RX_WATERMARK_BYTE_COUNTER_MASK) >> GENFSK_RX_WATERMARK_BYTE_COUNTER_SHIFT);

#if defined (RADIO_IS_GEN_3P5)
    /* This version of radio as opposed to other, does not account for AddrSize in byteCount.
     * Add it here for backward compatibility of SW API */
    if ((genfskLocal[mGenfskActiveInstance].genfskRegs.packetCfg & GENFSK_PACKET_CFG_AA_PLAYBACK_CNT_MASK) == 0U)
    {
        byteCount += genfskLocal[mGenfskActiveInstance].syncAddrSizeBytes;
    }

    if ((genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_BusyPendingRxTx) && (genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_BusyRxTx))
#endif
    {
        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
        (void) OSA_EventSet(mGenfskTaskEvent, gGenfskMwsReleaseEventFlag_c);
#endif /* gGENFSK_LLMwsControl_c */
    }

    if (genfskLocal[mGenfskActiveInstance].crcEnable == gGenfskCrcDisable)
    {
        /* CRC disabled */
        GENFSK_TaskRxWithoutCrc(byteCount);
    }
    else
    {
        /* CRC enabled */
        GENFSK_TaskRxWithCrc(byteCount);
    }
}

/* TX event handling. */
static void GENFSK_TaskTx(void)
{
#if defined (RADIO_IS_GEN_3P5)
    if ((genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_BusyPendingTxRx) && (genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_BusyTxRx))
#endif
    {
        genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
        (void) OSA_EventSet(mGenfskTaskEvent, gGenfskMwsReleaseEventFlag_c);
#endif /* gGENFSK_LLMwsControl_c */
    }

    GENFSK_CallEventNotifyCallback(gGenfskTxEvent, gGenfskSuccess);
}

/* Error event handling. */
static void GENFSK_TaskErrorEvents(uint32_t ev)
{
    /* H0 Fail event. */
    if ((ev & gGenfskH0FailEventFlag_c) != 0U)
    {
        GENFSK_CallEventNotifyCallback(gGenfskRxEvent, gGenfskH0Fail);
    }

    /* Length Fail event. */
    if ((ev & gGenfskLengthFailEventFlag_c) != 0U)
    {
        GENFSK_CallEventNotifyCallback(gGenfskRxEvent, gGenfskLengthFail);
    }

    /* H1 Fail event. */
    if ((ev & gGenfskH1FailEventFlag_c) != 0U)
    {
        GENFSK_CallEventNotifyCallback(gGenfskRxEvent, gGenfskH1Fail);
    }

    /* RX PLL Unlock event. */
    if ((ev & gGenfskRxPllUnlockEventFlag_c) != 0U)
    {
        GENFSK_CallEventNotifyCallback(gGenfskRxEvent, gGenfskSyncLost);
    }

    /* TX PLL Unlock event. */
    if ((ev & gGenfskTxPllUnlockEventFlag_c) != 0U)
    {
        GENFSK_CallEventNotifyCallback(gGenfskTxEvent, gGenfskSyncLost);
    }
}

static void GENFSK_Task(osaTaskParam_t argument)
{
    osaEventFlags_t osaev = 0;
    uint32_t ev;

    while(TRUE)
    {
        (void)OSA_EventWait(mGenfskTaskEvent, osaEventFlagsAll_c, FALSE, osaWaitForever_c, &osaev);

        ev = osaev;

        /* RX event. */
        if ((ev & gGenfskRxEventFlag_c) != 0U)
        {
#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
#if defined (RADIO_IS_GEN_3P5)
            /* Make sure access to GENFSK HW module is enabled. */
            RADIO_CTRL->LL_CTRL = 0x00000002U;
#endif
#endif
            GENFSK_TaskRx();
        }

        /* TX event. */
        if ((ev & gGenfskTxEventFlag_c) != 0U)
        {
            GENFSK_TaskTx();
        }

        /* Error events: H0 fail, H1 fail, Length fail, PLL unlock. */
        if ((ev & (gGenfskH0FailEventFlag_c | gGenfskH1FailEventFlag_c | gGenfskLengthFailEventFlag_c | gGenfskRxPllUnlockEventFlag_c | gGenfskTxPllUnlockEventFlag_c)) != 0U)
        {
            GENFSK_TaskErrorEvents(ev);
        }

        /* Network Address Match event. */
        if ((ev & gGenfskNwkAddressMatchEventFlag_c) != 0U)
        {
            GENFSK_CallEventNotifyCallback(gGenfskNwkAddressMatch, gGenfskSuccess);
        }

        /* Wake event. */
        if ((ev & gGenfskWakeEventFlag_c) != 0U)
        {
            GENFSK_CallEventNotifyCallback(gGenfskWakeEvent, gGenfskSuccess);
        }

#if (gGENFSK_MwsControl_c == gGENFSK_LLMwsControl_c)
        /* MWS release has been requested through:  GENFSK_AbortAll() or due to error events */
        if ((ev & gGenfskMwsReleaseEventFlag_c) != 0U)
        {
            MWS_Release(gMWS_GENFSK_c);
        }
#endif /* gGENFSK_LLMwsControl_c */

        /* For BM break the while() loop. */
        /* LCOV_EXCL_START */
        if (gUseRtos_c == 0U)
        {
            break;
        }
        /* LCOV_EXCL_STOP */
    }
}

#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
static uint32_t MWS_GENFSK_Callback(mwsEvents_t event)
{
    uint32_t status = gMWS_Success_c;
#if !defined (RADIO_IS_GEN_3P5)
    radio_mode_t radioMode;
#else
    const xcvr_config_t *xcvrConfig = NULL;
    const xcvr_coding_config_t *rbmeConfig = &xcvr_ble_uncoded_config;
#endif


    switch(event)
    {
    case gMWS_Init_c:
        /* Do nothing */
        break;
    case gMWS_Active_c:
#if !defined (RADIO_IS_GEN_3P5)
        GENFSK_GetXcvrConfig(genfskLocal[mGenfskActiveInstance].radioConfig.radioMode, &radioMode);
        (void)XCVR_ChangeMode(radioMode, (data_rate_t)genfskLocal[mGenfskActiveInstance].radioConfig.dataRate);
        /*XCVR_MISC->BLE_ARB_CTRL |= XCVR_CTRL_BLE_ARB_CTRL_BLE_RELINQUISH_MASK;*/
#else
        RADIO_CTRL->LL_CTRL = 0x00000000U;
        BTLE_RF->MISC_CTRL |= BTLE_RF_MISC_CTRL_BLE_RELINQUISH_MASK;

        GENFSK_GetXcvrConfig(genfskLocal[mGenfskActiveInstance].radioConfig.radioMode, genfskLocal[mGenfskActiveInstance].radioConfig.dataRate, &xcvrConfig);
        GENFSK_ChangeMode(mGenfskActiveInstance, &genfskLocal[mGenfskActiveInstance].radioConfig, xcvrConfig, rbmeConfig);
#endif
        GENFSK_UnprotectFromXcvrInterrupt();
        break;
    case gMWS_Idle_c:
        /* Do nothing */
        break;
    case gMWS_Abort_c:
    case gMWS_Release_c:
        /* Enter critical section */
        OSA_InterruptDisable();

        if (genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_NoInit)
        {
#if defined (RADIO_IS_GEN_3P5)
            RADIO_CTRL->LL_CTRL = 0x2U;
#endif
            if (genfskLocal[mGenfskActiveInstance].genfskState != gGENFSK_LL_Idle)
            {
                /* This is a workaround to cope with abort issue during TX warmup, see CONNRF-767 */
                if ((GENFSK->SEQ_STS & GENFSK_SEQ_STS_TX_IN_WARMUP_MASK) != 0x0U)
                {
                    /* If TX is in warmup then wait for warmup to complete before issuing abort to ensure ABORT does not crash TX */
                    uint32_t end_of_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;
                    while ((( XCVR_MISC->XCVR_STATUS & XCVR_MISC_XCVR_STATUS_TSM_COUNT_MASK) >> XCVR_MISC_XCVR_STATUS_TSM_COUNT_SHIFT ) < end_of_wu) {}; /* Wait for TSM TX WU complete */
                }

                GENFSK_AbortAll();
                /* Cancel the sent event (MWS release request) in GENFSK_AbortAll() as we are already in the right state */
                (void) OSA_EventClear(mGenfskTaskEvent, gGenfskMwsReleaseEventFlag_c);
            }

            GENFSK_ProtectFromXcvrInterrupt();
        }

        /* Exit critical section */
        OSA_InterruptEnable();
        break;

    case gMWS_GetInactivityDuration_c:
        /* Default status is 0 (Busy)  */
        if (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_Idle)
        {
            status = 0xFFFFFFFFU;
        }
        break;

    default:
        status = gMWS_InvalidParameter_c;
        break;
    }

    return status;
}
#endif /* (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c) */

