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
* @brief  Subtraction of three input values from the first input
* 
*******************************************************************************/
#ifndef _MLIB_SUB4_F16_H_
#define _MLIB_SUB4_F16_H_

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
#define MLIB_Sub4_F16_Ci(f16Min, f16Sub1, f16Sub2, f16Sub3)                    \
        MLIB_Sub4_F16_FCi(f16Min, f16Sub1, f16Sub2, f16Sub3)                   

/***************************************************************************//*!
*  Subtraction of three input values from the first input      
*  f16Out = f16Min + f16Sub1 + f16Sub2 + f16Sub3
*  function is not saturated
*******************************************************************************/
static inline frac16_t MLIB_Sub4_F16_FCi(register frac16_t f16Min, register frac16_t f16Sub1, 
                                         register frac16_t f16Sub2,register frac16_t f16Sub3)
{
    return((frac16_t)(f16Min - f16Sub1 - f16Sub2 - f16Sub3)); 
}
#if defined(__cplusplus)
}
#endif /* _MLIB_SUB4_F16_H_ */

#endif 
