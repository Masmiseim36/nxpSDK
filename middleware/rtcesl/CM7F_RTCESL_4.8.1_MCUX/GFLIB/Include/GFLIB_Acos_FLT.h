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
#ifndef _GFLIB_ACOS_FLT_H_
#define _GFLIB_ACOS_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/ 
#include "gflib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Acos_FLT_C(fltVal) GFLIB_Acos_FLT_FC(fltVal, &fltgflibAcosCoef) 
#define GFLIB_Acos_FLT_CRam(fltVal) GFLIB_Acos_FLT_FCRam(fltVal, &fltgflibAcosCoef)
  
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    const float_t  fltA[5];   /* Array of approximation coefficients */
}   GFLIB_ACOS_T_FLT;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern GFLIB_ACOS_T_FLT fltgflibAcosCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Acos_FLT_FC(float_t fltVal,const GFLIB_ACOS_T_FLT *const psParam);

RAM_FUNC_LIB 
extern float_t GFLIB_Acos_FLT_FCRam(float_t fltVal, GFLIB_ACOS_T_FLT * psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ACOS_FLT_H_ */

