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
* @brief  Algorithm of PMSM Back Electromotive Force observer in stationary 
*         reference frame
* 
*******************************************************************************/
#ifndef _AMCLIB_PMSM_BEMF_OBSRV_A32_AB_H_
#define _AMCLIB_PMSM_BEMF_OBSRV_A32_AB_H_

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
#define AMCLIB_PMSMBemfObsrvAB_F16_C(psIAlBe, psUAlBe, f16Speed, psCtrl)       \
        AMCLIB_PMSMBemfObsrvAB_F16_FC(psIAlBe, psUAlBe, f16Speed, psCtrl)
#define AMCLIB_PMSMBemfObsrvABInit_F16_Ci(psCtrl)                               \
        AMCLIB_PMSMBemfObsrvABInit_F16_FCi(psCtrl)            
/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
    GMCLIB_2COOR_ALBE_T_F32 sEObsrv;  /* Estimated back-EMF voltage - alpha,beta */            
    GMCLIB_2COOR_ALBE_T_F32 sIObsrv;  /* Estimated current  - alpha,beta */            
    
    /* Observer parameters for controllers */
    struct
    {
        frac32_t f32IAlpha_1;  /* Integral part state variable for alpha coefficient */     
        frac32_t f32IBeta_1;   /* Integral part state variable for beta coefficient */ 
        acc32_t  a32PGain;     /* Observer proportional gain coefficient */                           
        acc32_t  a32IGain;     /* Observer integral gain coefficient */                             
    } sCtrl;
    
    /* Configuration parameters */ 
    acc32_t  a32IGain;         /* Current scaling coefficient */           
    acc32_t  a32UGain;         /* Voltage scaling coefficient */               
    acc32_t  a32WIGain;        /* Angular speed scaling coefficient */         
    acc32_t  a32EGain;         /* Back-emf scaling coefficient */              
    
    /* Unity vector */
    GMCLIB_2COOR_SINCOS_T_F16 sUnityVctr;         
    
} AMCLIB_BEMF_OBSRV_AB_T_A32;
   
/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern void AMCLIB_PMSMBemfObsrvAB_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIAlBe,
                                          const GMCLIB_2COOR_ALBE_T_F16 *psUAlBe,
                                          frac16_t f16Speed,
                                          AMCLIB_BEMF_OBSRV_AB_T_A32 *psCtrl);

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    PMSM BEMF in AB reference frame initialization
*
* @param    ptr   AMCLIB_BEMF_OBSRV_AB_T_A32 *psCtrl - pointer to the parameters of the observer
*
* @param  in    None
*
* @return       None
*                         
* @remarks  Initializes the structure of the PMSM BEMF in AB reference frame
*           
*    sIObsrv_f32Alpha = 0;
*    sIObsrv_f32beta  = 0;  
*    sCtrl_f32IAlpha_1= 0;     
*    sCtrl_f32IBeta_1 = 0; 
*
****************************************************************************/
static inline void AMCLIB_PMSMBemfObsrvABInit_F16_FCi(AMCLIB_BEMF_OBSRV_AB_T_A32 *psCtrl)
{
    psCtrl -> sIObsrv.f32Alpha = 0;
    psCtrl -> sIObsrv.f32Beta  = 0;
    psCtrl -> sCtrl.f32IAlpha_1= 0;
    psCtrl -> sCtrl.f32IBeta_1 = 0;
}
 
#if defined(__cplusplus) 
} 
#endif 

#endif /* _AMCLIB_PMSM_BEMF_OBSRV_A32_AB_H_ */    
