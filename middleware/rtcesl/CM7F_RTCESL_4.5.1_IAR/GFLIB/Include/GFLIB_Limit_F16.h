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
* @brief  Limit functions with 16-bit fractional output
* 
*******************************************************************************/
#ifndef _GFLIB_LIMIT_F16_H_
#define _GFLIB_LIMIT_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gflib_types.h"
 
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Limit_F16_Ci(f16Val, f16LLim, f16ULim)                         \
        GFLIB_Limit_F16_FCi(f16Val, f16LLim, f16ULim)

/***************************************************************************//*!
*
* @brief    Limit function
*   
* @param    in   frac16_t f16Val  - Argument in <-1;1) in frac16_t
*                frac16_t f16LLim - LowerLimit in <-1;1) in frac16_t
*                frac16_t f16ULim - UpperLimit in <-1;1) in frac16_t
*                         
* @return   This function returns - frac16_t value <-1;1)
*       
* @remarks  This function trims the argument according to the upper f16ULim and 
*           lower f16LLim limits. The upper limit must >= lower limit.
*
****************************************************************************/  
RAM_FUNC_LIB 
static inline frac16_t GFLIB_Limit_F16_FCi(frac16_t f16Val, 
                                           frac16_t f16LLim, frac16_t f16ULim)
{
    if(f16Val > f16ULim) 
    {
        return(f16ULim);
    }
    if(f16Val < f16LLim) 
    {
        return(f16LLim);
    }
    return(f16Val);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LIMIT_F16_H_ */
