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
* @brief  Count of leading bits
* 
*******************************************************************************/
#ifndef _MLIB_CLB_F32_H_
#define _MLIB_CLB_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler   */
#define _clzl(x) __CLZ(x)
#else
#define _clzl(x) __builtin_clzl(x)
#endif
/*******************************************************************************
* Macros 
*******************************************************************************/
#define MLIB_Clb_U16l_Ci(f32Val) MLIB_Clb_U16l_FCi(f32Val)
  
/***************************************************************************//*!
* count of leading bits
*  - first, the absolute value of the input is calculated
*  - then the amount of zero bits before the first non-zero bits is counted (sign bit is not included)
****************************************************************************/ 
static inline uint16_t MLIB_Clb_U16l_FCi(register frac32_t f32Val)
{
    register frac32_t f32Temp;
    f32Temp = ((f32Val < 0) ? (~f32Val): f32Val);
    return (uint16_t)(_clzl(f32Temp) - 1);
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_CLB_F32_H_ */
