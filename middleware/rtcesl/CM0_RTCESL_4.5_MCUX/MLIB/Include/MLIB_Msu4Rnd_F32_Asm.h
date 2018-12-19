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
#ifndef _MLIB_MSU4RND_F32_ASM_H_
#define _MLIB_MSU4RND_F32_ASM_H_

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
#define MLIB_Msu4Rnd_F32_Asm(f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)    \
        MLIB_Msu4Rnd_F32_FAsm(f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)
#define MLIB_Msu4RndSat_F32_Asm(f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2) \
        MLIB_Msu4RndSat_F32_FAsm(f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac32_t MLIB_Msu4Rnd_F32_FAsm(register frac32_t f32MinMul1, register frac32_t f32MinMul2, 
                                      register frac32_t f32SubMul1, register frac32_t f32SubMul2);
extern frac32_t MLIB_Msu4RndSat_F32_FAsm(register frac32_t f32MinMul1,register frac32_t f32MinMul2,
                                         register frac32_t f32SubMul1,register frac32_t f32SubMul2);

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSU4RND_F32_ASM_H_ */
