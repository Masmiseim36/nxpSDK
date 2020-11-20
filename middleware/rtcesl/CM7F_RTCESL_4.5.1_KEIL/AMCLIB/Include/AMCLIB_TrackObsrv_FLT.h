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
static inline void AMCLIB_TrackObsrvInit_A32af_FCi(acc32_t a32ThetaInit,
                                                   AMCLIB_TRACK_OBSRV_T_FLT *psCtrl)
{
    psCtrl -> f32Theta = ((frac32_t)((frac16_t)a32ThetaInit))<<16; 
    psCtrl -> fltSpeed = 0.0F;
    psCtrl -> fltI_1   = 0.0F;        
}
 
#if defined(__cplusplus) 
}
#endif

#endif  /* _AMCLIB_TRACK_OBSRV_FLT_H_ */
