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
