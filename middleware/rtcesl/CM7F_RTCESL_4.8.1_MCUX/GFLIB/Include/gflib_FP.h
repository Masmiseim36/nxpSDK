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
#if (defined(RAM_RELOCATION))
#define GFLIB_Acos_FLT(fltVal)                                        GFLIB_Acos_FLT_CRam(fltVal)
#define GFLIB_Asin_FLT(fltVal)                                        GFLIB_Asin_FLT_CRam(fltVal)
#define GFLIB_AtanYX_A32f(fltY, fltX, pbErrFlag)                      GFLIB_AtanYX_A32f_CRam(fltY, fltX, pbErrFlag)
#define GFLIB_AtanYX_FLT(fltY, fltX, pbErrFlag)                       GFLIB_AtanYX_FLT_CRam(fltY, fltX, pbErrFlag)
#define GFLIB_Atan_A32f(fltVal)                                       GFLIB_Atan_A32f_CRam(fltVal)
#define GFLIB_Atan_FLT(fltVal)                                        GFLIB_Atan_FLT_CRam(fltVal)
#define GFLIB_Cos_FLT(fltAngle)                                       GFLIB_Cos_FLT_CRam(fltAngle)
#define GFLIB_Cos_FLTa(a32AngleExt)                                   GFLIB_Cos_FLTa_CRam(a32AngleExt)
#define GFLIB_CtrlBetaIPDpAW_FLT(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam) GFLIB_CtrlBetaIPDpAW_FLT_CRam(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlBetaIPpAW_FLT(fltInReq, fltIn, pbStopIntegFlag, psParam)    GFLIB_CtrlBetaIPpAW_FLT_CRam(fltInReq, fltIn, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlPIDpAW_FLT(fltInErr, fltInErrD, pbStopIntegFlag, psParam)   GFLIB_CtrlPIDpAW_FLT_CRam(fltInErr, fltInErrD, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlPIpAW_FLT(fltInErr, pbStopIntegFlag, psParam)               GFLIB_CtrlPIpAW_FLT_CRam(fltInErr, pbStopIntegFlag, psParam)    
#define GFLIB_DFlexRampCalcIncr_FLT(fltTarget, fltDuration, f32IncrSatMot, f32IncrSatGen, psParam)  GFLIB_DFlexRampCalcIncr_FLT_CRam(fltTarget, fltDuration, f32IncrSatMot, f32IncrSatGen, psParam)
#define GFLIB_DFlexRamp_FLT(fltInstant, pbStopFlagMot, pbStopFlagGen,psParam) GFLIB_DFlexRamp_FLT_CRam(fltInstant, pbStopFlagMot, pbStopFlagGen, psParam)
#define GFLIB_DRamp_FLT(fltTarget, fltInstant, pbStopFlag, psParam)   GFLIB_DRamp_FLT_CRam(fltTarget, fltInstant, pbStopFlag, psParam)
#define GFLIB_FlexRampCalcIncr_FLT(fltTarget, fltDuration, psParam)   GFLIB_FlexRampCalcIncr_FLT_CRam(fltTarget, fltDuration, psParam)
#define GFLIB_FlexRamp_FLT(psParam)                                   GFLIB_FlexRamp_FLT_CRam(psParam)
#define GFLIB_FlexSRampCalcIncr_FLT(fltTarget, fltDuration, psParam)  GFLIB_FlexSRampCalcIncr_FLT_CRam(fltTarget, fltDuration, psParam)
#define GFLIB_FlexSRamp_FLT(psParam)                                  GFLIB_FlexSRamp_FLT_CRam(psParam)
#define GFLIB_Lut1D_FLT(fltX, pfltTable, psParam)                     GFLIB_Lut1D_FLT_CRam(fltX, pfltTable, psParam)
#define GFLIB_LutPer1D_FLT(fltX, pfltTable, psParam)                  GFLIB_LutPer1D_FLT_CRam(fltX, pfltTable, psParam)
#define GFLIB_Sin_FLT(fltAngle)                                       GFLIB_Sin_FLT_CRam(fltAngle)
#define GFLIB_Sin_FLTa(a32AngleExt)                                   GFLIB_Sin_FLTa_CRam(a32AngleExt)
#define GFLIB_Tan_FLT(fltAngle)                                       GFLIB_Tan_FLT_CRam(fltAngle)
#define GFLIB_Tan_FLTa(a32AngleExt)                                   GFLIB_Tan_FLTa_CRam(a32AngleExt)
#define GFLIB_VectorLimit1_FLT(psVectorIn, fltLim, psVectorOut)       GFLIB_VectorLimit1_FLT_CRam(psVectorIn, fltLim, psVectorOut)  
#define GFLIB_VectorLimit_FLT(psVectorIn, fltLim, psVectorOut)        GFLIB_VectorLimit_FLT_CRam(psVectorIn, fltLim, psVectorOut) 
#else /* placed to ROM */
#define GFLIB_Acos_FLT(fltVal)                                        GFLIB_Acos_FLT_C(fltVal)
#define GFLIB_Asin_FLT(fltVal)                                        GFLIB_Asin_FLT_C(fltVal)
#define GFLIB_AtanYX_A32f(fltY, fltX, pbErrFlag)                      GFLIB_AtanYX_A32f_C(fltY, fltX, pbErrFlag)
#define GFLIB_AtanYX_FLT(fltY, fltX, pbErrFlag)                       GFLIB_AtanYX_FLT_C(fltY, fltX, pbErrFlag)
#define GFLIB_Atan_A32f(fltVal)                                       GFLIB_Atan_A32f_C(fltVal)
#define GFLIB_Atan_FLT(fltVal)                                        GFLIB_Atan_FLT_C(fltVal)
#define GFLIB_Cos_FLT(fltAngle)                                       GFLIB_Cos_FLT_C(fltAngle)
#define GFLIB_Cos_FLTa(a32AngleExt)                                   GFLIB_Cos_FLTa_C(a32AngleExt)
#define GFLIB_CtrlBetaIPDpAW_FLT(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam) GFLIB_CtrlBetaIPDpAW_FLT_C(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlBetaIPpAW_FLT(fltInReq, fltIn, pbStopIntegFlag, psParam)  GFLIB_CtrlBetaIPpAW_FLT_C(fltInReq, fltIn, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlPIDpAW_FLT(fltInErr, fltInErrD, pbStopIntegFlag, psParam) GFLIB_CtrlPIDpAW_FLT_C(fltInErr, fltInErrD, pbStopIntegFlag, psParam)    
#define GFLIB_CtrlPIpAW_FLT(fltInErr, pbStopIntegFlag, psParam)       GFLIB_CtrlPIpAW_FLT_C(fltInErr, pbStopIntegFlag, psParam)    
#define GFLIB_DFlexRampCalcIncr_FLT(fltTarget, fltDuration, f32IncrSatMot, f32IncrSatGen, psParam) GFLIB_DFlexRampCalcIncr_FLT_C(fltTarget, fltDuration, f32IncrSatMot, f32IncrSatGen, psParam)
#define GFLIB_DFlexRamp_FLT(fltInstant, pbStopFlagMot, pbStopFlagGen,psParam) GFLIB_DFlexRamp_FLT_C(fltInstant, pbStopFlagMot, pbStopFlagGen, psParam)
#define GFLIB_DRamp_FLT(fltTarget, fltInstant, pbStopFlag, psParam)   GFLIB_DRamp_FLT_C(fltTarget, fltInstant, pbStopFlag, psParam)
#define GFLIB_FlexRampCalcIncr_FLT(fltTarget, fltDuration, psParam)   GFLIB_FlexRampCalcIncr_FLT_C(fltTarget, fltDuration, psParam)
#define GFLIB_FlexRamp_FLT(psParam)                                   GFLIB_FlexRamp_FLT_C(psParam)
#define GFLIB_FlexSRampCalcIncr_FLT(fltTarget, fltDuration, psParam)  GFLIB_FlexSRampCalcIncr_FLT_C(fltTarget, fltDuration, psParam)
#define GFLIB_FlexSRamp_FLT(psParam)                                  GFLIB_FlexSRamp_FLT_C(psParam)
#define GFLIB_Lut1D_FLT(fltX, pfltTable, psParam)                     GFLIB_Lut1D_FLT_C(fltX, pfltTable, psParam)
#define GFLIB_LutPer1D_FLT(fltX, pfltTable, psParam)                  GFLIB_LutPer1D_FLT_C(fltX, pfltTable, psParam)
#define GFLIB_Sin_FLT(fltAngle)                                       GFLIB_Sin_FLT_C(fltAngle)
#define GFLIB_Sin_FLTa(a32AngleExt)                                   GFLIB_Sin_FLTa_C(a32AngleExt)
#define GFLIB_SinCos_FLTa(a32AngleExt, fltSinCos)                     GFLIB_SinCos_FLTa_Ci(a32AngleExt, fltSinCos)
#define GFLIB_Tan_FLT(fltAngle)                                       GFLIB_Tan_FLT_C(fltAngle)
#define GFLIB_Tan_FLTa(a32AngleExt)                                   GFLIB_Tan_FLTa_C(a32AngleExt)
#define GFLIB_VectorLimit1_FLT(psVectorIn, fltLim, psVectorOut)       GFLIB_VectorLimit1_FLT_C(psVectorIn, fltLim, psVectorOut)
#define GFLIB_VectorLimit_FLT(psVectorIn, fltLim, psVectorOut)        GFLIB_VectorLimit_FLT_C(psVectorIn, fltLim, psVectorOut) 
#endif /*(defined(RAM_RELOCATION))*/

