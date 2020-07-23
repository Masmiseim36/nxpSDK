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
* @brief  Elimination of the DC-Bus voltage ripple for FOC
* 
*******************************************************************************/
#ifndef _GMCLIB_ELIMDCBUSRIP_FLT_H_
#define _GMCLIB_ELIMDCBUSRIP_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "gmclib_types.h"
#include "mlib_FP.h"      

/*******************************************************************************
* Macros 
*******************************************************************************/   
#define GMCLIB_ElimDcBusRipFOC_F16ff_C(fltUDCBus, psUAlBe, psUAlBeComp)             \
        GMCLIB_ElimDcBusRipFOC_F16ff_FC(fltUDCBus, psUAlBe, psUAlBeComp)   
#define GMCLIB_ElimDcBusRip_F16fff_C(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp)    \
        GMCLIB_ElimDcBusRip_F16fff_FC(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp) 
#define GMCLIB_ElimDcBusRipFOC_F16ff_CRam(fltUDCBus, psUAlBe, psUAlBeComp)          \
        GMCLIB_ElimDcBusRipFOC_F16ff_FCRam(fltUDCBus, psUAlBe, psUAlBeComp)   
#define GMCLIB_ElimDcBusRip_F16fff_CRam(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp) \
        GMCLIB_ElimDcBusRip_F16fff_FCRam(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp) 		
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern void GMCLIB_ElimDcBusRipFOC_F16ff_FC(float_t fltUDCBus,
                                            const GMCLIB_2COOR_ALBE_T_FLT *psUAlBe,
                                            GMCLIB_2COOR_ALBE_T_F16 *psUAlBeComp);

RAM_FUNC_LIB 
extern void GMCLIB_ElimDcBusRipFOC_F16ff_FCRam(float_t fltUDCBus,
                                               const GMCLIB_2COOR_ALBE_T_FLT *psUAlBe,
                                               GMCLIB_2COOR_ALBE_T_F16 *psUAlBeComp);

extern void GMCLIB_ElimDcBusRip_F16fff_FC(float_t fltUDCBus,
                                          float_t fltIdxMod,
                                          const GMCLIB_2COOR_ALBE_T_FLT *psUAlBe,
                                          GMCLIB_2COOR_ALBE_T_F16 *psUAlBeComp);											
											
RAM_FUNC_LIB 
extern void GMCLIB_ElimDcBusRip_F16fff_FCRam(float_t fltUDCBus,
                                             float_t fltIdxMod,
                                             const GMCLIB_2COOR_ALBE_T_FLT *psUAlBe,
                                             GMCLIB_2COOR_ALBE_T_F16 *psUAlBeComp);

#if defined(__cplusplus) 
}
#endif

#endif  /* _GMCLIB_ELIMDCBUSRIP_FLT_H_ */
