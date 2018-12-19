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
#define GMCLIB_ElimDcBusRipFOC_F16ff_C(fltUDCBus, psUAlBe, psUAlBeComp)          \
        GMCLIB_ElimDcBusRipFOC_F16ff_FC(fltUDCBus, psUAlBe, psUAlBeComp)   
#define GMCLIB_ElimDcBusRip_F16fff_C(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp) \
        GMCLIB_ElimDcBusRip_F16fff_FC(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp)   
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern void GMCLIB_ElimDcBusRipFOC_F16ff_FC(float_t fltUDCBus,
                                            const GMCLIB_2COOR_ALBE_T_FLT *psUAlBe,
                                            GMCLIB_2COOR_ALBE_T_F16 *psUAlBeComp);
extern void GMCLIB_ElimDcBusRip_F16fff_FC(float_t fltUDCBus,
                                          float_t fltIdxMod,
                                          const GMCLIB_2COOR_ALBE_T_FLT *psUAlBe,
                                          GMCLIB_2COOR_ALBE_T_F16 *psUAlBeComp);

#if defined(__cplusplus) 
}
#endif

#endif  /* _GMCLIB_ELIMDCBUSRIP_FLT_H_ */
