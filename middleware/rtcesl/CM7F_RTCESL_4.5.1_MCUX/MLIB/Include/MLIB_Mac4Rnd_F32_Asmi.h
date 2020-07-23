/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
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
* 
*
****************************************************************************//*!
*
* @brief  Multiply accumulate of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MAC4RND_F32_ASM_H_
#define _MLIB_MAC4RND_F32_ASM_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Mac4RndSat_F32_Asmi(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)  \
        MLIB_Mac4RndSat_F32_FAsmi(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)
  
/***************************************************************************//*!
*
* f32Out = (f32Add1Mul1 * f32Add1Mul2) + (f32Add2Mul1 * f32Add2Mul2)
* With saturation
*******************************************************************************/ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac32_t MLIB_Mac4RndSat_F32_FAsmi(register frac32_t f32Add1Mul1, register frac32_t f32Add1Mul2, 
                                                 register frac32_t f32Add2Mul1, register frac32_t f32Add2Mul2)
{
    #if defined(__CC_ARM)                                                        /* For ARM Compiler */
        __asm volatile{ smull f32Add1Mul2, f32Add1Mul1, f32Add1Mul2, f32Add1Mul1 /* f16Add1Mul1 * f16Add1Mul2 */
                        smlal f32Add1Mul2, f32Add1Mul1, f32Add2Mul2, f32Add2Mul1 /* f16Add1Mul1 * f16Add1Mul2 + f16Add2Mul1 * f16Add2Mul2 */
                        adds f32Add1Mul2, f32Add1Mul2, #0x40000000               /* Rounding */
                        adc f32Add1Mul1, f32Add1Mul1, #0                         /* Adds carry bit */
                        cmp f32Add1Mul1, #0x80000000                             /* Compares the high part of result with fractional (-1) */
                        it eq                                                    /* If result is fractional (-1), then saturates */
                        subeq f32Add1Mul1, f32Add1Mul1, #1                       /* result = result - 1 */
                        add f32Add2Mul1, f32Add1Mul1, f32Add1Mul1                /* Converts high part of result to fractional value */
                        qadd f32Add1Mul1, f32Add1Mul1, f32Add1Mul1               /* Converts high part of result to fractional value with saturation */
                        cmp f32Add1Mul1, f32Add2Mul1                             /* Compares result with saturation and result without saturation */
                        it eq                                                    /* If results are equal, then adds the last bit */
                        orreq f32Add1Mul1, f32Add1Mul1, f32Add1Mul2, lsr #31 };  /* Result = Result + Last bit */
    #else
        __asm volatile( "smull %1, %0, %1, %0 \n"                                /* f16Add1Mul1 * f16Add1Mul2 */
                        "smlal %1, %0, %3, %2 \n"                                /* f16Add1Mul1 * f16Add1Mul2 + f16Add2Mul1 * f16Add2Mul2 */
                        "adds %1, %1, #0x40000000 \n"                            /* Rounding */
                        "adc %0, %0, #0 \n"                                      /* Adds carry bit */
                        "cmp %0, #0x80000000 \n"                                 /* Compares the high part of result with fractional (-1) */
                        "it eq \n"                                               /* If result is fractional (-1), then saturates */
                        "subeq %0, %0, #1 \n"                                    /* result = result - 1 */
                        "add %2, %0, %0 \n"                                      /* Converts high part of result to fractional value */
                        "qadd %0, %0, %0 \n"                                     /* Converts high part of result to fractional value with saturation */
                        "cmp %0, %2 \n"                                          /* Compares result with saturation and result without saturation */
                        "it eq \n"                                               /* If results are equal, then adds the last bit */
                        "orreq %0, %0, %1, lsr #31 \n"                           /* Result = Result + Last bit */
                        : "+l"(f32Add1Mul1), "+l"(f32Add1Mul2), "+l"(f32Add2Mul1), "+l"(f32Add2Mul2):);
    #endif

    return f32Add1Mul1;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4RND_F32_ASM_H_ */
