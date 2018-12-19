/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
static inline frac32_t MLIB_MnacSat_F32lss_FCi(register frac32_t f32Accum, 
                                               register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register frac32_t f32Temp;
    
    f32Temp = ((int32_t)f16Mult1 * (int32_t)f16Mult2);
    f32Accum = f32Accum >> 1;
    f32Temp = f32Temp - f32Accum ;        
    f32Temp = (f32Temp > 1073741823) ? 1073741823 : f32Temp;
    f32Temp = (f32Temp < -1073741824) ? -1073741824 : f32Temp;
    return(f32Temp << 1);  
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MNAC_F32_H_ */
