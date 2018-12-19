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
* @brief  Lower limit functions with float output
* 
*******************************************************************************/
#ifndef _GFLIB_LOWERLIMIT_FLT_H_
#define _GFLIB_LOWERLIMIT_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_LowerLimit_FLT_Ci(fltVal, fltLLim)                               \
        GFLIB_LowerLimit_FLT_FCi(fltVal, fltLLim)
  
/***************************************************************************//*!
*
* @brief    Lower limit function
*   
* @param    in  float_t fltVal  - Argument in float_t
*               float_t fltLLim - LowerLimit in float_t
*                         
* @return   This function returns - float_t value 
*       
* @remarks  This function trims the argument above or equal to lower fltLLim 
*           limit. 
*
****************************************************************************/ 
static inline float_t GFLIB_LowerLimit_FLT_FCi(float_t fltVal, float_t fltLLim)
{
    if(fltVal < fltLLim)
    {
        fltVal = fltLLim;
    } 
    return(fltVal);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LOWERLIMIT_FLT_H_ */


