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
* @brief Main MLIB header file for devices with FPU. 
* 
*******************************************************************************/
#ifndef _MLIB_FP_H_
#define _MLIB_FP_H_

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include "mlib.h"
#include "MLIB_Abs_FLT.h"
#include "MLIB_Abs_FLT_Asmi.h"
#include "MLIB_Add4_FLT.h"
#include "MLIB_Add_FLT.h"
#include "MLIB_ConvSc_FLT_Asmi.h"
#include "MLIB_Conv_FLT_Asmi.h"
#include "MLIB_Div_FLT.h"
#include "MLIB_Mac4_FLT.h"
#include "MLIB_Mac_FLT.h"
#include "MLIB_Mnac_FLT.h"    
#include "MLIB_Msu4_FLT.h"
#include "MLIB_Msu_FLT.h"
#include "MLIB_Mul_FLT.h"
#include "MLIB_Neg_FLT.h"
#include "MLIB_Sign_FLT_Asmi.h"
#include "MLIB_Sub4_FLT.h"  
#include "MLIB_Sub_FLT.h"

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_Abs_FLT(fltVal)                                               MLIB_Abs_FLT_Asmi(fltVal)
#define MLIB_Add4_FLT(fltAdd1, fltAdd2, fltAdd3, fltAdd4)                  MLIB_Add4_FLT_Ci(fltAdd1, fltAdd2, fltAdd3, fltAdd4) 
#define MLIB_Add_FLT(fltAdd1, fltAdd2)                                     MLIB_Add_FLT_Ci(fltAdd1, fltAdd2)  
#define MLIB_ConvSc_F16ff(fltVal, fltSc)                                   MLIB_ConvSc_F16ff_Asmi(fltVal, fltSc)
#define MLIB_ConvSc_F32ff(fltVal, fltSc)                                   MLIB_ConvSc_F32ff_Asmi(fltVal, fltSc)
#define MLIB_ConvSc_FLTlf(f32Val, fltSc)                                   MLIB_ConvSc_FLTlf_Asmi(f32Val, fltSc)
#define MLIB_ConvSc_FLTsf(f16Val, fltSc)                                   MLIB_ConvSc_FLTsf_Asmi(f16Val, fltSc)
#define MLIB_ConvSc_A32ff(fltVal, fltSc)                                   MLIB_ConvSc_A32ff_Asmi(fltVal, fltSc)
#define MLIB_ConvSc_FLTaf(a32Val, fltSc)                                   MLIB_ConvSc_FLTaf_Asmi(a32Val, fltSc)
#define MLIB_Conv_F16f(fltVal)                                             MLIB_Conv_F16f_Asmi(fltVal)   
#define MLIB_Conv_F32f(fltVal)                                             MLIB_Conv_F32f_Asmi(fltVal)        
#define MLIB_Conv_FLTl(f32Val)                                             MLIB_Conv_FLTl_Asmi(f32Val)
#define MLIB_Conv_FLTs(f16Val)                                             MLIB_Conv_FLTs_Asmi(f16Val)
#define MLIB_Conv_A32f(fltVal)                                             MLIB_Conv_A32f_Asmi(fltVal)
#define MLIB_Conv_FLTa(a32Val)                                             MLIB_Conv_FLTa_Asmi(a32Val)
#define MLIB_Div_FLT(fltNum, fltDenom)                                     MLIB_Div_FLT_Ci(fltNum, fltDenom)  
#define MLIB_Mac4_FLT(fltAdd1Mul1, fltAdd1Mul2, fltAdd2Mul1, fltAdd2Mul2)  MLIB_Mac4_FLT_Ci(fltAdd1Mul1, fltAdd1Mul2, fltAdd2Mul1, fltAdd2Mul2)   
#define MLIB_Mac_FLT(fltAccum, fltMult1, fltMult2)                         MLIB_Mac_FLT_Ci(fltAccum, fltMult1, fltMult2)
#define MLIB_Mnac_FLT(fltAccum, fltMult1, fltMult2)                        MLIB_Mnac_FLT_Ci(fltAccum, fltMult1, fltMult2)
#define MLIB_Msu4_FLT(fltMinMul1, fltMinMul2, fltSubMul1, fltSubMul2)      MLIB_Msu4_FLT_Ci(fltMinMul1, fltMinMul2, fltSubMul1, fltSubMul2) 
#define MLIB_Msu_FLT(fltAccum, fltMult1, fltMult2)                         MLIB_Msu_FLT_Ci(fltAccum, fltMult1, fltMult2)
#define MLIB_MulNeg_FLT( fltAccum, fltMult)                                MLIB_MulNeg_FLT_Ci( fltAccum, fltMult)
#define MLIB_Mul_FLT(fltAccum, fltMult)                                    MLIB_Mul_FLT_Ci(fltAccum, fltMult) 
#define MLIB_Neg_FLT(fltVal)                                               MLIB_Neg_FLT_Ci(fltVal) 
#define MLIB_Sign_FLT(fltVal)                                              MLIB_Sign_FLT_Asmi(fltVal)
#define MLIB_Sub4_FLT(fltMin, fltSub1, fltSub2, fltSub3)                   MLIB_Sub4_FLT_Ci(fltMin, fltSub1, fltSub2, fltSub3) 
#define MLIB_Sub_FLT(fltMin, fltSub)                                       MLIB_Sub_FLT_Ci(fltMin, fltSub) 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_FP_H_ */
