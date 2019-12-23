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
#ifndef _MLIB_MSU4RND_F16_ASM_H_
#define _MLIB_MSU4RND_F16_ASM_H_


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
#define MLIB_Msu4RndSat_F16_Asmi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) \
        MLIB_Msu4RndSat_F16_FAsmi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)
  
/***************************************************************************//*!
*
* f16Out = (f16MinMul1 * f16MinMul2) - (f16SubMul1 * f16SubMul2)
* With saturation
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_Msu4RndSat_F16_FAsmi(register frac16_t f16MinMul1, register frac16_t f16MinMul2, 
                                                 register frac16_t f16SubMul1, register frac16_t f16SubMul2)
{
    register frac32_t f32SatVal = 0x8000;
    #if defined(__CC_ARM)                                        /* For ARM Compiler */
        __asm volatile{ sxth f16MinMul1, f16MinMul1              /* Converts 16-bit input to 32-bit */
                        sxth f16MinMul2, f16MinMul2              /* Converts 16-bit input to 32-bit */
                        sxth f16SubMul1, f16SubMul1              /* Converts 16-bit input to 32-bit */
                        sxth f16SubMul2, f16SubMul2              /* Converts 16-bit input to 32-bit */
                        muls f16SubMul1, f16SubMul1, f16SubMul2  /* f16SubMul1 * f16SubMul2 */
                        muls f16MinMul1, f16MinMul1, f16MinMul2  /* f16MinMul1 * f16MinMul2 */
                        subs f16MinMul1, f16MinMul1, f16SubMul1  /* f16MinMul1 * f16MinMul2 - f16SubMul1 * f16SubMul2 */
                        asrs f16MinMul1, f16MinMul1, #7          /* f16MinMul1 >> 7 */
                        adds f16MinMul1, f16MinMul1, #128        /* Rounding */
                        asrs f16MinMul1, f16MinMul1, #8          /* f16MinMul1 >> 8 */
                        movs f16SubMul2, f16MinMul1              /* Copies result to f16SubMul2 */

                        cmp f32SatVal, f16MinMul1                /* Compares result with 0x8000 */
                        bgt NegTest                              /* If result < 0x8000, then go to NegTest */
                        subs f16MinMul1, f32SatVal, #1           /* If result >= 0x8000, then result = 0x7FFF */
                        b SatEnd                                 /* Goes to SatEnd */
                    NegTest:
                        sxth f16SubMul1, f32SatVal               /* f16SubMul1 = 0xFFFF8000 */
                        cmp f16SubMul1, f16MinMul1               /* Compares result with 0xFFFF8000 */
                        ble SatEnd                               /* If result >= 0xFFFF8000, then goes to SatEnd */
                        mov f16MinMul1, f16SubMul1               /* If result < 0xFFFF8000, then result = 0xFFFF8000 */
                        lsls f16SubMul1, f16SubMul1, #1          /* f16SubMul1 = 0xFFFF0000 */
                        cmp f16SubMul1, f16SubMul2               /* Compares result with 0xFFFF0000 */
                        bne SatEnd                               /* If result <> 0xFFFF0000, then goes to SatEnd */
                        subs f16MinMul1, f32SatVal, #1           /* If result = 0xFFFF0000, then result = 0x7FFF */
                    SatEnd: };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "sxth %0, %0 \n\t"             /* Converts 16-bit input to 32-bit */
                        "sxth %1, %1 \n\t"             /* Converts 16-bit input to 32-bit */
                        "sxth %2, %2 \n\t"             /* Converts 16-bit input to 32-bit */
                        "sxth %3, %3 \n\t"            /* Converts 16-bit input to 32-bit */
                        "muls %2, %2, %3 \n\t"         /* f16SubMul1 * f16SubMul2 */
                        "muls %0, %0, %1 \n\t"         /* f16MinMul1 * f16MinMul2 */
                        "subs %0, %0, %2 \n\t"         /* f16MinMul1 * f16MinMul2 - f16SubMul1 * f16SubMul2 */
                        "asrs %0, %0, #7 \n\t"         /* f16MinMul1 >> 7 */
                        "adds %0, %0, #128 \n\t"       /* Rounding */
                        "asrs %0, %0, #8 \n\t"         /* f16MinMul1 >> 8 */
                        "movs %1, %0 \n\t"             /* Copies result to f16SubMul2 */

                        "cmp %4, %0 \n\t"              /* Compares result with 0x7FFF */
                        "bgt MLIB_Msu4RndSat_F16_NegTest \n\t"/* If result < 0x8000, then jumps through two commands */
                        "subs %0, %4, #1 \n\t"         /* If result >= 0x8000, then result = 0x7FFF */
                        "b MLIB_Msu4RndSat_F16_NegTest \n\t"/* Goes to the end of function */
                    "MLIB_Msu4RndSat_F16_NegTest: \n\t"
                        "sxth %2, %4 \n\t"             /* f16SubMul1 = 0xFFFF8000 */
                        "cmp %2, %0 \n\t"              /* Compares result with 0xFFFF8000 */
                        "ble MLIB_Msu4RndSat_F16_SatEnd \n\t"/* If result >= 0xFFFF8000, then jumps through four commands */
                        "mov %0, %2 \n\t"              /* If result < 0xFFFF8000, then result = 0xFFFF8000 */
                        "lsls %2, %2, #1 \n\t"         /* f16SubMul1 = 0xFFFF0000 */
                        "cmp %2, %1 \n\t"              /* Compares result with 0xFFFF0000 */
                        "bne MLIB_Msu4RndSat_F16_SatEnd \n\t" /* If result <> 0xFFFF0000, then jumps through next command */
                        "subs %0, %4, #1 \n\t"         /* If result = 0xFFFF0000, then result = 0x7FFF */
                    "MLIB_Msu4RndSat_F16_SatEnd: \n\t"
                        : "+l"(f16MinMul1), "+l"(f16MinMul2), "+l"(f16SubMul1), "+l"(f16SubMul2): "l"(f32SatVal));
    #endif

    return f16MinMul1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSU4RND_F16_ASM_H_ */
