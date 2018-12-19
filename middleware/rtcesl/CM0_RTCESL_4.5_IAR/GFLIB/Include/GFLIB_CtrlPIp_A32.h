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
* @brief  Parallel PI controller with and without anti-wind-up protection
* 
*******************************************************************************/
#ifndef _GFLIB_CTRLPIP_A32_H_
#define _GFLIB_CTRLPIP_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros            
*******************************************************************************/    
#define GFLIB_CtrlPIpAW_F16_Asm(f16InErr, pbStopIntegFlag, psParam)          \
        GFLIB_CtrlPIpAW_F16_FAsm(f16InErr, pbStopIntegFlag, psParam)
#define GFLIB_CtrlPIpAWInit_F16_Ci(f16InitVal, psParam)                      \
        GFLIB_CtrlPIpAWInit_F16_FCi(f16InitVal, psParam)
    
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    acc32_t   a32PGain;        /* Proportional Gain */
    acc32_t   a32IGain;        /* Integral Gain */
    frac32_t  f32IAccK_1;      /* State variable output at step k-1 */
    frac16_t  f16InErrK_1;     /* State variable input error at step k-1 */
    frac16_t  f16UpperLim;     /* Upper Limit of the controller */
    frac16_t  f16LowerLim;     /* Lower Limit of the controller */
    bool_t    bLimFlag;        /* Limitation flag, if set to 1, the controller 
                                  output has reached either the UpperLimit or LowerLimit */
}GFLIB_CTRL_PI_P_AW_T_A32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_CtrlPIpAW_F16_FAsm(frac16_t f16InErr,
                                       const bool_t *pbStopIntegFlag,
                                       GFLIB_CTRL_PI_P_AW_T_A32 *psParam);
  
/***************************************************************************//*!
* @brief    The function initializes the actual values of CtrlPIpAWInit controller.
*
* @params   in   frac16_t f16InitVal - Initial state value for Integrating part
*           ptr  GFLIB_CTRL_PI_P_AW_T_A32 *psParam - Pointer to controller structure
*
* @return   N/A
* 
*******************************************************************************/
static inline void GFLIB_CtrlPIpAWInit_F16_FCi(frac16_t f16InitVal, 
                                               GFLIB_CTRL_PI_P_AW_T_A32 *psParam)
{
    psParam->f32IAccK_1 = (frac32_t)(f16InitVal << 16);
    psParam->f16InErrK_1 = (frac16_t)0;
}
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_CTRLPIP_A32_H_ */
