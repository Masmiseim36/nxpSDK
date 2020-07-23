/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
