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
#include "GMCLIB_DTCompLut1D_F16.h"
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
#define GMCLIB_DTCompLut1D_F16(psIABC, psUAlBe, f16U_DCB, psParam, psUAlBeDtComp)   \
        GMCLIB_DTCompLut1D_F16_C(psIABC, psUAlBe, f16U_DCB, psParam, psUAlBeDtComp)  
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
#define GMCLIB_SvmStdShifted_F16(psIn, param, psCfgPWM, psCfgMeas)                  \
        GMCLIB_SvmStdShifted_F16_C(psIn, param, psCfgPWM, psCfgMeas)
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
