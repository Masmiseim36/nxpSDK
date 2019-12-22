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
* @brief  ACIM Flux Observer  
* 
*******************************************************************************/
#ifndef _AMCLIB_ACIMROTFLUXOBSERVER_FLT_H_
#define _AMCLIB_ACIMROTFLUXOBSERVER_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/****************************************************************************
* Includes
****************************************************************************/   
#include "amclib_types.h"  
#include "gflib_fp.h"     
#include "gmclib_fp.h"     
#include "mlib_fp.h"

/******************************************************************************
* Macros 
******************************************************************************/   
#define AMCLIB_ACIMRotFluxObsrv_FLT_C( psIsAlBe, sUSAlBe, psCtrl)             \
        AMCLIB_ACIMRotFluxObsrv_FLT_FC(psIsAlBe, sUSAlBe, psCtrl)                                                    
#define AMCLIB_ACIMRotFluxObsrvInit_FLT_Ci( psCtrl)                           \
        AMCLIB_ACIMRotFluxObsrvInit_FLT_FCi(psCtrl)              
            
/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{   /* function output variables */     
    GMCLIB_2COOR_DQ_T_FLT sPsiRotRDQ;      /* Rotor flux estimated structure from rotor (current) model, in/out structure */   
    GMCLIB_2COOR_ALBE_T_FLT sPsiRotSAlBe;  /* Rotor flux estimated structure from stator (voltage) model, in/out structure */   
    GMCLIB_2COOR_ALBE_T_FLT sPsiStatSAlBe; /* Stator flux estimated structure from stator (voltage) model, in/out structure */ 
    float_t fltTorque;                     /* Motor torque calculated from currents and fluxes */
    acc32_t a32RotFluxPos;                 /* Rotor flux estimated position (angle), in/out variable */
    
    /* function state variables */
    struct  
    {   
        float_t  fltCompAlphaInteg_1;      /* Integral part state variable for alpha coefficient */                               
        float_t  fltCompBetaInteg_1;       /* Integral part state variable for beta coefficient */ 
        float_t  fltCompAlphaErr_1;        /* Error_1 state variable for alpha coefficient */                               
        float_t  fltCompBetaErr_1;         /* Error_1 state variable for beta coefficient */ 
        float_t  fltPGain;                 /* Proportional gain Kp for PI controller */                              
        float_t  fltIGain;                 /* Integration gain Ki for PI controller */       
    } sCtrl;       
    
    /* function parameters */              /* Constant Tau_r = Lr/Rr, CoeffRFO    */
    float_t fltPsiRA1Gain;                 /* Constant determined by: Tau_r / (Tau_r+Ts) */                   
    float_t fltPsiRB1Gain;                 /* Constant determined by: Lm*Ts / (Tau_r+Ts) */                   
    float_t fltPsiSA1Gain;                 /* Constant determined by: Tau_r / (Tau_r + Ts)*/                   
    float_t fltPsiSA2Gain;                 /* Constant determined by: (1 / (2*PIPI*CoeffRFO))/((1/(2*PI*CoeffRFO))+Ts) */  
    float_t fltKrInvGain;                  /* Constant determined by: Lr / Lm */                              
    float_t fltKrLsTotLeakGain;            /* Constant determined by: fltKrInvGain * Ls * (1-Lm*Lm/(Ls*Lr) */                           
    float_t fltRsEst;                      /* Estimated stator resistance = Rs [ohm] */  
    float_t fltTorqueGain;                 /* Torque constant given by: 3*Pp*Lm/(2*Lr) */ 
} AMCLIB_ACIM_ROT_FLUX_OBSRV_T_FLT;     
   
/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern void AMCLIB_ACIMRotFluxObsrv_FLT_FC(const GMCLIB_2COOR_ALBE_T_FLT *psISAlBe,
                                           const GMCLIB_2COOR_ALBE_T_FLT *psUSAlBe,                                               
                                           AMCLIB_ACIM_ROT_FLUX_OBSRV_T_FLT *psCtrl);   
                                     
/****************************************************************************
* Inline functions 
****************************************************************************/
                                     
/***************************************************************************//*!
* @brief  The function initializes the actual values of float AMCLIB_ACIMRotFluxObsrv function.
*
* @params:      ptr  AMCLIB_ACIM_ROT_FLUX_OBSRV_T_FLT *psParam - Pointer to flux observer structure                
*
* @return       N/A
* 
*******************************************************************************/
static inline void AMCLIB_ACIMRotFluxObsrvInit_FLT_FCi(AMCLIB_ACIM_ROT_FLUX_OBSRV_T_FLT *psCtrl)
{
    psCtrl->sPsiRotRDQ.fltD           = (float_t) 0.0F;
    psCtrl->sPsiRotRDQ.fltQ           = (float_t) 0.0F;    
    psCtrl->sPsiRotSAlBe.fltAlpha     = (float_t) 0.0F;
    psCtrl->sPsiRotSAlBe.fltBeta      = (float_t) 0.0F;
    psCtrl->sPsiStatSAlBe.fltAlpha    = (float_t) 0.0F;
    psCtrl->sPsiStatSAlBe.fltBeta     = (float_t) 0.0F;
    psCtrl->a32RotFluxPos             = (acc32_t) 0;
    psCtrl->sCtrl.fltCompAlphaInteg_1 = (float_t) 0.0F;
    psCtrl->sCtrl.fltCompBetaInteg_1  = (float_t) 0.0F;
    psCtrl->sCtrl.fltCompAlphaErr_1   = (float_t) 0.0F;
    psCtrl->sCtrl.fltCompBetaErr_1    = (float_t) 0.0F; 
}
 
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_ACIMROTFLUXOBSERVER_FLT_H_ */
