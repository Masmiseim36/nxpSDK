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
* @brief  Hysteresis function
* 
*******************************************************************************/
#ifndef _GFLIB_HYST_F16_ASMI_H_
#define _GFLIB_HYST_F16_ASMI_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Hyst_F16_Asmi(f16Val, psParam) GFLIB_Hyst_F16_FAsmi(f16Val, psParam)

/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    frac16_t f16HystOn;    /* Value determining the upper threshold */
    frac16_t f16HystOff;   /* Value determining the lower threshold */
    frac16_t f16OutValOn;  /* Value of the output when input is higher than the upper threshold */
    frac16_t f16OutValOff; /* Value of output when input is lower than lower threshold */
    frac16_t f16OutState;  /* Actual state of the output */
}   GFLIB_HYST_T_F16;

/***************************************************************************//*!
*
* @brief    Hysteresis function
*
* @param    ptr         GFLIB_HYST_T_F16 *psParam
*                         - f16HystOn: Upper threshold
*                         - f16HystOff: Lower threshold
*                         - f16OutValOn: Output when input is above f16HystOn
*                         - f16OutValOff: Output when input is below f16HystOff
*                         - f16OutState: Output of the function
*
* @param    in          frac16_t f16Val
*                         - Argument in <-1;1) in frac16_t
*
* @return   N/A
*
* @remarks  The function represents a hysteresis (or relay) function. The
*           function switches output between the two predefined values. When
*           the input is higher than upper threshold f16HystOn, the output
*           is high; when the input is below another (lower) threshold
*           f16HystOff, the output is low; when the input is between the two,
*           the output retains its value.
*
****************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t GFLIB_Hyst_F16_FAsmi(frac16_t f16Val, GFLIB_HYST_T_F16 *psParam)
{
    register frac16_t f16Temp=0, f16Out=0;
    #if defined(__CC_ARM)                            /* For ARM Compiler */
        __asm volatile{ ldrsh f16Temp, [psParam, #2] /* f16Temp = psParam->f16HystOff */
                        sxth f16Val, f16Val          /* Sign extend */
                        cmp f16Temp, f16Val          /* Compares f16HystOff with f16Val*/
                        blt Hyst_F16_HystOn          /* If f16HystOff < f16Val, then goes to the Hyst_F16_HystOn */
                        ldrh f16Out, [psParam, #6]   /* f16Out = psParam->f16OutValOff */
                        b Hyst_F16_End               /* Goes to the Hyst_F16_End */
                    Hyst_F16_HystOn:
                        ldrsh f16Temp, [psParam]     /* f16Temp = psParam->f16HystOn */
                        cmp f16Temp, f16Val          /* Compares f16HystOn with f16Val*/
                        bgt Hyst_F16_State           /* If f16HystOn > f16Val, then goes to the Hyst_F16_State */
                        ldrh f16Out, [psParam, #4]   /* f16Out = psParam->f16OutValOn */
                        b Hyst_F16_End               /* Goes to the Hyst_F16_End */
                    Hyst_F16_State:
                        ldrh f16Out, [psParam, #8]   /* f16Out = psParam->f16State */
                        ldrh f16Temp, [psParam, #4]  /* f16Temp = psParam->f16OutValOn */
                        cmp f16Out, f16Temp          /* Compares f16State with f16OutValOn */
                        it ne                        /* If f16State != f16OutValOn, then executes next command */
                        ldrhne f16Out, [psParam, #6] /* f16Out = psParam->f16OutValOff */
                    Hyst_F16_End:
                        strh f16Out, [psParam, #8] };/* psParam->f16State = f16Out */
    #else
        __asm volatile( "ldrsh %2, [%3, #2] \n"      /* f16Temp = psParam->f16HystOff */
                        "sxth %1, %1 \n"             /* Sign extend */
                        "cmp %2, %1 \n"              /* Compares f16HystOff with f16Val*/
                        "blt.n Hyst_F16_HystOn \n"   /* If f16HystOff < f16Val, then goes to the Hyst_F16_HystOn */
                        "ldrh %0, [%3, #6] \n"       /* f16Out = psParam->f16OutValOff */
                        "b.n Hyst_F16_End \n"        /* Goes to the Hyst_F16_End */
                    "Hyst_F16_HystOn: \n"
                        "ldrsh %2, [%3] \n"          /* f16Temp = psParam->f16HystOn */
                        "cmp %2, %1 \n"              /* Compares f16HystOn with f16Val*/
                        "bgt.n Hyst_F16_State \n"    /* If f16HystOn > f16Val, then goes to the Hyst_F16_State */
                        "ldrh %0, [%3, #4] \n"       /* f16Out = psParam->f16OutValOn */
                        "b.n Hyst_F16_End \n"        /* Goes to the Hyst_F16_End */
                    "Hyst_F16_State: \n"
                        "ldrh %0, [%3, #8] \n"       /* f16Out = psParam->f16State */
                        "ldrh %2, [%3, #4] \n"       /* f16Temp = psParam->f16OutValOn */
                        "cmp %0, %2 \n"              /* Compares f16State with f16OutValOn */
                        "it ne \n"                   /* If f16State != f16OutValOn, then executes next command */
                        "ldrhne %0, [%3, #6] \n"     /* f16Out = psParam->f16OutValOff */
                    "Hyst_F16_End: \n"
                        "strh %0, [%3, #8] \n"       /* psParam->f16State = f16Out */
                        : "+l"(f16Out), "+l"(f16Val), "+l"(f16Temp): "l"(psParam));
    #endif

    return f16Out;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#ifdef __cplusplus
}
#endif

#endif    /* _GFLIB_HYST_F16_ASMI_H_ */
