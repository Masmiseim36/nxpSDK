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
* @brief  Multiply accumulate of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MAC4_F32_H_
#define _MLIB_MAC4_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Shift_F32.h"

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_Mac4_F32ssss_Ci(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)    \
        MLIB_Mac4_F32ssss_FCi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2) 

/***************************************************************************//*!
*
* f32Out = (f16Add1Mul1 * f16Add1Mul2) + (f16Add2Mul1 * f16Add2Mul2)
* Without saturation
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_Mac4_F32ssss_FCi(register frac16_t f16Add1Mul1, register frac16_t f16Add1Mul2, 
                                             register frac16_t f16Add2Mul1, register frac16_t f16Add2Mul2)
{
    return  (MLIB_Sh1L_F32_Ci(((int32_t)f16Add1Mul1 * (int32_t)f16Add1Mul2) + 
                              ((int32_t)f16Add2Mul1 * (int32_t)f16Add2Mul2)));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4_F32_H_ */
