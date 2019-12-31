/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief GENFSK Interrupt install and enable. */
void GENFSK_InstallInterrupt(void);

/*! @brief Interrupt handler for GENFSK LL. */
void GENFSK_InterruptHandler(void);

/*! @brief Disable GENFSK global interrupt. */
void GENFSK_ProtectFromXcvrInterrupt(void);

/*! @brief Enable GENFSK global interrupt. */
void GENFSK_UnprotectFromXcvrInterrupt(void);

extern void GENFSK_TimeISR(void);
extern void GENFSK_TimeDisableWaitTimeout(void);
extern bool_t PWR_ClearRSIMDsmInt( void );
extern genfskEventStatus_t GENFSK_RxIsrContext(void);
extern bool_t GENFSK_CheckPacketMatch();

/*******************************************************************************
 * Variables
 ******************************************************************************/     
static uint8_t mGENFSK_IrqDisableCnt = 1;
extern GENFSK_LocalStruct_t genfskLocal[];
extern uint8_t mGenfskActiveInstance;
extern osaEventId_t mGenfskTaskEvent;
extern genfskTimerId_t rxTimeoutTimer;

#if defined(RADIO_IS_GEN_3P0) && (RADIO_IS_GEN_3P0 != 0) && defined(RF_OSC_26MHZ) && (RF_OSC_26MHZ != 0)
extern uint8_t gGenfskRxRecycleHdrError;
extern uint8_t gGenfskRxRecycleCrcError;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Disable GENFSK global interrupt.
 *
 * This function disables GENFSK global interrupt.
 */
void GENFSK_ProtectFromXcvrInterrupt(void)
{
    volatile uint32_t temp;
    
    /* Enter critical section. */
    OSA_InterruptDisable();
    
    if (mGENFSK_IrqDisableCnt == 0)
    {
        temp = GENFSK->IRQ_CTRL;
        temp &= ~(gGENFSK_AllIrqFlags | GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK);        
        GENFSK->IRQ_CTRL = temp;
    }
    
    mGENFSK_IrqDisableCnt++;
    
    /* Exit critical section. */
    OSA_InterruptEnable();
}

/*!
 * @brief Enable GENFSK global interrupt.
 *
 * This function enables GENFSK global interrupt.
 */
void GENFSK_UnprotectFromXcvrInterrupt(void)
{
    volatile uint32_t temp;
    
    /* Enter critical section. */
    OSA_InterruptDisable();
    
    if (mGENFSK_IrqDisableCnt)
    {
        mGENFSK_IrqDisableCnt--;
        
        if (mGENFSK_IrqDisableCnt == 0)
        {
            temp = GENFSK->IRQ_CTRL;
            temp &= ~gGENFSK_AllIrqFlags;
            temp |= (GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK | GENFSK_IRQ_CTRL_CRC_IGNORE_MASK);
            GENFSK->IRQ_CTRL = temp;
        }
    }
    
    /* Exit critical section. */
    OSA_InterruptEnable();
}

/*! @brief Disable GENFSK interrupts based on _GENFSK_interrupt_enable. */
void GENFSK_DisableInterrupts(uint32_t mask)
{
    uint32_t irqSts;
    
    mask &= gGENFSK_AllIrqEnable;
    
    irqSts = GENFSK->IRQ_CTRL;
    irqSts &= ~gGENFSK_AllIrqFlags;
    irqSts |= (mask >> 16);  /* Clear interrupt flags. */
    irqSts &= ~(mask);
    GENFSK->IRQ_CTRL = irqSts;
    genfskLocal[mGenfskActiveInstance].genfskRegs.irqCtrl = irqSts;
}

/*! @brief Enable GENFSK interrupts based on _GENFSK_interrupt_enable. */
void GENFSK_EnableInterrupts(uint32_t mask)
{
    uint32_t irqSts;
    
    mask &= gGENFSK_AllIrqEnable;
    
    irqSts = GENFSK->IRQ_CTRL;
    irqSts &= ~gGENFSK_AllIrqFlags;
    irqSts |= (mask >> 16);  /* Clear interrupt flags. */
    irqSts |= (mask);
    GENFSK->IRQ_CTRL = irqSts;
    genfskLocal[mGenfskActiveInstance].genfskRegs.irqCtrl = irqSts;
}

