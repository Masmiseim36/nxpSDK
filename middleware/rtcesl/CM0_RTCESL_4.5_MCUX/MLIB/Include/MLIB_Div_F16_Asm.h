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
#ifndef _MLIB_DIV_F16_ASM_H_
#define _MLIB_DIV_F16_ASM_H_

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
#define MLIB_Div_F16_Asm(f16Num, f16Denom)    MLIB_Div_F16_FAsm(f16Num, f16Denom)
#define MLIB_DivSat_F16_Asm(f16Num, f16Denom) MLIB_DivSat_F16_FAsm(f16Num, f16Denom)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t MLIB_Div_F16_FAsm(frac16_t f16Num, frac16_t f16Denom);
extern frac16_t MLIB_DivSat_F16_FAsm(frac16_t f16Num, frac16_t f16Denom);

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIV_F16_ASM_H_ */
