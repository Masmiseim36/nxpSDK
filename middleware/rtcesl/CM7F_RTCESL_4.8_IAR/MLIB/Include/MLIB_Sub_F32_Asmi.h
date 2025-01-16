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
* f32Out = f32Min - f32Sub
* With saturation
*******************************************************************************/ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_SubSat_F32_FAsmi(register frac32_t f32Min, register frac32_t f32Sub)
{
#if defined(__CC_ARM)                                 /* For ARM Compiler */
    __asm volatile{ qsub f32Min, f32Min, f32Sub};     /* Subtracts with saturation */
#else
    __asm volatile(
                   #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                       ".syntax unified \n"           /* Using unified asm syntax */
                   #endif
    	           "qsub %0, %0, %1 \n"               /* Subtracts with saturation */
                   #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                       ".syntax divided \n"
                   #endif
    	    	: "+l"(f32Min): "l"(f32Sub));
#endif

    return f32Min;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_F32_ASM_H_ */
