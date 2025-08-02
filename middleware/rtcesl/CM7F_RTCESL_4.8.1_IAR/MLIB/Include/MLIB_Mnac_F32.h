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
* @brief  Multiply negate accumulate
* 
*******************************************************************************/
#ifndef _MLIB_MNAC_F32_H_
#define _MLIB_MNAC_F32_H_

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
#define MLIB_MnacSat_F32lss_Ci(f32Accum, f16Mult1, f16Mult2)                    \
        MLIB_MnacSat_F32lss_FCi(f32Accum, f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f16Mult1 * f16Mult2) 
* With saturation
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_MnacSat_F32lss_FCi(register frac32_t f32Accum, 
                                               register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register frac32_t f32Temp;
    
    f32Temp = ((int32_t)f16Mult1 * (int32_t)f16Mult2);
    f32Accum = MLIB_Sh1R_F32_Ci(f32Accum);
    f32Temp = f32Temp - f32Accum;
    f32Temp = (f32Temp > 1073741823)  ?  1073741823 : f32Temp;
    f32Temp = (f32Temp < -1073741824) ? -1073741824 : f32Temp;
    return MLIB_Sh1L_F32_Ci(f32Temp);
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MNAC_F32_H_ */
