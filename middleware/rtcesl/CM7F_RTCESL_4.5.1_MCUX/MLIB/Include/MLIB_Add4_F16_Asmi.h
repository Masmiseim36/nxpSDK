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
* @brief  Summation of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_ADD4_F16_ASM_H_
#define _MLIB_ADD4_F16_ASM_H_

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
#define MLIB_Add4_F16_Asmi(f16Add1, f16Add2, f16Add3, f16Add4)                  \
        MLIB_Add4_F16_FAsmi(f16Add1, f16Add2, f16Add3, f16Add4)
#define MLIB_Add4Sat_F16_Asmi(f16Add1, f16Add2, f16Add3, f16Add4)               \
        MLIB_Add4Sat_F16_FAsmi(f16Add1, f16Add2, f16Add3, f16Add4)

/***************************************************************************//*!
*  Summation of four inputs
*     f16Out = f16Add1 + f16Add2 + f16Add3 + f16Add4
*  function is not saturated
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_Add4_F16_FAsmi(register frac16_t f16Add1, register frac16_t f16Add2,
                                           register frac16_t f16Add3, register frac16_t f16Add4)
{
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{add f16Add1, f16Add1, f16Add2    /* f16Add1 = f16Add1 + f16Add2 */
                       add f16Add1, f16Add1, f16Add3    /* f16Add1 = f16Add1 + f16Add3 */
                       add f16Add1, f16Add1, f16Add4};  /* f16Add1 = f16Add1 + f16Add4 */

    #else
        __asm volatile( "add %0, %0, %1 \n"             /* f16Add1 = f16Add1 + f16Add2 */
                        "add %0, %0, %2 \n"             /* f16Add1 = f16Add1 + f16Add3 */
                        "add %0, %0, %3 \n"             /* f16Add1 = f16Add1 + f16Add4 */
                        : "+l"(f16Add1): "l"(f16Add2), "l"(f16Add3), "l"(f16Add4));
    #endif

    return f16Add1;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*  Saturated summation of four inputs
*     f16Out = f16Add1 + f16Add2 + f16Add3 + f16Add4
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_Add4Sat_F16_FAsmi(register frac16_t f16Add1, register frac16_t f16Add2,
                                              register frac16_t f16Add3, register frac16_t f16Add4)
{        
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ sxth f16Add1, f16Add1               /* Transforms 16-bit input f16Val to 32-bit */
                        sxth f16Add2, f16Add2               /* Transforms 16-bit input f16Val to 32-bit */
                        sxth f16Add3, f16Add3               /* Transforms 16-bit input f16Val to 32-bit */
                        sxth f16Add4, f16Add4               /* Transforms 16-bit input f16Val to 32-bit */
                        add f16Add1, f16Add1, f16Add3       /* f16Add1 + f16Add3 */
                        add f16Add2, f16Add2, f16Add4       /* f16Add2 + f16Add4 */
                        add f16Add1, f16Add1, f16Add2       /* f16Add1 + f16Add3 + f16Add2 + f16Add4 */
                        ssat f16Add1, #16, f16Add1 };       /* Saturation */
    #else
        __asm volatile( "sxth %0, %0 \n"                     /* Transforms 16-bit input f16Val to 32-bit */
                        "sxth %1, %1 \n"                     /* Transforms 16-bit input f16Val to 32-bit */
                        "sxth %2, %2 \n"                     /* Transforms 16-bit input f16Val to 32-bit */
                        "sxth %3, %3 \n"                     /* Transforms 16-bit input f16Val to 32-bit */
                        "add %0, %0, %2 \n"                  /* f16Add1 + f16Add3 */
                        "add %1, %1, %3 \n"                  /* f16Add2 + f16Add4 */
                        "add %0, %0, %1 \n"                  /* f16Add1 + f16Add3 + f16Add2 + f16Add4 */
                        "ssat %0, #16, %0 \n"                /* Saturation */
                        : "+l"(f16Add1), "+l"(f16Add2), "+l"(f16Add3), "+l"(f16Add4):);
    #endif

    return f16Add1;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD4_F16_ASM_H_ */
