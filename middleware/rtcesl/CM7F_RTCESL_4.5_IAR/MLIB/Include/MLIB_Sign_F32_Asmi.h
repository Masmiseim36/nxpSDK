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
*     This function returns the sign of the argument:
*            f32Val = 0: returns 0
*            f32Val > 0: returns 2147483647 (fractional 1.0)
*            f32Val < 0: returns -2147483648 (fractional -1.0)
*
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Sign_F32_FAsmi(register frac32_t f32Val)
{
    #if defined(__CC_ARM)                              /* For ARM Compiler */
        __asm volatile{ ssat f32Val, #2, f32Val        /* Saturates input to 2-bit value */
                        lsl f32Val, f32Val, #30        /* f16Val << 30 */
                        qadd f32Val, f32Val, f32Val }; /* Addition with saturation */
    #else
        __asm volatile( "ssat %0, #2, %0 \n"           /* Saturates input to 2-bit value */
                        "lsl %0, %0, #30 \n"           /* f16Val << 30 */
                        "qadd %0, %0, %0 \n"           /* Addition with saturation */
                        : "+l"(f32Val):);
    #endif

    return f32Val;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SIGN_F32_ASM_H_ */
