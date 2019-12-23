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
* @brief  Sign
* 
*******************************************************************************/
#ifndef _MLIB_SIGN_F32_ASM_H_
#define _MLIB_SIGN_F32_ASM_H_

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
#define MLIB_Sign_F32_Asmi(f32Val) MLIB_Sign_F32_FAsmi(f32Val)

/***************************************************************************//*!
*   This function returns the sign of the argument:
*           f32Val = 0: returns 0
*           f32Val > 0: returns 2147483647 (fractional 1.0)
*           f32Val < 0: returns -2147483648 (fractional -1.0)
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
static inline frac32_t MLIB_Sign_F32_FAsmi(register frac32_t f32Val)
{
    register frac32_t f32SigVal = 0x80;
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ cmp f32Val, #0                  /* Compares f32Val with 0 */
                        bge Next                        /* If f32Val >= 0, then goes to Next */
                        rev f32Val, f32SigVal           /* If f32Val < 0, then f32Val = 0x80000000 */
                    Next:
                        ble SignEnd                     /* If f32Val = 0, then goes to SignEnd */
                        rev f32Val, f32SigVal           /* If f32Val > 0, then f32Val = 0x80000000 */
                        subs f32Val, f32Val, #1         /* f32Val = f32Val - 1 */
                    SignEnd: };
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "cmp %0, #0 \n"                 /* Compares f32Val with 0 */
                        "bge .+4 \n"                    /* If f32Val >= 0, then jumps through next command */
                        "rev %0, %1 \n"                 /* If f32Val < 0, then f32Val = 0x80000000 */
                        "ble .+6 \n"                    /* If f32Val = 0, then jumps through next command */
                        "rev %0, %1 \n"                 /* If f32Val > 0, then f32Val = 0x80000000 */
                        "subs %0, %0, #1 \n"            /* f32Val = f32Val - 1 */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Val): "l"(f32SigVal));
    #endif

    return f32Val;
}
#if defined(__IAR_SYSTEMS_ICC__) /* set no optimization for IAR compiler   */
    #pragma language=restore     /* Restore original optimization level */         
#elif defined(__CC_ARM)          /* set no optimization for ARM(KEIL) compiler */  
#else                            /* set no optimization for GCC(KDS) compiler */                
#endif 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SIGN_F32_ASM_H_ */
