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
RAM_FUNC_LIB 
RTCESL_INLINE static inline float_t GFLIB_UpperLimit_FLT_FCi(float_t fltVal, float_t fltULim)
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



