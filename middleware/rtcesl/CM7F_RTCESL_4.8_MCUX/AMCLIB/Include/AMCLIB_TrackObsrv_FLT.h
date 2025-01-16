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
* @brief  Tracking observer 
* 
*******************************************************************************/
#ifndef _AMCLIB_TRACK_OBSRV_FLT_H_
#define _AMCLIB_TRACK_OBSRV_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "amclib_types.h"
#include "mlib_FP.h"
    
/*******************************************************************************
* Macros 
*******************************************************************************/    
#define AMCLIB_TrackObsrv_A32af_C(a32ThetaErr, psCtrl)                         \
        AMCLIB_TrackObsrv_A32af_FC(a32ThetaErr, psCtrl)  
#define AMCLIB_TrackObsrv_A32af_CRam(a32ThetaErr, psCtrl)                      \
        AMCLIB_TrackObsrv_A32af_FCRam(a32ThetaErr, psCtrl)           
#define AMCLIB_TrackObsrvInit_A32af_Ci(a32ThetaInit, psCtrl)                   \
        AMCLIB_TrackObsrvInit_A32af_FCi(a32ThetaInit, psCtrl)
            
/*******************************************************************************
* Types
*******************************************************************************/          
typedef struct
{
    frac32_t f32Theta;   /* Estimated position */
    float_t fltSpeed;    /* Estimated speed - first integrator output */  
    float_t fltI_1;      /* State variable of observer controller part */
    float_t fltIGain;    /* Observer integral gain */
    float_t fltPGain;    /* Observer proportional gain */
    float_t fltThGain;   /* Observer gain for output integrator of position */
}AMCLIB_TRACK_OBSRV_T_FLT;
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern acc32_t AMCLIB_TrackObsrv_A32af_FC(acc32_t a32ThetaErr, AMCLIB_TRACK_OBSRV_T_FLT *psCtrl);
RAM_FUNC_NAME
extern acc32_t AMCLIB_TrackObsrv_A32af_FCRam(acc32_t a32ThetaErr,AMCLIB_TRACK_OBSRV_T_FLT *psCtrl);
/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    Tracking observer initialization
*
* @param  in  - acc32_t a32ThetaInit  - init angle <-1;1) corresponds to <-pi;pi)
* @param  ptr - AMCLIB_TRACK_OBSRV_T_FLT *psCtrl
*               - frac32_t f32Theta - Estimated position 
*               - float_t fltSpeed  - Estimated speed 
*               - float_t fltI_1    - Internal integrator 
*               - float_t fltIGain  - Integ. constant to get speed from error 
*               - float_t fltPGain  - Prop. constant to get speed from angle 
*               - float_t fltThGain - Constant to get angle from speed 
*
* @return None
*                         
* @remarks   Initializes the structure of the tracking observer with an angle
*            according to following rules:   
*           
*   f32Theta = a32ThetaInit << 16 
*   fltSpeed = 0.0F
*   fltI_1   = 0.0F
*
****************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline void AMCLIB_TrackObsrvInit_A32af_FCi(acc32_t a32ThetaInit,
                                                   AMCLIB_TRACK_OBSRV_T_FLT *psCtrl)
{
    psCtrl -> f32Theta = MLIB_Conv_F32s((frac16_t)a32ThetaInit);
    psCtrl -> fltSpeed = 0.0F;
    psCtrl -> fltI_1   = 0.0F;        
}
 
#if defined(__cplusplus) 
}
#endif

#endif  /* _AMCLIB_TRACK_OBSRV_FLT_H_ */
