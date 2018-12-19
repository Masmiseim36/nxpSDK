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
#define AMCLIB_AngleTrackObsrv_A32ff_C(psSinCos, psCtrl)                     \
        AMCLIB_AngleTrackObsrv_A32ff_FC(psSinCos, psCtrl)     
            
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
extern void AMCLIB_AngleTrackObsrvInit_A32_FC(acc32_t a32ThetaInit, 
                                              AMCLIB_ANGLE_TRACK_OBSRV_T_FLT *psCtrl);
#if defined(__cplusplus) 
} 
#endif 

#endif /* _AMCLIB_ANGLE_TRACK_OBSRV_FLT_H_ */
