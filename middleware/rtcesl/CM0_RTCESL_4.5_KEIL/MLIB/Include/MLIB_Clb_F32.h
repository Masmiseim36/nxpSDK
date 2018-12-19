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
    register uint32_t u32Out;        
    register uint32_t u32shift;
    
    f32Val = (f32Val <= 0) ? (~f32Val) : (f32Val);
    f32Val = (f32Val == 0) ? f32Val-1 : f32Val;
    
    u32Out   = (f32Val > 0xFFFF) << 4; f32Val >>= u32Out;
    u32shift = (f32Val > 0xFF  ) << 3; f32Val >>= u32shift; u32Out |= u32shift;
    u32shift = (f32Val > 0xF   ) << 2; f32Val >>= u32shift; u32Out |= u32shift;
    u32shift = (f32Val > 0x3   ) << 1; f32Val >>= u32shift; u32Out |= u32shift;
    u32Out  |= (f32Val >> 1);
      
    return((uint16_t)30 - (uint16_t)(u32Out));
}
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_CLB_F32_H_ */
