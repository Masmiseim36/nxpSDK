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

#ifndef _GFLIB_SQRT_F32_H_
#define _GFLIB_SQRT_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib.h"
#include "GFLIB_Sqrt_F16.h"    

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Sqrt_F16l_C(f32Val) GFLIB_Sqrt_F16l_FC(f32Val, &gsSqrtTable_F32)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Sqrt_F16l_FC(frac32_t f32Val,
                                   const GFLIB_SQRT_TABLE_T_F32 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRT_F32_H_ */