#define GFLIB_CtrlBetaIPDpAWInit_FLT(fltInitVal, psParam)             GFLIB_CtrlBetaIPDpAWInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_CtrlBetaIPpAWInit_FLT(fltInitVal, psParam)              GFLIB_CtrlBetaIPpAWInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_CtrlPIDpAWInit_FLT(fltInitVal, psParam)                 GFLIB_CtrlPIDpAWInit_FLT_Ci(fltInitVal, psParam)   
#define GFLIB_CtrlPIpAWInit_FLT(fltInitVal, psParam)                  GFLIB_CtrlPIpAWInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_DFlexRampInit_FLT(fltInitVal, psParam)                  GFLIB_DFlexRampInit_FLT_C(fltInitVal, psParam)
#define GFLIB_DRampInit_FLT(fltInitVal, psParam)                      GFLIB_DRampInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_FlexRampInit_FLT(fltInitVal, psParam)                   GFLIB_FlexRampInit_FLT_C(fltInitVal, psParam)
#define GFLIB_FlexSRampInit_FLT(fltInitVal, psParam)                  GFLIB_FlexSRampInit_FLT_C(fltInitVal, psParam)
#define GFLIB_Hyst_FLT(fltVal, psParam)                               GFLIB_Hyst_FLT_Ci(fltVal, psParam)    
#define GFLIB_IntegratorInit_FLT(fltInitVal, psParam)                 GFLIB_IntegratorInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_Integrator_FLT(fltInVal, psParam)                       GFLIB_Integrator_FLT_Ci(fltInVal, psParam)  
#define GFLIB_Limit_FLT(fltVal, fltLLim, fltULim)                     GFLIB_Limit_FLT_Ci(fltVal, fltLLim, fltULim)
#define GFLIB_LowerLimit_FLT(fltVal, fltLLim)                         GFLIB_LowerLimit_FLT_Ci(fltVal, fltLLim)
#define GFLIB_Lut1DInit_FLT(fltMin, fltMax, pfltTable, psParam)       GFLIB_Lut1DInit_FLT_Ci(fltMin, fltMax, pfltTable, psParam)
#define GFLIB_LutPer1DInit_FLT(fltMin, fltMax, pfltTable, psParam)    GFLIB_LutPer1DInit_FLT_Ci(fltMin, fltMax, pfltTable, psParam)
#define GFLIB_RampInit_FLT(fltInitVal, psParam)                       GFLIB_RampInit_FLT_Ci(fltInitVal, psParam)
#define GFLIB_Ramp_FLT(fltTarget, psParam)                            GFLIB_Ramp_FLT_Ci(fltTarget, psParam)  
#define GFLIB_SinCos_FLTa(a32AngleExt, fltSinCos)                     GFLIB_SinCos_FLTa_Ci(a32AngleExt, fltSinCos)
#define GFLIB_Sqrt_FLT(fltVal)                                        GFLIB_Sqrt_FLT_Asmi(fltVal)  
#define GFLIB_UpperLimit_FLT(fltVal, fltULim)                         GFLIB_UpperLimit_FLT_Ci(fltVal, fltULim)  

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_FP_H_ */  
