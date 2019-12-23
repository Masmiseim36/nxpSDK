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
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
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
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "ldrh %2, [%1, #4] \n\t"            /* Loads psParam -> f16State */
                        "sxth %2, %2 \n\t"                  /* Transforms 16-bit value to 32-bit */
                        "sxth %0, %0 \n\t"                  /* Transforms 16-bit input value to 32-bit */
                        "cmp %0, %2 \n\t"                   /* Compares f16Target with psParam -> f16State */
                        "blt GFLIB_Ramp_F16_Down \n\t"                /* If f16Target < psParam -> f16State, then ramps down */
                        "ldrh %3, [%1] \n\t"                /* Loads psParam -> f16RampUp */
                        "adds %2, %2, %3 \n\t"              /* f32Val1 = f16State + f16RampUp */
                        "cmp %0, %2 \n\t"                   /* Compares f32Val1 with f16Target */
                        "blt GFLIB_Ramp_F16_End \n\t"                  /* If f16Target < f32Val1, then executes next command */
                        "mov %0, %2 \n\t"                   /* f16Target = f32Val1 */
                        "b GFLIB_Ramp_F16_End \n\t"         /* Goes to the RampEnd label */
                    "GFLIB_Ramp_F16_Down: \n\t"         
                        "ldrh %3, [%1, #2] \n\t"            /* Loads psParam -> f16RampDown */
                        "subs %2, %2, %3 \n\t"              /* f32Val1 = f16State - f16RampDown */
                        "cmp %0, %2 \n\t"                   /* Compares f32Val1 with f16Target */
                        "bgt GFLIB_Ramp_F16_End \n\t"                      /* If f16Target > f32Val1, then executes next command */
                        "mov %0, %2 \n\t"                   /* f16Target = f32Val1 */
                    "GFLIB_Ramp_F16_End: \n\t"                   
                        "strh %0, [%1, #4] \n\t"            /* Loads psParam -> f16State */
                        : "+l"(f16Target), "+l"(psParam), "+l"(f32Val1), "+l"(f32Val2):);
    #endif
    return(f16Target);
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_RAMP_F16_ASMI_H */

