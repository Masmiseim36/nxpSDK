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
#define MLIB_Sub4_F16_Asmi(f16Min, f16Sub1, f16Sub2, f16Sub3)                   \
        MLIB_Sub4_F16_FAsmi(f16Min, f16Sub1, f16Sub2, f16Sub3)
#define MLIB_Sub4Sat_F16_Asmi(f16Min, f16Sub1, f16Sub2, f16Sub3)                \
        MLIB_Sub4Sat_F16_FAsmi(f16Min, f16Sub1, f16Sub2, f16Sub3)

/***************************************************************************//*!
*  Subtraction of three input values from the first input
*     f16Out = f16Min - f16Sub1 - f16Sub2 - f16Sub3
*  function is not saturated
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_Sub4_F16_FAsmi(register frac16_t f16Min, register frac16_t f16Sub1,
                                           register frac16_t f16Sub2, register frac16_t f16Sub3)
{
    #if defined(__CC_ARM)                              /* For ARM Compiler */
        __asm volatile{sub f16Min, f16Min, f16Sub1     /* f16Min = f16Min - f16Sub1 */
                       sub f16Min, f16Min, f16Sub2     /* f16Min = f16Min - f16Sub2 */
                       sub f16Min, f16Min, f16Sub3};   /* f16Min = f16Min - f16Sub3 */
    #else
        __asm volatile( "sub %0, %0, %1 \n"            /* f16Min = f16Min - f16Sub1 */
                        "sub %0, %0, %2 \n"            /* f16Min = f16Min - f16Sub2 */
                        "sub %0, %0, %3 \n"            /* f16Min = f16Min - f16Sub3 */
                        : "+l"(f16Min): "l"(f16Sub1), "l"(f16Sub2), "l"(f16Sub3));
    #endif

    return f16Min;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*  Subtraction of three input values from the first input
*     f16Out = f16Min - f16Sub1 - f16Sub2 - f16Sub3
*  function is saturated
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_Sub4Sat_F16_FAsmi(register frac16_t f16Min, register frac16_t f16Sub1,
                                              register frac16_t f16Sub2, register frac16_t f16Sub3)
{        
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ sxth f16Min, f16Min                 /* Transforms 16-bit input f16Val to 32-bit */
                        sxth f16Sub1, f16Sub1               /* Transforms 16-bit input f16Val to 32-bit */
                        sxth f16Sub2, f16Sub2               /* Transforms 16-bit input f16Val to 32-bit */
                        sxth f16Sub3, f16Sub3               /* Transforms 16-bit input f16Val to 32-bit */
                        sub f16Min, f16Min, f16Sub2         /* f16Min - f16Sub2 */
                        add f16Sub1, f16Sub1, f16Sub3       /* f16Sub1 + f16Sub3 */
                        sub f16Min, f16Min, f16Sub1         /* f16Min - f16Sub2 - f16Sub1 - f16Sub3 */
                        ssat f16Min, #16, f16Min };         /* Saturation */
    #else
        __asm volatile( "sxth %0, %0 \n"                     /* Transforms 16-bit input f16Val to 32-bit */
                        "sxth %1, %1 \n"                     /* Transforms 16-bit input f16Val to 32-bit */
                        "sxth %2, %2 \n"                     /* Transforms 16-bit input f16Val to 32-bit */
                        "sxth %3, %3 \n"                     /* Transforms 16-bit input f16Val to 32-bit */
                        "sub %0, %0, %2 \n"                  /* f16Min - f16Sub2 */
                        "add %1, %1, %3 \n"                  /* f16Sub1 + f16Sub3 */
                        "sub %0, %0, %1 \n"                  /* f16Min - f16Sub2 - f16Sub1 - f16Sub3 */
                        "ssat %0, #16, %0 \n"                /* Saturation */
                        : "+l"(f16Min), "+l"(f16Sub1), "+l"(f16Sub2), "+l"(f16Sub3):);
    #endif

    return f16Min;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif /* _MLIB_SUB4_F16_ASM_H_ */

#endif 
