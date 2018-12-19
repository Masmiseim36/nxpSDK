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
* @brief  Arcus sine function
* 
*******************************************************************************/
#ifndef _GFLIB_ASIN_FLT_H_
#define _GFLIB_ASIN_FLT_H_

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
#define GFLIB_Asin_FLT_C(fltVal) GFLIB_Asin_FLT_FC(fltVal, &fltgflibAsinCoef)
 
/*******************************************************************************
* Types
*******************************************************************************/ 
typedef struct
{
    const float_t  fltA[8];
}   GFLIB_ASIN_T_FLT;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern const GFLIB_ASIN_T_FLT fltgflibAsinCoef;
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Asin_FLT_FC(float_t fltVal,const GFLIB_ASIN_T_FLT *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ASIN_FLT_H_ */
