/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
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
 */

/*******************************************************************************
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
#include "AMCLIB_PMSMBemfObsrvDQ_A32.h"
#include "AMCLIB_TrackObsrv_F32.h"  

#include "AMCLIB_PMSMBemfObsrvDQHw_A32.h"
    
/*******************************************************************************
* Macros 
*******************************************************************************/        
#define AMCLIB_AngleTrackObsrvInit_F16(f16ThetaInit, psCtrl)                   \
        AMCLIB_AngleTrackObsrvInit_F16_C(f16ThetaInit, psCtrl)          
#define AMCLIB_AngleTrackObsrv_F16(f16ThetaErr, psCtrl)                        \
        AMCLIB_AngleTrackObsrv_F16_C(f16ThetaErr, psCtrl)              
#define AMCLIB_PMSMBemfObsrvDQInit_F16(psCtrl)                                 \
        AMCLIB_PMSMBemfObsrvDQInit_F16_Ci(psCtrl)
#define AMCLIB_TrackObsrvInit_F16(f16ThetaInit, psCtrl)                        \
        AMCLIB_TrackObsrvInit_F16_Ci(f16ThetaInit, psCtrl)           
#define AMCLIB_TrackObsrv_F16(f16ThetaErr, psCtrl)                             \
        AMCLIB_TrackObsrv_F16_Asm(f16ThetaErr, psCtrl)  
        
#if (RTCESL_MMDVSQ)     
#define AMCLIB_PMSMBemfObsrvDQ_F16(psIDQ, psUDQ, f16Speed, psCtrl)             \
        AMCLIB_PMSMBemfObsrvDQHw_F16_Asm(psIDQ, psUDQ, f16Speed, psCtrl)
#else            
#define AMCLIB_PMSMBemfObsrvDQ_F16(psIDQ, psUDQ, f16Speed, psCtrl)             \
        AMCLIB_PMSMBemfObsrvDQ_F16_Asm(psIDQ, psUDQ, f16Speed, psCtrl)
#endif            
            
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_H_ */ 
