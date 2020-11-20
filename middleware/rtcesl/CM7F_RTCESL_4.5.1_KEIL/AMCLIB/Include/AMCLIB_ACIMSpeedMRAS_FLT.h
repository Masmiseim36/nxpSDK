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
* @brief ACIM Speed Estimator based on MRAS   
* 
*******************************************************************************/
#ifndef _AMCLIB_ACIMSPEEDMRAS_FLT_H_
#define _AMCLIB_ACIMSPEEDMRAS_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/****************************************************************************
* Includes
****************************************************************************/   
#include "amclib_types.h" 
#include "gdflib_FP.h"  
#include "gflib_FP.h"     
#include "gmclib_FP.h"     
#include "mlib_FP.h"

/******************************************************************************
* Macros 
******************************************************************************/      
#define AMCLIB_ACIMSpeedMRAS_FLT_C( psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl)    \
        AMCLIB_ACIMSpeedMRAS_FLT_FC(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl) 
#define AMCLIB_ACIMSpeedMRAS_FLT_CRam( psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl) \
        AMCLIB_ACIMSpeedMRAS_FLT_FCRam(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl) 
#define AMCLIB_ACIMSpeedMRASInit_FLT_Ci( psCtrl)                                 \
        AMCLIB_ACIMSpeedMRASInit_FLT_FCi(psCtrl)                                              
            
/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{   
    GDFLIB_FILTER_IIR1_T_FLT fltSpeedElIIR1Param;/* IIR1 filter structure for estimated speed */ 
    
    /* function output variables */
    GMCLIB_2COOR_DQ_T_FLT sPsiRotRDQ;            /* Rotor flux estimated structure from rotor (current) model, in/out structure */      
    float_t fltSpeedEl;                          /* Rotor estimated electric speed, output variable  */
    float_t fltSpeedElIIR1;                      /* Rotor estimated electric speed filtered,  output variable  */
    float_t fltSpeedMeIIR1;                      /* Rotor estimated mechanical speed filtered,  output variable  */ 
    acc32_t a32RotPos;                           /* Rotor estimated electric position, output variable  */        
    
    /* function state variables */
    struct                                       
    {                                            
        float_t  fltSpeedElInteg_1;              /* Integral part state variable for rotor electrical estimated speed controller  */  
        float_t  fltSpeedElErr_1;                /* Error_1 state variable for rotor electrical estimated speed controller */        
        float_t  fltPGain;                       /* Proportional gain Kp for MRAS PI controller */                                              
        float_t  fltIGain;                       /* Integration gain Ki for MRAS PI controller */ 
    } sCtrl;    
    
    /* function parameters */    
    float_t fltPsiRA1Gain;                       /* Constant determined by: Tau_r / (Tau_r+Ts) */   
    float_t fltPsiRB1Gain;                       /* Constant determined by: Lm*Ts / (Tau_r) */   
    float_t fltTs;                               /* Sample time constant */  
    float_t fltSpeedMeGain;                      /* Speed gain constant given by: 60 / (2*PI*PolePairs) */
} AMCLIB_ACIM_SPEED_MRAS_T_FLT;   
   
/****************************************************************************
* Exported function prototypes
****************************************************************************/   
extern void AMCLIB_ACIMSpeedMRAS_FLT_FC(const GMCLIB_2COOR_ALBE_T_FLT *psISAlBe,    
                                        const GMCLIB_2COOR_ALBE_T_FLT *psPsiRAlBe,  
                                        acc32_t a32RotPos,
                                        AMCLIB_ACIM_SPEED_MRAS_T_FLT *psCtrl);
RAM_FUNC_LIB
extern void AMCLIB_ACIMSpeedMRAS_FLT_FCRam(const GMCLIB_2COOR_ALBE_T_FLT *psISAlBe,    
                                           const GMCLIB_2COOR_ALBE_T_FLT *psPsiRAlBe,  
                                           acc32_t a32RotPos,
                                           AMCLIB_ACIM_SPEED_MRAS_T_FLT *psCtrl); 
                                        
/****************************************************************************
* Inline functions 
****************************************************************************/                                 

/***************************************************************************//*!
* @brief  The function initializes the actual values of float AMCLIB_ACIMSpeedMRAS function.
*
* @params:      ptr  AMCLIB_ACIM_SPEED_MRAS_T_FLT *psParam - Pointer to MRAS structure                
*
* @return       N/A
* 
*******************************************************************************/
RAM_FUNC_LIB 
static inline void AMCLIB_ACIMSpeedMRASInit_FLT_FCi(AMCLIB_ACIM_SPEED_MRAS_T_FLT *psCtrl)
{
    GDFLIB_FilterIIR1Init_FLT (&psCtrl->fltSpeedElIIR1Param);
    psCtrl->sPsiRotRDQ.fltD         = (float_t) 0.0F;
    psCtrl->sPsiRotRDQ.fltQ         = (float_t) 0.0F;    
    psCtrl->fltSpeedEl              = (float_t) 0.0F;
    psCtrl->fltSpeedElIIR1          = (float_t) 0.0F; 
    psCtrl->fltSpeedMeIIR1          = (float_t) 0.0F; 
    psCtrl->a32RotPos               = (acc32_t) 0;
    psCtrl->sCtrl.fltSpeedElInteg_1 = (float_t) 0.0F;    
    psCtrl->sCtrl.fltSpeedElErr_1   = (float_t) 0.0F;
}
   
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_ACIMSPEEDMRAS_FLT_H_ */