WEAK bool_t PWR_ClearRSIMDsmInt(void)
{
  return FALSE;
}

/*! @brief GENFSK LL Interrupt handler. */
void GENFSK_InterruptHandler(void)
{
    uint32_t irqStatus;
    uint32_t eventFlags = 0;
    
    if(PWR_ClearRSIMDsmInt())
    {        
      return;
    }
    
    /* Read current GENFSK status and interrupt status. */
    irqStatus = GENFSK->IRQ_CTRL;
    
    /* Clear all GENFSK interrupts. */
    GENFSK->IRQ_CTRL = irqStatus;   
    
    /* Keep only interrupt status bits */
    irqStatus &= gGENFSK_AllIrqFlags;
    
    /* TX interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_TX_IRQ_MASK)
    {           
        GENFSK->T1_CMP &= ~GENFSK_T1_CMP_T1_CMP_EN_MASK;
        
        if (genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskTxEvent)
        {
            eventFlags |= gGenfskTxEventFlag_c;
        }
        else
        {
            /* No notification enabled */
            genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;
        }
    }
    
    /* RX interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_RX_IRQ_MASK)
    {
#if defined(RADIO_IS_GEN_3P0) && (RADIO_IS_GEN_3P0 != 0) && defined(RF_OSC_26MHZ) && (RF_OSC_26MHZ != 0)
        if ( (genfskLocal[mGenfskActiveInstance].radioConfig.dataRate == gGenfskDR2Mbps) &&
             ( ( gGenfskRxRecycleHdrError && 
                    (GENFSK->PACKET_CFG & (GENFSK_PACKET_CFG_H0_FAIL_MASK |
                                           GENFSK_PACKET_CFG_H1_FAIL_MASK |
                                           GENFSK_PACKET_CFG_LENGTH_FAIL_MASK)) != 0 ) || 
               ( gGenfskRxRecycleCrcError && 
                    (GENFSK->IRQ_CTRL & GENFSK_IRQ_CTRL_CRC_VALID_MASK) == 0 )))
        {
            /* Recycle RX by issuing the RX START NOW command */
            GENFSK->XCVR_CTRL = 0x05;
        }
        else 
