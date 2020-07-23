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
#ifndef _MLIB_DIV1Q_F16_H_
#define _MLIB_DIV1Q_F16_H_

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
#define MLIB_Div1Q_F16_Ci(f16Num, f16Denom)    MLIB_Div1Q_F16_FCi(f16Num, f16Denom) 
#define MLIB_Div1QSat_F16_Ci(f16Num, f16Denom) MLIB_Div1QSat_F16_FCi(f16Num, f16Denom)  

/***************************************************************************//*!
* @brief  16-bit inputs 16-output single quadrant division function
*
* @param  in frac16_t f16Num   - Numerator in <0;1) in frac16_t
*            frac16_t f16Denom - Denominator in <0;1) in frac16_t                      
*
* @return This function returns - frac16_t value <0;1)
*       
* @remarks  This function divides two non-negative fractional inputs:
*           result = f16Num / f16Denom.
*           The function does not saturate the output.
*           If the denominator is 0, the output is 0x7FFF.          
*
****************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t MLIB_Div1Q_F16_FCi(register frac16_t f16Num, register frac16_t f16Denom)
{
    if (f16Denom == (frac16_t)0)
    {
        return((frac16_t)INT16_MAX);
    }
    else
    {
        return((frac16_t)((frac32_t)(f16Num << 15) / (frac32_t)f16Denom));
    }
}
 
/***************************************************************************//*!
* @brief  16-bit inputs 16-output single quadrant division function with saturation
*
* @param  in  frac16_t f16Num  - Numerator in <0;1) in frac16_t
*             frac16_t f16Denom- Denominator in <0;1) in frac16_t                      
*
* @return This function returns - frac16_t value <0;1)
*       
* @remarks  This function divides two non-negative fractional inputs:
*           result = f16Num / f16Denom.
*           The function saturates the output if f16Num > f16Denom
*           to 0x7FFF.
*
****************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t MLIB_Div1QSat_F16_FCi(register frac16_t f16Num, register frac16_t f16Denom)
{
    if (f16Denom == (frac16_t)0)
    {
        return((frac16_t)INT16_MAX);
    }
    else
    {
        if (f16Num >= f16Denom)
        {
            return((frac16_t)INT16_MAX);
        }
        else
        {
            return(frac16_t)((frac32_t)(f16Num << 15) / (frac16_t)f16Denom);
        }
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_DIV1Q_F16_H_ */ 
