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
#ifndef _AMCLIB_TRACK_OBSRV_F32_H_
#define _AMCLIB_TRACK_OBSRV_F32_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "amclib_types.h"
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/                       
#define AMCLIB_TrackObsrv_F16_Asm(f16ThetaErr, psCtrl)                        \
        AMCLIB_TrackObsrv_F16_FAsm(f16ThetaErr, psCtrl)                      
#define AMCLIB_TrackObsrvInit_F16_Ci(f16ThetaInit, psCtrl)                    \
        AMCLIB_TrackObsrvInit_F16_FCi(f16ThetaInit, psCtrl)            

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32Theta;    /* Estimated position */
    frac32_t f32Speed;    /* Estimated speed - first integrator output */  
    frac32_t f32I_1;      /* State variable of observer controller part */
    frac16_t f16IGain;    /* Observer integral gain */
    int16_t i16IGainSh;   /* Observer integral gain shift*/
    frac16_t f16PGain;    /* Observer proportional gain */
    int16_t i16PGainSh;   /* Observer proportional gain shift*/
    frac16_t f16ThGain;   /* Observer gain for output integrator of position */
    int16_t i16ThGainSh;  /* Observer gain shift for integrator of position */
}AMCLIB_TRACK_OBSRV_T_F32;
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t AMCLIB_TrackObsrv_F16_FAsm(frac16_t f16ThetaErr,
                                           AMCLIB_TRACK_OBSRV_T_F32 *psCtrl);
/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    Tracking observer initialization
*
* @param  in  - frac16_t f16ThetaInit - init angle <-1;1) corresponds to <-pi;pi)
* @param  ptr - AMCLIB_TRACK_OBSRV_T_F32 *psCtrl
*               -  frac32_t f32Theta  - Estimated position <-1;1) corresponds to <-pi;pi)
*               -  frac32_t f32Speed  - Estimated speed <-1;1)
*               -  frac16_t f32I_1    - Internal integrator <-1;1)
*               -  frac16_t f16IGain  - Integ. constant to get speed from error <0;1)
*               -  int16_t i16IGainSh - Shift for f16IGain <-15;15>
*               -  frac16_t f16PGain  - Prop. constant to get speed from angle <0;1)
*               -  int16_t i16PGainSh - Shift for f16PGain <-15;15>
*               -  frac16_t f16ThGain - Constant to get angle from speed <0;1)
*               -  int16_t i16ThGainSh- Shift for f16ThGain <-15;15>
*
* @return None
*                         
* @remarks  Initializes the structure of the tracking observer with an angle
*           according to following rules:
*           
*   f32Theta = f16ThetaInit << 16  
*   f32Speed = 0
*   f32I_1 = 0
*
****************************************************************************/
static inline void AMCLIB_TrackObsrvInit_F16_FCi(frac16_t f16ThetaInit,
                                                 AMCLIB_TRACK_OBSRV_T_F32 *psCtrl)
{
    psCtrl -> f32Theta = MLIB_Conv_F32s(f16ThetaInit);
    psCtrl -> f32Speed = 0;
    psCtrl -> f32I_1   = 0;
}
 
#if defined(__cplusplus) 
}
#endif

#endif  /* _AMCLIB_TRACK_OBSRV_F32_H_ */
