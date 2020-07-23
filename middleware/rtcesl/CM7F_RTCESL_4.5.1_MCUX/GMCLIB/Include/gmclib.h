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
* @brief Main GMCLIB header file for devices without FPU.
* 
*******************************************************************************/
#ifndef _GMCLIB_H_
#define _GMCLIB_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "GMCLIB_Clark_F16_Asmi.h"
#include "GMCLIB_Decoupling_A32.h"  
#include "GMCLIB_ElimDcBusRip_A32_Asmi.h"
#include "GMCLIB_ElimDcBusRip_F16_Asmi.h"
#include "GMCLIB_Park_F16_Asmi.h"
#include "GMCLIB_Svm_F16.h" 

/*******************************************************************************
* Macros 
*******************************************************************************/ 
#if (defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW)) /* placed to RAM */

#define GMCLIB_DecouplingPMSM_F16(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)      \
        GMCLIB_DecouplingPMSM_F16_AsmRam(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)
#define GMCLIB_SvmIct_F16(psIn, psOut)                                              \
        GMCLIB_SvmIct_F16_CRam(psIn, psOut)                                            
#define GMCLIB_SvmStd_F16(psIn, psOut)                                              \
        GMCLIB_SvmStd_F16_CRam(psIn, psOut)
#define GMCLIB_SvmU0n_F16(psIn, psOut)                                              \
        GMCLIB_SvmU0n_F16_CRam(psIn, psOut)              
#define GMCLIB_SvmU7n_F16(psIn, psOut)                                              \
        GMCLIB_SvmU7n_F16_CRam(psIn, psOut)           
#define GMCLIB_SvmDpwm_F16(psIn, psOut)                                             \
        GMCLIB_SvmDpwm_F16_CRam(psIn, psOut)                                            
#define GMCLIB_SvmExDpwm_F16(psIn, psAngle, psOut)                                  \
        GMCLIB_SvmExDpwm_F16_CRam(psIn, psAngle,psOut)

#else    

#define GMCLIB_DecouplingPMSM_F16(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)      \
        GMCLIB_DecouplingPMSM_F16_Asm(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)
#define GMCLIB_SvmIct_F16(psIn, psOut)                                              \
        GMCLIB_SvmIct_F16_C(psIn, psOut)                                            
#define GMCLIB_SvmStd_F16(psIn, psOut)                                              \
        GMCLIB_SvmStd_F16_C(psIn, psOut)
#define GMCLIB_SvmU0n_F16(psIn, psOut)                                              \
        GMCLIB_SvmU0n_F16_C(psIn, psOut)              
#define GMCLIB_SvmU7n_F16(psIn, psOut)                                              \
        GMCLIB_SvmU7n_F16_C(psIn, psOut)           
#define GMCLIB_SvmDpwm_F16(psIn, psOut)                                             \
        GMCLIB_SvmDpwm_F16_C(psIn, psOut)                                            
#define GMCLIB_SvmExDpwm_F16(psIn, psAngle, psOut)                                  \
        GMCLIB_SvmExDpwm_F16_C(psIn, psAngle,psOut)            

#endif

#define GMCLIB_Clark_F16(psIn, psOut)                                               \
        GMCLIB_Clark_F16_Asmi(psIn, psOut)
#define GMCLIB_ClarkInv_F16(psIn, psOut)                                            \
        GMCLIB_ClarkInv_F16_Asmi(psIn, psOut)
#define GMCLIB_ElimDcBusRip_F16sas(f16UDCBus, a32IdxMod, psUAlBe, psUAlBeComp)      \
        GMCLIB_ElimDcBusRip_F16sas_Asmi(f16UDCBus, a32IdxMod, psUAlBe, psUAlBeComp)
#define GMCLIB_ElimDcBusRipFOC_F16(f16UDCBus, psUAlBe, psUAlBeComp)                 \
        GMCLIB_ElimDcBusRipFOC_F16_Asmi(f16UDCBus, psUAlBe, psUAlBeComp)
#define GMCLIB_Park_F16(psIn, psAnglePos, psOut)                                    \
        GMCLIB_Park_F16_Asmi(psIn, psAnglePos, psOut)
#define GMCLIB_ParkInv_F16(psIn, psAnglePos, psOut)                                 \
        GMCLIB_ParkInv_F16_Asmi(psIn, psAnglePos, psOut)


#if defined(__cplusplus) 
}
#endif 

#endif /* _GMCLIB_H_ */
