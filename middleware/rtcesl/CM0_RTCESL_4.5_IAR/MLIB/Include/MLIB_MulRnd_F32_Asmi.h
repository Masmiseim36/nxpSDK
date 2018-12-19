/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
*
****************************************************************************//*!
*
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_F32_ASM_H_
#define _MLIB_MULRND_F32_ASM_H_

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
#define MLIB_MulRnd_F32_Asmi(f32Mult1, f32Mult2)    MLIB_MulRnd_F32_FAsmi(f32Mult1, f32Mult2)
#define MLIB_MulRndSat_F32_Asmi(f32Mult1, f32Mult2) MLIB_MulRndSat_F32_FAsmi(f32Mult1, f32Mult2)
#define MLIB_MulNegRnd_F32_Asmi(f32Mult1, f32Mult2) MLIB_MulNegRnd_F32_FAsmi(f32Mult1, f32Mult2)

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* Without saturation
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_MulRnd_F32_FAsmi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ uxth f32Val1, f32Mult1              /* f32Val1 = f32Mult1.L */
                        uxth f32Val2, f32Mult2              /* f32Val2 = f32Mult2.L */

                        asrs f32Mult1, f32Mult1, #16        /* f32Mult1 = f32Mult1.H */
                        asrs f32Mult2, f32Mult2, #16        /* f32Mult2 = f32Mult2.H */

                        movs f32Val3, f32Val1               /* f32Val3 = f32Mult1.L */
                        muls f32Val3, f32Val3, f32Val2      /* f32Val3 = f32Mult1.L * f32Mult2.L */
                        lsrs f32Val3, f32Val3, #16          /* f32Val3 >> 16 */

                        muls f32Val1, f32Val1, f32Mult2     /* f32Val1 = f32Mult1.L * f32Mult2.H */
                        adds f32Val1, f32Val1, f32Val3      /* f32Val1 = f32Val1 + f32Val3 */
                        asrs f32Val1, f32Val1, #7           /* f32Val1 >> 7 */

                        muls f32Val2, f32Val2, f32Mult1     /* f32Val2 = f32Mult2.L * f32Mult1.H */
                        asrs f32Val2, f32Val2, #7           /* f32Val2 >> 7 */
                        adds f32Val2, f32Val2, f32Val1      /* f32Val2 = f32Val2 + f32Val1 */
                        adds f32Val2, f32Val2, #128         /* Rounding */
                        asrs f32Val2, f32Val2, #8           /* f32Val2 >> 8 */

                        muls f32Mult1, f32Mult1, f32Mult2   /* f32Mult1 = f32Mult1.H * f32Mult2.H */
                        lsls f32Mult1, f32Mult1, #1         /* f32Mult1 << 1 */
                        adds f32Mult1, f32Mult1, f32Val2 }; /* f32Mult1 = f32Mult1 + f32Val2 */
    #else
        __asm volatile(
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "uxth %2, %0 \n"                    /* f32Val1 = f32Mult1.L */
                        "uxth %3, %1 \n"                    /* f32Val2 = f32Mult2.L */

                        "asrs %0, %0, #16 \n"               /* f32Mult1 = f32Mult1.H */
                        "asrs %1, %1, #16 \n"               /* f32Mult2 = f32Mult2.H */

                        "movs %4, %2 \n"                    /* f32Val3 = f32Mult1.L */
                        "muls %4, %4, %3 \n"                /* f32Val3 = f32Mult1.L * f32Mult2.L */
                        "lsrs %4, %4, #16 \n"               /* f32Val3 >> 16 */

                        "muls %2, %2, %1 \n"                /* f32Val1 = f32Mult1.L * f32Mult2.H */
                        "adds %2, %2, %4 \n"                /* f32Val1 = f32Val1 + f32Val3 */
                        "asrs %2, %2, #7 \n"                /* f32Val1 >> 7 */

                        "muls %3, %3, %0 \n"                /* f32Val2 = f32Mult2.L * f32Mult1.H */
                        "asrs %3, %3, #7 \n"                /* f32Val2 >> 7 */
                        "adds %3, %3, %2 \n"                /* f32Val2 = f32Val2 + f32Val1 */
                        "adds %3, %3, #128 \n"              /* Rounding */
                        "asrs %3, %3, #8 \n"                /* f32Val2 >> 8 */

                        "muls %0, %0, %1 \n"                /* f32Mult1 = f32Mult1.H * f32Mult2.H */
                        "lsls %0, %0, #1 \n"                /* f32Mult1 << 1 */
                        "adds %0, %0, %3 \n"                /* f32Mult1 = f32Mult1 + f32Val2 */
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Mult1), "+l"(f32Mult2), "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3):);
    #endif

    return f32Mult1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* With saturation
