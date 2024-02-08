/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
* @brief Space vector modulation 
* 
*******************************************************************************/
#ifndef _GMCLIB_SVM_F16_H_
#define _GMCLIB_SVM_F16_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "mlib.h"
#include "gmclib_types.h"
  
/*******************************************************************************
* Macros 
*******************************************************************************/ 
#define GMCLIB_SvmDpwm_F16_C(psIn, psOut)                            GMCLIB_SvmDpwm_F16_FC(psIn, psOut)       
#define GMCLIB_SvmExDpwm_F16_C(psIn, psAngle, psOut)                 GMCLIB_SvmExDpwm_F16_FC(psIn, psAngle,psOut)        
#define GMCLIB_SvmIct_F16_C(psIn, psOut)                             GMCLIB_SvmIct_F16_FC(psIn, psOut)  
#define GMCLIB_SvmStd_F16_Asm(psIn, psOut)                           GMCLIB_SvmStd_F16_FAsm(psIn, psOut) 
#define GMCLIB_SvmStdShifted_F16_C(psIn, param, psCfgPWM, psCfgMeas) GMCLIB_SvmStdShifted_F16_FC(psIn, param, psCfgPWM, psCfgMeas) 
#define GMCLIB_SvmU7n_F16_C(psIn, psOut)                             GMCLIB_SvmU7n_F16_FC(psIn, psOut)    
#define GMCLIB_SvmU0n_F16_C(psIn, psOut)                             GMCLIB_SvmU0n_F16_FC(psIn, psOut) 

/******************************************************************************
* Types 
******************************************************************************/
/* The GMCLIB_SvmStdShifted parameters structure */
typedef struct
{
    frac16_t f16LowerLim;
    frac16_t f16UpperLim;
    frac16_t f16MinT1T2;
} GMCLIB_SVMSTDSHIFTED_T_F16;

/* The GMCLIB_SvmStdShifted PWM signal configuration. */
typedef struct
{
    GMCLIB_3COOR_T_F16 sDuty;        /* Three-phase duty-cycle. */
    GMCLIB_3COOR_T_F16 sShift;       /* Edge shift. */
} GMCLIB_PWM_CONFIG_T_F16;

/* The GMCLIB_SvmStdShifted phase index enumeration. */
typedef enum
{
    kPhaseA = 0U,
    kPhaseB = 1U,
    kPhaseC = 2U
} GMCLIB_PHASE_INDEX_T;

/* The GMCLIB_SvmStdShifted measurement configuration structure. */
typedef struct
{
    uint16_t ui16SectorSVM;
    frac16_t f16SmplFirstEdge;
    frac16_t f16SmplSecondEdge;
    GMCLIB_PHASE_INDEX_T eSmplOnePh;
    GMCLIB_PHASE_INDEX_T eSmplTwoPh;
    GMCLIB_PHASE_INDEX_T eCalcPh;
} GMCLIB_ADC_CONFIG_T_F16;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/   
extern uint16_t GMCLIB_SvmStd_F16_FAsm(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                       GMCLIB_3COOR_T_F16 *psOut);
extern uint16_t GMCLIB_SvmIct_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                     GMCLIB_3COOR_T_F16 *psOut);
extern uint16_t GMCLIB_SvmU7n_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                     GMCLIB_3COOR_T_F16 *psOut);
extern void GMCLIB_SvmStdShifted_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                        const GMCLIB_SVMSTDSHIFTED_T_F16 *psParam,
                                        GMCLIB_ADC_CONFIG_T_F16 *psCfgMeas,                             
                                        GMCLIB_PWM_CONFIG_T_F16 *psCfgPWM);                                        
extern uint16_t GMCLIB_SvmU0n_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                     GMCLIB_3COOR_T_F16 *psOut);
extern uint16_t GMCLIB_SvmDpwm_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn, 
                                      GMCLIB_3COOR_T_F16 *psOut);
extern uint16_t GMCLIB_SvmExDpwm_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn, 
                                        const GMCLIB_2COOR_SINCOS_T_F16 *psAngle, 
                                        GMCLIB_3COOR_T_F16 *psOut);
#if defined(__cplusplus)  
}
#endif

#endif  /* _GMCLIB_SVM_F16_H_ */
