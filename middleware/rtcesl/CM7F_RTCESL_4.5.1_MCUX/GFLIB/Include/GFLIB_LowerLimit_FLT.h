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
* @brief  Lower limit functions with float output
* 
*******************************************************************************/
#ifndef _GFLIB_LOWERLIMIT_FLT_H_
#define _GFLIB_LOWERLIMIT_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_LowerLimit_FLT_Ci(fltVal, fltLLim)                               \
        GFLIB_LowerLimit_FLT_FCi(fltVal, fltLLim)
  
/***************************************************************************//*!
*
* @brief    Lower limit function
*   
* @param    in  float_t fltVal  - Argument in float_t
*               float_t fltLLim - LowerLimit in float_t
*                         
* @return   This function returns - float_t value 
*       
* @remarks  This function trims the argument above or equal to lower fltLLim 
*           limit. 
*
****************************************************************************/ 
RAM_FUNC_LIB 
static inline float_t GFLIB_LowerLimit_FLT_FCi(float_t fltVal, float_t fltLLim)
{
    if(fltVal < fltLLim)
    {
        fltVal = fltLLim;
    } 
    return(fltVal);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LOWERLIMIT_FLT_H_ */


