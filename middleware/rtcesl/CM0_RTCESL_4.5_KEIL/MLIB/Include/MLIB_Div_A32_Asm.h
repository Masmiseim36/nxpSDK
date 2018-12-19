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
#ifndef _MLIB_DIV_A32_ASM_H_
#define _MLIB_DIV_A32_ASM_H_

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
#define MLIB_Div_A32ss_Asm(f16Num, f16Denom) MLIB_Div_A32ss_FAsm(f16Num, f16Denom)
#define MLIB_Div_A32ls_Asm(f32Num, f16Denom) MLIB_Div_A32ls_FAsm(f32Num, f16Denom)
#define MLIB_Div_A32ll_Asm(f32Num, f32Denom) MLIB_Div_A32ll_FAsm(f32Num, f32Denom)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern acc32_t MLIB_Div_A32ll_FAsm(frac32_t f32Num, frac32_t f32Denom);
extern acc32_t MLIB_Div_A32ss_FAsm(frac16_t f16Num, frac16_t f16Denom);
extern acc32_t MLIB_Div_A32ls_FAsm(frac32_t f32Num, frac16_t f16Denom);

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_DIV_A32_H_ASM_ */
