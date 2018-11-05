/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
* @brief  Hardware Square root
* 
*******************************************************************************/
#ifndef _GFLIB_SQRTHW_F32_H_
#define _GFLIB_SQRTHW_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "MLIB_MMDVSQ.h"

/*******************************************************************************
* Macros 
*******************************************************************************/ 
#define GFLIB_SqrtHw_F16l_C(f32Val) GFLIB_SqrtHw_F16l_FC(f32Val) 

/***************************************************************************//*!
* @brief        This function returns the square root of input value.
*
* @param[in]         f32Val      The input value.
*
* @return      The function returns the square root of the input value. The
*              return value is within the [0, 1) 16 bit fraction range.
*              If input is <= 0, then the function returns 0. 
*
*              The computations are made by hardware MMDVSQ
*
*
****************************************************************************/    
static inline frac16_t GFLIB_SqrtHw_F16l_FC(frac32_t f32Val)
{
    uint16_t    u16InternalCount;
    frac16_t    f16Out;
    frac32_t    f32Out;
  
    if (f32Val <= 0)
    {
        f16Out = 0;
    }
    else
    {
        do
        {
            u16InternalCount = gu16CntMmdvsq + 1;
            gu16CntMmdvsq = u16InternalCount;
         
            RTCESL_MMDVSQ_CSR = 0x00000000;
            RTCESL_MMDVSQ_RCND = ((int32_t)(f32Val>>1)); 
            while (((RTCESL_MMDVSQ_CSR)>>31) == 1);
        
            f32Out = (RTCESL_MMDVSQ_RES); 
        }
        while (u16InternalCount != gu16CntMmdvsq);
        
        f16Out = (frac16_t)f32Out;
    }
    return (f16Out);
}
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRTHW_F32_H_ */

