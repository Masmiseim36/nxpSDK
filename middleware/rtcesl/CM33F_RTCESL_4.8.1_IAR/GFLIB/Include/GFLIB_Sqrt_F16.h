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
* @brief  Square root 
* 
*******************************************************************************/

#ifndef _GFLIB_SQRT_F16_H_
#define _GFLIB_SQRT_F16_H_

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
#define GFLIB_Sqrt_F16_C(f16Val) GFLIB_Sqrt_F16_FC(f16Val, &gsSqrtTable_F32)

/******************************************************************************
* Types
******************************************************************************/

/* Polynom table line */
typedef struct
{
    frac16_t f16Dummy;
    frac16_t f16XkOffset;
    frac16_t f16PolyCoef[5];
    frac16_t f16NYScl;
    frac32_t f32YkOffset;
} GFLIB_SQRT_ROW_T_F32;

/* Polynom table line pointer */
typedef struct
{
    GFLIB_SQRT_ROW_T_F32 *psLine;
} GFLIB_SQRT_OFFSET_T_F32;

/* Polynom table */
typedef struct
{
    GFLIB_SQRT_OFFSET_T_F32 sLine1;   
    GFLIB_SQRT_OFFSET_T_F32 sLine2;
    GFLIB_SQRT_OFFSET_T_F32 sLine3;
    GFLIB_SQRT_ROW_T_F32 sInterval1;  
    GFLIB_SQRT_ROW_T_F32 sInterval2;  
    GFLIB_SQRT_ROW_T_F32 sInterval3;  
} GFLIB_SQRT_TABLE_T_F32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Sqrt_F16_FC(frac16_t f16Val,
                                  const GFLIB_SQRT_TABLE_T_F32 *psParam);
extern GFLIB_SQRT_TABLE_T_F32 gsSqrtTable_F32;

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRT_F16_H_ */

