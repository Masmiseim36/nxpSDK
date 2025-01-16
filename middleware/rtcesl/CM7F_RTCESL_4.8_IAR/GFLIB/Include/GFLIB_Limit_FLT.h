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
* @brief  Limit functions with float output
* 
*******************************************************************************/
#ifndef _GFLIB_LIMIT_FLT_H_
#define _GFLIB_LIMIT_FLT_H_

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
#define GFLIB_Limit_FLT_Ci(fltVal, fltLLim, fltULim)                           \
        GFLIB_Limit_FLT_FCi(fltVal, fltLLim, fltULim)

/***************************************************************************//*!
*
* @brief    Limit function float version
* 
* @param    in   float_t fltVal  - Argument in float_t
*                float_t fltLLim - Lower limit in float_t
*                float_t fltULim - Upper limit in float_t
*                         
* @return   This function returns - float_t value 
*       
* @remarks  This function trims the argument according to the upper fltULim and 
*           lower fltLLim limits. The upper limit must >= lower limit.
*
****************************************************************************/  
RAM_FUNC_LIB 
RTCESL_INLINE static inline float_t GFLIB_Limit_FLT_FCi(float_t fltVal, 
                                          float_t fltLLim, float_t fltULim)
{
    if(fltVal > fltULim) 
    {
        return(fltULim);
    }
    if(fltVal < fltLLim) 
    {
        return(fltLLim);
    }
   
    return(fltVal);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LIMIT_FLT_H_ */

