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
* @brief Space vector modulation 
* 
*******************************************************************************/
#ifndef _GMCLIB_SVM_F16_H_
#define _GMCLIB_SVM_F16_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "mlib.h"
#include "gmclib_types.h"
  
/*******************************************************************************
* Macros 
*******************************************************************************/ 
#define GMCLIB_SvmStd_F16_C(psIn, psOut)             GMCLIB_SvmStd_F16_FC(psIn, psOut)
#define GMCLIB_SvmIct_F16_C(psIn, psOut)             GMCLIB_SvmIct_F16_FC(psIn, psOut)   
#define GMCLIB_SvmU7n_F16_C(psIn, psOut)             GMCLIB_SvmU7n_F16_FC(psIn, psOut)  
#define GMCLIB_SvmU0n_F16_C(psIn, psOut)             GMCLIB_SvmU0n_F16_FC(psIn, psOut) 
#define GMCLIB_SvmDpwm_F16_C(psIn, psOut)            GMCLIB_SvmDpwm_F16_FC(psIn, psOut) 
#define GMCLIB_SvmExDpwm_F16_C(psIn, psAngle, psOut) GMCLIB_SvmExDpwm_F16_FC(psIn, psAngle,psOut)

#define GMCLIB_SvmStd_F16_CRam(psIn, psOut)             GMCLIB_SvmStd_F16_FCRam(psIn, psOut)
#define GMCLIB_SvmIct_F16_CRam(psIn, psOut)             GMCLIB_SvmIct_F16_FCRam(psIn, psOut)   
#define GMCLIB_SvmU7n_F16_CRam(psIn, psOut)             GMCLIB_SvmU7n_F16_FCRam(psIn, psOut)  
#define GMCLIB_SvmU0n_F16_CRam(psIn, psOut)             GMCLIB_SvmU0n_F16_FCRam(psIn, psOut) 
#define GMCLIB_SvmDpwm_F16_CRam(psIn, psOut)            GMCLIB_SvmDpwm_F16_FCRam(psIn, psOut) 
#define GMCLIB_SvmExDpwm_F16_CRam(psIn, psAngle, psOut) GMCLIB_SvmExDpwm_F16_FCRam(psIn, psAngle,psOut)        
    
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/   
extern uint16_t GMCLIB_SvmStd_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

RAM_FUNC_LIB 
extern uint16_t GMCLIB_SvmStd_F16_FCRam(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

extern uint16_t GMCLIB_SvmIct_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

RAM_FUNC_LIB 
extern uint16_t GMCLIB_SvmIct_F16_FCRam(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

extern uint16_t GMCLIB_SvmU7n_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

RAM_FUNC_LIB 
extern uint16_t GMCLIB_SvmU7n_F16_FCRam(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

extern uint16_t GMCLIB_SvmU0n_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

RAM_FUNC_LIB 
extern uint16_t GMCLIB_SvmU0n_F16_FCRam(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

extern uint16_t GMCLIB_SvmDpwm_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

RAM_FUNC_LIB 
extern uint16_t GMCLIB_SvmDpwm_F16_FCRam(const GMCLIB_2COOR_ALBE_T_F16 *psIn, GMCLIB_3COOR_T_F16 *psOut);

extern uint16_t GMCLIB_SvmExDpwm_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIn, const GMCLIB_2COOR_SINCOS_T_F16 *psAngle, GMCLIB_3COOR_T_F16 *psOut);

RAM_FUNC_LIB 
extern uint16_t GMCLIB_SvmExDpwm_F16_FCRam(const GMCLIB_2COOR_ALBE_T_F16 *psIn, const GMCLIB_2COOR_SINCOS_T_F16 *psAngle, GMCLIB_3COOR_T_F16 *psOut);

#if defined(__cplusplus)  
}
#endif

#endif  /* _GMCLIB_SVM_F16_H_ */
