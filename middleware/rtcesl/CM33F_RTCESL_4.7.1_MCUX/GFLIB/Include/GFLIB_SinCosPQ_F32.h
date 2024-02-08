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
* @brief  Sine, Cosine
* 
*******************************************************************************/
#ifndef _GFLIB_SINCOSPQ_A32_H_
#define _GFLIB_SINCOSPQ_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h" 
  
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SinPQ_F16_Ci(f16Angle) GFLIB_SinPQ_F16_FCi(f16Angle) 
#define GFLIB_CosPQ_F16_Ci(f16Angle) GFLIB_CosPQ_F16_FCi(f16Angle)
/****************************************************************************
* Inline functions 
****************************************************************************/ 

/***************************************************************************//*!
* @brief  Calculates the sine of the given argument using PowerQuad hardware accelerator.
*
* @param  in   frac16_t f16Angle - Argument in frac16_t range 
*
* @return This function returns - frac16_t value 
*       
* @remarks  
*   This function calculates sin(x) using PQ module:
*
*******************************************************************************/
static inline frac16_t GFLIB_SinPQ_F16_FCi(frac16_t f16Angle)
{
    int32_t ret;
    uint32_t cppre;
    rtcesl_pq_convert_t magic;
    rtcesl_pq_convert_t valFloat;

    magic.u32  = 0x30c90fdb;
    valFloat.flt = magic.flt * (float_t)MLIB_Conv_F32s(f16Angle);

    cppre = RTCESL_PQ_CPPRE;
    RTCESL_PQ_CPPRE = RTCESL_PQ_CPPRE_CPPRE_OUT(31);
    RTCESL_PQ_SIN0(valFloat.u32);

    ret = (frac32_t)RTCESL_PQ_READ_ADD0();
    ret = (frac32_t)RTCESL_PQ_READ_ADD0_FLT();
    ret = MLIB_Conv_F16l(ret);

    RTCESL_PQ_CPPRE = cppre;

    return (frac16_t)ret;
}

/***************************************************************************//*!
* @brief  Calculates the cosine of the given argument using PowerQuad hardware accelerator.
*
* @param  in   float_t frac16_t - Argument in frac16_t range 
*
* @return This function returns - frac16_t value 
*       
* @remarks  
*   This function calculates cos(x) using PQ module:
*
*******************************************************************************/ 
static inline frac16_t GFLIB_CosPQ_F16_FCi(register frac16_t f16Angle)
{
	frac32_t ret;
    uint32_t cppre;

    rtcesl_pq_convert_t magic;
    rtcesl_pq_convert_t valFloat;

    magic.u32  = 0x30c90fdb;
    valFloat.flt = magic.flt * (float_t)MLIB_Conv_F32s(f16Angle);

    cppre           = RTCESL_PQ_CPPRE;
    RTCESL_PQ_CPPRE = RTCESL_PQ_CPPRE_CPPRE_OUT(31);
    RTCESL_PQ_COS0(valFloat.u32);

    ret = (frac32_t)RTCESL_PQ_READ_ADD0();
    ret = (frac32_t)RTCESL_PQ_READ_ADD0_FLT();
    ret = MLIB_Conv_F16l(ret);

    RTCESL_PQ_CPPRE = cppre;

    return (frac16_t)ret;
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOSPQ_A32_H_ */

