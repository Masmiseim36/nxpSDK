/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
*
****************************************************************************//*!
*
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_A32_ASM_H_
#define _MLIB_MULRND_A32_ASM_H_

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
#define MLIB_MulRndSat_F16as_Asmi(a32Accum, f16Mult)    MLIB_MulRndSat_F16as_FAsmi(a32Accum, f16Mult)
#define MLIB_MulNegRndSat_F16as_Asmi(a32Accum, f16Mult) MLIB_MulNegRndSat_F16as_FAsmi(a32Accum, f16Mult)
#define MLIB_MulRndSat_A32_Asmi(a32Mult1, a32Mult2)     MLIB_MulRndSat_A32_FAsmi(a32Mult1, a32Mult2)
#define MLIB_MulNegRndSat_A32_Asmi(a32Mult1, a32Mult2)  MLIB_MulNegRndSat_A32_FAsmi(a32Mult1, a32Mult2)
/***************************************************************************//*!
*
* f16Out = a32Accum * f16Mult
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac16_t MLIB_MulRndSat_F16as_FAsmi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ lsl f16Mult, f16Mult, #16                    /* f16Mult << 16 */
                        smull a32Accum, f16Mult, f16Mult, a32Accum   /* a32Accum * (f16Mult << 16) */
                        lsrs a32Accum, a32Accum, #31                 /* a32Accum >> 31 */
                        adc a32Accum, a32Accum, #0                   /* Rounding */
                        qdadd f16Mult, a32Accum, f16Mult             /* Correction of result for fractional multiplication */
                        ssat f16Mult, #16, f16Mult };                /* Saturation */
    #else
        __asm volatile( "lsl %0, %0, #16 \n"                         /* f16Mult << 16 */
                        "smull %1, %0, %0, %1 \n"                    /* a32Accum * (f16Mult << 16) */
                        "lsrs %1, %1, #31 \n"                        /* a32Accum >> 31 */
                        "adc %1, %1, #0 \n"                          /* Rounding */
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
static inline frac16_t MLIB_MulNegRndSat_F16as_FAsmi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ lsl f16Mult, f16Mult, #16                    /* f16Mult << 16 */
                        smull a32Accum, f16Mult, f16Mult, a32Accum   /* a32Accum * (f16Mult << 16) */
                        lsrs a32Accum, a32Accum, #31                 /* a32Accum >> 31 */
                        adc a32Accum, a32Accum, #0                   /* Rounding */
                        rsb a32Accum, a32Accum, #0                   /* a32Accum = - a32Accum */
                        qdsub f16Mult, a32Accum, f16Mult             /* f16Mult = a32Accum - 2 * f16Mult */
                        ssat f16Mult, #16, f16Mult };                /* Saturation */
    #else
        __asm volatile( "lsl %0, %0, #16 \n"                         /* f16Mult << 16 */
                        "smull %1, %0, %0, %1 \n"                    /* a32Accum * (f16Mult << 16) */
                        "lsrs %1, %1, #31 \n"                        /* a32Accum >> 31 */
                        "adc %1, %1, #0 \n"                          /* Rounding */
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
* a32Out = a32Mult1 * a32Mult2
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline acc32_t MLIB_MulRndSat_A32_FAsmi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Out=0;
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull a32Mult1, a32Mult2, a32Mult2, a32Mult1 /* a32Mult1 * a32Mult2 */
                        lsrs a32Mult1, a32Mult1, #15                 /* a32Mult1 >> 15 */
                        adc a32Mult1, a32Mult1, #0                   /* Rounding */
                        ssat a32Out, #15, a32Mult2                   /* Saturates to a signed 15-bit value */
                        cmp a32Out, a32Mult2                         /* Compares saturated and unsaturated values */
                        lsl a32Out, a32Out, #17                      /* a32Out << 17 */
                        it ne                                        /* Condition block */
                        movne a32Mult1, a32Out                       /* If saturated and unsaturated values are not equal, then a32Mult1 = a32Out */
                        qadd a32Out, a32Out, a32Mult1 };             /* a32Out = a32Out + a32Mult1 with saturation */
    #else
        __asm volatile( "smull %1, %2, %2, %1 \n"                    /* a32Mult1 * a32Mult2 */
                        "lsrs %1, %1, #15\n"                         /* a32Mult1 >> 15 */
                        "adc %1, %1, #0  \n"                         /* Rounding */
                        "ssat %0, #15, %2 \n"                        /* Saturates to a signed 15-bit value */
                        "cmp %0, %2 \n"                              /* Compares saturated and unsaturated values */
                        "lsl %0, %0, #17 \n"                         /* a32Out << 17 */
                        "it ne \n"                                   /* Condition block */
                        "movne %1, %0 \n"                            /* If saturated and unsaturated values are not equal, then a32Mult1 = a32Out */
                        "qadd %0, %0, %1 \n"                         /* a32Out = a32Out + a32Mult1 with saturation */
                        : "=&l"(a32Out), "+l"(a32Mult1), "+l"(a32Mult2):);
    #endif

    return a32Out;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* a32Out = - a32Mult1 * a32Mult2
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline acc32_t MLIB_MulNegRndSat_A32_FAsmi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Out=0;
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull a32Mult1, a32Mult2, a32Mult2, a32Mult1 /* a32Mult1 * a32Mult2 */
                        lsrs a32Mult1, a32Mult1, #15                 /* a32Mult1 >> 15 */
                        adc a32Mult1, a32Mult1, #-1                  /* a32Mult1 = a32Mult1 - 1 + Carry */
                        ssat a32Out, #15, a32Mult2                   /* Saturates to a signed 15-bit value */
                        cmp a32Out, a32Mult2                         /* Compares saturated and unsaturated values */
                        it ne                                        /* Condition block */
                        lslne a32Mult1, a32Out, #17                  /* If saturated and unsaturated values are not equal, then a32Mult1 = a32Out << 17 */
                        mvn a32Mult2, a32Out, lsl #17                /* a32Mult2 = -1 - a32Out << 17 */
                        qsub a32Out, a32Mult2, a32Mult1 };           /* a32Out = (-1 - a32Out << 17) - a32Mult1 */
    #else
        __asm volatile( "smull %1, %2, %2, %1 \n"                    /* a32Mult1 * a32Mult2 */
                        "lsrs %1, %1, #15\n"                         /* a32Mult1 >> 15 */
                        "adc %1, %1, #-1  \n"                        /* a32Mult1 = a32Mult1 - 1 + Carry */
                        "ssat %0, #15, %2 \n"                        /* Saturates to a signed 15-bit value */
                        "cmp %0, %2 \n"                              /* Compares saturated and unsaturated values */
                        "it ne \n"                                   /* Condition block */
                        "lslne %1, %0, #17 \n"                       /* If saturated and unsaturated values are not equal, then a32Mult1 = a32Out << 17 */
                        "mvn %2, %0, lsl #17 \n"                     /* a32Mult2 = -1 - a32Out << 17 */
                        "qsub %0, %2, %1 \n"                         /* a32Out = (-1 - a32Out << 17) - a32Mult1 */
                        : "=&l"(a32Out), "+l"(a32Mult1), "+l"(a32Mult2):);
    #endif

    return a32Out;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_A32_ASM_H_ */
