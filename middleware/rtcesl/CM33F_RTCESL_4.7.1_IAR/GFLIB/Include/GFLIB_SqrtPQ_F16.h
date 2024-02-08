/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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

#ifndef _GFLIB_SQRTPQ_F16_H_
#define _GFLIB_SQRTPQ_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SqrtPQ_F16_Ci(f16Val) GFLIB_SqrtPQ_F16_FCi(f16Val)
  
/****************************************************************************
* Inline functions 
****************************************************************************/

/***************************************************************************//*!
*
* @brief  Calculates the square root of the argument.
*
* @param    ptr  GFLIB_SQRT_TABLE_T_F32 *psParam - Pointer to the polynomial table 
* @param    in   frac32_t f32Val - Argument in <0;1) in frac32_t
*
* @return This function returns - frac16_t value <0;1)
*		
* @remarks 	This function calculates square root. The argument must be in the
*		range [0;1]. The algorithm calculates square root using PQ module.
*		If the value is negative the function returns zero value.
*
****************************************************************************/
static inline frac16_t GFLIB_SqrtPQ_F16_FCi(frac16_t f16Val)
{
    frac32_t f32Temp;
    
    if(f16Val > 0)
    {   
        f32Temp = MLIB_ShL_F32_Ci((frac32_t)f16Val, 15U);
        RTCESL_PQ_SQRT_FLT0((uint32_t)f32Temp);
        f32Temp = (frac32_t)RTCESL_PQ_READ_MULT0_FLT();
        return (frac16_t)f32Temp;
    }
    else
    {
        f32Temp = 0;
    }   
    return (frac16_t)f32Temp;  
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRTPQ_F16_H_ */
