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
static inline frac32_t MLIB_Mac4_F32ssss_FCi(register frac16_t f16Add1Mul1, register frac16_t f16Add1Mul2, 
                                             register frac16_t f16Add2Mul1, register frac16_t f16Add2Mul2)
{
    return (((frac32_t)(((int32_t)(f16Add1Mul1)*(int32_t)(f16Add1Mul2))<<1))+
            ((frac32_t)(((int32_t)(f16Add2Mul1)*(int32_t)(f16Add2Mul2))<<1))); 
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4_F32_H_ */
