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
* @brief  Digital IIR Filter, 1st order 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR1_F32_ASMI_H_
#define _GDFLIB_FILTERIIR1_F32_ASMI_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gdflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterIIR1_F16_Asmi(f16InX, psParam) \
        GDFLIB_FilterIIR1_F16_FAsmi(f16InX, psParam)

/***************************************************************************//*!
*
* @brief  16-bit input and output 1st order IIR filter
*
* @param  ptr   GDFLIB_FILTER_IIR1_T_F32 *psParam
*               - pointer to the structure of the filter
*
*               GDFLIB_FILTER_IIR1_T_F32 data structure:
*               GDFLIB_FILTER_IIR1_COEFF_T_F32    sFltCoeff
*               - filter coefficients in frac32_t divided by 2
*                 in the following range
*                 -1.0 <= coef < 1.0
*               - B coefficients:
*                   frac32_t  f32B0
*                   frac32_t  f32B1
*               -A coefficient, the sign is negated
*                   frac32_t  f32A1;
*               frac32_t f32FltBfrY[1];
*               - y history buffer in frac32_t
*               frac16_t f16FltBfrX[1];
*               - x history buffer in frac16_t
* @param  in   frac16_t f16InX
*              - Input signal at the step K step
*                fractional value in the range <-1;1)
*
* @return This function filtered value
*              - frac16_t value <-1;1)
*
*            -----------
*     x(k)  |           |  y(k)
*   --------|   IIR1    |---------
*           |           |
*            -----------
*
*   x(k) - input signal
*   y(k) - filtered output signal
*
*   Filter equation:
*   y(n) = b0*x(n) + b1*x(n-1) - a1*y(n-1)
*
*   All coefficients must be divided by 2, i.e. if the desired b1 coefficient
*   is 1.756 the value is then:  b1 = FRAC32(1.786 / 2.0);
*
*   All A coefficients must be negated, i.e. if the desired a2 coefficient
*   is 3.789, the value is then: a2 = FRAC32(-3.789 / 2.0);
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac16_t GDFLIB_FilterIIR1_F16_FAsmi(frac16_t f16InX,
                                                   GDFLIB_FILTER_IIR1_T_F32 *psParam)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0, f32Val4=0;
    #if defined(__CC_ARM)                                        /* For ARM Compiler */
        __asm volatile{ sxth f16InX, f16InX                      /* Converts 16-bit input into 32-bit value */
                        ldr  f32Val1, [psParam, #0]              /* f32Val1= b0 */
                        ldr  f32Val2, [psParam, #4]              /* f32Val2= b1 */
                        ldr  f32Val3, [psParam, #8]              /* f32Val3= a1 */
                        ldrsh f32Val4, [psParam, #16]            /* f32Val4 = x(n-1) */
                        strh f16InX, [psParam, #16]              /* Saves x(n) for next step */
                        smull f32Val4, f32Val2, f32Val4, f32Val2 /* f32Val2:f32Val4 = b1*x(n-1) */
                        smlal f32Val4, f32Val2, f32Val1, f16InX  /* f32Val2:f32Val4 = b0*x(n) + b1*x(n-1) */
                        ldr f16InX, [psParam, #12]               /* f16InX = y(n-1) */
                        smull f32Val3, f32Val1, f32Val3, f16InX  /* f32Val1:f32Val3 = - a1*y(n-1) */
                        lsr f32Val3, f32Val3, #16                /* f32Val3 >> 16 */
                        add f32Val3, f32Val3, f32Val1, lsl #16   /* f32Val3 = f32Val3 + f32Val1 << 16 */
                        adds f32Val4, f32Val4, f32Val3           /* f32Val4 = f32Val4 + f32Val3 */
                        adc f32Val2, f32Val2, f32Val1, asr #16   /* f32Val2:f32Val4 = b0*x(n) + b1*x(n-1) - a1*y(n-1) */
                        lsr f16InX, f32Val4, #14                 /* f16InX = f32Val4 >> 14 */
                        add f16InX, f16InX, f32Val2, lsl #18     /* f16InX = f16InX + f32Val2 << 18 */
                        asr f32Val2, f32Val2, #13                /* f32Val2 >> 13 */
                        cmp f32Val2, f16InX, asr #31             /* Compares f32Val2 with (f16InX >> 31)*/
                        ittt ne                                  /* If f32Val2 != (f16InX >> 18), then saturates output */
                        ssatne f16InX, #1, f32Val2               /* Saturates f32Val2 as an 1 bit value */
                        addne f16InX, f16InX, #1                 /* f16InX + 1 */
                        rsbne f16InX, f16InX, #0x80000000        /* f16InX = 0x80000000 - f16InX */
                        str f16InX, [psParam, #12]               /* Saves y(n) for next step */
                        asrs f16InX, f16InX, #16 };              /* f16InX >> 16 */
    #else
        __asm volatile( "sxth %0, %0 \n"                /* Converts 16-bit input into 32-bit value */
                        "ldr %1, [%5, #0]\n"            /* f32Val1= b0 */
                        "ldr %2, [%5, #4]\n"            /* f32Val2= b1 */
                        "ldr %3, [%5, #8]\n"            /* f32Val3= a1 */
                        "ldrsh %4, [%5, #16] \n"        /* f32Val4 = x(n-1) */
                        "strh %0, [%5, #16] \n"         /* Saves x(n) for next step */
                        "smull %4, %2, %4, %2 \n"       /* f32Val2:f32Val4 = b1*x(n-1) */
                        "smlal %4, %2, %1, %0 \n"       /* f32Val2:f32Val4 = b0*x(n) + b1*x(n-1) */
                        "ldr %0, [%5, #12] \n"          /* f16InX = y(n-1) */
                        "smull %3, %1, %3, %0 \n"       /* f32Val1:f32Val3 = - a1*y(n-1) */
                        "lsr %3, %3, #16 \n"            /* f32Val3 >> 16 */
                        "add %3, %3, %1, lsl #16 \n"    /* f32Val3 = f32Val3 + f32Val1 << 16 */
                        "adds %4, %4, %3 \n"            /* f32Val4 = f32Val4 + f32Val3 */
                        "adc %2, %2, %1, asr #16 \n"    /* f32Val2:f32Val4 = b0*x(n) + b1*x(n-1) - a1*y(n-1) */
                        "lsr %0, %4, #14 \n"            /* f16InX = f32Val4 >> 14 */
                        "add %0, %0, %2, lsl #18 \n"    /* f16InX = f16InX + f32Val2 << 18 */
                        "asr %2, %2, #13 \n"            /* f32Val2 >> 13 */
                        "cmp %2, %0, asr #31 \n"        /* Compares f32Val2 with (f16InX >> 31)*/
                        "ittt ne \n"                    /* If f32Val2 != (f16InX >> 18), then saturates output */
                        "ssatne %0, #1, %2 \n"          /* Saturates f32Val2 as an 1 bit value */
                        "addne %0, %0, #1 \n"           /* f16InX + 1 */
                        "rsbne %0, %0, #0x80000000 \n"  /* f16InX = 0x80000000 - f16InX */
                        "str %0, [%5, #12] \n"          /* Saves y(n) for next step */
                        "asrs %0, %0, #16 \n"           /* f16InX >> 16 */
                        : "+l"(f16InX), "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3), "+l"(f32Val4): "l"(psParam));
    #endif

    return(f16InX);
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR1_F32_ASMI_H_ */
