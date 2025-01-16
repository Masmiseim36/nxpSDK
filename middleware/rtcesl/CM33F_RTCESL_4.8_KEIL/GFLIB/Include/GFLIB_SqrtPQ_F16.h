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
* @brief  Square root 
* 
*******************************************************************************/

#ifndef _GFLIB_SQRTPQ_F16_H_
#define _GFLIB_SQRTPQ_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SqrtPQ_F16_Ci(f16Val) GFLIB_SqrtPQ_F16_FCi(f16Val)
  
/****************************************************************************
* Inline functions 
****************************************************************************/

/***************************************************************************//*!
*
* @brief  Calculates the square root of the argument.
*
* @param    ptr  GFLIB_SQRT_TABLE_T_F32 *psParam - Pointer to the polynomial table 
* @param    in   frac32_t f32Val - Argument in <0;1) in frac32_t
*
* @return This function returns - frac16_t value <0;1)
*		
* @remarks 	This function calculates square root. The argument must be in the
*		range [0;1]. The algorithm calculates square root using PQ module.
*		If the value is negative the function returns zero value.
*
****************************************************************************/
static inline frac16_t GFLIB_SqrtPQ_F16_FCi(frac16_t f16Val)
{
    frac32_t f32Temp;
    
    if(f16Val > 0)
    {   
        f32Temp = MLIB_ShL_F32_Ci((frac32_t)f16Val, 15U);
        RTCESL_PQ_SQRT_FLT0((uint32_t)f32Temp);
        f32Temp = (frac32_t)RTCESL_PQ_READ_MULT0_FLT();
        return (frac16_t)f32Temp;
    }
    else
    {
        f32Temp = 0;
    }   
    return (frac16_t)f32Temp;  
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRTPQ_F16_H_ */
