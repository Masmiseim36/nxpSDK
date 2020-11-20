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
* @brief  Arcus cosine function
* 
*******************************************************************************/
#ifndef _GFLIB_ACOS_F16_H_
#define _GFLIB_ACOS_F16_H_

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
#define GFLIB_Acos_F16_C(f16Val) GFLIB_Acos_F16_FC(f16Val, &f16gflibAcosCoef) 
#define GFLIB_Acos_F16_CRam(f16Val) GFLIB_Acos_F16_FCRam(f16Val, &f16gflibAcosCoefRam) 

/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{   /* Array of five 16-bit elements for storing coefficients of the piece-wise polynomial */
    const frac16_t  f16A[5];   
}   GFLIB_ACOS_TAYLOR_COEF_T_F16;
 
typedef struct
{   /* Array of two elements for storing two sub-arrays (each sub-array
    contains five 16-bit coefficients) for all sub-intervals*/
    GFLIB_ACOS_TAYLOR_COEF_T_F16  GFLIB_ACOS_SECTOR_F16[2]; 
                                                            
}   GFLIB_ACOS_T_F16;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern const GFLIB_ACOS_T_F16 f16gflibAcosCoef;
extern GFLIB_ACOS_T_F16 f16gflibAcosCoefRam;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Acos_F16_FC(frac16_t f16Val, const GFLIB_ACOS_T_F16 *const psParam);

RAM_FUNC_LIB 
extern frac16_t GFLIB_Acos_F16_FCRam(frac16_t f16Val, GFLIB_ACOS_T_F16 *const psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ACOS_F16_H_ */
