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
* @brief  Main GDFLIB header file for devices without FPU.
* 
*******************************************************************************/
#ifndef _GDFLIB_H_
#define _GDFLIB_H_

#if defined(__cplusplus)
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "GDFLIB_FilterExp_F32.h" 
#include "GDFLIB_FilterIIR1_F32.h"
#include "GDFLIB_FilterIIR1_F32_Asmi.h"
#include "GDFLIB_FilterIIR2_F32.h"
#include "GDFLIB_FilterIIR3_F32.h"
#include "GDFLIB_FilterIIR4_F32.h"
#include "GDFLIB_FilterMA_A32.h"
   
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterExpInit_F16(f16InX, psParam)                              \
        GDFLIB_FilterExpInit_F16_Ci(f16InX, psParam)             
#define GDFLIB_FilterExp_F16(f16InX, psParam)                                  \
        GDFLIB_FilterExp_F16_Ci(f16InX, psParam)    
#define GDFLIB_FilterIIR1Init_F16(psParam)                                     \
        GDFLIB_FilterIIR1Init_F16_Ci(psParam)                                  
#define GDFLIB_FilterIIR1_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR1_F16_Asmi(f16InX, psParam)
#define GDFLIB_FilterIIR3Init_F16(psParam)                                     \
        GDFLIB_FilterIIR3Init_F16_C(psParam)                                  
#define GDFLIB_FilterIIR3_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR3_F16_C(f16InX, psParam)                              
#define GDFLIB_FilterIIR4Init_F16(psParam)                                     \
        GDFLIB_FilterIIR4Init_F16_C(psParam)                                  
#define GDFLIB_FilterIIR4_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR4_F16_C(f16InX, psParam)             
#define GDFLIB_FilterMAInit_F16(f16InitVal, psParam)                           \
        GDFLIB_FilterMAInit_F16_Ci(f16InitVal, psParam)                        
#define GDFLIB_FilterMA_F16(f16InX, psParam)                                   \
        GDFLIB_FilterMA_F16_Ci(f16InX, psParam)                   
         
#if (RTCESL_PQ)
#else /* RTCESL_PQ */ 
  #define GDFLIB_FilterIIR2Init_F16(psParam)                                   \
          GDFLIB_FilterIIR2Init_F16_C(psParam)                                 
  #define GDFLIB_FilterIIR2_F16(f16InX, psParam)                               \
          GDFLIB_FilterIIR2_F16_Asm(f16InX, psParam)
#endif /* RTCESL_PQ */   
          
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_H_ */
