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
* @brief  Multiply subtract of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MSU4RND_F16_ASM_H_
#define _MLIB_MSU4RND_F16_ASM_H_


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
#define MLIB_Msu4RndSat_F16_Asmi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) \
        MLIB_Msu4RndSat_F16_FAsmi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)
  
/***************************************************************************//*!
*
* f16Out = (f16MinMul1 * f16MinMul2) - (f16SubMul1 * f16SubMul2)
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_Msu4RndSat_F16_FAsmi(register frac16_t f16MinMul1, register frac16_t f16MinMul2, 
                                                 register frac16_t f16SubMul1, register frac16_t f16SubMul2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smulbb f16MinMul1, f16MinMul1, f16MinMul2    /* f16MinMul1 * f16MinMul2 */
                        smulbb f16SubMul1, f16SubMul1, f16SubMul2    /* f16SubMul1 * f16SubMul2 */
                        add f16MinMul1, f16MinMul1, #0x4000          /* Rounding */
                        qsub f16MinMul1, f16MinMul1, f16SubMul1      /* Addition with saturation */
                        ssat f16MinMul1, #16, f16MinMul1, asr #15 }; /* Result = Result * 2 */
    #else
        __asm volatile( "smulbb %0, %0, %2 \n"                       /* f16MinMul1 * f16MinMul2 */
                        "smulbb %1, %1, %3 \n"                       /* f16SubMul1 * f16SubMul2 */
                        "add %0, %0, #0x4000 \n"                     /* Rounding */
                        "qsub %0, %0, %1 \n"                         /* Addition with saturation */
                        "ssat %0, #16, %0, asr #15 \n"               /* Result = Result * 2 */
                        : "+l"(f16MinMul1), "+l"(f16SubMul1): "l"(f16MinMul2), "l"(f16SubMul2));
    #endif

    return f16MinMul1;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSU4RND_F16_ASM_H_ */
