/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
* @brief  Negation
* 
*******************************************************************************/
#ifndef _MLIB_NEG_F16_ASM_H_
#define _MLIB_NEG_F16_ASM_H_

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
#define MLIB_NegSat_F16_Asmi(f16Val) MLIB_NegSat_F16_FAsmi(f16Val)

/***************************************************************************//*!
*
* Output = -f16Val
* With saturation
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_NegSat_F16_FAsmi(register frac16_t f16Val)
{
    register frac32_t f32SatVal = 0x8000;
    #if defined(__CC_ARM)                                 /* For ARM Compiler */
        __asm volatile{ sxth f16Val, f16Val               /* Transforms 16-bit input f16Val to 32-bit */
                        rsbs f16Val, f16Val, #0           /* f16Val = - f16Val */
                        cmp f16Val, f32SatVal             /* Compares f16Val with 0x00008000 */
                        bne SatEnd                        /* If f32Val != 0x8000, then goes to SatEnd */
                        subs f16Val, f16Val, #1           /* Compares f16Val with 0x00008000 */
        SatEnd: };
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "sxth %0, %0 \n\t"                /* Transforms 16-bit input f16Val to 32-bit */
                        "rsbs %0, %0, #0 \n\t"            /* f16Val = - f16Val */
                        "cmp %0, %1 \n\t"                 /* Compares f16Val with 0x00008000 */
                        "bne MLIB_NegSat_F16_SatEnd%= \n\t" /* If f32Val != 0x8000, then jumps through next command */
                        "subs %0, %0, #1 \n\t"            /* Compares f16Val with 0x00008000 */
                    "MLIB_NegSat_F16_SatEnd%=: \n\t"    
                        : "+l"(f16Val): "l"(f32SatVal));
    #else
        __asm volatile(
                        #if defined(__GNUC__)             /* For GCC compiler */
                            ".syntax unified \n"          /* Using unified asm syntax */
                        #endif  
                        "sxth %0, %0 \n"                  /* Transforms 16-bit input f16Val to 32-bit */
                        "rsbs %0, %0, #0 \n"              /* f16Val = - f16Val */
                        "cmp %0, %1 \n"                   /* Compares f16Val with 0x00008000 */
                        "bne .+4 \n"                      /* If f32Val != 0x8000, then jumps through next command */
                        "subs %0, %0, #1 \n"              /* Compares f16Val with 0x00008000 */
                        #if defined(__GNUC__)             /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Val): "l"(f32SatVal));
    #endif

    return f16Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_NEG_F16_ASM_H_ */
