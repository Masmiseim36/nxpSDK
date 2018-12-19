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
* @brief  Tangent
* 
*******************************************************************************/
#ifndef GFLIB_TAN_FLT_H
#define GFLIB_TAN_FLT_H

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Tan_FLT_C(fltAngle)     GFLIB_Tan_FLT_FC(fltAngle, &fltgflibTanCoef)
#define GFLIB_Tan_FLTa_C(a32AngleExt) GFLIB_Tan_FLTa_FC(a32AngleExt)
 
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    const float_t fltA[4];
}   GFLIB_TAN_T_FLT;

/*******************************************************************************
* Global variables
*******************************************************************************/ 
extern const GFLIB_TAN_T_FLT fltgflibTanCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Tan_FLT_FC(float_t fltAngle,
                                const GFLIB_TAN_T_FLT *const psParam);
extern float_t GFLIB_Tan_FLTa_FC(acc32_t a32AngleExt);

#if defined(__cplusplus)
}
#endif

#endif /* GFLIB_TAN_FLT_H */
