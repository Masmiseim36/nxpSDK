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
* @brief  Multiply subtract of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MSU4RND_F32_H_
#define _MLIB_MSU4RND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Sub_F64.h" 
#include "MLIB_Sub_F32.h"  
#include "MLIB_MulRnd_F32.h"   
  
/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Msu4Rnd_F32_Ci( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)    \
        MLIB_Msu4Rnd_F32_FCi( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2) 

/***************************************************************************//*!
*
* f32Out = (f32MinMul1 * f32MinMul2) - (f32SubMul1 * f32SubMul2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_Msu4Rnd_F32_FCi(register frac32_t f32MinMul1, register frac32_t f32MinMul2,
                                            register frac32_t f32SubMul1, register frac32_t f32SubMul2)
{
  return(frac32_t)(((MLIB_Sub_F64_Ci((int64_t)(f32MinMul1)*(int64_t)(f32MinMul2),
                                     (int64_t)(f32SubMul1)*(int64_t)(f32SubMul2))) + 0x40000000)>>31);       
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSU4RND_F32_H_ */
