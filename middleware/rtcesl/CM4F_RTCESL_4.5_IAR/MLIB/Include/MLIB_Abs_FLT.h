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
* @brief  Absolute Value
* 
*******************************************************************************/
#ifndef _MLIB_ABS_FLT_H_
#define _MLIB_ABS_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Neg_FLT.h"    

/*******************************************************************************
* Macros 
*******************************************************************************/
#define MLIB_Abs_FLT_Ci(fltVal) MLIB_Abs_FLT_FCi(fltVal)
  
/***************************************************************************//*!
*  Absolute value      
*  fltOut = |fltIn|
*  The output saturation is not implemented.
*******************************************************************************/
static inline float_t MLIB_Abs_FLT_FCi(register float_t fltVal)
{
    return((fltVal < (float_t)0) ? (MLIB_Neg_FLT_FCi(fltVal)) : (fltVal));
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ABS_FLT_H_*/
