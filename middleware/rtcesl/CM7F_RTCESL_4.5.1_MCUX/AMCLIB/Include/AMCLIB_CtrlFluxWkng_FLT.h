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
#ifndef _AMCLIB_CTRL_FLUX_WKNG_FLT_H_
#define _AMCLIB_CTRL_FLUX_WKNG_FLT_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "mlib_FP.h"      
#include "gflib_FP.h"     
#include "gdflib_FP.h"    

/******************************************************************************
* Macros 
******************************************************************************/
#define AMCLIB_CtrlFluxWkng_FLT_C(fltIQErr, fltUQReq, fltUQLim, psCtrl)        \
        AMCLIB_CtrlFluxWkng_FLT_FC(fltIQErr, fltUQReq, fltUQLim, psCtrl)                         
#define AMCLIB_CtrlFluxWkng_FLT_CRam(fltIQErr, fltUQReq, fltUQLim, psCtrl)     \
        AMCLIB_CtrlFluxWkng_FLT_FCRam(fltIQErr, fltUQReq, fltUQLim, psCtrl)       
#define AMCLIB_CtrlFluxWkngInit_FLT_Ci(fltInitVal, psCtrl)                     \
        AMCLIB_CtrlFluxWkngInit_FLT_FCi(fltInitVal, psCtrl)   

/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
    /* function parameters */
    GFLIB_CTRL_PI_P_AW_T_FLT sFWPiParam;      /* PIp controller structure for FW controller */ 
    GDFLIB_FILTER_IIR1_T_FLT sIqErrIIR1Param; /* Iq error IIR filter structure for Iq measured */ 
    
    /* function parameters */
    float_t fltIqErrIIR1;     /* Iq filtered by IIR - calculated by algorithm */
    float_t fltUFWErr;        /* Required voltage error - calculated by algorithm */
    float_t fltFWErr;         /* Flux weakening error - calculated by algorithm */
    float_t fltIGainUgain;    /* Gain for proper float scaling: fltIGainUgain = IGain / Ugain, fltIGainUgain > 0 */
    bool_t  *pbStopIntegFlag; /* Flux weakening controller stop integration input flag 
                                 Set from application when saturation occurs */
} AMCLIB_CTRL_FLUX_WKNG_T_FLT;
   
/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern float_t AMCLIB_CtrlFluxWkng_FLT_FC(float_t fltIQErr, float_t fltUQReq, float_t fltUQLim,
                                          AMCLIB_CTRL_FLUX_WKNG_T_FLT *psCtrl);
RAM_FUNC_LIB 
extern float_t AMCLIB_CtrlFluxWkng_FLT_FCRam(float_t fltIQErr, float_t fltUQReq, float_t fltUQLim,
                                             AMCLIB_CTRL_FLUX_WKNG_T_FLT *psCtrl);

/***************************************************************************//*!
*
* @brief  Initialization for the Flux weakening control function
*
* @param  ptr AMCLIB_CTRL_FLUX_WKNG_T_FLT *psCtrl - Pointer to controller structure 
* @param  in  float_t fltInitVal - Initial state value for CtrlPIpAW integrating part            
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
static inline void AMCLIB_CtrlFluxWkngInit_FLT_FCi(float_t fltInitVal,
                                                   AMCLIB_CTRL_FLUX_WKNG_T_FLT *psCtrl)
{
    GFLIB_CtrlPIpAWInit_FLT(fltInitVal, &psCtrl->sFWPiParam);
    GDFLIB_FilterIIR1Init_FLT (&psCtrl->sIqErrIIR1Param);             
}


#if defined(__cplusplus) 
} 
#endif 

#endif /* _AMCLIB_CTRL_FLUX_WKNG_FLT_H_ */
