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
* @brief  Tangent
* 
*******************************************************************************/
#ifndef _GFLIB_TAN_F32_H_
#define _GFLIB_TAN_F32_H_

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
#define GFLIB_Tan_F16_C(f16Angle) GFLIB_Tan_F16_FC(f16Angle, &gsTanCoef)  
  
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    const frac32_t f32A[4];
}   GFLIB_TAN_COEF_T_F32;

typedef struct
{
    GFLIB_TAN_COEF_T_F32  GFLIB_TAN_SECTOR_F32[8];
}   GFLIB_TAN_T_F32;
  
/*******************************************************************************
* Global variables
*******************************************************************************/  
extern GFLIB_CONST GFLIB_TAN_T_F32 gsTanCoef;
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Tan_F16_FC(frac16_t f16Angle,
                                 GFLIB_CONST GFLIB_TAN_T_F32 *const pParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_TAN_F32_H_ */