#endif
        {
            GENFSK_TimeCancelEvent(&rxTimeoutTimer);
                       
            GENFSK->T1_CMP &= ~GENFSK_T1_CMP_T1_CMP_EN_MASK;

            if (genfskLocal[mGenfskActiveInstance].packetReceivedCallbackIsr != NULL)
            {
                /* Examine RX buffer in ISR context if CallbackIsr is set */
                GENFSK_RxIsrContext();
            }

            if (genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskRxEvent)
            {
                eventFlags |= gGenfskRxEventFlag_c;
            }
            else
            {
                /* No notification enabled */
                genfskLocal[mGenfskActiveInstance].genfskState = gGENFSK_LL_Idle;            
            }
        }
    }
    
    /* Sequence End interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK)
    {
        if (genfskLocal[mGenfskActiveInstance].enabledEvents & gGENFSK_PromiscuousEventMask_c)
        {
            if (GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_H0_FAIL_MASK)
            {
                eventFlags |= gGenfskH0FailEventFlag_c;
            }
            else if (GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_LENGTH_FAIL_MASK)
            {
                eventFlags |= gGenfskLengthFailEventFlag_c;
            }
            else if (GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_H1_FAIL_MASK)
            {
                eventFlags |= gGenfskH1FailEventFlag_c;
            }
            else
            {
                /* Do nothing. */
            }
        }
    }
    
    /* Network Address Match interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_NTW_ADR_IRQ_MASK)
    {
        if ((genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskGapEvent))
        {
            bool_t match;
            uint32_t ntw_adr = (GENFSK->NTW_ADR_CTRL & GENFSK_NTW_ADR_CTRL_NTW_ADR_MCH_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_MCH_SHIFT;
            
            if (ntw_adr & 0x1) /* NW ADDR 0 has matched */
            {
                match = GENFSK_CheckPacketMatch();

                if(match)
                {
                    GENFSK_AbortAll();
                    eventFlags |= gGenfskGapMatchEventFlag_c;
                }
            }
        }

        if (genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskNwkAddressMatch)
        {
            eventFlags |= gGenfskNwkAddressMatchEventFlag_c;
        }
    }
    
    /* Timer (T1) Compare interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_T1_IRQ_MASK)
    {
        /* Do nothing. */
    }
    
    /* Timer (T2) Compare interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_T2_IRQ_MASK)
    {
        GENFSK_TimeDisableWaitTimeout();
        GENFSK_TimeISR();
    }
    
    /* PLL Unlock interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_MASK)
    {        
        if ((genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingRx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyRx))
        {
            GENFSK_AbortAll();
            
            if (genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskRxEvent)
            {
                eventFlags |= gGenfskRxPllUnlockEventFlag_c;
            }
        }
        
        if ((genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingTx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyTx))
        {
            GENFSK_AbortAll();
            
            if (genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskTxEvent)
            {
                eventFlags |= gGenfskTxPllUnlockEventFlag_c;
            }
        }
    }
    
    /* Wake interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_WAKE_IRQ_MASK)
    {
        uint32_t timeAdjust = 0;
        //uint32_t timeAdjust = RSIM->GEN_WAKE;
        
        //timeAdjust = (timeAdjust - RSIM->GEN_SLEEP)/32768U * 1000000U; /* [us] */
        
        GENFSK->EVENT_TMR = (timeAdjust) | GENFSK_EVENT_TMR_EVENT_TMR_ADD_MASK;                                          
        
        if (genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskWakeEvent)
        {
            eventFlags |= gGenfskWakeEventFlag_c;            
        }
    }
    
    /* RX Watermark interrupt. */
    if (irqStatus & GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_MASK)
    {
    }
    
    OSA_EventSet(mGenfskTaskEvent, eventFlags);
}

/*!
 * @brief GENFSK LL interrupt install.
 *
 * This function install the interrupt handler for GENFSK LL, sets interrupt priority and enables the interrupt.
 */
void GENFSK_InstallInterrupt(void)
{
#if (defined(CPU_MKW21Z256VHT4) || defined(CPU_MKW21Z512VHT4) || defined(CPU_MKW31Z256CAx4) || \
     defined(CPU_MKW31Z256VHT4) || defined(CPU_MKW31Z512CAx4) || defined(CPU_MKW31Z512VHT4) || \
     defined(CPU_MKW41Z256VHT4) || defined(CPU_MKW41Z512VHT4) || \
     defined(CPU_MKW35A512VFP4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW36A512VFP4) || \
     defined(CPU_MKW36A512VHT4) || defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4))
    if (gGENFSK_IrqNo_d == Radio_0_IRQn)
#elif (defined(CPU_K32W032S1M2VPJ_cm0plus))
    if (gGENFSK_IrqNo_d == RF0_0_IRQn)
#endif
    {    
        XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK);
        XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL(GENFSK_LL));
    }
    else
    {
        XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK);
        XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL(GENFSK_LL));
    }
    
    OSA_InstallIntHandler(gGENFSK_IrqNo_d, GENFSK_InterruptHandler);
        
    /* Enable transceiver interrupt request. */
    NVIC_ClearPendingIRQ(gGENFSK_IrqNo_d);
    NVIC_EnableIRQ(gGENFSK_IrqNo_d);
    
    /* Set transceiver interrupt priority. */
    NVIC_SetPriority(gGENFSK_IrqNo_d, gGENFSK_IrqPriority_c >> (8 - __NVIC_PRIO_BITS));
    
    GENFSK_UnprotectFromXcvrInterrupt();
}


