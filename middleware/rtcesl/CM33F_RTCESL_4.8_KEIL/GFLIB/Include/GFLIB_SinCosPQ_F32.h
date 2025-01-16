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

