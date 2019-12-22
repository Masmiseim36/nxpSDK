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
#include "gdflib_fp.h"  
#include "gflib_fp.h"     
#include "gmclib_fp.h"     
#include "mlib_fp.h"

/******************************************************************************
* Macros 
******************************************************************************/      
#define AMCLIB_ACIMCtrlMTPAInit_FLT_Ci( fltIDMin, fltIDMax, psCtrl)           \
        AMCLIB_ACIMCtrlMTPAInit_FLT_FCi(fltIDMin, fltIDMax, psCtrl)                            
#define AMCLIB_ACIMCtrlMTPA_FLT_C( fltIq, psCtrl)                             \
        AMCLIB_ACIMCtrlMTPA_FLT_FC(fltIq, psCtrl)                              
            
/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{   
    GDFLIB_FILTER_EXP_T_FLT fltIdExpParam;   /* Exponential filter structure for Id measured */ 
    float_t fltLowerLim;                     /* Min value of Id required */
    float_t fltUpperLim;                     /* Max value of Id required */ 
} AMCLIB_ACIM_CTRL_MTPA_T_FLT;   
   
/****************************************************************************
* Exported function prototypes
****************************************************************************/   
extern float_t AMCLIB_ACIMCtrlMTPA_FLT_FC(float_t fltIq,
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
static inline void AMCLIB_ACIMCtrlMTPAInit_FLT_FCi(float_t fltIDMin, float_t fltIDMax, 
                                                   AMCLIB_ACIM_CTRL_MTPA_T_FLT *psCtrl)
{
    GDFLIB_FilterExpInit_FLT(0.0F, &psCtrl->fltIdExpParam);
    psCtrl->fltLowerLim = fltIDMin;
    psCtrl->fltUpperLim = fltIDMax;    
}
   
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_ACIMFLUXMTPA_FLT_H_ */
