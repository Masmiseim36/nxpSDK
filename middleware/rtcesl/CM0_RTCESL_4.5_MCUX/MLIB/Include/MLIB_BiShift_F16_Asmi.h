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
#ifndef _MLIB_BISHIFT_F16_ASM_H_
#define _MLIB_BISHIFT_F16_ASM_H_

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
#define MLIB_ShLBiSat_F16_Asmi(f16Val, i16Sh) MLIB_ShLBiSat_F16_FAsmi(f16Val, i16Sh)
#define MLIB_ShRBiSat_F16_Asmi(f16Val, i16Sh) MLIB_ShRBiSat_F16_FAsmi(f16Val, i16Sh)

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of i16Sh to the left. 
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
static inline frac16_t MLIB_ShLBiSat_F16_FAsmi(register frac16_t f16Val, register int16_t i16Sh)
{
    register frac16_t f16SatVal=0, f16CmpVal=0;
    #if defined(__CC_ARM)                                /* For ARM Compiler */
        __asm volatile{ sxth f16Val, f16Val              /* Converts 16-bit input to 32-bit*/
                        sxth i16Sh, i16Sh                /* Converts 16-bit input to 32-bit*/
                        rsbs f16CmpVal, i16Sh, #0        /* f16CmpVal = - i16Sh */
                        bmi LeftSh                       /* If i16Sh >= 0, then goes to LeftSh */
                        asrs f16Val, f16Val, f16CmpVal   /* f16Val = f16Val >> (-i16Sh) */
                        b ShEnd                          /* Goes to ShEnd */
                    LeftSh:
                        movs f16SatVal, #128             /* f16SatVal = 0x80 */
                        lsls f16SatVal, f16SatVal, #8    /* f16SatVal = 0x00008000 */
                        mov f16CmpVal, f16SatVal         /* f16CmpVal = 0x00008000 */
                        lsrs f16CmpVal, f16CmpVal, i16Sh /* f16CmpVal = f16CmpVal >> u16Sh */

                        cmp f16Val, f16CmpVal            /* Compares f16Val with (0x00008000 >> u16Sh) */
                        blt NegTest                      /* If f16Val < f16CmpVal, then goes to NegTest */
                        subs f16Val, f16SatVal, #1       /* f16Val = 0x7FFF */
                        b ShEnd
                    NegTest:
                        mvns f16CmpVal, f16SatVal        /* f16CmpVal = 0xFFFF7FFF */
                        asrs f16CmpVal, f16CmpVal, i16Sh /* f16CmpVal = f16CmpVal >> u16Sh */

                        cmp f16Val, f16CmpVal            /* Compares f16Val with (0xFFFF7FFF >> u16Sh) */
                        bgt NotSat                       /* If f16Val < f16CmpVal, then goes to NotSat */
                        sxth f16Val, f16SatVal           /* f16Val = 0xFFFF8000 */
                        b ShEnd
                    NotSat:
                        lsls f16Val, f16Val, i16Sh       /* f16CmpVal = f16CmpVal << u16Sh */
                    ShEnd: }
    #else
        __asm volatile(
                        #if defined(__GNUC__)            /* For GCC compiler */
                            ".syntax unified \n"         /* Using unified asm syntax */
                        #endif
                        "sxth %0, %0 \n"                 /* Converts 16-bit input to 32-bit*/
                        "sxth %1, %1 \n"                 /* Converts 16-bit input to 32-bit*/
                        "rsbs %2, %1, #0 \n"             /* f16CmpVal = - i16Sh */
                        "bmi .+6 \n"                     /* If i16Sh >= 0, then jumps through two commands */
                        "asrs %0, %0, %2 \n"             /* f16Val = f16Val >> (-i16Sh) */
                        "b .+32 \n"                      /* Goes to the end of function */

                        "movs %3, #128 \n"               /* f16SatVal = 0x80 */
                        "lsls %3, %3, #8 \n"             /* f16SatVal = 0x00008000 */
                        "mov %2, %3 \n"                  /* f16CmpVal = 0x00008000 */
                        "lsrs %2, %2, %1 \n"             /* f16CmpVal = f16CmpVal >> u16Sh */

                        "cmp %0, %2 \n"                  /* Compares f16Val with (0x00008000 >> u16Sh) */
                        "blt .+6 \n"                     /* If f16Val < f16CmpVal, then jumps through two commands */
                        "subs %0, %3, #1 \n"             /* f16Val = 0x7FFF */
                        "b .+16 \n"

                        "mvns %2, %3 \n"                 /* f16CmpVal = 0xFFFF7FFF */
                        "asrs %2, %2, %1 \n"             /* f16CmpVal = f16CmpVal >> u16Sh */

                        "cmp %0, %2 \n"                  /* Compares f16Val with (0xFFFF7FFF >> u16Sh) */
                        "bgt .+6 \n"                     /* If f16Val < f16CmpVal, then jumps through two commands */
                        "sxth %0, %3 \n"                 /* f16Val = 0xFFFF8000 */
                        "b .+4 \n"
                        "lsls %0, %0, %1 \n"             /* f16CmpVal = f16CmpVal << u16Sh */

                        #if defined(__GNUC__)            /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Val), "+l"(i16Sh), "+l"(f16CmpVal), "+l"(f16SatVal):);
    #endif

    return f16Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of i16Sh to the right. 
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
static inline frac16_t MLIB_ShRBiSat_F16_FAsmi(register frac16_t f16Val, register int16_t i16Sh)
{
    register frac16_t f16SatVal=0, f16LSh=0;
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ sxth f16Val, f16Val             /* Converts 16-bit input to 32-bit*/
                        sxth i16Sh, i16Sh               /* Converts 16-bit input to 32-bit*/
                        rsbs f16LSh, i16Sh, #0          /* f16LSh = - i16Sh */
                        bpl LeftSh                      /* If i16Sh < 0, then goes to LeftSh */
                        asrs f16Val, f16Val, i16Sh      /* f16Val = f16Val >> (-i16Sh) */
                        b ShEnd                         /* Goes to ShEnd */
                    LeftSh:
                        movs f16SatVal, #128            /* f16SatVal = 0x80 */
                        lsls f16SatVal, f16SatVal, #8   /* f16SatVal = 0x00008000 */
                        mov i16Sh, f16SatVal            /* i16Sh = 0x00008000 */
                        lsrs i16Sh, i16Sh, f16LSh       /* i16Sh = i16Sh >> f16LSh */

                        cmp f16Val, i16Sh               /* Compares f16Val with (0x00008000 >> f16LSh) */
                        blt NegTest                     /* If f16Val < i16Sh, then goes to NegTest */
                        subs f16Val, f16SatVal, #1      /* f16Val = 0x7FFF */
                        b ShEnd
                    NegTest:
                        mvns i16Sh, f16SatVal           /* i16Sh = 0xFFFF7FFF */
                        asrs i16Sh, i16Sh, f16LSh       /* i16Sh = i16Sh >> f16LSh */

                        cmp f16Val, i16Sh               /* Compares f16Val with (0xFFFF7FFF >> f16LSh) */
                        bgt NotSat                      /* If f16Val < i16Sh, then goes to NotSat */
                        sxth f16Val, f16SatVal          /* f16Val = 0xFFFF8000 */
                        b ShEnd
                    NotSat:
                        lsls f16Val, f16Val, f16LSh     /* f16Val = f16Val << f16LSh */
                    ShEnd: }
    #else
        __asm volatile(
                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax unified \n"        /* Using unified asm syntax */
                        #endif
                        "sxth %0, %0 \n"                /* Converts 16-bit input to 32-bit*/
                        "sxth %1, %1 \n"                /* Converts 16-bit input to 32-bit*/
                        "rsbs %2, %1, #0 \n"            /* f16LSh = - i16Sh */
                        "bpl .+6 \n"                    /* If i16Sh < 0, then jumps through three commands */
                        "asrs %0, %0, %1 \n"            /* f16Val = f16Val >> (-i16Sh) */
                        "b .+32 \n"                     /* Goes to the end of function */

                        "movs %3, #128 \n"              /* f16SatVal = 0x80 */
                        "lsls %3, %3, #8 \n"            /* f16SatVal = 0x00008000 */
                        "mov %1, %3 \n"                 /* i16Sh = 0x00008000 */
                        "lsrs %1, %1, %2 \n"            /* i16Sh = i16Sh >> f16LSh */

                        "cmp %0, %1 \n"                 /* Compares f16Val with (0x00008000 >> f16LSh) */
                        "blt .+6 \n"                    /* If f16Val < i16Sh, then jumps through two commands */
                        "subs %0, %3, #1 \n"            /* f16Val = 0x7FFF */
                        "b .+16 \n"

                        "mvns %1, %3 \n"                /* i16Sh = 0xFFFF7FFF */
                        "asrs %1, %1, %2 \n"            /* i16Sh = i16Sh >> f16LSh */

                        "cmp %0, %1 \n"                 /* Compares f16Val with (0xFFFF7FFF >> f16LSh) */
                        "bgt .+6 \n"                    /* If f16Val < i16Sh, then jumps through two commands */
                        "sxth %0, %3 \n"                /* f16Val = 0xFFFF8000 */
                        "b .+4 \n"
                        "lsls %0, %0, %2 \n"            /* f16Val = f16Val << f16LSh */

                        #if defined(__GNUC__)           /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Val), "+l"(i16Sh), "+l"(f16LSh), "+l"(f16SatVal):);
    #endif

    return f16Val;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_BISHIFT_F16_ASM_H_ */
