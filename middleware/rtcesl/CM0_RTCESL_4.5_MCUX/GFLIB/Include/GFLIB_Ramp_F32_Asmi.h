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
* @brief  Ramp 
* 
*******************************************************************************/
#ifndef _GFLIB_RAMP_F32_ASMI_H_
#define _GFLIB_RAMP_F32_ASMI_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib.h"
  
/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Ramp_F32_Asmi(f32Target, psParam) GFLIB_Ramp_F32_FAsmi(f32Target, psParam)

/***************************************************************************//*!
* @brief  Ramp function
*
* @param  ptr           GFLIB_RAMP_T_F32 *psParam
*                         - rampUp: Ramp-up increment
*                         - rampDown: Ramp-down increment
*                         - state: Actual state
* @param  in            frac32_t f32Target
*                         - Desired value in <-1;1) in frac32_t
*
* @return This function returns
*                       frac32_t value <-1;1)
*
* @remarks  This function ramps the value from the f32State value up/down to
*           the f32Target value using the up/down increments defined in
*           the psParam structure.
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t GFLIB_Ramp_F32_FAsmi(frac32_t f32Target, GFLIB_RAMP_T_F32 *psParam)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ ldr     f32Val1, [psParam, #8]      /* Loads psParam -> f32State */
                        cmp     f32Target, f32Val1          /* Compares f32Target with psParam -> f32State */
                        blt     RampDown_F32                /* If f32Target < psParam -> f32State, then ramps down */
                        ldr     f32Val2, [psParam]          /* Loads psParam -> f32RampUp */
                        adds    f32Val3, f32Val1, f32Val2   /* f32Val3 = f32State + f32RampUp */
                        eors    f32Val2, f32Val2, f32Val1   /* f32Val2 = f32State ^ f32RampUp */
                        bmi     RampUpLim_F32               /* If f32Val2 < 0, then goes to RampUpLim_F32 */
                        eors    f32Val1, f32Val1, f32Val3   /* f32Val1 = f32State ^ (f32State + f32RampUp) */
                        bmi     RampEnd_F32                 /* If f32Val1 < 0, then goes to RampEnd_F32 */
                    RampUpLim_F32:
                        cmp     f32Target, f32Val3          /* Compares Result with f32Target */
                        blt     RampEnd_F32                 /* If f32Target < Result, then executes next command */
                        mov     f32Target, f32Val3          /* f32Target = Result */
                        b       RampEnd_F32                 /* Goes to the RampEnd_F32 label */
                    RampDown_F32:
                        ldr     f32Val2, [psParam, #4]      /* Loads psParam -> f32RampDown */
                        subs    f32Val3, f32Val1, f32Val2   /* f32Val3 = f32State - f32RampDown */
                        eors    f32Val2, f32Val2, f32Val1   /* f32Val2 = f32State ^ f32RampDown */
                        bpl     RampDownLim_F32             /* If f32Val2 >= 0, then goes to RampDownLim_F32 */
                        eors    f32Val1, f32Val1, f32Val3   /* f32Val1 = f32State ^ (f32State - f32RampDown) */
                        bmi     RampEnd_F32                 /* If f32Val1 >= 0, then goes to RampEnd_F32 */
                    RampDownLim_F32:
                        cmp     f32Target, f32Val3          /* Compares Result with f32Target */
                        bgt     RampEnd_F32                 /* If f32Target > Result, then executes next command */
                        mov     f32Target, f32Val3          /* f32Target = Result */
                    RampEnd_F32:
                        str     f32Target, [psParam, #8] }; /* Loads psParam -> f32State */

    #else
        __asm volatile(
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "ldr     %2, [%1, #8] \n"           /* Loads psParam -> f32State */
                        "cmp     %0, %2 \n"                 /* Compares f32Target with psParam -> f32State */
                        "blt     RampDown_F32 \n"           /* If f32Target < psParam -> f32State, then ramps down */
                        "ldr     %3, [%1] \n"               /* Loads psParam -> f32RampUp */
                        "adds    %4, %2, %3 \n"             /* f32Val3 = f32State + f32RampUp */
                        "eors    %3, %3, %2 \n"             /* f32Val2 = f32State ^ f32RampUp */
                        "bmi     RampUpLim_F32 \n"          /* If f32Val2 < 0, then goes to RampUpLim_F32 */
                        "eors    %2, %2, %4 \n"             /* f32Val1 = f32State ^ (f32State + f32RampUp) */
                        "bmi     RampEnd_F32 \n"            /* If f32Val1 < 0, then goes to RampEnd_F32 */
                    "RampUpLim_F32: \n"
                        "cmp     %0, %4 \n"                 /* Compares Result with f32Target */
                        "blt     RampEnd_F32 \n"            /* If f32Target < Result, then executes next command */
                        "mov     %0, %4 \n"                 /* f32Target = Result */
                        "b       RampEnd_F32 \n"            /* Goes to the RampEnd label */
                    "RampDown_F32: \n"
                        "ldr     %3, [%1, #4] \n"           /* Loads psParam -> f32RampDown */
                        "subs    %4, %2, %3 \n"             /* f32Val3 = f32State - f32RampDown */
                        "eors    %3, %3, %2 \n"             /* f32Val2 = f32State ^ f32RampDown */
                        "bpl     RampDownLim_F32 \n"        /* If f32Val2 >= 0, then goes to RampDownLim_F32 */
                        "eors    %2, %2, %4 \n"             /* f32Val1 = f32State ^ (f32State - f32RampDown) */
                        "bmi     RampEnd_F32 \n"            /* If f32Val1 >= 0, then goes to RampEnd_F32 */
                    "RampDownLim_F32: \n"
                        "cmp     %0, %4 \n"                 /* Compares Result with f32Target */
                        "bgt     RampEnd_F32 \n"            /* If f32Target > Result, then executes next command */
                        "mov     %0, %4 \n"                 /* f32Target = Result */
                    "RampEnd_F32: \n"
                        "str     %0, [%1, #8] \n"           /* Loads psParam -> f32State */
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Target), "+l"(psParam), "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3):);
    #endif
    return(f32Target);
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_RAMP_F32_ASMI_H */

