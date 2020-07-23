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
* @brief  Digital IIR Filter, 4th order
*
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR4_F32_H_
#define _GDFLIB_FILTERIIR4_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gdflib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define GDFLIB_FilterIIR4Init_F16_C(psParam)                                   \
        GDFLIB_FilterIIR4Init_F16_FC(psParam)    
#define GDFLIB_FilterIIR4Init_F16_CRam(psParam)                                \
        GDFLIB_FilterIIR4Init_F16_FCRam(psParam)    		
#define GDFLIB_FilterIIR4_F16_C(f16InX, psParam)                               \
        GDFLIB_FilterIIR4_F16_FC(f16InX, psParam)
#define GDFLIB_FilterIIR4_F16_CRam(f16InX, psParam)                            \
        GDFLIB_FilterIIR4_F16_FCRam(f16InX, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32B0; /* B0 coefficient of an IIR4 filter */
    frac32_t f32B1; /* B1 coefficient of an IIR4 filter */
    frac32_t f32B2; /* B2 coefficient of an IIR4 filter */
    frac32_t f32B3; /* B3 coefficient of an IIR4 filter */
    frac32_t f32B4; /* B4 coefficient of an IIR4 filter */
    frac32_t f32A1; /* A1 coefficient of an IIR4 filter */
    frac32_t f32A2; /* A2 coefficient of an IIR4 filter */
    frac32_t f32A3; /* A3 coefficient of an IIR4 filter */
    frac32_t f32A4; /* A4 coefficient of an IIR4 filter */
} GDFLIB_FILTER_IIR4_COEFF_T_F32;

typedef struct{
    GDFLIB_FILTER_IIR4_COEFF_T_F32 sFltCoeff; /* Sub-structure containing filter coefficients. */
    frac32_t f32FltBfrY[4]; /* Internal accumulator buffer */
    frac16_t f16FltBfrX[4]; /* Input buffer of an IIR4 filter */
} GDFLIB_FILTER_IIR4_T_F32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/ 
void GDFLIB_FilterIIR4Init_F16_FC(GDFLIB_FILTER_IIR4_T_F32 *psParam);

RAM_FUNC_LIB
void GDFLIB_FilterIIR4Init_F16_FCRam(GDFLIB_FILTER_IIR4_T_F32 *psParam);

frac16_t GDFLIB_FilterIIR4_F16_FC(frac16_t f16InX,
                                  GDFLIB_FILTER_IIR4_T_F32 *psParam);

RAM_FUNC_LIB 
frac16_t GDFLIB_FilterIIR4_F16_FCRam(frac16_t f16InX,
                                  GDFLIB_FILTER_IIR4_T_F32 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR4_F32_H_ */
