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
#ifndef GFLIB_TAN_FLT_H
#define GFLIB_TAN_FLT_H

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Tan_FLT_C(fltAngle)        GFLIB_Tan_FLT_FC(fltAngle, &fltgflibTanCoef)
#define GFLIB_Tan_FLT_CRam(fltAngle)     GFLIB_Tan_FLT_FCRam(fltAngle, &fltgflibTanCoefRam)
#define GFLIB_Tan_FLTa_C(a32AngleExt)    GFLIB_Tan_FLTa_FC(a32AngleExt)
#define GFLIB_Tan_FLTa_CRam(a32AngleExt) GFLIB_Tan_FLTa_FCRam(a32AngleExt)
 
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    const float_t fltA[4];
}   GFLIB_TAN_T_FLT;

/*******************************************************************************
* Global variables
*******************************************************************************/ 
extern const GFLIB_TAN_T_FLT fltgflibTanCoef;
extern GFLIB_TAN_T_FLT fltgflibTanCoefRam;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Tan_FLT_FC(float_t fltAngle,
                                const GFLIB_TAN_T_FLT *const psParam);
RAM_FUNC_LIB 
extern float_t GFLIB_Tan_FLT_FCRam(float_t fltAngle,
                                   GFLIB_TAN_T_FLT *const psParam);
								
extern float_t GFLIB_Tan_FLTa_FC(acc32_t a32AngleExt);					
RAM_FUNC_LIB 
extern float_t GFLIB_Tan_FLTa_FCRam(acc32_t a32AngleExt);

#if defined(__cplusplus)
}
#endif

#endif /* GFLIB_TAN_FLT_H */
