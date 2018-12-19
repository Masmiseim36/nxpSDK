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
* @param  ptr   *pf16Table
*                 - Pointer to the table values
*         in    f16X
*                 - Argument in <-1;1) in frac16_t
*               u16TableSize
*                 - Size of the look-up table in bit shifts, 3 for 8 values
*
* @return This function returns
*               - frac16_t value <-1;1)
*
* @remarks  This function calculates the number from the table using interpolation
*           of two values in the table.
*           The table size must be based on 2^x, i.e. 256 values contains
*           the size 8.
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
inline static frac16_t GFLIB_LutPer1D_F16_FAsmi(frac16_t f16X, const frac16_t *pf16Table, uint16_t u16TableSize)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;
    #if defined(__CC_ARM)                                           /* For ARM Compiler */
        __asm volatile{ sxth   f16X, f16X                           /* Transforms 16-bit input f16Val to 32-bit */
                        add    f32Val1, f16X, #32768                /* f32Val1 = 32768 + f16X */
                        /* Calculates the number of bit shifts between the 16-bit and table resolution */
                        rsb    f32Val2, u16TableSize, #16           /* f32Val2 = 16 - u16TableSize */
                        /* 16-bit argument is converted to the table table size bits */
                        lsr    f32Val1, f32Val1, f32Val2            /* f32Val1 = f32Val1 >> f32Val2 */
                        /* The exact position between two table points is calculated by keeping the LSBits of the original
                         * argument, below the table resolution. Then it is shifted to get the frac16_t resolution */
                        sub    f32Val2, u16TableSize, #1            /* f32Val2 = u16TableSize - 1 */
                        lsl    f16X, f16X, f32Val2                  /* f16X << (u16TableSize - 1) */
                        bfc    f16X, #15, #17                       /* Clears a bit-field */
                        ldrsh  f32Val2, [pf16Table, f32Val1, lsl #1]/* Loads y1 */
                        add    f32Val1, f32Val1, #1                 /* f32Val1 = f32Val1 + 1 */
                        mov    f32Val3, #1                          /* f32Val3 = 1 */
                        lsl    f32Val3, f32Val3, u16TableSize       /* f32Val3 = 1 << u16TableSize */
                        sub    f32Val3, f32Val3, #1                 /* f32Val3 = (1 << u16TableSize) - 1 */
                        and    f32Val3, f32Val3, f32Val1            /* f32Val3 = f32Val3 & f32Val1 */
                        ldrsh  f32Val3, [pf16Table, f32Val3, lsl #1]/* Loads y2 */
                        /* Subtracts two table values and interpolates, the next value to the previous is +1. */
                        sub    f32Val3, f32Val3, f32Val2            /* y2 - y1 */
                        mul    f16X, f32Val3, f16X                  /* (y2 - y1) * x */
                        add    f16X, f32Val2, f16X, asr # 15 };     /* result = y1 + (y2 - y1) * x */
    #else
        __asm volatile( "sxth   %0, %0 \n"              /* Transforms 16-bit input f16Val to 32-bit */
                        "add    %3, %0, #32768 \n"      /* f32Val1 = 32768 + f16X */
                        /* Calculates the number of bit shifts between the 16-bit and table resolution */
                        "rsb    %4, %2, #16 \n"         /* f32Val2 = 16 - u16TableSize */
                        /* 16-bit argument is converted to the table table size bits */
                        "lsr    %3, %3, %4 \n"          /* f32Val1 = f32Val1 >> f32Val2 */
                        /* The exact position between two table points is calculated by keeping the LSBits of the original
                         * argument, below the table resolution. Then it is shifted to get the frac16_t resolution */
                        "sub    %4, %2, #1 \n"          /* f32Val2 = u16TableSize - 1 */
                        "lsl    %0, %0, %4 \n"          /* f16X << (u16TableSize - 1) */
                        "bfc    %0, #15, #17 \n"        /* Clears a bit-field */
                        "ldrsh  %4, [%1, %3, lsl #1] \n"/* Loads y1 */
                        "add    %3, %3, #1 \n"          /* f32Val1 = f32Val1 + 1 */
                        "mov    %5, #1 \n"              /* f32Val3 = 1 */
                        "lsl    %5, %5, %2 \n"          /* f32Val3 = 1 << u16TableSize */
                        "sub    %5, %5, #1 \n"          /* f32Val3 = (1 << u16TableSize) - 1 */
                        "and    %5, %5, %3 \n"          /* f32Val3 = f32Val3 & f32Val1 */
                        "ldrsh  %5, [%1, %5, lsl #1] \n"/* Loads y2 */
                        /* Subtracts two table values and interpolates, the next value to the previous is +1. */
                        "sub    %5, %5, %4 \n"          /* y2 - y1 */
                        "mul    %0, %5, %0 \n"          /* (y2 - y1) * x */
                        "add    %0, %4, %0, asr # 15 \n"/* result = y1 + (y2 - y1) * x */
                        : "+l"(f16X), "+l"(pf16Table), "+l"(u16TableSize), "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3):);
    #endif

    return f16X;
}
RTCESL_INLINE_OPTIM_RESTORE 


#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LUTPER1D_ASMI_F16_H_ */
