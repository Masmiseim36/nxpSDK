/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
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
    volatile float_t pfltTemp = 0.0F;
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
    volatile float_t pfltTemp = 0.0F;
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

