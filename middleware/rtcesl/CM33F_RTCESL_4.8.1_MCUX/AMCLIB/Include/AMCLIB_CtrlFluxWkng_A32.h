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
* @brief   Flux weakening controller
* 
*******************************************************************************/
#ifndef _AMCLIB_CTRL_FLUX_WKNG_A32_H_
#define _AMCLIB_CTRL_FLUX_WKNG_A32_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/      
#include "gflib.h"     
#include "gdflib.h"    

/******************************************************************************
* Macros 
******************************************************************************/
#define AMCLIB_CtrlFluxWkng_F16_C(f16IQErr, f16UQReq, f16UQLim, psCtrl)       \
        AMCLIB_CtrlFluxWkng_F16_FC(f16IQErr, f16UQReq, f16UQLim, psCtrl)                        
#define AMCLIB_CtrlFluxWkngInit_F16_Ci(f16InitVal, psCtrl)                    \
        AMCLIB_CtrlFluxWkngInit_F16_FCi(f16InitVal, psCtrl)

/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
    /* function parameters */
    GFLIB_CTRL_PI_P_AW_T_A32 sFWPiParam;     /* PIp controller structure for FW controller */ 
    GDFLIB_FILTER_IIR1_T_F32 sIqErrIIR1Param; /* Iq error IIR filter structure for Iq measured */ 
    
    /* function parameters */
    frac16_t f16IqErrIIR1;     /* Iq filtered by IIR - calculated by algorithm  */
    frac16_t f16UFWErr;        /* Required voltage error - calculated by algorithm  */
    frac16_t f16FWErr;         /* Flux weakening error - calculated by algorithm  */              
    bool_t   *pbStopIntegFlag; /* Flux weakening controller stop integration flag - set by application */  
} AMCLIB_CTRL_FLUX_WKNG_T_A32;
   
/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern frac16_t AMCLIB_CtrlFluxWkng_F16_FC(frac16_t f16IQErr, frac16_t f16UQReq, frac16_t f16UQLim,
                                           AMCLIB_CTRL_FLUX_WKNG_T_A32 *psCtrl);

/***************************************************************************//*!
*
* @brief  Initialization for the Flux weakening control function
*
* @param  ptr AMCLIB_CTRL_FLUX_WKNG_T_A32 *psCtrl - Pointer to controller structure 
* @param  in  frac16_t f16InitVal - Initial state value for CtrlPIpAW integrating part            
*
* @return none
*
* @remarks  The function initializes Flux weakening control function by following:
*
*  - PI controller with anti-wind-up protection initialization. 
*  - Iq IIR filter initialization
*
****************************************************************************/
static inline void AMCLIB_CtrlFluxWkngInit_F16_FCi(frac16_t f16InitVal,
                                                   AMCLIB_CTRL_FLUX_WKNG_T_A32 *psCtrl)
{
    GFLIB_CtrlPIpAWInit_F16(f16InitVal, &psCtrl->sFWPiParam);
    GDFLIB_FilterIIR1Init_F16 (&psCtrl->sIqErrIIR1Param);
}

#if defined(__cplusplus) 
} 
#endif 

#endif /* _AMCLIB_CTRL_FLUX_WKNG_A32_H_ */
