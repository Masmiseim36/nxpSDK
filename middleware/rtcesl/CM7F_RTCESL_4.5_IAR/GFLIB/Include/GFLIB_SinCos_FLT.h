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
#define GFLIB_Sin_FLT_C(fltAngle)     GFLIB_Sin_FLT_FC(fltAngle, &gfltSinCoef)
#define GFLIB_Sin_FLTa_C(a32AngleExt) GFLIB_Sin_FLTa_FC(a32AngleExt)
#define GFLIB_Cos_FLT_C(fltAngle)     GFLIB_Cos_FLT_FC(fltAngle, &gfltCosCoef)
#define GFLIB_Cos_FLTa_C(a32AngleExt) GFLIB_Cos_FLTa_FC(a32AngleExt)

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
extern GFLIB_CONST GFLIB_SIN_T_FLT gfltSinCoef;
extern GFLIB_CONST GFLIB_COS_T_FLT gfltCosCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Sin_FLT_FC(float_t fltAngle, const GFLIB_SIN_T_FLT *psParam);
extern float_t GFLIB_Sin_FLTa_FC(acc32_t a32AngleExt);
extern float_t GFLIB_Cos_FLT_FC(float_t fltAngle, const GFLIB_COS_T_FLT *psParam);
extern float_t GFLIB_Cos_FLTa_FC(acc32_t a32AngleExt);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOS_FLT_H_ */

