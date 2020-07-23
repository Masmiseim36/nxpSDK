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
* @brief  Multiply 
* 
*******************************************************************************/
#ifndef _MLIB_MUL_A32_ASM_H_
#define _MLIB_MUL_A32_ASM_H_

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
#define MLIB_MulSat_F16as_Asmi(a32Accum, f16Mult)    MLIB_MulSat_F16as_FAsmi(a32Accum, f16Mult)
#define MLIB_MulNegSat_F16as_Asmi(a32Accum, f16Mult) MLIB_MulNegSat_F16as_FAsmi(a32Accum, f16Mult)
#define MLIB_MulSat_A32_Asmi(a32Mult1, a32Mult2)     MLIB_MulSat_A32_FAsmi(a32Mult1, a32Mult2)
#define MLIB_MulNegSat_A32_Asmi(a32Mult1, a32Mult2)  MLIB_MulNegSat_A32_FAsmi(a32Mult1, a32Mult2)
/***************************************************************************//*!
*
* f16Out = a32Accum * f16Mult
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_MulSat_F16as_FAsmi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ lsl f16Mult, f16Mult, #16                    /* f16Mult << 16 */
                        smull a32Accum, f16Mult, f16Mult, a32Accum   /* a32Accum * (f16Mult << 16) */
                        lsr a32Accum, a32Accum, #31                  /* a32Accum >> 31 */
                        qdadd f16Mult, a32Accum, f16Mult             /* Correction of result for fractional multiplication */
                        ssat f16Mult, #16, f16Mult };                /* Saturation */
    #else
        __asm volatile( "lsl %0, %0, #16 \n"                         /* f16Mult << 16 */
                        "smull %1, %0, %0, %1 \n"                    /* a32Accum * (f16Mult << 16) */
                        "lsr %1, %1, #31 \n"                         /* a32Accum >> 31 */
                        "qdadd %0, %1, %0 \n"                        /* Correction of result for fractional multiplication */
                        "ssat %0, #16, %0 \n"                        /* Saturation */
                        : "+l"(f16Mult), "+l"(a32Accum):);
    #endif

    return f16Mult;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f16Out = - a32Accum * f16Mult
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_MulNegSat_F16as_FAsmi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ lsl f16Mult, f16Mult, #16                    /* f16Mult << 16 */
                        smull a32Accum, f16Mult, f16Mult, a32Accum   /* a32Accum * (f16Mult << 16) */
                        lsr a32Accum, a32Accum, #31                  /* a32Accum >> 31 */
                        rsb a32Accum, a32Accum, #0                   /* a32Accum = - a32Accum */
                        qdsub f16Mult, a32Accum, f16Mult             /* f16Mult = a32Accum - 2 * f16Mult */
                        ssat f16Mult, #16, f16Mult };                /* Saturation */
    #else
        __asm volatile( "lsl %0, %0, #16 \n"                         /* f16Mult << 16 */
                        "smull %1, %0, %0, %1 \n"                    /* a32Accum * (f16Mult << 16) */
                        "lsr %1, %1, #31 \n"                         /* a32Accum >> 31 */
                        "rsb %1, %1, #0 \n"                          /* a32Accum = - a32Accum */
                        "qdsub %0, %1, %0 \n"                        /* f16Mult = a32Accum - 2 * f16Mult */
                        "ssat %0, #16, %0 \n"                        /* Saturation */
                        : "+l"(f16Mult), "+l"(a32Accum):);
    #endif

    return f16Mult;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline acc32_t MLIB_MulSat_A32_FAsmi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Out=0;
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull a32Mult1, a32Mult2, a32Mult2, a32Mult1 /* a32Mult1 * a32Mult2 */
                        ssat a32Out, #15, a32Mult2                   /* Saturates to a signed 15-bit value */
                        cmp a32Out, a32Mult2                         /* Compares saturated and unsaturated values */
                        lsl a32Out, a32Out, #17                      /* a32Out << 17 */
                        ite eq                                       /* Condition block */
                        addeq a32Out, a32Out, a32Mult1, lsr #15      /* If saturated and unsaturated values are equal, then does not saturate result */
                        qaddne a32Out, a32Out, a32Out };             /* If saturated and unsaturated values are not equal, then saturates result */
    #else
        __asm volatile( "smull %1, %2, %2, %1 \n"                    /* a32Mult1 * a32Mult2 */
                        "ssat %0, #15, %2 \n"                        /* Saturates to a signed 15-bit value */
                        "cmp %0, %2 \n"                              /* Compares saturated and unsaturated values */
                        "lsl %0, %0, #17 \n"                         /* a32Out << 17 */
                        "ite eq \n"                                  /* Condition block */
                        "addeq %0, %0, %1, lsr #15 \n"               /* If saturated and unsaturated values are equal, then does not saturate result */
                        "qaddne %0, %0, %0 \n"                       /* If saturated and unsaturated values are not equal, then saturates result */
                        : "=&l"(a32Out), "+l"(a32Mult1), "+l"(a32Mult2):);
    #endif

    return a32Out;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = - f32Mult1 * f32Mult2
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline acc32_t MLIB_MulNegSat_A32_FAsmi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Out=0;
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull a32Mult1, a32Mult2, a32Mult2, a32Mult1 /* a32Mult1 * a32Mult2 */
                        ssat a32Out, #15, a32Mult2                   /* Saturates to a signed 15-bit value */
                        cmp a32Out, a32Mult2                         /* Compares saturated and unsaturated values */
                        lsl a32Out, a32Out, #17                      /* a32Out << 17 */
                        ittee eq                                     /* Condition block */
                        addeq a32Out, a32Out, a32Mult1, lsr #15      /* If saturated and unsaturated values are equal, then does not saturate result */
                        rsbeq a32Out, a32Out, #0                     /* Negation */
                        qaddne a32Out, a32Out, a32Out                /* If saturated and unsaturated values are not equal, then saturates result */
                        mvnne a32Out, a32Out };                      /* Negation */
    #else
        __asm volatile( "smull %1, %2, %2, %1 \n"                    /* a32Mult1 * a32Mult2 */
                        "ssat %0, #15, %2 \n"                        /* Saturates to a signed 15-bit value */
                        "cmp %0, %2 \n"                              /* Compares saturated and unsaturated values */
                        "lsl %0, %0, #17 \n"                         /* a32Out << 17 */
                        "ittee eq \n"                                /* Condition block */
                        "addeq %0, %0, %1, lsr #15 \n"               /* If saturated and unsaturated values are equal, then does not saturate result */
                        "rsbeq %0, %0, #0 \n"                        /* Negation */
                        "qaddne %0, %0, %0 \n"                       /* If saturated and unsaturated values are not equal, then saturates result */
                        "mvnne %0, %0 \n"                            /* Negation */
                        : "=&l"(a32Out), "+l"(a32Mult1), "+l"(a32Mult2):);
    #endif

    return a32Out;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_A32_ASM_H_ */
