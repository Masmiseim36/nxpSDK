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
* @brief  Sign
* 
*******************************************************************************/
#ifndef _MLIB_SIGN_F32_ASM_H_
#define _MLIB_SIGN_F32_ASM_H_

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
#define MLIB_Sign_F32_Asmi(f32Val) MLIB_Sign_F32_FAsmi(f32Val)

/***************************************************************************//*!
*   This function returns the sign of the argument:
*           f32Val = 0: returns 0
*           f32Val > 0: returns 2147483647 (fractional 1.0)
*           f32Val < 0: returns -2147483648 (fractional -1.0)
*
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_Sign_F32_FAsmi(register frac32_t f32Val)
{
    register frac32_t f32SigVal = 0x80;
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ cmp f32Val, #0                  /* Compares f32Val with 0 */
                        bge Next                        /* If f32Val >= 0, then goes to Next */
                        rev f32Val, f32SigVal           /* If f32Val < 0, then f32Val = 0x80000000 */
                    Next:
                        ble SignEnd                     /* If f32Val = 0, then goes to SignEnd */
                        rev f32Val, f32SigVal           /* If f32Val > 0, then f32Val = 0x80000000 */
                        subs f32Val, f32Val, #1         /* f32Val = f32Val - 1 */
                    SignEnd: };
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "cmp %0, #0 \n"                 /* Compares f32Val with 0 */
                        "bge .+4 \n"                    /* If f32Val >= 0, then jumps through next command */
                        "rev %0, %1 \n"                 /* If f32Val < 0, then f32Val = 0x80000000 */
                        "ble .+6 \n"                    /* If f32Val = 0, then jumps through next command */
                        "rev %0, %1 \n"                 /* If f32Val > 0, then f32Val = 0x80000000 */
                        "subs %0, %0, #1 \n"            /* f32Val = f32Val - 1 */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Val): "l"(f32SigVal));
    #endif

    return f32Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SIGN_F32_ASM_H_ */
