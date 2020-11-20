/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
RAM_FUNC_LIB 
static inline uint16_t MLIB_Log2_U16_FCi(register uint16_t u16Val)
{
    return ((int32_t)u16Val > 0 ? (uint16_t)(0x1F - _clz(u16Val)): 0); 
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_LOG2_U16_H_ */
