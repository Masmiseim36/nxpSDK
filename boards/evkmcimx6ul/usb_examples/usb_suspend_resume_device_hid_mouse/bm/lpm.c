/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef FSL_RTOS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "fsl_gpt.h"
#endif

#include "lpm.h"
#include "fsl_gpc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPM_GPC_IMR_NUM (sizeof(GPC->IMR) / sizeof(GPC->IMR[0]))

#ifdef FSL_RTOS_FREE_RTOS
/* Define the counter clock of the systick (GPT). For accuracy purpose,
 * please make LPM_SYSTICK_COUNTER_FREQ divisible by 32768, and times of
 * configTICK_RATE_HZ.
 */
#define LPM_SYSTICK_COUNTER_FREQ (32768)
#define LPM_COUNT_PER_TICK (LPM_SYSTICK_COUNTER_FREQ / configTICK_RATE_HZ)

struct _lpm_power_mode_listener
{
    lpm_power_mode_callback_t callback;
    void *data;
    struct _lpm_power_mode_listener *next;
};

typedef struct _lpm_power_mode_listener lpm_power_mode_listener_t;

typedef struct _lpm_gpt_context
{
    uint32_t CR;
    uint32_t PR;
    uint32_t IR;
    uint32_t OCR;
} lpm_gpt_context_t;
#endif

typedef struct _lpm_gic_context
{
    uint32_t C_CTLR;
    uint32_t C_PMR;
    uint32_t C_BPR;

    uint32_t D_CTLR;
    uint32_t D_ISENABLER[16];
    uint8_t D_IPRIORITYR[512];
} lpm_gic_context_t;

typedef void (*lpm_system_func_t)(uint32_t context);
typedef void (*freertos_tick_func_t)(void);

static volatile lpm_power_mode_t s_curMode;
static lpm_gic_context_t s_gicContext;

#ifdef FSL_RTOS_FREE_RTOS
static SemaphoreHandle_t s_mutex;
static bool s_pendingTick;
static lpm_power_mode_listener_t *s_listenerHead;
static lpm_power_mode_listener_t *s_listenerTail;
static lpm_gpt_context_t s_gptContext;

static void LPM_SuspendSystick(void);
static void LPM_ResumeSystick(void);
#endif

extern void LPM_Suspend(lpm_system_func_t func);
extern void LPM_SystemIdle(uint32_t context);
extern void LPM_SystemDsm(uint32_t context);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * ERR007265: CCM: When improper low-power sequence is used,
 * the SoC enters low power mode before the ARM core executes WFI.
 *
 * Software workaround:
 * 1) Software should trigger IRQ #32 (IOMUX) to be always pending
 *    by setting IOMUX_GPR1_GINT.
 * 2) Software should then unmask IRQ #32 in GPC before setting CCM
 *    Low-Power mode.
 * 3) Software should mask IRQ #32 right after CCM Low-Power mode
 *    is set (set bits 0-1 of CCM_CLPCR).
 *
 * Note that IRQ #32 is GIC SPI #0.
 */
static void LPM_SetClockMode(clock_mode_t mode, uint32_t clpcr)
{
    switch (mode)
    {
        case kCLOCK_ModeRun:
            CCM->CLPCR = clpcr;
            break;
        default:
            LPM_EnableWakeupSource(IOMUXC_IRQn);
            CCM->CLPCR = clpcr;
            LPM_DisableWakeupSource(IOMUXC_IRQn);
            break;
    }
}

#ifdef FSL_RTOS_FREE_RTOS /* Only FreeRTOS need tickless idle */
static void GPT1_IRQHandler(uint32_t giccIar, void *userParam)
{
    freertos_tick_func_t tickHandler = (freertos_tick_func_t)userParam;

    /* Call FreeRTOS tick handler */
    tickHandler();

    if (s_pendingTick)
    {
        /* This is the pending tick in tickless idle wait, we won't reset
         * compare value and leave the operation to next interrupt */
        s_pendingTick = false;
    }
    else
    {
        /* This is the first tick since the MCU left a low power mode the
         * compare value need to be reset to its default. */
        if (GPT_GetOutputCompareValue(GPT1, kGPT_OutputCompare_Channel1) != LPM_COUNT_PER_TICK - 1)
        {
            /* Counter will be reset and cause minor accuracy loss */
            GPT_SetOutputCompareValue(GPT1, kGPT_OutputCompare_Channel1, LPM_COUNT_PER_TICK - 1UL);
        }
    }
}

