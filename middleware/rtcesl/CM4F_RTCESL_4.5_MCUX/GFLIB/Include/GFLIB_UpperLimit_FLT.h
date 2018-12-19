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
* @brief  Upper limit functions with float output
* 
*******************************************************************************/
#ifndef _GFLIB_UPPERLIMIT_FLT_H_
#define _GFLIB_UPPERLIMIT_FLT_H_

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
#define GFLIB_UpperLimit_FLT_Ci(fltVal, fltULim)                               \
        GFLIB_UpperLimit_FLT_FCi(fltVal, fltULim)
  
/***************************************************************************//*!
*
* @brief    Upper limit function
*   
* @param    in  float_t fltVal - Argument in float_t
*               float_t fltULim - UpperLimit in float_t
*                         
* @return   This function returns - float_t value 
*       
* @remarks  This function trims the argument under or equal to upper fltULim 
*           limit. 
*
****************************************************************************/  
static inline float_t GFLIB_UpperLimit_FLT_FCi(float_t fltVal, float_t fltULim)
{
    if(fltVal > fltULim)
    {
        fltVal = fltULim;
    } 
    return(fltVal);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_UPPERLIMIT_FLT_H_ */



