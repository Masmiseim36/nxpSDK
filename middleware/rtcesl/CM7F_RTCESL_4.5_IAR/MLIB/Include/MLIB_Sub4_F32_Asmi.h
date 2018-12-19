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
#define MLIB_Sub4_F32_Asmi(f32Min, f32Sub1, f32Sub2, f32Sub3)                  \
        MLIB_Sub4_F32_FAsmi(f32Min, f32Sub1, f32Sub2, f32Sub3)
#define MLIB_Sub4Sat_F32_Asmi(f32Min, f32Sub1, f32Sub2, f32Sub3)               \
        MLIB_Sub4Sat_F32_FAsmi(f32Min, f32Sub1, f32Sub2, f32Sub3)
                                                                  
/***************************************************************************//*!
*  Subtraction of three input values from the first input
*     f32Out = f32Min + f32Sub1 + f32Sub2 + f32Sub3
*  function is not saturated
*******************************************************************************/  
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Sub4_F32_FAsmi(register frac32_t f32Min, register frac32_t f32Sub1,
                                           register frac32_t f32Sub2, register frac32_t f32Sub3)
{
    #if defined(__CC_ARM)                              /* For ARM Compiler */
        __asm volatile{sub f32Min, f32Min, f32Sub1     /* f32Min = f32Min - f32Sub1 */
                       sub f32Min, f32Min, f32Sub2     /* f32Min = f32Min - f32Sub2 */
                       sub f32Min, f32Min, f32Sub3};   /* f32Min = f32Min - f32Sub3 */

    #else
        __asm volatile( "sub %0, %0, %1 \n"            /* f32Min = f32Min - f32Sub1 */
                        "sub %0, %0, %2 \n"            /* f32Min = f32Min - f32Sub2 */
                        "sub %0, %0, %3 \n"            /* f32Min = f32Min - f32Sub3 */
                        : "+l"(f32Min): "l"(f32Sub1), "l"(f32Sub2), "l"(f32Sub3));
    #endif

    return f32Min;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*  Subtraction of three input values from the first input
*     f32Out = f32Min + f32Sub1 + f32Sub2 + f32Sub3
*  function is saturated
*******************************************************************************/  
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Sub4Sat_F32_FAsmi(register frac32_t f32Min, register frac32_t f32Sub1, 
                                              register frac32_t f32Sub2, register frac32_t f32Sub3)
{
    register frac32_t f32Sign1=0, f32Sign2=0;
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ asr f32Sign1, f32Min, #31           /* f32Sign1 = sign of f32Min */
                        asr f32Sign2, f32Sub1, #31          /* f32Sign2 = sign of f32Sub1 */
                        subs f32Min, f32Min, f32Sub1        /* f32Min - f32Sub1 */
                        sbc f32Sign1, f32Sign1, f32Sign2    /* f32Sign1 - f32Sign2 - Carry */

                        asr f32Sign2, f32Sub2, #31          /* f32Sign2 = sign of f32Sub2 */
                        subs f32Min, f32Min, f32Sub2        /* f32Min - f32Sub1 - f32Sub2 */
                        sbc f32Sign1, f32Sign1, f32Sign2    /* f32Sign1 - f32Sign2 - Carry */

                        asr f32Sign2, f32Sub3, #31          /* f32Sign2 = sign of f32Sub3 */
                        subs f32Min, f32Min, f32Sub3        /* f32Min - f32Sub1 - f32Sub2 - f32Sub3 */
                        sbc f32Sign1, f32Sign1, f32Sign2    /* f32Sign1 - f32Sign2 - Carry */

                        mov f32Sign2, #0x80000000           /* f32Sign2 = 0x80000000 */

                        lsr f32Sub1, f32Min, #31            /* First bit of low part of result */
                        adds f32Sign1, f32Sign1, f32Sub1

                        it gt                               /* If f32Sign1 > 0, then executes next command */
                        subgt f32Min, f32Sign2, #1          /* Result = 0x7FFFFFFF*/

                        it lt                               /* If f32Sign1 > 0, then executes next command */
                        movlt f32Min, f32Sign2  };          /* Result = 0x80000000*/
    #else
        __asm volatile( "asr %0, %2, #31 \n"                /* f32Sign1 = sign of f32Min */
                        "asr %1, %3, #31 \n"                /* f32Sign2 = sign of f32Sub1 */
                        "subs %2, %2, %3 \n"                /* f32Min - f32Sub1 */
                        "sbc %0, %0, %1 \n"                 /* f32Sign1 - f32Sign2 - Carry */

                        "asr %1, %4, #31 \n"                /* f32Sign2 = sign of f32Sub2 */
                        "subs %2, %2, %4 \n"                /* f32Min - f32Sub1 - f32Sub2 */
                        "sbc %0, %0, %1 \n"                 /* f32Sign1 - f32Sign2 - Carry */

                        "asr %1, %5, #31 \n"                /* f32Sign2 = sign of f32Sub3 */
                        "subs %2, %2, %5 \n"                /* f32Min - f32Sub1 - f32Sub2 - f32Sub3 */
                        "sbc %0, %0, %1 \n"                 /* f32Sign1 - f32Sign2 - Carry */

                        "mov %1, #0x80000000 \n"            /* f32Sign2 = 0x80000000 */

                        "lsr %3, %2, #31 \n"                /* First bit of low part of result */
                        "adds %0, %0, %3 \n"

                        "it gt \n"                          /* If f32Sign1 > 0, then executes next command */
                        "subgt %2, %1, #1 \n"               /* Result = 0x7FFFFFFF*/

                        "it lt \n"                          /* If f32Sign1 > 0, then executes next command */
                        "movlt %2, %1 \n"                   /* Result = 0x80000000*/
                        : "+l"(f32Sign1), "+l"(f32Sign2), "+l"(f32Min), "+l"(f32Sub1): "l"(f32Sub2), "l"(f32Sub3) );
    #endif

    return f32Min;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_SUB4_F32_ASM_H_ */
