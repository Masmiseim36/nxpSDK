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
#ifndef _MLIB_SHIFT_F16_ASM_H_
#define _MLIB_SHIFT_F16_ASM_H_

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
#define MLIB_Sh1LSat_F16_Asmi(f16Val)       MLIB_Sh1LSat_F16_FAsmi(f16Val)
#define MLIB_ShLSat_F16_Asmi(f16Val, u16Sh) MLIB_ShLSat_F16_FAsmi(f16Val, u16Sh)

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by 1 to the left. 
* The function saturates the output.
*              
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_Sh1LSat_F16_FAsmi(register frac16_t f16Val)
{
    register frac16_t f16TestVal=0, f16Out=0;

    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ sxth f16Val, f16Val             /* Transforms 16-bit input f16Val to 32-bit */
                        mov f16TestVal, f16Val          /* f16TestVal = f16Val */
                        lsls f16Out, f16Val, #1         /* f16Out = f16Val << 1 */
                        sxth f16Out, f16Out             /* Transforms 16-bit value to 32-bit */
                        eors f16Val, f16Val, f16Out     /* f16Val = f16Val ^ f16Out */
                        bpl SatEnd                      /* If f16Val >= 0, then goes to SatEnd */
                        movs f16Out, #128               /* f16Out = 0x80 */
                        lsls f16Out, f16Out, #8         /* f16Out = 0x00008000 */
                        cmp f16TestVal, #0              /* Compares input value with 0 */
                        blt SatEnd                      /* If f16TestVal < 0, then goes to SatEnd */
                        subs f16Out, f16Out, #1         /* Else f16Out = 0x00007FFF*/
                    SatEnd: };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "sxth %1, %1 \n\t"                /* Transforms 16-bit input f16Val to 32-bit */
                        "mov %2, %1 \n\t"                 /* f16TestVal = f16Val */
                        "lsls %0, %1, #1 \n\t"            /* f16Out = f16Val << 1 */
                        "sxth %0, %0 \n\t"                /* Transforms 16-bit value to 32-bit */
                        "eors %1, %1, %0 \n\t"            /* f16Val = f16Val ^ f16Out */
                        "bpl MLIB_Sh1LSat_F16_SatEnd \n\t"/* If f16Val >= 0, then jumps to the end of function */
                        "movs %0, #128 \n\t"              /* f16Out = 0x80 */
                        "lsls %0, %0, #8 \n\t"            /* f16Out = 0x00008000 */
                        "cmp %2, #0 \n\t"                 /* Compares input value with 0 */
                        "blt MLIB_Sh1LSat_F16_SatEnd \n\t"/* If f16TestVal < 0, then jumps through next command */
                        "subs %0, %0, #1 \n\t"            /* Else f16Out = 0x00007FFF */
					"MLIB_Sh1LSat_F16_SatEnd: \n\t"
                        : "+l"(f16Out), "+l"(f16Val), "+l"(f16TestVal):);
    #endif

    return f16Out;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of u16Sh to the left. 
* The function saturates the output.
*             
*******************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_ShLSat_F16_FAsmi(register frac16_t f16Val, register uint16_t u16Sh)
{
    register frac16_t f16SatVal=0, f16CmpVal=0;

    #if defined(__CC_ARM)                                /* For ARM Compiler */
        __asm volatile{ sxth f16Val, f16Val              /* Transforms 16-bit input f16Val to 32-bit */
                        movs f16SatVal, #128             /* f16SatVal = 0x80 */
                        lsls f16SatVal, f16SatVal, #8    /* f16SatVal = 0x00008000 */
                        mov f16CmpVal, f16SatVal         /* f16CmpVal = 0x00008000 */
                        lsrs f16CmpVal, f16CmpVal, u16Sh /* f16CmpVal = f16CmpVal >> u16Sh */

                        cmp f16Val, f16CmpVal            /* Compares f16Val with (0x00008000 >> u16Sh) */
                        blt NegTest                      /* If f16Val <= f16CmpVal, then jumps through two commands */
                        subs f16Val, f16SatVal, #1       /* f16Val = 0x7FFF */
                        b SatEnd
                    NegTest:
                        mvns f16CmpVal, f16SatVal        /* f16CmpVal = 0xFFFF7FFF */
                        asrs f16CmpVal, f16CmpVal, u16Sh /* f16CmpVal = f16CmpVal >> u16Sh */

                        cmp f16Val, f16CmpVal            /* Compares f16Val with (0xFFFF7FFF >> u16Sh) */
                        bgt NotSat                       /* If f16Val < f16CmpVal, then jumps through two commands */
                        sxth f16Val, f16SatVal           /* f16Val = 0xFFFF8000 */
                        b SatEnd
                    NotSat:
                        lsls f16Val, f16Val, u16Sh       /* f16CmpVal = f16CmpVal << u16Sh */
                    SatEnd: }
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "sxth %0, %0 \n\t"                /* Transforms 16-bit input f16Val to 32-bit */
                        "movs %3, #128 \n\t"               /* f16SatVal = 0x80 */
                        "lsls %3, %3, #8 \n\t"             /* f16SatVal = 0x00008000 */
                        "mov %2, %3 \n\t"                  /* f16CmpVal = 0x00008000 */
                        "lsrs %2, %2, %1 \n\t"             /* f16CmpVal = f16CmpVal >> u16Sh */
                        "cmp %0, %2 \n\t"                  /* Compares f16Val with (0x00008000 >> u16Sh) */
                        "blt MLIB_ShLSat_F16_NegTest\n\t"  /* If f16Val < f16CmpVal, then jumps through two commands */
                        "subs %0, %3, #1 \n\t"             /* f16Val = 0x7FFF */
                        "b MLIB_ShLSat_F16_SatEnd\n\t"
					"MLIB_ShLSat_F16_NegTest:\n\t"
                        "mvns %2, %3 \n\t"                 /* f16CmpVal = 0xFFFF7FFF */
                        "asrs %2, %2, %1 \n\t"             /* f16CmpVal = f16CmpVal >> u16Sh */
                        "cmp %0, %2 \n\t"                  /* Compares f16Val with (0xFFFF7FFF >> u16Sh) */
                        "bgt MLIB_ShLSat_F16_NotSat\n\t"   /* If f16Val < f16CmpVal, then jumps through two commands */
                        "sxth %0, %3 \n\t"                 /* f16Val = 0xFFFF8000 */
                        "b MLIB_ShLSat_F16_SatEnd\n\t"
					"MLIB_ShLSat_F16_NotSat:\n\t"
                        "lsls %0, %0, %1 \n\t"             /* f16CmpVal = f16CmpVal << u16Sh */
					"MLIB_ShLSat_F16_SatEnd:\n\t"
                        : "+l"(f16Val), "+l"(u16Sh), "+l"(f16CmpVal), "+l"(f16SatVal):);
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

#endif /* _MLIB_SHIFT_F16_ASM_H_ */
