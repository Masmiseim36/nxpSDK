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
* @brief  Moving average filter 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERMA_A32_ASMI_H_
#define _GDFLIB_FILTERMA_A32_ASMI_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gdflib_types.h"
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterMA_F16_Asmi(f16InX, psParam) \
        GDFLIB_FilterMA_F16_FAsmi(f16InX, psParam)
  
/***************************************************************************//*!
*
* @brief  The function calculates recursive form of moving average filter
*
* @param  ptr   GDFLIB_FILTER_MA_T_A32 * psParam
*               - Pointer to filter structure
*               
*               GDFLIB_FILTER_MA_T_A32 structure:                    
*               - a32Acc - accumulator of the filter
*               - u16Sh - 2 ^ u16Sh is the number of values
*
* @param  in    f16x - input signal
*
* @return This function returns
*               - frac16_t value <-1;1)
*        
* @remarks 
* 
*         Filter Equation:
*
*         sum(k)   = sum(k-1) + input
*         out(k)   = sum(k)/N
*         sum(k-1) = sum(k) - out(k)
*
****************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t GDFLIB_FilterMA_F16_FAsmi(frac16_t f16InX,
                                               GDFLIB_FILTER_MA_T_A32 *psParam)
{

    register frac32_t f32Val1=0, f32Val2=0;
    #if defined(__CC_ARM)                              /* For ARM Compiler */
        __asm volatile{ sxth f16InX, f16InX            /* Sign extend */
                        /* a32Acc + f16InX  */
                        ldr f32Val1, [psParam]         /* f32Val1 = psParam.a32Acc */
                        adds f32Val2, f32Val1, f16InX  /* f32Val2 = a32Acc + f16InX */
                        subs f32Val1, f32Val2, f32Val1 /* f32Val1 = f32Val2 - a32Acc */
                        cmp f32Val1, f16InX            /* Compares f32Val2 and f32Val1 */
                        beq Add_NotSat                 /* If f32Val2 != f32Val1, then saturates result */
                        asrs f16InX, f16InX, #31       /* f16InX >> 16 */
                        movs f32Val2, #0x80            /* f32Val2 = 0x80 */
                        rev f32Val2, f32Val2           /* f32Val2 = 0x80000000 */
                        subs f32Val2, f32Val2, #1      /* f32Val2 = 0x7FFFFFFF */
                        subs f32Val2, f32Val2, f16InX  /* f32Val2 = 0x7FFFFFFF - f16InX */
                        /* (a32Acc + f16InX) >> u16Sh */
                    Add_NotSat:
                        ldrh f32Val1, [psParam, #4]    /* Loads psParam.u16Sh */
                        mov f16InX, f32Val2            /* f16InX = a32Acc + f16InX */
                        asrs f16InX, f16InX, f32Val1   /* f32Val1 = (a32Acc + f16InX) >> u16Sh */
                        /* (a32Acc + f16InX) - ((a32Acc + f16InX) >> u16Sh)*/
                        subs f32Val1, f32Val2, f16InX  /* f32Val1 = (a32Acc + f16InX) - ((a32Acc + f16InX) >> u16Sh) */
                        subs f32Val2, f32Val2, f32Val1 /* f32Val2 = f32Val1 - (a32Acc + f16InX) */
                        cmp f32Val2, f16InX            /* Compares values */
                        beq Sub_NotSat                 /* If values are not equal, then saturates result */
                        asrs f32Val1, f16InX, #31      /* f32Val1 = f16InX >> 16 */
                        movs f32Val2, #0x80            /* f32Val2 = 0x80 */
                        rev f32Val2, f32Val2           /* f32Val2 = 0x80000000 */
                        subs f32Val1, f32Val2, f32Val1 /* f32Val1 = 0x80000000 - f32Val1 */
                    Sub_NotSat:
                        str f32Val1, [psParam]         /* Saves new value of psParam.a32Acc */
                        /* Output saturation */
                        sxth f32Val1, f16InX           /* Sign extend */
                        cmp f32Val1, f16InX            /* Compares f32Val1 with f16InX */
                        beq Res_NotSat                 /* If f32Val1 != f16InX, then saturates result */
                        movs f32Val1, #0x80            /* f32Val1 = 0x80 */
                        lsls f32Val1, #8               /* f32Val1 = 0x8000 */
                        asrs f16InX, f16InX, #31       /* f16InX >> 16*/
                        subs f32Val1, f32Val1, #1      /* f32Val1 = f32Val1 - 1 */
                        subs f16InX, f32Val1, f16InX   /* f16InX = f32Val1 - f16InX */
                    Res_NotSat:};
    #else
        __asm volatile(
                        #if defined(__GNUC__)     /* For GCC compiler */
                            ".syntax unified \n"  /* Using unified asm syntax */
                        #endif
                        "sxth %0, %0 \n"          /* Sign extend */
                        /* a32Acc + f16InX  */
                        "ldr %1, [%3] \n"         /* f32Val1 = psParam.a32Acc */
                        "adds %2, %1, %0 \n"      /* f32Val2 = a32Acc + f16InX */
                        "subs %1, %2, %1 \n"      /* f32Val1 = f32Val2 - a32Acc */
                        "cmp %1, %0 \n"           /* Compares f32Val2 and f32Val1 */
                        "beq .+12 \n"             /* If f32Val2 != f32Val1, then saturates result */
                        "asrs %0, %0, #31 \n"     /* f16InX >> 16 */
                        "movs %2, #0x80 \n"       /* f32Val2 = 0x80 */
                        "rev %2, %2 \n"           /* f32Val2 = 0x80000000 */
                        "subs %2, %2, #1 \n"      /* f32Val2 = 0x7FFFFFFF */
                        "subs %2, %2, %0 \n"      /* f32Val2 = 0x7FFFFFFF - f16InX */
                        /* (a32Acc + f16InX) >> u16Sh */
                        "ldrh %1, [%3, #4] \n"    /* Loads psParam.u16Sh */
                        "mov %0, %2 \n"           /* f16InX = a32Acc + f16InX */
                        "asrs %0, %0, %1 \n"      /* f32Val1 = (a32Acc + f16InX) >> u16Sh */
                        /* (a32Acc + f16InX) - ((a32Acc + f16InX) >> u16Sh)*/
                        "subs %1, %2, %0 \n"      /* f32Val1 = (a32Acc + f16InX) - ((a32Acc + f16InX) >> u16Sh) */
                        "subs %2, %2, %1 \n"      /* f32Val2 = f32Val1 - (a32Acc + f16InX) */
                        "cmp %2, %0 \n"           /* Compares values */
                        "beq .+10 \n"             /* If values are not equal, then saturates result */
                        "asrs %1, %0, #31 \n"     /* f32Val1 = f16InX >> 16 */
                        "movs %2, #0x80 \n"       /* f32Val2 = 0x80 */
                        "rev %2, %2 \n"           /* f32Val2 = 0x80000000 */
                        "subs %1, %2, %1 \n"      /* f32Val1 = 0x80000000 - f32Val1 */
                        "str %1, [%3] \n"         /* Saves new value of psParam.a32Acc */
                        /* Output saturation */
                        "sxth %1, %0 \n"          /* Sign extend */
                        "cmp %1, %0 \n"           /* Compares f32Val1 with f16InX */
                        "beq .+12 \n"             /* If f32Val1 != f16InX, then saturates result */
                        "movs %1, #0x80 \n"       /* f32Val1 = 0x80 */
                        "lsls %1, %1, #8 \n"      /* f32Val1 = 0x8000 */
                        "asrs %0, %0, #31 \n"     /* f16InX >> 16*/
                        "subs %1, %1, #1 \n"      /* f32Val1 = f32Val1 - 1 */
                        "subs %0, %1, %0 \n"      /* f16InX = f32Val1 - f16InX */
                        #if defined(__GNUC__)     /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16InX), "+l"(f32Val1), "+l"(f32Val2): "l"(psParam));
    #endif

    return(f16InX);
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERMA_A32_ASMI_H_ */
