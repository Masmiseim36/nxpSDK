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
* @brief  Multiply subtract with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MSURND_A32_H_
#define _MLIB_MSURND_A32_H_

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
#define MLIB_MsuRnd_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)                   \
        MLIB_MsuRnd_A32ass_FCi(a32Accum, f16Mult1, f16Mult2)
    
/***************************************************************************//*!
*
* a32Out = a32Accum - ( f16Mult1 * f16Mult2)
*
*******************************************************************************/
static inline acc32_t MLIB_MsuRnd_A32ass_FCi(register acc32_t a32Accum, 
                                             register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register frac64_t f64Temp;
    register uint64_t u64Temp;

    u64Temp = (uint64_t)a32Accum << 15U;
    if (a32Accum < 0)                    /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x0800000000000U;}    /* subtract the value 2^(data type bits number - shifted value) */
    f64Temp = (frac64_t)u64Temp;         /* cast back the corrected values */
    f64Temp += (frac64_t)((int64_t)f16Mult1 * (int64_t)(-f16Mult2)) + 0x00004000;
    u64Temp = (uint64_t)f64Temp >> 15U;  /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                     /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x2000000000000U;}    /* subtract the value 2^(data type bits number - shifted value) */
    return (acc32_t)u64Temp;             /* cast back the corrected values */
}

 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSURND_A32_H_ */
