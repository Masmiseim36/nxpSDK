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
* @brief  Limit functions with 16-bit fractional output
* 
*******************************************************************************/
#ifndef _GFLIB_LIMIT_F16_H_
#define _GFLIB_LIMIT_F16_H_

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
#define GFLIB_Limit_F16_Ci(f16Val, f16LLim, f16ULim)                         \
        GFLIB_Limit_F16_FCi(f16Val, f16LLim, f16ULim)

/***************************************************************************//*!
*
* @brief    Limit function
*   
* @param    in   frac16_t f16Val  - Argument in <-1;1) in frac16_t
*                frac16_t f16LLim - LowerLimit in <-1;1) in frac16_t
*                frac16_t f16ULim - UpperLimit in <-1;1) in frac16_t
*                         
* @return   This function returns - frac16_t value <-1;1)
*       
* @remarks  This function trims the argument according to the upper f16ULim and 
*           lower f16LLim limits. The upper limit must >= lower limit.
*
****************************************************************************/  
static inline frac16_t GFLIB_Limit_F16_FCi(frac16_t f16Val, 
                                           frac16_t f16LLim, frac16_t f16ULim)
{
    if(f16Val > f16ULim) 
    {
        return(f16ULim);
    }
    if(f16Val < f16LLim) 
    {
        return(f16LLim);
    }
    return(f16Val);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LIMIT_F16_H_ */
