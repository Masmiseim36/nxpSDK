/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
* @brief  Clarke, Inverse Clarke Transformation
* 
*******************************************************************************/
#ifndef _GMCLIB_CLARK_F16_ASMI_H_
#define _GMCLIB_CLARK_F16_ASMI_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/******************************************************************************
* Includes
******************************************************************************/
#include "mlib.h"
#include "gmclib_types.h"      
  
/******************************************************************************
* Macros 
******************************************************************************/ 
#define GMCLIB_Clark_F16_Asmi(psIn, psOut) GMCLIB_Clark_F16_FAsmi(psIn, psOut)
#define GMCLIB_ClarkInv_F16_Asmi(psIn, psOut) GMCLIB_ClarkInv_F16_FAsmi(psIn, psOut)
  
/***************************************************************************//*!
*
* @brief   The function calculates Clarke Transformation which is used 
*          for transforming values (current, voltage, flux) from the 
*          three phase stationary coordination system  to alpha-beta 
*          stationary orthogonal coordination system.
*
* @param   ptr GMCLIB_3COOR_T_F16 *psIn
*                  IN - pointer to structure containing data of 
*                  three phase stationary system
*              GMCLIB_2COOR_ALBE_T_F16 *psOut
*                  OUT - pointer to structure containing data of 
*                  two phase stationary orthogonal system
*
* @remarks Modifies the structure pointed by psOut pointer 
*          according to the following equations:
*          alpha = a
*          beta  = b * 1 / sqrt(3)  - c * 1 / sqrt(3) 
* 
****************************************************************************/  
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline void GMCLIB_Clark_F16_FAsmi(const GMCLIB_3COOR_T_F16 *psIn,
                                          GMCLIB_2COOR_ALBE_T_F16 *psOut)
{
    register frac32_t f32Val1=0, f32Val2=0;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ ldrh f32Val1, [psIn]            /* f32Val1 = psIn->a */
                        strh f32Val1, [psOut]           /* psOut->alpha = psIn->a */
                        ldrh f32Val1, [psIn, #2]        /* f32Val1 = psIn->b */
                        sxth f32Val1, f32Val1           /* Sign extend */
                        ldrh f32Val2, [psIn, #4]        /* f32Val2 = psIn->c */
                        sxth f32Val2, f32Val2           /* Sign extend */
                        subs f32Val1, f32Val1, f32Val2  /* f32Val1 = b - c */
                        movs f32Val2, #0x93             /* f32Val2 = 0x93 */
                        lsls f32Val2, f32Val2, #8       /* f32Val2 = 0x9300 */
                        adds f32Val2, #0xCD             /* f32Val2 = 0x93CD = 2 * FRAC16(1/sqrt(3)) */
                        muls f32Val2, f32Val1, f32Val2  /* f32Val2 = (b - c)/sqrt(3) */
                        asrs f32Val2, f32Val2, #16      /* f32Val2 >> 16 */
                        eors f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 ^ f32Val2 */
                        bpl Clark_F16_NotSat            /* If f32Val1 < 0, then saturates result */
                        asrs f32Val2, f32Val2, #16      /* f32Val2 >> 16 */
                        movs f32Val1, #0x80             /* f32Val1 = 0x80 */
                        lsls f32Val1, f32Val1, #8       /* f32Val1 = 0x8000 */
                        adds f32Val2, f32Val2, f32Val1  /* f32Val2 = f32Val1 + f32Val2 */
                    Clark_F16_NotSat:
                        strh f32Val2, [psOut, #2] };    /* psOut->beta = f32Val2 */
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "ldrh %0, [%2] \n"              /* f32Val1 = psIn->a */
                        "strh %0, [%3] \n"              /* psOut->alpha = psIn->a */
                        "ldrh %0, [%2, #2] \n"          /* f32Val1 = psIn->b */
                        "sxth %0, %0 \n"                /* Sign extend */
                        "ldrh %1, [%2, #4] \n"          /* f32Val2 = psIn->c */
                        "sxth %1, %1 \n"                /* Sign extend */
                        "subs %0, %0, %1 \n"            /* f32Val1 = b - c */
                        "movs %1, #0x93 \n"             /* f32Val2 = 0x93 */
                        "lsls %1, %1, #8 \n"            /* f32Val2 = 0x9300 */
                        "adds %1, #0xCD \n"             /* f32Val2 = 0x93CD = 2 * FRAC16(1/sqrt(3)) */
                        "muls %1, %0, %1 \n"            /* f32Val2 = (b - c)/sqrt(3) */
                        "asrs %1, %1, #16 \n"           /* f32Val2 >> 16 */
                        "eors %0, %0, %1 \n"            /* f32Val1 = f32Val1 ^ f32Val2 */
                        "bpl Clark_F16_NotSat \n"       /* If f32Val1 < 0, then saturates result */
                        "asrs %1, %1, #16 \n"           /* f32Val2 >> 16 */
                        "movs %0, #0x80 \n"             /* f32Val1 = 0x80 */
                        "lsls %0, %0, #8 \n"            /* f32Val1 = 0x8000 */
                        "adds %1, %1, %0 \n"            /* f32Val2 = f32Val1 + f32Val2 */
                    "Clark_F16_NotSat: \n"
                        "strh %1, [%3, #2] \n"          /* psOut->beta = f32Val2 */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Val1), "+l"(f32Val2): "l"(psIn), "l"(psOut));
    #endif

    return;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/****************************************************************************
*
* @brief        The function calculates Inverse Clarke Transformation which is used
*               for transforming values (current, voltage, flux) from alpha-beta 
*               stationary orthogonal coordination system to three phase 
*               stationary coordination system
*
* @param  ptr   GMCLIB_2COOR_ALBE_T_F16 *psIn
*                   IN - pointer to structure containing data of two phase stationary orthogonal system
*               GMCLIB_3COOR_T_F16 *psOut
*                   OUT - pointer to structure containing data of three phase stationary system                      
*
* @remarks      Modifies the structure pointed by p_abc pointer 
*               according to the following equations:
*               a = alpha
*               b = -0.5 * alpha + sgrt(3) / 2 * beta
*               c = -0.5 * alpha - sgrt(3) / 2 * beta
*
****************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline void GMCLIB_ClarkInv_F16_FAsmi(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                             GMCLIB_3COOR_T_F16 *psOut)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ ldrh f32Val2, [psIn]            /* f32Val1 = psIn->alpha */
                        strh f32Val2, [psOut]           /* psOut->a = psIn->alpha */
                        ldrh f32Val1, [psIn, #2]        /* f32Val2 = psIn->beta */
                        sxth f32Val2, f32Val2           /* Sign extend */
                        sxth f32Val1, f32Val1           /* Sign extend */

                        rsbs f32Val2, f32Val2, #0       /* f32Val2 = - alpha */
                        asrs f32Val2, f32Val2, #1       /* f32Val2 = - alpha / 2 */

                        movs psIn, #0xDD                /* psIn = 0xDD */
                        lsls psIn, psIn, #8             /* psIn = 0xDD00 */
                        adds psIn, #0xB4                /* psIn = 0xDDB4 = 2 * FRAC16(sqrt(3)/2) */
                        muls f32Val1, f32Val1, psIn     /* f32Val1 = beta * sqrt(3)/2 */
                        asrs f32Val1, f32Val1, #16      /* f32Val1 >> 16 */

                        subs f32Val3, f32Val2, f32Val1  /* c = - alpha / 2 - beta * sqrt(3)/2*/
                        adds psIn, f32Val2, f32Val1     /* b = - alpha / 2 + beta * sqrt(3)/2*/

                        movs f32Val1, #0x80             /* f32Val1 = 0x80 */
                        lsls f32Val1, f32Val1, #8       /* f32Val1 = 0x8000 */

                        adds f32Val2, f32Val3, f32Val1  /* f32Val2 = c + 0x8000 */
                        asrs f32Val2, f32Val2, #16      /* f32Val2 >> 16 */
                        beq ClarkInv_F16_TestB          /* If f32Val2 != 0, then saturates c */
                        asrs f32Val3, f32Val3, #31      /* f32Val3 = f32Val3 >> 31 */
                        adds f32Val3, f32Val3, #1       /* f32Val3 = f32Val3 + 1 */
                        subs f32Val3, f32Val1, f32Val3  /* f32Val3 = 0x8000 - f32Val3 */
                        b ClarkInv_F16_SatEnd           /* If c is saturated, then b does not need saturation */
                    ClarkInv_F16_TestB:
                        adds f32Val2, psIn, f32Val1     /* f32Val2 = b + 0x8000 */
                        asrs f32Val2, f32Val2, #16      /* f32Val2 >> 16 */
                        beq ClarkInv_F16_SatEnd         /* If f32Val2 != 0, then saturates b */
                        asrs psIn, psIn, #31            /* psIn = psIn >> 31 */
                        adds psIn, psIn, #1             /* psIn = psIn + 1 */
                        subs psIn, f32Val1, psIn        /* psIn = 0x8000 - psIn */
                    ClarkInv_F16_SatEnd:
                        strh psIn, [psOut, #2]          /* Stores psOut->b */
                        strh f32Val3, [psOut, #4] };    /* Stores psOut->c */
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "ldrh %1, [%3] \n"              /* f32Val1 = psIn->alpha */
                        "strh %1, [%4] \n"              /* psOut->a = psIn->alpha */
                        "ldrh %0, [%3, #2] \n"          /* f32Val2 = psIn->beta */
                        "sxth %1, %1 \n"                /* Sign extend */
                        "sxth %0, %0 \n"                /* Sign extend */

                        "rsbs %1, %1, #0 \n"            /* f32Val2 = - alpha */
                        "asrs %1, %1, #1 \n"            /* f32Val2 = - alpha / 2 */

                        "movs %3, #0xDD \n"             /* psIn = 0xDD */
                        "lsls %3, %3, #8 \n"            /* psIn = 0xDD00 */
                        "adds %3, #0xB4 \n"             /* psIn = 0xDDB4 = 2 * FRAC16(sqrt(3)/2) */
                        "muls %0, %0, %3 \n"            /* f32Val1 = beta * sqrt(3)/2 */
                        "asrs %0, %0, #16 \n"           /* f32Val1 >> 16 */

                        "subs %2, %1, %0 \n"            /* c = - alpha / 2 - beta * sqrt(3)/2*/
                        "adds %3, %1, %0 \n"            /* b = - alpha / 2 + beta * sqrt(3)/2*/

                        "movs %0, #0x80 \n"             /* f32Val1 = 0x80 */
                        "lsls %0, %0, #8 \n"            /* f32Val1 = 0x8000 */

                        "adds %1, %2, %0 \n"            /* f32Val2 = c + 0x8000 */
                        "asrs %1, %1, #16 \n"           /* f32Val2 >> 16 */
                        "beq .+10 \n"                   /* If f32Val2 != 0, then saturates c */
                        "asrs %2, %2, #31 \n"           /* f32Val3 = f32Val3 >> 31 */
                        "adds %2, %2, #1 \n"            /* f32Val3 = f32Val3 + 1 */
                        "subs %2, %0, %2 \n"            /* f32Val3 = 0x8000 - f32Val3 */
                        "b ClarkInv_F16_SatEnd \n"      /* If c is saturated, then b does not need saturation */

                        "adds %1, %3, %0 \n"            /* f32Val2 = b + 0x8000 */
                        "asrs %1, %1, #16 \n"           /* f32Val2 >> 16 */
                        "beq ClarkInv_F16_SatEnd \n"    /* If f32Val2 != 0, then saturates b */
                        "asrs %3, %3, #31 \n"           /* psIn = psIn >> 31 */
                        "adds %3, %3, #1 \n"            /* psIn = psIn + 1 */
                        "subs %3, %0, %3 \n"            /* psIn = 0x8000 - psIn */

                    "ClarkInv_F16_SatEnd: \n"
                        "strh %3, [%4, #2] \n"          /* Stores psOut->b */
                        "strh %2, [%4, #4] \n"          /* Stores psOut->c */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3), "+l"(psIn): "l"(psOut));
    #endif

    return;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif


#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_CLARK_F16_ASMI_H_ */
