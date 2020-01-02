/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_device_registers.h"
#include "timer.h"
#include "fsl_tpm.h"

typedef struct _hal_timer_handle_struct_t
{
    uint32_t timeout;
    uint32_t timerClock_Hz;
    hal_timer_callback_t callback;
    void *callbackParam;
    uint8_t instance;
} hal_timer_handle_struct_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

static TPM_Type *const s_TPMBase[] = TPM_BASE_PTRS;

static hal_timer_handle_t s_timerHandle[sizeof(s_TPMBase) / sizeof(TPM_Type *)];

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static void HAL_TimerInterruptHandle(uint8_t instance)
{
    hal_timer_handle_struct_t *halTimerState = (hal_timer_handle_struct_t *)s_timerHandle[instance];

    TPM_ClearStatusFlags(s_TPMBase[halTimerState->instance], (uint32_t)kTPM_TimeOverflowFlag);
    if (halTimerState->callback != NULL)
    {
        halTimerState->callback(halTimerState->callbackParam);
    }
}

static void HAL_TimerHwInit(hal_timer_handle_t halTimerHandle)
{
    IRQn_Type mTPMIrqId[]                    = TPM_IRQS;
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    IRQn_Type irqId;
    tpm_config_t tpmInfo;

    assert(halTimerHandle);
    TPM_GetDefaultConfig(&tpmInfo);
    /* Initialize TPM module */
    tpmInfo.prescale = kTPM_Prescale_Divide_128;
    TPM_Init(s_TPMBase[halTimerState->instance], (void *)&tpmInfo);
    TPM_StopTimer(s_TPMBase[halTimerState->instance]);

    /* Set the timer to be in free-running mode */
    s_TPMBase[halTimerState->instance]->MOD = 0xFFFF;

    HAL_TimerUpdateTimeout(halTimerHandle, halTimerState->timeout);

    /* Install ISR */
    irqId = mTPMIrqId[halTimerState->instance];
    TPM_EnableInterrupts(s_TPMBase[halTimerState->instance], (uint32_t)kTPM_TimeOverflowInterruptEnable);

    s_timerHandle[halTimerState->instance] = halTimerHandle;
    NVIC_SetPriority(irqId, HAL_TIMER_ISR_PRIORITY);
    (void)EnableIRQ(irqId);
}
void TPM0_IRQHandler(void);
void TPM1_IRQHandler(void);
void TPM2_IRQHandler(void);

void TPM0_IRQHandler(void)
{
    HAL_TimerInterruptHandle(0);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
   exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
void TPM1_IRQHandler(void)
{
    HAL_TimerInterruptHandle(1);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
void TPM2_IRQHandler(void)
{
    HAL_TimerInterruptHandle(2);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
 exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
hal_timer_status_t HAL_TimerInit(hal_timer_handle_t halTimerHandle, hal_timer_config_t *halTimerConfig)
{
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    tpm_config_t tpmInfo;

    assert(sizeof(hal_timer_handle_struct_t) == HAL_TIMER_HANDLE_SIZE);
    assert(halTimerConfig);
    assert(halTimerHandle);
    assert(halTimerConfig->instance < (sizeof(s_TPMBase) / sizeof(TPM_Type *)));

    halTimerState->timeout  = halTimerConfig->timeout;
    halTimerState->instance = halTimerConfig->instance;
    assert(s_TPMBase[halTimerState->instance]);

    /* TPM clock divide by 128 */
    tpmInfo.prescale             = kTPM_Prescale_Divide_128;
    halTimerState->timerClock_Hz = halTimerConfig->srcClock_Hz / (1UL << (uint8_t)tpmInfo.prescale);

    HAL_TimerHwInit(halTimerHandle);
    return kStatus_HAL_TimerSuccess;
}
void HAL_TimerDeinit(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    s_timerHandle[halTimerState->instance]   = NULL;
    TPM_Deinit(s_TPMBase[halTimerState->instance]);
}
/*************************************************************************************/
void HAL_TimerEnable(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    TPM_StartTimer(s_TPMBase[halTimerState->instance], kTPM_SystemClock);
}

/*************************************************************************************/
void HAL_TimerDisable(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    TPM_StopTimer(s_TPMBase[halTimerState->instance]);
}

/*************************************************************************************/
void HAL_TimerInstallCallback(hal_timer_handle_t halTimerHandle, hal_timer_callback_t callback, void *callbackParam)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    halTimerState->callback                  = callback;
    halTimerState->callbackParam             = callbackParam;
}
uint32_t HAL_TimerGetMaxTimeout(hal_timer_handle_t halTimerHandle)
{
    uint32_t reserveCount;
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    reserveCount                             = (uint32_t)MSEC_TO_COUNT((4), (halTimerState->timerClock_Hz));
    if (reserveCount < MSEC_TO_COUNT((1), (halTimerState->timerClock_Hz)))
    {
        return 1000;
    }
    return (uint32_t)COUNT_TO_USEC((0xFFFFUL - (uint64_t)reserveCount), (halTimerState->timerClock_Hz));
}
/* return micro us */
uint32_t HAL_TimerGetCurrentTimerCount(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    return (uint32_t)COUNT_TO_USEC((TPM_GetCurrentTimerCount(s_TPMBase[halTimerState->instance])),
                                   (halTimerState->timerClock_Hz));
}

hal_timer_status_t HAL_TimerUpdateTimeout(hal_timer_handle_t halTimerHandle, uint32_t timeout)
{
    uint32_t tickCount;

    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    halTimerState->timeout                   = timeout;
    tickCount = (uint32_t)USEC_TO_COUNT((halTimerState->timeout), (halTimerState->timerClock_Hz));
    if ((tickCount < 1U) || (tickCount > 0xfff0U))
    {
        return kStatus_HAL_TimerOutOfRanger;
    }
    TPM_SetTimerPeriod(s_TPMBase[halTimerState->instance], tickCount);
    return kStatus_HAL_TimerSuccess;
}

void HAL_TimerExitLowpower(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    HAL_TimerHwInit(halTimerHandle);
}

void HAL_TimerEnterLowpower(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
}
