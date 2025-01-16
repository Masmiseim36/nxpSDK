/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
* 
*
****************************************************************************//*!
*
* @brief  Multiply accumulate of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MAC4_F32_ASM_H_
#define _MLIB_MAC4_F32_ASM_H_

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
#define MLIB_Mac4Sat_F32ssss_Asmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)  \
        MLIB_Mac4Sat_F32ssss_FAsmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)

/***************************************************************************//*!
*
* f32Out = (f16Add1Mul1 * f16Add1Mul2) + (f16Add2Mul1 * f16Add2Mul2)
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_Mac4Sat_F32ssss_FAsmi(register frac16_t f16Add1Mul1, register frac16_t f16Add1Mul2,
                                                  register frac16_t f16Add2Mul1, register frac16_t f16Add2Mul2)
{
    register frac32_t f32Out=0;
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smulbb f16Add1Mul1, f16Add1Mul1, f16Add1Mul2 /* f16Add1Mul1 * f16Add1Mul2 */
                        smulbb f32Out, f16Add2Mul1, f16Add2Mul2      /* f16Add2Mul1 * f16Add2Mul2 */
                        qadd f32Out, f32Out, f16Add1Mul1             /* Addition with saturation */
                        qadd f32Out, f32Out, f32Out };               /* Addition with saturation */
    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)            /* For GCC compiler */
                            ".syntax unified \n"                     /* Using unified asm syntax */
                        #endif
                        "smulbb %1, %1, %2 \n"                       /* f16Add1Mul1 * f16Add1Mul2 */
                        "smulbb %2, %3, %4 \n"                       /* f16Add2Mul1 * f16Add2Mul2 */
                        "qadd %2, %2, %1 \n"                         /* Addition with saturation */
                        "qadd %0, %2, %2 \n"                         /* Addition with saturation */
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)            /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "=l"(f32Out), "+l"(f16Add1Mul1), "+l"(f16Add1Mul2): "l"(f16Add2Mul1), "l"(f16Add2Mul2));
    #endif

    return f32Out;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4_F32_ASM_H_ */
