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
* @brief  Reciprocal value
* 
*******************************************************************************/
#ifndef _MLIB_RCP1Q_A32_H_
#define _MLIB_RCP1Q_A32_H_

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
#define MLIB_Rcp1Q1_A32s_Ci(f16Denom) MLIB_Rcp1Q1_A32s_FCi(f16Denom)  
#define MLIB_Rcp1Q_A32s_Ci(f16Denom)  MLIB_Rcp1Q_A32s_FCi(f16Denom)  

/***************************************************************************//*!
* @brief  16-bit input 32-output 16-bit precision one quadrant reciprocal function
*
* @param  in  frac16_t f16Denom - Denominator in <-1;1) in frac16_t
* @return This function returns - acc32_t value [-65536.0;65536.0 - (2^-15)]
*       
* @remarks  This function calculates the multiplicative inverse value of
*           the fractional input:
*           result = FRAC16(1) / f16Denom. The function calculates
*           the result with 16-bit division precision.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/
RAM_FUNC_LIB 
static inline acc32_t MLIB_Rcp1Q1_A32s_FCi(register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return(frac32_t)(INT32_MAX);
    }
    else
    {
        return(acc32_t)((frac32_t)0x40000000 / (frac32_t)f16Denom);
    }
}
 
/***************************************************************************//*!
* @brief  16-bit input 32-output 16-bit precision one quadrant reciprocal function
*
* @param  in frac16_t f16Denom - Denominator in <-1;1) in frac16_t
* @return This function returns- acc32_t value [-65536.0;65536.0 - (2^-15)]
*       
* @remarks  This function calculates the multiplicative inverse value of
*           the fractional input:
*           result = FRAC16(1) / f16Denom. The function calculates
*           the result with 16-bit division precision.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/
RAM_FUNC_LIB 
static inline acc32_t MLIB_Rcp1Q_A32s_FCi(register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return(frac32_t)(INT32_MAX);
    }
    else
    {
        return(acc32_t)((frac32_t)0x40000000 / (frac32_t)f16Denom);
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_RCP1Q_A32_H_ */
