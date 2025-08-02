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
extern GFLIB_CONST GFLIB_ACOS_T_F16 f16gflibAcosCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Acos_F16_FC(frac16_t f16Val, GFLIB_CONST GFLIB_ACOS_T_F16 *const psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ACOS_F16_H_ */
