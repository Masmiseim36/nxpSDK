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
* @brief  Absolute Value
* 
*******************************************************************************/
#ifndef _MLIB_ABS_F32_ASM_H_
#define _MLIB_ABS_F32_ASM_H_

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
#define MLIB_AbsSat_F32_Asmi(f32Val) MLIB_AbsSat_F32_FAsmi(f32Val)

/***************************************************************************//*!
*  Absolute value saturated
*  Output = |f32In|
*******************************************************************************/  
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_AbsSat_F32_FAsmi(register frac32_t f32Val)
{
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ cmp f32Val, #0                  /* Compares f32Val with 0 */
                        bge AbsEnd                      /* If f32Val >= 0, then goes to AbsEnd */
                        rsbs f32Val, f32Val, #0         /* If f32Val < 0, then f32Val = 0 - f32Val */
                        bpl AbsEnd                      /* If f32Val >= 0, then goes to AbsEnd */
                        subs f32Val, f32Val, #1         /* If f32Val = 0x80000000, f32Val = 0x7FFFFFFF */
                    AbsEnd: };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "cmp %0, #0 \n\t"                 /* Compares f32Val with 0 */
                        "bge MLIB_AbsSat_F32_AbsEnd \n\t"                    /* If f32Val >= 0, then jumps through three commands */
                        "rsbs %0, %0, #0 \n\t"            /* If f32Val < 0, then f32Val = 0 - f32Val */
                        "bpl MLIB_AbsSat_F32_AbsEnd \n\t"                    /* If f32Val >= 0, then jumps through next command */
                        "subs %0, %0, #1 \n\t"            /* If f32Val = 0x80000000, f32Val = 0x7FFFFFFF */
                     "MLIB_AbsSat_F32_AbsEnd: \n\t"
                        : "+l"(f32Val):);
    #endif

    return f32Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ABS_F32_ASM_H_*/
