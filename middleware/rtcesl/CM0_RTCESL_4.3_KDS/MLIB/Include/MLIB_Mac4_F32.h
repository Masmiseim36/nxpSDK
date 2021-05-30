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
* @brief  Multiply accumulate of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MAC4_F32_H_
#define _MLIB_MAC4_F32_H_

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
#define MLIB_Mac4_F32ssss_Ci(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)    \
        MLIB_Mac4_F32ssss_FCi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2) 
#define MLIB_Mac4Sat_F32ssss_Ci(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2) \
        MLIB_Mac4Sat_F32ssss_FCi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2) 

/***************************************************************************//*!
*
* f32Out = (f16Add1Mul1 * f16Add1Mul2) + (f16Add2Mul1 * f16Add2Mul2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_Mac4_F32ssss_FCi(register frac16_t f16Add1Mul1, register frac16_t f16Add1Mul2, 
                                             register frac16_t f16Add2Mul1, register frac16_t f16Add2Mul2)
{
    return (((frac32_t)(((int32_t)(f16Add1Mul1)*(int32_t)(f16Add1Mul2))<<1))+
            ((frac32_t)(((int32_t)(f16Add2Mul1)*(int32_t)(f16Add2Mul2))<<1))); 
}
/***************************************************************************//*!
*
* f32Out = (f16Add1Mul1 * f16Add1Mul2) + (f16Add2Mul1 * f16Add2Mul2)
* With saturation
*******************************************************************************/
static inline frac32_t MLIB_Mac4Sat_F32ssss_FCi(register frac16_t f16Add1Mul1, register frac16_t f16Add1Mul2, 
                                                register frac16_t f16Add2Mul1, register frac16_t f16Add2Mul2)
{
    int32_t i32Temp;
    
    i32Temp = (int32_t)((int32_t)(f16Add1Mul1)*(int32_t)(f16Add1Mul2) + 
                        (int32_t)(f16Add2Mul1)*(int32_t)(f16Add2Mul2));
    i32Temp = (int32_t)(((i32Temp == INT32_MIN)|(i32Temp >= 1073741824L)) ? INT32_MAX :
                       ((i32Temp <= -1073741824L) ? INT32_MIN : (i32Temp << 1)));

    return ((frac32_t)i32Temp);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4_F32_H_ */
