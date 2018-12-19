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
#ifndef _MLIB_SIGN_F16_ASM_H_
#define _MLIB_SIGN_F16_ASM_H_

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
#define MLIB_Sign_F16_Asmi(f16Val) MLIB_Sign_F16_FAsmi(f16Val)

/***************************************************************************//*!
*     This function returns the sign of the argument:
*            f16Val = 0: returns 0
*            f16Val > 0: returns 32767 (fractional 1.0)
*            f16Val < 0: returns -32768 (fractional -1.0)
*
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac16_t MLIB_Sign_F16_FAsmi(register frac16_t f16Val)
{
    #if defined(__CC_ARM)                           /* For ARM Compiler */
        __asm volatile{ lsl f16Val, f16Val, #16     /* f16Val << 16 */
                        ssat f16Val, #16, f16Val }; /* Saturates to 16-bit value */
    #else
        __asm volatile( "lsl %0, %0, #16 \n"        /* f16Val << 16 */
                        "ssat %0, #16, %0 \n"       /* Saturates to 16-bit value */
                        : "+l"(f16Val):);
    #endif

    return f16Val;
}
RTCESL_INLINE_OPTIM_RESTORE 


#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_SIGN_F16_ASM_H_ */
