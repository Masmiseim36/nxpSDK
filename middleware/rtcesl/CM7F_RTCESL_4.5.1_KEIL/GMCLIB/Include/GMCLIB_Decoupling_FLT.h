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
#define GMCLIB_DecouplingPMSM_FLT_C(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)    \
        GMCLIB_DecouplingPMSM_FLT_FC(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)
#define GMCLIB_DecouplingPMSM_FLT_CRam(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec) \
        GMCLIB_DecouplingPMSM_FLT_FCRam(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)

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

RAM_FUNC_LIB 
extern void GMCLIB_DecouplingPMSM_FLT_FCRam(const GMCLIB_2COOR_DQ_T_FLT *psUDQ,
                                            const GMCLIB_2COOR_DQ_T_FLT *psIDQ,
                                            float_t fltSpeedEl,
                                            const GMCLIB_DECOUPLINGPMSM_T_FLT *psParam,
                                            GMCLIB_2COOR_DQ_T_FLT *psUDQDec);

#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_DECOUPLING_FLT_H_ */
