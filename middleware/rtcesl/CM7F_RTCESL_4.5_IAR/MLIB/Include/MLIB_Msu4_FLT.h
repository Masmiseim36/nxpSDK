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
* @brief  Multiply subtract of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MSU4_FLT_H_
#define _MLIB_MSU4_FLT_H_

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
#define MLIB_Msu4_FLT_Ci(fltMinMul1, fltMinMul2, fltSubMul1, fltSubMul2)       \
        MLIB_Msu4_FLT_FCi(fltMinMul1, fltMinMul2, fltSubMul1, fltSubMul2) 

/***************************************************************************//*!
*
* fltOut = (fltMinMul1 * fltMinMul2) - (fltSubMul1 * fltSubMul2)
* The output saturation is not implemented, thus in case 
* the MSU4 of input values is outside the (-2^128, 2^128) 
* interval, the output value will overflow without any detection. 
*******************************************************************************/
static inline float_t MLIB_Msu4_FLT_FCi(register float_t fltMinMul1, register float_t fltMinMul2, 
                                        register float_t fltSubMul1, register float_t fltSubMul2)
{
    return (float_t)(((float_t)fltMinMul1 * (float_t)fltMinMul2) -
                     ((float_t)fltSubMul1 * (float_t)fltSubMul2));  
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MSU4_FLT_H_ */
