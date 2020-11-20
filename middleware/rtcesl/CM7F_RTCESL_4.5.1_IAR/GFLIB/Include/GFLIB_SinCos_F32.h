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
* @brief  Sine, Cosine
* 
*******************************************************************************/
#ifndef _GFLIB_SINCOS_A32_H_
#define _GFLIB_SINCOS_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h" 
#include "gflib_types.h"
  
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Sin_F16_Asm(f16Angle)    GFLIB_Sin_F16_FAsm(f16Angle, &gsSinCoef)
#define GFLIB_Sin_F16_AsmRam(f16Angle) GFLIB_Sin_F16_FAsmRam(f16Angle, &gsSinCoefRam)
#define GFLIB_Cos_F16_Asmi(f16Angle)   GFLIB_Cos_F16_FAsmi(f16Angle)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t  f32A[5];    /* Array of fractional coefficients */
} GFLIB_SIN_T_F32;

/*******************************************************************************
* Global variables
*******************************************************************************/  
extern const GFLIB_SIN_T_F32 gsSinCoef;
extern GFLIB_SIN_T_F32 gsSinCoefRam;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Sin_F16_FAsm(frac16_t f16Angle, const GFLIB_SIN_T_F32 *psParam);
RAM_FUNC_LIB 
extern frac16_t GFLIB_Sin_F16_FAsmRam(frac16_t f16Angle, GFLIB_SIN_T_F32 *psParam);

/***************************************************************************//*!
* Cosine is calculated using the GFLIB_Sin_F16 function summed with FRAC(0.5)
*******************************************************************************/  
RAM_FUNC_LIB 
static inline frac16_t GFLIB_Cos_F16_FAsmi(register frac16_t f16Angle)
{
#if (defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW)) /* placed to RAM */
    return(GFLIB_Sin_F16_AsmRam((f16Angle +(frac16_t)16384)));
#else /* Placed to ROM */
    return(GFLIB_Sin_F16_Asm((f16Angle +(frac16_t)16384)));
#endif    
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOS_A32_H_ */

