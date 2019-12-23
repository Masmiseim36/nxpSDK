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
#ifndef _MLIB_ADD4_F16_ASM_H_
#define _MLIB_ADD4_F16_ASM_H_

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
#define MLIB_Add4Sat_F16_Asmi(f16Add1, f16Add2, f16Add3, f16Add4)             \
        MLIB_Add4Sat_F16_FAsmi(f16Add1, f16Add2, f16Add3, f16Add4)

/***************************************************************************//*!
*  Saturated summation of four inputs      
*  Output = f16Add1 + f16Add2 + f16Add3 + f16Add4
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_Add4Sat_F16_FAsmi(register frac16_t f16Add1, register frac16_t f16Add2,
                                              register frac16_t f16Add3, register frac16_t f16Add4)
{        
    register frac32_t f32Val = 0x8000;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ sxth f16Add1, f16Add1           /* Transforms 16-bit input f16Add1 to 32-bit */
                        sxth f16Add2, f16Add2           /* Transforms 16-bit input f16Add2 to 32-bit */
                        sxth f16Add3, f16Add3           /* Transforms 16-bit input f16Add1 to 32-bit */
                        sxth f16Add4, f16Add4           /* Transforms 16-bit input f16Add2 to 32-bit */
                        adds f16Add1, f16Add2, f16Add1  /* f16Add1 = f16Add1 + f16Add2 */
                        adds f16Add1, f16Add1, f16Add3  /* f16Add1 = f16Add1 + f16Add3 */
                        adds f16Add1, f16Add1, f16Add4  /* f16Add1 = f16Add1 + f16Add4 */

                        cmp f32Val, f16Add1             /* Compares f16Add1 with 0x8000 */
                        bgt NegTest                     /* If f16Add1 < 0x8000, then goes to NegTest */
                        subs f16Add1, f32Val, #1        /* If f16Add1 >= 0x8000, then f16Add1 = 0x7FFF */
                        b SatEnd                        /* Goes to SatEnd */
                    NegTest:
                        sxth f32Val, f32Val             /* f32Val = 0xFFFF8000 */
                        cmp f32Val, f16Add1             /* Compares f16Add1 with 0xFFFF8000 */
                        ble SatEnd                      /* If f16Add1 >= 0xFFFF8000, then goes to SatEnd */
                        mov f16Add1, f32Val             /* If f16Add1 < 0xFFFF8000, then f16Add1 = 0xFFFF8000 */
                    SatEnd: };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "sxth %0, %0 \n\t"                /* Transforms 16-bit input f16Add1 to 32-bit */
                        "sxth %1, %1 \n\t"                /* Transforms 16-bit input f16Add2 to 32-bit */
                        "sxth %2, %2 \n\t"                /* Transforms 16-bit input f16Add1 to 32-bit */
                        "sxth %3, %3 \n\t"                /* Transforms 16-bit input f16Add2 to 32-bit */
                        "adds %0, %0, %1 \n\t"            /* f16Add1 = f16Add1 + f16Add2 */
                        "adds %0, %0, %2 \n\t"            /* f16Add1 = f16Add1 + f16Add3 */
                        "adds %0, %0, %3 \n\t"            /* f16Add1 = f16Add1 + f16Add4 */

                        "cmp %4, %0 \n\t"                 /* Compares f16Add1 with 0x8000 */
                        "bgt MLIB_Add4Sat_F16_NegTest \n\t"                    /* If f16Add1 < 0x8000, then jumps through two commands */
                        "subs %0, %4, #1 \n\t"            /* If f16Add1 >= 0x8000, then f16Add1 = 0x7FFF */
                        "b MLIB_Add4Sat_F16_SatEnd \n\t"                     /* Jumps through four commands */
					"MLIB_Add4Sat_F16_NegTest: \n\t" 
                        "sxth %4, %4 \n\t"                /* f32Val = 0xFFFF8000 */
                        "cmp %4, %0 \n\t"                 /* Compares f16Add1 with 0xFFFF8000 */
                        "ble MLIB_Add4Sat_F16_SatEnd \n\t"                    /* If f16Add1 >= 0xFFFF8000, then jumps through next commands */
                        "mov %0, %4 \n\t"                 /* If f16Add1 < 0xFFFF8000, then f16Add1 = 0xFFFF8000 */
                    "MLIB_Add4Sat_F16_SatEnd: \n\t"
                        : "+l"(f16Add1), "+l"(f16Add2), "+l"(f16Add3), "+l"(f16Add4), "+l"(f32Val):);
    #endif

    return f16Add1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD4_F16_ASM_H_ */
