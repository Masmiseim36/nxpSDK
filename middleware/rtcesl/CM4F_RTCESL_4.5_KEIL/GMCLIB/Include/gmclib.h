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
#define GMCLIB_Clark_F16(psIn, psOut)                                               \
        GMCLIB_Clark_F16_Asmi(psIn, psOut)
#define GMCLIB_ClarkInv_F16(psIn, psOut)                                            \
        GMCLIB_ClarkInv_F16_Asmi(psIn, psOut)
#define GMCLIB_DecouplingPMSM_F16(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)      \
        GMCLIB_DecouplingPMSM_F16_Asm(psUDQ, psIDQ, f16SpeedEl, psParam, psUDQDec)
#define GMCLIB_ElimDcBusRip_F16sas(f16UDCBus, a32IdxMod, psUAlBe, psUAlBeComp)      \
        GMCLIB_ElimDcBusRip_F16sas_Asmi(f16UDCBus, a32IdxMod, psUAlBe, psUAlBeComp)
#define GMCLIB_ElimDcBusRipFOC_F16(f16UDCBus, psUAlBe, psUAlBeComp)                 \
        GMCLIB_ElimDcBusRipFOC_F16_Asmi(f16UDCBus, psUAlBe, psUAlBeComp)
#define GMCLIB_Park_F16(psIn, psAnglePos, psOut)                                    \
        GMCLIB_Park_F16_Asmi(psIn, psAnglePos, psOut)
#define GMCLIB_ParkInv_F16(psIn, psAnglePos, psOut)                                 \
        GMCLIB_ParkInv_F16_Asmi(psIn, psAnglePos, psOut)
#define GMCLIB_SvmIct_F16(psIn, psOut)                                              \
        GMCLIB_SvmIct_F16_C(psIn, psOut)                                            
#define GMCLIB_SvmStd_F16(psIn, psOut)                                              \
        GMCLIB_SvmStd_F16_Asm(psIn, psOut)
#define GMCLIB_SvmU0n_F16(psIn, psOut)                                              \
        GMCLIB_SvmU0n_F16_C(psIn, psOut)              
#define GMCLIB_SvmU7n_F16(psIn, psOut)                                              \
        GMCLIB_SvmU7n_F16_C(psIn, psOut)           
#define GMCLIB_SvmDpwm_F16(psIn, psOut)                                             \
        GMCLIB_SvmDpwm_F16_C(psIn, psOut)                                            
#define GMCLIB_SvmExDpwm_F16(psIn, psAngle, psOut)                                  \
        GMCLIB_SvmExDpwm_F16_C(psIn, psAngle,psOut)            
            
#if defined(__cplusplus) 
}
#endif 

#endif /* _GMCLIB_H_ */
