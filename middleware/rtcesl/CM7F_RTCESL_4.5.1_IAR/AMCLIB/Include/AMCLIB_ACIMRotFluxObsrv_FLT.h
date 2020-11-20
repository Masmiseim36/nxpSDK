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
#include "gflib_FP.h"     
#include "gmclib_FP.h"     
#include "mlib_FP.h"

/******************************************************************************
* Macros 
******************************************************************************/   
#define AMCLIB_ACIMRotFluxObsrv_FLT_C( psIsAlBe, psUSAlBe, psCtrl)            \
        AMCLIB_ACIMRotFluxObsrv_FLT_FC(psIsAlBe, psUSAlBe, psCtrl)                                                    
#define AMCLIB_ACIMRotFluxObsrv_FLT_CRam( psIsAlBe, pUSAlBe, psCtrl)          \
        AMCLIB_ACIMRotFluxObsrv_FLT_FCRam(psIsAlBe, pUSAlBe, psCtrl)
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
    float_t fltPsiRB1Gain;                 /* Constant determined by: Lm*Ts / (Tau_r) */                   
    float_t fltPsiSA1Gain;                 /* Constant determined by: Tau_r / (Tau_r + Ts)*/                   
    float_t fltPsiSA2Gain;                 /* Constant determined by: (1 / (2*PI*CoeffRFO))/((1/(2*PI*CoeffRFO))+Ts) */  
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
RAM_FUNC_LIB 
extern void AMCLIB_ACIMRotFluxObsrv_FLT_FCRam(const GMCLIB_2COOR_ALBE_T_FLT *psISAlBe,
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
RAM_FUNC_LIB 
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
