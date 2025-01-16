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
#define GFLIB_VectorLimit_FLT_CRam(psVectorIn, fltLim, psVectorOut)            \
        GFLIB_VectorLimit_FLT_FCRam(psVectorIn, fltLim, psVectorOut)     		
#define GFLIB_VectorLimit1_FLT_C(psVectorIn, fltLim, psVectorOut)              \
        GFLIB_VectorLimit1_FLT_FC(psVectorIn, fltLim, psVectorOut) 
#define GFLIB_VectorLimit1_FLT_CRam(psVectorIn, fltLim, psVectorOut)           \
        GFLIB_VectorLimit1_FLT_FCRam(psVectorIn, fltLim, psVectorOut) 		
        
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
RAM_FUNC_LIB 
extern bool_t GFLIB_VectorLimit_FLT_FCRam(const GFLIB_VECTORLIMIT_T_FLT *psVectorIn, 
                                          float_t fltLim,
                                          GFLIB_VECTORLIMIT_T_FLT *psVectorOut);
									   
extern bool_t GFLIB_VectorLimit1_FLT_FC(const GFLIB_VECTORLIMIT_T_FLT *psVectorIn, 
                                        float_t fltLim,
                                        GFLIB_VECTORLIMIT_T_FLT *psVectorOut);									   									   
RAM_FUNC_LIB 
extern bool_t GFLIB_VectorLimit1_FLT_FCRam(const GFLIB_VECTORLIMIT_T_FLT *psVectorIn, 
                                           float_t fltLim,
                                           GFLIB_VECTORLIMIT_T_FLT *psVectorOut);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ASIN_FLT_H_ */
