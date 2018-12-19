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
* @brief  Square root 
* 
*******************************************************************************/
#ifndef _MLIB_SQRT_FLT_ASM_H_
#define _MLIB_SQRT_FLT_ASM_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib_FP.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Sqrt_FLT_Asmi(fltVal) GFLIB_Sqrt_FLT_FAsmi(fltVal)
  

/***************************************************************************//*!
* @brief     This function returns the square root of input value.
*
* @param     in fltVal - The input value.
*
* @return    The function returns the square root of the input value. The
*            return value is float range. If input is <= 0, then the function 
*            returns 0. The function uses VSQRT instruction in Cortex M FPU.
*
* @remarks   The function uses VSQRT instruction in Cortex M FPU.
*
****************************************************************************/  
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline float_t GFLIB_Sqrt_FLT_FAsmi(register float_t fltVal)
{
    #if defined(__CC_ARM)                        /* For ARM Compiler */
        __asm volatile{ vsqrt.f32 fltVal, fltVal };
    #else
        __asm volatile( "vsqrt.f32 %0, %0"
                       : "+t"(fltVal):);
    #endif
    
    return fltVal;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_SQRT_FLT_ASM_H_*/
