/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#define GFLIB_CtrlPIpAW_F16_Asm(f16InErr, pbStopIntegFlag, psParam)           \
        GFLIB_CtrlPIpAW_F16_FAsm(f16InErr, pbStopIntegFlag, psParam)
#define GFLIB_CtrlPIpAW_F16_AsmRam(f16InErr, pbStopIntegFlag, psParam)        \
        GFLIB_CtrlPIpAW_F16_FAsmRam(f16InErr, pbStopIntegFlag, psParam)
#define GFLIB_CtrlPIpAWInit_F16_Ci(f16InitVal, psParam)                       \
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
RAM_FUNC_LIB 
extern frac16_t GFLIB_CtrlPIpAW_F16_FAsmRam(frac16_t f16InErr,
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
RAM_FUNC_LIB 
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
