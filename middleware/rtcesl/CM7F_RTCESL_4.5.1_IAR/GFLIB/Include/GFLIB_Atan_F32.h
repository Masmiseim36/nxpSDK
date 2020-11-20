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
* @brief  Inverse tangent 
* 
*******************************************************************************/
#ifndef _GFLIB_ATAN_F32_H_
#define _GFLIB_ATAN_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Atan_F16_Asm(f16Val) GFLIB_Atan_F16_FAsm(f16Val, &gsAtanCoef)
#define GFLIB_Atan_F16_AsmRam(f16Val) GFLIB_Atan_F16_FAsmRam(f16Val, &gsAtanCoefRam)
    
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t  f32A[3];
}GFLIB_ATAN_COEF_T_F32;

typedef struct
{
    GFLIB_ATAN_COEF_T_F32  GFLIB_ATAN_SECTOR_F32[8];
}GFLIB_ATAN_T_F32;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern const GFLIB_ATAN_T_F32 gsAtanCoef;
extern GFLIB_ATAN_T_F32 gsAtanCoefRam;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Atan_F16_FAsm(frac16_t f16Val, const GFLIB_ATAN_T_F32 *const psParam);

RAM_FUNC_LIB 
extern frac16_t GFLIB_Atan_F16_FAsmRam(frac16_t f16Val, GFLIB_ATAN_T_F32 *const psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ATAN_F32_H_ */
