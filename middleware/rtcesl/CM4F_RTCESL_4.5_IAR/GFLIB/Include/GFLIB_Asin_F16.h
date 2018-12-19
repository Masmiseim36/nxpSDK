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
#ifndef _GFLIB_ASIN_F16_H_
#define _GFLIB_ASIN_F16_H_

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
#define GFLIB_Asin_F16_C(f16Val) GFLIB_Asin_F16_FC(f16Val, &f16gflibAsinCoef)
  
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    const frac16_t  f16A[5];
}   GFLIB_ASIN_TAYLOR_COEF_T_F16;

typedef struct
{
    GFLIB_ASIN_TAYLOR_COEF_T_F16  GFLIB_ASIN_SECTOR_F16[2];
}   GFLIB_ASIN_T_F16;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern const GFLIB_ASIN_T_F16 f16gflibAsinCoef;
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Asin_F16_FC(frac16_t f16Val, const GFLIB_ASIN_T_F16 *psParam);

#if defined(__cplusplus)
}
#endif
#endif  /* _GFLIB_ASIN_F16_H_ */
