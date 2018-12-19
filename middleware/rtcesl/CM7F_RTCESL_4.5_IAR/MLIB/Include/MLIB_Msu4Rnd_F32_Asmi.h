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
* @brief  Multiply subtract of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MSU4RND_F32_ASM_H_
#define _MLIB_MSU4RND_F32_ASM_H_

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
#define MLIB_Msu4RndSat_F32_Asmi( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2) \
        MLIB_Msu4RndSat_F32_FAsmi( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)

/***************************************************************************//*!
*
* f32Out = (f32MinMul1 * f32MinMul2) - (f32SubMul1 * f32SubMul2)
* With saturation
*******************************************************************************/  
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Msu4RndSat_F32_FAsmi(register frac32_t f32MinMul1,register frac32_t f32MinMul2,
                                                 register frac32_t f32SubMul1,register frac32_t f32SubMul2)
{         
    #if defined(__CC_ARM)                                                        /* For ARM Compiler */
        __asm volatile{ smull f32MinMul2, f32MinMul1, f32MinMul2, f32MinMul1     /* f16Add1Mul1 * f16Add1Mul2 */
                        smull f32SubMul2, f32SubMul1, f32SubMul2, f32SubMul1     /* f16Add2Mul1 * f16Add2Mul2 */
                        subs f32MinMul2, f32MinMul2, f32SubMul2                  /* Subtraction of lower parts */
                        sbc f32MinMul1, f32MinMul1, f32SubMul1                   /* Subtraction of higher parts */
                        adds f32MinMul2, f32MinMul2, #0x40000000                 /* Rounding */
                        adc f32MinMul1, f32MinMul1, #0                           /* Adds carry bit */
                        add f32SubMul1, f32MinMul1, f32MinMul1                   /* Converts high part of result to fractional value */
                        qadd f32MinMul1, f32MinMul1, f32MinMul1                  /* Converts high part of result to fractional value with saturation */
                        cmp f32MinMul1, f32SubMul1                               /* Compares result with saturation and result without saturation */
                        it eq                                                    /* If values are equal, then adds the last bit */
                        orreq f32MinMul1, f32MinMul1, f32MinMul2, lsr #31 };     /* Result = Result + Last bit */
    #else
        __asm volatile( "smull %1, %0, %1, %0 \n"                                /* f16Add1Mul1 * f16Add1Mul2 */
                        "smull %3, %2, %3, %2 \n"                                /* f16Add2Mul1 * f16Add2Mul2 */
                        "subs %1, %1, %3 \n"                                     /* Subtraction of lower parts */
                        "sbc %0, %0, %2 \n"                                      /* Subtraction of higher parts */
                        "adds %1, %1, #0x40000000 \n"                            /* Rounding */
                        "adc %0, %0, #0 \n"                                      /* Adds carry bit */
                        "add %2, %0, %0 \n"                                      /* Converts high part of result to fractional value */
                        "qadd %0, %0, %0 \n"                                     /* Converts high part of result to fractional value with saturation */
                        "cmp %0, %2 \n"                                          /* Compares result with saturation and result without saturation */
                        "it eq \n"                                               /* If values are equal, then adds the last bit */
                        "orreq %0, %0, %1, lsr #31 \n"                           /* Result = Result + Last bit */
                        : "+l"(f32MinMul1), "+l"(f32MinMul2), "+l"(f32SubMul1), "+l"(f32SubMul2):);
    #endif

    return f32MinMul1;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSU4RND_F32_ASM_H_ */
