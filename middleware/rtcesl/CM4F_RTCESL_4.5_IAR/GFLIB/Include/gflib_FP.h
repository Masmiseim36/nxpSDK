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
* @brief  Main GFLIB header file for devices with FPU.
* 
*******************************************************************************/
#ifndef _GFLIB_FP_H_
#define _GFLIB_FP_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/ 
#include "gflib.h"
#include "GFLIB_Acos_FLT.h" 
#include "GFLIB_Asin_FLT.h" 
#include "GFLIB_AtanYX_FLT.h" 
#include "GFLIB_Atan_FLT.h" 
#include "GFLIB_CtrlBetaIPp_FLT.h" 
#include "GFLIB_CtrlBetaIPDp_FLT.h"      
#include "GFLIB_CtrlPIDp_FLT.h"
#include "GFLIB_CtrlPIp_FLT.h"     
#include "GFLIB_DFlexRamp_FLT.h"   
#include "GFLIB_FlexRamp_FLT.h" 
#include "GFLIB_FlexSRamp_FLT.h"      
#include "GFLIB_DRamp_FLT.h"
#include "GFLIB_Hyst_FLT.h"   
#include "GFLIB_Integrator_FLT.h"  
#include "GFLIB_Limit_FLT.h" 
#include "GFLIB_LowerLimit_FLT.h"
#include "GFLIB_Lut_FLT.h"  
#include "GFLIB_LutPer_FLT.h"
#include "GFLIB_Ramp_FLT.h"
#include "GFLIB_SinCos_FLT.h"
#include "GFLIB_Sqrt_FLT_Asmi.h"     
#include "GFLIB_Tan_FLT.h"    
#include "GFLIB_UpperLimit_FLT.h"  
#include "GFLIB_VectorLimit_FLT.h"  

