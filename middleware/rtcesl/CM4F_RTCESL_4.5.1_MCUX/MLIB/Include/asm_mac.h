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
* @brief  asm_mac.h
*
*******************************************************************************/

#ifndef ASM_MAC_H_
#define ASM_MAC_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Macros
*******************************************************************************/

#if defined(__IAR_SYSTEMS_ASM__)           /* IAR compiler   */
    #define ASM_PREFIX(x)                   x
    #define ASM_LABEL(label)                label
    #define ASM_EXTERN(label)               EXTERN  label
    #define ASM_ALIGN(value)                ALIGNROM  value
    #define ASM_PUBLIC(label)               PUBLIC label
    #define ASM_CONST16(value)              DC16 value
    #define ASM_CONST32(value)              DC32 value
    #define ASM_LABEL_CONST32(label,value)  ASM_LABEL(label) ASM_CONST32(value)
    #define ASM_DATA_SECTION(label)         SECTION label : DATA (4)
    #define ASM_CODE_SECTION(label)         SECTION label : CODE (4)
    #define ASM_END                         END
    #define ASM_EQUATE(label, value)        label   EQU  value
    #define ASM_COMP_SPECIFIC_DIRECTIVES
    #define ASM_CODE                        CODE
    #define ASM_DATA                        DATA
#elif defined(__GNUC__)                     /* GCC compiler */
    #define ASM_PREFIX(x)                   x
    #define ASM_LABEL(label)                label:
    #define ASM_EXTERN(label)               .extern ASM_PREFIX(label)
    #define ASM_ALIGN(value)                .balign value
    #define ASM_PUBLIC(label)               .global ASM_PREFIX(label)
    #define ASM_CONST16(value)              .short value
    #define ASM_CONST32(value)              .long value
    #define ASM_LABEL_CONST32(label,value)  ASM_LABEL(label) ASM_CONST32(value)
    #define ASM_DATA_SECTION(name)          .section name
    #define ASM_CODE_SECTION(name)          .section name
    #define ASM_END                         .end
    #define ASM_EQUATE(label,value)         .equ label, value
    #define ASM_COMP_SPECIFIC_DIRECTIVES    .syntax unified
    #define ASM_CODE                        .thumb
    #define ASM_DATA
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* ARM compiler */
    #define ASM_PREFIX(x)                   x
    #define ASM_LABEL(label)                label
    #define ASM_EXTERN(label)               EXTERN label
    #define ASM_ALIGN(value)                ALIGN value
    #define ASM_PUBLIC(label)               EXPORT label
    #define ASM_CONST16(value)              DCWU value
    #define ASM_CONST32(value)              DCDU value
    #define ASM_LABEL_CONST32(label,value)  ASM_LABEL(label) ASM_CONST32(value)
    #define ASM_DATA_SECTION(label)         AREA |label|, DATA
    #define ASM_CODE_SECTION(label)         AREA |label|, CODE
    #define ASM_END                         END
    #define ASM_EQUATE(label, value)        label   EQU  value
    #define ASM_COMP_SPECIFIC_DIRECTIVES
    #define ASM_CODE                        THUMB
    #define ASM_DATA
#endif

#if defined(__cplusplus)
}
#endif

#endif /* ASM_MAC_H_ */
