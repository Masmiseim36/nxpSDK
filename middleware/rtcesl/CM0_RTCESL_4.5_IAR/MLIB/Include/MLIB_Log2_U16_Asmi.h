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
* @brief  Binary logarithm
* 
*******************************************************************************/
#ifndef _MLIB_LOG2_U16_ASM_H_
#define _MLIB_LOG2_U16_ASM_H_

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
#define MLIB_Log2_U16_Asmi(u16Val) MLIB_Log2_U16_FAsmi(u16Val)
  
/***************************************************************************//*!
*
* @brief    Binary logarithm of 16-bit input
*
* @param    in  uint16_t u16Val
*                 - Argument in uint16_t
*
* @return   This function returns
*                 - uint16_t value
*       
* @remarks  This function returns the 16-bit integer part of binary logarithm of the input. 
*           Returns 0 for input u16Val = 0.
*
*******************************************************************************/
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline uint16_t MLIB_Log2_U16_FAsmi(register uint16_t u16Val)
{
    register uint16_t u16Log2 = 0;                      /* Initialization of u16Log2 */

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ cmp u16Val, #0xFF               /* Compares u16Val with 0xFF */
                        ble Cmp1                        /* If u16Val <= 0xFF, then goes to Cmp1 */
                        movs u16Log2, #8                /* u16Log2 = 8 */
                        lsrs u16Val, u16Val, #8         /* u16Val >> 8 */
                    Cmp1:
                        cmp u16Val, #0xF                /* Compares u16Val with 0xF */
                        ble Cmp2                        /* If u16Val <= 0xF, then goes to Cmp2 */
                        adds u16Log2, #4                /* u16Log2 = u16Log2 + 4 */
                        lsrs u16Val, u16Val, #4         /* u16Val >> 4 */
                    Cmp2:
                        cmp u16Val, #0x3                /* Compares u16Val with 0x3 */
                        ble Cmp3                        /* If u16Val <= 0x3, then goes to Cmp3 */
                        adds u16Log2, #2                /* u16Log2 = u16Log2 + 2 */
                        lsrs u16Val, u16Val, #2         /* u16Val >> 2 */
                    Cmp3:
                        lsrs u16Val, u16Val, #1         /* u16Val >> 1 */
                        orrs u16Val, u16Val, u16Log2 }; /* u16Val = u16Log2 | u16Val */
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "cmp %0, #0xFF \n"              /* Compares u16Val with 0xFF */
                        "ble .+6 \n"                    /* If u16Val <= 0xFF, then jumps through two commands */
                        "movs %1, #8 \n"                /* u16Log2 = 8 */
                        "lsrs %0, %0, #8 \n"            /* u16Val >> 8 */

                        "cmp %0, #0xF \n"               /* Compares u16Val with 0xF */
                        "ble .+6 \n"                    /* If u16Val <= 0xF, then jumps through two commands */
                        "adds %1, #4 \n"                /* u16Log2 = u16Log2 + 4 */
                        "lsrs %0, %0, #4 \n"            /* u16Val >> 4 */

                        "cmp %0, #0x3 \n"               /* Compares u16Val with 0x3 */
                        "ble .+6 \n"                    /* If u16Val <= 0x3, then jumps through two commands */
                        "adds %1, #2 \n"                /* u16Log2 = u16Log2 + 2 */
                        "lsrs %0, %0, #2 \n"            /* u16Val >> 2 */

                        "lsrs %0, %0, #1 \n"            /* u16Val = 1 */
                        "orrs %0, %0, %1 \n"            /* u16Val = u16Log2 | u16Val */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(u16Val), "+l"(u16Log2):);
    #endif

   return u16Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_LOG2_U16_ASM_H_ */
