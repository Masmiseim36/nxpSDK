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
#define GFLIB_AtanYX_FLT_CRam(fltY, fltX, pbErrFlag)                          \
        GFLIB_AtanYX_FLT_FCRam(fltY, fltX, pbErrFlag)		
#define GFLIB_AtanYX_A32f_C(fltY, fltX, pbErrFlag)                            \
        GFLIB_AtanYX_A32f_FC(fltY, fltX, pbErrFlag)
#define GFLIB_AtanYX_A32f_CRam(fltY, fltX, pbErrFlag)                         \
        GFLIB_AtanYX_A32f_FCRam(fltY, fltX, pbErrFlag)

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern float_t GFLIB_AtanYX_FLT_FC(float_t fltY, float_t fltX, 
                                   bool_t *pbErrFlag);

RAM_FUNC_LIB 
extern float_t GFLIB_AtanYX_FLT_FCRam(float_t fltY, float_t fltX, 
                                      bool_t *pbErrFlag);
								   								   
extern acc32_t GFLIB_AtanYX_A32f_FC(float_t fltY, float_t fltX, 
                                    bool_t *pbErrFlag);
RAM_FUNC_LIB 
extern acc32_t GFLIB_AtanYX_A32f_FCRam(float_t fltY, float_t fltX, 
                                       bool_t *pbErrFlag);
									

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ATANYX_FLT_H_ */
