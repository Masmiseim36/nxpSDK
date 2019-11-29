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
#include "MpmInterface.h"
#include "FunctionLib.h"
#include "board.h"

#include "Phy.h"
#include "fsl_xcvr.h"
#include "ifr_radio.h"

#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"
#include "Flash_Adapter.h"

#if gMWS_UseCoexistence_d
#include "MWS.h"
#endif


/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define gPhyHwIndQueueSize_d        (128)
#define mPhyDSM_GuardTime_d         (5) /* DSM_TIME ticks (32.768KHz) */

/*! *********************************************************************************
*************************************************************************************
* Private functions prototype
*************************************************************************************
********************************************************************************** */
#if gPhyUseNeighborTable_d
static int32_t PhyGetIndexOf( uint16_t checksum );
#endif


/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
const  uint8_t gPhyIdlePwrState = gPhyDefaultIdlePwrMode_c;
const  uint8_t gPhyActivePwrState = gPhyPwrIdle_c; /* Do not change! */
static uint32_t mPhyDSMDuration = 0xFFFFF0;
extern uint8_t mXcvrDisallowSleep;

#if gPhyUseNeighborTable_d
/* Limit HW indirect queue size to ~10% */
const uint8_t gPhyIndirectQueueSize_c = gPhyHwIndQueueSize_d/10;
#else
const uint8_t gPhyIndirectQueueSize_c = gPhyHwIndQueueSize_d;
#endif
const uint8_t gPhyHwIndQueueSize_c = gPhyHwIndQueueSize_d;

/* Accept FrameVersion 0 and 1 packets, reject all others (FRM_VER[11:8] = b0011)
 * Accept Beacon, Data and MAC command frame types. */
const uint32_t mDefaultRxFiltering = ZLL_RX_FRAME_FILTER_FRM_VER_FILTER(3) |
                                     ZLL_RX_FRAME_FILTER_CMD_FT_MASK       |
                                     ZLL_RX_FRAME_FILTER_DATA_FT_MASK      |
                                     ZLL_RX_FRAME_FILTER_BEACON_FT_MASK;

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  Initialize the 802.15.4 Radio registers
*
********************************************************************************** */
void PhyHwInit
(
void
)
{
    uint32_t phyReg;
#if gPhyUseNeighborTable_d
    uint32_t i;
#endif

    XCVR_Init( ZIGBEE_MODE, DR_500KBPS );
    
    XCVR_SetXtalTrim( (uint8_t)gHardwareParameters.xtalTrim );
    
    /* Enable 16 bit mode for TC2 - TC2 prime EN, disable all timers,
       enable AUTOACK, mask all interrupts */
    ZLL->PHY_CTRL = (gCcaCCA_MODE1_c << ZLL_PHY_CTRL_CCATYPE_SHIFT) |
                   ZLL_PHY_CTRL_TC2PRIME_EN_MASK    |
                   ZLL_PHY_CTRL_TSM_MSK_MASK        |
                   ZLL_PHY_CTRL_WAKE_MSK_MASK       |
                   ZLL_PHY_CTRL_CRC_MSK_MASK        |
                   ZLL_PHY_CTRL_PLL_UNLOCK_MSK_MASK |
                   ZLL_PHY_CTRL_FILTERFAIL_MSK_MASK |
                   ZLL_PHY_CTRL_RX_WMRK_MSK_MASK    |
                   ZLL_PHY_CTRL_CCAMSK_MASK         |
                   ZLL_PHY_CTRL_RXMSK_MASK          |
                   ZLL_PHY_CTRL_TXMSK_MASK          |
                   ZLL_PHY_CTRL_SEQMSK_MASK         |
                   ZLL_PHY_CTRL_AUTOACK_MASK        |
                   ZLL_PHY_CTRL_TRCV_MSK_MASK;

    /* Clear all PP IRQ bits to avoid unexpected interrupts immediately after init
       disable all timer interrupts */
    ZLL->IRQSTS = ZLL->IRQSTS;

    /* Enable Source Addresing Match module */
    ZLL->SAM_CTRL |= ZLL_SAM_CTRL_SAP0_EN_MASK;
#if (gMpmIncluded_d)
    ZLL->SAM_CTRL &= ~ZLL_SAM_CTRL_SAP1_START_MASK;
    ZLL->SAM_CTRL |= ZLL_SAM_CTRL_SAP1_EN_MASK | ZLL_SAM_CTRL_SAP1_START(gPhyHwIndQueueSize_d/2);
#if gPhyUseNeighborTable_d
    ZLL->SAM_CTRL &= ~(ZLL_SAM_CTRL_SAA0_START_MASK | ZLL_SAM_CTRL_SAA1_START_MASK);
    ZLL->SAM_CTRL |= ZLL_SAM_CTRL_SAA0_EN_MASK | 
                     ZLL_SAM_CTRL_SAA1_EN_MASK | 
                     ZLL_SAM_CTRL_SAA0_START(gPhyIndirectQueueSize_c/2) |
                     ZLL_SAM_CTRL_SAA1_START(gPhyHwIndQueueSize_d/2 + gPhyIndirectQueueSize_c/2);
#endif

#elif gPhyUseNeighborTable_d
    ZLL->SAM_CTRL &= ~ZLL_SAM_CTRL_SAA0_START_MASK;
    ZLL->SAM_CTRL |= ZLL_SAM_CTRL_SAA0_EN_MASK | ZLL_SAM_CTRL_SAA0_START(gPhyIndirectQueueSize_c);
#endif

    /* Clear HW indirect queue */
    ZLL->SAM_TABLE |= ZLL_SAM_TABLE_INVALIDATE_ALL_MASK;
#if gPhyUseNeighborTable_d
    for( i=0; i<gPhyHwIndQueueSize_d; i++ )
    {
        /* Invalidate current index and checksum */
        PhyPp_RemoveFromIndirect(i, 0);
    }
#endif

    /* Frame Filtering */
    ZLL->RX_FRAME_FILTER = mDefaultRxFiltering;

    /* Set prescaller to obtain 1 symbol (16us) timebase */
    ZLL->TMR_PRESCALE = 0x05;

    /* Set CCA threshold to -75 dBm */
    ZLL->CCA_LQI_CTRL &= ~ZLL_CCA_LQI_CTRL_CCA1_THRESH_MASK;
    ZLL->CCA_LQI_CTRL |= ZLL_CCA_LQI_CTRL_CCA1_THRESH(0xB5);

    /* Adjust ACK delay to fulfill the 802.15.4 turnaround requirements */
    ZLL->ACKDELAY &= ~ZLL_ACKDELAY_ACKDELAY_MASK;
    ZLL->ACKDELAY |= ZLL_ACKDELAY_ACKDELAY(-8);
    
    /* Adjust LQI compensation */
    ZLL->CCA_LQI_CTRL &= ~ZLL_CCA_LQI_CTRL_LQI_OFFSET_COMP_MASK;
    ZLL->CCA_LQI_CTRL |= ZLL_CCA_LQI_CTRL_LQI_OFFSET_COMP(96);

    /* Enable the RxWatermark IRQ and FilterFail IRQ */
    ZLL->PHY_CTRL &= ~(ZLL_PHY_CTRL_RX_WMRK_MSK_MASK | ZLL_PHY_CTRL_FILTERFAIL_MSK_MASK);
    /* Set default Rx watermark level */
    ZLL->RX_WTR_MARK = gPhyDefaultRxWatermarkLevel_d;

    /* Set default channels */
    PhyPlmeSetCurrentChannelRequest(0x0B, 0); /* 2405 MHz */
    PhyPlmeSetCurrentChannelRequest(0x0B, 1); /* 2405 MHz */

    /* Set the default power level */
    PhyPlmeSetPwrLevelRequest(gPhyDefaultTxPowerLevel_d);

    /* DSM settings */
    phyReg = (RSIM->RF_OSC_CTRL & RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL_MASK) >> 
             RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL_SHIFT;
    phyReg = (1024U << phyReg) / (CORE_CLOCK_FREQ / 32768) + 1;
    RSIM->DSM_OSC_OFFSET = phyReg;

    /* Install PHY ISR */
    PHY_InstallIsr();
}

