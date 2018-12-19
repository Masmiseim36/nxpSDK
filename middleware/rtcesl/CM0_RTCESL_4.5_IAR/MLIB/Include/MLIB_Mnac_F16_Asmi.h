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
#ifndef _MLIB_MNAC_F16_ASM_H_
#define _MLIB_MNAC_F16_ASM_H_

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
#define MLIB_MnacSat_F16_Asmi( f16Accum, f16Mult1, f16Mult2)                   \
        MLIB_MnacSat_F16_FAsmi(f16Accum, f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f16Out = - f16Accum + ( f16Mult1 * f16Mult2)
* With saturation
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_MnacSat_F16_FAsmi(register frac16_t f16Accum, 
                                              register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ sxth f16Mult1, f16Mult1             /* Converts 16-bit input to 32-bit */
                        sxth f16Mult2, f16Mult2             /* Converts 16-bit input to 32-bit */
                        sxth f16Accum, f16Accum             /* Converts 16-bit input to 32-bit */
                        muls f16Mult1, f16Mult1, f16Mult2   /* f16Mult1 * f16Mult2 */
                        asrs f16Mult1, f16Mult1, #15        /* f16Mult1 >> 15 */
                        subs f16Accum, f16Mult1, f16Accum   /* f16Mult1 * f16Mult2 - f16Accum */

                        movs f16Mult1, #128                 /* f16Mult1 = 0x80 */
                        lsls f16Mult1, f16Mult1, #8         /* f16Mult1 = 0x8000 */

                        cmp f16Mult1, f16Accum              /* Compares f16Accum with 0x8000 */
                        bgt NegTest                         /* If f16Accum < 0x8000, then goes to NegTest */
                        subs f16Accum, f16Mult1, #1         /* If f16Accum >= 0x8000, then f16Accum = 0x7FFF */
                        b SatEnd                            /* Goes to SatEnd */
                     NegTest:
                        sxth f16Mult1, f16Mult1             /* f16Mult1 = 0xFFFF8000 */
                        cmp f16Mult1, f16Accum              /* Compares f16Accum with 0xFFFF8000 */
                        ble SatEnd                          /* If f16Accum >= 0xFFFF8000, then goes to SatEnd */
                        mov f16Accum, f16Mult1              /* If f16Accum < 0xFFFF8000, then f16Mult1 = 0xFFFF8000 */
                     SatEnd: };
    #else
        __asm volatile(
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "sxth %0, %0 \n"                    /* Converts 16-bit input to 32-bit */
                        "sxth %1, %1 \n"                    /* Converts 16-bit input to 32-bit */
                        "sxth %2, %2 \n"                    /* Converts 16-bit input to 32-bit */
                        "muls %1, %1, %2 \n"                /* f16Mult1 * f16Mult2 */
                        "asrs %1, %1, #15 \n"               /* f16Mult1 >> 15 */
                        "subs %0, %1, %0 \n"                /* f16Mult1 * f16Mult2 - f16Accum */
                        
                        "movs %1, #128 \n"                  /* f16Mult1 = 0x80 */
                        "lsls %1, %1, #8 \n"                /* f16Mult1 = 0x8000 */
                            
                        "cmp %1, %0 \n"                     /* Compares f16Accum with 0x8000 */
                        "bgt .+6 \n"                        /* If f16Accum < 0x8000, then jumps through two commands */
                        "subs %0, %1, #1 \n"                /* If f16Accum >= 0x8000, then f16Accum = 0x7FFF */
                        "b .+10 \n"                         /* Jumps through four commands */
                        "sxth %1, %1 \n"                    /* f16Mult1 = 0xFFFF8000 */
                        "cmp %1, %0 \n"                     /* Compares f16Accum with 0xFFFF8000 */
                        "ble .+4 \n"                        /* If f16Accum >= 0xFFFF8000, then jumps through next commands */
                        "mov %0, %1 \n"                     /* If f16Accum < 0xFFFF8000, then f16Accum = 0xFFFF8000 */
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Accum), "+l"(f16Mult1), "+l"(f16Mult2):);
    #endif

    return f16Accum;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MNAC_F16_ASM_H_ */
