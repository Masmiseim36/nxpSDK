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
#include "gflib.h"    
#include "GFLIB_Atan_F32.h"
#include "gflib_types.h"

/*******************************************************************************
* Global variables
*******************************************************************************/
extern GFLIB_ATAN_T_F32 gsAtanCoef;

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
                                      const GFLIB_ATAN_T_F32 *psParam);

#if defined(__cplusplus)
}
#endif
#endif /* _GFLIB_ATANYX_F16_H_ */
