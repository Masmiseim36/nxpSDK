/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include "fsl_qtmr.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Gets the instance from the base address to be used to gate or ungate the module clock
 *
 * @param base Quad Timer peripheral base address
 *
 * @return The Quad Timer instance
 */
static uint32_t QTMR_GetInstance(TMR_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to Quad Timer bases for each instance. */
static TMR_Type *const s_qtmrBases[] = TMR_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to Quad Timer clocks for each instance. */
static const clock_ip_name_t s_qtmrClocks[] = TMR_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t QTMR_GetInstance(TMR_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_qtmrBases); instance++)
    {
        if (s_qtmrBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_qtmrBases));

    return instance;
}

void QTMR_Init(TMR_Type *base, const qtmr_config_t *config)
{
    assert(config);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the module clock */
    CLOCK_EnableClock(s_qtmrClocks[QTMR_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Setup the counter sources */
    base->CTRL = (TMR_CTRL_PCS(config->primarySource) | TMR_CTRL_SCS(config->secondarySource));

    /* Setup the master mode operation */
    base->SCTRL = (TMR_SCTRL_EEOF(config->enableExternalForce) | TMR_SCTRL_MSTR(config->enableMasterMode));

    /* Setup debug mode */
    base->CSCTRL = TMR_CSCTRL_DBG_EN(config->debugMode);

    /* Setup input filter */
    base->FILT = (TMR_FILT_FILT_CNT(config->faultFilterCount) | TMR_FILT_FILT_PER(config->faultFilterPeriod));
}

void QTMR_Deinit(TMR_Type *base)
{
    /* Stop the counter */
    base->CTRL &= ~TMR_CTRL_CM_MASK;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable the module clock */
    CLOCK_DisableClock(s_qtmrClocks[QTMR_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void QTMR_GetDefaultConfig(qtmr_config_t *config)
{
    assert(config);

    /* Halt counter during debug mode */
    config->debugMode = kQTMR_RunNormalInDebug;
    /* Another counter cannot force state of OFLAG signal */
    config->enableExternalForce = false;
    /* Compare function's output from this counter is not broadcast to other counters */
    config->enableMasterMode = false;
    /* Fault filter count is set to 0 */
    config->faultFilterCount = 0;
    /* Fault filter period is set to 0 which disables the fault filter */
    config->faultFilterPeriod = 0;
    /* Primary count source is IP bus clock divide by 2 */
    config->primarySource = kQTMR_ClockDivide_2;
    /* Secondary count source is counter 0 input pin */
    config->secondarySource = kQTMR_Counter0InputPin;
}

status_t QTMR_SetupPwm(
    TMR_Type *base, uint32_t pwmFreqHz, uint8_t dutyCyclePercent, bool outputPolarity, uint32_t srcClock_Hz)
{
    uint32_t periodCount, highCount, lowCount, reg;

    if (dutyCyclePercent > 100)
    {
        /* Invalid dutycycle */
        return kStatus_Fail;
    }

    /* Set OFLAG pin for output mode and force out a low on the pin */
    base->SCTRL |= (TMR_SCTRL_FORCE_MASK | TMR_SCTRL_OEN_MASK);

    /* Counter values to generate a PWM signal */
    periodCount = (srcClock_Hz / pwmFreqHz);
    highCount = (periodCount * dutyCyclePercent) / 100;
    lowCount = periodCount - highCount;

    /* Setup the compare registers for PWM output */
    base->COMP1 = lowCount;
    base->COMP2 = highCount;

    /* Setup the pre-load registers for PWM output */
    base->CMPLD1 = lowCount;
    base->CMPLD2 = highCount;

    reg = base->CSCTRL;
    /* Setup the compare load control for COMP1 and COMP2.
     * Load COMP1 when CSCTRL[TCF2] is asserted, load COMP2 when CSCTRL[TCF1] is asserted
     */
    reg &= ~(TMR_CSCTRL_CL1_MASK | TMR_CSCTRL_CL2_MASK);
    reg |= (TMR_CSCTRL_CL1(kQTMR_LoadOnComp2) | TMR_CSCTRL_CL2(kQTMR_LoadOnComp1));
    base->CSCTRL = reg;

    if (outputPolarity)
    {
        /* Invert the polarity */
        base->SCTRL |= TMR_SCTRL_OPS_MASK;
    }
    else
    {
        /* True polarity, no inversion */
        base->SCTRL &= ~TMR_SCTRL_OPS_MASK;
    }

    reg = base->CTRL;
    reg &= ~(TMR_CTRL_OUTMODE_MASK);
    /* Count until compare value is  reached and re-initialize the counter, toggle OFLAG output
     * using alternating compare register
     */
    reg |= (TMR_CTRL_LENGTH_MASK | TMR_CTRL_OUTMODE(kQTMR_ToggleOnAltCompareReg));
    base->CTRL = reg;

    return kStatus_Success;
}

void QTMR_SetupInputCapture(TMR_Type *base,
                            qtmr_input_source_t capturePin,
                            bool inputPolarity,
                            bool reloadOnCapture,
                            qtmr_input_capture_edge_t captureMode)
{
    uint16_t reg;

    /* Clear the prior value for the input source for capture */
    reg = base->CTRL & (~TMR_CTRL_SCS_MASK);

    /* Set the new input source */
    reg |= TMR_CTRL_SCS(capturePin);
    base->CTRL = reg;

    /* Clear the prior values for input polarity, capture mode. Set the external pin as input */
    reg = base->SCTRL & (~(TMR_SCTRL_IPS_MASK | TMR_SCTRL_CAPTURE_MODE_MASK | TMR_SCTRL_OEN_MASK));
    /* Set the new values */
    reg |= (TMR_SCTRL_IPS(inputPolarity) | TMR_SCTRL_CAPTURE_MODE(captureMode));
    base->SCTRL = reg;

    /* Setup if counter should reload when a capture occurs */
    if (reloadOnCapture)
    {
        base->CSCTRL |= TMR_CSCTRL_ROC_MASK;
    }
    else
    {
        base->CSCTRL &= ~TMR_CSCTRL_ROC_MASK;
    }
}

void QTMR_EnableInterrupts(TMR_Type *base, uint32_t mask)
{
    uint16_t reg;

    reg = base->SCTRL;
    /* Compare interrupt */
    if (mask & kQTMR_CompareInterruptEnable)
    {
        reg |= TMR_SCTRL_TCFIE_MASK;
    }
    /* Overflow interrupt */
    if (mask & kQTMR_OverflowInterruptEnable)
    {
        reg |= TMR_SCTRL_TOFIE_MASK;
    }
    /* Input edge interrupt */
    if (mask & kQTMR_EdgeInterruptEnable)
    {
        reg |= TMR_SCTRL_IEFIE_MASK;
    }
    base->SCTRL = reg;

    reg = base->CSCTRL;
    /* Compare 1 interrupt */
    if (mask & kQTMR_Compare1InterruptEnable)
    {
        reg |= TMR_CSCTRL_TCF1EN_MASK;
    }
    /* Compare 2 interrupt */
    if (mask & kQTMR_Compare2InterruptEnable)
    {
        reg |= TMR_CSCTRL_TCF2EN_MASK;
    }
    base->CSCTRL = reg;
}

void QTMR_DisableInterrupts(TMR_Type *base, uint32_t mask)
{
    uint16_t reg;

    reg = base->SCTRL;
    /* Compare interrupt */
    if (mask & kQTMR_CompareInterruptEnable)
    {
        reg &= ~TMR_SCTRL_TCFIE_MASK;
    }
    /* Overflow interrupt */
    if (mask & kQTMR_OverflowInterruptEnable)
    {
        reg &= ~TMR_SCTRL_TOFIE_MASK;
    }
    /* Input edge interrupt */
    if (mask & kQTMR_EdgeInterruptEnable)
    {
        reg &= ~TMR_SCTRL_IEFIE_MASK;
    }
    base->SCTRL = reg;

    reg = base->CSCTRL;
    /* Compare 1 interrupt */
    if (mask & kQTMR_Compare1InterruptEnable)
    {
        reg &= ~TMR_CSCTRL_TCF1EN_MASK;
    }
    /* Compare 2 interrupt */
    if (mask & kQTMR_Compare2InterruptEnable)
    {
        reg &= ~TMR_CSCTRL_TCF2EN_MASK;
    }
    base->CSCTRL = reg;
}

uint32_t QTMR_GetEnabledInterrupts(TMR_Type *base)
{
    uint32_t enabledInterrupts = 0;
    uint16_t reg;

    reg = base->SCTRL;
    /* Compare interrupt */
    if (reg & TMR_SCTRL_TCFIE_MASK)
    {
        enabledInterrupts |= kQTMR_CompareFlag;
    }
    /* Overflow interrupt */
    if (reg & TMR_SCTRL_TOFIE_MASK)
    {
        enabledInterrupts |= kQTMR_OverflowInterruptEnable;
    }
    /* Input edge interrupt */
    if (reg & TMR_SCTRL_IEFIE_MASK)
    {
        enabledInterrupts |= kQTMR_EdgeInterruptEnable;
    }

    reg = base->CSCTRL;
    /* Compare 1 interrupt */
    if (reg & TMR_CSCTRL_TCF1EN_MASK)
    {
        enabledInterrupts |= kQTMR_Compare1InterruptEnable;
    }
    /* Compare 2 interrupt */
    if (reg & TMR_CSCTRL_TCF2EN_MASK)
    {
        enabledInterrupts |= kQTMR_Compare2InterruptEnable;
    }

    return enabledInterrupts;
}

uint32_t QTMR_GetStatus(TMR_Type *base)
{
    uint32_t statusFlags = 0;
    uint16_t reg;

    reg = base->SCTRL;
    /* Timer compare flag */
    if (reg & TMR_SCTRL_TCF_MASK)
    {
        statusFlags |= kQTMR_CompareFlag;
    }
    /* Timer overflow flag */
    if (reg & TMR_SCTRL_TOF_MASK)
    {
        statusFlags |= kQTMR_OverflowFlag;
    }
    /* Input edge flag */
    if (reg & TMR_SCTRL_IEF_MASK)
    {
        statusFlags |= kQTMR_EdgeFlag;
    }

    reg = base->CSCTRL;
    /* Compare 1 flag */
    if (reg & TMR_CSCTRL_TCF1_MASK)
    {
        statusFlags |= kQTMR_Compare1Flag;
    }
    /* Compare 2 flag */
    if (reg & TMR_CSCTRL_TCF2_MASK)
    {
        statusFlags |= kQTMR_Compare2Flag;
    }

    return statusFlags;
}

void QTMR_ClearStatusFlags(TMR_Type *base, uint32_t mask)
{
    uint16_t reg;

    reg = base->SCTRL;
    /* Timer compare flag */
    if (mask & kQTMR_CompareFlag)
    {
        reg &= ~TMR_SCTRL_TCF_MASK;
    }
    /* Timer overflow flag */
    if (mask & kQTMR_OverflowFlag)
    {
        reg &= ~TMR_SCTRL_TOF_MASK;
    }
    /* Input edge flag */
    if (mask & kQTMR_EdgeFlag)
    {
        reg &= ~TMR_SCTRL_IEF_MASK;
    }
    base->SCTRL = reg;

    reg = base->CSCTRL;
    /* Compare 1 flag */
    if (mask & kQTMR_Compare1Flag)
    {
        reg &= ~TMR_CSCTRL_TCF1_MASK;
    }
    /* Compare 2 flag */
    if (mask & kQTMR_Compare2Flag)
    {
        reg &= ~TMR_CSCTRL_TCF2_MASK;
    }
    base->CSCTRL = reg;
}

void QTMR_SetTimerPeriod(TMR_Type *base, uint16_t ticks)
{
    /* Set the length bit to reinitialize the counters on a match */
    base->CTRL |= TMR_CTRL_LENGTH_MASK;

    if (base->CTRL & TMR_CTRL_DIR_MASK)
    {
        /* Counting down */
        base->COMP2 = ticks;
    }
    else
    {
        /* Counting up */
        base->COMP1 = ticks;
    }
}
