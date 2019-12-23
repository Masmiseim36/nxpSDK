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
* @brief  Summation of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_ADD4_F32_ASM_H_
#define _MLIB_ADD4_F32_ASM_H_

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
#define MLIB_Add4Sat_F32_Asmi(f32Add1, f32Add2, f32Add3, f32Add4) MLIB_Add4Sat_F32_FAsmi(f32Add1, f32Add2, f32Add3, f32Add4)

/***************************************************************************//*!
*  Saturated summation of four inputs      
*  Output = f32Add1 + f32Add2 + f32Add3 + f32Add4
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
static inline frac32_t MLIB_Add4Sat_F32_FAsmi(register frac32_t f32Add1, register frac32_t f32Add2, register frac32_t f32Add3, register frac32_t f32Add4)
{
    register frac32_t f32Val1=0, f32Val2=0;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ asrs f32Val1, f32Add1, #31      /* f32Val1 = sign of f32Add1 */
                        asrs f32Val2, f32Add2, #31      /* f32Val2 = sign of f32Add2 */

                        adds f32Add1, f32Add1, f32Add2  /* f32Add1 = f32Add1 + f32Add2 */
                        adcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 + f32Val2 + carry */

                        asrs f32Val2, f32Add3, #31      /* f32Val2 = sign of f32Add3 */
                        adds f32Add1, f32Add1, f32Add3  /* f32Add1 = f32Add1 + f32Add3 */
                        adcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 + f32Val2 + carry */

                        asrs f32Val2, f32Add4, #31      /* f32Val2 = sign of f32Add4 */
                        adds f32Add1, f32Add1, f32Add4  /* f32Add1 = f32Add1 + f32Add4 */
                        adcs f32Val1, f32Val1, f32Val2  /* f32Val1 = f32Val1 + f32Val2 + carry */

                        lsrs f32Val2, f32Add1, #31      /* f32Val2 = first bit of f32Add1 */
                        adds f32Val2, f32Val2, f32Val1  /* f32Val2 = f32Val2 + f32Val1 */
                        beq SatEnd                      /* If r3 != 0, then saturates output */

                        movs f32Add1, #128              /* f32Add1 = 0x80 */
                        rev f32Add1, f32Add1            /* f32Add1 = 0x80000000 */
                        subs f32Add1, f32Add1, #1       /* f32Add1 = 0x7FFFFFFF */
                        asrs f32Val2, f32Val2, #16      /* f32Val2 = sign of result */
                        subs f32Add1, f32Add1, f32Val2  /* f32Add1 = 0x7FFFFFFF - f32Val2 */
                    SatEnd: };

    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "asrs %1, %0, #31 \n"           /* f32Val1 = sign of f32Add1 */
                        "asrs %2, %3, #31 \n"           /* f32Val2 = sign of f32Add2 */

                        "adds %0, %0, %3 \n"            /* f32Add1 = f32Add1 + f32Add2 */
                        "adcs %1, %1, %2 \n"            /* f32Val1 = f32Val1 + f32Val2 + carry */

                        "asrs %2, %4, #31 \n"           /* f32Val2 = sign of f32Add3 */
                        "adds %0, %0, %4 \n"            /* f32Add1 = f32Add1 + f32Add3 */
                        "adcs %1, %1, %2 \n"            /* f32Val1 = f32Val1 + f32Val2 + carry */

                        "asrs %2, %5, #31 \n"           /* f32Val2 = sign of f32Add4 */
                        "adds %0, %0, %5 \n"            /* f32Add1 = f32Add1 + f32Add4 */
                        "adcs %1, %1, %2 \n"            /* f32Val1 = f32Val1 + f32Val2 + carry */

                        "lsrs %2, %0, #31 \n"           /* f32Val2 = first bit of f32Add1 */
                        "adds %2, %2, %1 \n"            /* f32Val2 = f32Val2 + f32Val1 */
                        "beq .+12 \n"                      /* If r3 != 0, then saturates output */

                        "movs %0, #128 \n"              /* f32Add1 = 0x80 */
                        "rev %0, %0 \n"                 /* f32Add1 = 0x80000000 */
                        "subs %0, %0, #1 \n"            /* f32Add1 = 0x7FFFFFFF */
                        "asrs %2, %2, #16 \n"           /* f32Val2 = sign of result */
                        "subs %0, %0, %2 \n"            /* f32Add1 = 0x7FFFFFFF - f32Val2 */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                       : "+l"(f32Add1), "+l"(f32Val1), "+l"(f32Val2): "l"(f32Add2), "l"(f32Add3), "l"(f32Add4));
    #endif

    return f32Add1;
}
#if defined(__IAR_SYSTEMS_ICC__) /* set no optimization for IAR compiler   */
    #pragma language=restore     /* Restore original optimization level */         
#elif defined(__CC_ARM)          /* set no optimization for ARM(KEIL) compiler */  
#else                            /* set no optimization for GCC(KDS) compiler */                
#endif 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_ADD4_F32_ASM_H_ */
