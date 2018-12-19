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
#ifndef _MLIB_MULRND_F32_ASM_H_
#define _MLIB_MULRND_F32_ASM_H_

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
#define MLIB_MulRndSat_F32_Asmi(f32Mult1, f32Mult2)   MLIB_MulRndSat_F32_FAsmi(f32Mult1, f32Mult2)
#define MLIB_MulRndSat_F32ls_Asmi(f32Mult1, f16Mult2) MLIB_MulRndSat_F32ls_FAsmi(f32Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* With saturation
*******************************************************************************/  
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MulRndSat_F32_FAsmi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult1, f32Mult2 /* f32Mult1 * f32Mult2 */
                        lsrs f32Mult2, f32Mult2, #31                 /* f32Mult2 >> 31 */
                        adc f32Mult2, f32Mult2, #0                   /* Rounding */
                        qdadd f32Mult1, f32Mult2, f32Mult1 };        /* 2 * f32Mult1 + f32Mult2 with saturation */
    #else
        __asm volatile( "smull %1, %0, %0, %1 \n"                    /* f32Mult1:f32Mult2 = f32Mult1 * f32Mult2 */
                        "lsrs %1, %1, #31 \n"                        /* f32Mult2 >> 31 */
                        "adc %1, %1, #0 \n"                          /* Rounding */
                        "qdadd %0, %1, %0 \n"                        /* 2 * f32Mult1 + f32Mult2 with saturation */
                        : "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Mult1;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f16Mult2
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MulRndSat_F32ls_FAsmi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ lsl f16Mult2, f16Mult2, #16                  /* f16Mult2 << 16 */
                        smull f16Mult2, f32Mult1, f32Mult1, f16Mult2 /* f32Mult1 * f16Mult2 */
                        lsrs f16Mult2, f16Mult2, #31                 /* f16Mult2 >> 31 */
                        adc f16Mult2, f16Mult2, #0                   /* Rounding */
                        qdadd f32Mult1, f16Mult2, f32Mult1 };        /* 2 * f32Mult1 + f16Mult2 with saturation */
    #else
        __asm volatile( "lsl %1, %1, #16 \n"                         /* f16Mult2 << 16 */
                        "smull %1, %0, %0, %1 \n"                    /* f32Mult1:f16Mult2 = f32Mult1 * f16Mult2 */
                        "lsrs %1, %1, #31 \n"                        /* f16Mult2 >> 31 */
                        "adc %1, %1, #0 \n"                          /* Rounding */
                        "qdadd %0, %1, %0 \n"                        /* 2 * f32Mult1 + f16Mult2 with saturation */
                        : "+l"(f32Mult1), "+l"(f16Mult2):);
    #endif

    return f32Mult1;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_F32_ASM_H_ */
