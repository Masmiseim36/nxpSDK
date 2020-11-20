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
#ifndef _MLIB_DIV_A32_H_
#define _MLIB_DIV_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_BiShift_F32.h"
#include "MLIB_Clb_F32.h"  

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define MLIB_Div_A32ss_Ci(f16Num, f16Denom)  MLIB_Div_A32ss_FCi(f16Num, f16Denom)
#define MLIB_Div_A32ls_Ci(f32Num, f16Denom)  MLIB_Div_A32ls_FCi(f32Num, f16Denom)

#define MLIB_Div_A32ll_Asm(f32Num, f32Denom) MLIB_Div_A32ll_FAsm(f32Num, f32Denom)
#define MLIB_Div_A32ll_AsmRam(f32Num, f32Denom) MLIB_Div_A32ll_FAsmRam(f32Num, f32Denom)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
RAM_FUNC_LIB 
extern acc32_t MLIB_Div_A32ll_FAsmRam(frac32_t f32Num, frac32_t f32Denom);  

extern acc32_t MLIB_Div_A32ll_FAsm(frac32_t f32Num, frac32_t f32Denom);  

/***************************************************************************//*!
*
* @brief  16-bit numerator, 16-bit denominator inputs 32-bit accumulator output
*         4-quadrant division function
*
* @param  in  frac16_t f16Num - Numerator in <-1;1) in frac16_t
*             frac16_t f16Denom- Denominator in <-1;1) in frac16_t
*                       
* @return This function returns
*          - acc32_t value [-65536.0;65536.0 - (2^-15)]
*       
* @remarks  This function divides fractional inputs:
*           result = f16Num / f16Denom.
*           The output is the accumulator type where the lower word is the fractional part.
*           If the denominator is 0, the output is 0x7FFF FFFF or 0x8000 0000
*           depending on the numerator's sign. For the both are 0, the output is
*           0x7FFF FFFF.
*
****************************************************************************/  
RAM_FUNC_LIB 
static inline acc32_t MLIB_Div_A32ss_FCi(register frac16_t f16Num, register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return((f16Num >= 0) ? INT32_MAX : INT32_MIN);
    }
    else
    {
        return(acc32_t)((frac32_t)(f16Num << 15) / (frac32_t)f16Denom);
    }
}
 
/***************************************************************************//*!
*
* @brief  32-bit numerator, 16-bit denominator inputs 32-bit accumulator output
*         4-quadrant division function      
* 
* @param  in  frac32_t f32Num - Numerator in <-1;1) in frac32_t
*             frac16_t f16Denom - Denominator in <-1;1) in frac16_t
*                       
* @return This function returns
*         - acc32_t value [-65536.0;65536.0 - (2^-15)]
*       
* @remarks  This function divides two negative fractional inputs:
*           result = f32Num / f16Denom.
*           The output is the accumulator type where the lower
*           word is the fractional part.
*           If the denominator is 0, the output is 0x7FFF FFFF or 0x8000 0000
*           depending on the numerator's sign. For the both are 0, the output is
*           0x7FFF FFFF.
*
****************************************************************************/
RAM_FUNC_LIB 
static inline acc32_t MLIB_Div_A32ls_FCi(register frac32_t f32Num, register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return((f32Num >= 0) ? INT32_MAX : INT32_MIN);
    }
    else
    {
        return(acc32_t)((frac32_t)(f32Num / (frac32_t)f16Denom) >> 1);
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_DIV_A32_H_ */
