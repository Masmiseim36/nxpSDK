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
#ifndef _MLIB_MULRND_A32_H_
#define _MLIB_MULRND_A32_H_

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
#define MLIB_MulRnd_F16as_Ci(a32Accum, f16Mult)    MLIB_MulRnd_F16as_FCi(a32Accum, f16Mult)
#define MLIB_MulNegRnd_F16as_Ci(a32Accum, f16Mult) MLIB_MulNegRnd_F16as_FCi(a32Accum, f16Mult)
#define MLIB_MulRnd_A32_Ci(a32Mult1, a32Mult2)     MLIB_MulRnd_A32_FCi(a32Mult1, a32Mult2)
#define MLIB_MulNegRnd_A32_Ci(a32Mult1, a32Mult2)  MLIB_MulNegRnd_A32_FCi(a32Mult1, a32Mult2)

/***************************************************************************//*!
*
* f16Out = a32Accum * f16Mult
* Without saturation
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t MLIB_MulRnd_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{                                                                              
    return(frac16_t)((((frac64_t)a32Accum*f16Mult)+0x4000)>>15);
}
 
/***************************************************************************//*!
*
* f16Out = a32Accum * -f16Mult
* 
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t MLIB_MulNegRnd_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    return (frac16_t)((((frac64_t)a32Accum*(-f16Mult))+0x4000)>>15);
}
 
/***************************************************************************//*!
*
* a32Out = a32Mult1 * a32Mult2
* Without saturation
*******************************************************************************/
RAM_FUNC_LIB 
static inline acc32_t MLIB_MulRnd_A32_FCi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Temp;

    a32Temp = ((acc64_t)a32Mult1*a32Mult2 + (acc64_t)0x4000) >> 15;
    return (acc32_t)a32Temp;
}
 
/***************************************************************************//*!
*
* a32Out = - a32Mult1 * a32Mult2
* Without saturation
*******************************************************************************/
RAM_FUNC_LIB 
static inline acc32_t MLIB_MulNegRnd_A32_FCi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Temp;

    a32Temp = ((acc64_t)a32Mult1*a32Mult2 + (acc64_t)0x4000) >> 15;
    return (acc32_t)(-a32Temp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_A32_H_ */
