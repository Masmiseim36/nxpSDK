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
* @brief  Summation of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_ADD4_F32_ASM_H_
#define _MLIB_ADD4_F32_ASM_H_

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
#define MLIB_Add4_F32_Asmi(f32Add1, f32Add2, f32Add3, f32Add4)                 \
        MLIB_Add4_F32_FAsmi(f32Add1, f32Add2, f32Add3, f32Add4)
#define MLIB_Add4Sat_F32_Asmi(f32Add1, f32Add2, f32Add3, f32Add4)              \
        MLIB_Add4Sat_F32_FAsmi(f32Add1, f32Add2, f32Add3, f32Add4)
  
/***************************************************************************//*!
*  Summation of four inputs
*     f32Out = f32Add1 + f32Add2 + f32Add3 + f32Add4
*  function is not saturated
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Add4_F32_FAsmi(register frac32_t f32Add1, register frac32_t f32Add2, 
                                           register frac32_t f32Add3, register frac32_t f32Add4)
{
    #if defined(__CC_ARM)                              /* For ARM Compiler */
        __asm volatile{ add f32Add1, f32Add1, f32Add2  /* f32Add1 = f32Add1 + f32Add2 */
                        add f32Add1, f32Add1, f32Add3  /* f32Add1 = f32Add1 + f32Add3 */
                        add f32Add1, f32Add1, f32Add4};/* f32Add1 = f32Add1 + f32Add4 */

    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                            ".syntax unified \n"           /* Using unified asm syntax */
                        #endif
                        "add %0, %0, %1 \n"            /* f32Add1 = f32Add1 + f32Add2 */
                        "add %0, %0, %2 \n"            /* f32Add1 = f32Add1 + f32Add3 */
                        "add %0, %0, %3 \n"            /* f32Add1 = f32Add1 + f32Add4 */
                        : "+l"(f32Add1): "l"(f32Add2), "l"(f32Add3), "l"(f32Add4));
    #endif

    return f32Add1;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*  Saturated summation of four inputs
*     f32Out = f32Add1 + f32Add2 + f32Add3 + f32Add4
*******************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_Add4Sat_F32_FAsmi(register frac32_t f32Add1, register frac32_t f32Add2, 
                                              register frac32_t f32Add3, register frac32_t f32Add4)
{
    register frac32_t f32Sign1=0, f32Sign2=0;
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ asr f32Sign1, f32Add1, #31          /* f32Sign1 = sign of f32Add1 */
                        asr f32Sign2, f32Add2, #31          /* f32Sign2 = sign of f32Add2 */
                        adds f32Add1, f32Add1, f32Add2      /* f32Add1 + f32Add2 */
                        adc f32Sign1, f32Sign1, f32Sign2    /* f32Sign1 + f32Sign2 + Carry */

                        asr f32Sign2, f32Add3, #31          /* f32Sign2 = sign of f32Add3 */
                        adds f32Add1, f32Add1, f32Add3      /* f32Add1 + f32Add2 + f32Add3 */
                        adc f32Sign1, f32Sign1, f32Sign2    /* f32Sign1 + f32Sign2 + Carry */

                        asr f32Sign2, f32Add4, #31          /* f32Sign2 = sign of f32Add4 */
                        adds f32Add1, f32Add1, f32Add4      /* f32Add1 + f32Add2 + f32Add3 + f32Add4 */
                        adc f32Sign1, f32Sign1, f32Sign2    /* f32Sign1 + f32Sign2 + Carry */

                        mov f32Sign2, #0x80000000           /* f32Sign2 = 0x80000000 */

                        lsr f32Add2, f32Add1, #31           /* First bit of low part of result */
                        adds f32Sign1, f32Sign1, f32Add2

                        it gt                               /* If f32Sign1 > 0, then executes next command */
                        subgt f32Add1, f32Sign2, #1         /* Result = 0x7FFFFFFF*/

                        it lt                               /* If f32Sign1 > 0, then executes next command */
                        movlt f32Add1, f32Sign2  };         /* Result = 0x80000000*/
    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)   /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "asr %0, %2, #31 \n"                /* f32Sign1 = sign of f32Add1 */
                        "asr %1, %3, #31 \n"                /* f32Sign2 = sign of f32Add2 */
                        "adds %2, %2, %3 \n"                /* f32Add1 + f32Add2 */
                        "adc %0, %0, %1 \n"                 /* f32Sign1 + f32Sign2 + Carry */

                        "asr %1, %4, #31 \n"                /* f32Sign2 = sign of f32Add3 */
                        "adds %2, %2, %4 \n"                /* f32Add1 + f32Add2 + f32Add3 */
                        "adc %0, %0, %1 \n"                 /* f32Sign1 + f32Sign2 + Carry */

                        "asr %1, %5, #31 \n"                /* f32Sign2 = sign of f32Add4 */
                        "adds %2, %2, %5 \n"                /* f32Add1 + f32Add2 + f32Add3 + f32Add4 */
                        "adc %0, %0, %1 \n"                 /* f32Sign1 + f32Sign2 + Carry */

                        "mov %1, #0x80000000 \n"            /* f32Sign2 = 0x80000000 */
                        "lsr %3, %2, #31 \n"                /* First bit of low part of result */
                        "adds %0, %0, %3 \n"

                        "it gt \n"                          /* If f32Sign1 > 0, then executes next command */
                        "subgt %2, %1, #1 \n"               /* Result = 0x7FFFFFFF*/

                        "it lt \n"                          /* If f32Sign1 > 0, then executes next command */
                        "movlt %2, %1 \n"                   /* Result = 0x80000000*/
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)   /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Sign1), "+l"(f32Sign2), "+l"(f32Add1), "+l"(f32Add2): "l"(f32Add3), "l"(f32Add4) );
    #endif

    return f32Add1;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_ADD4_F32_ASM_H_ */
