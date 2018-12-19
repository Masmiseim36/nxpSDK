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
#ifndef _MLIB_MUL_FLT_H_
#define _MLIB_MUL_FLT_H_

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
#define MLIB_Mul_FLT_Ci(fltMult1, fltMult2)    MLIB_Mul_FLT_FCi(fltMult1, fltMult2)
#define MLIB_MulNeg_FLT_Ci(fltMult1, fltMult2) MLIB_MulNeg_FLT_FCi(fltMult1, fltMult2)
  
/***************************************************************************//*!
*
* fltOut = fltMult1 * fltMult2
* The output saturation is not implemented, thus in case 
* the multiplication of input values is outside the (-2^128, 2^128) 
* interval, the output value will overflow without any detection.
*******************************************************************************/   
static inline float_t MLIB_Mul_FLT_FCi(register float_t fltMult1, register float_t fltMult2)
{
    return((float_t)(fltMult1 * fltMult2));   
}
 
/***************************************************************************//*!
*
* fltOut = fltMult1 * -fltMult2
* The output saturation is not implemented, thus in case 
* the multiplication of input values is outside the (-2^128, 2^128) 
* interval, the output value will overflow without any detection.
* 
*******************************************************************************/  
static inline float_t MLIB_MulNeg_FLT_FCi(register float_t fltMult1, register float_t fltMult2)
{
    return (float_t)(-(fltMult1 * fltMult2));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_FLT_H_ */
