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
* @brief  Multiply accumulate
* 
*******************************************************************************/
#ifndef _MLIB_MAC_FLT_H_
#define _MLIB_MAC_FLT_H_

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
#define MLIB_Mac_FLT_Ci(fltAccum, fltMult1, fltMult2)                          \
        MLIB_Mac_FLT_FCi(fltAccum, fltMult1, fltMult2)

/***************************************************************************//*!
*
* fltOut = fltAccum + ( fltMult1 * fltMult2)
* The output saturation is not implemented, thus in case 
* the MAC of input values is outside the (-2^128, 2^128) 
* interval, the output value will overflow without any detection. 
*******************************************************************************/
static inline float_t MLIB_Mac_FLT_FCi(register float_t fltAccum, 
                                       register float_t fltMult1, register float_t fltMult2)
{
    return((float_t) (fltAccum + (fltMult1 * fltMult2)));  
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MAC_FLT_H_ */
