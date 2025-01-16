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
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_F16_H_
#define _MLIB_MULRND_F16_H_

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
#define MLIB_MulRnd_F16_Ci(f16Mult1, f16Mult2)    MLIB_MulRnd_F16_FCi(f16Mult1, f16Mult2)
#define MLIB_MulNegRnd_F16_Ci(f16Mult1, f16Mult2) MLIB_MulNegRnd_F16_FCi(f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f16Out = f16Mult1 * f16Mult2
* Without saturation
*******************************************************************************/
static inline frac16_t MLIB_MulRnd_F16_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return(frac16_t)MLIB_ShR_F32_Ci((int32_t)f16Mult1 * (int32_t)f16Mult2 + 0x4000, 15U);
}
 
/***************************************************************************//*!
*
* f16Out = f16Mult1 * f16Mult2
* 
*******************************************************************************/
static inline frac16_t MLIB_MulNegRnd_F16_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return(frac16_t)MLIB_ShR_F32_Ci((int32_t)f16Mult1 * (int32_t)(-f16Mult2) + 0x4000, 15U);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_F16_H_ */
