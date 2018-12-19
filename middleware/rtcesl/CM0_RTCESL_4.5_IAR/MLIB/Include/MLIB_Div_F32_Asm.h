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
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIV_F32_ASM_H_
#define _MLIB_DIV_F32_ASM_H_

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
#define MLIB_Div_F32_Asm(f32Num, f32Denom)      MLIB_Div_F32_FAsm(f32Num, f32Denom)
#define MLIB_Div_F32ls_Asm(f32Num, f16Denom)    MLIB_Div_F32ls_FAsm(f32Num, f16Denom)
#define MLIB_Div_F16ll_Asm(f32Num, f32Denom)    MLIB_Div_F16ll_FAsm(f32Num, f32Denom)
#define MLIB_DivSat_F32_Asm(f32Num, f32Denom)   MLIB_DivSat_F32_FAsm(f32Num, f32Denom)
#define MLIB_DivSat_F32ls_Asm(f32Num, f16Denom) MLIB_DivSat_F32ls_FAsm(f32Num, f16Denom)
#define MLIB_DivSat_F16ll_Asm(f32Num, f32Denom) MLIB_DivSat_F16ll_FAsm(f32Num, f32Denom)
#define MLIB_Div_F16ls_Asm(f32Num, f16Denom)    MLIB_Div_F16ls_FAsm(f32Num, f16Denom)
#define MLIB_DivSat_F16ls_Asm(f32Num, f16Denom) MLIB_DivSat_F16ls_FAsm(f32Num, f16Denom)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac32_t MLIB_Div_F32_FAsm(frac32_t f32Num, frac32_t f32Denom);
extern frac32_t MLIB_DivSat_F32_FAsm(frac32_t f32Num, frac32_t f32Denom);
extern frac32_t MLIB_Div_F32ls_FAsm(frac32_t f32Num, frac16_t f16Denom);
extern frac32_t MLIB_DivSat_F32ls_FAsm(frac32_t f32Num, frac16_t f16Denom);
extern frac16_t MLIB_Div_F16ll_FAsm(frac32_t f32Num, frac32_t f32Denom);
extern frac16_t MLIB_DivSat_F16ll_FAsm(frac32_t f32Num, frac32_t f32Denom);
extern frac16_t MLIB_Div_F16ls_FAsm(frac32_t f32Num, frac16_t f16Denom);
extern frac16_t MLIB_DivSat_F16ls_FAsm(frac32_t f32Num, frac16_t f16Denom);

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIV_F32_ASM_H_ */
