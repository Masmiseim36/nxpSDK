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
#ifndef _MLIB_RCPHW_A32_ASM_H_
#define _MLIB_RCPHW_A32_ASM_H_

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
#define MLIB_RcpHw1_A32s_Asm(f16Denom) MLIB_RcpHw_A32s_FAsm(f16Denom, &gu16CntMmdvsq)
#define MLIB_RcpHw_A32s_Asm(f16Denom)  MLIB_RcpHw_A32s_FAsm(f16Denom, &gu16CntMmdvsq)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern acc32_t MLIB_RcpHw_A32s_FAsm(register frac16_t f16Denom, const volatile uint16_t *pVal);

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_RCPHW_A32_ASM_H_ */
