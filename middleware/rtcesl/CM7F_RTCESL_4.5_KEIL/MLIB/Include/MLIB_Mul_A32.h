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
#define MLIB_Mul_A32_Ci(a32Mult1, a32Mult2)     MLIB_Mul_A32_FCi( a32Mult1, a32Mult2)
#define MLIB_MulNeg_A32_Ci(a32Mult1, a32Mult2)  MLIB_MulNeg_A32_FCi( a32Mult1, a32Mult2)

/***************************************************************************//*!
*
* f16Out = a32Accum * f16Mult
* Without saturation
*******************************************************************************/  
static inline frac16_t MLIB_Mul_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    return(frac16_t)(((frac64_t)a32Accum*f16Mult)>>15);
}
 
/***************************************************************************//*!
*
* f16Out = a32Accum * -f16Mult

*******************************************************************************/
static inline frac16_t MLIB_MulNeg_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    return(frac16_t)(((frac64_t)a32Accum*(-f16Mult))>>15);
}
 
/***************************************************************************//*!
*
* a32Out = a32Mult1 * a32Mult2

*******************************************************************************/
static inline acc32_t MLIB_Mul_A32_FCi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t f32Temp;
    
    f32Temp = ((acc64_t)a32Mult1*a32Mult2)>> 15;
    return f32Temp;
}
 
/***************************************************************************//*!
*
* a32Out = - a32Mult1 * a32Mult2

*******************************************************************************/
static inline acc32_t MLIB_MulNeg_A32_FCi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t f32Temp;
    
    f32Temp = ((acc64_t)a32Mult1*a32Mult2)>> 15;
    return (-f32Temp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_A32_H_ */
