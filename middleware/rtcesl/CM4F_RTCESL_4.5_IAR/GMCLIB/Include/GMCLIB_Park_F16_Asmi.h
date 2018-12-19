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
* @brief  Park, Inverse Park Transformation 
* 
*******************************************************************************/
#ifndef _GMCLIB_PARK_F16_ASMI_H_
#define _GMCLIB_PARK_F16_ASMI_H_

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
#define GMCLIB_Park_F16_Asmi(psIn, psAnglePos, psOut)                         \
        GMCLIB_Park_F16_FAsmi(psIn, psAnglePos, psOut)
#define GMCLIB_ParkInv_F16_Asmi(psIn, psAnglePos, psOut)                      \
        GMCLIB_ParkInv_F16_FAsmi(psIn, psAnglePos, psOut)

/***************************************************************************//*!
*
* @brief       The function calculates Park Transformation which is used for 
*              transforming values (current, voltage, flux) from 
*              alpha-beta stationary orthogonal coordination system 
*              to d-q rotating orthogonal coordination system
*
* @param  ptr  GMCLIB_2COOR_ALBE_T_F16 *psIn
*                  IN - pointer to structure containing data of two phase
*                  stationary orthogonal system
*              GMCLIB_2COORD_SINCOS_T_F16 *psAnglePos
*                  IN - pointer to structure where the values 
*                  of sine and cosine are stored
*              GMCLIB_2_COOR_DQ_T_F16 *psOut
*                  OUT - pointer to structure containing data of 
*                  DQ coordinate two-phase stationary orthogonal system                 
*
* @remarks     Modifies the structure pointed by pDQ pointer
*              according to the following equations
*              d = alpha * cos(theta) + beta * sin(theta)
*              q = beta * cos(theta) - alpha * sin(theta)
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline void GMCLIB_Park_F16_FAsmi(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                         const GMCLIB_2COOR_SINCOS_T_F16 *psAnglePos,
                                         GMCLIB_2COOR_DQ_T_F16 *psOut)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;

    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ ldrsh f32Val1, [psIn]                        /* Loads alpha */
                        ldrsh f32Val2, [psIn, #2]                    /* Loads beta */
                        ldrsh f32Val3, [psAnglePos]                  /* Loads sin(theta) */
                        ldrsh psIn, [psAnglePos, #2]                 /* Loads cos(theta) */

                        mul psAnglePos, f32Val1, psIn                /* alpha * cos(theta) */
                        mla psAnglePos, f32Val2, f32Val3, psAnglePos /* alpha * cos(theta) + beta * sin(theta) */
                        ssat psAnglePos, #16, psAnglePos, asr #15    /* Saturation */
                        mul f32Val2, f32Val2, psIn                   /* beta * cos(theta) */
                        mls f32Val2, f32Val1, f32Val3, f32Val2       /* beta * cos(theta) - alpha * sin(theta) */
                        ssat f32Val2, #16, f32Val2, asr #15          /* Saturation */

                        strh psAnglePos, [psOut]                     /* Stores psOut->f16D */
                        strh f32Val2, [psOut, #2] };                 /* Stores psOut->f16Q */
    #else
        __asm volatile( "ldrsh %0, [%3] \n"                          /* Loads alpha */
                        "ldrsh %1, [%3, #2] \n"                      /* Loads beta */
                        "ldrsh %2, [%4] \n"                          /* Loads sin(theta) */
                        "ldrsh %3, [%4, #2] \n"                      /* Loads cos(theta) */

                        "mul %4, %0, %3 \n"                          /* alpha * cos(theta) */
                        "mla %4, %1, %2, %4 \n"                      /* alpha * cos(theta) + beta * sin(theta) */
                        "ssat %4, #16, %4, asr #15 \n"               /* Saturation */
                        "mul %1, %1, %3 \n"                          /* beta * cos(theta) */
                        "mls %1, %0, %2, %1 \n"                      /* beta * cos(theta) - alpha * sin(theta) */
                        "ssat %1, #16, %1, asr #15 \n"               /* Saturation */

                        "strh %4, [%5] \n"                           /* Stores psOut->f16D */
                        "strh %1, [%5, #2] \n"                       /* Stores psOut->f16Q */
                        : "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3), "+l"(psIn), "+l"(psAnglePos): "l"(psOut));
    #endif

    return;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* @brief        The function calculates Inverse Park Transformation which is used 
*               for transforming values (current, voltage, flux) from 
*               d-q rotating orthogonal coordination system to alpha-beta 
*               stationary orthogonal coordination system.
*
* @param  ptr   GMCLIB_2COOR_DQ_T_F16 *psIn
*                   IN - pointer to structure containing data of 
*                   DQ coordinate two-phase stationary orthogonal system              
*               GMCLIB_ANGLE_T *psAnglePos
*                   IN - pointer to structure where the values 
*                   of sine and cosine are stored
*               GMCLIB_2COOR_ALBE_T_F16 *psOut
*                   OUT - pointer to structure containing data of two phase
*                   stationary orthogonal system
*                       
* @remarks      Modifies the structure pointed by pAlphaBeta pointer 
*               according following equations:
*               alpha = d * cos(theta) - q * sin(theta)
*               beta  = d * sin(theta) + q * cos(theta)
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline void GMCLIB_ParkInv_F16_FAsmi(const GMCLIB_2COOR_DQ_T_F16 *psIn,
                                            const GMCLIB_2COOR_SINCOS_T_F16 *psAnglePos,
                                            GMCLIB_2COOR_ALBE_T_F16 *psOut)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;

    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ ldrsh f32Val1, [psIn]                        /* Loads d */
                        ldrsh f32Val2, [psIn, #2]                    /* Loads q */
                        ldrsh f32Val3, [psAnglePos]                  /* Loads sin(theta) */
                        ldrsh psIn, [psAnglePos, #2]                 /* Loads cos(theta) */

                        mul psAnglePos, f32Val1, psIn                /* d * cos(theta) */
                        mls psAnglePos, f32Val2, f32Val3, psAnglePos /* d * cos(theta) - q * sin(theta) */
                        ssat psAnglePos, #16, psAnglePos, asr #15    /* Saturation */
                        mul f32Val2, f32Val2, psIn                   /* q * cos(theta) */
                        mla f32Val2, f32Val1, f32Val3, f32Val2       /* q * cos(theta) + d * sin(theta) */
                        ssat f32Val2, #16, f32Val2, asr #15          /* Saturation */

                        strh psAnglePos, [psOut]                     /* Stores psOut->f16Alpha */
                        strh f32Val2, [psOut, #2] };                 /* Stores psOut->f16Beta */
    #else
        __asm volatile( "ldrsh %0, [%3] \n"                          /* Loads d */
                        "ldrsh %1, [%3, #2] \n"                      /* Loads q */
                        "ldrsh %2, [%4] \n"                          /* Loads sin(theta) */
                        "ldrsh %3, [%4, #2] \n"                      /* Loads cos(theta) */

                        "mul %4, %0, %3 \n"                          /* d * cos(theta) */
                        "mls %4, %1, %2, %4 \n"                      /* d * cos(theta) - q * sin(theta) */
                        "ssat %4, #16, %4, asr #15 \n"               /* Saturation */
                        "mul %1, %1, %3 \n"                          /* q * cos(theta) */
                        "mla %1, %0, %2, %1 \n"                      /* q * cos(theta) + d * sin(theta) */
                        "ssat %1, #16, %1, asr #15 \n"               /* Saturation */

                        "strh %4, [%5] \n"                           /* Stores psOut->f16Alpha */
                        "strh %1, [%5, #2] \n"                       /* Stores psOut->f16Beta */
                        : "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3), "+l"(psIn), "+l"(psAnglePos): "l"(psOut));
    #endif

    return;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_PARK_F16_ASMI_H_ */
