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
#define GMCLIB_DecouplingPMSM_F16_Asm(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)    \
        GMCLIB_DecouplingPMSM_F16_FAsm(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)
#define GMCLIB_DecouplingPMSM_F16_AsmRam(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec) \
        GMCLIB_DecouplingPMSM_F16_FAsmRam(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)

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

RAM_FUNC_LIB 
extern void GMCLIB_DecouplingPMSM_F16_FAsmRam(const GMCLIB_2COOR_DQ_T_F16 *psUDQ,
                                              const GMCLIB_2COOR_DQ_T_F16 *psIDQ,
                                              frac16_t f16SpeedEl,
                                              const GMCLIB_DECOUPLINGPMSM_T_A32 *psParam,
                                              GMCLIB_2COOR_DQ_T_F16 *psUDQDec);

#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_DECOUPLING_A32_H_ */
