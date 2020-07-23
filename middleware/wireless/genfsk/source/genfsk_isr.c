/*

Copyright 2016-2019 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Interrupt handler for GENFSK LL. */
void GENFSK_InterruptHandler(void);

WEAK bool_t PWR_ClearRSIMDsmInt(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t mGENFSK_IrqDisableCnt = 1;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * Handler of GENFSK_IRQ_CTRL_TX_IRQ_MASK.
 * Set gGenfskTxEventFlag_c if event.
 */
static inline void GENFSK_IRQTx(osaEventFlags_t *pEventFlags)
{
    GENFSK->T1_CMP &= ~GENFSK_T1_CMP_T1_CMP_EN_MASK;

    *pEventFlags |= gGenfskTxEventFlag_c;
}

/*!
 * Handler of GENFSK_IRQ_CTRL_RX_IRQ_MASK.
 * Set gGenfskRxEventFlag_c.
 */
static inline void GENFSK_IRQRx(osaEventFlags_t *pEventFlags)
{
#if defined(RADIO_IS_GEN_3P0) && (RADIO_IS_GEN_3P0 != 0) && defined(RF_OSC_26MHZ) && (RF_OSC_26MHZ != 0)
    if ( (genfskLocal[mGenfskActiveInstance].radioConfig.dataRate == gGenfskDR2Mbps) &&
        ( ( gGenfskRxRecycleHdrError &&
           (GENFSK->PACKET_CFG & (GENFSK_PACKET_CFG_H0_FAIL_MASK |
                                  GENFSK_PACKET_CFG_H1_FAIL_MASK |
                                      GENFSK_PACKET_CFG_LENGTH_FAIL_MASK)) != 0U) ||
         ( gGenfskRxRecycleCrcError &&
          (GENFSK->IRQ_CTRL & GENFSK_IRQ_CTRL_CRC_VALID_MASK) == 0U)))
    {
        /* Recycle RX by issuing the RX START NOW command */
        GENFSK->XCVR_CTRL = 0x05;
    }
    else
#endif
    {
        GENFSK_TimeCancelEvent(&rxTimeoutTimer);

        GENFSK->T1_CMP &= ~GENFSK_T1_CMP_T1_CMP_EN_MASK;

        *pEventFlags |= gGenfskRxEventFlag_c;
    }
}

/*!
 * Handler of GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK.
 * Set gGenfskH0FailEventFlag_c | gGenfskLengthFailEventFlag_c | gGenfskH1FailEventFlag_c if gGenfskPromiscuousEvent is enabled.
 */
static inline void GENFSK_IRQSequenceEnd(osaEventFlags_t *pEventFlags)
{
    if ((genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskPromiscuousEvent) != 0U)
    {
        if ((GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_H0_FAIL_MASK) != 0U)
        {
            *pEventFlags |= gGenfskH0FailEventFlag_c;
        }
        else if ((GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_LENGTH_FAIL_MASK) != 0U)
        {
            *pEventFlags |= gGenfskLengthFailEventFlag_c;
        }
        else if ((GENFSK->PACKET_CFG & GENFSK_PACKET_CFG_H1_FAIL_MASK) != 0U)
        {
            *pEventFlags |= gGenfskH1FailEventFlag_c;
        }
        else
        {
            /* Do nothing. */
        }
#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
        /* Request a MWS release on sequence end interrupt currently used only in promiscuous mode. */
        (void) OSA_EventSet(mGenfskTaskEvent, gGenfskMwsReleaseEventFlag_c);
#endif /* (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c) */
    }
}

/*!
 * Handler of GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_MASK.
 * Set gGenfskRxPllUnlockEventFlag_c event
 */
static inline void GENFSK_IRQPllUnlock(osaEventFlags_t *pEventFlags)
{
    /* If current state is Rx or RxTx sequence, Abort and send event */
    if ((genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingRx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyRx)
#if defined (RADIO_IS_GEN_3P5)
        || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingRxTx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyRxTx)
#endif
            )
    {
        GENFSK_AbortAll();

        *pEventFlags |= gGenfskRxPllUnlockEventFlag_c;
    }

    /* If current state is Tx or TxRx sequence, Abort and send event */
    if ((genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingTx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyTx)
#if defined (RADIO_IS_GEN_3P5)
        || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyPendingTxRx) || (genfskLocal[mGenfskActiveInstance].genfskState == gGENFSK_LL_BusyTxRx)
#endif
            )
    {
        GENFSK_AbortAll();

        *pEventFlags |= gGenfskTxPllUnlockEventFlag_c;
    }
}

/*!
 * Handler of GENFSK_IRQ_CTRL_WAKE_IRQ_MASK.
 * Set gGenfskWakeEventFlag_c if event is enabled.
 */
