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
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_F16_H_
#define _MLIB_MULRND_F16_H_

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
#define MLIB_MulRnd_F16_Ci(f16Mult1, f16Mult2)    MLIB_MulRnd_F16_FCi(f16Mult1, f16Mult2)
#define MLIB_MulNegRnd_F16_Ci(f16Mult1, f16Mult2) MLIB_MulNegRnd_F16_FCi(f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f16Out = f16Mult1 * f16Mult2
* Without saturation
*******************************************************************************/
static inline frac16_t MLIB_MulRnd_F16_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    frac32_t f32Temp;
    
    f32Temp = ((frac32_t)(((int32_t)(f16Mult1)*(int32_t)(f16Mult2)))+0x4000);
    return((frac16_t)(f32Temp>>15));
}
 
/***************************************************************************//*!
*
* f16Out = f16Mult1 * f16Mult2
* 
*******************************************************************************/
static inline frac16_t MLIB_MulNegRnd_F16_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return(frac16_t)((((int32_t)(f16Mult1)*(int32_t)((-f16Mult2)))+0x4000)>>15); 
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_F16_H_ */
