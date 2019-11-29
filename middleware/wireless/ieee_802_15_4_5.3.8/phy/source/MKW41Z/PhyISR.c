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
#include "Phy.h"
#include "PhyInterface.h"
#include "FunctionLib.h"

#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"

#include "fsl_xcvr.h"


/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#ifndef gPhyIrqPriority_c
#define gPhyIrqPriority_c     (0x80)
#endif
#define gPhyIrqNo_d           (Radio_1_IRQn)

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
extern Phy_PhyLocalStruct_t     phyLocal;
static volatile phyRxParams_t * mpRxParams        = NULL;
static const uint8_t            mPhyInstance      = 0;
uint8_t                         mPhyLastRxLQI     = 0;
uint8_t                         mPhyLastRxRSSI    = 0;
uint8_t                         mPhyIrqDisableCnt = 1;
bool_t                          mPhyForceFP = FALSE;


/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static void PhyIsrSeqCleanup(void);
static void PhyIsrTimeoutCleanup(void);
static void Phy_GetRxParams(void);
static uint8_t Phy_LqiConvert(uint8_t hwLqi);


/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  Disable the 802.15.4 radio IRQ
*
********************************************************************************** */
void ProtectFromXcvrInterrupt(void)
{
    OSA_InterruptDisable();
   
    if( mPhyIrqDisableCnt == 0 )
    {
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TRCV_MSK_MASK;
    }
    
    mPhyIrqDisableCnt++;
    
    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Enable the 802.15.4 radio IRQ
*
********************************************************************************** */
void UnprotectFromXcvrInterrupt(void)
{
    OSA_InterruptDisable();
    
    if( mPhyIrqDisableCnt )
    {
        mPhyIrqDisableCnt--;
        
        if( mPhyIrqDisableCnt == 0 )
        {
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TRCV_MSK_MASK;
        }
    }

    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Sets the location of the Rx parameters
*
* \param[in]  pRxParam pointer to Rx parameters
*
********************************************************************************** */
void PhyIsrPassRxParams
(
  volatile phyRxParams_t * pRxParam
)
{
    mpRxParams = pRxParam;
}

/*! *********************************************************************************
* \brief  Clear and mask PHY IRQ, set sequence to Idle
*
********************************************************************************** */
static void PhyIsrSeqCleanup
(
  void
)
{
    uint32_t irqStatus;

    /* Set the PHY sequencer back to IDLE */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_XCVSEQ_MASK;
    /* Mask SEQ, RX, TX and CCA interrupts */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_CCAMSK_MASK |
                     ZLL_PHY_CTRL_RXMSK_MASK  |
                     ZLL_PHY_CTRL_TXMSK_MASK  |
                     ZLL_PHY_CTRL_SEQMSK_MASK;

    while( ZLL->SEQ_STATE & ZLL_SEQ_STATE_SEQ_STATE_MASK ) {}

    irqStatus = ZLL->IRQSTS;
    /* Mask TMR3 interrupt */
    irqStatus |= ZLL_IRQSTS_TMR3MSK_MASK;
    /* Clear transceiver interrupts except TMRxIRQ */
    irqStatus &= ~( ZLL_IRQSTS_TMR1IRQ_MASK |
                    ZLL_IRQSTS_TMR2IRQ_MASK |
                    ZLL_IRQSTS_TMR3IRQ_MASK |
                    ZLL_IRQSTS_TMR4IRQ_MASK );
    ZLL->IRQSTS = irqStatus;
}

/*! *********************************************************************************
* \brief  Clear and mask PHY IRQ, disable timeout, set sequence to Idle
*
********************************************************************************** */
static void PhyIsrTimeoutCleanup
(
  void
)
{
    uint32_t irqStatus;

    /* Set the PHY sequencer back to IDLE and disable TMR3 comparator and timeout */
    ZLL->PHY_CTRL &= ~(ZLL_PHY_CTRL_TMR3CMP_EN_MASK | 
                       ZLL_PHY_CTRL_TC3TMOUT_MASK   | 
                       ZLL_PHY_CTRL_XCVSEQ_MASK);
    /* Mask SEQ, RX, TX and CCA interrupts */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_CCAMSK_MASK |
                     ZLL_PHY_CTRL_RXMSK_MASK  |
                     ZLL_PHY_CTRL_TXMSK_MASK  |
                     ZLL_PHY_CTRL_SEQMSK_MASK;

    while( ZLL->SEQ_STATE & ZLL_SEQ_STATE_SEQ_STATE_MASK ) {}

    irqStatus = ZLL->IRQSTS;
    /* Mask TMR3 interrupt */
    irqStatus |= ZLL_IRQSTS_TMR3MSK_MASK;
    /* Clear transceiver interrupts except TMR1IRQ and TMR4IRQ. */
    irqStatus &= ~( ZLL_IRQSTS_TMR1IRQ_MASK |
                    ZLL_IRQSTS_TMR4IRQ_MASK );
    ZLL->IRQSTS = irqStatus;
}

/*! *********************************************************************************
* \brief  Scales energy level to 0-255
*
* \param[in]  energyLevel  the energy level reported by HW
*
* \return  uint8_t  the energy level scaled in 0x00-0xFF
*
********************************************************************************** */
uint8_t Phy_GetEnergyLevel
(
    uint8_t energyLevel /* [dbm] */
)
{
    int32_t temp = (int8_t)energyLevel;

    if( temp <= -82 )
    {
        temp = 0x00;
    }
    else if( temp >= -3 )
    {
        temp = 0xFF;
    }
    else
    {
        /* Convert energy level from dbm into a 0x00-0xFF value */
        temp = (255 * temp + 20910) / 79; 
    }

    return (uint8_t)temp;
}

/*! *********************************************************************************
* \brief  This function returns the LQI for the las received packet
*
* \return  uint8_t  LQI value
*
********************************************************************************** */
uint8_t PhyGetLastRxLqiValue(void)
{
    return mPhyLastRxLQI;
}

/*! *********************************************************************************
* \brief  This function returns the RSSI for the las received packet
*
* \return  uint8_t  RSSI value
*
********************************************************************************** */
uint8_t PhyGetLastRxRssiValue(void)
{
    /*RSSI*/
    /*
    **       LQI
    **RSSI = ---  - LQI_COMP
    **       2.25
    */
    uint8_t LQI_COMP = (ZLL->CCA_LQI_CTRL & ZLL_CCA_LQI_CTRL_LQI_OFFSET_COMP_MASK) >> ZLL_CCA_LQI_CTRL_LQI_OFFSET_COMP_SHIFT;
    int32_t lqi_to_rssi= ((mPhyLastRxRSSI * 456u) >> 10u) + 5 - LQI_COMP;
    
    return (uint8_t)lqi_to_rssi;
}
    
/*! *********************************************************************************
* \brief  This function converts the LQI reported by the PHY into an signed RSSI value
*
* \param[in]  LQI  the LQI reported by the PHY
*
* \return  the RSSI value in dbm
*
********************************************************************************** */
int8_t PhyConvertLQIToRSSI(uint8_t lqi)
{
    int32_t rssi = (36*lqi - 9836) / 109;

    return (int8_t)rssi;
}

/*! *********************************************************************************
* \brief  PHY ISR
*
********************************************************************************** */
void PHY_InterruptHandler(void)
{
    uint8_t xcvseqCopy;
    uint32_t irqStatus;
    uint32_t length;

    /* RSIM Wake-up IRQ */
    if (RSIM->DSM_CONTROL & RSIM_DSM_CONTROL_ZIG_SYSCLK_REQ_INT_MASK)
    {
        RSIM->DSM_CONTROL = RSIM->DSM_CONTROL;
        return;
    }

    /* Read current XCVRSEQ and interrup status */
    xcvseqCopy  = ZLL->PHY_CTRL & ZLL_PHY_CTRL_XCVSEQ_MASK;
    irqStatus   = ZLL->IRQSTS;
    /* Clear ZLL interrupts. The TX, RX, CCA and TC3 IRQs will be handled on SEQIRQ and must not be cleared. */
    ZLL->IRQSTS = irqStatus & ~(ZLL_IRQSTS_TMR3IRQ_MASK | ZLL_IRQSTS_CCAIRQ_MASK | ZLL_IRQSTS_RXIRQ_MASK | ZLL_IRQSTS_TXIRQ_MASK);

    /* WAKE IRQ */
    if( irqStatus & ZLL_IRQSTS_WAKE_IRQ_MASK )
    {
        uint32_t timeAdjust = RSIM->ZIG_WAKE;
        /* Adjust the 802.15.4 EVENT_TMR */
        timeAdjust = (timeAdjust - RSIM->ZIG_SLEEP)/32768U * 1000000U; /* [us] */
        ZLL->EVENT_TMR = (timeAdjust << 4) | ZLL_EVENT_TMR_EVENT_TMR_ADD_MASK;
        Radio_Phy_TimeRxTimeoutIndication(mPhyInstance);
    }

    /* Flter Fail IRQ */
    if( irqStatus & ZLL_IRQSTS_FILTERFAIL_IRQ_MASK )
    {
#if gMWS_UseCoexistence_d
        MWS_CoexistenceReleaseAccess();
#endif
        if( !(ZLL->PHY_CTRL & ZLL_PHY_CTRL_FILTERFAIL_MSK_MASK) )
        {
            Radio_Phy_PlmeFilterFailRx(mPhyInstance);
        }
    }
    /* Rx Watermark IRQ */
    else
    {
        if( (!(ZLL->PHY_CTRL & ZLL_PHY_CTRL_RX_WMRK_MSK_MASK)) && (irqStatus & ZLL_IRQSTS_RXWTRMRKIRQ_MASK) )
        {
            length = (irqStatus & ZLL_IRQSTS_RX_FRAME_LENGTH_MASK) >> ZLL_IRQSTS_RX_FRAME_LENGTH_SHIFT;
            Radio_Phy_PlmeRxWatermark(mPhyInstance, length);
#if gMWS_UseCoexistence_d
            if( (xcvseqCopy == gRX_c) && (gMWS_Success_c != MWS_CoexistenceRequestAccess(gMWS_RxState_c)) )
            {
                PhyAbort();
                Radio_Phy_TimeRxTimeoutIndication(mPhyInstance);
            }
#endif
        }
    }

    /* Timer 1 Compare Match */
    if( (irqStatus & ZLL_IRQSTS_TMR1IRQ_MASK) && (!(irqStatus & ZLL_IRQSTS_TMR1MSK_MASK)) )
    {
        PhyTimeDisableWaitTimeout();
        Radio_Phy_TimeWaitTimeoutIndication(mPhyInstance);
    }

    /* Sequencer interrupt, the autosequence has completed */
    if( (!(ZLL->PHY_CTRL & ZLL_PHY_CTRL_SEQMSK_MASK)) && (irqStatus & ZLL_IRQSTS_SEQIRQ_MASK) )
    {
        /* PLL unlock, the autosequence has been aborted due to PLL unlock */
        if( irqStatus & ZLL_IRQSTS_PLL_UNLOCK_IRQ_MASK )
        {
            PhyIsrSeqCleanup();
#if gMWS_UseCoexistence_d
            MWS_CoexistenceReleaseAccess();
#endif
            Radio_Phy_PlmeSyncLossIndication(mPhyInstance);
        }
        /* TMR3 timeout, the autosequence has been aborted due to TMR3 timeout */
        else if( (irqStatus & ZLL_IRQSTS_TMR3IRQ_MASK) &&
            (!(irqStatus & ZLL_IRQSTS_RXIRQ_MASK)) &&
            (gTX_c != xcvseqCopy) )
        {
            PhyIsrTimeoutCleanup();
#if gMWS_UseCoexistence_d
            MWS_CoexistenceReleaseAccess();
#endif
            Radio_Phy_TimeRxTimeoutIndication(mPhyInstance);
        }
        else
        {
            PhyIsrSeqCleanup();
#if gMWS_UseCoexistence_d
            MWS_CoexistenceReleaseAccess();
#endif      
            switch(xcvseqCopy)
            {
            case gTX_c:
                if( (ZLL->PHY_CTRL & ZLL_PHY_CTRL_CCABFRTX_MASK) && (irqStatus & ZLL_IRQSTS_CCA_MASK ) )
                {
                    Radio_Phy_PlmeCcaConfirm(gPhyChannelBusy_c, mPhyInstance);
                }
                else
                {
                    Radio_Phy_PdDataConfirm(mPhyInstance, FALSE);
                }
                break;
                
            case gTR_c:
                if( (ZLL->PHY_CTRL & ZLL_PHY_CTRL_CCABFRTX_MASK) && (irqStatus & ZLL_IRQSTS_CCA_MASK ) )
                {
                    Radio_Phy_PlmeCcaConfirm(gPhyChannelBusy_c, mPhyInstance);
                }
                else
                {
                    Phy_GetRxParams();
                    Radio_Phy_PdDataConfirm(mPhyInstance, (irqStatus & ZLL_IRQSTS_RX_FRM_PEND_MASK) > 0);
                }
                break;
                
            case gRX_c:
                /* Check SAA0 and SAA1 (source address absent) */
                if( irqStatus & ZLL_IRQSTS_PI_MASK )
                {
                    /* Save the state of the FP bit sent in ACK frame */
                    if( PhyPpIsTxAckDataPending() )
                    {
                        phyLocal.flags |= gPhyFlagTxAckFP_c;
                    }
                    else
                    {
                        phyLocal.flags &= ~gPhyFlagTxAckFP_c;
                    }

                    if( ((ZLL->SAM_MATCH & (ZLL_SAM_MATCH_SAA0_ADDR_ABSENT_MASK | ZLL_SAM_MATCH_SAP0_ADDR_PRESENT_MASK)) == ZLL_SAM_MATCH_SAA0_ADDR_ABSENT_MASK) ||
                        ((ZLL->SAM_MATCH & (ZLL_SAM_MATCH_SAA1_ADDR_ABSENT_MASK | ZLL_SAM_MATCH_SAP1_ADDR_PRESENT_MASK)) == ZLL_SAM_MATCH_SAA1_ADDR_ABSENT_MASK) )
                    {
                        mPhyForceFP = TRUE;
                    }
                }
                
                Phy_GetRxParams();
                Radio_Phy_PdDataIndication(mPhyInstance);
                break;
                
            case gCCA_c:
                if( gCcaED_c == ((ZLL->PHY_CTRL & ZLL_PHY_CTRL_CCATYPE_MASK) >> ZLL_PHY_CTRL_CCATYPE_SHIFT) )
                {
                    Radio_Phy_PlmeEdConfirm( (ZLL->LQI_AND_RSSI & ZLL_LQI_AND_RSSI_CCA1_ED_FNL_MASK) >> ZLL_LQI_AND_RSSI_CCA1_ED_FNL_SHIFT, mPhyInstance );
                }
                else /* CCA */
                {
                    if( irqStatus & ZLL_IRQSTS_CCA_MASK )
                    {
                        Radio_Phy_PlmeCcaConfirm(gPhyChannelBusy_c, mPhyInstance);
                    }
                    else
                    {
                        Radio_Phy_PlmeCcaConfirm(gPhyChannelIdle_c, mPhyInstance);
                    }
                }
                break;
                
            case gCCCA_c:
                Radio_Phy_PlmeCcaConfirm(gPhyChannelIdle_c, mPhyInstance);
                break;
                
            default:
                Radio_Phy_PlmeSyncLossIndication(mPhyInstance);
                break;
            }
        }
    }
    /* Timers interrupt */
    else
    {
        /* Timer 2 Compare Match */
        if( (irqStatus & ZLL_IRQSTS_TMR2IRQ_MASK) && (!(irqStatus & ZLL_IRQSTS_TMR2MSK_MASK)) )
        {
            PhyTimeDisableEventTrigger();

            if( gIdle_c != xcvseqCopy )
            {
                Radio_Phy_TimeStartEventIndication(mPhyInstance);
            }
        }

        /* Timer 3 Compare Match */
        if( (irqStatus & ZLL_IRQSTS_TMR3IRQ_MASK) && (!(irqStatus & ZLL_IRQSTS_TMR3MSK_MASK)) )
        {
            PhyTimeDisableEventTimeout();

            /* Ensure that we're not issuing TimeoutIndication while the Automated sequence is still in progress */
            /* TMR3 can expire during R-T turnaround for example, case in which the sequence is not interrupted */
            if( gIdle_c == xcvseqCopy )
            {
                Radio_Phy_TimeRxTimeoutIndication(mPhyInstance);
            }
        }

        /* Timer 4 Compare Match */
        if( (irqStatus & ZLL_IRQSTS_TMR4IRQ_MASK) && (!(irqStatus & ZLL_IRQSTS_TMR4MSK_MASK)) )
        {
            /* Disable TMR4 comparator */
            ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR4CMP_EN_MASK;
            /* Mask and clear TMR4 interrupt (do not change other IRQ status) */
            irqStatus &= ~( ZLL_IRQSTS_TMR1IRQ_MASK |
                            ZLL_IRQSTS_TMR2IRQ_MASK |
                            ZLL_IRQSTS_TMR3IRQ_MASK );
            irqStatus |= ZLL_IRQSTS_TMR4IRQ_MASK | ZLL_IRQSTS_TMR4MSK_MASK;
            ZLL->IRQSTS = irqStatus;
            PhyTime_TMR4Callback();
        }
    }

    Radio_Phy_Notify();
}

/*! *********************************************************************************
* \brief  This function installs the PHY ISR
*
********************************************************************************** */
void PHY_InstallIsr( void )
{
    OSA_InstallIntHandler(gPhyIrqNo_d, PHY_InterruptHandler);

    /* enable transceiver SPI interrupt request */
    NVIC_ClearPendingIRQ(gPhyIrqNo_d);
    NVIC_EnableIRQ(gPhyIrqNo_d);
    
    /* set transceiver interrupt priority */
    NVIC_SetPriority(gPhyIrqNo_d, gPhyIrqPriority_c >> (8 - __NVIC_PRIO_BITS));
    UnprotectFromXcvrInterrupt();
}

/*! *********************************************************************************
* \brief  This function forces the PHY IRQ to be triggered to run the ISR
*
********************************************************************************** */
void PHY_ForceIrqPending( void )
{
    NVIC_SetPendingIRQ(gPhyIrqNo_d);
}

/*! *********************************************************************************
 * \brief Check is an XCVR IRQ is pending
 *
 * \return  TRUE if ISR pending, else FALSE
 *
 ********************************************************************************** */
bool_t PHY_isIrqPending(void)
{
    bool_t status = FALSE;
    uint32_t temp;
    
    if( !(ZLL->PHY_CTRL & ZLL_PHY_CTRL_TRCV_MSK_MASK) )
    {
        /* Check usual ZLL IRQs */
        temp = ZLL->PHY_CTRL & ( ZLL_PHY_CTRL_SEQMSK_MASK         |
                                 ZLL_PHY_CTRL_TXMSK_MASK          |
                                 ZLL_PHY_CTRL_RXMSK_MASK          |
                                 ZLL_PHY_CTRL_CCAMSK_MASK         |
                                 ZLL_PHY_CTRL_RX_WMRK_MSK_MASK    |
                                 ZLL_PHY_CTRL_FILTERFAIL_MSK_MASK |
                                 ZLL_PHY_CTRL_PLL_UNLOCK_MSK_MASK );
        temp = temp >> ZLL_PHY_CTRL_SEQMSK_SHIFT;
        
        if( ZLL->IRQSTS & temp )
        {
            status = TRUE;
        }
        else
        {
            /* Check ZLL Timers IRQs */
            temp = ZLL->IRQSTS & ( ZLL_IRQSTS_TMR1IRQ_MASK |
                                   ZLL_IRQSTS_TMR2IRQ_MASK |
                                   ZLL_IRQSTS_TMR3IRQ_MASK |
                                   ZLL_IRQSTS_TMR4IRQ_MASK );
            temp = temp >> 4;
            
            if( ZLL->IRQSTS & temp )
            {
                status = TRUE;
            }
        }
    }

    return status;
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************* */

/*! *********************************************************************************
* \brief  Fill the Rx parameters: RSSI, LQI, Timestamp and PSDU length
*
********************************************************************************** */
static void Phy_GetRxParams(void)
{
    if(NULL != mpRxParams)
    {
        mPhyLastRxRSSI = (ZLL->LQI_AND_RSSI & ZLL_LQI_AND_RSSI_LQI_VALUE_MASK) >> ZLL_LQI_AND_RSSI_LQI_VALUE_SHIFT;
        mPhyLastRxLQI = Phy_LqiConvert(mPhyLastRxRSSI);
        mpRxParams->linkQuality = mPhyLastRxLQI;
        mpRxParams->timeStamp = ZLL->TIMESTAMP;
        mpRxParams->psduLength = (ZLL->IRQSTS & ZLL_IRQSTS_RX_FRAME_LENGTH_MASK) >> ZLL_IRQSTS_RX_FRAME_LENGTH_SHIFT; /* Including FCS (2 bytes) */
        mpRxParams = NULL;
    }
}

/*! *********************************************************************************
* \brief  Scales LQI to 0-255
*
* \param[in]  hwLqi  the LQI reported by HW
*
* \return  uint8_t  the LQI scaled in 0x00-0xFF
*
********************************************************************************** */
static uint8_t Phy_LqiConvert
(
uint8_t hwLqi
)
{
    if( hwLqi >= 220 )
    {
        hwLqi = 255;
    }
    else
    {
        hwLqi = (51 * hwLqi) / 44;
    }
    
    return hwLqi;
}