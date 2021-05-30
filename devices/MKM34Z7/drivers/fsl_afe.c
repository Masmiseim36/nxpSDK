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

#include "fsl_afe.h"

/*******************************************************************************
* Variables
******************************************************************************/
/*! @brief Pointers to AFE bases for each instance. */
static AFE_Type *const s_AFEBases[] = AFE_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to AFE clocks for each instance. */
static const clock_ip_name_t s_AFEClocks[] = AFE_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
* Code
*******************************************************************************/
static uint32_t AFE_GetInstance(AFE_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_AFEBases); instance++)
    {
        if (s_AFEBases[instance] == base)
        {
            break;
        }
    }
    assert(instance < ARRAY_SIZE(s_AFEBases));

    return instance;
}

void AFE_Init(AFE_Type *base, const afe_config_t *config)
{
    assert(config != NULL);

    uint32_t regData = 0U;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable clock gate */
    CLOCK_EnableClock(s_AFEClocks[AFE_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
    regData = base->CKR & ~(AFE_CKR_CLS_MASK | AFE_CKR_DIV_MASK); /* Clear old clock select, divider value */
    regData |= AFE_CKR_CLS(config->clockSource) | AFE_CKR_DIV(config->clockDivider);
    base->CKR = regData;

    /* Clear old startup time, result format, low power mode value */
    regData = base->CR & ~(AFE_CR_STRTUP_CNT_MASK | AFE_CR_RESULT_FORMAT_MASK | AFE_CR_RST_B_MASK | AFE_CR_LPM_EN_MASK);
    /* Set new startup time, result format, low power mode value */
    regData |= AFE_CR_STRTUP_CNT((uint8_t)config->startupCount) | AFE_CR_RESULT_FORMAT(config->resultFormat) |
               AFE_CR_LPM_EN(config->enableLowPower);
    /* Write value to CR register */
    base->CR = regData;

    AFE_Enable(base, true); /* Enable the AFE after the initialization. */
}

void AFE_Deinit(AFE_Type *base)
{
    AFE_Enable(base, false); /* Disable the AFE module. */
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable clock gate */
    CLOCK_DisableClock(s_AFEClocks[AFE_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void AFE_GetDefaultConfig(afe_config_t *config)
{
    assert(config != NULL);

    /* Fills the user configure structure. */
    config->enableLowPower = false;
    config->resultFormat = kAFE_ResultFormatRight;
    config->clockDivider = kAFE_ClockDivider2;
    config->clockSource = kAFE_ClockSource1;
    /* Minimum value of startup time */
    config->startupCount = 2U;
}

void AFE_SetChannelConfig(AFE_Type *base, uint32_t channel, const afe_channel_config_t *config)
{
    assert(config != NULL);
    assert(channel < FSL_FEATURE_AFE_CHANNEL_NUMBER);

    uint32_t regData = 0U;

    regData = base->CFR[channel];
    /* Enable decimation filter. */
    regData |= AFE_CFR_DEC_EN_MASK;
    /* If bypass mode is enabled.  */
    if (config->channelMode != kAFE_BypassDisable)
    {
        regData &= ~(AFE_CFR_DEC_CLK_EDGE_SEL_MASK | AFE_CFR_DEC_CLK_INP_SEL_MASK);
        regData |= (AFE_CFR_BYP_MODE_MASK | AFE_CFR_DEC_CLK_EDGE_SEL((config->channelMode >> 1U) & 0x1U) |
                    AFE_CFR_DEC_CLK_INP_SEL(config->channelMode & 0x1U));
    }
    else
    {
        regData |= AFE_CFR_SD_MOD_EN_MASK;
        if (config->pgaGainSelect == kAFE_PgaDisable)
        {
            regData &= ~AFE_CFR_PGA_EN_MASK; /* PGA is disabled. */
        }
        else
        {
            /* Set PGA value*/
            regData &= ~AFE_CFR_PGA_GAIN_SEL_MASK;
            regData |= (AFE_CFR_PGA_EN_MASK | AFE_CFR_PGA_GAIN_SEL(config->pgaGainSelect));
        }
    }
    /* Clear old Decimator OverSample Ratio, Conversion Mode, Trigger Select value */
    regData &= ~(AFE_CFR_DEC_OSR_MASK | AFE_CFR_HW_TRG_MASK | AFE_CFR_CC_MASK);
    /* Set new Decimator OverSampling Ratio, Conversion Mode, Trigger Select value */
    regData |= AFE_CFR_DEC_OSR((uint32_t)config->decimatorOversampleRatio) |
               AFE_CFR_CC(config->enableContinuousConversion) | AFE_CFR_HW_TRG(config->enableHardwareTrigger);

    base->CFR[channel] = regData;
}

void AFE_GetDefaultChannelConfig(afe_channel_config_t *config)
{
    assert(config != NULL);

    /* Fills the channel configuration structure. */
    config->enableHardwareTrigger = false;
    config->enableContinuousConversion = false;
    config->channelMode = kAFE_BypassDisable;
    config->decimatorOversampleRatio = kAFE_DecimatorOversampleRatio64;
    config->pgaGainSelect = kAFE_PgaDisable;
}

uint32_t AFE_GetChannelConversionValue(AFE_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_AFE_CHANNEL_NUMBER);

    return base->RR[channel];
}

void AFE_SetChannelPhaseDelayValue(AFE_Type *base, uint32_t channel, uint32_t value)
{
    assert(channel < FSL_FEATURE_AFE_CHANNEL_NUMBER);

    /* Set delay value for this channel */
    base->DR[channel] = AFE_DR_DLY(value);
}

void AFE_EnableChannelDMA(AFE_Type *base, uint32_t mask, bool enable)
{
    uint32_t regData =
        mask & (kAFE_Channel0DMAEnable | kAFE_Channel1DMAEnable | kAFE_Channel2DMAEnable | kAFE_Channel3DMAEnable);

    if (enable)
    {
        base->DI |= regData;
    }
    else
    {
        base->DI &= ~regData;
    }
}
