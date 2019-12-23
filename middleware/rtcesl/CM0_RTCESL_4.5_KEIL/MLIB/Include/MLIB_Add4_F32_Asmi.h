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
* @brief  Summation of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_ADD4_F32_ASM_H_
#define _MLIB_ADD4_F32_ASM_H_

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
#define MLIB_Add4Sat_F32_Asmi(f32Add1, f32Add2, f32Add3, f32Add4)              \
        MLIB_Add4Sat_F32_FAsmi(f32Add1, f32Add2, f32Add3, f32Add4)

/***************************************************************************//*!
*  Saturated summation of four inputs      
*  Output = f32Add1 + f32Add2 + f32Add3 + f32Add4
*******************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_Add4Sat_F32_FAsmi(register frac32_t f32Add1, register frac32_t f32Add2,
                                              register frac32_t f32Add3, register frac32_t f32Add4)
{
    register frac32_t f32Val1=0, f32Val2=0;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ asrs f32Val1, f32Add1, #31      /* f32Val1 = sign of f32Add1 */
                        asrs f32Val2, f32Add2, #31      /* f32Val2 = sign of f32Add2 */

                        adds f32Add1, f32Add1, f32Add2  /* f32Add1 = f32Add1 + f32Add2 */
                        adcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 + f32Val2 + carry */

                        asrs f32Val2, f32Add3, #31      /* f32Val2 = sign of f32Add3 */
                        adds f32Add1, f32Add1, f32Add3  /* f32Add1 = f32Add1 + f32Add3 */
                        adcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 + f32Val2 + carry */

                        asrs f32Val2, f32Add4, #31      /* f32Val2 = sign of f32Add4 */
                        adds f32Add1, f32Add1, f32Add4  /* f32Add1 = f32Add1 + f32Add4 */
                        adcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 + f32Val2 + carry */

                        lsrs f32Val2, f32Add1, #31      /* f32Val2 = first bit of f32Add1 */
                        adds f32Val2, f32Val2, f32Val1  /* f32Val2 = f32Val2 + f32Val1 */
                        beq SatEnd                      /* If r3 != 0, then saturates output */

                        movs f32Add1, #128              /* f32Add1 = 0x80 */
                        rev f32Add1, f32Add1            /* f32Add1 = 0x80000000 */
                        subs f32Add1, f32Add1, #1       /* f32Add1 = 0x7FFFFFFF */
                        asrs f32Val2, f32Val2, #16      /* f32Val2 = sign of result */
                        subs f32Add1, f32Add1, f32Val2  /* f32Add1 = 0x7FFFFFFF - f32Val2 */
                    SatEnd: };

    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "asrs %1, %0, #31 \n\t"           /* f32Val1 = sign of f32Add1 */
                        "asrs %2, %3, #31 \n\t"           /* f32Val2 = sign of f32Add2 */

                        "adds %0, %0, %3 \n\t"            /* f32Add1 = f32Add1 + f32Add2 */
                        "adcs %1, %1, %2 \n\t"            /* f32Val1 = f32Val1 + f32Val2 + carry */

                        "asrs %2, %4, #31 \n\t"           /* f32Val2 = sign of f32Add3 */
                        "adds %0, %0, %4 \n\t"            /* f32Add1 = f32Add1 + f32Add3 */
                        "adcs %1, %1, %2 \n\t"            /* f32Val1 = f32Val1 + f32Val2 + carry */

                        "asrs %2, %5, #31 \n\t"           /* f32Val2 = sign of f32Add4 */
                        "adds %0, %0, %5 \n\t"            /* f32Add1 = f32Add1 + f32Add4 */
                        "adcs %1, %1, %2 \n\t"            /* f32Val1 = f32Val1 + f32Val2 + carry */

                        "lsrs %2, %0, #31 \n\t"           /* f32Val2 = first bit of f32Add1 */
                        "adds %2, %2, %1 \n\t"            /* f32Val2 = f32Val2 + f32Val1 */
                        "beq MLIB_Add4Sat_F32_SatEnd \n\t"                      /* If r3 != 0, then saturates output */

                        "movs %0, #128 \n\t"              /* f32Add1 = 0x80 */
                        "rev %0, %0 \n\t"                 /* f32Add1 = 0x80000000 */
                        "subs %0, %0, #1 \n\t"            /* f32Add1 = 0x7FFFFFFF */
                        "asrs %2, %2, #16 \n\t"           /* f32Val2 = sign of result */
                        "subs %0, %0, %2 \n\t"            /* f32Add1 = 0x7FFFFFFF - f32Val2 */
					"MLIB_Add4Sat_F32_SatEnd: \n\t"					
                       : "+l"(f32Add1), "+l"(f32Val1), "+l"(f32Val2): "l"(f32Add2), "l"(f32Add3), "l"(f32Add4));
    #endif

    return f32Add1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_ADD4_F32_ASM_H_ */
