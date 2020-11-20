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
#ifndef _GFLIB_ATAN_FLT_H_
#define _GFLIB_ATAN_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib_FP.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Atan_FLT_C(fltVal)  GFLIB_Atan_FLT_FC(fltVal, &gfltAtanCoef)
#define GFLIB_Atan_FLT_CRam(fltVal)  GFLIB_Atan_FLT_FCRam(fltVal, &gfltAtanCoefRam)
#define GFLIB_Atan_A32f_C(fltVal) GFLIB_Atan_A32f_FC(fltVal)
#define GFLIB_Atan_A32f_CRam(fltVal) GFLIB_Atan_A32f_FCRam(fltVal)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t  fltA[6];
}   GFLIB_ATAN_T_FLT;
  
/*******************************************************************************
* Global variables
*******************************************************************************/  
extern const GFLIB_ATAN_T_FLT gfltAtanCoef;
extern GFLIB_ATAN_T_FLT gfltAtanCoefRam;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Atan_FLT_FC(float_t fltVal, const GFLIB_ATAN_T_FLT *const psParam);
RAM_FUNC_LIB 
extern float_t GFLIB_Atan_FLT_FCRam(float_t fltVal, GFLIB_ATAN_T_FLT *const psParam);

extern acc32_t GFLIB_Atan_A32f_FC(float_t fltVal);
RAM_FUNC_LIB 
extern acc32_t GFLIB_Atan_A32f_FCRam(float_t fltVal);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ATAN_FLT_H_ */
