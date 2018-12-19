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
* @brief  Binary logarithm
* 
*******************************************************************************/
#ifndef _MLIB_LOG2_U16_H_
#define _MLIB_LOG2_U16_H_

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
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler   */
#define _clz(x) __CLZ(x)
#else
#define _clz(x) __builtin_clz(x)
#endif

#define MLIB_Log2_U16_Ci(u16Val) MLIB_Log2_U16_FCi(u16Val)
  
/***************************************************************************//*!
*
* @brief  Binary logarithm of 16-bit input
*
* @param  in  uint16_t u16Val - Argument in uint16_t
*
* @return This function returns - uint16_t value
*       
* @remarks  This function returns the 16-bit integer part of binary logarithm of the input. 
*           Returns 0 for input u16Val <= 0.
*
*******************************************************************************/
static inline uint16_t MLIB_Log2_U16_FCi(register uint16_t u16Val)
{
    return ((int32_t)u16Val > 0 ? (uint16_t)(0x1F - _clz(u16Val)): 0); 
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_LOG2_U16_H_ */
