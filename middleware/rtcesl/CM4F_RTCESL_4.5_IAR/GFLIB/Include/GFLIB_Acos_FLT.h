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
* @brief  Arcus cosine function
* 
*******************************************************************************/
#ifndef _GFLIB_ACOS_FLT_H_
#define _GFLIB_ACOS_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/ 
#include "gflib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Acos_FLT_C(fltVal) GFLIB_Acos_FLT_FC(fltVal, &fltgflibAcosCoef) 
  
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    const float_t  fltA[5];   /* Array of approximation coefficients */
}   GFLIB_ACOS_T_FLT;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern const GFLIB_ACOS_T_FLT fltgflibAcosCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Acos_FLT_FC(float_t fltVal,const GFLIB_ACOS_T_FLT *const psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ACOS_FLT_H_ */

