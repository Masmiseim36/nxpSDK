/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
#define GDFLIB_FilterIIR4_F16_C(f16InX, psParam)                               \
        GDFLIB_FilterIIR4_F16_FC(f16InX, psParam)

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
frac16_t GDFLIB_FilterIIR4_F16_FC(frac16_t f16InX,
                                  GDFLIB_FILTER_IIR4_T_F32 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR4_F32_H_ */
