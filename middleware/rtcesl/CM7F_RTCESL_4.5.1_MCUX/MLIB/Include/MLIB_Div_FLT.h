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
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIV_FLT_H_
#define _MLIB_DIV_FLT_H_

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
#define MLIB_Div_FLT_Ci(fltNum, fltDenom) MLIB_Div_FLT_FCi(fltNum, fltDenom) 

/***************************************************************************//*!
* @brief  float numerator, float denominator inputs 16-output 1-quadrant
*         division function
*
* @param  in  float_t fltNum   - Numerator in float_t
*         in  float_t fltDenom - Denominator in  float_t
*
* @return This function returns - float_t value 
*      
* @remarks  This function divides two fractional:
*           result = fltNum / fltDenom. 
*           If denumerator is +zero(-zero) it returns plus infinity = 0x7F800000 
*           (or minus infinity = 0xFF800000). Both are not numbers float value.   
*           The output saturation is not implemented, thus in case 
*           the division of input values is outside the (-2^128, 2^128) 
*           interval, the output value will overflow without any detection. 
*
*******************************************************************************/
RAM_FUNC_LIB 
static inline float_t MLIB_Div_FLT_FCi(register float_t fltNum, register float_t fltDenom)
{   
    return((float_t)(fltNum / fltDenom));
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIV_FLT_H_ */
