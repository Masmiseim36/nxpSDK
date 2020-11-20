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
* @brief Main GMCLIB header file for devices with FPU. 
* 
*******************************************************************************/
#ifndef _GMCLIB_FP_H_
#define _GMCLIB_FP_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gmclib.h"
#include "GMCLIB_Clark_FLT.h" 
#include "GMCLIB_Decoupling_FLT.h"       
#include "GMCLIB_ElimDcBusRip_FLT.h"  
#include "GMCLIB_Park_FLT.h"    

/*******************************************************************************
* Macros 
*******************************************************************************/   
#if (defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW)) /* placed to RAM */

#define GMCLIB_DecouplingPMSM_FLT(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)        \
        GMCLIB_DecouplingPMSM_FLT_C(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)    
#define GMCLIB_ElimDcBusRip_F16fff(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp)        \
        GMCLIB_ElimDcBusRip_F16fff_C(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp) 	
#define GMCLIB_ElimDcBusRipFOC_F16ff(fltUDCBus, psUAlBe, psUAlBeComp)                 \
        GMCLIB_ElimDcBusRipFOC_F16ff_C(fltUDCBus, psUAlBe, psUAlBeComp) 

#else
	                                       
#define GMCLIB_DecouplingPMSM_FLT(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)        \
        GMCLIB_DecouplingPMSM_FLT_C(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)    
#define GMCLIB_ElimDcBusRip_F16fff(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp)        \
        GMCLIB_ElimDcBusRip_F16fff_C(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp) 	
#define GMCLIB_ElimDcBusRipFOC_F16ff(fltUDCBus, psUAlBe, psUAlBeComp)                 \
        GMCLIB_ElimDcBusRipFOC_F16ff_C(fltUDCBus, psUAlBe, psUAlBeComp)			      

#endif
		
#define GMCLIB_Clark_FLT(psIn, psOut)                                                 \
        GMCLIB_Clark_FLT_Ci(psIn, psOut)                                              
#define GMCLIB_ClarkInv_FLT(psIn, psOut)                                              \
        GMCLIB_ClarkInv_FLT_Ci(psIn, psOut)
#define GMCLIB_Park_FLT(psIn, psAnglePos, psOut)                                      \
        GMCLIB_Park_FLT_Ci(psIn, psAnglePos, psOut)                                   
#define GMCLIB_ParkInv_FLT(psIn, psAnglePos, psOut)                                   \
        GMCLIB_ParkInv_FLT_Ci(psIn, psAnglePos, psOut)
		
#if defined(__cplusplus) 
}
#endif 

#endif /* _GMCLIB_FP_H_ */
