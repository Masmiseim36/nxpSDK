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
* @brief  Multiply subtract of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MSU4_FLT_H_
#define _MLIB_MSU4_FLT_H_

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
#define MLIB_Msu4_FLT_Ci(fltMinMul1, fltMinMul2, fltSubMul1, fltSubMul2)       \
        MLIB_Msu4_FLT_FCi(fltMinMul1, fltMinMul2, fltSubMul1, fltSubMul2) 

/***************************************************************************//*!
*
* fltOut = (fltMinMul1 * fltMinMul2) - (fltSubMul1 * fltSubMul2)
* The output saturation is not implemented, thus in case 
* the MSU4 of input values is outside the (-2^128, 2^128) 
* interval, the output value will overflow without any detection. 
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline float_t MLIB_Msu4_FLT_FCi(register float_t fltMinMul1, register float_t fltMinMul2, 
                                        register float_t fltSubMul1, register float_t fltSubMul2)
{
    return (float_t)(((float_t)fltMinMul1 * (float_t)fltMinMul2) -
                     ((float_t)fltSubMul1 * (float_t)fltSubMul2));  
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MSU4_FLT_H_ */
