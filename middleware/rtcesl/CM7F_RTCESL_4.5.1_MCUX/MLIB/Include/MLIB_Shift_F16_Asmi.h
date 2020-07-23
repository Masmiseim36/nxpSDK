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
* @brief  Shift
* 
*******************************************************************************/
#ifndef _MLIB_SHIFT_F16_ASM_H_
#define _MLIB_SHIFT_F16_ASM_H_

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
#define MLIB_Sh1LSat_F16_Asmi(f16Val)       MLIB_Sh1LSat_F16_FAsmi(f16Val)
#define MLIB_ShLSat_F16_Asmi(f16Val, u16Sh) MLIB_ShLSat_F16_FAsmi(f16Val, u16Sh)

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by 1 to the left. 
*              The function saturates the output.
*
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_Sh1LSat_F16_FAsmi(register frac16_t f16Val)
{
    #if defined(__CC_ARM)                                /* For ARM Compiler */
        __asm volatile{ qadd16 f16Val, f16Val, f16Val }; /* Addition with saturation */
    #else
        __asm volatile("qadd16 %0, %0, %0 \n"            /* Addition with saturation */
                        : "+l"(f16Val):);
    #endif

    return f16Val;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of u16Sh to the left. 
*          The function saturates the output.
*
*******************************************************************************/ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_ShLSat_F16_FAsmi(register frac16_t f16Val, register uint16_t u16Sh)
{
    #if defined(__CC_ARM)                              /* For ARM Compiler */
        __asm volatile{ sxth f16Val, f16Val            /* Transforms 16-bit input f16Val to 32-bit */
                        usat u16Sh, #4, u16Sh          /* Saturates to 4-bit value */
                        lsl f16Val, f16Val, u16Sh      /* f16Val << u16Sh */
                        ssat f16Val, #16, f16Val };    /* Saturates to 16-bit value */
    #else
        __asm volatile( "sxth %0, %0 \n"               /* Transforms 16-bit input f16Val to 32-bit */
                        "usat %1, #4, %1 \n"           /* Saturates to 4-bit value */
                        "lsl %0, %0, %1 \n"            /* f16Val << u16Sh */
                        "ssat %0, #16, %0 \n"          /* Saturates to 16-bit value */
                        : "+l"(f16Val), "+l"(u16Sh):);
    #endif

    return f16Val;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SHIFT_F16_ASM_H_ */
