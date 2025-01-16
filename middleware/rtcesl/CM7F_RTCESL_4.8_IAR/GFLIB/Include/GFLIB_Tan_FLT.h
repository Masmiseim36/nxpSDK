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
#ifndef GFLIB_TAN_FLT_H
#define GFLIB_TAN_FLT_H

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Tan_FLT_C(fltAngle)        GFLIB_Tan_FLT_FC(fltAngle, &fltgflibTanCoef)
#define GFLIB_Tan_FLT_CRam(fltAngle)     GFLIB_Tan_FLT_FCRam(fltAngle, &fltgflibTanCoef)
#define GFLIB_Tan_FLTa_C(a32AngleExt)    GFLIB_Tan_FLTa_FC(a32AngleExt)
#define GFLIB_Tan_FLTa_CRam(a32AngleExt) GFLIB_Tan_FLTa_FCRam(a32AngleExt)
 
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    const float_t fltA[4];
}   GFLIB_TAN_T_FLT;

/*******************************************************************************
* Global variables
*******************************************************************************/ 
extern GFLIB_TAN_T_FLT fltgflibTanCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Tan_FLT_FC(float_t fltAngle,
                                const GFLIB_TAN_T_FLT *const psParam);
RAM_FUNC_LIB 
extern float_t GFLIB_Tan_FLT_FCRam(float_t fltAngle,
                                   GFLIB_TAN_T_FLT * psParam);
								
extern float_t GFLIB_Tan_FLTa_FC(acc32_t a32AngleExt);					
RAM_FUNC_LIB 
extern float_t GFLIB_Tan_FLTa_FCRam(acc32_t a32AngleExt);

#if defined(__cplusplus)
}
#endif

#endif /* GFLIB_TAN_FLT_H */
