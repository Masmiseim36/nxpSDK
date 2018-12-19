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
* @brief  Algorithm of float PMSM Back Electromotive Force observer in stationary 
*         reference frame    
* 
*******************************************************************************/
#ifndef _AMCLIB_PMSM_BEMF_OBSRV_FLT_AB_H_
#define _AMCLIB_PMSM_BEMF_OBSRV_FLT_AB_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "amclib_types.h"
#include "mlib_FP.h"
#include "gflib_FP.h"

/*******************************************************************************
* Macros 
*******************************************************************************/    
#define AMCLIB_PMSMBemfObsrvAB_FLT_C(psIAlBe, psUAlBe, fltSpeed, psCtrl)        \
        AMCLIB_PMSMBemfObsrvAB_FLT_FC(psIAlBe, psUAlBe, fltSpeed, psCtrl)
#define AMCLIB_PMSMBemfObsrvABInit_FLT_Ci(psCtrl)                               \
        AMCLIB_PMSMBemfObsrvABInit_FLT_FCi( psCtrl)            
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{       
    GMCLIB_2COOR_ALBE_T_FLT sEObsrv;  /* Estimated back-EMF voltage - alpha/beta */
    GMCLIB_2COOR_ALBE_T_FLT sIObsrv;  /* Estimated current  - alpha/beta */
    
    /* Observer parameters for controllers */
    struct
    {
        float_t  fltIAlpha_1;  /* Integral part state variable for alpha coefficient */           
        float_t  fltIBeta_1;   /* Integral part state variable for beta coefficient */    
        float_t  fltPGain;     /* Observer proportional gain coefficient */                               
        float_t  fltIGain;     /* Observer integral gain coefficient */ 
    } sCtrl;               
    
    /* Configuration parameters */
    float_t      fltIGain;     /* Current scaling coefficient */                        
    float_t      fltUGain;     /* Voltage scaling coefficient */                        
    float_t      fltWIGain;    /* Angular speed scaling coefficient */              
    float_t      fltEGain;     /* Back-emf scaling coefficient */
       
    /* Unity vector */
    GMCLIB_2COOR_SINCOS_T_FLT sUnityVctr;                                     
       
} AMCLIB_BEMF_OBSRV_AB_T_FLT;
   
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern void AMCLIB_PMSMBemfObsrvAB_FLT_FC(const GMCLIB_2COOR_ALBE_T_FLT *psIAlBe,
                                          const GMCLIB_2COOR_ALBE_T_FLT *psUAlBe,
                                          float_t fltSpeed, 
                                          AMCLIB_BEMF_OBSRV_AB_T_FLT *psCtrl
);

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    PMSM BEMF in AB reference frame initialization
*
* @param    ptr   AMCLIB_BEMF_OBSRV_AB_T_FLT *psCtrl - pointer to the parameters of the observer
*
* @param  in    None
*
* @return       None
*                         
* @remarks  Initializes the structure of the PMSM BEMF in AB reference frame
*           
*    sIObsrv_fltAlpha = 0;
*    sIObsrv_fltbeta  = 0;  
*    sCtrl_fltIAlpha_1= 0;     
*    sCtrl_fltIBeta_1 = 0; 
*
****************************************************************************/
static inline void AMCLIB_PMSMBemfObsrvABInit_FLT_FCi(AMCLIB_BEMF_OBSRV_AB_T_FLT *psCtrl) 
{
    psCtrl -> sIObsrv.fltAlpha = 0.0F;
    psCtrl -> sIObsrv.fltBeta  = 0.0F;  
    psCtrl -> sCtrl.fltIAlpha_1= 0.0F;     
    psCtrl -> sCtrl.fltIBeta_1 = 0.0F;      
}
 
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_PMSM_BEMF_OBSRV_FLT_AB_H_ */       
