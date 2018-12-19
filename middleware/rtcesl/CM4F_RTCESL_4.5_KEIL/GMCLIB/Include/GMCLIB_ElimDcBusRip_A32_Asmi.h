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
* @brief  Elimination of the DC-Bus voltage ripple with index
* 
*******************************************************************************/
#ifndef _GMCLIB_ELIMDCBUSRIP_A32_ASMI_H_
#define _GMCLIB_ELIMDCBUSRIP_A32_ASMI_H_

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
#define GMCLIB_ElimDcBusRip_F16sas_Asmi(f16UDCBus, a32IdxMod, psUAlBe, psUAlBeComp)   \
        GMCLIB_ElimDcBusRip_F16sas_FAsmi(f16UDCBus, a32IdxMod, psUAlBe, psUAlBeComp)

/***************************************************************************//*!
* @brief    The function is used for elimination of the DC-Bus voltage ripple.
*
* @param    ptr   - psUAlBe - Direct(alpha) and quadrature(beta)
*                   component of the stator voltage vector in the stationary
*                   reference frame. Format Q15, range 8000-7FFF
*                 - psUAlBeComp - Direct(alpha) and quadrature(beta)
*                   component of the stator voltage vector in the stationary
*                   reference frame. Format Q15, range 8000-7FFF
* @param    in    - f16UDCBus - Actual effective value of the
*                   DC-Bus voltage. Format Q15, range 0-7FFF
*                 - a32IdxMod - Modulation Index, accumulator 32 bit format.
*
* @return   Function reads, recalculate and fills variables alpha,
*           beta in the data structure GMCLIB_2COOR_ALBE_T_F16.
*
* @remarks: The GMCLIB_ElimDcBusRipFOC function compensates an amplitude
*           of the direct-a and the quadrature-ß component of the
*           stator-reference voltage vector in the fractional arithmetic by the
*           formula shown in the following equations:
*
*           for ModIndex > 0
*           UalfaReq = 1.0                        if       Ua > 0 && Abs(Ua) >= Udcbus / ModIndex
*           UalfaReq = -1.0                       if       Ua < 0 && Abs(Ua) <= Udcbus / ModIndex
*           UalfaReq = Ua * ModIndex / Udcbus     else
*
*           UbetaReq = 1.0                        if       Ub > 0 && Abs(Ub) >= Udcbus / ModIndex
*           UbetaReq = -1.0                       if       Ub < 0 && Abs(Ub) <= Udcbus / ModIndex
*           UbetaReq = Ub * ModIndex / Udcbus     else
*
*           for ModIndex = 0
*           UaReq = 1.0                           Ua > 0 && Udcbus = 0
*           UaReq = -1.0                          Ua < 0 && Udcbus = 0
*           UaReq =  0.0                          Ua = 0 && Udcbus = 0
*
*           UbReq = 1.0                           Ub > 0 && Udcbus = 0
*           UbReq = -1.0                          Ub < 0 && Udcbus = 0
*           UbReq =  0.0                          Ub = 0 && Udcbus = 0
*
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline void GMCLIB_ElimDcBusRip_F16sas_FAsmi(frac16_t f16UDCBus,
                                                    acc32_t a32IdxMod,
                                                    const GMCLIB_2COOR_ALBE_T_F16 *psUAlBe,
                                                    GMCLIB_2COOR_ALBE_T_F16 *psUAlBeComp)
{
    register frac32_t f32Val = 0x40000000;                                 /* f32Val = FRAC32(0.5) */

    #if defined(__CC_ARM)                                                  /* For ARM Compiler */
        __asm volatile{ sxth    f16UDCBus, f16UDCBus                       /* Sign extend */
                        cmp     f16UDCBus, #0                              /* Compares Udcbus with 0 */
                        it      eq                                         /* If Udcbus = 0, then ModIndex / Udcbus = 0x7FFFFFFF */
                        qaddeq  f16UDCBus, f32Val, f32Val                  /* Saturation */
                        beq.n   ElimDcBusRip_UdcZerro                      /* If Udcbus = 0, then goes to ElimDcBusRip_UdcZerro */
                        /* ModIndex / Udcbus */
                        sdiv    f16UDCBus, f32Val, f16UDCBus               /* f16UDCBus = 1 / Udcbus */
                        smull   f16UDCBus, a32IdxMod, f16UDCBus, a32IdxMod /* a32IdxMod:f16UDCBus = ModIndex / Udcbus */
                        lsr     f16UDCBus, f16UDCBus, #14                  /* f16UDCBus >> 14 */
                        add     f16UDCBus, f16UDCBus, a32IdxMod, lsl #18   /* f16UDCBus = f16UDCBus + a32IdxMod << 18 */
                        asrs    a32IdxMod,a32IdxMod, #13                   /* a32IdxMod >> 13 */
                        it      ne                                         /* If a32IdxMod > 0, then saturates result */
                        qaddne  f16UDCBus, f32Val, f32Val                  /* Saturation */
                        ElimDcBusRip_UdcZerro:
                        /* UaReq = Ua * ModIndex / Udcbus */
                        ldrh    a32IdxMod, [psUAlBe]                       /* Loads Ua */
                        smulwb  f32Val, f16UDCBus, a32IdxMod               /* f32Val = Ua * ModIndex / Udcbus */
                        ssat    f32Val, #16, f32Val                        /* Saturation */
                        strh    f32Val, [psUAlBeComp]                      /* Stores UaReq */
                        /* UbReq = Ub * ModIndex / Udcbus */
                        ldrh    a32IdxMod, [psUAlBe, #2]                   /* Loads Ub */
                        smulwb  f32Val, f16UDCBus, a32IdxMod               /* f32Val = Ub * ModIndex / Udcbus */
                        ssat    f32Val, #16, f32Val                        /* Saturation */
                        strh    f32Val, [psUAlBeComp, #2] };               /* Stores UbReq */
    #else
        __asm volatile( "sxth    %0, %0 \n"                                /* Sign extend */
                        "cmp     %0, #0 \n"                                /* Compares Udcbus with 0 */
                        "it      eq \n"                                    /* If Udcbus = 0, then ModIndex / Udcbus = 0x7FFFFFFF */
                        "qaddeq  %0, %2, %2 \n"                            /* Saturation */
                        "beq.n   ElimDcBusRip_UdcZerro \n"                 /* If Udcbus = 0, then goes to ElimDcBusRip_UdcZerro */
                        /* ModIndex / Udcbus */
                        "sdiv    %0, %2, %0 \n"                            /* f16UDCBus = 1 / Udcbus */
                        "smull   %0, %1, %0, %1 \n"                        /* a32IdxMod:f16UDCBus = ModIndex / Udcbus */
                        "lsr     %0, %0, #14 \n"                           /* f16UDCBus >> 14 */
                        "add     %0, %0, %1, lsl #18 \n"                   /* f16UDCBus = f16UDCBus + a32IdxMod << 18 */
                        "asrs    %1,%1, #13 \n"                            /* a32IdxMod >> 13 */
                        "it      ne \n"                                    /* If a32IdxMod > 0, then saturates result */
                        "qaddne  %0, %2, %2 \n"                            /* Saturation */
                        "ElimDcBusRip_UdcZerro: \n"
                        /* UaReq = Ua * ModIndex / Udcbus */
                        "ldrh    %1, [%4] \n"                              /* Loads Ua */
                        "smulwb  %2, %0, %1 \n"                            /* f32Val = Ua * ModIndex / Udcbus */
                        "ssat    %2, #16, %2 \n"                           /* Saturation */
                        "strh    %2, [%3] \n"                              /* Stores UaReq */
                        /* UbReq = Ub * ModIndex / Udcbus */
                        "ldrh    %1, [%4, #2] \n"                          /* Loads Ub */
                        "smulwb  %2, %0, %1 \n"                            /* f32Val = Ub * ModIndex / Udcbus */
                        "ssat    %2, #16, %2 \n"                           /* Saturation */
                        "strh    %2, [%3, #2] \n"                          /* Stores UbReq */
                        : "+l"(f16UDCBus), "+l"(a32IdxMod), "+l"(f32Val): "l"(psUAlBeComp), "l"(psUAlBe));
    #endif
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus) 
}
#endif

#endif  /* _GMCLIB_ELIMDCBUSRIP_A32_ASMI_H_ */
