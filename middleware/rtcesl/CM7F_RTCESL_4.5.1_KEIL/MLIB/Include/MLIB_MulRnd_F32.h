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
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_F32_H_
#define _MLIB_MULRND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Neg_F32.h"  

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_MulRnd_F32_Ci(f32Mult1, f32Mult2)      MLIB_MulRnd_F32_FCi(f32Mult1, f32Mult2)
#define MLIB_MulRnd_F32ls_Ci(f32Mult1, f16Mult2)    MLIB_MulRnd_F32ls_FCi(f32Mult1, f16Mult2)
#define MLIB_MulNegRnd_F32_Ci(f32Mult1, f32Mult2)   MLIB_MulNegRnd_F32_FCi(f32Mult1, f32Mult2)
#define MLIB_MulNegRnd_F32ls_Ci(f32Mult1, f16Mult2) MLIB_MulNegRnd_F32ls_FCi(f32Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* Without saturation
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac32_t MLIB_MulRnd_F32_FCi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    register frac32_t f32Temp;

    f32Temp = ((frac64_t)f32Mult1*f32Mult2 + (frac64_t)0x40000000) >> 31;    
    return (frac32_t)f32Temp;
}
 
/***************************************************************************//*!
*
* f32Out = f32Mult1 * f16Mult2
* Without saturation
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac32_t MLIB_MulRnd_F32ls_FCi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    register frac32_t f32Temp;
    
    f32Temp = ((frac64_t)f32Mult1*(frac16_t)f16Mult2 + 0x4000)>>15;
    return(frac32_t)f32Temp;
}
 
/***************************************************************************//*!
*
* f32Out = f32Mult1 * -f32Mult2
* 
*******************************************************************************/ 
RAM_FUNC_LIB 
static inline frac32_t MLIB_MulNegRnd_F32_FCi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    int64_t i64Mult1; 
    int64_t i64Mult2; 
    
    i64Mult1 = (int64_t)f32Mult1;
    i64Mult2 = (int64_t)f32Mult2;
    return((frac32_t) (((i64Mult1*(-i64Mult2))+0x40000000)>>31));
}
 
/***************************************************************************//*!
*
* f32Out = f32Mult1 * -f16Mult2
* 
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac32_t MLIB_MulNegRnd_F32ls_FCi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    frac32_t f32Temp1;
    
    f32Temp1 = MLIB_Neg_F32_Ci((int32_t)(f16Mult2));
    return(((frac64_t)f32Mult1*(frac32_t)f32Temp1 + 0x4000)>>15);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_F32_H_ */
