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
* @brief  Upper limit functions with 32-bit fractional output
* 
*******************************************************************************/
#ifndef _GFLIB_UPPERLIMIT_F32_H_
#define _GFLIB_UPPERLIMIT_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_UpperLimit_F32_Ci(f32Val, f32ULim)                               \
        GFLIB_UpperLimit_F32_FCi(f32Val, f32ULim)
  
/***************************************************************************//*!
*
* @brief    Upper limit function
*   
* @param    in   frac32_t f32Val  - Argument in <-1;1) in frac32_t
*                frac32_t f32ULim - LowerLimit in <-1;1) in frac32_t
*                         
* @return   This function returns - frac32_t value <-1;1)
*       
* @remarks  This function trims the argument under or equal to upper f32ULim 
*           limit. 
*
****************************************************************************/ 
static inline frac32_t GFLIB_UpperLimit_F32_FCi(frac32_t f32Val, frac32_t f32ULim)
{
    if(f32Val > f32ULim)
    {
        f32Val = f32ULim;
    } 
    return(f32Val);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_UPPERLIMIT_F32_H_ */
