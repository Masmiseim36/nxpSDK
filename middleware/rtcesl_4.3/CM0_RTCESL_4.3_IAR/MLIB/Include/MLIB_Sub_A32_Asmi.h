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
* @brief  Subtraction
* 
*******************************************************************************/
#ifndef _MLIB_SUB_A32_ASM_H_
#define _MLIB_SUB_A32_ASM_H_

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
#define MLIB_Sub_A32ss_Asmi(f16Min, f16Sub) MLIB_Sub_A32ss_FAsmi(f16Min, f16Sub)
#define MLIB_Sub_A32as_Asmi(a32Accum, f16Sub) MLIB_Sub_A32as_FAsmi(a32Accum, f16Sub)
  
/***************************************************************************//*!
*
* a32Out = f16Min - f16Sub
* 
*******************************************************************************/   
/* inline function without any optimization (compilation issue) */
#if defined(__IAR_SYSTEMS_ICC__) /* set no optimization for IAR compiler   */
    #pragma language=save        /* Save existing optimization level */
    #pragma optimize=none        /* Optimization none level now */        
#elif defined(__CC_ARM)          /* set no optimization for ARM(KEIL) compiler */
    #pragma O0                   /* Optimization level now O0 */                    
#else                            /* set no optimization for GCC(KDS) compiler */
    __attribute__((optimize("O0")))  
#endif
static inline acc32_t MLIB_Sub_A32ss_FAsmi(register frac16_t f16Min, register frac16_t f16Sub)
{
    register acc32_t a32Val=0;
    #if defined(__CC_ARM)                                  /* For ARM Compiler */
        __asm volatile{ sxth f16Min, f16Min                /* Transforms 16-bit input f16Sub to 32-bit */
                        sxth f16Sub, f16Sub                /* Transforms 16-bit input f16Sub to 32-bit */
                        subs a32Val, f16Min, f16Sub};      /* f16Min = f16Min - f16Sub */
    #else
        __asm volatile(
                        #if defined(__GNUC__)              /* For GCC compiler */
                            ".syntax unified \n"           /* Using unified asm syntax */
                        #endif
                        "sxth %1, %1 \n"                   /* Transforms 16-bit input f16Sub to 32-bit */
                        "sxth %2, %2 \n"                   /* Transforms 16-bit input f16Sub to 32-bit */
                        "subs %0, %1, %2 \n"               /* f16Min = f16Min - f16Sub */
                        #if defined(__GNUC__)              /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "=l"(a32Val), "+l"(f16Min), "+l"(f16Sub):);
    #endif

    return a32Val;
}
#if defined(__IAR_SYSTEMS_ICC__) /* set no optimization for IAR compiler   */
    #pragma language=restore     /* Restore original optimization level */         
#elif defined(__CC_ARM)          /* set no optimization for ARM(KEIL) compiler */  
#else                            /* set no optimization for GCC(KDS) compiler */                
#endif 

/***************************************************************************//*!
*
* a32Out = a32Accum - f16Sub
* 
*******************************************************************************/   
/* inline function without any optimization (compilation issue) */
#if defined(__IAR_SYSTEMS_ICC__) /* set no optimization for IAR compiler   */
    #pragma language=save        /* Save existing optimization level */
    #pragma optimize=none        /* Optimization none level now */        
#elif defined(__CC_ARM)          /* set no optimization for ARM(KEIL) compiler */
    #pragma O0                   /* Optimization level now O0 */                    
#else                            /* set no optimization for GCC(KDS) compiler */
    __attribute__((optimize("O0")))  
#endif
static inline acc32_t MLIB_Sub_A32as_FAsmi(register acc32_t a32Accum, register frac16_t f16Sub)
{
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ sxth f16Sub, f16Sub                 /* Transforms 16-bit input f16Sub to 32-bit */
                        subs a32Accum, a32Accum, f16Sub};   /* a32Accum = a32Accum - f16Sub */

    #else
        __asm volatile(
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "sxth %1, %1 \n"                    /* Transforms 16-bit input f16Sub to 32-bit */
                        "subs %0, %0, %1 \n"                /* a32Accum = a32Accum - f16Sub */
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Accum), "+l"(f16Sub):);
    #endif

    return a32Accum;
}
#if defined(__IAR_SYSTEMS_ICC__) /* set no optimization for IAR compiler   */
    #pragma language=restore     /* Restore original optimization level */         
#elif defined(__CC_ARM)          /* set no optimization for ARM(KEIL) compiler */  
#else                            /* set no optimization for GCC(KDS) compiler */                
#endif 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_A32_ASM_H_ */
