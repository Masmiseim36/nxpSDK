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
#ifndef _MLIB_MUL_A32_H_
#define _MLIB_MUL_A32_H_

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
#define MLIB_Mul_F16as_Ci(a32Accum, f16Mult)    MLIB_Mul_F16as_FCi(a32Accum, f16Mult)
#define MLIB_MulNeg_F16as_Ci(a32Accum, f16Mult) MLIB_MulNeg_F16as_FCi(a32Accum, f16Mult)

/***************************************************************************//*!
*
* f16Out = a32Accum * f16Mult
* Without saturation
*******************************************************************************/  
static inline frac16_t MLIB_Mul_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    return((frac16_t)(((((int32_t)(a32Accum)&0xffff)*(int32_t)(f16Mult))>>15)+
                      ((((int32_t)(a32Accum)>>16)*(int32_t)(f16Mult))<<1)));
  
}
/***************************************************************************//*!
*
* f16Out = a32Accum * -f16Mult

*******************************************************************************/
static inline frac16_t MLIB_MulNeg_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    return ((frac16_t)(((((int32_t)(a32Accum)&0xffff)*(int32_t)(-f16Mult))>>15)+
                       ((((int32_t)(a32Accum)>>16)*(int32_t)(-f16Mult))<<1)));
}
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_A32_H_ */
