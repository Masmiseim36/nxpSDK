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
****************************************************************************//*!
*
* @brief Main GDFLIB header file for devices with FPU. 
* 
*******************************************************************************/
#ifndef _GDFLIB_FP_H_
#define _GDFLIB_FP_H_

#if defined(__cplusplus)
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "gdflib.h"
#include "GDFLIB_FilterExp_FLT.h" 
#include "GDFLIB_FilterIIR1_FLT.h"
#include "GDFLIB_FilterIIR2_FLT.h"
#include "GDFLIB_FilterIIR3_FLT.h"
#include "GDFLIB_FilterIIR4_FLT.h"    
#include "GDFLIB_FilterMA_FLT.h"      

/*******************************************************************************
* Macros 
*******************************************************************************/
#if (defined(RAM_RELOCATION)) /* placed to RAM */                                
#define GDFLIB_FilterIIR2Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR2Init_FLT_CRam(psParam)                                  
#define GDFLIB_FilterIIR2_FLT(fltInX, psParam)                                 \
        GDFLIB_FilterIIR2_FLT_CRam(fltInX, psParam) 
#define GDFLIB_FilterIIR3Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR3Init_FLT_CRam(psParam)                                  
#define GDFLIB_FilterIIR3_FLT(fltInX, psParam)                                 \
        GDFLIB_FilterIIR3_FLT_CRam(fltInX, psParam)                              
#define GDFLIB_FilterIIR4Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR4Init_FLT_CRam(psParam)                                  
#define GDFLIB_FilterIIR4_FLT(fltInX, psParam)                                 \
        GDFLIB_FilterIIR4_FLT_CRam(fltInX, psParam)                             
#else /* placed to ROM */
#define GDFLIB_FilterIIR2Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR2Init_FLT_C(psParam)                                  
#define GDFLIB_FilterIIR2_FLT(fltInX, psParam)                                 \
        GDFLIB_FilterIIR2_FLT_C(fltInX, psParam) 
#define GDFLIB_FilterIIR3Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR3Init_FLT_C(psParam)                                  
#define GDFLIB_FilterIIR3_FLT(fltInX, psParam)                                 \
        GDFLIB_FilterIIR3_FLT_C(fltInX, psParam)                              
#define GDFLIB_FilterIIR4Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR4Init_FLT_C(psParam)                                  
#define GDFLIB_FilterIIR4_FLT(fltInX, psParam)                                 \
        GDFLIB_FilterIIR4_FLT_C(fltInX, psParam)    
#endif /* defined(RAM_RELOCATION) */

#define GDFLIB_FilterExpInit_FLT(fltInitVal, psParam)                          \
        GDFLIB_FilterExpInit_FLT_Ci(fltInitVal, psParam)                       
#define GDFLIB_FilterExp_FLT(fltInX, psParam)                                  \
        GDFLIB_FilterExp_FLT_Ci(fltInX, psParam) 
#define GDFLIB_FilterIIR1Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR1Init_FLT_Ci(psParam)
#define GDFLIB_FilterIIR1_FLT(fltInX, psParam)                                 \
        GDFLIB_FilterIIR1_FLT_Ci(fltInX, psParam)
#define GDFLIB_FilterMAInit_FLT(fltInitVal, psParam)                           \
        GDFLIB_FilterMAInit_FLT_Ci(fltInitVal, psParam)                        
#define GDFLIB_FilterMA_FLT(fltInX, psParam)                                   \
        GDFLIB_FilterMA_FLT_Ci(fltInX, psParam) 
        
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FP_H_ */
