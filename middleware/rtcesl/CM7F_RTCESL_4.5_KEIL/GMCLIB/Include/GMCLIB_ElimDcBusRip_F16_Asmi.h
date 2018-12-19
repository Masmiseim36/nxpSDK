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
#ifndef _GMCLIB_ELIMDCBUSRIP_F16_ASMI_H_
#define _GMCLIB_ELIMDCBUSRIP_F16_ASMI_H_

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
#define GMCLIB_ElimDcBusRipFOC_F16_Asmi(f16UDCBus, psUAlBe, psUAlBeComp) \
        GMCLIB_ElimDcBusRipFOC_F16_FAsmi(f16UDCBus, psUAlBe, psUAlBeComp)
  
/***************************************************************************//*!
*
* @brief    The function is used for elimination of the DC-Bus voltage
*           ripple in general cases where the phase voltage is scale as
*           measured dc bus voltage divided by the mod. index       .
*
* @param    ptr   - psUAlBe - Direct(alpha) and quadrature(beta)
*                   component of the stator voltage vector in the stationary
*                   reference frame. Format Q15, range 8000-7FFF
*                 - psUAlBeComp - Direct(alpha) and quadrature(beta)
*                   component of the stator voltage vector in the stationary
*                   reference frame. Format Q15, range 8000-7FFF
*
* @param    in    - f16UDCBus - Actual effective value of the
*                   DC-Bus voltage. Format Q15, range 0-7FFF
*
* @return   Function reads, recalculate and fills variables alpha,
*           beta in the data structure GMCLIB_2COOR_ALBE_T_F16.
*
* @remarks: The GMCLIB_ElimDcBusRipFOC function compensates an amplitude
*           of the direct-a and the quadrature-ß component of the
*           stator-reference voltage vector in the fractional arithmetic by the
*           formula shown in the following equations:
*
*           UalfaReq = 0.0             if       Ualfa = 0 && UDCBus = 0
*           UalfaReq = 1.0             if       Ualfa > 0 && Abs(Ualfa) >= UDCBus
*           UalfaReq = -1.0            if       Ualfa < 0 && Abs(Ualfa) <= UDCBus
*           UalfaReq = Ualfa/UDCBus    else
*
*           UbetaReq = 0.0             if       Ubeta = 0 && UDCBus = 0
*           UbetaReq = 1.0             if       Ubeta > 0 && Abs(Ubeta) >= UDCBus
*           UbetaReq = -1.0            if       Ubeta < 0 && Abs(Ubeta) <= UDCBus
*           UbetaReq = Ubeta/UDCBus    else
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline void GMCLIB_ElimDcBusRipFOC_F16_FAsmi(frac16_t f16UDCBus,
                                                    const GMCLIB_2COOR_ALBE_T_F16 *psUAlBe,
                                                    GMCLIB_2COOR_ALBE_T_F16 *psUAlBeComp)
{
    register frac32_t f32Val=0;                              /* f32Val = FRAC32(0.5) */

    #if defined(__CC_ARM)                                  /* For ARM Compiler */
        __asm volatile{ sxth    f16UDCBus, f16UDCBus       /* Sign extend */
                        cmp     f16UDCBus, #0              /* Compares Udcbus with 0 */
                        it      eq                         /* If Udcbus = 0, then Udcbus = 1 */
                        moveq   f16UDCBus, #1              /* Udcbus = 1 */
                        /* UaReq = Ua / Udcbus */
                        ldrsh   f32Val, [psUAlBe]          /* Loads Ua */
                        lsls    f32Val, f32Val, #15        /* f32Val << 15 */
                        sdiv    f32Val, f32Val, f16UDCBus  /* f32Val = Ua / Udcbus */
                        ssat    f32Val, #16, f32Val        /* Saturation */
                        strh    f32Val, [psUAlBeComp]      /* Stores UaReq */
                        /* UbReq = Ub / Udcbus */
                        ldrsh   f32Val, [psUAlBe, #2]      /* Loads Ub */
                        lsls    f32Val, f32Val, #15        /* f32Val << 15 */
                        sdiv    f32Val, f32Val, f16UDCBus  /* f32Val = Ub / Udcbus */
                        ssat    f32Val, #16, f32Val        /* Saturation */
                        strh    f32Val, [psUAlBeComp, #2]};/* Stores UbReq */
    #else
        __asm volatile( "sxth    %0, %0 \n"                /* Sign extend */
                        "cmp     %0, #0 \n"                /* Compares Udcbus with 0 */
                        "it      eq \n"                    /* If Udcbus = 0, then Udcbus = 1 */
                        "moveq   %0, #1 \n"                /* Udcbus = 1 */
                        /* UaReq = Ua / Udcbus */
                        "ldrsh   %1, [%3] \n"              /* Loads Ua */
                        "lsls    %1, %1, #15 \n"           /* f32Val << 15 */
                        "sdiv    %1, %1, %0 \n"            /* f32Val = Ua / Udcbus */
                        "ssat    %1, #16, %1 \n"           /* Saturation */
                        "strh    %1, [%2] \n"              /* Stores UaReq */
                        /* UbReq = Ub / Udcbus */
                        "ldrsh   %1, [%3, #2] \n"          /* Loads Ub */
                        "lsls    %1, %1, #15 \n"           /* f32Val << 15 */
                        "sdiv    %1, %1, %0 \n"            /* f32Val = Ub / Udcbus */
                        "ssat    %1, #16, %1 \n"           /* Saturation */
                        "strh    %1, [%2, #2] \n"          /* Stores UbReq */
                        : "+l"(f16UDCBus), "+l"(f32Val): "l"(psUAlBeComp), "l"(psUAlBe));
    #endif
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus) 
}
#endif

#endif  /* _GMCLIB_ELIMDCBUSRIP_F16_ASMI_H_ */