static uint32_t LPM_EnterTicklessIdle(uint32_t timeoutMilliSec, uint32_t *pCounter)
{
    uint32_t counter, expired = 0;
    uint32_t ms, maxMS;
    uint32_t flag;
    uint32_t timeoutTicks;

    if ((uint64_t)timeoutMilliSec * configTICK_RATE_HZ < 2 * 1000)
    {
        /* if timeout < 2 ticks, don't do tickless idle */
        return 0;
    }

    maxMS = 0xFFFFFFFFU / LPM_SYSTICK_COUNTER_FREQ * 1000;
    ms = timeoutMilliSec > maxMS ? maxMS : timeoutMilliSec;

    /* Calculate the GPT counter needed for timeout */
    timeoutTicks = (uint64_t)ms * configTICK_RATE_HZ / 1000;
    counter = timeoutTicks * LPM_COUNT_PER_TICK;

    GPT_StopTimer(GPT1);
    flag = GPT_GetStatusFlags(GPT1, kGPT_OutputCompare1Flag);
    if (flag)
    { /* tick already pending, just clear it and add 1 tick counter */
        GPT_ClearStatusFlags(GPT1, kGPT_OutputCompare1Flag);
        expired = LPM_COUNT_PER_TICK;
    }
    expired += GPT_GetCurrentTimerCount(GPT1);
    /* Minus those already expired to get accurate waiting counter */
    counter -= expired;

    /* Update compare channel1 value will reset counter */
    GPT_SetOutputCompareValue(GPT1, kGPT_OutputCompare_Channel1, counter - 1UL);
    /* Restart timer */
    GPT_StartTimer(GPT1);

    /* return waiting counter */
    *pCounter = counter;

    return timeoutTicks;
}

static void LPM_ExitTicklessIdle(uint32_t timeoutTicks, uint32_t timeoutCounter)
{
    uint32_t flag, counter, expired;
    uint32_t completeTicks;

    GPT_StopTimer(GPT1);
    flag = GPT_GetStatusFlags(GPT1, kGPT_OutputCompare1Flag);
    expired = GPT_GetCurrentTimerCount(GPT1);
    if (flag)
    {
        /* Tick already pending, indicates idle timeout. Complete ticks
         * minus 1 because pending interrupt will be handled immediately
         * when interrupt unmasked.
         */
        completeTicks = timeoutTicks - 1;
        /* If counter already exceeds 1 tick, then it means wakeup takes too
         * much time and we have already lost some ticks. This is a common
         * case in low power idle (recovery need about 3.8ms) if ticks HZ
         * is high.
         */
        counter = expired < LPM_COUNT_PER_TICK ? LPM_COUNT_PER_TICK - expired : LPM_COUNT_PER_TICK;
        s_pendingTick = true;
    }
    else
    {
        /* remaining counter */
        counter = timeoutCounter - expired;
        completeTicks = timeoutTicks - (counter - 1) / LPM_COUNT_PER_TICK - 1;
        counter = (counter - 1) % LPM_COUNT_PER_TICK + 1;
    }
    /* Update compare channel1 value will reset counter */
    GPT_SetOutputCompareValue(GPT1, kGPT_OutputCompare_Channel1, counter - 1UL);
    /* Restart timer */
    GPT_StartTimer(GPT1);

    vTaskStepTick(completeTicks);
}
#endif /* FSL_RTOS_FREE_RTOS */

static void LPM_DisableRbcBypass(void)
{
    uint32_t gpcIMR[LPM_GPC_IMR_NUM];
    uint32_t i;

    /* Mask all GPC interrupts before disabling the RBC counters */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        gpcIMR[i] = GPC->IMR[i];
        GPC->IMR[i] = 0xFFFFFFFFU;
    }

    /* Disable the RBC bypass counter */
    CCM->CCR &= ~CCM_CCR_RBC_EN_MASK;
    CCM->CCR &= ~CCM_CCR_REG_BYPASS_COUNT_MASK;

    /* Now delay for 2 CKIL cycles (61usec). ARM is at 528MHz at this point
     * so a short loop should be enough.
     */
    for (i = 0; i < 528 * 22; i++)
    {
        __NOP();
    }

    /* Recover all the GPC interrupts. */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = gpcIMR[i];
    }
}