static inline void GENFSK_IRQWake(osaEventFlags_t *pEventFlags)
{
    uint32_t timeAdjust = 0;

    GENFSK->EVENT_TMR = (timeAdjust) | GENFSK_EVENT_TMR_EVENT_TMR_ADD_MASK;

    if ((genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskWakeEvent) != 0U)
    {
        *pEventFlags |= gGenfskWakeEventFlag_c;
    }
}

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

    if (mGENFSK_IrqDisableCnt == 0U)
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

    if (mGENFSK_IrqDisableCnt != 0U)
    {
        mGENFSK_IrqDisableCnt--;

        if (mGENFSK_IrqDisableCnt == 0U)
        {
            temp = GENFSK->IRQ_CTRL;
            temp &= ~gGENFSK_AllIrqFlags;
            temp |= GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK;
#if !defined (RADIO_IS_GEN_3P5)
            temp |= GENFSK_IRQ_CTRL_CRC_IGNORE_MASK;
#endif
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
#if defined (RADIO_IS_GEN_3P5)
    /* TODO: remove this after radio init sequence rework as XCVR_MISC->XCVR_CTRL is overwritten during radio init*/
    XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK);
    XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL(GENFSK_LL));
#endif
}

#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
/* This function is overwritten when doing code coverage then do not monitor it */
/* LCOV_EXCL_START */
#endif
/*!
 * Default implementation for DSM clearing.
 * Can be overwritten on a per-device basis.
 */
WEAK bool_t PWR_ClearRSIMDsmInt(void)
{
  return FALSE;
}
#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
/* LCOV_EXCL_STOP */
#endif

/* LCOV_EXCL_START */
#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
WEAK void GENFSK_InterruptStatusHook(volatile uint32_t *irqStatus_p) {}
#endif
/* LCOV_EXCL_STOP */

/*! @brief GENFSK LL Interrupt handler. */
void GENFSK_InterruptHandler(void)
{
    uint32_t irqStatus;
    osaEventFlags_t eventFlags = 0;

#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
#if defined (RADIO_IS_GEN_3P5)
    /* Update LL_CTRL to allow access to Genfsk register. */
    RADIO_CTRL->LL_CTRL = 0x00000002U;
#endif
#endif /* (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c) */

    if(!PWR_ClearRSIMDsmInt())
    {
        /* Read current XCVR status and interrupt status. */
        irqStatus = GENFSK->IRQ_CTRL;

        /* Clear all GENFSK interrupts. */
        GENFSK->IRQ_CTRL = irqStatus;

#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
        GENFSK_InterruptStatusHook(&irqStatus);
#endif
        /* TX interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_TX_IRQ_MASK) != 0U)
        {
            GENFSK_IRQTx(&eventFlags);
        }

        /* RX interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_RX_IRQ_MASK) != 0U)
        {
            GENFSK_IRQRx(&eventFlags);
        }

        /* Sequence End interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK) != 0U)
        {
            GENFSK_IRQSequenceEnd(&eventFlags);
        }

        /* Network Address Match interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_NTW_ADR_IRQ_MASK) != 0U)
        {
            if ((genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskNwkAddressMatch) != 0U)
            {
                eventFlags |= gGenfskNwkAddressMatchEventFlag_c;
            }
        }

        /* Timer (T1) Compare interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_T1_IRQ_MASK) != 0U)
        {
            /* Do nothing. */
        }

        /* Timer (T2) Compare interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_T2_IRQ_MASK) != 0U)
        {
            GENFSK_TimeDisableWaitTimeout();
            GENFSK_TimeISR();
        }

        /* PLL Unlock interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_MASK) != 0U)
        {
            GENFSK_IRQPllUnlock(&eventFlags);
        }

        /* Wake interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_WAKE_IRQ_MASK) != 0U)
        {
            GENFSK_IRQWake(&eventFlags);
        }

        /* RX Watermark interrupt. */
        if ((irqStatus & GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_MASK) != 0U)
        {
        }

        if (eventFlags != 0U)
        {
            (void) OSA_EventSet(mGenfskTaskEvent, eventFlags);
        }
    }
}

/*!
 * @brief GENFSK LL interrupt install.
 *
 * This function install the interrupt handler for GENFSK LL, sets interrupt priority and enables the interrupt.
 */
void GENFSK_InstallInterrupt(void)
{
/* note: Radio_0_IRQn is defined for all platforms except for CPU_K32W042S1M2VPJ_cm0plus which defines RF0_0_IRQn instead */
#if ( (defined(Radio_0_IRQn) && (gGENFSK_IrqNo_d == Radio_0_IRQn)) || (defined(RF0_0_IRQn) && (gGENFSK_IrqNo_d == RF0_0_IRQn)) )
    {
        XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK);
        XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL(GENFSK_LL));
    }
#else
    {
        XCVR_MISC->XCVR_CTRL &= (uint32_t)~(uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK);
        XCVR_MISC->XCVR_CTRL |= (uint32_t)(XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL(GENFSK_LL));
    }
#endif

    OSA_InstallIntHandler((uint32_t)gGENFSK_IrqNo_d, GENFSK_InterruptHandler);

    /* Enable transceiver interrupt request. */
    NVIC_ClearPendingIRQ(gGENFSK_IrqNo_d);
    NVIC_EnableIRQ(gGENFSK_IrqNo_d);

    /* Set transceiver interrupt priority. */
    NVIC_SetPriority(gGENFSK_IrqNo_d, gGENFSK_IrqPriority_c >> (8 - __NVIC_PRIO_BITS));

    GENFSK_UnprotectFromXcvrInterrupt();
}
