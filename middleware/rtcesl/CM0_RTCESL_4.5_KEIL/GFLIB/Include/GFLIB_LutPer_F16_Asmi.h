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
* @brief  Periodical look-up table algorithm using linear interpolation
*
*******************************************************************************/
#ifndef _GFLIB_LUTPER1D_ASMI_F16_H_
#define _GFLIB_LUTPER1D_ASMI_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define GFLIB_LutPer1D_F16_Asmi(f16X, pf16Table, u16TableSize) GFLIB_LutPer1D_F16_FAsmi(f16X, pf16Table, u16TableSize)

/***************************************************************************//*!
*
* @brief  Periodical look-up table algorithm using linear interpolation
*
* @param  ptr   r1 - Pointer to the table values
* @param  in    r0 - Argument in <-1;1) in frac16_t
*               r2 - Size of the look-up table in bit shifts, 3 for 8 values
*
* @return This function returns
*               r0 - frac16_t value <-1;1)
*
* @remarks  This function calculates the number from the table using interpolation
*           of two values in the table.
*           The table size must be based on 2^x, i.e. 256 values contains
*           the size 8.
*
*******************************************************************************/
inline static frac16_t GFLIB_LutPer1D_F16_FAsmi(frac16_t f16X, const frac16_t *pf16Table, uint16_t u16TableSize)
{
    register frac32_t f32Val1 = 32768, f32Val2 = 16, f32Val3 = 1;
    #if defined(__CC_ARM)                                       /* For ARM Compiler */
        __asm volatile{ sxth   f16X, f16X                       /* Transforms 16-bit input f16Val to 32-bit */
                        adds   f32Val1, f32Val1, f16X           /* f32Val1 = 32768 + f16X */
                        /* Calculates the number of bit shifts between the 16-bit and table resolution */
                        subs   f32Val2, f32Val2, u16TableSize   /* f32Val2 = 16 - u16TableSize */
                        /* 16-bit argument is converted to the table table size bits */
                        lsrs   f32Val1, f32Val1, f32Val2        /* f32Val1 = f32Val1 >> f32Val2 */
                        /* The exact position between two table points is calculated by keeping the LSBits of the original
                         * argument, below the table resolution. Then it is shifted to get the frac16_t resolution */
                        subs   f32Val2, u16TableSize, #1        /* f32Val2 = u16TableSize - 1 */
                        lsls   f16X, f16X, f32Val2              /* f16X << (u16TableSize - 1) */
                        lsls   f16X, f16X, #17                  /* f16X << 17 */
                        lsrs   f16X, f16X, #17                  /* f16X >> 17: clears higher 17 bits */
                        lsls   f32Val2, f32Val1, #1             /* f32Val2 = f32Val1 << 1 */
                        ldrsh  f32Val2, [pf16Table, f32Val2]    /* Loads y1 */
                        adds   f32Val1, f32Val1, #1             /* f32Val1 = f32Val1 + 1 */
                        lsls   f32Val3, f32Val3, u16TableSize   /* f32Val3 = 1 << u16TableSize */
                        subs   f32Val3, f32Val3, #1             /* f32Val3 = (1 << u16TableSize) - 1 */
                        ands   f32Val3, f32Val3, f32Val1        /* f32Val3 = f32Val3 & f32Val1 */
                        lsls   f32Val3, f32Val3, #1             /* f32Val3 << 1 */
                        ldrsh  f32Val3, [pf16Table, f32Val3]    /* Loads y2 */
                        /* Subtracts two table values and interpolates, the next value to the previous is +1. */
                        subs   f32Val3, f32Val3, f32Val2        /* y2 - y1 */
                        muls   f16X, f32Val3, f16X              /* (y2 - y1) * x */
                        asrs   f16X, f16X, #15                  /* f16X >> 15 */
                        adds   f16X, f32Val2, f16X };           /* result = y1 + (y2 - y1) * x */
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "sxth   %0, %0 \n\t"              /* Transforms 16-bit input f16Val to 32-bit */
                        "adds   %3, %3, %0 \n\t"          /* f32Val1 = 32768 + f16X */
                        /* Calculates the number of bit shifts between the 16-bit and table resolution */
                        "subs   %4, %4, %2 \n\t"          /* f32Val2 = 16 - u16TableSize */
                        /* 16-bit argument is converted to the table table size bits */
                        "lsrs   %3, %3, %4 \n\t"          /* f32Val1 = f32Val1 >> f32Val2 */
                        /* The exact position between two table points is calculated by keeping the LSBits of the original
                         * argument, below the table resolution. Then it is shifted to get the frac16_t resolution */
                        "subs   %4, %2, #1 \n\t"          /* f32Val2 = u16TableSize - 1 */
                        "lsls   %0, %0, %4 \n\t"          /* f16X << (u16TableSize - 1) */
                        "lsls   %0, %0, #17 \n\t"         /* f16X << 17 */
                        "lsrs   %0, %0, #17 \n\t"         /* f16X >> 17: clears higher 17 bits */
                        "lsls   %4, %3, #1 \n\t"          /* f32Val2 = f32Val1 << 1 */
                        "ldrsh  %4, [%1, %4] \n\t"        /* Loads y1 */
                        "adds   %3, %3, #1 \n\t"          /* f32Val1 = f32Val1 + 1 */
                        "lsls   %5, %5, %2 \n\t"          /* f32Val3 = 1 << u16TableSize */
                        "subs   %5, %5, #1 \n\t"          /* f32Val3 = (1 << u16TableSize) - 1 */
                        "ands   %5, %5, %3 \n\t"          /* f32Val3 = f32Val3 & f32Val1 */
                        "lsls   %5, %5, #1 \n\t"          /* f32Val3 << 1 */
                        "ldrsh  %5, [%1, %5] \n\t"        /* Loads y2 */
                        /* Subtracts two table values and interpolates, the next value to the previous is +1. */
                        "subs   %5, %5, %4 \n\t"          /* y2 - y1 */
                        "muls   %0, %5, %0 \n\t"          /* (y2 - y1) * x */
                        "asrs   %0, %0, #15 \n\t"         /* f16X >> 15 */
                        "adds   %0, %4, %0 \n\t"          /* result = y1 + (y2 - y1) * x */
                        : "+l"(f16X), "+l"(pf16Table), "+l"(u16TableSize), "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3):);
    #endif

    return f16X;
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LUTPER1D_ASMI_F16_H_ */