bool LPM_Init(void)
{
    uint32_t i;
#ifdef FSL_RTOS_FREE_RTOS
    s_mutex = xSemaphoreCreateMutex();

    if (s_mutex == NULL)
    {
        return false;
    }

    s_listenerHead = s_listenerTail = NULL;
#endif

    s_curMode = LPM_PowerModeRun;
    CLOCK_SetMode(kCLOCK_ModeRun);

    CCM->CGPR |= CCM_CGPR_INT_MEM_CLK_LPM_MASK;

    /* Enable RC OSC. It needs at least 4ms to be stable, so self tuning need to be enabled. */
    PMU->LOWPWR_CTRL |= PMU_LOWPWR_CTRL_RC_OSC_EN_MASK;
    /* Configure RC OSC */
    XTALOSC24M->OSC_CONFIG0 = XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG_CUR(0x4) | XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG(0xA7) |
                              XTALOSC24M_OSC_CONFIG0_START_MASK | XTALOSC24M_OSC_CONFIG0_ENABLE_MASK;
    XTALOSC24M->OSC_CONFIG1 = XTALOSC24M_OSC_CONFIG1_COUNT_RC_CUR(0x40) | XTALOSC24M_OSC_CONFIG1_COUNT_RC_TRG(0x2DC);
    /* Wait at least 4ms */
    for (i = 0; i < 1000 * 1000; i++)
    {
        __NOP();
    }
    /* Add some hysteresis */
    XTALOSC24M->OSC_CONFIG0 |= XTALOSC24M_OSC_CONFIG0_SET_HYST_PLUS(3) | XTALOSC24M_OSC_CONFIG0_SET_HYST_MINUS(3);
    /* Set COUNT_1M_TRG */
    XTALOSC24M->OSC_CONFIG2 = (XTALOSC24M->OSC_CONFIG2 & ~XTALOSC24M_OSC_CONFIG2_SET_COUNT_1M_TRG_MASK) |
                              XTALOSC24M_OSC_CONFIG2_SET_COUNT_1M_TRG(0x2D7);
    /* Hardware requires to read OSC_CONFIG0 or OSC_CONFIG1 to make OSC_CONFIG2 write work */
    XTALOSC24M->OSC_CONFIG1 = XTALOSC24M->OSC_CONFIG1;

    /* ERR007265 */
    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_GINT_MASK;

    /* 6UL and 6ULL don't have MMDC CH0, CH0 handshake must be disabled */
    CCM->CLPCR |= 1U << 19;

    /* Initialize GPC to mask all IRQs */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = 0xFFFFFFFFU;
    }
    /* Enable GPC for low power wakeup */
    GPC_AllowIRQs(GPC);

    return true;
}

void LPM_Deinit(void)
{
#ifdef FSL_RTOS_FREE_RTOS
    if (s_mutex != NULL)
    {
        vSemaphoreDelete(s_mutex);
        s_mutex = NULL;
    }
#endif

    /* ERR007265 */
    IOMUXC_GPR->GPR1 &= ~IOMUXC_GPR_GPR1_GINT_MASK;
}

void LPM_EnableWakeupSource(uint32_t irq)
{
    GPC_EnableIRQ(GPC, irq);
}

void LPM_DisableWakeupSource(uint32_t irq)
{
    GPC_DisableIRQ(GPC, irq);
}

#ifdef FSL_RTOS_FREE_RTOS /* Only FreeRTOS supports listener notification */
bool LPM_NotifyPowerMode(lpm_power_mode_t mode)
{
    lpm_power_mode_listener_t *l1, *l2;
    bool ret = true;

    if (mode == s_curMode)
    {
        return ret;
    }

    /* Need to make sure the list of listeners is not changed when traversing the list. */
    xSemaphoreTake(s_mutex, portMAX_DELAY);

    for (l1 = s_listenerHead; l1 != NULL; l1 = l1->next)
    {
        if (l1->callback == NULL)
        {
            continue;
        }

        if (!l1->callback(s_curMode, mode, l1->data))
        {
            /* One stakeholder doesn't allow new mode */
            ret = false;
            break;
        }
    }

    if (!ret)
    {
        /* roll back the state change of previous listeners */
        for (l2 = s_listenerHead; l2 != l1; l2 = l2->next)
        {
            if (l2->callback == NULL)
            {
                continue;
            }

            l2->callback(mode, s_curMode, l2->data);
        }
    }

    xSemaphoreGive(s_mutex);

    return ret;
}
#endif /* FSL_RTOS_FREE_RTOS */

