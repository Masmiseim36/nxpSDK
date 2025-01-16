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
