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
* @brief  Shift
* 
*******************************************************************************/
#ifndef _MLIB_BISHIFT_F32_ASM_H_
#define _MLIB_BISHIFT_F32_ASM_H_

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
#define MLIB_ShLBiSat_F32_Asmi(f32Val, i16Sh) MLIB_ShLBiSat_F32_FAsmi(f32Val, i16Sh)
#define MLIB_ShRBiSat_F32_Asmi(f32Val, i16Sh) MLIB_ShRBiSat_F32_FAsmi(f32Val, i16Sh)
  
/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of i16Sh to the left. 
* If the i16Sh is negative, the input is shifted to the right.
* The function saturates the output.
* 
*******************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_ShLBiSat_F32_Asmi(register frac32_t f32Val, register int16_t i16Sh)
{
    register frac32_t f32SatVal=0, f32CmpVal=0;
    #if defined(__CC_ARM)                                /* For ARM Compiler */
        __asm volatile{ sxth i16Sh, i16Sh                /* Converts 16-bit input to 32-bit*/
                        rsbs f32CmpVal, i16Sh, #0        /* f32CmpVal = - i16Sh */
                        bmi LeftSh                       /* If i16Sh >= 0, then goes to LeftSh */
                        asrs f32Val, f32Val, f32CmpVal   /* f32Val = f32Val >> (-i16Sh) */
                        b ShEnd                          /* Goes to ShEnd */
                    LeftSh:
                        movs f32SatVal, #128             /* f32SatVal = 0x80 */
                        rev f32SatVal, f32SatVal         /* f32SatVal = 0x80000000 */
                        subs f32CmpVal, f32SatVal, #1    /* f32CmpVal = 0x7FFFFFFF */
                        asrs f32CmpVal, f32CmpVal, i16Sh /* f32CmpVal = f32CmpVal >> u16Sh */

                        cmp f32Val, f32CmpVal            /* Compares f32Val with (0x7FFFFFFF >> u16Sh) */
                        ble NegTest                      /* If f32Val <= f32CmpVal, then goes to NegTest */
                        subs f32Val, f32SatVal, #1       /* f32Val = 0x7FFF */
                        b ShEnd
                    NegTest:
                        mov f32CmpVal, f32SatVal         /* f32CmpVal = 0x80000000 */
                        asrs f32CmpVal, f32CmpVal, i16Sh /* f32CmpVal = f32CmpVal >> u16Sh */

                        cmp f32Val, f32CmpVal            /* Compares f32Val with (0x80000000 >> u16Sh) */
                        bgt NotSat                       /* If f32Val < f32CmpVal, then goes to NotSat */
                            mov f32Val, f32SatVal        /* f32Val = 0x80000000 */
                        b ShEnd
                    NotSat:
                        lsls f32Val, f32Val, i16Sh       /* f32Val = f32Val << u16Sh */
                    ShEnd: }
    #else
        __asm volatile(
                        #if defined(__GNUC__)            /* For GCC compiler */
                            ".syntax unified \n"         /* Using unified asm syntax */
                        #endif
                        "sxth %1, %1 \n"                 /* Converts 16-bit input to 32-bit*/
                        "rsbs %2, %1, #0 \n"             /* f32CmpVal = - i16Sh */
                        "bmi .+6 \n"                     /* If i16Sh >= 0, then jumps through two commands */
                        "asrs %0, %0, %2 \n"             /* f32Val = f32Val >> (-i16Sh) */
                        "b .+32 \n"                      /* Goes to the end of function */

                        "movs %3, #128 \n"               /* f32SatVal = 0x80 */
                        "rev %3, %3 \n"                  /* f32SatVal = 0x80000000 */
                        "subs %2, %3, #1 \n"             /* f32CmpVal = 0x7FFFFFFF */
                        "asrs %2, %2, %1 \n"             /* f32CmpVal = f32CmpVal >> u16Sh */

                        "cmp %0, %2 \n"                  /* Compares f32Val with (0x7FFFFFFF >> u16Sh) */
                        "ble .+6 \n"                     /* If f32Val <= f32CmpVal, then jumps through two commands */
                        "subs %0, %3, #1 \n"             /* f32Val = 0x7FFF */
                        "b .+16 \n"

                        "mov %2, %3 \n"                  /* f32CmpVal = 0x80000000 */
                        "asrs %2, %2, %1 \n"             /* f32CmpVal = f32CmpVal >> u16Sh */

                        "cmp %0, %2 \n"                  /* Compares f32Val with (0x80000000 >> u16Sh) */
                        "bgt .+6 \n"                     /* If f32Val < f32CmpVal, then jumps through two commands */
                        "mov %0, %3 \n"                  /* f32Val = 0x80000000 */
                        "b .+4 \n"
                        "lsls %0, %0, %1 \n"             /* f32Val = f32Val << u16Sh */

                        #if defined(__GNUC__)            /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Val), "+l"(i16Sh), "+l"(f32CmpVal), "+l"(f32SatVal):);
    #endif

    return f32Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of i16Sh to the right. 
* If the i16Sh is negative, the input is shifted to the left.
* The function saturates the output.
* 
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_ShRBiSat_F32_FAsmi(register frac32_t f32Val, register int16_t i16Sh)
{
    register int16_t i16LSh=0;
    register frac32_t f32SatVal=0;
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ sxth i16Sh, i16Sh               /* Converts 16-bit input to 32-bit*/
                        rsbs i16LSh, i16Sh, #0          /* i16LSh = - i16Sh */
                        bpl LeftSh                      /* If i16Sh < 0, then goes to LeftSh */
                        asrs f32Val, f32Val, i16Sh      /* f32Val = f32Val >> (-i16Sh) */
                        b ShEnd                         /* Goes to ShEnd */
                    LeftSh:
                        movs f32SatVal, #128            /* f32SatVal = 0x80 */
                        rev f32SatVal, f32SatVal        /* f32SatVal = 0x80000000 */
                        subs i16Sh, f32SatVal, #1       /* i16Sh = 0x7FFFFFFF */
                        asrs i16Sh, i16Sh, i16LSh       /* i16Sh = 0x7FFFFFFF >> i16LSh */

                        cmp f32Val, i16Sh               /* Compares f32Val with (0x7FFFFFFF >> i16LSh) */
                        ble NegTest                     /* If f32Val <= i16Sh, then go to NegTest */
                        subs f32Val, f32SatVal, #1      /* f32Val = 0x7FFF */
                        b ShEnd
                    NegTest:
                        mov i16Sh, f32SatVal            /* i16Sh = 0x80000000 */
                        asrs i16Sh, i16Sh, i16LSh       /* i16Sh = 0x80000000 >> i16LSh */

                        cmp f32Val, i16Sh               /* Compares f32Val with (0x80000000 >> i16LSh) */
                        bgt NotSat                      /* If f32Val < i16Sh, then goes to NotSat */
                        mov f32Val, f32SatVal           /* f32Val = 0x80000000 */
                        b ShEnd
                    NotSat:
                        lsls f32Val, f32Val, i16LSh     /* f32Val = f32Val << i16LSh */
                    ShEnd: }
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "sxth %1, %1 \n"                /* Converts 16-bit input to 32-bit*/
                        "rsbs %2, %1, #0 \n"            /* i16LSh = - i16Sh */
                        "bpl .+6 \n"                    /* If i16Sh < 0, then jumps through three commands */
                        "asrs %0, %0, %1 \n"            /* f32Val = f32Val >> (-i16Sh) */
                        "b .+32 \n"                     /* Goes to the end of function */

                        "movs %3, #128 \n"              /* f32SatVal = 0x80 */
                        "rev %3, %3 \n"                 /* f32SatVal = 0x80000000 */
                        "subs %1, %3, #1 \n"            /* i16Sh = 0x7FFFFFFF */
                        "asrs %1, %1, %2 \n"            /* i16Sh = 0x7FFFFFFF >> i16LSh */

                        "cmp %0, %1 \n"                 /* Compares f32Val with (0x7FFFFFFF >> i16LSh) */
                        "ble .+6 \n"                    /* If f32Val <= i16Sh, then jumps through two commands */
                        "subs %0, %3, #1 \n"            /* f32Val = 0x7FFF */
                        "b .+16 \n"

                        "mov %1, %3 \n"                 /* i16Sh = 0x80000000 */
                        "asrs %1, %1, %2 \n"            /* i16Sh = 0x80000000 >> i16LSh */

                        "cmp %0, %1 \n"                 /* Compares f32Val with (0x80000000 >> i16LSh) */
                        "bgt .+6 \n"                    /* If f32Val < i16Sh, then jumps through two commands */
                        "mov %0, %3 \n"                 /* f32Val = 0x80000000 */
                        "b .+4 \n"
                        "lsls %0, %0, %2 \n"            /* f32Val = f32Val << i16LSh */
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f32Val), "+l"(i16Sh), "+l"(i16LSh), "+l"(f32SatVal):);
    #endif

    return f32Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_BISHIFT_F32_ASM_H_ */