*******************************************************************************/  
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_MulRndSat_F32_FAsmi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ uxth f32Val1, f32Mult1              /* f32Val1 = f32Mult1.L */
                        uxth f32Val2, f32Mult2              /* f32Val2 = f32Mult2.L */

                        asrs f32Mult1, f32Mult1, #16        /* f32Mult1 = f32Mult1.H */
                        asrs f32Mult2, f32Mult2, #16        /* f32Mult2 = f32Mult2.H */

                        movs f32Val3, f32Val1               /* f32Val3 = f32Mult1.L */
                        muls f32Val3, f32Val3, f32Val2      /* f32Val3 = f32Mult1.L * f32Mult2.L */
                        lsrs f32Val3, f32Val3, #16          /* f32Val3 >> 16 */

                        muls f32Val1, f32Val1, f32Mult2     /* f32Val1 = f32Mult1.L * f32Mult2.H */
                        adds f32Val1, f32Val1, f32Val3      /* f32Val1 = f32Val1 + f32Val3 */
                        asrs f32Val1, f32Val1, #7           /* f32Val1 >> 7 */

                        muls f32Val2, f32Val2, f32Mult1     /* f32Val2 = f32Mult2.L * f32Mult1.H */
                        asrs f32Val2, f32Val2, #7           /* f32Val2 >> 7 */
                        adds f32Val2, f32Val2, f32Val1      /* f32Val2 = f32Val2 + f32Val1 */
                        adds f32Val2, f32Val2, #128         /* Rounding */
                        asrs f32Val2, f32Val2, #8           /* f32Val2 >> 8 */

                        muls f32Mult1, f32Mult1, f32Mult2   /* f32Mult1 = f32Mult1.H * f32Mult2.H */
                        lsls f32Mult1, f32Mult1, #1         /* f32Mult1 << 1 */
                        adds f32Mult1, f32Mult1, f32Val2    /* f32Mult1 = f32Mult1 + f32Val2 */

                        rev f32Mult2, f32Mult1              /* Byte-reverse of result */
                        cmp f32Mult2, #128                  /* Compares with 0x80 */
                        bne NotSat                          /* If result <> 0x80000000, goes to NotSat */
                        mvns f32Mult1, f32Mult1             /* If result = 0x80000000, then return 0x7FFFFFFF */
                      NotSat: };
    #else
        __asm volatile(
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "uxth %2, %0 \n"                    /* f32Val1 = f32Mult1.L */
                        "uxth %3, %1 \n"                    /* f32Val2 = f32Mult2.L */

                        "asrs %0, %0, #16 \n"               /* f32Mult1 = f32Mult1.H */
                        "asrs %1, %1, #16 \n"               /* f32Mult2 = f32Mult2.H */

                        "movs %4, %2 \n"                    /* f32Val3 = f32Mult1.L */
                        "muls %4, %4, %3 \n"                /* f32Val3 = f32Mult1.L * f32Mult2.L */
                        "lsrs %4, %4, #16 \n"               /* f32Val3 >> 16 */

                        "muls %2, %2, %1 \n"                /* f32Val1 = f32Mult1.L * f32Mult2.H */
                        "adds %2, %2, %4 \n"                /* f32Val1 = f32Val1 + f32Val3 */
                        "asrs %2, %2, #7 \n"                /* f32Val1 >> 7 */

                        "muls %3, %3, %0 \n"                /* f32Val2 = f32Mult2.L * f32Mult1.H */
                        "asrs %3, %3, #7 \n"                /* f32Val2 >> 7 */
                        "adds %3, %3, %2 \n"                /* f32Val2 = f32Val2 + f32Val1 */
                        "adds %3, %3, #128 \n"              /* Rounding */
                        "asrs %3, %3, #8 \n"                /* f32Val2 >> 8 */

                        "muls %0, %0, %1 \n"                /* f32Mult1 = f32Mult1.H * f32Mult2.H */
                        "lsls %0, %0, #1 \n"                /* f32Mult1 << 1 */
                        "adds %0, %0, %3 \n"                /* f32Mult1 = f32Mult1 + f32Val2 */

                        "rev %1, %0 \n"                     /* Byte-reverse of result */
                        "cmp %1, #128 \n"                   /* Compares with 0x80 */
                        "bne .+4 \n"                        /* If result <> 0x80000000, jumps through next command */
                        "mvns %0, %0 \n"                    /* If result = 0x80000000, then return 0x7FFFFFFF */
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Mult1), "+l"(f32Mult2), "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3):);
    #endif

    return f32Mult1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* f32Out = f32Mult1 * -f32Mult2
