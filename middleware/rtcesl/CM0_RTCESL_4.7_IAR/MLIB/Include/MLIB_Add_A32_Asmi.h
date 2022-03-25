/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
* @brief  Addition
* 
*******************************************************************************/
#ifndef _MLIB_ADD_A32_ASM_H_
#define _MLIB_ADD_A32_ASM_H_

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
#define MLIB_Add_A32ss_Asmi(f16Add1, f16Add2) MLIB_Add_A32ss_FAsmi(f16Add1, f16Add2)
#define MLIB_Add_A32as_Asmi(a32Accum, f16Add) MLIB_Add_A32as_FAsmi(a32Accum, f16Add)

/***************************************************************************//*!
*  Addition        
*  Output = f16Add1 + f16Add2
*  Result can overflow through fractional range and is stored in accumulator type
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline acc32_t MLIB_Add_A32ss_FAsmi(register frac16_t f16Add1, register frac16_t f16Add2)
{ 
    register acc32_t a32Val=0;
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ sxth f16Add1, f16Add1           /* Transforms 16-bit input f16Add2 to 32-bit */
                        sxth f16Add2, f16Add2           /* Transforms 16-bit input f16Add2 to 32-bit */
                        adds a32Val, f16Add1, f16Add2}; /* f16Add1 = f16Add1 + f16Add2 */
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "sxth %1, %1 \n\t"              /* Transforms 16-bit input f16Add2 to 32-bit */
                        "sxth %2, %2 \n\t"              /* Transforms 16-bit input f16Add2 to 32-bit */
                        "adds %0, %1, %2 \n\t"          /* f16Add1 = f16Add1 + f16Add2 */
                        : "=l"(a32Val), "+l"(f16Add1), "+l"(f16Add2):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "sxth %1, %1 \n"                /* Transforms 16-bit input f16Add2 to 32-bit */
                        "sxth %2, %2 \n"                /* Transforms 16-bit input f16Add2 to 32-bit */
                        "adds %0, %1, %2 \n"            /* f16Add1 = f16Add1 + f16Add2 */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "=l"(a32Val), "+l"(f16Add1), "+l"(f16Add2):);
    #endif

    return a32Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*  Addition        
*  Output = a32Accum + f16Add
*  Result can overflow through fractional range and is stored in accumulator type
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline acc32_t MLIB_Add_A32as_FAsmi(register acc32_t a32Accum, register frac16_t f16Add)
{ 
    #if defined(__CC_ARM)                                 /* For ARM Compiler */
        __asm volatile{ sxth f16Add, f16Add               /* Transforms 16-bit input f16Add2 to 32-bit */
                        adds a32Accum, a32Accum, f16Add}; /* f16Add1 = f16Add1 + f16Add2 */
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "sxth %1, %1 \n\t"                /* Transforms 16-bit input f16Add2 to 32-bit */
                        "adds %0, %0, %1 \n\t"            /* a32Accum = a32Accum + f16Add2 */
                        : "+l"(a32Accum), "+l"(f16Add):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)             /* For GCC compiler */
                            ".syntax unified \n"          /* Using unified asm syntax */
                        #endif
                        "sxth %1, %1 \n"                  /* Transforms 16-bit input f16Add2 to 32-bit */
                        "adds %0, %0, %1 \n"              /* a32Accum = a32Accum + f16Add2 */
                        #if defined(__GNUC__)             /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Accum), "+l"(f16Add):);
    #endif

    return a32Accum;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_ADD_A32_ASM_H_*/
