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
#ifndef _MLIB_ABS_F16_ASM_H_
#define _MLIB_ABS_F16_ASM_H_

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
#define MLIB_AbsSat_F16_Asmi(f16Val) MLIB_AbsSat_F16_FAsmi(f16Val)


/***************************************************************************//*!
*  Absolute value saturated
*  Output = |f16In|
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_AbsSat_F16_FAsmi(register frac16_t f16Val)
{
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ lsls f16Val, f16Val, #16        /* f16Val << 16 */
                        bpl AbsEnd                      /* If f16Val >= 0, then goes to AbsEnd */
                        rsbs f16Val, f16Val, #0         /* If f16Val < 0, then f16Val = 0 - f16Val */
                        bpl AbsEnd                      /* If f16Val >= 0, then goes to AbsEnd */
                        subs f16Val, f16Val, #1         /* If f16Val == 0xFFFF8000, f16Val = f16Val - 1 */
                    AbsEnd:
                        lsrs f16Val, f16Val, #16};      /* f16Val >> 16 */
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "lsls %0, %0, #16 \n\t"           /* f16Val << 16 */
                        "bpl MLIB_AbsSat_F16_AbsEnd \n\t"                    /* If f16Val >= 0, then jumps through three commands */
                        "rsbs %0, %0, #0 \n\t"            /* If f16Val < 0, then f16Val = 0 - f16Val */
                        "bpl MLIB_AbsSat_F16_AbsEnd \n\t"                    /* If f16Val >= 0, then jumps through next command */
                        "subs %0, %0, #1 \n\t"            /* If f16Val = 0x80000000, f16Val = 0x7FFFFFFF */
                    "MLIB_AbsSat_F16_AbsEnd: \n\t"
				         "lsrs %0, %0, #16 \n\t"           /* f16Val >> 16 */
                        : "+l"(f16Val):);
    #endif

    return f16Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /*_MLIB_ABS_F16_ASM_H_*/
