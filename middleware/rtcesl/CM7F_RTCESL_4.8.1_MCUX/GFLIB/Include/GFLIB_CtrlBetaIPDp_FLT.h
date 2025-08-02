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
* @brief  Float parallel beta IP controller with and without anti-wind-up protection
* 
*******************************************************************************/
#ifndef _GFLIB_CTRLBETAIPDP_FLT_H_
#define _GFLIB_CTRLBETAIPDP_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros            
*******************************************************************************/           
#define GFLIB_CtrlBetaIPDpAW_FLT_C(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam)     \
        GFLIB_CtrlBetaIPDpAW_FLT_FC(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam)
#define GFLIB_CtrlBetaIPDpAW_FLT_CRam(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam)  \
        GFLIB_CtrlBetaIPDpAW_FLT_FCRam(fltInReq, fltIn, fltInErrD, pbStopIntegFlag, psParam)		
#define GFLIB_CtrlBetaIPDpAWInit_FLT_Ci(fltInitVal, psParam)                                 \
        GFLIB_CtrlBetaIPDpAWInit_FLT_FCi(fltInitVal, psParam)         
  
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t  fltPGain;     /* Proportional Gain */
    float_t  fltIGain;     /* Integral Gain */
    float_t  fltDGain;     /* Derivative Gain */
    float_t  fltIAccK_1;   /* State variable output at step k-1 */
    float_t  fltInErrK_1;  /* State variable input error at step k-1 */
    float_t  fltUpperLim;  /* Upper Limit of the controller */
    float_t  fltLowerLim;  /* Lower Limit of the controller */
    float_t  fltInErrDK_1; /* State variable input error at step k-1 */  
    float_t  fltBetaGain;  /* Beta Gain for restricting the overshot */ 
    bool_t   bLimFlag;     /* Limitation flag, if set to 1, the controller output
                              has reached either the UpperLimit or LowerLimit */
} GFLIB_CTRL_BETA_IPD_P_AW_T_FLT;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_CtrlBetaIPDpAW_FLT_FC(float_t fltInReq, float_t fltIn, float_t fltInErrD,
                                           const bool_t *pbStopIntegFlag, 
                                           GFLIB_CTRL_BETA_IPD_P_AW_T_FLT *psParam);
RAM_FUNC_LIB 
extern float_t GFLIB_CtrlBetaIPDpAW_FLT_FCRam(float_t fltInReq, float_t fltIn, float_t fltInErrD,
                                              const bool_t *pbStopIntegFlag, 
                                              GFLIB_CTRL_BETA_IPD_P_AW_T_FLT *psParam);
                                      
/***************************************************************************//*!
* @brief    The float function initializes the actual values of CtrlBetaIPDpAWInit controller.
*
* @params   in  float_t fltInitVal- Initial state value for Integrating part
*           ptr GFLIB_CTRL_BETA_IPD_P_AW_T_FLT *psParam - Pointer to controller structure
*
* @return   N/A
* 
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline void GFLIB_CtrlBetaIPDpAWInit_FLT_FCi(float_t fltInitVal,
                                                    GFLIB_CTRL_BETA_IPD_P_AW_T_FLT *psParam)
{
    psParam->fltIAccK_1  = fltInitVal;
    psParam->fltInErrK_1 = 0.0F;
    psParam->fltInErrDK_1= 0.0F;
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_CTRLBETAIPDP_FLT_H_ */
