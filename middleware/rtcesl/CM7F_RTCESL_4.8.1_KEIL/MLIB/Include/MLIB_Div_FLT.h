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
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIV_FLT_H_
#define _MLIB_DIV_FLT_H_

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
#define MLIB_Div_FLT_Ci(fltNum, fltDenom) MLIB_Div_FLT_FCi(fltNum, fltDenom) 
  
/****************************************************************************
* Inline functions 
****************************************************************************/

/***************************************************************************//*!
* @brief  float numerator, float denominator inputs 16-output 1-quadrant
*         division function
*
* @param  in  float_t fltNum   - Numerator in float_t
*         in  float_t fltDenom - Denominator in  float_t
*
* @return This function returns - float_t value 
*      
* @remarks  This function divides two fractional:
*           result = fltNum / fltDenom. 
*           If denumerator is +zero(-zero) it returns plus infinity = 0x7F800000 
*           (or minus infinity = 0xFF800000). Both are not numbers float value.   
*           The output saturation is not implemented, thus in case 
*           the division of input values is outside the (-2^128, 2^128) 
*           interval, the output value will overflow without any detection. 
*
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline float_t MLIB_Div_FLT_FCi(register float_t fltNum, register float_t fltDenom)
{   
    return((float_t)(fltNum / fltDenom));
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIV_FLT_H_ */
