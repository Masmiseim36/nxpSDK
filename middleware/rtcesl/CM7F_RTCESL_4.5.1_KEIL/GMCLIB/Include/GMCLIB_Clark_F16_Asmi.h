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
#define GMCLIB_Clark_F16_Asmi(psIn, psOut)    GMCLIB_Clark_F16_FAsmi(psIn, psOut)
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
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline void GMCLIB_Clark_F16_FAsmi(const GMCLIB_3COOR_T_F16 *psIn,
                                          GMCLIB_2COOR_ALBE_T_F16 *psOut)
{
    register frac32_t f32Val1=0, f32Val2=0;

    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ ldrsh f32Val1, [psIn, #2]            /* f32Val1 = psIn->f16B */
                        ldrsh f32Val2, [psIn, #4]            /* f32Val2 = psIn->f16C */
                        sub f32Val1, f32Val1, f32Val2        /* f32Val1 = f16B - f16C */
                        movw f32Val2, #0x49E7                /* f32Val2 = FRAC16(1/sqrt(3)) */
                        mul f32Val1, f32Val1, f32Val2        /* f32Val1 = (f16B - f16C)/sqrt(3) */
                        ssat f32Val1, #16, f32Val1, asr #15  /* Saturation */
                        ldrsh f32Val2, [psIn]                /* f32Val2 = psIn->f16A */
                        strh f32Val2, [psOut]                /* psOut->f16Alpha = f32Val2 */
                        strh f32Val1, [psOut, #2] };         /* psOut->f16Beta = f32Val1 */
    #else
        __asm volatile( "ldrsh %0, [%2, #2] \n"              /* f32Val1 = psIn->f16B */
                        "ldrsh %1, [%2, #4] \n"              /* f32Val2 = psIn->f16C */
                        "sub %0, %0, %1 \n"                  /* f32Val1 = f16B - f16C */
                        "movw %1, #0x49E7 \n"                /* f32Val2 = FRAC16(1/sqrt(3)) */
                        "mul %0, %0, %1 \n"                  /* f32Val1 = (f16B - f16C)/sqrt(3) */
                        "ssat %0, #16, %0, asr #15 \n"       /* Saturation */
                        "ldrsh %1, [%2] \n"                  /* f32Val2 = psIn->f16A */
                        "strh %1, [%3] \n"                   /* psOut->f16Alpha = f32Val2 */
                        "strh %0, [%3, #2] \n"               /* psOut->f16Beta = f32Val1 */
                        : "+l"(f32Val1), "+l"(f32Val2): "l"(psIn), "l"(psOut));
    #endif

    return;
}
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
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline void GMCLIB_ClarkInv_F16_FAsmi(const GMCLIB_2COOR_ALBE_T_F16 *psIn,
                                             GMCLIB_3COOR_T_F16 *psOut)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;

    #if defined(__CC_ARM)                                      /* For ARM Compiler */
        __asm volatile{ ldrsh f32Val3, [psIn]                  /* f32Val3 = psIn->f16Alpha */
                        ldrsh f32Val1, [psIn, #2]              /* f32Val1 = psIn->f16Beta */
                        movw f32Val2, #0x6EDA                  /* f32Val2 = FRAC16(sqrt(3)/2) */
                        mul f32Val1, f32Val1, f32Val2          /* f32Val1 = f16Beta * sqrt(3)/2 */
                        str f32Val3, [psOut]                   /* psOut->f16A = f32Val3 */
                        asr f32Val1, f32Val1, #15              /* f32Val1 >> 15 */
                        add f32Val2, f32Val1, f32Val3, asr #1  /* f32Val2 = f16Beta * sqrt(3)/2 + f16Alpha/2 */
                        sub f32Val1, f32Val1, f32Val3, asr #1  /* f32Val1 = f16Beta * sqrt(3)/2 - f16Alpha/2 */
                        rsb f32Val2, f32Val2, #0               /* f32Val2 = - f16Beta * sqrt(3)/2 - f16Alpha/2 */
                        ssat f32Val1, #16, f32Val1             /* Saturation */
                        ssat f32Val2, #16, f32Val2             /* Saturation */
                        strh f32Val1, [psOut, #2]              /* psOut->f16B = f32Val1 */
                        strh f32Val2, [psOut, #4] };           /* psOut->f16C = f32Val2 */
    #else
        __asm volatile( "ldrsh %2, [%3] \n"                    /* f32Val3 = psIn->f16Alpha */
                        "ldrsh %0, [%3, #2] \n"                /* f32Val1 = psIn->f16Beta */
                        "movw %1, #0x6EDA \n"                  /* f32Val2 = FRAC16(sqrt(3)/2) */
                        "mul %0, %0, %1 \n"                    /* f32Val1 = f16Beta * sqrt(3)/2 */
                        "str %2, [%4] \n"                      /* psOut->f16A = f32Val3 */
                        "asr %0, %0, #15 \n"                   /* f32Val1 >> 15 */
                        "add %1, %0, %2, asr #1 \n"            /* f32Val2 = f16Beta * sqrt(3)/2 + f16Alpha/2 */
                        "sub %0, %0, %2, asr #1 \n"            /* f32Val1 = f16Beta * sqrt(3)/2 - f16Alpha/2 */
                        "rsb %1, %1, #0 \n"                    /* f32Val2 = - f16Beta * sqrt(3)/2 - f16Alpha/2 */
                        "ssat %0, #16, %0 \n"                  /* Saturation */
                        "ssat %1, #16, %1 \n"                  /* Saturation */
                        "strh %0, [%4, #2] \n"                 /* psOut->f16B = f32Val1 */
                        "strh %1, [%4, #4] \n"                 /* psOut->f16C = f32Val2 */
                        : "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3): "l"(psIn), "l"(psOut));
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

#endif /* _GMCLIB_CLARK_F16_ASMI_H_ */
