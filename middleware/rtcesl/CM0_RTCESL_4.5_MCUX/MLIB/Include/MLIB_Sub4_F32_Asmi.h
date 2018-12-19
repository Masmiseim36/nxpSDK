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
* @brief  Subtraction of three input values from the first input
* 
*******************************************************************************/
#ifndef _MLIB_SUB4_F32_ASM_H_
#define _MLIB_SUB4_F32_ASM_H_

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
#define MLIB_Sub4Sat_F32_Asmi(f32Min, f32Sub1, f32Sub2, f32Sub3)               \
        MLIB_Sub4Sat_F32_FAsmi(f32Min, f32Sub1, f32Sub2, f32Sub3)

/***************************************************************************//*!
*  Subtraction of three input values from the first input      
*    Output = f32Min - f32Sub1 - f32Sub2 - f32Sub3
*  function is saturated
*******************************************************************************/  
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_Sub4Sat_F32_FAsmi(register frac32_t f32Min, register frac32_t f32Sub1, 
                                              register frac32_t f32Sub2, register frac32_t f32Sub3)
{
    register frac32_t f32Val1=0, f32Val2=0;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ asrs f32Val1, f32Min, #31       /* f32Val1 = sign of f32Min */
                        asrs f32Val2, f32Sub1, #31      /* f32Val2 = sign of f32Sub1 */

                        subs f32Min, f32Min, f32Sub1    /* f32Min = f32Min - f32Sub1 */
                        sbcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 - f32Val2 - carry */

                        asrs f32Val2, f32Sub2, #31      /* f32Val2 = sign of f32Sub2 */
                        subs f32Min, f32Min, f32Sub2    /* f32Min = f32Min - f32Sub2 */
                        sbcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 - f32Val2 - carry */

                        asrs f32Val2, f32Sub3, #31      /* f32Val2 = sign of f32Sub3 */
                        subs f32Min, f32Min, f32Sub3    /* f32Min = f32Min - f32Sub3 */
                        sbcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 - f32Val2 - carry */

                        lsls f32Val1, f32Val1, #16      /* f32Val1 << 16 */
                        lsrs f32Val2, f32Min, #16       /* f32Val2 = f32Min >> 16 */
                        orrs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 | f32Val2 */

                        movs f32Val2, #128              /* f32Val2 = 0x80 */
                        lsls f32Val2, f32Val2, #8       /* f32Val2 = 0x8000 */
                        cmp f32Val2, f32Val1            /* Compares f32Val1 with 0x00008000 */
                        bgt NegTest                     /* If f32Val1 < 0x00008000, then goes to NegTest */
                        lsls f32Min, f32Val2, #16       /* If f32Val1 >= 0x00008000, then f32Min = 0x80000000 */
                        subs f32Min, f32Min, #1         /* f32Min = 0x7FFFFFFF */
                        b SatEnd                        /* Goes to SatEnd */
                    NegTest:
                        mvns f32Sub3, f32Val2           /* f32Sub3 = 0xFFFF7FFF */
                        cmp f32Sub3, f32Val1            /* Compares f32Val1 with 0xFFFF7FFFF */
                        blt SatEnd                      /* If f32Val1 > 0xFFFF7FFFF, then goes to SatEnd */
                        lsls f32Min, f32Val2, #16       /* If f32Val1 <= 0xFFFF7FFFF, then f32Min = 0x80000000 */
                    SatEnd: };

    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "asrs %2, %0, #31 \n"           /* f32Val1 = sign of f32Min */
                        "asrs %3, %1, #31 \n"           /* f32Val2 = sign of f32Sub1 */

                        "subs %0, %0, %1 \n"            /* f32Min = f32Min - f32Sub1 */
                        "sbcs %2, %2, %3 \n"            /* f32Val1 = f32Val1 - f32Val2 - carry */

                        "asrs %3, %4, #31 \n"           /* f32Val2 = sign of f32Sub2 */
                        "subs %0, %0, %4 \n"            /* f32Min = f32Min - f32Sub2 */
                        "sbcs %2, %2, %3 \n"            /* f32Val1 = f32Val1 - f32Val2 - carry */

                        "asrs %3, %5, #31 \n"           /* f32Val2 = sign of f32Sub3 */
                        "subs %0, %0, %5 \n"            /* f32Min = f32Min - f32Sub3 */
                        "sbcs %2, %2, %3 \n"            /* f32Val1 = f32Val1 - f32Val2 - carry */

                        "lsls %2, %2, #16 \n"           /* f32Val1 << 16 */
                        "lsrs %3, %0, #16 \n"           /* f32Val2 = f32Min >> 16 */
                        "orrs %2, %2, %3 \n"            /* f32Val1 = f32Val1 | f32Val2 */

                        "movs %3, #128 \n"              /* f32Val2 = 0x80 */
                        "lsls %3, %3, #8 \n"            /* f32Val2 = 0x8000 */
                        "cmp %3, %2 \n"                 /* Compares f32Val1 with 0x00008000 */
                        "bgt .+8 \n"                    /* If f32Val1 < 0x00008000, then jumps through three commands */
                        "lsls %0, %3, #16 \n"           /* If f32Val1 >= 0x00008000, then f32Min = 0x80000000 */
                        "subs %0, %0, #1 \n"            /* f32Min = 0x7FFFFFFF */
                        "b .+10 \n"                     /* Jumps through four commands */
                        "mvns %1, %3 \n"                /* f32Sub1 = 0xFFFF7FFF */
                        "cmp %1, %2 \n"                 /* Compares f32Val1 with 0xFFFF7FFFF */
                        "blt .+4 \n"                    /* If f32Val1 > 0xFFFF7FFFF, then jumps through next commands */
                        "lsls %0, %3, #16 \n"           /* If f32Val1 <= 0xFFFF7FFFF, then f32Min = 0x80000000 */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                       : "+l"(f32Min), "+l"(f32Sub1), "+l"(f32Val1), "+l"(f32Val2): "l"(f32Sub2), "l"(f32Sub3));
    #endif

    return f32Min;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_SUB4_F32_ASM_H_ */
