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
* @brief  Inverse tangent 
* 
*******************************************************************************/
#ifndef _GFLIB_ATAN_FLT_H_
#define _GFLIB_ATAN_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib_FP.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Atan_FLT_C(fltVal)  GFLIB_Atan_FLT_FC(fltVal, &gfltAtanCoef)
#define GFLIB_Atan_A32f_C(fltVal) GFLIB_Atan_A32f_FC(fltVal)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t  fltA[6];
}   GFLIB_ATAN_T_FLT;
  
/*******************************************************************************
* Global variables
*******************************************************************************/  
extern GFLIB_CONST GFLIB_ATAN_T_FLT gfltAtanCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Atan_FLT_FC(float_t fltVal, const GFLIB_ATAN_T_FLT *psParam);
extern acc32_t GFLIB_Atan_A32f_FC(float_t fltVal);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ATAN_FLT_H_ */
