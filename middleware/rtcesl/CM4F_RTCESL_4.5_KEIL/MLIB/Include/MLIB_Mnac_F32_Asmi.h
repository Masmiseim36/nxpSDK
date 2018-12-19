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
* @brief  Multiply negate accumulate
* 
*******************************************************************************/
#ifndef _MLIB_MNAC_F32_ASM_H_
#define _MLIB_MNAC_F32_ASM_H_

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
#define MLIB_Mnac_F32_Asmi(f32Accum, f32Mult1, f32Mult2)                       \
        MLIB_Mnac_F32_FAsmi(f32Accum, f32Mult1, f32Mult2)
#define MLIB_Mnac_F32lss_Asmi(f32Accum, f16Mult1, f16Mult2)                    \
        MLIB_Mnac_F32lss_FAsmi(f32Accum, f16Mult1, f16Mult2)
#define MLIB_MnacSat_F32_Asmi(f32Accum, f32Mult1, f32Mult2)                    \
        MLIB_MnacSat_F32_FAsmi(f32Accum, f32Mult1, f32Mult2)
#define MLIB_MnacSat_F32lss_Asmi(f32Accum, f16Mult1, f16Mult2)                 \
        MLIB_MnacSat_F32lss_FAsmi(f32Accum, f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f32Mult1 * f32Mult2)
* Without saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Mnac_F32_FAsmi(register frac32_t f32Accum, 
                                           register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult2, f32Mult1 /* R1:R2 = Mul1 * Mul2 */
                        lsr f32Mult2, f32Mult2, #31                  /* R2 >> 31 */
                        orr f32Mult1, f32Mult2, f32Mult1, lsl #1     /* (R1 << 1) | R2*/
                        sub f32Accum, f32Mult1, f32Accum };          /* Mul1 * Mul2 - Acc */
    #else
        __asm volatile( "smull %2, %1, %2, %1 \n"                    /* R1:R2 = Mul1 * Mul2 */
                        "lsr %2, %2, #31 \n"                         /* R2 >> 31 */
                        "orr %1, %2, %1, lsl #1 \n"                  /* (R1 << 1) | R2*/
                        "sub %0, %1, %0 \n"                          /* Mul1 * Mul2 - Acc */
                        : "+l"(f32Accum), "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f16Mult1 * f16Mult2) 
* Without saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Mnac_F32lss_FAsmi(register frac32_t f32Accum, 
                                              register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smulbb f16Mult1, f16Mult1, f16Mult2          /* f16Mult1 * f16Mult2 */
                        lsl f16Mult1, f16Mult1, #1                   /* f16Mult1 * f16Mult2 << 1*/ 
                        sub f32Accum, f16Mult1, f32Accum };          /* Substraction without saturation */
    #else                                                            
        __asm volatile( "smulbb %1, %1, %2 \n"                       /* f16Mult1 * f16Mult2 */
                        "lsl %1, %1, #1 \n"                          /* f16Mult1 * f16Mult2 << 1*/
                        "sub %0, %1, %0 \n"                          /* Substraction without saturation */
                        : "+l"(f32Accum), "+l"(f16Mult1): "l"(f16Mult2));
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f32Mult1 * f32Mult2)
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MnacSat_F32_FAsmi(register frac32_t f32Accum, 
                                              register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult2, f32Mult1 /* R1:R2 = Mul1 * Mul2 */
                        lsr f32Mult2, f32Mult2, #31                  /* R2 >> 31 */
                        orr f32Mult1, f32Mult2, f32Mult1, lsl #1     /* (R1 << 1) | R2 */
                        qsub f32Accum, f32Mult1, f32Accum };         /* (Mul1 * Mul2) - Acc */
    #else
        __asm volatile( "smull %2, %1, %2, %1 \n"                    /* R1:R2 = Mul1 * Mul2 */
                        "lsr %2, %2, #31 \n"                         /* R2 >> 31 */
                        "orr %1, %2, %1, lsl #1 \n"                  /* (R1 << 1) | R2 */
                        "qsub %0, %1, %0 \n"                         /* (Mul1 * Mul2) - Acc */
                        : "+l"(f32Accum), "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f16Mult1 * f16Mult2) 
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MnacSat_F32lss_FAsmi(register frac32_t f32Accum, 
                                                 register frac16_t f16Mult1, register frac16_t f16Mult2)
{   register frac32_t f32Out = 0;
    #if defined(__CC_ARM)                                      /* For ARM Compiler */
        __asm volatile{ smulbb f16Mult1, f16Mult1, f16Mult2    /* f16Mult1 * f16Mult2 */
                        qsub f32Accum, f32Out, f32Accum        /* Accumulator negation */
                        qdadd f32Accum, f32Accum, f16Mult1};   /* Addition with saturation */
    #else                                                      
        __asm volatile( "smulbb %2, %2, %3 \n"                 /* f16Mult1 * f16Mult2 */                      
                        "qsub %1, %0, %1 \n"                   /* Accumulator negation */ 
                        "qdadd %1, %1, %2 \n"                  /* Addition with saturation */
                        : "+l"(f32Out), "+l"(f32Accum), "+l"(f16Mult1): "l"(f16Mult2));
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MNAC_F32_ASM_H_ */
