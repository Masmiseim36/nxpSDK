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
#include "gdflib_fp.h"  
#include "gflib_fp.h"     
#include "gmclib_fp.h"     
#include "mlib_fp.h"

/******************************************************************************
* Macros 
******************************************************************************/      
#define AMCLIB_ACIMSpeedMRAS_FLT_C( psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl) \
        AMCLIB_ACIMSpeedMRAS_FLT_FC(psIsAlBe, psPsiRAlBe, a32RotPos,  psCtrl) 
#define AMCLIB_ACIMSpeedMRASInit_FLT_Ci( psCtrl)                              \
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
    float_t fltPsiRB1Gain;                       /* Constant determined by: Lm*Ts / (Tau_r+Ts) */   
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
