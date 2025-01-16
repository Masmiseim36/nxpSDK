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
* @brief  Multiply accumulate
* 
*******************************************************************************/
#ifndef _MLIB_MAC_F32_ASM_H_
#define _MLIB_MAC_F32_ASM_H_

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
#define MLIB_Mac_F32_Asmi(f32Accum, f32Mult1, f32Mult2)       MLIB_Mac_F32_FAsmi(f32Accum, f32Mult1, f32Mult2)
#define MLIB_Mac_F32lss_Asmi(f32Accum, f16Mult1, f16Mult2)    MLIB_Mac_F32lss_FAsmi(f32Accum, f16Mult1, f16Mult2)
#define MLIB_MacSat_F32_Asmi(f32Accum, f32Mult1, f32Mult2)    MLIB_MacSat_F32_FAsmi(f32Accum, f32Mult1, f32Mult2)
#define MLIB_MacSat_F32lss_Asmi(f32Accum, f16Mult1, f16Mult2) MLIB_MacSat_F32lss_FAsmi(f32Accum, f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = f32Accum + ( f32Mult1 * f32Mult2)
* Without saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_Mac_F32_FAsmi(register frac32_t f32Accum, 
                                          register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult2, f32Mult1 /* R1:R2 = Mul1 * Mul2 */
                        lsr f32Mult2, f32Mult2, #31                  /* R2 >> 31 */
                        orr f32Mult1, f32Mult2, f32Mult1, lsl #1     /* (R1 << 1) | R2*/
                        add f32Accum, f32Accum, f32Mult1 };          /* Acc + Mul1 * Mul2 */
    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                            ".syntax unified \n"           /* Using unified asm syntax */
                        #endif
                        "smull %2, %1, %2, %1 \n"                    /* R1:R2 = Mul1 * Mul2 */
                        "lsr %2, %2, #31 \n"                         /* R2 >> 31 */
                        "orr %1, %2, %1, lsl #1 \n"                  /* (R1 << 1) | R2*/
                        "add %0, %0, %1 \n"                          /* Acc + Mul1 * Mul2 */
                        : "+l"(f32Accum), "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = f32Accum + ( f16Mult1 * f16Mult2)
* Without saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_Mac_F32lss_FAsmi(register frac32_t f32Accum, 
                                             register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    #if defined(__CC_ARM)                                           /* For ARM Compiler */
        __asm volatile{ smulbb f16Mult1, f16Mult1, f16Mult2         /* f16Mult1 * f16Mult2 */
                        add f32Accum, f32Accum, f16Mult1, lsl #1 }; /* Addition without saturation */
    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                            ".syntax unified \n"           /* Using unified asm syntax */
                        #endif
                        "smulbb %1, %1, %2 \n"                      /* f16Mult1 * f16Mult2 */
                        "add %0, %0, %1, lsl #1 \n"                 /* Addition without saturation */
                        : "+l"(f32Accum), "+l"(f16Mult1): "l"(f16Mult2));
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = f32Accum + ( f32Mult1 * f32Mult2)
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_MacSat_F32_FAsmi(register frac32_t f32Accum, 
                                             register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult2, f32Mult1 /* R1:R2 = Mul1 * Mul2 */
                        lsr f32Mult2, f32Mult2, #31                  /* R2 >> 31 */
                        orr f32Mult1, f32Mult2, f32Mult1, lsl #1     /* (R1 << 1) | R2 */
                        rsb f32Mult1, f32Mult1, #0                   /* Negation for case of multiplication (-1)*(-1) */
                        qsub f32Accum, f32Accum, f32Mult1 };         /* Acc - (- Mul1 * Mul2) */
    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)  /* For GCC compiler */
                            ".syntax unified \n"           /* Using unified asm syntax */
                        #endif
                        "smull %2, %1, %2, %1 \n"                    /* R1:R2 = Mul1 * Mul2 */
                        "lsr %2, %2, #31 \n"                         /* R2 >> 31 */
                        "orr %1, %2, %1, lsl #1 \n"                  /* (R1 << 1) | R2 */
                        "rsb %1, %1, #0 \n"                          /* Negation for case of multiplication (-1)*(-1) */
                        "qsub %0, %0, %1 \n"                         /* Acc - (- Mul1 * Mul2) */
                        : "+l"(f32Accum), "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* f32Out = f32Accum + ( f16Mult1 * f16Mult2)
* With saturation
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_MacSat_F32lss_FAsmi(register frac32_t f32Accum, 
                                                register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    #if defined(__CC_ARM)                                     /* For ARM Compiler */
        __asm volatile{ smulbb f16Mult1, f16Mult1, f16Mult2   /* f16Mult1 * f16Mult2 */
                        qdadd f32Accum, f32Accum, f16Mult1 }; /* Addition with saturation */
    #else
        __asm volatile( 
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)     /* For GCC compiler */
                            ".syntax unified \n"              /* Using unified asm syntax */
                        #endif
                        "smulbb %1, %1, %2 \n"                /* f16Mult1 * f16Mult2 */
                        "qdadd %0, %0, %1 \n"                 /* Addition with saturation */
                        #if defined(__GNUC__) && !defined(__ARMCC_VERSION)     /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Accum), "+l"(f16Mult1): "l"(f16Mult2));
    #endif

    return f32Accum;
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MAC_F32_ASM_H_ */
