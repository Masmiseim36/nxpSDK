/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
#define GDFLIB_FilterExpInit_FLT(fltInitVal, psParam)                          \
        GDFLIB_FilterExpInit_FLT_Ci(fltInitVal, psParam)                       
#define GDFLIB_FilterExp_FLT(fltInX, psParam)                                  \
        GDFLIB_FilterExp_FLT_Ci(fltInX, psParam) 
#define GDFLIB_FilterIIR1Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR1Init_FLT_Ci(psParam)
#define GDFLIB_FilterIIR1_FLT(fltInX, psParam)                                 \
        GDFLIB_FilterIIR1_FLT_Ci(fltInX, psParam)                              
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
#define GDFLIB_FilterMAInit_FLT(fltInitVal, psParam)                           \
        GDFLIB_FilterMAInit_FLT_Ci(fltInitVal, psParam)                        
#define GDFLIB_FilterMA_FLT(fltInX, psParam)                                   \
        GDFLIB_FilterMA_FLT_Ci(fltInX, psParam)                 
            
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FP_H_ */
