/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#if (defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW)) /* placed to RAM */
#define AMCLIB_ACIMCtrlMTPA_FLT(fltIq, psCtrl)                                 \
        AMCLIB_ACIMCtrlMTPA_FLT_CRam(fltIq, psCtrl)                              
#define AMCLIB_ACIMRotFluxObsrv_FLT(psIsAlBe, sUSAlBe, psCtrl)                 \
        AMCLIB_ACIMRotFluxObsrv_FLT_CRam(psIsAlBe, sUSAlBe, psCtrl )               
#define AMCLIB_ACIMSpeedMRAS_FLT(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)     \
        AMCLIB_ACIMSpeedMRAS_FLT_CRam(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)
#define AMCLIB_AngleTrackObsrvInit_A32(a32ThetaInit, psCtrl)                   \
        AMCLIB_AngleTrackObsrvInit_A32_CRam(a32ThetaInit, psCtrl)          
#define AMCLIB_AngleTrackObsrv_A32ff(psSinCos, psCtrl)                         \
        AMCLIB_AngleTrackObsrv_A32ff_CRam(psSinCos, psCtrl)                        
#define AMCLIB_CtrlFluxWkng_FLT(fltIQErr, fltUQReq, fltUQLim, psCtrl)          \
        AMCLIB_CtrlFluxWkng_FLT_CRam(fltIQErr, fltUQReq, fltUQLim, psCtrl)         
#define AMCLIB_PMSMBemfObsrvDQ_A32fff(psIDQ, psUDQ, fltSpeed, psCtrl)          \
        AMCLIB_PMSMBemfObsrvDQ_A32fff_CRam(psIDQ, psUDQ, fltSpeed, psCtrl)        
#define AMCLIB_PMSMBemfObsrvAB_FLT(psIAlBe, psUAlBe, fltSpeed, psCtrl)         \
        AMCLIB_PMSMBemfObsrvAB_FLT_CRam(psIAlBe, psUAlBe, fltSpeed, psCtrl)
#define AMCLIB_TrackObsrv_A32af(a32ThetaErr, psCtrl)                           \
        AMCLIB_TrackObsrv_A32af_CRam(a32ThetaErr, psCtrl)	
#else  /* placed to ROM */
#define AMCLIB_ACIMCtrlMTPA_FLT(fltIq, psCtrl)                                 \
        AMCLIB_ACIMCtrlMTPA_FLT_C(fltIq, psCtrl)                              
#define AMCLIB_ACIMRotFluxObsrv_FLT(psIsAlBe, pUSAlBe, psCtrl)                 \
        AMCLIB_ACIMRotFluxObsrv_FLT_C(psIsAlBe, pUSAlBe, psCtrl )               
#define AMCLIB_ACIMSpeedMRAS_FLT(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)     \
        AMCLIB_ACIMSpeedMRAS_FLT_C(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)
#define AMCLIB_AngleTrackObsrvInit_A32(a32ThetaInit, psCtrl)                   \
        AMCLIB_AngleTrackObsrvInit_A32_C(a32ThetaInit, psCtrl)         
#define AMCLIB_AngleTrackObsrv_A32ff(psSinCos, psCtrl)                         \
        AMCLIB_AngleTrackObsrv_A32ff_C(psSinCos, psCtrl)                        
#define AMCLIB_CtrlFluxWkng_FLT(fltIQErr, fltUQReq, fltUQLim, psCtrl)          \
        AMCLIB_CtrlFluxWkng_FLT_C(fltIQErr, fltUQReq, fltUQLim, psCtrl)         
#define AMCLIB_PMSMBemfObsrvDQ_A32fff(psIDQ, psUDQ, fltSpeed, psCtrl)          \
        AMCLIB_PMSMBemfObsrvDQ_A32fff_C(psIDQ, psUDQ, fltSpeed, psCtrl)        
#define AMCLIB_PMSMBemfObsrvAB_FLT(psIAlBe, psUAlBe, fltSpeed, psCtrl)         \
        AMCLIB_PMSMBemfObsrvAB_FLT_C(psIAlBe, psUAlBe, fltSpeed, psCtrl)
#define AMCLIB_TrackObsrv_A32af(a32ThetaErr, psCtrl)                           \
        AMCLIB_TrackObsrv_A32af_C(a32ThetaErr, psCtrl)		
#endif /* defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW) */
                       
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