/*******************************************************************************
* Macros
*******************************************************************************/  
#define GFLIB_Acos_FLT(fltVal)                                                GFLIB_Acos_FLT_C(fltVal)
#define GFLIB_Asin_FLT(fltVal)                                                GFLIB_Asin_FLT_C(fltVal)
#define GFLIB_AtanYX_A32f(fltY, fltX, pbErrFlag)                              GFLIB_AtanYX_A32f_C(fltY, fltX, pbErrFlag)
#define GFLIB_AtanYX_FLT(fltY, fltX, pbErrFlag)                               GFLIB_AtanYX_FLT_C(fltY, fltX, pbErrFlag)
#define GFLIB_Atan_A32f(fltVal)                                               GFLIB_Atan_A32f_C(fltVal)
#define GFLIB_Atan_FLT(fltVal)                                                GFLIB_Atan_FLT_C(fltVal)
#define GFLIB_Cos_FLT(fltAngle)                                               GFLIB_Cos_FLT_C(fltAngle)
#define GFLIB_Cos_FLTa(a32AngleExt)                                           GFLIB_Cos_FLTa_C(a32AngleExt)
#define GFLIB_CtrlBetaIPDpAWInit_FLT(fltInitVal, psParam)                     GFLIB_CtrlBetaIPDpAWInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_CtrlBetaIPDpAW_FLT(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam)              GFLIB_CtrlBetaIPDpAW_FLT_C(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlBetaIPpAWInit_FLT(fltInitVal, psParam)                      GFLIB_CtrlBetaIPpAWInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_CtrlBetaIPpAW_FLT(fltInReq, fltIn, pbStopIntegFlag, psParam)    GFLIB_CtrlBetaIPpAW_FLT_C(fltInReq, fltIn, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlPIDpAWInit_FLT(fltInitVal, psParam)                         GFLIB_CtrlPIDpAWInit_FLT_Ci(fltInitVal, psParam)   
#define GFLIB_CtrlPIDpAW_FLT(fltInErr, fltInErrD, pbStopIntegFlag, psParam)   GFLIB_CtrlPIDpAW_FLT_C(fltInErr, fltInErrD, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlPIpAWInit_FLT(fltInitVal, psParam)                          GFLIB_CtrlPIpAWInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_CtrlPIpAW_FLT(fltInErr, pbStopIntegFlag, psParam)               GFLIB_CtrlPIpAW_FLT_C(fltInErr, pbStopIntegFlag, psParam)    
#define GFLIB_DFlexRampCalcIncr_FLT(fltTarget, fltDuration, f32IncrSatMot, f32IncrSatGen, psParam)  GFLIB_DFlexRampCalcIncr_FLT_C(fltTarget, fltDuration, f32IncrSatMot, f32IncrSatGen, psParam)
#define GFLIB_DFlexRampInit_FLT(fltInitVal, psParam)                                                GFLIB_DFlexRampInit_FLT_C(fltInitVal, psParam)
#define GFLIB_DFlexRamp_FLT(fltInstant, pbStopFlagMot, pbStopFlagGen,psParam)                       GFLIB_DFlexRamp_FLT_C(fltInstant, pbStopFlagMot, pbStopFlagGen, psParam)
#define GFLIB_DRampInit_FLT(fltInitVal, psParam)                              GFLIB_DRampInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_DRamp_FLT(fltTarget, fltInstant, pbStopFlag, psParam)           GFLIB_DRamp_FLT_C(fltTarget, fltInstant, pbStopFlag, psParam)
#define GFLIB_FlexRampCalcIncr_FLT(fltTarget, fltDuration, psParam)           GFLIB_FlexRampCalcIncr_FLT_C(fltTarget, fltDuration, psParam)
#define GFLIB_FlexRampInit_FLT(fltInitVal, psParam)                           GFLIB_FlexRampInit_FLT_C(fltInitVal, psParam)
#define GFLIB_FlexRamp_FLT(psParam)                                           GFLIB_FlexRamp_FLT_C(psParam)
#define GFLIB_FlexSRampCalcIncr_FLT(fltTarget, fltDuration, psParam)          GFLIB_FlexSRampCalcIncr_FLT_C(fltTarget, fltDuration, psParam)
#define GFLIB_FlexSRampInit_FLT(fltInitVal, psParam)                          GFLIB_FlexSRampInit_FLT_C(fltInitVal, psParam)
#define GFLIB_FlexSRamp_FLT(psParam)                                          GFLIB_FlexSRamp_FLT_C(psParam)
#define GFLIB_Hyst_FLT(fltVal, psParam)                                       GFLIB_Hyst_FLT_Ci(fltVal, psParam)    
#define GFLIB_IntegratorInit_FLT(fltInitVal, psParam)                         GFLIB_IntegratorInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_Integrator_FLT(fltInVal, psParam)                               GFLIB_Integrator_FLT_Ci(fltInVal, psParam)  
#define GFLIB_Limit_FLT(fltVal, fltLLim, fltULim)                             GFLIB_Limit_FLT_Ci(fltVal, fltLLim, fltULim)
#define GFLIB_LowerLimit_FLT(fltVal, fltLLim)                                 GFLIB_LowerLimit_FLT_Ci(fltVal, fltLLim)
#define GFLIB_Lut1DInit_FLT(fltMin, fltMax, pfltTable, psParam)               GFLIB_Lut1DInit_FLT_Ci(fltMin, fltMax, pfltTable, psParam)
#define GFLIB_Lut1D_FLT(fltX, pfltTable, psParam)                             GFLIB_Lut1D_FLT_C(fltX, pfltTable, psParam)
#define GFLIB_LutPer1DInit_FLT(fltMin, fltMax, pfltTable, psParam)            GFLIB_LutPer1DInit_FLT_Ci(fltMin, fltMax, pfltTable, psParam)
#define GFLIB_LutPer1D_FLT(fltX, pfltTable, psParam)                          GFLIB_LutPer1D_FLT_C(fltX, pfltTable, psParam)
#define GFLIB_RampInit_FLT(fltInitVal, psParam)                               GFLIB_RampInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_Ramp_FLT(fltTarget, psParam)                                    GFLIB_Ramp_FLT_Ci(fltTarget, psParam)  
#define GFLIB_Sin_FLT(fltAngle)                                               GFLIB_Sin_FLT_C(fltAngle)
#define GFLIB_Sin_FLTa(a32AngleExt)                                           GFLIB_Sin_FLTa_C(a32AngleExt)
#define GFLIB_Sqrt_FLT(fltVal)                                                GFLIB_Sqrt_FLT_Asmi(fltVal)  
#define GFLIB_Tan_FLT(fltAngle)                                               GFLIB_Tan_FLT_C(fltAngle)
#define GFLIB_Tan_FLTa(a32AngleExt)                                           GFLIB_Tan_FLTa_C(a32AngleExt)
#define GFLIB_UpperLimit_FLT(fltVal, fltULim)                                 GFLIB_UpperLimit_FLT_Ci(fltVal, fltULim)  
#define GFLIB_VectorLimit_FLT(psVectorIn, fltLim, psVectorOut)                GFLIB_VectorLimit_FLT_C(psVectorIn, fltLim, psVectorOut) 
#define GFLIB_VectorLimit1_FLT(psVectorIn, fltLim, psVectorOut)               GFLIB_VectorLimit1_FLT_C(psVectorIn, fltLim, psVectorOut)  
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_FP_H_ */  
