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
* @brief  Sine, Cosine
* 
*******************************************************************************/
#ifndef _GFLIB_SINCOS_FLT_H_
#define _GFLIB_SINCOS_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h" 
#include "gflib_types.h"
  
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Sin_FLT_C(fltAngle)        GFLIB_Sin_FLT_FC(fltAngle, &gfltSinCoef)
#define GFLIB_Sin_FLT_CRam(fltAngle)     GFLIB_Sin_FLT_FCRam(fltAngle, &gfltSinCoef)
#define GFLIB_Sin_FLTa_C(a32AngleExt)    GFLIB_Sin_FLTa_FC(a32AngleExt)
#define GFLIB_Sin_FLTa_CRam(a32AngleExt) GFLIB_Sin_FLTa_FCRam(a32AngleExt)
#define GFLIB_Cos_FLT_C(fltAngle)        GFLIB_Cos_FLT_FC(fltAngle, &gfltCosCoef)
#define GFLIB_Cos_FLT_CRam(fltAngle)     GFLIB_Cos_FLT_FCRam(fltAngle, &gfltCosCoef)
#define GFLIB_Cos_FLTa_C(a32AngleExt)    GFLIB_Cos_FLTa_FC(a32AngleExt)
#define GFLIB_Cos_FLTa_CRam(a32AngleExt) GFLIB_Cos_FLTa_FCRam(a32AngleExt)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t  fltA[3];    /* array of coefficients */
} GFLIB_SIN_T_FLT;

typedef struct
{
    float_t  fltA[3];    /* array of coefficients */
} GFLIB_COS_T_FLT;
 
/*******************************************************************************
* Global variables
*******************************************************************************/  
extern GFLIB_SIN_T_FLT gfltSinCoef;
extern GFLIB_COS_T_FLT gfltCosCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Sin_FLT_FC(float_t fltAngle, const GFLIB_SIN_T_FLT *const psParam);
RAM_FUNC_LIB 
extern float_t GFLIB_Sin_FLT_FCRam(float_t fltAngle, GFLIB_SIN_T_FLT * psParam);

extern float_t GFLIB_Sin_FLTa_FC(acc32_t a32AngleExt);
RAM_FUNC_LIB 
extern float_t GFLIB_Sin_FLTa_FCRam(acc32_t a32AngleExt);

extern float_t GFLIB_Cos_FLT_FC(float_t fltAngle, const GFLIB_COS_T_FLT *const psParam);
RAM_FUNC_LIB 
extern float_t GFLIB_Cos_FLT_FCRam(float_t fltAngle, GFLIB_COS_T_FLT * psParam);

extern float_t GFLIB_Cos_FLTa_FC(acc32_t a32AngleExt);
RAM_FUNC_LIB 
extern float_t GFLIB_Cos_FLTa_FCRam(acc32_t a32AngleExt);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOS_FLT_H_ */

