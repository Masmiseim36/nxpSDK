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
* @brief  Upper limit functions with 16-bit fractional output
* 
*******************************************************************************/
#ifndef _GFLIB_UPPERLIMIT_F16_H_
#define _GFLIB_UPPERLIMIT_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib.h"
  
/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_UpperLimit_F16_Ci(f16Val, f16ULim)                               \
        GFLIB_UpperLimit_F16_FCi(f16Val, f16ULim)

/***************************************************************************//*!
*
* @brief    Upper limit function
*   
* @param    in  frac16_t f16Val  - Argument in <-1;1) in frac16_t
*               frac16_t f16ULim - UpperLimit in <-1;1) in frac16_t
*                         
* @return   This function returns - frac16_t value <-1;1)
*       
* @remarks  This function trims the argument under or equal to upper f16ULim 
*           limit. 
*
****************************************************************************/     
static inline frac16_t GFLIB_UpperLimit_F16_FCi(frac16_t f16Val,frac16_t f16ULim)
{
    if(f16Val > f16ULim)
    {
        f16Val = f16ULim;
    } 
    return(f16Val);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_UPPERLIMIT_F16_H_ */

