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
* @brief  Absolute Value
* 
*******************************************************************************/
#ifndef _MLIB_ABS_F16_ASM_H_
#define _MLIB_ABS_F16_ASM_H_

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
#define MLIB_AbsSat_F16_Asmi(f16Val) MLIB_AbsSat_F16_FAsmi(f16Val)

/***************************************************************************//*!
*  Absolute value saturated
*     f16Out = |f16In|
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac16_t MLIB_AbsSat_F16_FAsmi(register frac16_t f16Val)
{
    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ lsls f16Val, f16Val, #16             /* f16Val << 16 */
                        it mi                                /* If f16Val < 0, then implements next command */
                        rsbmi f16Val, f16Val, #0             /* If f16Val < 0, then f16Val = 0 - f16Val */
                        lsr f16Val, f16Val, #16              /* f16Val >> 16 */
                        usat f16Val, #15, f16Val };          /* Saturates the result */
    #else
        __asm volatile(
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)    /* For GCC compiler */
                            ".syntax unified \n"             /* Using unified asm syntax */
                        #endif
                        "lsls %0, %0, #16 \n"                /* f16Val << 16 */
                        "it mi \n"                           /* If f16Val < 0, then implements next command */
                        "rsbmi %0, %0, #0 \n"                /* If f16Val < 0, then f16Val = 0 - f16Val */
                        "lsr %0, %0, #16 \n"                 /* f16Val >> 16 */
                        "usat %0, #15, %0 \n"                /* Saturates the result */
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)    /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Val):);
    #endif

    return f16Val;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /*_MLIB_ABS_F16_ASM_H_*/
