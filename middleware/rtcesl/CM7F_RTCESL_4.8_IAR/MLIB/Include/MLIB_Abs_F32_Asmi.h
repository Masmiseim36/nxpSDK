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
#ifndef _MLIB_ABS_F32_ASM_H_
#define _MLIB_ABS_F32_ASM_H_

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
#define MLIB_AbsSat_F32_Asmi(f32Val) MLIB_AbsSat_F32_FAsmi(f32Val)
  
/***************************************************************************//*!
*  Absolute value saturated
*     f32Out = |f32In|
*******************************************************************************/  
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_AbsSat_F32_FAsmi(register frac32_t f32Val)
{
      register frac32_t f32Temp = 1;
      #if defined(__CC_ARM)                                     /* For ARM Compiler */
              __asm volatile{ cmp f32Val, #0                    /* Compares f32Val with 0 */
                              itt lt                            /* If f32Val < 0, then executes next two commands */
                              mvnlt f32Val, f32Val              /* f32Val = ~f32Val */
                              qaddlt f32Val, f32Val, f32Temp }; /* f32Val = f32Val + 1 */
      #else
              __asm volatile( 
                              #if defined(__GNUC__) && !defined(__ARMCC_VERSION) /* For GCC compiler */
                                  ".syntax unified \n"          /* Using unified asm syntax */
                              #endif
                              "cmp %0, #0 \n"                   /* Compares f32Val with 0 */
                              "itt lt \n"                       /* If f32Val < 0, then executes next two commands */
                              "mvnlt %0, %0 \n"                 /* f32Val = ~f32Val */
                              "qaddlt %0, %0, %1 \n"            /* f32Val = f32Val + 1 */
                              #if defined(__GNUC__) && !defined(__ARMCC_VERSION) /* For GCC compiler */
                              ".syntax divided \n"
                              #endif
                              : "+l"(f32Val): "l"(f32Temp));
      #endif

      return f32Val;
}
RTCESL_INLINE_OPTIM_RESTORE 


#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ABS_F32_ASM_H_*/
