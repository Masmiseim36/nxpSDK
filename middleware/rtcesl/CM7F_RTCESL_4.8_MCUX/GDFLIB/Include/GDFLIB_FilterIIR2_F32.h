/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
* 
*
****************************************************************************//*!
*
* @brief  Digital IIR Filter, 2nd order 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR2_F32_H_
#define _GDFLIB_FILTERIIR2_F32_H_

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
#define GDFLIB_FilterIIR2Init_F16_C(psParam)                                  \
        GDFLIB_FilterIIR2Init_F16_FC(psParam)   
#define GDFLIB_FilterIIR2Init_F16_CRam(psParam)                               \
        GDFLIB_FilterIIR2Init_F16_FCRam(psParam)		
#define GDFLIB_FilterIIR2_F16_Asm(f16InX, psParam)                            \
        GDFLIB_FilterIIR2_F16_FAsm(f16InX, psParam)
#define GDFLIB_FilterIIR2_F16_AsmRam(f16InX, psParam)                         \
        GDFLIB_FilterIIR2_F16_FAsmRam(f16InX, psParam)
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32B0; /* B0 coefficient of an IIR2 filter */
    frac32_t f32B1; /* B1 coefficient of an IIR2 filter */
    frac32_t f32B2; /* B2 coefficient of an IIR2 filter */
    frac32_t f32A1; /* A1 coefficient of an IIR2 filter */
    frac32_t f32A2; /* A2 coefficient of an IIR2 filter */
} GDFLIB_FILTER_IIR2_COEFF_T_F32;

typedef struct
{
    GDFLIB_FILTER_IIR2_COEFF_T_F32 sFltCoeff; /* Sub-structure containing filter coefficients. */
    frac32_t f32FltBfrY[2]; /* Internal accumulator buffer */    
    frac16_t f16FltBfrX[2]; /* Input buffer of an IIR2 filter */
} GDFLIB_FILTER_IIR2_T_F32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/ 
void GDFLIB_FilterIIR2Init_F16_FC(GDFLIB_FILTER_IIR2_T_F32 *psParam);

RAM_FUNC_LIB 
void GDFLIB_FilterIIR2Init_F16_FCRam(GDFLIB_FILTER_IIR2_T_F32 *psParam);

frac16_t GDFLIB_FilterIIR2_F16_FAsm(frac16_t f16InX,
                                    GDFLIB_FILTER_IIR2_T_F32 *psParam);

RAM_FUNC_LIB 
frac16_t GDFLIB_FilterIIR2_F16_FAsmRam(frac16_t f16InX,
                                       GDFLIB_FILTER_IIR2_T_F32 *psParam);
                                  
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR2_F32_H_ */
