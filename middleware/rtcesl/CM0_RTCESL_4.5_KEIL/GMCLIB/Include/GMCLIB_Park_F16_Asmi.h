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
#define GMCLIB_Park_F16_Asmi(psIn, psAnglePos, psOut)    GMCLIB_Park_F16_FAsmi(psIn, psAnglePos, psOut)
#define GMCLIB_ParkInv_F16_Asmi(psIn, psAnglePos, psOut) GMCLIB_ParkInv_F16_FAsmi(psIn, psAnglePos, psOut)

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
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline void GMCLIB_Park_F16_FAsmi(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                         const GMCLIB_2COOR_SINCOS_T_F16 *psAnglePos,
                                         GMCLIB_2COOR_DQ_T_F16 *psOut)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;

    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ /* Loads input values */
                        ldrh f32Val1, [psIn, #2]             /* Loads beta */
                        ldrh f32Val2, [psIn]                 /* Loads alpha */
                        ldrh f32Val3, [psAnglePos, #2]       /* Loads cos(theta) */
                        ldrh psIn, [psAnglePos]              /* Loads sin(theta) */
                        sxth f32Val1, f32Val1                /* Sign extend */
                        sxth f32Val2, f32Val2                /* Sign extend */
                        sxth f32Val3, f32Val3                /* Sign extend */
                        sxth psIn, psIn                      /* Sign extend */
                        /* Counts d and q coordinates */
                        movs psAnglePos, f32Val2             /* alpha */
                        muls psAnglePos, f32Val3, psAnglePos /* alpha * cos(theta) */
                        muls f32Val2, f32Val2, psIn          /* alpha * sin(theta) */
                        muls psIn, psIn, f32Val1             /* beta * sin(theta) */
                        muls f32Val1, f32Val1, f32Val3       /* beta * cos(theta) */
                        subs f32Val1, f32Val1, f32Val2       /* q = beta * cos(theta) - alpha * sin(theta) */
                        adds psIn, psIn, psAnglePos          /* d = beta * sin(theta) + alpha * cos(theta) */
                        asrs f32Val1, f32Val1, #15           /* q << 15 */
                        asrs psIn, psIn, #15                 /* d << 15 */
                        /* Saturation */
                        movs f32Val3, #0x80                  /* f32Val3 = 0x80 */
                        lsls f32Val3, f32Val3, #8            /* f32Val3 = 0x8000 */
                        sxth f32Val2, f32Val1                /* Sign extend */
                        eors f32Val1, f32Val2, f32Val1       /* f32Val2 ^ f32Val1 */
                        bpl Park_F16_QNotSat                 /* If f32Val2 < 0, then saturates result */
                        asrs f32Val2, f32Val2, #16           /* f32Val2 >> 16 */
                        adds f32Val2, f32Val3, f32Val2       /* q = 0x8000 + f32Val2 */
                    Park_F16_QNotSat:
                        sxth f32Val1, psIn                   /* Sign extend */
                        eors psIn, f32Val1, psIn             /* f32Val2 ^ psIn */
                        bpl Park_F16_DNotSat                 /* If f32Val2 < 0, then saturates result */
                        asrs f32Val1, f32Val1, #16           /* f32Val1 >> 16*/
                        adds f32Val1, f32Val3, f32Val1       /* d = 0x8000 + f32Val1*/
                    Park_F16_DNotSat:
                        /* Stores d and q values */
                        strh f32Val1, [psOut]                /* Stores psOut->f16D */
                        strh f32Val2, [psOut, #2] };         /* Stores psOut->f16Q */
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        /* Loads input values */
                        "ldrh %0, [%3, #2] \n\t"               /* Loads beta */
                        "ldrh %1, [%3] \n\t"                   /* Loads alpha */
                        "ldrh %2, [%4, #2] \n\t"               /* Loads cos(theta) */
                        "ldrh %3, [%4] \n\t"                   /* Loads sin(theta) */
                        "sxth %0, %0 \n\t"                     /* Sign extend */
                        "sxth %1, %1 \n\t"                     /* Sign extend */
                        "sxth %2, %2 \n\t"                     /* Sign extend */
                        "sxth %3, %3 \n\t"                     /* Sign extend */
                        /* Counts d and q coordinates */
                        "movs %4, %1 \n\t"                     /* alpha */
                        "muls %4, %2, %4 \n\t"                 /* alpha * cos(theta) */
                        "muls %1, %1, %3 \n\t"                 /* alpha * sin(theta) */
                        "muls %3, %3, %0 \n\t"                 /* beta * sin(theta) */
                        "muls %0, %0, %2 \n\t"                 /* beta * cos(theta) */
                        "subs %0, %0, %1 \n\t"                 /* q = beta * cos(theta) - alpha * sin(theta) */
                        "adds %3, %3, %4 \n\t"                 /* d = beta * sin(theta) + alpha * cos(theta) */
                        "asrs %0, %0, #15 \n\t"                /* q << 15 */
                        "asrs %3, %3, #15 \n\t"                /* d << 15 */
                         /* Saturation */
                        "movs %2, #0x80 \n\t"                  /* f32Val3 = 0x80 */
                        "lsls %2, %2, #8 \n\t"                 /* f32Val3 = 0x8000 */
                        "sxth %1, %0 \n\t"                     /* Sign extend */
                        "eors %0, %1, %0 \n\t"                 /* f32Val2 ^ f32Val1 */
                        "bpl GMCLIB_Park_F16_QNotSat \n\t"     /* If f32Val2 < 0, then saturates result */
                        "asrs %1, %1, #16 \n\t"                /* f32Val2 >> 16 */
                        "adds %1, %2, %1 \n\t"                 /* q = 0x8000 + f32Val2 */
                    "GMCLIB_Park_F16_QNotSat: \n\t"	
                        "sxth %0, %3 \n\t"                     /* Sign extend */
                        "eors %3, %0, %3 \n\t"                 /* f32Val2 ^ psIn */
                        "bpl GMCLIB_Park_F16_DNotSat \n\t"     /* If f32Val2 < 0, then saturates result */
                        "asrs %0, %0, #16 \n\t"                /* f32Val1 >> 16*/
                        "adds %0, %2, %0 \n\t"                 /* d = 0x8000 + f32Val1*/
                    "GMCLIB_Park_F16_DNotSat: \n\t"	
                        /* Stores d and q values */
                        "strh %0, [%5] \n\t"                   /* Stores psOut->f16D */
                        "strh %1, [%5, #2] \n\t"               /* Stores psOut->f16Q */
                        : "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3), "+l"(psIn), "+l"(psAnglePos): "l"(psOut));
    #endif
}
/* inline function without any optimization (compilation issue) */ 
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
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline void GMCLIB_ParkInv_F16_FAsmi(const GMCLIB_2COOR_DQ_T_F16 *psIn,
                                            const GMCLIB_2COOR_SINCOS_T_F16 *psAnglePos,
                                            GMCLIB_2COOR_ALBE_T_F16 *psOut)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;

    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ /* Loads input values */
                        ldrh f32Val1, [psIn, #2]             /* Loads q */
                        ldrh f32Val2, [psIn]                 /* Loads d */
                        ldrh f32Val3, [psAnglePos, #2]       /* Loads cos(theta) */
                        ldrh psIn, [psAnglePos]              /* Loads sin(theta) */
                        sxth f32Val1, f32Val1                /* Sign extend */
                        sxth f32Val2, f32Val2                /* Sign extend */
                        sxth f32Val3, f32Val3                /* Sign extend */
                        sxth psIn, psIn                      /* Sign extend */
                        /* Counts alpha and beta coordinates */
                        movs psAnglePos, f32Val2             /* d */
                        muls psAnglePos, f32Val3, psAnglePos /* d * cos(theta) */
                        muls f32Val2, f32Val2, psIn          /* d * sin(theta) */
                        muls psIn, psIn, f32Val1             /* q * sin(theta) */
                        muls f32Val1, f32Val1, f32Val3       /* q * cos(theta) */
                        subs psIn, psAnglePos, psIn          /* alpha = d * cos(theta) - q * sin(theta) */
                        adds f32Val1, f32Val1, f32Val2       /* beta  = d * sin(theta) + q * cos(theta) */
                        asrs psIn, psIn, #15                 /* alpha << 15 */
                        asrs f32Val1, f32Val1, #15           /* beta << 15 */
                        /* Saturation */
                        movs f32Val3, #0x80                  /* f32Val3 = 0x80 */
                        lsls f32Val3, f32Val3, #8            /* f32Val3 = 0x8000 */
                        sxth f32Val2, f32Val1                /* Sign extend */
                        eors f32Val1, f32Val2, f32Val1       /* f32Val2 ^ f32Val1 */
                        bpl ParkInv_F16_QNotSat              /* If f32Val2 < 0, then saturates result */
                        asrs f32Val2, f32Val2, #16           /* f32Val2 >> 16 */
                        adds f32Val2, f32Val3, f32Val2       /* beta = 0x8000 + f32Val2 */
                    ParkInv_F16_QNotSat:
                        sxth f32Val1, psIn                   /* Sign extend */
                        eors psIn, f32Val1, psIn             /* f32Val2 ^ psIn */
                        bpl ParkInv_F16_DNotSat              /* If f32Val2 < 0, then saturates result */
                        asrs f32Val1, f32Val1, #16           /* f32Val1 >> 16*/
                        adds f32Val1, f32Val3, f32Val1       /* alpha = 0x8000 + f32Val1*/
                    ParkInv_F16_DNotSat:
                        /* Stores alpha and beta values */
                        strh f32Val1, [psOut]                /* Stores psOut->f16Alpha */
                        strh f32Val2, [psOut, #2] };         /* Stores psOut->f16Beta */
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        /* Loads input values */
                        "ldrh %0, [%3, #2] \n\t"                    /* Loads q */
                        "ldrh %1, [%3] \n\t"                        /* Loads d */
                        "ldrh %2, [%4, #2] \n\t"                    /* Loads cos(theta) */
                        "ldrh %3, [%4] \n\t"                        /* Loads sin(theta) */
                        "sxth %0, %0 \n\t"                          /* Sign extend */
                        "sxth %1, %1 \n\t"                          /* Sign extend */
                        "sxth %2, %2 \n\t"                          /* Sign extend */
                        "sxth %3, %3 \n\t"                          /* Sign extend */
                        /* Counts alpha and beta coordinates */
                        "movs %4, %1 \n\t"                          /* d */
                        "muls %4, %2, %4 \n\t"                      /* d * cos(theta) */
                        "muls %1, %1, %3 \n\t"                      /* d * sin(theta) */
                        "muls %3, %3, %0 \n\t"                      /* q * sin(theta) */
                        "muls %0, %0, %2 \n\t"                      /* q * cos(theta) */
                        "subs %3, %4, %3 \n\t"                      /* alpha = d * cos(theta) - q * sin(theta) */
                        "adds %0, %0, %1 \n\t"                      /* beta  = d * sin(theta) + q * cos(theta) */
                        "asrs %3, %3, #15 \n\t"                     /* alpha << 15 */
                        "asrs %0, %0, #15 \n\t"                     /* beta << 15 */
                        /* Saturation */     
                        "movs %2, #0x80 \n\t"                       /* f32Val3 = 0x80 */
                        "lsls %2, %2, #8 \n\t"                      /* f32Val3 = 0x8000 */
                        "sxth %1, %0 \n\t"                          /* Sign extend */
                        "eors %0, %1, %0 \n\t"                      /* f32Val2 ^ f32Val1 */
                        "bpl GMCLIB_Park_F16_Inv_F16_QNotSat \n\t"  /* If f32Val2 < 0, then saturates result */
                        "asrs %1, %1, #16 \n\t"                     /* f32Val2 >> 16 */
                        "adds %1, %2, %1 \n\t"                      /* beta = 0x8000 + f32Val2 */
                    "GMCLIB_Park_F16_Inv_F16_QNotSat: \n\t"												
                        "sxth %0, %3 \n\t"                          /* Sign extend */
                        "eors %3, %0, %3 \n\t"                      /* f32Val2 ^ psIn */
                        "bpl GMCLIB_Park_F16_Inv_F16_DNotSat \n\t"  /* If f32Val2 < 0, then saturates result */
                        "asrs %0, %0, #16 \n\t"                     /* f32Val1 >> 16*/
                        "adds %0, %2, %0 \n\t"                      /* alpha = 0x8000 + f32Val1*/
                    "GMCLIB_Park_F16_Inv_F16_DNotSat: \n\t"												
                        /* Stores alpha and beta values */
                        "strh %0, [%5] \n\t"                        /* Stores psOut->f16Alpha */
                        "strh %1, [%5, #2] \n\t"                    /* Stores psOut->f16Beta */
                        : "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3), "+l"(psIn), "+l"(psAnglePos): "l"(psOut));
    #endif
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_PARK_F16_ASMI_H_ */