* 
*******************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_MulNegRnd_F32_FAsmi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    register frac32_t f32Val1=0, f32Val2=0, f32Val3=0;
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ uxth f32Val1, f32Mult1              /* f32Val1 = f32Mult1.L */
                        uxth f32Val2, f32Mult2              /* f32Val2 = f32Mult2.L */

                        asrs f32Mult1, f32Mult1, #16        /* f32Mult1 = f32Mult1.H */
                        asrs f32Mult2, f32Mult2, #16        /* f32Mult2 = f32Mult2.H */

                        movs f32Val3, f32Val1               /* f32Val3 = f32Mult1.L */
                        muls f32Val3, f32Val3, f32Val2      /* f32Val3 = f32Mult1.L * f32Mult2.L */
                        lsrs f32Val3, f32Val3, #16          /* f32Val3 >> 16 */

                        muls f32Val1, f32Val1, f32Mult2     /* f32Val1 = f32Mult1.L * f32Mult2.H */
                        adds f32Val1, f32Val1, f32Val3      /* f32Val1 = f32Val1 + f32Val3 */
                        asrs f32Val1, f32Val1, #7           /* f32Val1 >> 7 */

                        muls f32Val2, f32Val2, f32Mult1     /* f32Val2 = f32Mult2.L * f32Mult1.H */
                        asrs f32Val2, f32Val2, #7           /* f32Val2 >> 7 */
                        adds f32Val2, f32Val2, f32Val1      /* f32Val2 = f32Val2 + f32Val1 */
                        adds f32Val2, f32Val2, #128         /* Rounding */
                        asrs f32Val2, f32Val2, #8           /* f32Val2 >> 8 */

                        muls f32Mult1, f32Mult1, f32Mult2   /* f32Mult1 = f32Mult1.H * f32Mult2.H */
                        lsls f32Mult1, f32Mult1, #1         /* f32Mult1 << 1 */
                        adds f32Mult1, f32Mult1, f32Val2    /* f32Mult1 = f32Mult1 + f32Val2 */
                        rsbs f32Mult1, f32Mult1, #0 };      /* Negation */
    #else
        __asm volatile(
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "uxth %2, %0 \n"                    /* f32Val1 = f32Mult1.L */
                        "uxth %3, %1 \n"                    /* f32Val2 = f32Mult2.L */

                        "asrs %0, %0, #16 \n"               /* f32Mult1 = f32Mult1.H */
                        "asrs %1, %1, #16 \n"               /* f32Mult2 = f32Mult2.H */

                        "movs %4, %2 \n"                    /* f32Val3 = f32Mult1.L */
                        "muls %4, %4, %3 \n"                /* f32Val3 = f32Mult1.L * f32Mult2.L */
                        "lsrs %4, %4, #16 \n"               /* f32Val3 >> 16 */

                        "muls %2, %2, %1 \n"                /* f32Val1 = f32Mult1.L * f32Mult2.H */
                        "adds %2, %2, %4 \n"                /* f32Val1 = f32Val1 + f32Val3 */
                        "asrs %2, %2, #7 \n"                /* f32Val1 >> 7 */

                        "muls %3, %3, %0 \n"                /* f32Val2 = f32Mult2.L * f32Mult1.H */
                        "asrs %3, %3, #7 \n"                /* f32Val2 >> 7 */
                        "adds %3, %3, %2 \n"                /* f32Val2 = f32Val2 + f32Val1 */
                        "adds %3, %3, #128 \n"              /* Rounding */
                        "asrs %3, %3, #8 \n"                /* f32Val2 >> 8 */

                        "muls %0, %0, %1 \n"                /* f32Mult1 = f32Mult1.H * f32Mult2.H */
                        "lsls %0, %0, #1 \n"                /* f32Mult1 << 1 */
                        "adds %0, %0, %3 \n"                /* f32Mult1 = f32Mult1 + f32Val2 */
                        "rsbs %0, %0, #0 \n"                /* Negation */
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Mult1), "+l"(f32Mult2), "+l"(f32Val1), "+l"(f32Val2), "+l"(f32Val3):);
    #endif

    return f32Mult1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_F32_ASM_H_ */
