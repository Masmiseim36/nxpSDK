/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
* @brief  Multiply negate accumulate with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MNACRND_F32_H_
#define _MLIB_MNACRND_F32_H_

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
#define MLIB_MnacRnd_F32lls_Ci(f32Accum, f32Mult1, f16Mult2)                    \
        MLIB_MnacRnd_F32lls_FCi(f32Accum, f32Mult1, f16Mult2)                   

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f32Mult1 * f16Mult2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_MnacRnd_F32lls_FCi(register frac32_t f32Accum, 
                                               register frac32_t f32Mult1, register frac16_t f16Mult2)
{
	register uint64_t u64Temp;
	register frac64_t f64Temp;

	f64Temp = (frac64_t)f32Mult1*(frac16_t)f16Mult2 + 0x4000;
	u64Temp = (uint64_t)f64Temp >> 15U;    /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                       /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x8000000000U;}         /* subtract the value 2^(data type bits number - shifted value) */
    return (frac32_t)((frac32_t)u64Temp - (frac32_t)f32Accum);   /* cast back the corrected values */
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MNACRND_F32_H_ */
