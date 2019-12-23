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
* @brief  Reciprocal value
* 
*******************************************************************************/
#ifndef _MLIB_RCPHW1Q_A32_ASM_H_
#define _MLIB_RCPHW1Q_A32_ASM_H_

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
#define MLIB_RcpHw1Q1_A32s_Asmi(f16Denom) MLIB_RcpHw1Q_A32s_FAsmi(f16Denom)
#define MLIB_RcpHw1Q_A32s_Asmi(f16Denom) MLIB_RcpHw1Q_A32s_FAsmi(f16Denom)

/*******************************************************************************
* @brief    16-bit input 32-output 16-bit precision one quadrant reciprocal 
*           function
*  
* @param    in      frac16_t f16Denom
*                     - Denominator in <0;1) in frac16_t
* @return   This function returns
*                   acc32_t value [1;65536.0)
*
* @remarks  This function calculates the multiplicative inverse value of
*           the fractional input:
*           result = FRAC16(1) / f16Denom. The function calculates
*           the result with 16-bit division precision.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
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
static inline acc32_t MLIB_RcpHw1Q_A32s_FAsmi(register frac16_t f16Denom)
{
  register acc32_t a32Result = 0x7FFFFFFF;
  register frac32_t f32BasePtr = 0xF0004000;
  register frac32_t f32Num = 0x40000000;
  register frac32_t f32Temp1=0, f32Temp2=0;

    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ sxth    f16Denom, f16Denom           /* Converts 16 input to 32-bit value */
                        cmp     f16Denom, #0                 /* Compares Denominator with 0 */
                        beq     RcpHw1Q_A32s_End             /* If Denominator = 0, then goes to RcpHw1Q_A32s_End */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                    RcpHw1Q_A32s_Recount:
                        adds    f32Temp2, f32Temp1, #1       /* f32Temp2 = gu16CntMmdvsq + 1 */
                        strh    f32Temp2, [&gu16CntMmdvsq]   /* gu16CntMmdvsq = f32Temp2 */
                        movs    f32Temp1, #0                 /* f32TestVal = 0 */
                        str     f32Temp1, [f32BasePtr, #8]   /* MMDVSQ_CSR = 0x00000000 */
                        str     f32Num, [f32BasePtr]         /* MMDVSQ_DEND = f32Num */
                        str     f16Denom, [f32BasePtr, #4]   /* MMDVSQ_DSOR = f16Denom */
                        ldr     a32Result, [f32BasePtr, #12] /* a32Result = MMDVSQ_RES */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                        cmp     f32Temp1, f32Temp2           /* Compares f32Temp1 and f32Temp2 */
                        bne     RcpHw1Q_A32s_Recount         /* If f32Temp1 != f32Temp2, then goes to RcpHw1Q_A32s_Recount */
                    RcpHw1Q_A32s_End:  };
    #else
        __asm volatile(
                        #if defined(__GNUC__)                /* For GCC compiler */
                            ".syntax unified \n"             /* Using unified asm syntax */
                        #endif
                        "sxth    %0, %0 \n"                  /* Converts 16 input to 32-bit value */
                        "cmp     %0, #0 \n"                  /* Compares Denominator with 0 */
                        "beq     RcpHw1Q_A32s_End \n"        /* If Denominator = 0, then goes to RcpHw1Q_A32s_End */
                        "ldrh    %2, [%4] \n"                /* f32Temp1 = gu16CntMmdvsq */
                    "RcpHw1Q_A32s_Recount: \n"
                        "adds    %3, %2, #1 \n"              /* f32Temp2 = gu16CntMmdvsq + 1 */
                        "strh    %3, [%4] \n"                /* gu16CntMmdvsq = f32Temp2 */
                        "movs    %2, #0 \n"                  /* f32TestVal = 0 */
                        "str     %2, [%5, #8] \n"            /* MMDVSQ_CSR = 0x00000000 */
                        "str     %6, [%5] \n"                /* MMDVSQ_DEND = f32Num */
                        "str     %0, [%5, #4] \n"            /* MMDVSQ_DSOR = f16Denom */
                        "ldr     %1, [%5, #12] \n"           /* f16Result = MMDVSQ_RES */
                        "ldrh    %2, [%4] \n"                /* f32Temp1 = gu16CntMmdvsq */
                        "cmp     %2, %3 \n"                  /* Compares f32Temp1 and f32Temp2 */
                        "bne     RcpHw1Q_A32s_Recount \n"    /* If f32Temp1 != f32Temp2, then goes to RcpHw1Q_A32s_Recount */
                    "RcpHw1Q_A32s_End: \n"
                        #if defined(__GNUC__)                /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Denom), "+l"(a32Result), "+l"(f32Temp1), "+l"(f32Temp2): "l"(&gu16CntMmdvsq), "l"(f32BasePtr), "l"(f32Num):);
    #endif
    return a32Result;
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

#endif /* _MLIB_RCPHW1Q_A32_ASM_H_ */
