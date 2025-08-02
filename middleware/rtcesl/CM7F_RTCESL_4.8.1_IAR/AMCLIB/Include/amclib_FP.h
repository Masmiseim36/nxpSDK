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
* @brief Main AMCLIB header file for devices with FPU. 
* 
*******************************************************************************/
#ifndef _AMCLIB_FP_H_
#define _AMCLIB_FP_H_

#if defined(__cplusplus) 
extern "C" {
#endif  
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "amclib.h"
#include "AMCLIB_ACIMCtrlMTPA_FLT.h" 
#include "AMCLIB_ACIMRotFluxObsrv_FLT.h"
#include "AMCLIB_ACIMSpeedMRAS_FLT.h"
#include "AMCLIB_AngleTrackObsrv_FLT.h"
#include "AMCLIB_CtrlFluxWkng_FLT.h"
#include "AMCLIB_PMSMBemfObsrvAB_FLT.h"
#include "AMCLIB_PMSMBemfObsrvDQ_FLT.h"
#include "AMCLIB_TrackObsrv_FLT.h"

/*******************************************************************************
* Macros 
*******************************************************************************/           
#if (defined(RAM_RELOCATION)) /* placed to RAM */
#define AMCLIB_ACIMCtrlMTPA_FLT(fltIq, psCtrl)                                      \
        AMCLIB_ACIMCtrlMTPA_FLT_CRam(fltIq, psCtrl)                              
#define AMCLIB_ACIMRotFluxObsrv_FLT(psIsAlBe, sUSAlBe, psCtrl)                      \
        AMCLIB_ACIMRotFluxObsrv_FLT_CRam(psIsAlBe, sUSAlBe, psCtrl )               
#define AMCLIB_ACIMSpeedMRAS_FLT(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)          \
        AMCLIB_ACIMSpeedMRAS_FLT_CRam(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)
#define AMCLIB_AngleTrackObsrvInit_A32(a32ThetaInit, psCtrl)                        \
        AMCLIB_AngleTrackObsrvInit_A32_CRam(a32ThetaInit, psCtrl)          
#define AMCLIB_AngleTrackObsrv_A32ff(psSinCos, psCtrl)                              \
        AMCLIB_AngleTrackObsrv_A32ff_CRam(psSinCos, psCtrl)                        
#define AMCLIB_CtrlFluxWkng_FLT(fltIQErr, fltUQReq, fltUQLim, psCtrl)               \
        AMCLIB_CtrlFluxWkng_FLT_CRam(fltIQErr, fltUQReq, fltUQLim, psCtrl) 
#define AMCLIB_PMSMBemfObsrvDQ_A32fff(psIDQ, psUDQ, fltSpeed, psCtrl)               \
        AMCLIB_PMSMBemfObsrvDQ_A32fff_CRam(psIDQ, psUDQ, fltSpeed, psCtrl)        
#define AMCLIB_PMSMBemfObsrvAB_FLT(psIAlBe, psUAlBe, fltSpeed, psCtrl)              \
        AMCLIB_PMSMBemfObsrvAB_FLT_CRam(psIAlBe, psUAlBe, fltSpeed, psCtrl)
#define AMCLIB_TrackObsrv_A32af(a32ThetaErr, psCtrl)                                \
        AMCLIB_TrackObsrv_A32af_CRam(a32ThetaErr, psCtrl)	
#else  /* placed to ROM */
#define AMCLIB_ACIMCtrlMTPA_FLT(fltIq, psCtrl)                                    \
        AMCLIB_ACIMCtrlMTPA_FLT_C(fltIq, psCtrl)                              
#define AMCLIB_ACIMRotFluxObsrv_FLT(psIsAlBe, pUSAlBe, psCtrl)                    \
        AMCLIB_ACIMRotFluxObsrv_FLT_C(psIsAlBe, pUSAlBe, psCtrl )               
#define AMCLIB_ACIMSpeedMRAS_FLT(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)        \
        AMCLIB_ACIMSpeedMRAS_FLT_C(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)
#define AMCLIB_AngleTrackObsrvInit_A32(a32ThetaInit, psCtrl)                      \
        AMCLIB_AngleTrackObsrvInit_A32_C(a32ThetaInit, psCtrl)         
#define AMCLIB_AngleTrackObsrv_A32ff(psSinCos, psCtrl)                            \
        AMCLIB_AngleTrackObsrv_A32ff_C(psSinCos, psCtrl)                        
#define AMCLIB_CtrlFluxWkng_FLT(fltIQErr, fltUQReq, fltUQLim, psCtrl)             \
        AMCLIB_CtrlFluxWkng_FLT_C(fltIQErr, fltUQReq, fltUQLim, psCtrl)         
#define AMCLIB_EstimRLInit_FLT(u32SamplingFreq, psParam, psCtrl)                  \
        AMCLIB_EstimRLInit_FLT_C(u32SamplingFreq, psParam, psCtrl)          
#define AMCLIB_EstimRL_FLT(fltUDcBus, psIAlBeFbck, psCtrl, psParam, psUAlBeReq)   \
        AMCLIB_EstimRL_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, psParam, psUAlBeReq)
#define AMCLIB_PMSMBemfObsrvDQ_A32fff(psIDQ, psUDQ, fltSpeed, psCtrl)             \
        AMCLIB_PMSMBemfObsrvDQ_A32fff_C(psIDQ, psUDQ, fltSpeed, psCtrl)        
#define AMCLIB_PMSMBemfObsrvAB_FLT(psIAlBe, psUAlBe, fltSpeed, psCtrl)            \
        AMCLIB_PMSMBemfObsrvAB_FLT_C(psIAlBe, psUAlBe, fltSpeed, psCtrl)
#define AMCLIB_TrackObsrv_A32af(a32ThetaErr, psCtrl)                              \
        AMCLIB_TrackObsrv_A32af_C(a32ThetaErr, psCtrl)		
#endif /* defined(RAM_RELOCATION) */
                       
#define AMCLIB_ACIMCtrlMTPAInit_FLT(fltIDMin, fltIDMax, psCtrl)                \
        AMCLIB_ACIMCtrlMTPAInit_FLT_Ci(fltIDMin, fltIDMax, psCtrl)              
#define AMCLIB_ACIMRotFluxObsrvInit_FLT(psCtrl)                                \
        AMCLIB_ACIMRotFluxObsrvInit_FLT_Ci(psCtrl)                              
#define AMCLIB_ACIMSpeedMRASInit_FLT(psCtrl)                                   \
        AMCLIB_ACIMSpeedMRASInit_FLT_Ci(psCtrl)                               
#define AMCLIB_CtrlFluxWkngInit_FLT(fltInitVal, psCtrl)                        \
        AMCLIB_CtrlFluxWkngInit_FLT_Ci(fltInitVal, psCtrl)             
#define AMCLIB_PMSMBemfObsrvDQInit_A32fff(psCtrl)                              \
        AMCLIB_PMSMBemfObsrvDQInit_A32fff_Ci(psCtrl)  
#define AMCLIB_PMSMBemfObsrvABInit_FLT(psCtrl)                                 \
        AMCLIB_PMSMBemfObsrvABInit_FLT_Ci( psCtrl)                 
#define AMCLIB_TrackObsrvInit_A32af(a32ThetaInit, psCtrl)                      \
        AMCLIB_TrackObsrvInit_A32af_Ci(a32ThetaInit, psCtrl)

#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_FP_H_ */ 
