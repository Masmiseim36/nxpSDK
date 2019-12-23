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
* @brief  Negation
* 
*******************************************************************************/
#ifndef _MLIB_NEG_F16_ASM_H_
#define _MLIB_NEG_F16_ASM_H_

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
#define MLIB_NegSat_F16_Asmi(f16Val) MLIB_NegSat_F16_FAsmi(f16Val)

/***************************************************************************//*!
*
* Output = -f16Val
* With saturation
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_NegSat_F16_FAsmi(register frac16_t f16Val)
{
    register frac32_t f32SatVal = 0x8000;
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ sxth f16Val, f16Val             /* Transforms 16-bit input f16Val to 32-bit */
                        rsbs f16Val, f16Val, #0         /* f16Val = - f16Val */
                        cmp f16Val, f32SatVal           /* Compares f16Val with 0x00008000 */
                        bne SatEnd                      /* If f32Val != 0x8000, then goes to SatEnd */
                        subs f16Val, f16Val, #1         /* Compares f16Val with 0x00008000 */
        SatEnd: };

    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "sxth %0, %0 \n\t"                /* Transforms 16-bit input f16Val to 32-bit */
                        "rsbs %0, %0, #0 \n\t"            /* f16Val = - f16Val */
                        "cmp %0, %1 \n\t"                 /* Compares f16Val with 0x00008000 */
                        "bne MLIB_NegSat_F16_SatEnd \n\t" /* If f32Val != 0x8000, then jumps through next command */
                        "subs %0, %0, #1 \n\t"            /* Compares f16Val with 0x00008000 */
                    "MLIB_NegSat_F16_SatEnd: \n\t"    
                        : "+l"(f16Val): "l"(f32SatVal));
    #endif

    return f16Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_NEG_F16_ASM_H_ */
