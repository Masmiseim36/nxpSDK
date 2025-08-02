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
* @brief  Arcus tangent function based on the provided x,y coordinates as arguments
*        using division and piece-wise polynomial approximation
* 
*******************************************************************************/
#ifndef _GFLIB_ATANYX_F16_H_
#define _GFLIB_ATANYX_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include "GFLIB_Atan_F32.h"

/*******************************************************************************
* Global variables
*******************************************************************************/
extern GFLIB_CONST GFLIB_ATAN_T_F32 gsAtanCoef;

/******************************************************************************
* Macros 
******************************************************************************/                         
#define GFLIB_AtanYX_F16_Asm(f16Y, f16X, pbErrFlag)                          \
        GFLIB_AtanYX_F16_FAsm(f16Y, f16X, pbErrFlag, &gsAtanCoef)

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern frac16_t GFLIB_AtanYX_F16_FAsm(frac16_t f16Y, frac16_t f16X,
                                      bool_t *pbErrFlag,
                                      GFLIB_CONST GFLIB_ATAN_T_F32 *const psParam);

#if defined(__cplusplus)
}
#endif
#endif /* _GFLIB_ATANYX_F16_H_ */
