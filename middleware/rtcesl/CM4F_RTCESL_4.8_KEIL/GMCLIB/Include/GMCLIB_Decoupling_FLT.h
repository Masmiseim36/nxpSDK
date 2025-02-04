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
* @brief  Decoupling calculation
* 
*******************************************************************************/

#ifndef _GMCLIB_DECOUPLING_FLT_H_
#define _GMCLIB_DECOUPLING_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/****************************************************************************
* Includes
****************************************************************************/  
#include "mlib_FP.h"
#include "gmclib_types.h" 
  
/******************************************************************************
* Macros 
******************************************************************************/   
#define GMCLIB_DecouplingPMSM_FLT_C(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec) \
        GMCLIB_DecouplingPMSM_FLT_FC(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)

/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{
    float_t fltLd; 
    float_t fltLq; 
}GMCLIB_DECOUPLINGPMSM_T_FLT;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern void GMCLIB_DecouplingPMSM_FLT_FC(const GMCLIB_2COOR_DQ_T_FLT *psUDQ,
                                         const GMCLIB_2COOR_DQ_T_FLT *psIDQ,
                                         float_t fltSpeedEl,
                                         const GMCLIB_DECOUPLINGPMSM_T_FLT *psParam,
                                         GMCLIB_2COOR_DQ_T_FLT *psUDQDec);

#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_DECOUPLING_FLT_H_ */
