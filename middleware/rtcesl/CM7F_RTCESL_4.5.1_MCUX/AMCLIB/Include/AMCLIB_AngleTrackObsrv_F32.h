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
#ifndef _AMCLIB_ANGLE_TRACK_OBSRV_F32_H_
#define _AMCLIB_ANGLE_TRACK_OBSRV_F32_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "gflib.h"
#include "gmclib.h"

/******************************************************************************
* Macros 
******************************************************************************/
#define AMCLIB_AngleTrackObsrv_F16_C(psSinCos, psCtrl)                        \
        AMCLIB_AngleTrackObsrv_F16_FC(psSinCos, psCtrl)
#define AMCLIB_AngleTrackObsrv_F16_CRam(psSinCos, psCtrl)                     \
        AMCLIB_AngleTrackObsrv_F16_FCRam(psSinCos, psCtrl)		
#define AMCLIB_AngleTrackObsrvInit_F16_C(f16ThetaInit, psCtrl)                \
        AMCLIB_AngleTrackObsrvInit_F16_FC(f16ThetaInit, psCtrl)
#define AMCLIB_AngleTrackObsrvInit_F16_CRam(f16ThetaInit, psCtrl)             \
        AMCLIB_AngleTrackObsrvInit_F16_FCRam(f16ThetaInit, psCtrl)		
        
/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
    frac32_t   f32Speed;       
    frac32_t   f32A2;          
    frac16_t   f16Theta;       
    frac16_t   f16SinEstim;    
    frac16_t   f16CosEstim;    
    frac16_t   f16K1Gain;        
    int16_t    i16K1GainSh;
    frac16_t   f16K2Gain;        
    int16_t    i16K2GainSh;
    frac16_t   f16A2Gain;        
    int16_t    i16A2GainSh;
} AMCLIB_ANGLE_TRACK_OBSRV_T_F32;
   
/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern frac16_t AMCLIB_AngleTrackObsrv_F16_FC(const GMCLIB_2COOR_SINCOS_T_F16 *psSinCos,
                                              AMCLIB_ANGLE_TRACK_OBSRV_T_F32 *psCtrl);

RAM_FUNC_LIB 
extern frac16_t AMCLIB_AngleTrackObsrv_F16_FCRam(const GMCLIB_2COOR_SINCOS_T_F16 *psSinCos,
                                                 AMCLIB_ANGLE_TRACK_OBSRV_T_F32 *psCtrl);
											  
extern void AMCLIB_AngleTrackObsrvInit_F16_FC(frac16_t f16ThetaInit, 
                                              AMCLIB_ANGLE_TRACK_OBSRV_T_F32 *psCtrl);
											  
RAM_FUNC_LIB 
extern void AMCLIB_AngleTrackObsrvInit_F16_FCRam(frac16_t f16ThetaInit, 
                                                 AMCLIB_ANGLE_TRACK_OBSRV_T_F32 *psCtrl);

#if defined(__cplusplus) 
} 
#endif 

#endif /* _AMCLIB_ANGLE_TRACK_OBSRV_F32_H_ */
