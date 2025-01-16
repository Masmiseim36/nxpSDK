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
* @brief  Addition
* 
*******************************************************************************/
#ifndef _MLIB_ADD_F32_ASM_H_
#define _MLIB_ADD_F32_ASM_H_

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
#define MLIB_AddSat_F32_Asmi(f32Add1, f32Add2) MLIB_AddSat_F32_FAsmi(f32Add1, f32Add2)

/***************************************************************************//*!
*  Saturated Addition
*     f32Out = f32Add1 + f32Add2
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_AddSat_F32_FAsmi(register frac32_t f32Add1, register frac32_t f32Add2)
{
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ qadd f32Add1, f32Add1, f32Add2};    /* Adds with saturation */
    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                            ".syntax unified \n"           /* Using unified asm syntax */
                        #endif
                        "qadd %0, %0, %1 \n"               /* Adds with saturation */
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Add1): "l"(f32Add2));
    #endif

    return f32Add1;
}
RTCESL_INLINE_OPTIM_RESTORE 


#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD_F32_ASM_H_ */
