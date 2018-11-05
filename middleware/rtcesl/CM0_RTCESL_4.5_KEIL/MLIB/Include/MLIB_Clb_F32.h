/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
