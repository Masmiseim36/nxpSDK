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
#define AMCLIB_CtrlFluxWkng_F16_CRam(f16IQErr, f16UQReq, f16UQLim, psCtrl)    \
        AMCLIB_CtrlFluxWkng_F16_FCRam(f16IQErr, f16UQReq, f16UQLim, psCtrl)		
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
RAM_FUNC_LIB 
extern frac16_t AMCLIB_CtrlFluxWkng_F16_FCRam(frac16_t f16IQErr, frac16_t f16UQReq, frac16_t f16UQLim,
                                              AMCLIB_CTRL_FLUX_WKNG_T_A32 *psCtrl);
										   
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
RAM_FUNC_LIB 
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
