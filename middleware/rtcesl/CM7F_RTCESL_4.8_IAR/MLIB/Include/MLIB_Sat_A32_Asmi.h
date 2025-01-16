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
* @brief  Saturation
* 
*******************************************************************************/
#ifndef _MLIB_SAT_A32_ASM_H_
#define _MLIB_SAT_A32_ASM_H_

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
#define MLIB_Sat_F16a_Asmi(a32Accum) MLIB_Sat_F16a_FAsmi(a32Accum)
  
/***************************************************************************//*!
*
* Takes the fractional part from number in accumulator format and saturate if the original number is greater than 1. 
* 
*******************************************************************************/ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac16_t MLIB_Sat_F16a_FAsmi(register acc32_t a32Accum)
{
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ ssat a32Accum, #16, a32Accum }; /* Saturation */

    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)         /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "ssat %0, #16, %0 \n"           /* Saturation */
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)         /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Accum):);
    #endif

    return (frac16_t)a32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SAT_A32_ASM_H_ */
