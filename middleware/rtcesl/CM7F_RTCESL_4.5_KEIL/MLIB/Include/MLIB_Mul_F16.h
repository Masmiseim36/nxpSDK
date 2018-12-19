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
* @brief  Multiply 
* 
*******************************************************************************/
#ifndef _MLIB_MUL_F16_H_
#define _MLIB_MUL_F16_H_

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
#define MLIB_Mul_F16_Ci(f16Mult1, f16Mult2)    MLIB_Mul_F16_FCi(f16Mult1, f16Mult2)
#define MLIB_MulNeg_F16_Ci(f16Mult1, f16Mult2) MLIB_MulNeg_F16_FCi(f16Mult1, f16Mult2)
  
/***************************************************************************//*!
*
* f16Out = f16Mult1 * f16Mult2
* Without saturation
*******************************************************************************/ 
static inline frac16_t MLIB_Mul_F16_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return((frac16_t)(((int32_t)(f16Mult1)*(int32_t)(f16Mult2))>>15));
}
 
/***************************************************************************//*!
*
* f16Out = f16Mult1 * f16Mult2
* 
*******************************************************************************/
static inline frac16_t MLIB_MulNeg_F16_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return((frac16_t)(((int32_t)(f16Mult1)*(int32_t)(-f16Mult2))>>15));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_F16_H_ */
