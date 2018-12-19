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
* @brief  Unsigned division
* 
*******************************************************************************/
#ifndef _MLIB_DIVHW1Q_F16_ASM_H_
#define _MLIB_DIVHW1Q_F16_ASM_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "MLIB_MMDVSQ.h"

/*******************************************************************************
* Macros 
*******************************************************************************/    
#define MLIB_DivHw1Q_F16_Asm(f16Num, f16Denom)    MLIB_DivHw1Q_F16_FAsm(f16Num, f16Denom, &gu16CntMmdvsq)
#define MLIB_DivHw1QSat_F16_Asm(f16Num, f16Denom) MLIB_DivHw1QSat_F16_FAsm(f16Num, f16Denom, &gu16CntMmdvsq)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t MLIB_DivHw1Q_F16_FAsm(frac16_t f16Num, frac16_t f16Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHw1QSat_F16_FAsm(frac16_t f16Num, frac16_t f16Denom, const volatile uint16_t *pVal);

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIVHW_F16_ASM_H_ */
