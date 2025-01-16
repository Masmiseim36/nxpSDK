/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
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

