/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
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
 */

/*******************************************************************************
*
* @brief  Multiply subtract of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MSU4_F32_H_
#define _MLIB_MSU4_F32_H_

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
#define MLIB_Msu4_F32ssss_Ci(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)    \
        MLIB_Msu4_F32ssss_FCi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) 
#define MLIB_Msu4Sat_F32ssss_Ci(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) \
        MLIB_Msu4Sat_F32ssss_FCi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) 

/***************************************************************************//*!
*
* f32Out = (f16MinMul1 * f16MinMul2) - (f16SubMul1 * f16SubMul2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_Msu4_F32ssss_FCi(register frac16_t f16MinMul1, register frac16_t f16MinMul2,
                                             register frac16_t f16SubMul1, register frac16_t f16SubMul2)
{
    return (((frac32_t)(((int32_t)(f16MinMul1)*(int32_t)(f16MinMul2))<<1)) - 
            ((frac32_t)(((int32_t)(f16SubMul1)*(int32_t)(f16SubMul2))<<1))); 
}
/***************************************************************************//*!
*
* f32Out = (f16MinMul1 * f16MinMul2) - (f16SubMul1 * f16SubMul2)
* With saturation
*******************************************************************************/
static inline frac32_t MLIB_Msu4Sat_F32ssss_FCi(register frac16_t f16MinMul1, register frac16_t f16MinMul2,
                                                register frac16_t f16SubMul1, register frac16_t f16SubMul2)
{
    int32_t i32Temp;
    
    i32Temp = ((frac32_t)((int32_t)(f16MinMul1)*(int32_t)(f16MinMul2))) - 
              ((frac32_t)((int32_t)(f16SubMul1)*(int32_t)(f16SubMul2)));
    if (i32Temp >= 1073741824)
    {
        i32Temp = 2147483647;
    }
    else
    {
        if (i32Temp <= -1073741824)
        {
            i32Temp = INT32_MIN;
        }
        else
        {
            i32Temp <<= 1;
        }
    }
    return (i32Temp); 
}
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MSU4_F32_H_ */
