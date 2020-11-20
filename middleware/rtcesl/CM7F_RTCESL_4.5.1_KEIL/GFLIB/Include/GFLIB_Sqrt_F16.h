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
* @brief  Square root 
* 
*******************************************************************************/

#ifndef _GFLIB_SQRT_F16_H_
#define _GFLIB_SQRT_F16_H_

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
#define GFLIB_Sqrt_F16_C(f16Val)    GFLIB_Sqrt_F16_FC(f16Val, &gsSqrtTable_F32)
#define GFLIB_Sqrt_F16_CRam(f16Val) GFLIB_Sqrt_F16_FCRam(f16Val, &gsSqrtTable_F32)

/******************************************************************************
* Types
******************************************************************************/

/* Polynom table line */
typedef struct
{
    frac16_t f16Dummy;
    frac16_t f16XkOffset;
    frac16_t f16PolyCoef[5];
    frac16_t f16NYScl;
    frac32_t f32YkOffset;
} GFLIB_SQRT_ROW_T_F32;

/* Polynom table line pointer */
typedef struct
{
    GFLIB_SQRT_ROW_T_F32 *psLine;
} GFLIB_SQRT_OFFSET_T_F32;

/* Polynom table */
typedef struct
{
    GFLIB_SQRT_OFFSET_T_F32 sLine1;   
    GFLIB_SQRT_OFFSET_T_F32 sLine2;
    GFLIB_SQRT_OFFSET_T_F32 sLine3;
    GFLIB_SQRT_ROW_T_F32 sInterval1;  
    GFLIB_SQRT_ROW_T_F32 sInterval2;  
    GFLIB_SQRT_ROW_T_F32 sInterval3;  
} GFLIB_SQRT_TABLE_T_F32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Sqrt_F16_FC(frac16_t f16Val,
                                  const GFLIB_SQRT_TABLE_T_F32 *psParam);
RAM_FUNC_LIB
extern frac16_t GFLIB_Sqrt_F16_FCRam(frac16_t f16Val,
                                     GFLIB_SQRT_TABLE_T_F32 *psParam);								  
extern GFLIB_SQRT_TABLE_T_F32 gsSqrtTable_F32;

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRT_F16_H_ */

