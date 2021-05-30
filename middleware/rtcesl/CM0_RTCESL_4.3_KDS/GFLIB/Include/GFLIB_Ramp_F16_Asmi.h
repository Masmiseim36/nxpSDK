/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
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
 */

/*******************************************************************************
*
* @brief  Ramp 
* 
*******************************************************************************/
#ifndef _GFLIB_RAMP_F16_ASMI_H_
#define _GFLIB_RAMP_F16_ASMI_H_

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
#define GFLIB_Ramp_F16_Asmi(f16Target, psParam) GFLIB_Ramp_F16_FAsmi(f16Target, psParam)

/***************************************************************************//*!
* @brief  Ramp function
*
* @param  ptr  GFLIB_RAMP_T_F16 *psParam
*                - rampUp: Ramp-up increment
*                - rampDown: Ramp-down increment
*                - state: Actual state
* @param  in   frac16_t f16Target
*                - Desired value in <-1;1) in frac16_t
*
* @return This function returns
*                       frac16_t value <-1;1)
*
* @remarks  This function ramps the value from the f16State value up/down to
*           the f16Target value using the up/down increments defined in
*           the psParam structure.
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */
#if defined(__IAR_SYSTEMS_ICC__) /* set no optimization for IAR compiler   */
    #pragma language=save        /* Save existing optimization level */
    #pragma optimize=none        /* Optimization none level now */        
#elif defined(__CC_ARM)          /* set no optimization for ARM(KEIL) compiler */
    #pragma O0                   /* Optimization level now O0 */                    
#else                            /* set no optimization for GCC(KDS) compiler */
    __attribute__((optimize("O0")))  
#endif
static inline frac16_t GFLIB_Ramp_F16_FAsmi(frac16_t f16Target, GFLIB_RAMP_T_F16 *psParam)
{
    register frac32_t f32Val1=0, f32Val2=0;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ ldrh f32Val1, [psParam, #4]     /* Loads psParam -> f16State */
                        sxth f32Val1, f32Val1           /* Transforms 16-bit value to 32-bit */
                        sxth f16Target, f16Target       /* Transforms 16-bit input value to 32-bit */
                        cmp f16Target, f32Val1          /* Compares f16Target with psParam -> f16State */
                        blt RampDown                    /* If f16Target < psParam -> f16State, then ramps down */
                        ldrh f32Val2, [psParam]         /* Loads psParam -> f16RampUp */
                        adds f32Val1, f32Val1, f32Val2  /* f32Val1 = f16State + f16RampUp */
                        cmp f16Target, f32Val1          /* Compares f32Val1 with f16Target */
                        blt RampEnd                     /* If f16Target < f32Val1, then executes next command */
                        mov f16Target, f32Val1          /* f16Target = f32Val1 */
                        b RampEnd                       /* Goes to the RampEnd label */
                     RampDown:
                        ldrh f32Val2, [psParam, #2]     /* Loads psParam -> f16RampDown */
                        subs f32Val1, f32Val1, f32Val2  /* f32Val1 = f16State - f16RampDown */
                        cmp f16Target, f32Val1          /* Compares f32Val1 with f16Target */
                        bgt RampEnd                     /* If f16Target > f32Val1, then executes next command */
                        mov f16Target, f32Val1          /* f16Target = f32Val1 */
                     RampEnd:
                        strh f16Target, [psParam, #4] };/* Loads psParam -> f16State */
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "ldrh %2, [%1, #4] \n"          /* Loads psParam -> f16State */
                        "sxth %2, %2 \n"                /* Transforms 16-bit value to 32-bit */
                        "sxth %0, %0 \n"                /* Transforms 16-bit input value to 32-bit */
                        "cmp %0, %2 \n"                 /* Compares f16Target with psParam -> f16State */
                        "blt RampDown \n"               /* If f16Target < psParam -> f16State, then ramps down */
                        "ldrh %3, [%1] \n"              /* Loads psParam -> f16RampUp */
                        "adds %2, %2, %3 \n"            /* f32Val1 = f16State + f16RampUp */
                        "cmp %0, %2 \n"                 /* Compares f32Val1 with f16Target */
                        "blt RampEnd \n"                /* If f16Target < f32Val1, then executes next command */
                        "mov %0, %2 \n"                 /* f16Target = f32Val1 */
                        "b RampEnd \n"                  /* Goes to the RampEnd label */
                    "RampDown: \n"
                        "ldrh %3, [%1, #2] \n"          /* Loads psParam -> f16RampDown */
                        "subs %2, %2, %3 \n"            /* f32Val1 = f16State - f16RampDown */
                        "cmp %0, %2 \n"                 /* Compares f32Val1 with f16Target */
                        "bgt .+4 \n"                    /* If f16Target > f32Val1, then executes next command */
                        "mov %0, %2 \n"                 /* f16Target = f32Val1 */
                    "RampEnd: \n"
                        "strh %0, [%1, #4] \n"          /* Loads psParam -> f16State */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Target), "+l"(psParam), "+l"(f32Val1), "+l"(f32Val2):);
    #endif
    return(f16Target);
}
#if defined(__IAR_SYSTEMS_ICC__) /* set no optimization for IAR compiler   */
    #pragma language=restore     /* Restore original optimization level */         
#elif defined(__CC_ARM)          /* set no optimization for ARM(KEIL) compiler */  
#else                            /* set no optimization for GCC(KDS) compiler */                
#endif 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_RAMP_F16_ASMI_H */

