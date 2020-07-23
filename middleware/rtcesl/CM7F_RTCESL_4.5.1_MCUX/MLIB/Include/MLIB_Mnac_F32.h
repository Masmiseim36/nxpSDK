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
* @brief  Multiply negate accumulate
* 
*******************************************************************************/
#ifndef _MLIB_MNAC_F32_H_
#define _MLIB_MNAC_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Sub_F32.h"

/*******************************************************************************
* Macros
*******************************************************************************/                                                                 
#define MLIB_MnacSat_F32lss_Ci(f32Accum, f16Mult1, f16Mult2)                    \
        MLIB_MnacSat_F32lss_FCi(f32Accum, f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f16Mult1 * f16Mult2) 
* With saturation
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac32_t MLIB_MnacSat_F32lss_FCi(register frac32_t f32Accum, 
                                               register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register frac32_t f32Temp;
    
    f32Temp = ((int32_t)f16Mult1 * (int32_t)f16Mult2);
    f32Accum = f32Accum >> 1;
    f32Temp = f32Temp - f32Accum ;        
    f32Temp = (f32Temp > 1073741823) ? 1073741823 : f32Temp;
    f32Temp = (f32Temp < -1073741824) ? -1073741824 : f32Temp;
    return(f32Temp << 1);  
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MNAC_F32_H_ */
