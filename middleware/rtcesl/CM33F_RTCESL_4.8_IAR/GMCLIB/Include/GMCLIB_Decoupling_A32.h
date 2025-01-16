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

#ifndef _GMCLIB_DECOUPLING_A32_H_
#define _GMCLIB_DECOUPLING_A32_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/****************************************************************************
* Includes
****************************************************************************/  
#include "mlib.h"
#include "gmclib_types.h" 
  
/******************************************************************************
* Macros 
******************************************************************************/   
#define GMCLIB_DecouplingPMSM_F16_Asm(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec) \
        GMCLIB_DecouplingPMSM_F16_FAsm(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)

/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{
    acc32_t a32KdGain; 
    acc32_t a32KqGain; 
}GMCLIB_DECOUPLINGPMSM_T_A32;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern void GMCLIB_DecouplingPMSM_F16_FAsm(const GMCLIB_2COOR_DQ_T_F16 *psUDQ,
                                           const GMCLIB_2COOR_DQ_T_F16 *psIDQ,
                                           frac16_t f16SpeedEl,
                                           const GMCLIB_DECOUPLINGPMSM_T_A32 *psParam,
                                           GMCLIB_2COOR_DQ_T_F16 *psUDQDec);

#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_DECOUPLING_A32_H_ */
