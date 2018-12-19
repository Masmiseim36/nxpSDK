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
* @brief  Look-up table algorithm using linear interpolation
*
*******************************************************************************/
#ifndef _GFLIB_LUT1D_ASMI_F16_H_
#define _GFLIB_LUT1D_ASMI_F16_H_

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
#define GFLIB_Lut1D_F16_Asmi(f16X, pf16Table, u16TableSize)                    \
        GFLIB_Lut1D_F16_FAsmi(f16X, pf16Table, u16TableSize)

/***************************************************************************//*!
*
* @brief    Look-up table algorithm using linear interpolation
*
* @param    ptr   *pf16Table
*                   - Pointer to the table values
*           in    f16X
*                   - Argument in <-1;1) in frac16_t
*                 u16TableSize
*                   - Size of the look-up table in bit shifts, 3 for 8 values
*
* @return   This function returns
*                 - frac16_t value <-1;1)
*
* @remarks  This function calculates the number from the table using interpolation
*           of two values in the table.
*           The table size must be based on 2^x+1, i.e. 257 values contains
*           the size 8.
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET     
inline static frac16_t GFLIB_Lut1D_F16_FAsmi(frac16_t f16X, const frac16_t *pf16Table, uint16_t u16TableSize)
{
    register frac32_t f32Val1=0, f32Val2=0;
    #if defined(__CC_ARM)                                           /* For ARM Compiler */
        __asm volatile{ sxth   f16X, f16X                           /* Transforms 16-bit input f16Val to 32-bit */
                        sub    u16TableSize, u16TableSize, #1       /* u16TableSize = u16TableSize - 1 */
                        movs   f32Val1, #2                          /* f32Val1 = 2 */
                        lsl    f32Val1, f32Val1, u16TableSize       /* f32Val1 << u16TableSize */
                        /* Extract the fractional part of the input abscissa */
                        lsl    f32Val2, f16X, u16TableSize          /* f32Val2 = f16X << u16TableSize */
                        bfc    f32Val2, #15, #17                    /* Clears a bit-field */
                        add    pf16Table, pf16Table, f32Val1        /* pf16Table = pf16Table + f32Val1 */
                        rsb    f32Val1, u16TableSize, #15           /* f32Val1 = 15 - u16TableSize */
                        asr    f16X, f16X, f32Val1                  /* f16X >> f32Val1 */
                        add    pf16Table, pf16Table, f16X, lsl #1   /* pf16Table = pf16Table + f16X *2 */
                        /* Read interpolated data */
                        ldrsh  u16TableSize, [pf16Table], #2        /* Loads y1 */
                        ldrsh  f32Val1, [pf16Table]                 /* Loads y2 */
                        /* Perform the actual interpolation with 16-bit accuracy */
                        sub    f16X, f32Val1, u16TableSize          /* y2 - y1 */
                        mul    f16X, f16X, f32Val2                  /* (y2 - y1) * x */
                        add    f16X, u16TableSize, f16X, asr # 15 };/* result = y1 + (y2 - y1) * x */
    #else
        __asm volatile( "sxth   %0, %0 \n"              /* Transforms 16-bit input f16Val to 32-bit */
                        "sub    %2, %2, #1 \n"          /* u16TableSize = u16TableSize - 1 */
                        "movs   %3, #2 \n"              /* f32Val1 = 2 */
                        "lsl    %3, %3, %2 \n"          /* f32Val1 << u16TableSize */
                        /* Extract the fractional part of the input abscissa */
                        "lsl    %4, %0, %2 \n"          /* f32Val2 = f16X << u16TableSize */
                        "bfc    %4, #15, #17 \n"        /* Clears a bit-field */
                        "add    %1, %1, %3 \n"          /* pf16Table = pf16Table + f32Val1 */
                        "rsb    %3, %2, #15 \n"         /* f32Val1 = 15 - u16TableSize */
                        "asr    %0, %0, %3 \n"          /* f16X >> f32Val1 */
                        "add    %1, %1, %0, lsl #1 \n"  /* pf16Table = pf16Table + f16X *2 */
                        /* Read interpolated data */
                        "ldrsh  %2, [%1], #2 \n"        /* Loads y1 */
                        "ldrsh  %3, [%1] \n"            /* Loads y2 */
                        /* Perform the actual interpolation with 16-bit accuracy */
                        "sub    %0, %3, %2 \n"          /* y2 - y1 */
                        "mul    %0, %0, %4 \n"          /* (y2 - y1) * x */
                        "add    %0, %2, %0, asr # 15 \n"/* result = y1 + (y2 - y1) * x */
                        : "+l"(f16X), "+l"(pf16Table), "+l"(u16TableSize), "+l"(f32Val1), "+l"(f32Val2):);
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

#endif /* _GFLIB_LUT1D_ASMI_F16_H_ */
