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
* @brief  Subtraction
* 
*******************************************************************************/
#ifndef _MLIB_SUB_F32_ASM_H_
#define _MLIB_SUB_F32_ASM_H_

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
#define MLIB_SubSat_F32_Asmi(f32Min, f32Sub) MLIB_SubSat_F32_FAsmi(f32Min, f32Sub)
  
/***************************************************************************//*!
*
* Output = f32Min - f32Sub
* With saturation
*******************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_SubSat_F32_FAsmi(register frac32_t f32Min, register frac32_t f32Sub)
{
    register frac32_t f32SubVal=0, f32SatVal=0;

    #if defined(__CC_ARM)                                  /* For ARM Compiler */
        __asm volatile{ subs f32SubVal, f32Min, f32Sub     /* f32SubVal = f32Min - f32Sub */
                        movs f32SatVal, f32Min             /* f32SatVal = f32Min */
                        eors f32SatVal, f32SatVal, f32Sub  /* f32SatVal = f32Min ^ f32Sub */
                        bpl  SatEnd                        /* If f32SatVal >= 0, then goes to the SatEnd */
                        eors f32Min, f32Min, f32SubVal     /* f32Min = f32Min ^ (f32Min - f32Sub) */
                        bpl SatEnd                         /* If f32Min >= 0, then goes to the SatEnd */
                        movs f32SubVal, #128               /* f32SubVal = 0x80 */
                        lsls f32SubVal, f32SubVal, #24     /* f32SubVal = 0x80000000 */
                        cmp f32Sub, #0                     /* Compares f32Sub with 0 */
                        bgt SatEnd                         /* If f32Min > 0, then goes to the SatEnd */
                        subs f32SubVal, f32SubVal, #1      /* f32SubVal = 0x7FFFFFFF */
                    SatEnd:};
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "subs %0, %1, %2 \n\t"             /* f32SubVal = f32Min - f32Sub */
                        "movs %3, %1 \n\t"                 /* f32SatVal = f32Min */
                        "eors %3, %3, %2 \n\t"             /* f32SatVal = f32Min ^ f32Sub */
                        "bpl MLIB_SUB_F32_SatEnd%= \n\t"     /* If f32SatVal >= 0, then goes to the end of function */
                        "eors %1, %1, %0 \n\t"             /* f32Min = f32Min ^ (f32Min - f32Sub) */
                        "bpl MLIB_SUB_F32_SatEnd%= \n\t"     /* If f32Min >= 0, then goes to the end of function */
                        "movs %0, #128 \n\t"               /* f32SubVal = 0x80 */
                        "lsls %0, %0, #24 \n\t"            /* f32SubVal = 0x80000000 */
                        "cmp %2, #0 \n\t"                  /* Compares f32Sub with 0 */
                        "bgt MLIB_SUB_F32_SatEnd%= \n\t"     /* If f32Min > 0, then goes to the end of function */
                        "subs %0, %0, #1 \n\t"             /* f32SubVal = 0x7FFFFFFF */
                    "MLIB_SUB_F32_SatEnd%=: \n\t" 
                        : "=&l"(f32SubVal), "+l"(f32Min), "+l"(f32Sub), "+l"(f32SatVal):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)              /* For GCC compiler */
                            ".syntax unified \n"           /* Using unified asm syntax */
                        #endif
                        "subs %0, %1, %2 \n"               /* f32SubVal = f32Min - f32Sub */
                        "movs %3, %1 \n"                   /* f32SatVal = f32Min */
                        "eors %3, %3, %2 \n"               /* f32SatVal = f32Min ^ f32Sub */
                        "bpl .+16 \n"                      /* If f32SatVal >= 0, then goes to the end of function */
                        "eors %1, %1, %0 \n"               /* f32Min = f32Min ^ (f32Min - f32Sub) */
                        "bpl .+12 \n"                      /* If f32Min >= 0, then goes to the end of function */
                        "movs %0, #128 \n"                 /* f32SubVal = 0x80 */
                        "lsls %0, %0, #24 \n"                  /* f32SubVal = 0x80000000 */
                        "cmp %2, #0 \n"                    /* Compares f32Sub with 0 */
                        "bgt .+4 \n"                       /* If f32Min > 0, then goes to the end of function */
                        "subs %0, %0, #1 \n"               /* f32SubVal = 0x7FFFFFFF */
                        #if defined(__GNUC__)              /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "=&l"(f32SubVal), "+l"(f32Min), "+l"(f32Sub), "+l"(f32SatVal):);
    #endif

    return f32SubVal;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_F32_ASM_H_ */
