/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
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
RAM_FUNC_LIB 
RTCESL_INLINE static inline float_t GFLIB_Sqrt_FLT_FAsmi(register float_t fltVal)
{
    #if defined(__CC_ARM)                        /* For ARM Compiler */
        __asm volatile{ vsqrt.f32 fltVal, fltVal };
    #else
    __asm volatile(
                    #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                        ".syntax unified \n"           /* Using unified asm syntax */
                    #endif
                    "vsqrt.f32 %0, %0 \n"
                    #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                        ".syntax divided \n"
                    #endif
                    : "+t"(fltVal):);
    #endif
    
    return fltVal;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_SQRT_FLT_ASM_H_*/
