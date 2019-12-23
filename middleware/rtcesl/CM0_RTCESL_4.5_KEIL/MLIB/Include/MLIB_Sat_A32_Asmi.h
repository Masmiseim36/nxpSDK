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
* @brief  Saturation
* 
*******************************************************************************/
#ifndef _MLIB_SAT_A32_ASM_H_
#define _MLIB_SAT_A32_ASM_H_

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
#define MLIB_Sat_F16a_Asmi(a32Accum) MLIB_Sat_F16a_FAsmi(a32Accum)

/***************************************************************************//*!
*
* Takes the fractional part from number in accumulator format and saturate if the original number is greater than 1. 
* 
*******************************************************************************/ 
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_Sat_F16a_FAsmi(register acc32_t a32Accum)
{
    register frac16_t f16Val=0;

    #if defined(__CC_ARM)                             /* For ARM Compiler */
        __asm volatile{ sxth f16Val, a32Accum         /* Takes fractional part */
                        cmp f16Val, a32Accum          /* Compares a32Accum and f16Val */
                        beq SatEnd                    /* If a32Accum != f16Val, then saturates output */
                        movs f16Val, #0x80            /* f16Val = 0x80 */
                        lsls f16Val, f16Val, #8       /* f16Val = 0x8000 */
                        subs f16Val, #1               /* f16Val = 0x7FFF */
                        asrs a32Accum, a32Accum, #31  /* a32Accum >> 31 */
                        subs f16Val, f16Val, a32Accum /* f16Val = 0x7FFF - a32Accum */
                    SatEnd: };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "sxth %0, %1 \n\t"              /* Takes fractional part */
                        "cmp %0, %1 \n\t"               /* Compares a32Accum and f16Val */
                        "beq MLIB_Sat_F16a_SatEnd \n\t" /* If a32Accum != f16Val, then saturates output */
                        "movs %0, #0x80 \n\t"           /* f16Val = 0x80 */
                        "lsls %0, %0, #8 \n\t"          /* f16Val = 0x8000 */
                        "subs %0, #1 \n\t"              /* f16Val = 0x7FFF */
                        "asrs %1, %1, #31 \n\t"         /* a32Accum >> 31 */
                        "subs %0, %0, %1 \n\t"          /* f16Val = 0x7FFF - a32Accum */
                    "MLIB_Sat_F16a_SatEnd:"
                        :"=&l"(f16Val), "+l"(a32Accum):);
    #endif

    return (frac16_t)f16Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SAT_A32_ASM_H_ */
