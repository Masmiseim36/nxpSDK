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
* @brief  Multiply accumulate of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MAC4RND_F32_H_
#define _MLIB_MAC4RND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Add_F32.h"
#include "MLIB_MulRnd_F32.h" 

/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Mac4Rnd_F32_Ci(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)    \
        MLIB_Mac4Rnd_F32_FCi(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2) 
  
/***************************************************************************//*!
*
* f32Out = (f32Add1Mul1 * f32Add1Mul2) + (f32Add2Mul1 * f32Add2Mul2)
* Without saturation
*******************************************************************************/  
RAM_FUNC_LIB 
static inline frac32_t MLIB_Mac4Rnd_F32_FCi(register frac32_t f32Add1Mul1, register frac32_t f32Add1Mul2, 
                                            register frac32_t f32Add2Mul1, register frac32_t f32Add2Mul2)
{
    return(frac32_t)((((int64_t)(f32Add1Mul1)*(int64_t)(f32Add1Mul2) + 
                       (int64_t)(f32Add2Mul1)*(int64_t)(f32Add2Mul2))+ 0x40000000)>>31);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4RND_F32_H_ */