/*! *********************************************************************************
* \brief  Aborts the current sequence and force the radio to IDLE
*
********************************************************************************** */
void PhyAbort
(
void
)
{
    //ProtectFromXcvrInterrupt();
    OSA_InterruptDisable();
    
    /* Mask SEQ interrupt */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_SEQMSK_MASK;
    /* Disable timer trigger (for scheduled XCVSEQ) */
    if( ZLL->PHY_CTRL & ZLL_PHY_CTRL_TMRTRIGEN_MASK )
    {
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMRTRIGEN_MASK;
        /* give the FSM enough time to start if it was triggered */
        while( (XCVR_MISC->XCVR_CTRL & XCVR_CTRL_XCVR_STATUS_TSM_COUNT_MASK) == 0) {}
    }

    /* If XCVR is not idle, abort current SEQ */
    if( ZLL->PHY_CTRL & ZLL_PHY_CTRL_XCVSEQ_MASK )
    {
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_XCVSEQ_MASK;
        /* wait for Sequence Idle (if not already) */
        while( ZLL->SEQ_STATE & ZLL_SEQ_STATE_SEQ_STATE_MASK ) {}
    }

#if gMWS_UseCoexistence_d
    MWS_CoexistenceReleaseAccess();
#endif

    /* Stop timers */
    ZLL->PHY_CTRL &= ~(ZLL_PHY_CTRL_TMR2CMP_EN_MASK |
                       ZLL_PHY_CTRL_TMR3CMP_EN_MASK |
                       ZLL_PHY_CTRL_TC3TMOUT_MASK );
    /* clear all PP IRQ bits to avoid unexpected interrupts( do not change TMR1 and TMR4 IRQ status ) */
    ZLL->IRQSTS &= ~(ZLL_IRQSTS_TMR1IRQ_MASK | ZLL_IRQSTS_TMR4IRQ_MASK);

    PhyIsrPassRxParams(NULL);

    ZLL->RX_WTR_MARK = gPhyDefaultRxWatermarkLevel_d;

    if( mXcvrDisallowSleep )
    {
        mXcvrDisallowSleep = 0;
        PWR_AllowXcvrToSleep();
    }

    //UnprotectFromXcvrInterrupt();
    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Get the state of the ZLL
*
* \return  uint8_t state
*
********************************************************************************** */
uint8_t PhyPpGetState
(
void
)
{
    return (ZLL->PHY_CTRL & ZLL_PHY_CTRL_XCVSEQ_MASK) >> ZLL_PHY_CTRL_XCVSEQ_SHIFT;
}

/*! *********************************************************************************
* \brief  Set the value of the MAC PanId
*
* \param[in]  pPanId
* \param[in]  pan
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPpSetPanId
(
uint8_t *pPanId,
uint8_t pan
)
{
    uint16_t value;

#ifdef PHY_PARAMETERS_VALIDATION
    if(NULL == pPanId)
    {
        return gPhyInvalidParameter_c;
    }
#endif /* PHY_PARAMETERS_VALIDATION */
    
    /* Avoid unaligned memory access issues */
    FLib_MemCpy(&value, pPanId, sizeof(value));

    if( 0 == pan )
    {
        ZLL->MACSHORTADDRS0 &= ~ZLL_MACSHORTADDRS0_MACPANID0_MASK;
        ZLL->MACSHORTADDRS0 |= ZLL_MACSHORTADDRS0_MACPANID0(value);
    }
    else
    {
        ZLL->MACSHORTADDRS1 &= ~ZLL_MACSHORTADDRS1_MACPANID1_MASK;
        ZLL->MACSHORTADDRS1 |= ZLL_MACSHORTADDRS1_MACPANID1(value);
    }

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  Set the value of the MAC Short Address
*
* \param[in]  pShortAddr
* \param[in]  pan
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPpSetShortAddr
(
uint8_t *pShortAddr,
uint8_t pan
)
{
    uint16_t value;

#ifdef PHY_PARAMETERS_VALIDATION
    if(NULL == pShortAddr)
    {
        return gPhyInvalidParameter_c;
    }
#endif /* PHY_PARAMETERS_VALIDATION */

    /* Avoid unaligned memory access issues */
    FLib_MemCpy(&value, pShortAddr, sizeof(value));
    
    if( pan == 0 )
    {
        ZLL->MACSHORTADDRS0 &= ~ZLL_MACSHORTADDRS0_MACSHORTADDRS0_MASK;
        ZLL->MACSHORTADDRS0 |= ZLL_MACSHORTADDRS0_MACSHORTADDRS0(value);
    }
    else
    {
        ZLL->MACSHORTADDRS1 &= ~ZLL_MACSHORTADDRS1_MACSHORTADDRS1_MASK;
        ZLL->MACSHORTADDRS1 |= ZLL_MACSHORTADDRS1_MACSHORTADDRS1(value);
    }

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  Set the value of the MAC extended address
*
* \param[in]  pLongAddr
* \param[in]  pan
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPpSetLongAddr
(
uint8_t *pLongAddr,
uint8_t pan
)
{
    uint32_t addrLo;
    uint32_t addrHi;

#ifdef PHY_PARAMETERS_VALIDATION
    if(NULL == pLongAddr)
    {
        return gPhyInvalidParameter_c;
    }
#endif /* PHY_PARAMETERS_VALIDATION */

    /* Avoid unaligned memory access issues */
    FLib_MemCpy(&addrLo, pLongAddr, sizeof(addrLo));
    pLongAddr += sizeof(addrLo);
    FLib_MemCpy(&addrHi, pLongAddr, sizeof(addrHi));

    if( 0 == pan )
    {
        ZLL->MACLONGADDRS0_LSB = addrLo;
        ZLL->MACLONGADDRS0_MSB = addrHi;
    }
    else
    {
        ZLL->MACLONGADDRS1_LSB = addrLo;
        ZLL->MACLONGADDRS1_MSB = addrHi;
    }

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  Set the MAC PanCoordinator role
*
* \param[in]  macRole
* \param[in]  pan
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPpSetMacRole
(
  bool_t macRole,
  uint8_t pan
)
{
    uint8_t panCoord;

    if(gMacRole_PanCoord_c == macRole)
    {
        panCoord = 1;
    }
    else
    {
        panCoord = 0;
    }

    if( 0 == pan )
    {
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_PANCORDNTR0_MASK;
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_PANCORDNTR0(panCoord);
    }
    else
    {
        ZLL->DUAL_PAN_CTRL &= ~ZLL_DUAL_PAN_CTRL_PANCORDNTR1_MASK;
        ZLL->DUAL_PAN_CTRL |= ZLL_DUAL_PAN_CTRL_PANCORDNTR1(panCoord);
    }

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  Set the PHY in Promiscuous mode
*
* \param[in]  mode
*
********************************************************************************** */
void PhyPpSetPromiscuous(bool_t mode)
{
    if( mode )
    {
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_PROMISCUOUS_MASK;
        /* FRM_VER[11:8] = b1111. Any Frame version/type accepted */
        ZLL->RX_FRAME_FILTER |= (ZLL_RX_FRAME_FILTER_FRM_VER_FILTER_MASK  |
                                 ZLL_RX_FRAME_FILTER_EXTENDED_FT_MASK     |
                                 ZLL_RX_FRAME_FILTER_MULTIPURPOSE_FT_MASK |
                                 ZLL_RX_FRAME_FILTER_LLDN_FT_MASK         |
                                 ZLL_RX_FRAME_FILTER_CMD_FT_MASK          |
                                 ZLL_RX_FRAME_FILTER_DATA_FT_MASK         |
                                 ZLL_RX_FRAME_FILTER_BEACON_FT_MASK       |
                                 ZLL_RX_FRAME_FILTER_ACK_FT_MASK          |
                                 ZLL_RX_FRAME_FILTER_NS_FT_MASK);
    }
    else
    {
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_PROMISCUOUS_MASK;
        /* FRM_VER[11:8] = b0011. Accept FrameVersion 0 and 1 packets, reject all others */
        /* Beacon, Data and MAC command frame types accepted */
        ZLL->RX_FRAME_FILTER = mDefaultRxFiltering;
    }
}

/*! *********************************************************************************
* \brief  Set the PHY in ActivePromiscuous mode
*
* \param[in]  state
*
********************************************************************************** */
void PhySetActivePromiscuous(bool_t state)
{
    if( state )
    {
        if( ZLL->PHY_CTRL & ZLL_PHY_CTRL_PROMISCUOUS_MASK )
        {
            /* Disable Promiscuous mode */
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_PROMISCUOUS_MASK;
            ZLL->RX_FRAME_FILTER |= ZLL_RX_FRAME_FILTER_ACTIVE_PROMISCUOUS_MASK;
        }
    }
    else
    {
        if( ZLL->RX_FRAME_FILTER & ZLL_RX_FRAME_FILTER_ACTIVE_PROMISCUOUS_MASK )
        {
            ZLL->RX_FRAME_FILTER &= ~ZLL_RX_FRAME_FILTER_ACTIVE_PROMISCUOUS_MASK;
            /* Enable Promiscuous mode */
            ZLL->PHY_CTRL |= ZLL_PHY_CTRL_PROMISCUOUS_MASK;
        }
    }
}

/*! *********************************************************************************
* \brief  Get the state of the ActivePromiscuous mode
*
* \return  bool_t state
*
********************************************************************************** */
bool_t PhyGetActivePromiscuous
(
void
)
{
    return !!(ZLL->RX_FRAME_FILTER & ZLL_RX_FRAME_FILTER_ACTIVE_PROMISCUOUS_MASK);
}

/*! *********************************************************************************
* \brief  Set the state of the SAM HW module
*
* \param[in]  state
*
********************************************************************************** */
void PhyPpSetSAMState
(
  bool_t state
)
{
    ZLL->SAM_CTRL &= ~ZLL_SAM_CTRL_SAP0_EN_MASK;
    ZLL->SAM_CTRL |= ZLL_SAM_CTRL_SAP0_EN(state);
#if gMpmIncluded_d
    ZLL->SAM_CTRL &= ~ZLL_SAM_CTRL_SAP1_EN_MASK;
    ZLL->SAM_CTRL |= ZLL_SAM_CTRL_SAP1_EN(state);
#endif
}

/*! *********************************************************************************
* \brief  Add a new element to the PHY indirect queue
*
* \param[in]  index
* \param[in]  checkSum
* \param[in]  instanceId
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPp_IndirectQueueInsert
(
uint8_t  index,
uint16_t checkSum,
instanceId_t instanceId
)
{
    uint32_t temp;
    phyStatus_t status = gPhySuccess_c;

    instanceId = instanceId;
    if( index >= gPhyHwIndQueueSize_d )
    {
        status = gPhyInvalidParameter_c;
    }
    else
    {
        temp = ZLL->SAM_TABLE & (ZLL_SAM_TABLE_ACK_FRM_PND_CTRL_MASK | ZLL_SAM_TABLE_ACK_FRM_PND_MASK);
        temp |= ((uint32_t)index << ZLL_SAM_TABLE_SAM_INDEX_SHIFT) |
                ((uint32_t)checkSum << ZLL_SAM_TABLE_SAM_CHECKSUM_SHIFT) |
                 ZLL_SAM_TABLE_SAM_INDEX_WR_MASK |
                 ZLL_SAM_TABLE_SAM_INDEX_EN_MASK;
        ZLL->SAM_TABLE = temp;
    }

    return status;
}

/*! *********************************************************************************
* \brief  Remove an eleent from the PHY indirect queue
*
* \param[in]  index
* \param[in]  instanceId
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPp_RemoveFromIndirect(uint8_t index,instanceId_t instanceId)
{
    phyStatus_t status = gPhySuccess_c;
    uint32_t temp;

    if( index >= gPhyHwIndQueueSize_d )
    {
        status = gPhyInvalidParameter_c;
    }
    else
    {
        temp = ZLL->SAM_TABLE & (ZLL_SAM_TABLE_ACK_FRM_PND_CTRL_MASK | ZLL_SAM_TABLE_ACK_FRM_PND_MASK);
        temp |= ((uint32_t)0xFFFF << ZLL_SAM_TABLE_SAM_CHECKSUM_SHIFT) |
                ((uint32_t)index << ZLL_SAM_TABLE_SAM_INDEX_SHIFT) |
                ZLL_SAM_TABLE_SAM_INDEX_INV_MASK |
                ZLL_SAM_TABLE_SAM_INDEX_WR_MASK;
        ZLL->SAM_TABLE = temp;
    }

    return status;
}

/*! *********************************************************************************
* \brief  Return TRUE if the received packet is a PollRequest
*
* \return  bool_t
*
********************************************************************************** */
bool_t PhyPpIsPollIndication
(
void
)
{
    return !!(ZLL->IRQSTS & ZLL_IRQSTS_PI_MASK);
}

/*! *********************************************************************************
* \brief  Return the state of the FP bit of the received ACK
*
* \return  bool_t
*
********************************************************************************** */
bool_t PhyPpIsRxAckDataPending
(
void
)
{
    return !!(ZLL->IRQSTS & ZLL_IRQSTS_RX_FRM_PEND_MASK);
}

/*! *********************************************************************************
* \brief  Return TRUE if there is data pending for the Poling Device
*
* \return  bool_t
*
********************************************************************************** */
bool_t PhyPpIsTxAckDataPending
(
void
)
{
    bool_t status;
    
    if( ZLL->SAM_CTRL & (ZLL_SAM_CTRL_SAP0_EN_MASK | ZLL_SAM_CTRL_SAP1_EN_MASK) )
    {
        status = !!(ZLL->IRQSTS & ZLL_IRQSTS_SRCADDR_MASK);
    }
    else
    {
        status = !!(ZLL->SAM_TABLE & ZLL_SAM_TABLE_ACK_FRM_PND_MASK);
    }
    
    return status;
}

/*! *********************************************************************************
* \brief  Set the state of the FP bit of an outgoing ACK frame
*
* \param[in]  FP  the state of the FramePending bit
*
********************************************************************************** */
void PhyPpSetFpManually
(
  bool_t FP
)
{
    /* Disable the Source Address Matching feature and set FP manually */
    ZLL->SAM_TABLE = ZLL_SAM_TABLE_ACK_FRM_PND_CTRL_MASK | ZLL_SAM_TABLE_ACK_FRM_PND(FP);
}

/*! *********************************************************************************
* \brief  Set the value of the CCA threshold
*
* \param[in]  ccaThreshold
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPpSetCcaThreshold
(
uint8_t ccaThreshold
)
{
    ZLL->CCA_LQI_CTRL &= ~ZLL_CCA_LQI_CTRL_CCA1_THRESH_MASK;
    ZLL->CCA_LQI_CTRL |= ZLL_CCA_LQI_CTRL_CCA1_THRESH(ccaThreshold);
    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function will set the value for the FAD threshold
*
* \param[in]  FADThreshold   the FAD threshold
*
* \return  phyStatus_t
*
********************************************************************************** */
uint8_t PhyPlmeSetFADThresholdRequest(uint8_t FADThreshold)
{
    XCVR_ZBDEM->FAD_THR = FADThreshold;
    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function will enable/disable the FAD
*
* \param[in]  state   the state of the FAD
*
* \return  phyStatus_t
*
********************************************************************************** */
uint8_t PhyPlmeSetFADStateRequest(bool_t state)
{
    XCVR_MISC->FAD_CTRL &= ~(XCVR_CTRL_FAD_CTRL_FAD_EN_MASK | XCVR_CTRL_FAD_CTRL_FAD_NOT_GPIO_MASK);
    XCVR_TSM->TIMING49  &= ~(XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_MASK);
    XCVR_TSM->TIMING50  &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK);
    
    if (state)
    {
        XCVR_MISC->FAD_CTRL |= XCVR_CTRL_FAD_CTRL_FAD_EN(1) | XCVR_CTRL_FAD_CTRL_FAD_NOT_GPIO(0x0F);
        XCVR_TSM->TIMING49 |= XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI(1);
        XCVR_TSM->TIMING50 |= XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI(1);
    }
    else
    {
        XCVR_MISC->FAD_CTRL |= XCVR_CTRL_FAD_CTRL_FAD_EN(0) | XCVR_CTRL_FAD_CTRL_FAD_NOT_GPIO(0);
        XCVR_TSM->TIMING49 |= XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI(0xFF);
        XCVR_TSM->TIMING50 |= XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI(0xFF);
    }

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function will set the LQI mode
*
* \return  uint8_t
*
********************************************************************************** */
uint8_t PhyPlmeSetLQIModeRequest(uint8_t lqiMode)
{
    ZLL->CCA_LQI_CTRL &= ~ZLL_CCA_LQI_CTRL_CCA3_AND_NOT_OR_MASK;
    ZLL->CCA_LQI_CTRL |= ZLL_CCA_LQI_CTRL_CCA3_AND_NOT_OR(lqiMode>0);

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function will return the RSSI level
*
* \return  uint8_t
*
********************************************************************************** */
uint8_t PhyPlmeGetRSSILevelRequest(void)
{
    return (ZLL->LQI_AND_RSSI & ZLL_LQI_AND_RSSI_RSSI_MASK) >> ZLL_LQI_AND_RSSI_RSSI_SHIFT;
}

/*! *********************************************************************************
* \brief  This function will enable/disable the ANTX
*
* \param[in]  state   the state of the ANTX
*
* \return  phyStatus_t
*
********************************************************************************** */
uint8_t PhyPlmeSetANTXStateRequest(bool_t state)
{
    XCVR_MISC->FAD_CTRL &= ~XCVR_CTRL_FAD_CTRL_ANTX_MASK;
    XCVR_MISC->FAD_CTRL |= XCVR_CTRL_FAD_CTRL_ANTX(state);

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief Invert the logic of the ANT pads
*
* \param[in] invAntA - invert the ANT_A pad
* \param[in] invAntB - invert the ANT_A pad
* \param[in] invTx   - invert the ANT_TX pad
* \param[in] invRx   - invert the ANT_RX pad
*
* \return gPhySuccess
*
********************************************************************************** */
uint8_t PhyPlmeSetANTPadInvertedRequest(bool_t invAntA, bool_t invAntB, bool_t invTx, bool_t invRx)
{
    uint32_t settings = 0;

    XCVR_MISC->FAD_CTRL &= ~XCVR_CTRL_FAD_CTRL_ANTX_POL_MASK;
    
    if( invAntA ) 
    {
        settings |= (1 << 0 );
    }
    if( invAntB ) 
    {
        settings |= (1 << 1 );
    }
    if( invTx )   
    {
        settings |= (1 << 2 );
    }
    if( invRx )   
    {
        settings |= (1 << 3 );
    }
    
    XCVR_MISC->FAD_CTRL |= XCVR_CTRL_FAD_CTRL_ANTX_POL(settings);

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief Enable the ANT pads
*
* \param[in] antAB_on - 
* \param[in] rxtxSwitch_on - 
*
* \return gPhySuccess
*
********************************************************************************** */
uint8_t PhyPlmeSetANTPadStateRequest(bool_t antAB_on, bool_t rxtxSwitch_on)
{
    uint32_t settings = 0;

    XCVR_MISC->FAD_CTRL &= ~XCVR_CTRL_FAD_CTRL_ANTX_EN_MASK;

    if( antAB_on ) 
    {
        settings |= (1 << 1 );
    }

    if( rxtxSwitch_on ) 
    {
        settings |= (1 << 0 );
    }

    XCVR_MISC->FAD_CTRL |= XCVR_CTRL_FAD_CTRL_ANTX_EN(settings);
    
    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function will retrn the state of the ANTX
*
* \return  uint8_t
*
********************************************************************************** */
uint8_t PhyPlmeGetANTXStateRequest(void)
{
    return (XCVR_MISC->FAD_CTRL & XCVR_CTRL_FAD_CTRL_ANTX_MASK) >> XCVR_CTRL_FAD_CTRL_ANTX_SHIFT;
}

/*! *********************************************************************************
* \brief  Set the state of the Dual Pan Auto mode
*
* \param[in]  mode TRUE/FALSE
*
********************************************************************************** */
void PhyPpSetDualPanAuto
(
bool_t mode
)
{
    ZLL->DUAL_PAN_CTRL &= ~ZLL_DUAL_PAN_CTRL_DUAL_PAN_AUTO_MASK;
    ZLL->DUAL_PAN_CTRL |= ZLL_DUAL_PAN_CTRL_DUAL_PAN_AUTO(mode);
}

/*! *********************************************************************************
* \brief  Get the state of the Dual Pan Auto mode
*
* \return  bool_t state
*
********************************************************************************** */
bool_t PhyPpGetDualPanAuto
(
void
)
{
    return !!(ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_DUAL_PAN_AUTO_MASK);
}

/*! *********************************************************************************
* \brief  Set the dwell for the Dual Pan Auto mode
*
* \param[in]  dwell
*
********************************************************************************** */
void PhyPpSetDualPanDwell
(
uint8_t dwell
)
{
    ZLL->DUAL_PAN_CTRL &= ~ZLL_DUAL_PAN_CTRL_DUAL_PAN_DWELL_MASK;
    ZLL->DUAL_PAN_CTRL |= ZLL_DUAL_PAN_CTRL_DUAL_PAN_DWELL(dwell);
}

/*! *********************************************************************************
* \brief  Get the dwell for the Dual Pan Auto mode
*
* \return  uint8_t PAN dwell
*
********************************************************************************** */
uint8_t PhyPpGetDualPanDwell
(
void
)
{
    return (ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_DUAL_PAN_DWELL_MASK) >> ZLL_DUAL_PAN_CTRL_DUAL_PAN_DWELL_SHIFT;
}

/*! *********************************************************************************
* \brief  Get the remeining time before a PAN switch occures
*
* \return  uint8_t remaining time
*
********************************************************************************** */
uint8_t PhyPpGetDualPanRemain
(
void
)
{
    return (ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_DUAL_PAN_REMAIN_MASK) >> ZLL_DUAL_PAN_CTRL_DUAL_PAN_REMAIN_SHIFT;
}

/*! *********************************************************************************
* \brief  Set the current active Nwk
*
* \param[in]  nwk index of the nwk
*
********************************************************************************** */
void PhyPpSetDualPanActiveNwk
(
uint8_t nwk
)
{
    ZLL->DUAL_PAN_CTRL &= ~ZLL_DUAL_PAN_CTRL_ACTIVE_NETWORK_MASK;
    ZLL->DUAL_PAN_CTRL |= ZLL_DUAL_PAN_CTRL_ACTIVE_NETWORK(nwk);
}

/*! *********************************************************************************
* \brief  Return the index of the Acive PAN
*
* \return  uint8_t index
*
********************************************************************************** */
uint8_t PhyPpGetDualPanActiveNwk
(
void
)
{
    return !!(ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_ACTIVE_NETWORK_MASK);
}

/*! *********************************************************************************
* \brief  Returns the PAN bitmask for the last Rx packet.
*         A packet can be received on multiple PANs
*
* \return  uint8_t bitmask
*
********************************************************************************** */
uint8_t PhyPpGetPanOfRxPacket(void)
{
  uint8_t PanBitMask = 0;

  if( ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_DUAL_PAN_AUTO_MASK )
  {
      if( ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_RECD_ON_PAN0_MASK )
      {
          PanBitMask |= (1<<0);
      }

      if( ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_RECD_ON_PAN1_MASK )
      {
          PanBitMask |= (1<<1);
      }
  }
  else
  {
      if(ZLL->DUAL_PAN_CTRL & ZLL_DUAL_PAN_CTRL_ACTIVE_NETWORK_MASK )
      {
          PanBitMask |= (1<<1);
      }
      else
      {
          PanBitMask |= (1<<0);
      }
	  
  }

  return PanBitMask;
}

/*! *********************************************************************************
* \brief  Get the indirect queue level at which the HW queue will be split between PANs
*
* \return  uint8_t level
*
********************************************************************************** */
uint8_t PhyPpGetDualPanSamLvl(void)
{
    return (ZLL->SAM_CTRL & ZLL_SAM_CTRL_SAP1_START_MASK) >> ZLL_SAM_CTRL_SAP1_START_SHIFT;
}

/*! *********************************************************************************
* \brief  Set the indirect queue level at which the HW queue will be split between PANs
*
* \param[in]  level
*
********************************************************************************** */
void PhyPpSetDualPanSamLvl( uint8_t level )
{
    ZLL->SAM_CTRL &= ~ZLL_SAM_CTRL_SAA1_START_MASK;
    ZLL->SAM_CTRL |= ZLL_SAM_CTRL_SAP1_START(level);

    if( level )
    {
        ZLL->SAM_CTRL |= ZLL_SAM_CTRL_SAP1_EN_MASK;
    }
    else
    {
        ZLL->SAM_CTRL &= ~ZLL_SAM_CTRL_SAP1_EN_MASK;
    }
}

/*! *********************************************************************************
* \brief  This function compute the hash code for an 802.15.4 device
*
* \param[in]  pAddr     Pointer to an 802.15.4 address
* \param[in]  addrMode  The 802.15.4 addressing mode
* \param[in]  PanId     The 802.15.2 PAN Id
*
* \return  hash code
*
********************************************************************************** */
uint16_t PhyGetChecksum(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId)
{
    uint16_t checksum;
    
    /* Short address */
    checksum  = PanId;
    checksum += *pAddr++;
    checksum += (uint16_t)((uint16_t)(*pAddr++) << 8);
    
    if( addrMode == 3 )    
    {
        /* Extended address */
        checksum += *pAddr++;
        checksum += (uint16_t)((uint16_t)(*pAddr++) << 8);
        checksum += *pAddr++;
        checksum += (uint16_t)((uint16_t)(*pAddr++) << 8);
        checksum += *pAddr++;
        checksum += (uint16_t)((uint16_t)(*pAddr++) << 8);
    }

    return checksum;
}

/*! *********************************************************************************
* \brief  This function adds an 802.15.4 device to the neighbor table.
*         If a polling device is not in the neighbor table, the ACK will have FP=1
*
* \param[in]  pAddr     Pointer to an 802.15.4 address
* \param[in]  addrMode  The 802.15.4 addressing mode
* \param[in]  PanId     The 802.15.2 PAN Id
*
********************************************************************************** */
uint8_t PhyAddToNeighborTable(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId)
{
#if gPhyUseNeighborTable_d
    int32_t index;
    uint32_t phyReg;
    uint16_t checksum = PhyGetChecksum(pAddr, addrMode, PanId);
    uint32_t min = (ZLL->SAM_CTRL & ZLL_SAM_CTRL_SAA0_START_MASK) >> ZLL_SAM_CTRL_SAA0_START_SHIFT;
    uint32_t max = gPhyHwIndQueueSize_d;

    if( PhyGetIndexOf(checksum) != -1 )
    {
        /* Device is allready in the table */
        return 0;
    }

    /* Find first free index */
    phyReg = ZLL->SAM_TABLE & (ZLL_SAM_TABLE_ACK_FRM_PND_CTRL_MASK | ZLL_SAM_TABLE_ACK_FRM_PND_MASK);
    ZLL->SAM_TABLE = phyReg | ZLL_SAM_TABLE_FIND_FREE_IDX_MASK;

    while (ZLL->SAM_TABLE & ZLL_SAM_TABLE_SAM_BUSY_MASK)
    {
    }

    index = (ZLL->SAM_FREE_IDX & ZLL_SAM_FREE_IDX_SAA0_1ST_FREE_IDX_MASK) >> ZLL_SAM_FREE_IDX_SAA0_1ST_FREE_IDX_SHIFT;

    if( (index >= min) && (index < max) )
    {
        PhyPp_IndirectQueueInsert((uint8_t)index, checksum, 0);
        return 0;
    }
#endif
    return 1;
}

/*! *********************************************************************************
* \brief  This function removes an 802.15.4 device to the neighbor table.
*         If a polling device is not in the neighbor table, the ACK will have FP=1
*
* \param[in]  pAddr     Pointer to an 802.15.4 address
* \param[in]  addrMode  The 802.15.4 addressing mode
* \param[in]  PanId     The 802.15.2 PAN Id
*
********************************************************************************** */
uint8_t PhyRemoveFromNeighborTable(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId)
{
#if gPhyUseNeighborTable_d
    uint16_t checksum;
    int32_t  index;

    checksum = PhyGetChecksum(pAddr, addrMode, PanId);
    index    = PhyGetIndexOf(checksum);
    
    if( index != -1 )
    {
        /* Invalidate current index and checksum */
        PhyPp_RemoveFromIndirect(index,0);
        return 0;
    }
#endif
    return 1;
}

/*! *********************************************************************************
* \brief  Invalidate all entries from the Neighbour Table
*
********************************************************************************** */
void PhyResetNeighborTable(void)
{
#if gPhyUseNeighborTable_d
    uint32_t min = (ZLL->SAM_CTRL & ZLL_SAM_CTRL_SAA0_START_MASK) >> ZLL_SAM_CTRL_SAA0_START_SHIFT;
    uint32_t max = gPhyHwIndQueueSize_d;
    uint32_t i;
    
    for (i=min; i< max; i++)
    {
        PhyPp_RemoveFromIndirect(i,0);
    }
#endif
}

/*! *********************************************************************************
* \brief  This function checks if an 802.15.4 device is in the neighbor table.
*         If a polling device is not in the neighbor table, the ACK will have FP=1
*
* \param[in]  checksum     hash code generated by PhyGetChecksum()
*
* \return  TRUE if the device is present in the neighbor table, FALSE if not.
*
********************************************************************************** */
bool_t PhyCheckNeighborTable(uint16_t checksum)
{
#if gPhyUseNeighborTable_d
    if( PhyGetIndexOf(checksum) != -1 )
    {
        return TRUE;
    }
#endif
    return FALSE;
}

/*! *********************************************************************************
* \brief  This function returns the table index of the specified checksum.
*
* \param[in]  checksum     hash code generated by PhyGetChecksum()
*
* \return  The table index where the checksum was found or
*          -1 if no entry was found with the specified chacksum
*          
*
********************************************************************************** */
#if gPhyUseNeighborTable_d
static int32_t PhyGetIndexOf( uint16_t checksum )
{
    uint32_t i, phyReg;
    uint32_t start = (ZLL->SAM_CTRL & ZLL_SAM_CTRL_SAA0_START_MASK) >> ZLL_SAM_CTRL_SAA0_START_SHIFT;
    uint32_t stop = gPhyHwIndQueueSize_d;
    
    for(i=start; i<stop; i++)
    {
        /* Set the index value */
        phyReg = ZLL->SAM_TABLE & (ZLL_SAM_TABLE_ACK_FRM_PND_CTRL_MASK | ZLL_SAM_TABLE_ACK_FRM_PND_MASK);
        ZLL->SAM_TABLE = phyReg | (i << ZLL_SAM_TABLE_SAM_INDEX_SHIFT);

        /* Read checksum located at the specified index */
        phyReg = (ZLL->SAM_TABLE & ZLL_SAM_TABLE_SAM_CHECKSUM_MASK) >> ZLL_SAM_TABLE_SAM_CHECKSUM_SHIFT;

        if( phyReg == checksum )
        {
            return i;
        }
    }
    
    return -1;
}
#endif

/*! *********************************************************************************
* \brief  Change the XCVR DSM duration
*
* \param[in]  duration  the new XCVR sleep duration
*
********************************************************************************** */
void PhyPlmeSetDSMDuration(uint32_t duration)
{
    /* Minimum DSM duration */
    mPhyDSMDuration = RSIM->DSM_OSC_OFFSET + mPhyDSM_GuardTime_d;
    
    if( duration > mPhyDSMDuration )
    {
        mPhyDSMDuration = duration;
    }
}

/*! *********************************************************************************
* \brief  Change the XCVR power state
*
* \param[in]  state  the new XCVR power state
*
* \return  phyStatus_t
*
********************************************************************************** */
phyStatus_t PhyPlmeSetPwrState( uint8_t state )
{
    phyStatus_t status = gPhySuccess_c;
    static uint8_t mPhyPwrState = gPhyPwrIdle_c;

    /* Parameter validation */
    if( state > gPhyPwrReset_c )
    {
        status = gPhyInvalidParameter_c;
    }
    /* Check if the new power state = old power state */
    else if( state == mPhyPwrState )
    {
        status = gPhyBusy_c;
    }
    else
    {
        switch(state)
        {
        case gPhyPwrIdle_c:
            /* Set XCVR in run mode if not allready */
            if(RSIM->DSM_CONTROL & RSIM_DSM_CONTROL_ZIG_DEEP_SLEEP_STATUS_MASK)
            {
                RSIM->ZIG_WAKE = (RSIM->DSM_TIMER + mPhyDSM_GuardTime_d);
                while(RSIM->DSM_CONTROL & RSIM_DSM_CONTROL_ZIG_DEEP_SLEEP_STATUS_MASK);
            }
            break;

        case gPhyPwrDSM_c:
            /* Set XCVR in low power mode if not allready */
            if(!(RSIM->DSM_CONTROL & RSIM_DSM_CONTROL_ZIG_DEEP_SLEEP_STATUS_MASK))
            {
                uint32_t minTime = (RSIM->DSM_OSC_OFFSET > mPhyDSM_GuardTime_d) ? RSIM->DSM_OSC_OFFSET : mPhyDSM_GuardTime_d;

                RSIM->ZIG_SLEEP = RSIM->DSM_TIMER + minTime;
                RSIM->ZIG_WAKE = RSIM->DSM_TIMER + mPhyDSMDuration;
                RSIM->DSM_CONTROL |= RSIM_DSM_CONTROL_ZIG_SYSCLK_REQUEST_EN_MASK |
                                     RSIM_DSM_CONTROL_DSM_TIMER_EN_MASK | 
                                     RSIM_DSM_CONTROL_ZIG_SYSCLK_INTERRUPT_EN_MASK;
            }
            break;

        default:
            status = gPhyInvalidPrimitive_c;
            /* do not change current state */
            state = mPhyPwrState;
            break;
        }
        
        mPhyPwrState = state;
    }
    
    return status;
}