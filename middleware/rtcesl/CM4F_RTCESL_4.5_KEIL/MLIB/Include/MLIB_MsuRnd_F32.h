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
* @brief  Multiply subtract with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MSURND_F32_H_
#define _MLIB_MSURND_F32_H_

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
#define MLIB_MsuRnd_F32_Ci(f32Accum, f32Mult1, f32Mult2)                      \
        MLIB_MsuRnd_F32_FCi(f32Accum, f32Mult1, f32Mult2)                      

/***************************************************************************//*!
*
* f32Out = f32Accum - ( f32Mult1 * f32Mult2)
* Without saturation
*******************************************************************************/  
static inline frac32_t MLIB_MsuRnd_F32_FCi(register frac32_t f32Accum,
                                           register frac32_t f32Mult1,register frac32_t f32Mult2)
{
    register int64_t i64Temp;
    
    i64Temp = (((int64_t)f32Mult1 * (int64_t)f32Mult2)+ 0x40000000);
    i64Temp = i64Temp >> 31;
    i64Temp = (int64_t)f32Accum - i64Temp;
    return((frac32_t)i64Temp); 
}
 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSURND_F32_H_ */
