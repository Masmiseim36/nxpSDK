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
