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
#include "fsl_ftm.h"

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

static FTM_Type *const s_FtmBase[] = FTM_BASE_PTRS;

static hal_timer_handle_t s_timerHandle[sizeof(s_FtmBase) / sizeof(FTM_Type *)];
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

    FTM_ClearStatusFlags(s_FtmBase[instance], kFTM_TimeOverflowFlag);
    if (halTimerState->callback != NULL)
        halTimerState->callback(halTimerState->callbackParam);
}

void FTM0_IRQHandler(void)
{
    HAL_TimerInterruptHandle(0);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
 exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void FTM1_IRQHandler(void)
{
    HAL_TimerInterruptHandle(1);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void FTM2_IRQHandler(void)
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
    IRQn_Type mFTMIrqId[]                    = FTM_IRQS;
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    IRQn_Type irqId;
    ftm_config_t ftmInfo;
    FTM_Type *FTMBaseAddr;

    assert(sizeof(hal_timer_handle_struct_t) == HAL_TIMER_HANDLE_SIZE);
    assert(halTimerConfig);
    assert(halTimerHandle);
    assert(halTimerConfig->instance < (sizeof(s_FtmBase) / sizeof(FTM_Type *)));

    halTimerState->timeout  = halTimerConfig->timeout;
    halTimerState->instance = halTimerConfig->instance;
    FTMBaseAddr             = (FTM_Type *)s_FtmBase[halTimerState->instance];
    assert(FTMBaseAddr);
    FTM_GetDefaultConfig(&ftmInfo);
    /* FTM clock divide by 128 */
    ftmInfo.prescale = kFTM_Prescale_Divide_128;
    /* Initialize FTM module */
    FTM_Init(FTMBaseAddr, &ftmInfo);
    FTM_StopTimer(FTMBaseAddr);
    halTimerState->timerClock_Hz = halTimerConfig->srcClock_Hz / (1 << ftmInfo.prescale);
    /* Set the timer to be in free-running mode */
    FTMBaseAddr->MOD = 0xFFFF;
    if (USEC_TO_COUNT(halTimerState->timeout, halTimerState->timerClock_Hz) > 0xFFFF)
        return kStatus_HAL_TimerOutOfRanger;
    /* Configure channel to Software compare; output pin not used */
    FTM_SetTimerPeriod(FTMBaseAddr, USEC_TO_COUNT(halTimerState->timeout, halTimerState->timerClock_Hz));
    /* Install ISR */
    irqId = mFTMIrqId[halTimerState->instance];
    FTM_EnableInterrupts(FTMBaseAddr, kFTM_TimeOverflowInterruptEnable);
    NVIC_SetPriority((IRQn_Type)irqId, HAL_TIMER_ISR_PRIORITY);
    s_timerHandle[halTimerState->instance] = halTimerHandle;
    EnableIRQ(irqId);
    return kStatus_HAL_TimerSuccess;
}
void HAL_TimerDeinit(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    s_timerHandle[halTimerState->instance]   = NULL;
    FTM_Deinit(s_FtmBase[halTimerState->instance]);
}
/*************************************************************************************/
void HAL_TimerEnable(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    FTM_StartTimer(s_FtmBase[halTimerState->instance], kFTM_SystemClock);
}

/*************************************************************************************/
void HAL_TimerDisable(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    FTM_StopTimer(s_FtmBase[halTimerState->instance]);
}

/*************************************************************************************/
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
    return COUNT_TO_USEC(0xFFFF - reserveCount, halTimerState->timerClock_Hz);
}
/* return micro us */
uint32_t HAL_TimerGetCurrentTimerCount(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    return COUNT_TO_USEC(FTM_GetCurrentTimerCount(s_FtmBase[halTimerState->instance]), halTimerState->timerClock_Hz);
}

hal_timer_status_t HAL_TimerUpdateTimeout(hal_timer_handle_t halTimerHandle, uint32_t timeout)
{
    uint32_t tickCount;
    assert(halTimerHandle);
    hal_timer_handle_struct_t *halTimerState = halTimerHandle;
    halTimerState->timeout                   = timeout;
    tickCount                                = USEC_TO_COUNT(halTimerState->timeout, halTimerState->timerClock_Hz);
    if ((tickCount < 1) || (tickCount > 0xfff0))
        return kStatus_HAL_TimerOutOfRanger;
    FTM_SetTimerPeriod(s_FtmBase[halTimerState->instance], tickCount);
    return kStatus_HAL_TimerSuccess;
}

void HAL_TimerExitLowpower(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
}

void HAL_TimerEnterLowpower(hal_timer_handle_t halTimerHandle)
{
    assert(halTimerHandle);
}
