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
* @brief  Inverse tangent 
* 
*******************************************************************************/
#ifndef _GFLIB_ATAN_F32_H_
#define _GFLIB_ATAN_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Atan_F16_Asm(f16Val) GFLIB_Atan_F16_FAsm(f16Val, &gsAtanCoef)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t  f32A[3];
}GFLIB_ATAN_COEF_T_F32;

typedef struct
{
    GFLIB_ATAN_COEF_T_F32  GFLIB_ATAN_SECTOR_F32[8];
}GFLIB_ATAN_T_F32;

/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Atan_F16_FAsm(frac16_t f16Val, GFLIB_CONST GFLIB_ATAN_T_F32 *const psParam);


#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ATAN_F32_H_ */
