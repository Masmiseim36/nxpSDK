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
#ifndef _GFLIB_ACOS_F16_H_
#define _GFLIB_ACOS_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib.h"  
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Acos_F16_C(f16Val) GFLIB_Acos_F16_FC(f16Val, &f16gflibAcosCoef) 

/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{   /* Array of five 16-bit elements for storing coefficients of the piece-wise polynomial */
    const frac16_t  f16A[5];   
}   GFLIB_ACOS_TAYLOR_COEF_T_F16;
 
typedef struct
{   /* Array of two elements for storing two sub-arrays (each sub-array
    contains five 16-bit coefficients) for all sub-intervals*/
    GFLIB_ACOS_TAYLOR_COEF_T_F16  GFLIB_ACOS_SECTOR_F16[2]; 
                                                            
}   GFLIB_ACOS_T_F16;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern const GFLIB_ACOS_T_F16 f16gflibAcosCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Acos_F16_FC(frac16_t f16Val, const GFLIB_ACOS_T_F16 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ACOS_F16_H_ */
