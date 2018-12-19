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
#include "AMCLIB_PMSMBemfObsrvDQ_FLT.h"
#include "AMCLIB_PMSMBemfObsrvAB_FLT.h"    
#include "AMCLIB_TrackObsrv_FLT.h" 

/*******************************************************************************
* Macros 
*******************************************************************************/           
#define AMCLIB_ACIMCtrlMTPAInit_FLT(fltIDMin, fltIDMax, psCtrl)                \
        AMCLIB_ACIMCtrlMTPAInit_FLT_Ci(fltIDMin, fltIDMax, psCtrl)                           
#define AMCLIB_ACIMCtrlMTPA_FLT(fltIq, psCtrl)                                 \
        AMCLIB_ACIMCtrlMTPA_FLT_C(fltIq, psCtrl)                              
#define AMCLIB_ACIMRotFluxObsrvInit_FLT(psCtrl)                                \
        AMCLIB_ACIMRotFluxObsrvInit_FLT_Ci(psCtrl)                              
#define AMCLIB_ACIMRotFluxObsrv_FLT(psIsAlBe, sUSAlBe, psCtrl)                 \
        AMCLIB_ACIMRotFluxObsrv_FLT_C(psIsAlBe, sUSAlBe, psCtrl )                    
#define AMCLIB_ACIMSpeedMRASInit_FLT(psCtrl)                                   \
        AMCLIB_ACIMSpeedMRASInit_FLT_Ci(psCtrl)                                                          
#define AMCLIB_ACIMSpeedMRAS_FLT(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)     \
        AMCLIB_ACIMSpeedMRAS_FLT_C(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl) 
#define AMCLIB_AngleTrackObsrvInit_A32(a32ThetaInit, psCtrl)                   \
        AMCLIB_AngleTrackObsrvInit_A32_C(a32ThetaInit, psCtrl)     
#define AMCLIB_AngleTrackObsrv_A32ff(psSinCos, psCtrl)                         \
        AMCLIB_AngleTrackObsrv_A32ff_C(psSinCos, psCtrl)                           
#define AMCLIB_CtrlFluxWkngInit_FLT(fltInitVal, psCtrl)                        \
        AMCLIB_CtrlFluxWkngInit_FLT_Ci(fltInitVal, psCtrl)             
#define AMCLIB_CtrlFluxWkng_FLT(fltIQErr, fltUQReq, fltUQLim, psCtrl)          \
        AMCLIB_CtrlFluxWkng_FLT_C(fltIQErr, fltUQReq, fltUQLim, psCtrl)                      
#define AMCLIB_PMSMBemfObsrvDQInit_A32fff(psCtrl)                              \
        AMCLIB_PMSMBemfObsrvDQInit_A32fff_C(psCtrl)  
#define AMCLIB_PMSMBemfObsrvDQ_A32fff(psIDQ, psUDQ, fltSpeed, psCtrl)          \
        AMCLIB_PMSMBemfObsrvDQ_A32fff_C(psIDQ, psUDQ, fltSpeed, psCtrl)        
#define AMCLIB_PMSMBemfObsrvABInit_FLT(psCtrl)                                 \
        AMCLIB_PMSMBemfObsrvABInit_FLT_Ci( psCtrl)                 
#define AMCLIB_PMSMBemfObsrvAB_FLT(psIAlBe, psUAlBe, fltSpeed, psCtrl)         \
        AMCLIB_PMSMBemfObsrvAB_FLT_C(psIAlBe, psUAlBe, fltSpeed, psCtrl)
#define AMCLIB_TrackObsrvInit_A32af(a32ThetaInit, psCtrl)                      \
        AMCLIB_TrackObsrvInit_A32af_Ci(a32ThetaInit, psCtrl)
#define AMCLIB_TrackObsrv_A32af(a32ThetaErr, psCtrl)                           \
        AMCLIB_TrackObsrv_A32af_C(a32ThetaErr, psCtrl)                                
  
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_FP_H_ */ 
