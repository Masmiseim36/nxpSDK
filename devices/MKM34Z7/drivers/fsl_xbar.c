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

#include "fsl_xbar.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the XBAR instance from peripheral base address.
 *
 * @param base XBAR peripheral base address.
 * @return XBAR instance.
 */
static uint32_t XBAR_GetInstance(XBAR_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Array of XBAR peripheral base address. */
static XBAR_Type *const s_xbarBases[] = XBAR_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Array of XBAR clock name. */
static const clock_ip_name_t s_xbarClock[] = XBAR_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t XBAR_GetInstance(XBAR_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_xbarBases); instance++)
    {
        if (s_xbarBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_xbarBases));

    return instance;
}

void XBAR_Init(XBAR_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable XBAR module clock. */
    CLOCK_EnableClock(s_xbarClock[XBAR_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void XBAR_Deinit(XBAR_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable XBAR module clock. */
    CLOCK_DisableClock(s_xbarClock[XBAR_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void XBAR_SetSignalsConnection(XBAR_Type *base, xbar_input_signal_t input, xbar_output_signal_t output)
{
    XBAR_WR_SELx_SELx(base, (((uint16_t)input) & 0xFFU), (((uint16_t)output) & 0xFFU));
}

uint32_t XBAR_GetStatusFlags(XBAR_Type *base)
{
    uint32_t status_flag;

    status_flag = ((base->CTRL0 & (XBAR_CTRL0_STS0_MASK | XBAR_CTRL0_STS1_MASK)) |
                   ((base->CTRL1 & (XBAR_CTRL1_STS2_MASK | XBAR_CTRL1_STS3_MASK)) << 16U));

    return status_flag;
}

void XBAR_ClearStatusFlags(XBAR_Type *base, uint32_t mask)
{
    uint16_t regVal;

    /* Assign regVal to CTRL0 register's value */
    regVal = (base->CTRL0);
    /* Perform this command to avoid writing 1 into interrupt flag bits */
    regVal &= (uint16_t)(~(XBAR_CTRL0_STS0_MASK | XBAR_CTRL0_STS1_MASK));
    /* Write 1 to interrupt flag bits corresponding to mask */
    regVal |= (uint16_t)(mask & (XBAR_CTRL0_STS0_MASK | XBAR_CTRL0_STS1_MASK));
    /* Write regVal value into CTRL0 register */
    base->CTRL0 = regVal;

    /* Assign regVal to CTRL1 register's value */
    regVal = (base->CTRL1);
    /* Perform this command to avoid writing 1 into interrupt flag bits */
    regVal &= (uint16_t)(~(XBAR_CTRL1_STS2_MASK | XBAR_CTRL1_STS3_MASK));
    /* Write 1 to interrupt flag bits corresponding to mask */
    regVal |= (uint16_t)((mask >> 16U) & (XBAR_CTRL1_STS2_MASK | XBAR_CTRL1_STS3_MASK));
    /* Write regVal value into CTRL1 register */
    base->CTRL1 = regVal;
}

void XBAR_SetOutputSignalConfig(XBAR_Type *base,
                                xbar_output_signal_t output,
                                const xbar_control_config_t *controlConfig)

{
    uint16_t regVal;
    /* Set active edge for edge detection, set interrupt or DMA function. */
    switch ((uint16_t)output)
    {
        case kXBAR_OutputXbDmaInt0:
            /* Assign regVal to CTRL0 register's value */
            regVal = base->CTRL0;
            /* Perform this command to avoid writing 1 into interrupt flag bits and clears bit DEN0, IEN0 */
            regVal &= (uint16_t)(
                ~(XBAR_CTRL0_STS0_MASK | XBAR_CTRL0_STS1_MASK | XBAR_CTRL0_DEN0_MASK | XBAR_CTRL0_IEN0_MASK));
            /* Configure edge and request type */
            regVal |= (uint16_t)(XBAR_CTRL0_EDGE0(controlConfig->activeEdge) |
                                 ((controlConfig->requestType) << XBAR_CTRL0_DEN0_SHIFT));
            /* Write regVal value into CTRL0 register */
            base->CTRL0 = regVal;
            break;
        case kXBAR_OutputXbDmaInt1:
            /* Assign regVal to CTRL0 register's value */
            regVal = base->CTRL0;
            /* Perform this command to avoid writing 1 into interrupt flag bits and clears bit DEN1, IEN1 */
            regVal &= (uint16_t)(
                ~(XBAR_CTRL0_STS0_MASK | XBAR_CTRL0_STS1_MASK | XBAR_CTRL0_DEN1_MASK | XBAR_CTRL0_IEN1_MASK));
            /* Configure edge and request type */
            regVal |= (uint16_t)(XBAR_CTRL0_EDGE1(controlConfig->activeEdge) |
                                 ((controlConfig->requestType) << XBAR_CTRL0_DEN1_SHIFT));
            /* Write regVal value into CTRL0 register */
            base->CTRL0 = regVal;
            break;
        case kXBAR_OutputXbDmaInt2:
            /* Assign regVal to CTRL1 register's value */
            regVal = (base->CTRL1);
            /* Perform this command to avoid writing 1 into interrupt flag bits and clears bit DEN2, IEN2 */
            regVal &= (uint16_t)(
                ~(XBAR_CTRL1_STS2_MASK | XBAR_CTRL1_STS3_MASK | XBAR_CTRL1_DEN2_MASK | XBAR_CTRL1_IEN2_MASK));
            /* Configure edge and request type */
            regVal |= (uint16_t)((XBAR_CTRL1_EDGE2(controlConfig->activeEdge) | (controlConfig->requestType))
                                 << XBAR_CTRL1_DEN2_SHIFT);
            /* Write regVal value into CTRL1 register */
            base->CTRL1 = regVal;
            break;
        case kXBAR_OutputXbDmaInt3:
            /* Assign regVal to CTRL1 register's value */
            regVal = (base->CTRL1);
            /* Perform this command to avoid writing 1 into interrupt flag bits and clears bit DEN3, IEN3 */
            regVal &= (uint16_t)(
                ~(XBAR_CTRL1_STS2_MASK | XBAR_CTRL1_STS3_MASK | XBAR_CTRL1_DEN3_MASK | XBAR_CTRL1_IEN3_MASK));
            /* Configure edge and request type */
            regVal |= (uint16_t)(XBAR_CTRL1_EDGE3(controlConfig->activeEdge) |
                                 ((controlConfig->requestType) << XBAR_CTRL1_DEN3_SHIFT));
            /* Write regVal value into CTRL1 register */
            base->CTRL1 = regVal;
            break;
        default:
            break;
    }
}
