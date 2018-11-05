/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
* @brief  Subtraction of three input values from the first input
* 
*******************************************************************************/
#ifndef _MLIB_SUB4_F16_ASM_H_
#define _MLIB_SUB4_F16_ASM_H_

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
#define MLIB_Sub4Sat_F16_Asmi(f16Min, f16Sub1, f16Sub2, f16Sub3)               \
        MLIB_Sub4Sat_F16_FAsmi(f16Min, f16Sub1, f16Sub2, f16Sub3)

/***************************************************************************//*!
*  Subtraction of three input values from the first input
*     Output = f16Min - f16Sub1 - f16Sub2 - f16Sub3
*  function is saturated
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_Sub4Sat_F16_FAsmi(register frac16_t f16Min, register frac16_t f16Sub1, 
                                              register frac16_t f16Sub2, register frac16_t f16Sub3)
{        
    register frac32_t f32Val = 0x8000;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ sxth f16Min, f16Min             /* Transforms 16-bit input f16Min to 32-bit */
                        sxth f16Sub1, f16Sub1           /* Transforms 16-bit input f16Sub1 to 32-bit */
                        sxth f16Sub2, f16Sub2           /* Transforms 16-bit input f16Min to 32-bit */
                        sxth f16Sub3, f16Sub3           /* Transforms 16-bit input f16Sub1 to 32-bit */
                        subs f16Min, f16Min, f16Sub1    /* f16Min = f16Min - f16Sub1 */
                        subs f16Min, f16Min, f16Sub2    /* f16Min = f16Min - f16Sub2 */
                        subs f16Min, f16Min, f16Sub3    /* f16Min = f16Min - f16Sub3 */

                        cmp f32Val, f16Min              /* Compares f16Min with 0x8000 */
                        bgt NegTest                     /* If f16Min < 0x8000, then goes to NegTest */
                        subs f16Min, f32Val, #1         /* If f16Min >= 0x8000, then f16Min = 0x8000 */
                        b SatEnd                        /* Goes to SatEnd */
                    NegTest:
                        sxth f32Val, f32Val             /* f32Val = 0xFFFF8000 */
                        cmp f32Val, f16Min              /* Compares f16Min with 0xFFFF8000 */
                        ble SatEnd                      /* If f16Min >= 0xFFFF8000, then goes to SatEnd */
                        mov f16Min, f32Val              /* If f16Min < 0xFFFF8000, then f16Min = 0xFFFF8000 */
                    SatEnd: };
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "sxth %1, %1 \n"                /* Transforms 16-bit input f16Min to 32-bit */
                        "sxth %2, %2 \n"                /* Transforms 16-bit input f16Sub1 to 32-bit */
                        "sxth %3, %3 \n"                /* Transforms 16-bit input f16Min to 32-bit */
                        "sxth %4, %4 \n"                /* Transforms 16-bit input f16Sub1 to 32-bit */
                        "subs %1, %1, %2 \n"            /* f16Min = f16Min - f16Sub1 */
                        "subs %1, %1, %3 \n"            /* f16Min = f16Min - f16Sub2 */
                        "subs %1, %1, %4 \n"            /* f16Min = f16Min - f16Sub3 */

                        "cmp %0, %1 \n"                 /* Compares f16Min with 0x8000 */
                        "bgt .+6 \n"                    /* If f16Min < 0x8000, then jumps through two commands */
                        "subs %1, %0, #1 \n"            /* If f16Min >= 0x8000, then f16Min = 0x8000 */
                        "b .+10 \n"                     /* Jumps through four commands */

                        "sxth %0, %0 \n"                /* f32Val = 0xFFFF8000 */
                        "cmp %0, %1 \n"                 /* Compares f16Min with 0xFFFF8000 */
                        "ble .+4 \n"                    /* If f16Min >= 0xFFFF8000, then jumps through next commands */
                        "mov %1, %0 \n"                 /* If f16Min < 0xFFFF8000, then f16Min = 0xFFFF8000 */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Val), "+l"(f16Min), "+l"(f16Sub1), "+l"(f16Sub2), "+l"(f16Sub3):);
    #endif

    return f16Min;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif /* _MLIB_SUB4_F16_ASM_H_ */

#endif 
