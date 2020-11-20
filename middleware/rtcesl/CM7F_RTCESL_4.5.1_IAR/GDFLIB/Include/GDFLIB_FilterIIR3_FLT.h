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
* @brief  Digital Float IIR Filter, 3rd order
*
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR3_FLT_H_
#define _GDFLIB_FILTERIIR3_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gdflib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define GDFLIB_FilterIIR3Init_FLT_C(psParam)                                   \
        GDFLIB_FilterIIR3Init_FLT_FC(psParam)
#define GDFLIB_FilterIIR3Init_FLT_CRam(psParam)                                \
        GDFLIB_FilterIIR3Init_FLT_FCRam(psParam)   		
#define GDFLIB_FilterIIR3_FLT_C(fltInX, psParam)                               \
        GDFLIB_FilterIIR3_FLT_FC(fltInX, psParam)
#define GDFLIB_FilterIIR3_FLT_CRam(fltInX, psParam)                            \
        GDFLIB_FilterIIR3_FLT_FCRam(fltInX, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t fltB0; /* B0 coefficient of an IIR3 filter */
    float_t fltB1; /* B1 coefficient of an IIR3 filter */
    float_t fltB2; /* B2 coefficient of an IIR3 filter */
    float_t fltB3; /* B3 coefficient of an IIR3 filter */
    float_t fltA1; /* A1 coefficient of an IIR3 filter */
    float_t fltA2; /* A2 coefficient of an IIR3 filter */
    float_t fltA3; /* A3 coefficient of an IIR3 filter */
} GDFLIB_FILTER_IIR3_COEFF_T_FLT;

typedef struct
{
    GDFLIB_FILTER_IIR3_COEFF_T_FLT sFltCoeff; /* Sub-structure containing filter coefficients. */
    float_t fltFltBfrY[3]; /* Internal accumulator buffer */
    float_t fltFltBfrX[3]; /* Input buffer of an IIR3 filter */
} GDFLIB_FILTER_IIR3_T_FLT;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/ 
void GDFLIB_FilterIIR3Init_FLT_FC(GDFLIB_FILTER_IIR3_T_FLT *psParam);

RAM_FUNC_LIB 
void GDFLIB_FilterIIR3Init_FLT_FCRam(GDFLIB_FILTER_IIR3_T_FLT *psParam);

float_t GDFLIB_FilterIIR3_FLT_FC(float_t fltInX,
                                  GDFLIB_FILTER_IIR3_T_FLT *psParam);

RAM_FUNC_LIB 
float_t GDFLIB_FilterIIR3_FLT_FCRam(float_t fltInX,
                                    GDFLIB_FILTER_IIR3_T_FLT *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR3_FLT_H_ */
