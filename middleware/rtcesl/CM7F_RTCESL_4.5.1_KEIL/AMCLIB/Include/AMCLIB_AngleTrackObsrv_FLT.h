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
* @brief  Angle tracking observer
* 
*******************************************************************************/
#ifndef _AMCLIB_ANGLE_TRACK_OBSRV_FLT_H_
#define _AMCLIB_ANGLE_TRACK_OBSRV_FLT_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gmclib_FP.h"
    
/******************************************************************************
* Macros 
******************************************************************************/                    
#define AMCLIB_AngleTrackObsrvInit_A32_C(a32ThetaInit, psCtrl)               \
        AMCLIB_AngleTrackObsrvInit_A32_FC(a32ThetaInit, psCtrl)
#define AMCLIB_AngleTrackObsrvInit_A32_CRam(a32ThetaInit, psCtrl)            \
        AMCLIB_AngleTrackObsrvInit_A32_FCRam(a32ThetaInit, psCtrl)       		
#define AMCLIB_AngleTrackObsrv_A32ff_C(psSinCos, psCtrl)                     \
		AMCLIB_AngleTrackObsrv_A32ff_FC(psSinCos, psCtrl)     
#define AMCLIB_AngleTrackObsrv_A32ff_CRam(psSinCos, psCtrl)                  \
        AMCLIB_AngleTrackObsrv_A32ff_FCRam(psSinCos, psCtrl)            
/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
    float_t  fltSpeed;     /* Estimated speed */ 
    frac32_t f32A2;        /* Internal integrator */     
    acc32_t  a32Theta;     /* Estimated angle <-1;1) corresponds to <-pi;pi)*/    
    float_t  fltSinEstim;  /* Sine of the estimated angle */  
    float_t  fltCosEstim;  /* Cosine of the estimated angle */  
    float_t  fltK1Gain;    /* Constant to get speed from error */      
    float_t  fltK2Gain;    /* Prop. constant to get angle from speed */       
    float_t  fltA2Gain;    /* Integ. constant to get angle from speed */                   
} AMCLIB_ANGLE_TRACK_OBSRV_T_FLT; 

/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern acc32_t AMCLIB_AngleTrackObsrv_A32ff_FC(const GMCLIB_2COOR_SINCOS_T_FLT *psSinCos,
                                               AMCLIB_ANGLE_TRACK_OBSRV_T_FLT *psCtrl);
RAM_FUNC_LIB 
extern acc32_t AMCLIB_AngleTrackObsrv_A32ff_FCRam(const GMCLIB_2COOR_SINCOS_T_FLT *psSinCos,
                                                  AMCLIB_ANGLE_TRACK_OBSRV_T_FLT *psCtrl);
											  
extern void AMCLIB_AngleTrackObsrvInit_A32_FC(acc32_t a32ThetaInit, 
                                              AMCLIB_ANGLE_TRACK_OBSRV_T_FLT *psCtrl);
RAM_FUNC_LIB 
extern void AMCLIB_AngleTrackObsrvInit_A32_FCRam(acc32_t a32ThetaInit, 
                                                 AMCLIB_ANGLE_TRACK_OBSRV_T_FLT *psCtrl);
											  
#if defined(__cplusplus) 
} 
#endif 

#endif /* _AMCLIB_ANGLE_TRACK_OBSRV_FLT_H_ */
