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
*         using division and piece-wise polynomial approximation
* 
*******************************************************************************/
#ifndef _GFLIB_ATANYX_FLT_H_
#define _GFLIB_ATANYX_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_FP.h"
#include "GFLIB_Atan_FLT.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_AtanYX_FLT_C(fltY, fltX, pbErrFlag)                             \
        GFLIB_AtanYX_FLT_FC(fltY, fltX, pbErrFlag)                            
#define GFLIB_AtanYX_A32f_C(fltY, fltX, pbErrFlag)                            \
        GFLIB_AtanYX_A32f_FC(fltY, fltX, pbErrFlag)

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern float_t GFLIB_AtanYX_FLT_FC(float_t fltY, float_t fltX, 
                                   bool_t *pbErrFlag);
extern acc32_t GFLIB_AtanYX_A32f_FC(float_t fltY, float_t fltX, 
                                    bool_t *pbErrFlag);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ATANYX_FLT_H_ */
