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
* @brief Main AMCLIB header file for devices without FPU.  
* 
*******************************************************************************/
#ifndef _AMCLIB_H_
#define _AMCLIB_H_

#if defined(__cplusplus) 
extern "C" {
#endif  
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"    
#include "AMCLIB_AngleTrackObsrv_F32.h" 
#include "AMCLIB_CtrlFluxWkng_A32.h"    
#include "AMCLIB_PMSMBemfObsrvDQ_A32.h"
#include "AMCLIB_PMSMBemfObsrvAB_A32.h"    
#include "AMCLIB_TrackObsrv_F32.h"   

/*******************************************************************************
* Macros 
*******************************************************************************/      
#define AMCLIB_AngleTrackObsrvInit_F16(f16ThetaInit, psCtrl)                   \
        AMCLIB_AngleTrackObsrvInit_F16_C(f16ThetaInit, psCtrl)                 
#define AMCLIB_AngleTrackObsrv_F16(f16ThetaErr, psCtrl)                        \
        AMCLIB_AngleTrackObsrv_F16_C(f16ThetaErr, psCtrl)       
#define AMCLIB_CtrlFluxWkngInit_F16(f16InitVal, psCtrl)                        \
        AMCLIB_CtrlFluxWkngInit_F16_Ci(f16InitVal, psCtrl)            
#define AMCLIB_CtrlFluxWkng_F16(f16IQErr, f16UQReq, f16UQLim, psCtrl)          \
        AMCLIB_CtrlFluxWkng_F16_C(f16IQErr, f16UQReq, f16UQLim, psCtrl)                   
#define AMCLIB_PMSMBemfObsrvDQInit_F16(psCtrl)                                 \
        AMCLIB_PMSMBemfObsrvDQInit_F16_Ci(psCtrl)                              
#define AMCLIB_PMSMBemfObsrvDQ_F16(psIDQ, psUDQ, f16Speed, psCtrl)             \
        AMCLIB_PMSMBemfObsrvDQ_F16_Asm(psIDQ, psUDQ, f16Speed, psCtrl)         
#define AMCLIB_PMSMBemfObsrvABInit_F16(psCtrl)                                 \
        AMCLIB_PMSMBemfObsrvABInit_F16_Ci(psCtrl)                              
#define AMCLIB_PMSMBemfObsrvAB_F16(psIAlBe, psUAlBe, f16Speed, psCtrl)         \
        AMCLIB_PMSMBemfObsrvAB_F16_C(psIAlBe, psUAlBe, f16Speed, psCtrl)     
#define AMCLIB_TrackObsrvInit_F16(f16ThetaInit, psCtrl)                        \
        AMCLIB_TrackObsrvInit_F16_Ci(f16ThetaInit, psCtrl)                     
#define AMCLIB_TrackObsrv_F16(f16ThetaErr, psCtrl)                             \
        AMCLIB_TrackObsrv_F16_Asm(f16ThetaErr, psCtrl)
        
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_H_ */ 
