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
* @brief  Reciprocal value
* 
*******************************************************************************/
#ifndef _MLIB_RCP1Q_A32_ASM_H_
#define _MLIB_RCP1Q_A32_ASM_H_

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
#define MLIB_Rcp1Q1_A32s_Asm(f16Denom) MLIB_Rcp1Q1_A32s_FAsm(f16Denom)
#define MLIB_Rcp1Q_A32s_Asm(f16Denom)  MLIB_Rcp1Q_A32s_FAsm(f16Denom)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern acc32_t MLIB_Rcp1Q1_A32s_FAsm(register frac16_t f16Denom);
extern acc32_t MLIB_Rcp1Q_A32s_FAsm(register frac16_t f16Denom);

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_RCP1Q_A32_ASM_H_ */
