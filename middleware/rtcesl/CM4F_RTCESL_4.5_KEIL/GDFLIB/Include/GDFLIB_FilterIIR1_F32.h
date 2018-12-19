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
* @brief  Digital IIR Filter, 1st order 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR1_F32_H_
#define _GDFLIB_FILTERIIR1_F32_H_

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
#define GDFLIB_FilterIIR1Init_F16_Ci(psParam)                                  \
        GDFLIB_FilterIIR1Init_F16_FCi(psParam)                                                             
#define GDFLIB_FilterIIR1_F16_C(f16InX, psParam)                               \
        GDFLIB_FilterIIR1_F16_FC(f16InX, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32B0; /* B0 coefficient of an IIR1 filter */
    frac32_t f32B1; /* B1 coefficient of an IIR1 filter */
    frac32_t f32A1; /* A1 coefficient of an IIR1 filter */
} GDFLIB_FILTER_IIR1_COEFF_T_F32;

typedef struct
{
    GDFLIB_FILTER_IIR1_COEFF_T_F32 sFltCoeff; /* Sub-structure containing filter coefficients. */
    frac32_t f32FltBfrY[1]; /* Internal accumulator buffer */    
    frac16_t f16FltBfrX[1]; /* Input buffer of an IIR1 filter */
} GDFLIB_FILTER_IIR1_T_F32;

/***************************************************************************//*!
*
* @brief  The function clears buffer of 2nd order IIR filter pointed to by argument
*
* @param  ptr   GDFLIB_FILTER_IIR1_T_F16 *pudtFilter - Pointer to filter structure
*
* @return N/A 
*        
*******************************************************************************/
 static inline void GDFLIB_FilterIIR1Init_F16_FCi(GDFLIB_FILTER_IIR1_T_F32 *psParam)
 {
    psParam->f32FltBfrY[0] = (frac32_t)0;    
    psParam->f16FltBfrX[0] = (frac16_t)0;
 }
 
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR1_F32_H_ */
