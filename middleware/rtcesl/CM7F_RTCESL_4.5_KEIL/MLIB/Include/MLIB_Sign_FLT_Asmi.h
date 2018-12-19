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
#ifndef _MLIB_SIGN_FLT_ASMI_H_
#define _MLIB_SIGN_FLT_ASMI_H_

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
#define MLIB_Sign_FLT_Asmi(fltVal) MLIB_Sign_FLT_FAsmi(fltVal)

/***************************************************************************//*! 
*  This function returns the sign of the argument:
*  fltVal = 0: returns  0.0F
*  fltVal > 0: returns  1.0F 
*  fltVal < 0: returns -1.0F
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)            /* IAR compiler */
#pragma diag_suppress=Pe549                 /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline float_t MLIB_Sign_FLT_FAsmi(register float_t fltVal)
{
    register frac32_t f32Val1=0, f32Val2=0;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ vmov  f32Val1, fltVal               /* f32Val = fltVal */
                        lsls f32Val2, f32Val1, #1           /* Clears the sign */
                        itt ne                              /* If fltVal != 0, then executes next two commands */
                        andne f32Val1, f32Val1, #0x80000000 /* f32Val = f32Val & 0x80000000 */
                        orrne f32Val1, f32Val1, #0x3F800000 /* f32Val = f32Val | 0x3F800000 */
                        vmov  fltVal, f32Val1  };           /* fltVal = f32Val */
    #else
        __asm volatile( "vmov  %1, %0 \n"                   /* f32Val = fltVal */
                        "lsls %2, %1, #1 \n"                /* Clears the sign */
                        "itt ne \n"                         /* If fltVal != 0, then executes next two commands */
                        "andne %1, %1, #0x80000000 \n"      /* f32Val = f32Val & 0x80000000 */
                        "orrne %1, %1, #0x3F800000 \n"      /* f32Val = f32Val | 0x3F800000 */
                        "vmov  %0, %1 \n"                   /* fltVal = f32Val */
                        : "+t"(fltVal), "+l"(f32Val1), "+l"(f32Val2):);
    #endif
    return (fltVal);
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)            /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SIGN_FLT_ASMI_H_ */