void LPM_SetPowerMode(lpm_power_mode_t mode)
{
    s_curMode = mode;
}

void LPM_WaitForInterrupt(uint32_t timeoutMilliSec)
{
    uint32_t irqMask;
#ifdef FSL_RTOS_FREE_RTOS /* Only FreeRTOS need tickless idle */
    uint32_t counter;
    uint32_t timeoutTicks;
#endif
    uint32_t clpcr;

    irqMask = DisableGlobalIRQ();

    clpcr = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));

#ifdef FSL_RTOS_FREE_RTOS /* Only FreeRTOS need tickless idle */
    if (s_curMode != LPM_PowerModeDsm)
    {
        timeoutTicks = LPM_EnterTicklessIdle(timeoutMilliSec, &counter);
    }
    else
    {
        /* Systick will be disabled in DSM mode, so tickless idle is not needed. */
        timeoutTicks = 0;
    }
#endif

    if (s_curMode == LPM_PowerModeRun)
    {
        /* Do nothing */
    }
    else if (s_curMode == LPM_PowerModeWait)
    {
        LPM_SetClockMode(kCLOCK_ModeWait, clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
        __DSB();
        __WFI();
        LPM_SetClockMode(kCLOCK_ModeRun, clpcr);
    }
    else if (s_curMode == LPM_PowerModeIdle)
    {
        LPM_SetClockMode(kCLOCK_ModeWait, clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
        LPM_Suspend(LPM_SystemIdle);
        /* RBC bypass enabled in LPM_SystemIdle */
        LPM_DisableRbcBypass();
        LPM_SetClockMode(kCLOCK_ModeRun, clpcr);
    }
    else if (s_curMode == LPM_PowerModeDsm)
    {
#ifdef FSL_RTOS_FREE_RTOS
        LPM_SuspendSystick();
#endif
        LPM_SetClockMode(kCLOCK_ModeStop, clpcr | CCM_CLPCR_LPM(kCLOCK_ModeStop) | CCM_CLPCR_VSTBY_MASK |
                                              CCM_CLPCR_STBY_COUNT_MASK | CCM_CLPCR_SBYOS_MASK);
        LPM_Suspend(LPM_SystemDsm);
        /* RBC bypass enabled in LPM_SystemDsm */
        LPM_DisableRbcBypass();
        LPM_SetClockMode(kCLOCK_ModeRun, clpcr);
#ifdef FSL_RTOS_FREE_RTOS
        LPM_ResumeSystick();
#endif
    }
    else
    {
        assert(false);
    }

#ifdef FSL_RTOS_FREE_RTOS /* Only FreeRTOS need tickless idle */
    if (timeoutTicks > 0)
    {
        LPM_ExitTicklessIdle(timeoutTicks, counter);
    }
#endif

    EnableGlobalIRQ(irqMask);
}

#ifdef FSL_RTOS_FREE_RTOS /* Only FreeRTOS supports listener notification */
void LPM_RegisterPowerListener(lpm_power_mode_callback_t callback, void *data)
{
    lpm_power_mode_listener_t *l = (lpm_power_mode_listener_t *)pvPortMalloc(sizeof(lpm_power_mode_listener_t));
    assert(l);

    l->callback = callback;
    l->data = data;
    l->next = NULL;

    xSemaphoreTake(s_mutex, portMAX_DELAY);

    if (s_listenerHead)
    {
        s_listenerTail->next = l;
        s_listenerTail = l;
    }
    else
    {
        s_listenerHead = s_listenerTail = l;
    }

    xSemaphoreGive(s_mutex);
}

void LPM_UnregisterPowerListener(lpm_power_mode_callback_t callback, void *data)
{
    lpm_power_mode_listener_t *l, *p = NULL;

    xSemaphoreTake(s_mutex, portMAX_DELAY);

    for (l = s_listenerHead; l != NULL; l = l->next)
    {
        if (l->callback == callback && l->data == data)
        {
            if (p)
            {
                p->next = l->next;
            }
            else
            {
                s_listenerHead = l->next;
            }

            if (l->next == NULL)
            {
                s_listenerTail = p;
            }

            vPortFree(l);
            break;
        }
        p = l;
    }

    xSemaphoreGive(s_mutex);
}
#endif

/************ Internal public API start **************/

#ifdef FSL_RTOS_FREE_RTOS /* Only FreeRTOS need tickless idle */
/* Leverage GPT1 to provide Systick */
void LPM_SetupSystick(uint32_t tickRateHz, void *tickHandler, uint32_t intPriority)
{
    gpt_config_t gptConfig;

    /* Install IRQ handler for GPT1 */
    SystemInstallIrqHandler(GPT1_IRQn, GPT1_IRQHandler, tickHandler);

    GPT_GetDefaultConfig(&gptConfig);

    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    gptConfig.enableMode = false;                     /* Keep counter when stop */
    gptConfig.enableRunInDoze = true;
    /* Initialize GPT module */
    GPT_Init(GPT1, &gptConfig);

    /* Divide GPT clock source to LPM_SYSTICK_COUNTER_FREQ */
    GPT_SetClockDivider(GPT1, 32768U / LPM_SYSTICK_COUNTER_FREQ);

    /* Use channel1 of GPT1 for regular tick count setting */
    GPT_SetOutputCompareValue(GPT1, kGPT_OutputCompare_Channel1, LPM_SYSTICK_COUNTER_FREQ / tickRateHz - 1UL);

    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(GPT1, kGPT_OutputCompare1InterruptEnable);

    /* Set interrupt priority */
    GIC_SetPriority(GPT1_IRQn, intPriority);
    /* Enable IRQ */
    GIC_EnableIRQ(GPT1_IRQn);

    /* Start GPT counter */
    GPT_StartTimer(GPT1);

    /* Set GPT1 as wakeup source in low power mode */
    LPM_EnableWakeupSource(GPT1_IRQn);
}

void LPM_ClearSystickFlag(void)
{
    GPT_ClearStatusFlags(GPT1, kGPT_OutputCompare1Flag);
}

static void LPM_SuspendSystick(void)
{
    s_gptContext.CR = GPT1->CR;
    s_gptContext.PR = GPT1->PR;
    s_gptContext.IR = GPT1->IR;

    GPT_StopTimer(GPT1);
}

static void LPM_ResumeSystick(void)
{
    GPT_SoftwareReset(GPT1);

    GPT1->PR = s_gptContext.PR;
    GPT1->OCR[0] = LPM_COUNT_PER_TICK;
    GPT1->IR = s_gptContext.IR;
    GPT1->CR = s_gptContext.CR;
}
#endif /* FSL_RTOS_FREE_RTOS */

void LPM_GicStateSave(void)
{
    uint32_t i, j;
    uint32_t irqRegs;
    GIC_Type *gic = (GIC_Type *)(__get_CBAR() & 0xFFFF0000UL);

    irqRegs = (gic->D_TYPER & 0x1FUL) + 1;

    s_gicContext.C_CTLR = gic->C_CTLR;
    s_gicContext.C_PMR = gic->C_PMR;
    s_gicContext.C_BPR = gic->C_BPR;

    s_gicContext.D_CTLR = gic->D_CTLR;
    for (i = 0; i < irqRegs; i++)
    {
        s_gicContext.D_ISENABLER[i] = gic->D_ISENABLER[i];

        for (j = 0; j < 32U; j++)
        {
            s_gicContext.D_IPRIORITYR[i * 32 + j] = gic->D_IPRIORITYR[i * 32 + j];
        }
    }
}

void LPM_GicStateRestore(void)
{
    uint32_t i, j;
    uint32_t irqRegs;
    GIC_Type *gic = (GIC_Type *)(__get_CBAR() & 0xFFFF0000UL);

    irqRegs = (gic->D_TYPER & 0x1FUL) + 1;

    for (i = 0; i < irqRegs; i++)
    {
        gic->D_ISENABLER[i] = s_gicContext.D_ISENABLER[i];

        for (j = 0; j < 32U; j++)
        {
            gic->D_IPRIORITYR[i * 32 + j] = s_gicContext.D_IPRIORITYR[i * 32 + j];
        }
    }

    gic->D_CTLR = s_gicContext.D_CTLR;

    gic->C_BPR = s_gicContext.C_BPR;
    gic->C_PMR = s_gicContext.C_PMR;
    gic->C_CTLR = s_gicContext.C_CTLR;
}
