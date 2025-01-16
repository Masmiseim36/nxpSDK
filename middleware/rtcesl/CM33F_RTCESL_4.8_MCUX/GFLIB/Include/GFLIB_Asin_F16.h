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
extern GFLIB_CONST GFLIB_ASIN_T_F16 f16gflibAsinCoef;
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Asin_F16_FC(frac16_t f16Val, GFLIB_CONST GFLIB_ASIN_T_F16 *const psParam);

#if defined(__cplusplus)
}
#endif
#endif  /* _GFLIB_ASIN_F16_H_ */
