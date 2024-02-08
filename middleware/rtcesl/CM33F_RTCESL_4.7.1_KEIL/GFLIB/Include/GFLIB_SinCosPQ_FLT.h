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
#ifndef _GFLIB_SINCOSPQ_FLT_H_
#define _GFLIB_SINCOSPQ_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h" 
  
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SinPQ_FLT_Ci(fltAngle)  GFLIB_SinPQ_FLT_FCi(fltAngle)
#define GFLIB_SinPQ_FLTa_Ci(fltAngle) GFLIB_SinPQ_FLTa_FCi(fltAngle) 
#define GFLIB_CosPQ_FLT_Ci(fltAngle)  GFLIB_CosPQ_FLT_FCi(fltAngle)
#define GFLIB_CosPQ_FLTa_Ci(fltAngle) GFLIB_CosPQ_FLTa_FCi(fltAngle)

/****************************************************************************
* Inline functions 
****************************************************************************/

/***************************************************************************//*!
* @brief  Calculates the sine of the given argument using PowerQuad hardware accelerator.
*
* @param  in   float_t fltAngle - Argument in float_t range 
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates sin(x) using PQ module:
*
*******************************************************************************/
static inline float_t GFLIB_SinPQ_FLT_FCi(float_t fltAngle)
{
    uint32_t u32Temp;
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    RTCESL_PQ_SIN0(*(uint32_t *)(void *)&fltAngle);
    u32Temp = RTCESL_PQ_READ_ADD0();
    return *(float_t*)(void *)&u32Temp;
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
}

/***************************************************************************//*!
* @brief  Calculates the cosine of the given argument using PowerQuad hardware accelerator.
*
* @param  in   float_t fltAngle - Argument in float_t range 
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates cos(x) using PQ module:
*
**************************************************************/                                                         
static inline float_t GFLIB_CosPQ_FLT_FCi(float_t fltAngle)
{
    uint32_t u32Temp;

    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    RTCESL_PQ_COS0(*(uint32_t *)(void *)&fltAngle);
    u32Temp = RTCESL_PQ_READ_ADD0();
    return *(float_t*)(void *)&u32Temp;
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
} 

/***************************************************************************//*!
* @brief  Calculates the sine of the given argument using PowerQuad hardware accelerator.
*
* @param  in   acc32 a32AngleExt - Argument in range in acc32_t, fractional part 
*                                  of which corresponds to [-pi; pi 
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates sin(x) using PQ module:
*   x = fractional part of a32AngleExt * pi / 2^15
*
*******************************************************************************/
static inline float_t GFLIB_SinPQ_FLTa_FCi(acc32_t a32AngleExt)
{
    float_t pfltTemp = 0.0F;
    uint32_t u32Temp;
    
    pfltTemp = (float_t)((float_t)(frac16_t)a32AngleExt *
              ((float_t)FLOAT_PI/(float_t)32768.0F)); /* Converts fractional to float */
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    RTCESL_PQ_SIN0(*(uint32_t *)(void *)&pfltTemp);
    u32Temp = RTCESL_PQ_READ_ADD0();
    return *(float_t*)(void *)&u32Temp;
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif	
}

/***************************************************************************//*!
* @brief  Calculates the cosine of the given argument using PowerQuad hardware accelerator.
*
* @param  in   acc32 a32AngleExt - Argument in range in acc32_t, fractional part 
*                                  of which corresponds to [-pi; pi
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates cos(x) using PQ module:
*   x = fractional part of a32AngleExt * pi / 2^15
*
**************************************************************/                                                         
static inline float_t GFLIB_CosPQ_FLTa_FCi(acc32_t a32AngleExt)
{
    float_t pfltTemp = 0.0F;
    uint32_t u32Temp;

    pfltTemp = (float_t)((float_t)(frac16_t)a32AngleExt *
              ((float_t)FLOAT_PI/(float_t)32768.0F)); /* Converts fractional to float */
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    RTCESL_PQ_COS0(*(uint32_t *)(void *)&pfltTemp);
    u32Temp = RTCESL_PQ_READ_ADD0();
    return *(float_t*)(void *)&u32Temp;
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif	
} 

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOSPQ_FLT_H_ */

