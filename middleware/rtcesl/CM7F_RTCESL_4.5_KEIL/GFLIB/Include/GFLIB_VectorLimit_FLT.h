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
* @brief  Vector limit functions with float output
* 
*******************************************************************************/
#ifndef _GFLIB_VECTORLIMIT_FLT_H_
#define _GFLIB_VECTORLIMIT_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_FP.h"  

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_VectorLimit_FLT_C(psVectorIn, fltLim, psVectorOut)               \
        GFLIB_VectorLimit_FLT_FC(psVectorIn, fltLim, psVectorOut)              
#define GFLIB_VectorLimit1_FLT_C(psVectorIn, fltLim, psVectorOut)              \
        GFLIB_VectorLimit1_FLT_FC(psVectorIn, fltLim, psVectorOut) 
        
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    float_t fltA;   /* First argument */
    float_t fltB;   /* Second argument */
} GFLIB_VECTORLIMIT_T_FLT;
 
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern bool_t GFLIB_VectorLimit_FLT_FC(const GFLIB_VECTORLIMIT_T_FLT *psVectorIn, 
                                       float_t fltLim,
                                       GFLIB_VECTORLIMIT_T_FLT *psVectorOut);
extern bool_t GFLIB_VectorLimit1_FLT_FC(const GFLIB_VECTORLIMIT_T_FLT *psVectorIn, 
                                        float_t fltLim,
                                        GFLIB_VECTORLIMIT_T_FLT *psVectorOut);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ASIN_FLT_H_ */
