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
* @brief ACIM flux MTPA algotitm   
* 
*******************************************************************************/
#ifndef _AMCLIB_ACIMFLUXMTPA_FLT_H_
#define _AMCLIB_ACIMFLUXMTPA_FLT_H_

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
#define AMCLIB_ACIMCtrlMTPAInit_FLT_Ci( fltIDMin, fltIDMax, psCtrl)           \
        AMCLIB_ACIMCtrlMTPAInit_FLT_FCi(fltIDMin, fltIDMax, psCtrl)                            
#define AMCLIB_ACIMCtrlMTPA_FLT_C( fltIq, psCtrl)                             \
        AMCLIB_ACIMCtrlMTPA_FLT_FC(fltIq, psCtrl)                              
#define AMCLIB_ACIMCtrlMTPA_FLT_CRam( fltIq, psCtrl)                          \
        AMCLIB_ACIMCtrlMTPA_FLT_FCRam(fltIq, psCtrl)
          
/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{   
    GDFLIB_FILTER_EXP_T_FLT sIdExpParam;   /* Exponential filter structure for Id measured */ 
    float_t fltLowerLim;                     /* Min value of Id required */
    float_t fltUpperLim;                     /* Max value of Id required */ 
} AMCLIB_ACIM_CTRL_MTPA_T_FLT;   
   
/****************************************************************************
* Exported function prototypes
****************************************************************************/   
extern float_t AMCLIB_ACIMCtrlMTPA_FLT_FC(float_t fltIq,
                                          AMCLIB_ACIM_CTRL_MTPA_T_FLT *psCtrl); 
RAM_FUNC_LIB 
extern float_t AMCLIB_ACIMCtrlMTPA_FLT_FCRam(float_t fltIq,
                                             AMCLIB_ACIM_CTRL_MTPA_T_FLT *psCtrl); 
                                        
/****************************************************************************
* Inline functions 
****************************************************************************/                                 

/***************************************************************************//*!
* @brief  The function initializes the actual values of float AMCLIB_ACIMCtrlMTPA_FLT function.
*
* @params:      ptr  AMCLIB_ACIM_CTRL_MTPA_T_FLT *psParam - Pointer to ACIM flux MTPA 
*                                                           function structure                
*
* @return       N/A
* 
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline void AMCLIB_ACIMCtrlMTPAInit_FLT_FCi(float_t fltIDMin, float_t fltIDMax, 
                                                   AMCLIB_ACIM_CTRL_MTPA_T_FLT *psCtrl)
{
    GDFLIB_FilterExpInit_FLT(0.0F, &psCtrl->sIdExpParam);
    psCtrl->fltLowerLim = fltIDMin;
    psCtrl->fltUpperLim = fltIDMax;    
}
   
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_ACIMFLUXMTPA_FLT_H_ */
