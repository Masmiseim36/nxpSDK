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
#ifndef _MLIB_ABS_F16_H_
#define _MLIB_ABS_F16_H_

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
#define MLIB_Abs_F16_Ci(f16Val)    MLIB_Abs_F16_FCi(f16Val)
#define MLIB_AbsSat_F16_Ci(f16Val) MLIB_AbsSat_F16_FCi(f16Val)
  
/***************************************************************************//*!
*  Absolute value      
*  f16Out = |f16In|
*  not saturated
*******************************************************************************/
static inline frac16_t MLIB_Abs_F16_FCi(register frac16_t f16Val)
{
    return((f16Val < (frac16_t)0) ? (-f16Val) : (f16Val));
}
/***************************************************************************//*!
*  Absolute value saturated
*  f16Out = |f16In|
*******************************************************************************/
static inline frac16_t MLIB_AbsSat_F16_FCi(register frac16_t f16Val)
{
    register int16_t i16Temp;
  
    i16Temp = (int16_t)(MLIB_Abs_F16_Ci(f16Val)); 
    i16Temp = (i16Temp == INT16_MIN) ? INT16_MAX : i16Temp; /* Correction for -1.0 * (-1.0) */
    return((frac16_t)(i16Temp));
}
#if defined(__cplusplus)
}
#endif

#endif /*_MLIB_ABS_F16_H_*/
