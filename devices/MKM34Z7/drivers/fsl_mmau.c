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

#include "fsl_mmau.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
void MMAU_Reset(MMAU_Type *base)
{
    /* Reset all MMAU register */
    base->X0 = 0;
    base->X1 = 0;
    base->X2 = 0;
    base->X3 = 0;
    base->A0 = 0;
    base->A1 = 0;
    base->CSR = 0;
    base->CSR_IF_CLR = 0;
}

void MMAU_ClearInterruptFlags(MMAU_Type *base, uint32_t mask)
{
    uint32_t regVal;

    /* Assign regVal to MMAU_CSR_IF_CLR register's value */
    regVal = base->CSR_IF_CLR;
    /* Perform this command to avoid writing 1 into interrupt flag bits, which do not corresponding to the mask */
    regVal &=
        (uint32_t)(~(kMMAU_AccumOverflowInterruptFlag | kMMAU_OverflowInterruptFlag | kMMAU_DivideByZeroInterruptFlag));
    /* Write 1 to interrupt flag bits corresponding to mask */
    regVal |= mask & (kMMAU_AccumOverflowInterruptFlag | kMMAU_OverflowInterruptFlag | kMMAU_DivideByZeroInterruptFlag);
    /* Write regVal's value into MMAU_CSR_IF_CLR register */
    base->CSR_IF_CLR = regVal;
}

void MMAU_SetInstructionFlags(MMAU_Type *base, uint32_t mask)
{
    uint32_t regVal;

    /* Assign regVal to MMAU_CSR_IF_CLR register's value */
    regVal = base->CSR_IF_CLR;
    /* Write 1 to interrupt flag bits corresponding to mask */
    regVal |= mask & (kMMAU_AccumOverflowInstructionFlag | kMMAU_OverflowInstructionFlag |
                      kMMAU_DivideByZeroInstructionFlag | kMMAU_NegativeInstructionFlag);
    /* Perform this command to avoid writing 1 into interrupt flag bits, which do not corresponding to the mask */
    regVal &=
        (uint32_t)(~(kMMAU_AccumOverflowInterruptFlag | kMMAU_OverflowInterruptFlag | kMMAU_DivideByZeroInterruptFlag));
    /* Write regVal's value into MMAU_CSR_IF_CLR register */
    base->CSR_IF_CLR = regVal;
}

void MMAU_ClearInstructionFlags(MMAU_Type *base, uint32_t mask)
{
    uint32_t regVal;

    /* Assign regVal to MMAU_CSR_IF_CLR register's value */
    regVal = base->CSR_IF_CLR;
    /* Write 0 to interrupt flag bits corresponding to mask */
    regVal &= ~(mask & (kMMAU_AccumOverflowInstructionFlag | kMMAU_OverflowInstructionFlag |
                        kMMAU_DivideByZeroInstructionFlag | kMMAU_NegativeInstructionFlag));
    /* Perform this command to avoid writing 1 into interrupt flag bits, which do not corresponding to the mask */
    regVal &=
        (uint32_t)(~(kMMAU_AccumOverflowInterruptFlag | kMMAU_OverflowInterruptFlag | kMMAU_DivideByZeroInterruptFlag));
    /* Write regVal's value into MMAU_CSR_IF_CLR register */
    base->CSR_IF_CLR = regVal;
}
