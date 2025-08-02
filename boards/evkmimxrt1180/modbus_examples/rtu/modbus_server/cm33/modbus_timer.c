/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "fsl_tpm.h"
#include "fsl_cache.h"
#include "fsl_lpuart.h"
#include "app.h"
#include "port.h"

/* ----------------------- static functions ---------------------------------*/
static void TIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
void BOARD_TPM_HANDLER(void)
{
    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);
    TIMERExpiredISR();
    SDK_ISR_EXIT_BARRIER;
}

BOOL
xMBPortTimersInit( USHORT usTimerT35_50us )
{
    tpm_config_t tpmInfo;
    
    TPM_GetDefaultConfig(&tpmInfo);

    /* TPM clock divide by TPM_PRESCALER */
    tpmInfo.prescale = TPM_PRESCALER;

    /* Initialize TPM module */
    TPM_Init(BOARD_TPM, &tpmInfo);

    /* Set timer period */
    TPM_SetTimerPeriod(BOARD_TPM, USEC_TO_COUNT(TIME_PERIOD_US, TPM_SOURCE_CLOCK / (1U << tpmInfo.prescale)));

    TPM_EnableInterrupts(BOARD_TPM, kTPM_TimeOverflowInterruptEnable);

    EnableIRQ(BOARD_TPM_IRQ_NUM);

    return 1;
}


void vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);
    TPM_EnableInterrupts(BOARD_TPM, kTPM_TimeOverflowInterruptEnable);
    EnableIRQ(BOARD_TPM_IRQ_NUM);
    TPM_StartTimer(BOARD_TPM, kTPM_SystemClock);
}

void vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
    TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);
    TPM_DisableInterrupts(BOARD_TPM, kTPM_TimeOverflowInterruptEnable);
    DisableIRQ(BOARD_TPM_IRQ_NUM);
    TPM_StopTimer(BOARD_TPM);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void TIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

