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
* @brief  Multiply 
* 
*******************************************************************************/
#ifndef _MLIB_MUL_F32_ASM_H_
#define _MLIB_MUL_F32_ASM_H_

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
#define MLIB_MulSat_F32_Asmi(f32Mult1, f32Mult2)   MLIB_MulSat_F32_FAsmi(f32Mult1, f32Mult2)
#define MLIB_MulSat_F32ss_Asmi(f16Mult1, f16Mult2) MLIB_MulSat_F32ss_FAsmi(f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* With saturation
*******************************************************************************/  
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MulSat_F32_FAsmi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult1, f32Mult2 /* f32Mult1 * f32Mult2 */
                        lsr f32Mult2, f32Mult2, #31                  /* f32Mult2 >> 31 */
                        qdadd f32Mult1, f32Mult2, f32Mult1 };        /* Converts result to frac32 and saturates */
    #else
        __asm volatile( "smull %1, %0, %0, %1 \n"                    /* f32Mult1 * f32Mult2 */
                        "lsr %1, %1, #31 \n"                         /* f32Mult2 >> 31 */
                        "qdadd %0, %1, %0 \n"                        /* Converts result to frac32 and saturates */
                        : "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Mult1;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = f16Mult1 * f16Mult2
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_MulSat_F32ss_FAsmi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register frac32_t f32Out=0;
    #if defined(__CC_ARM)                                        /* For ARM Compiler */
        __asm volatile{ smulbb f16Mult1, f16Mult1, f16Mult2      /* f16Mult1 * f16Mult2 */
                        qadd f32Out, f16Mult1, f16Mult1 };       /* Converts result to fractional and saturates */
    #else
        __asm volatile( "smulbb %1, %1, %2 \n"                   /* f16Mult1 * f16Mult2 */
                        "qadd %0, %1, %1 \n"                     /* Converts result to fractional and saturates */
                        : "=l"(f32Out), "+l"(f16Mult1): "l"(f16Mult2));
    #endif

    return f32Out;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_F32_ASM_H_ */
