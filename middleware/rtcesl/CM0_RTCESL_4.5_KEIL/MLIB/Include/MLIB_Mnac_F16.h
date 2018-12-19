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
* @brief  Multiply negate accumulate
* 
*******************************************************************************/
#ifndef _MLIB_MNAC_F16_H_
#define _MLIB_MNAC_F16_H_

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
#define MLIB_Mnac_F16_Ci( f16Accum, f16Mult1, f16Mult2)                        \
        MLIB_Mnac_F16_FCi( f16Accum, f16Mult1, f16Mult2)                               
  
/***************************************************************************//*!
*
* f16Out = - f16Accum + ( f16Mult1 * f16Mult2)
* Without saturation
*******************************************************************************/
static inline frac16_t MLIB_Mnac_F16_FCi(register frac16_t f16Accum, 
                                         register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register acc32_t a32Temp;
    
    a32Temp = ((int32_t)f16Mult1 * (int32_t)f16Mult2);
    a32Temp = a32Temp >> 15;
    a32Temp =  a32Temp - (acc32_t)f16Accum;
    return((frac16_t)a32Temp);  
}
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MNAC_F16_H_ */
