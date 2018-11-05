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
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIVHW_F32_ASM_H_
#define _MLIB_DIVHW_F32_ASM_H_

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
#define MLIB_DivHw_F32_Asm(f32Num, f32Denom)      MLIB_DivHw_F32_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)
#define MLIB_DivHw_F32ls_Asm(f32Num, f16Denom)    MLIB_DivHw_F32ls_FAsm(f32Num, f16Denom, &gu16CntMmdvsq)
#define MLIB_DivHw_F16ll_Asm(f32Num, f32Denom)    MLIB_DivHw_F16ll_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)
#define MLIB_DivHw_F16ls_Asm(f32Num, f16Denom)    MLIB_DivHw_F16ls_FAsm(f32Num, f16Denom, &gu16CntMmdvsq)

#define MLIB_DivHwSat_F32_Asm(f32Num, f32Denom)   MLIB_DivHwSat_F32_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)
#define MLIB_DivHwSat_F32ls_Asm(f32Num, f16Denom) MLIB_DivHwSat_F32ls_FAsm(f32Num, f16Denom, &gu16CntMmdvsq)
#define MLIB_DivHwSat_F16ll_Asm(f32Num, f32Denom) MLIB_DivHwSat_F16ll_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)
#define MLIB_DivHwSat_F16ls_Asm(f32Num, f16Denom) MLIB_DivHwSat_F16ls_FAsm(f32Num, f16Denom, &gu16CntMmdvsq)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/  
extern frac32_t MLIB_DivHw_F32_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
extern frac32_t MLIB_DivHw_F32ls_FAsm(frac32_t f32Num, frac16_t f16Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHw_F16ll_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHw_F16ls_FAsm(frac32_t f32Num, frac16_t f16Denom, const volatile uint16_t *pVal);

extern frac32_t MLIB_DivHwSat_F32_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
extern frac32_t MLIB_DivHwSat_F32ls_FAsm(frac32_t f32Num, frac16_t f16Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHwSat_F16ll_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHwSat_F16ls_FAsm(frac32_t f32Num, frac16_t f16Denom, const volatile uint16_t *pVal);

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIVHW_F32_ASM_H_ */
