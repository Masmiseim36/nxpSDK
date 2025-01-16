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
* @brief  Limit functions with 32-bit fractional output
* 
*******************************************************************************/
#ifndef _GFLIB_LIMIT_F32_H_
#define _GFLIB_LIMIT_F32_H_

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
#define GFLIB_Limit_F32_Ci(f32Val, f32LLim, f32ULim)                            \
        GFLIB_Limit_F32_FCi(f32Val, f32LLim, f32ULim)

/***************************************************************************//*!
*
* @brief    Limit function 32-bit version
* 
* @param    in   frac32_t f32Val  - Argument in <-1;1) in frac32_t
*                frac32_t f32LLim - Lower limit in <-1;1) in frac32_t
*                frac32_t f32ULim - Upper limit in <-1;1) in frac32_t
*                         
* @return   This function returns - frac32_t value <-1;1)
*       
* @remarks  This function trims the argument according to the upper f32ULim and 
*           lower f32LLim limits. The upper limit must >= lower limit.
*
****************************************************************************/  
static inline frac32_t GFLIB_Limit_F32_FCi(frac32_t f32Val, 
                                           frac32_t f32LLim, frac32_t f32ULim)
{
    if(f32Val > f32ULim) 
    {
        return(f32ULim);
    }
    if(f32Val < f32LLim) 
    {
        return(f32LLim);
    }
   
    return(f32Val);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LIMIT_F32_H_ */

