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
* @brief  Subtraction
* 
*******************************************************************************/
#ifndef _MLIB_SUB_FLT_H_
#define _MLIB_SUB_FLT_H_

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
#define MLIB_Sub_FLT_Ci(fltMin, fltSub) MLIB_Sub_FLT_FCi(fltMin, fltSub)
  
/***************************************************************************//*!
*
*  fltOut = fltMin - fltSub
*  The output saturation is not implemented, thus in case 
*  the subtraction of input value is outside the (-2^128, 2^128) 
*  interval, the output value will overflow without any detection.
*******************************************************************************/ 
static inline float_t MLIB_Sub_FLT_FCi(register float_t fltMin, register float_t fltSub)
{
    return(fltMin - fltSub);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_FLT_H_ */
