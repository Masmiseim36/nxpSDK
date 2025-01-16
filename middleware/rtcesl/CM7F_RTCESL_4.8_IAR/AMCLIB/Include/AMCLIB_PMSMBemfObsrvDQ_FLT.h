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
* @brief  Algorithm of float PMSM Back Electromotive Force observer in rotating 
*         reference frame       
* 
*******************************************************************************/
#ifndef _AMCLIB_PMSM_BEMF_OBSRV_FLT_DQ_H_
#define _AMCLIB_PMSM_BEMF_OBSRV_FLT_DQ_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "amclib_types.h"
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "GFLIB_AtanYX_FLT.h"    

/*******************************************************************************
* Macros 
*******************************************************************************/    
#define AMCLIB_PMSMBemfObsrvDQ_A32fff_C(psIDQ, psUDQ, fltSpeed, psCtrl)        \
        AMCLIB_PMSMBemfObsrvDQ_A32fff_FC(psIDQ, psUDQ, fltSpeed, psCtrl)
#define AMCLIB_PMSMBemfObsrvDQ_A32fff_CRam(psIDQ, psUDQ, fltSpeed, psCtrl)     \
        AMCLIB_PMSMBemfObsrvDQ_A32fff_FCRam(psIDQ, psUDQ, fltSpeed, psCtrl)
#define AMCLIB_PMSMBemfObsrvDQInit_A32fff_Ci(psCtrl)                           \
        AMCLIB_PMSMBemfObsrvDQInit_A32fff_FCi(psCtrl)
        
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{       
    GMCLIB_2COOR_DQ_T_FLT sEObsrv;  /* Estimated back-EMF voltage - d/q */
    GMCLIB_2COOR_DQ_T_FLT sIObsrv;  /* Estimated current  - d/q */
    
    /* Observer parameters for controllers */
    struct
    {
        float_t  fltID_1;      /* Integral part state variable for D coefficient */           
        float_t  fltIQ_1;      /* Integral part state variable for Q coefficient */    
        float_t  fltPGain;     /* Observer proportional gain coefficient */                               
        float_t  fltIGain;     /* Observer integral gain coefficient */ 
    } sCtrl;               
    
    /* Configuration parameters */
    float_t      fltIGain;     /* Current scaling coefficient */                        
    float_t      fltUGain;     /* Voltage scaling coefficient */                        
    float_t      fltWIGain;    /* Angular speed scaling coefficient */              
    float_t      fltEGain;     /* Back-emf scaling coefficient */
    
    /* Estimated phase error between real d/q frame system and estimated d/q 
    reference system */
    acc32_t      a32Error;                                       
       
} AMCLIB_BEMF_OBSRV_DQ_T_FLT;
   
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern acc32_t AMCLIB_PMSMBemfObsrvDQ_A32fff_FC(const GMCLIB_2COOR_DQ_T_FLT *psIDQ,
                                                const GMCLIB_2COOR_DQ_T_FLT *psUDQ,
                                                float_t fltSpeed, 
                                                AMCLIB_BEMF_OBSRV_DQ_T_FLT *psCtrl);
RAM_FUNC_LIB 
extern acc32_t AMCLIB_PMSMBemfObsrvDQ_A32fff_FCRam(const GMCLIB_2COOR_DQ_T_FLT *psIDQ,
                                                   const GMCLIB_2COOR_DQ_T_FLT *psUDQ,
                                                   float_t fltSpeed, 
                                                   AMCLIB_BEMF_OBSRV_DQ_T_FLT *psCtrl);
/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    PMSM BEMF in DQ reference frame initialization
*
* @param    ptr - AMCLIB_BEMF_OBSRV_DQ_T_FLT *psCtrl - pointer to the observer parameters 
*
* @param    in  - None
*
* @return   None
*                         
* @remarks  Initializes the structure of the PMSM BEMF in DQ reference frame
*           
*    sIObsrv_fltD  = 0;
*    sIObsrv_fltQ  = 0;  
*    sCtrl_fltID_1 = 0;     
*    sCtrl_fltIQ_1 = 0; 
*
****************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline void AMCLIB_PMSMBemfObsrvDQInit_A32fff_FCi(AMCLIB_BEMF_OBSRV_DQ_T_FLT *psCtrl)
{
    psCtrl -> sIObsrv.fltD  = 0.0F;
    psCtrl -> sIObsrv.fltQ  = 0.0F;  
    psCtrl -> sCtrl.fltID_1 = 0.0F;     
    psCtrl -> sCtrl.fltIQ_1 = 0.0F;      
}
 
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_PMSM_BEMF_OBSRV_FLT_DQ_H_ */       
