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
* @brief  Shift
* 
*******************************************************************************/
#ifndef _MLIB_SHIFT_F32_ASM_H_
#define _MLIB_SHIFT_F32_ASM_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_Sh1LSat_F32_Asmi(f32Val)       MLIB_Sh1LSat_F32_FAsmi(f32Val)
#define MLIB_ShLSat_F32_Asmi(f32Val, u16Sh) MLIB_ShLSat_F32_FAsmi(f32Val, u16Sh)

/***************************************************************************//*!
*
* This function returns the f32Val input shifted by 1 to the left. 
*              The function saturates the output.
*
*******************************************************************************/ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Sh1LSat_F32_FAsmi(register frac32_t f32Val)
{
    #if defined(__CC_ARM)                              /* For ARM Compiler */
        __asm volatile{ qadd f32Val, f32Val, f32Val }; /* Addition with saturation */
    #else
        __asm volatile("qadd %0, %0, %0 \n"            /* Addition with saturation */
                        : "+l"(f32Val):);
    #endif

    return f32Val;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of u16Sh to the left. 
*          The function saturates the output.
*
*******************************************************************************/  
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_ShLSat_F32_FAsmi(register frac32_t f32Val, register uint16_t u16Sh)
{
    register frac32_t f32Out=0;
    register frac32_t f32SatVal = f32Val;
    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ ssat f32SatVal, #2, f32SatVal        /* Saturates f32SatVal to 2-bit value */
                        lsl f32SatVal, f32SatVal, #30        /* f32SatVal << 30 */
                        qadd f32SatVal, f32SatVal, f32SatVal /* Addition with saturation */
                        lsl f32Out, f32Val, u16Sh            /* f32Out = f32Val << u16Sh */
                        asr u16Sh, f32Out, u16Sh             /* u16Sh = f32Out << u16Sh */
                        cmp u16Sh, f32Val                    /* Compares u16Sh and f32Val */
                        it ne                                /* If u16Sh != f32Val, then returns the saturate value */
                        movne f32Out, f32SatVal };
    #else
        __asm volatile( "ssat %1, #2, %1 \n"                 /* Saturates f32SatVal to 2-bit value */
                        "lsl %1, %1, #30 \n"                 /* f32SatVal << 30 */
                        "qadd %1, %1, %1 \n"                 /* Addition with saturation */
                        "lsl %0, %2, %3 \n"                  /* f32Out = f32Val << u16Sh */
                        "asr %3, %0, %3 \n"                  /* u16Sh = f32Out << u16Sh */
                        "cmp %3, %2 \n"                      /* Compares u16Sh and f32Val */
                        "it ne \n"                           /* If u16Sh != f32Val, then returns the saturate value */
                        "movne %0, %1 \n"
                        : "=&l"(f32Out), "+l"(f32SatVal), "+l"(f32Val), "+l"(u16Sh):);
    #endif

    return f32Out;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SHIFT_F32_ASM_H_ */
