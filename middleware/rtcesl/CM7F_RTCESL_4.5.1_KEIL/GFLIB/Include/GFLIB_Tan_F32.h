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
* @brief  Tangent
* 
*******************************************************************************/
#ifndef _GFLIB_TAN_F32_H_
#define _GFLIB_TAN_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/  
#include "gflib.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Tan_F16_C(f16Angle) GFLIB_Tan_F16_FC(f16Angle, &gsTanCoef)  
#define GFLIB_Tan_F16_CRam(f16Angle) GFLIB_Tan_F16_FCRam(f16Angle, &gsTanCoefRam)  
  
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    const frac32_t f32A[4];
}   GFLIB_TAN_COEF_T_F32;

typedef struct
{
    GFLIB_TAN_COEF_T_F32  GFLIB_TAN_SECTOR_F32[8];
}   GFLIB_TAN_T_F32;
  
/*******************************************************************************
* Global variables
*******************************************************************************/  
extern const GFLIB_TAN_T_F32 gsTanCoef;
extern GFLIB_TAN_T_F32 gsTanCoefRam;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Tan_F16_FC(frac16_t f16Angle,
                                 const GFLIB_TAN_T_F32 *const pParam);	
RAM_FUNC_LIB 
extern frac16_t GFLIB_Tan_F16_FCRam(frac16_t f16Angle,
                                    GFLIB_TAN_T_F32 *const pParam);						 

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_TAN_F32_H_ */
