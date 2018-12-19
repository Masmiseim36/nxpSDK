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
* @brief  Multiply negate accumulate with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MNACRND_F32_ASM_H_
#define _MLIB_MNACRND_F32_ASM_H_

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
#define MLIB_MnacRnd_F32_Asmi(f32Accum, f32Mult1, f32Mult2)                    \
        MLIB_MnacRnd_F32_FAsmi(f32Accum, f32Mult1, f32Mult2)
#define MLIB_MnacRndSat_F32_Asmi(f32Accum, f32Mult1, f32Mult2)                 \
        MLIB_MnacRndSat_F32_FAsmi(f32Accum, f32Mult1, f32Mult2)
#define MLIB_MnacRndSat_F32lls_Asmi(f32Accum, f32Mult1, f16Mult2)              \
        MLIB_MnacRndSat_F32lls_FAsmi(f32Accum, f32Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f32Mult1 * f32Mult2)
* Without saturation
*******************************************************************************/ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MnacRnd_F32_FAsmi(register frac32_t f32Accum, 
                                              register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult2, f32Mult1 /* R1:R2 = Mul1 * Mul2 */
                        adds f32Mult2, f32Mult2, #0x40000000         /* Rounding */
                        adc f32Mult1, f32Mult1, #0                   /* Adds carry */
                        lsr f32Mult2, f32Mult2, #31                  /* R2 >> 31 */
                        orr f32Mult1, f32Mult2, f32Mult1, lsl #1     /* (R1 << 1) | R2*/
                        sub f32Accum, f32Mult1, f32Accum };          /* Mul1 * Mul2 - Acc */
    #else
        __asm volatile( "smull %2, %1, %2, %1 \n"                    /* R1:R2 = Mul1 * Mul2 */
                        "adds %2, %2, #0x40000000 \n"                /* Rounding */
                        "adc %1, %1, #0 \n"                          /* Adds carry */
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
* f32Out = - f32Accum + ( f32Mult1 * f32Mult2)
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MnacRndSat_F32_FAsmi(register frac32_t f32Accum, 
                                                 register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult2, f32Mult1 /* R1:R2 = Mul1 * Mul2 */
                        adds f32Mult2, f32Mult2, #0x40000000         /* Rounding */
                        adc f32Mult1, f32Mult1, #0                   /* Adds carry*/
                        lsr f32Mult2, f32Mult2, #31                  /* R2 >> 31 */
                        orr f32Mult1, f32Mult2, f32Mult1, lsl #1     /* (R1 << 1) | R2*/
                        qsub f32Accum, f32Mult1, f32Accum };         /* ( Mul1 * Mul2) - Acc */
    #else
        __asm volatile( "smull %2, %1, %2, %1 \n"                    /* R1:R2 = Mul1 * Mul2 */
                        "adds %2, %2, #0x40000000 \n"                /* Rounding */
                        "adc %1, %1, #0 \n"                          /* Adds carry*/
                        "lsr %2, %2, #31 \n"                         /* R2 >> 31 */
                        "orr %1, %2, %1, lsl #1 \n"                  /* (R1 << 1) | R2*/
                        "qsub %0, %1, %0 \n"                         /* ( Mul1 * Mul2) - Acc */
                        : "+l"(f32Accum), "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f32Mult1 * f16Mult2)
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MnacRndSat_F32lls_FAsmi(register frac32_t f32Accum, 
                                                    register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ sxth f16Mult2, f16Mult2                      /* Transforms 16-bit input f16Mult2 to 32-bit */
                        smull f32Mult1, f16Mult2, f16Mult2, f32Mult1 /* f32Mult1:f32Mult2 = f32Mult1 * f16Mult2 */
                        adds f32Mult1, f32Mult1, #0x4000             /* Rounding*/
                        adc f16Mult2, f16Mult2, #0                   /* Adds Carry Bit */
                        lsl f16Mult2, f16Mult2, #17                  /* f32Mult1 << 17 */
                        orr f16Mult2, f16Mult2, f32Mult1, lsr #15    /* Adds last 17 bits of result */    
                        qsub f32Accum, f16Mult2, f32Accum };         /* (Mul1 * Mul2) - Acc */
    #else
        __asm volatile( "sxth %1, %1 \n"                             /* Transforms 16-bit input f16Mult2 to 32-bit */
                        "smull %2, %1, %1, %2 \n"                    /* f32Mult1:f32Mult2 = f32Mult1 * f16Mult2 */
                        "adds %2, %2, #0x4000 \n"                    /* Rounding*/
                        "adc %1, %1, #0 \n"                          /* Adds Carry Bit */
                        "lsl %1, %1, #17 \n"                         /* f32Mult1 << 17 */
                        "orr %1, %1, %2, lsr #15 \n"                 /* Adds last 17 bits of result */   
                        "qsub %0, %1, %0 \n"                         /* (Mul1 * Mul2) - Acc */
                        : "+l"(f32Accum), "+l"(f16Mult2), "+l"(f32Mult1):);
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MNACRND_F32_ASM_H_ */
