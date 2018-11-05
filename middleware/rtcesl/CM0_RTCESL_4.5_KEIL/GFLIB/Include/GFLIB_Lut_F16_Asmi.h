/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#define GFLIB_Lut1D_F16_Asmi(f16X, pf16Table, u16TableSize) GFLIB_Lut1D_F16_FAsmi(f16X, pf16Table, u16TableSize)

/***************************************************************************//*!
*
* @brief  Look-up table algorithm using linear interpolation
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
*           The table size must be based on 2^x+1, i.e. 257 values contains
*           the size 8.
*
*******************************************************************************/
inline static frac16_t GFLIB_Lut1D_F16_FAsmi(frac16_t f16X, const frac16_t *pf16Table, uint16_t u16TableSize)
{
    register frac32_t f32Val1 = 2;
    register frac32_t f32Val2 = f16X;
    #if defined(__CC_ARM)                                       /* For ARM Compiler */
        __asm volatile{ sxth    f16X, f16X                      /* Transforms 16-bit input f16Val to 32-bit */
                        subs    u16TableSize, u16TableSize, #1  /* u16TableSize = u16TableSize - 1 */
                        lsls    f32Val1, f32Val1, u16TableSize  /* f32Val1 << u16TableSize */
                        /* Extract the fractional part of the input abscissa */
                        lsls    f32Val2, f32Val2, u16TableSize  /* f32Val2 << u16TableSize */
                        lsls    f32Val2, f32Val2, #17           /* f32Val2 << 17 */
                        lsrs    f32Val2, f32Val2, #17           /* f32Val2 >> 17: clears higher 17 bits */
                        adds    pf16Table, pf16Table, f32Val1   /* pf16Table = pf16Table + f32Val1 */
                        movs    f32Val1, #15                    /* f32Val1 = 15 */
                        subs    f32Val1, f32Val1, u16TableSize  /* f32Val1 = f32Val1 - u16TableSize */
                        asrs    f16X, f16X, f32Val1             /* f16X >> f32Val1 */
                        lsls    f16X, f16X, #1                  /* f16X = f16X *2 */
                        /* Read interpolated data */
                        ldrsh  u16TableSize, [pf16Table, f16X]  /* Loads y1 */
                        adds   f16X, f16X, #2                   /* f16X = f16X + 2 */
                        ldrsh  f32Val1, [pf16Table, f16X]       /* Loads y2 */
                        /* Perform the actual interpolation with 16-bit accuracy */
                        subs    f16X, f32Val1, u16TableSize     /* y2 - y1 */
                        muls    f16X, f16X, f32Val2             /* (y2 - y1) * x */
                        asrs    f16X, f16X, #15                 /* f16X >> 15 */
                        adds    f16X, f16X, u16TableSize };     /* result = y1 + (y2 - y1) * x */
    #else
        __asm volatile(
                        #if defined(__GNUC__)        /* For GCC compiler */
                            ".syntax unified \n"     /* Using unified asm syntax */
                        #endif
                        "sxth    %0, %0 \n"          /* Transforms 16-bit input f16Val to 32-bit */
                        "subs    %2, %2, #1 \n"      /* u16TableSize = u16TableSize - 1 */
                        "lsls    %3, %3, %2 \n"      /* f32Val1 << u16TableSize */
                        /* Extract the fractional part of the input abscissa */
                        "lsls    %4, %4, %2 \n"      /* f32Val2 << u16TableSize */
                        "lsls    %4, %4, #17 \n"     /* f32Val2 << 17 */
                        "lsrs    %4, %4, #17 \n"     /* f32Val2 >> 17: clears higher 17 bits */
                        "adds    %1, %1, %3 \n"      /* pf16Table = pf16Table + f32Val1 */
                        "movs    %3, #15 \n"         /* f32Val1 = 15 */
                        "subs    %3, %3, %2 \n"      /* f32Val1 = f32Val1 - u16TableSize */
                        "asrs    %0, %0, %3 \n"      /* f16X >> f32Val1 */
                        "lsls    %0, %0, #1 \n"      /* f16X = f16X *2 */
                        /* Read interpolated data */
                        "ldrsh  %2, [%1, %0] \n"     /* Loads y1 */
                        "adds   %0, %0, #2 \n"       /* f16X = f16X + 2 */
                        "ldrsh  %3, [%1, %0] \n"     /* Loads y2 */
                        /* Perform the actual interpolation with 16-bit accuracy */
                        "subs    %0, %3, %2 \n"      /* y2 - y1 */
                        "muls    %0, %0, %4 \n"      /* (y2 - y1) * x */
                        "asrs    %0, %0, #15 \n"     /* f16X >> 15 */
                        "adds    %0, %0, %2 \n"      /* result = y1 + (y2 - y1) * x */
                        #if defined(__GNUC__)        /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16X), "+l"(pf16Table), "+l"(u16TableSize), "+l"(f32Val1), "+l"(f32Val2):);
    #endif

    return f16X;
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LUT1D_ASMI_F16_H_ */
