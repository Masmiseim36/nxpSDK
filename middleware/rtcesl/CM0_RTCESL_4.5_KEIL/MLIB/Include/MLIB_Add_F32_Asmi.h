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
*  Output = f32Add1 + f32Add2
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac32_t MLIB_AddSat_F32_FAsmi(register frac32_t f32Add1, register frac32_t f32Add2)
{
    register frac32_t f32AddVal=0, f32SatVal=0;

    #if defined(__CC_ARM)                                  /* For ARM Compiler */
        __asm volatile{ adds f32AddVal, f32Add1, f32Add2   /* f32AddVal = f32Add1 + f32Add2 */
                        movs f32SatVal, f32Add1            /* f32SatVal = f32Add1 */
                        eors f32SatVal, f32SatVal, f32Add2 /* f32SatVal = f32Add1 ^ f32Add2 */
                        bmi  SatEnd                        /* If f32SatVal < 0, then goes to the end of function */
                        eors f32Add1, f32Add1, f32AddVal   /* f32Add1 = f32Add1 ^ (f32Add1 + f32Add2) */
                        bpl SatEnd                         /* If f32Add1 >= 0, then goes to the end of function */
                        movs f32AddVal, #128               /* f32AddVal = 0x80 */
                        lsls f32AddVal, f32AddVal, #24     /* f32AddVal = 0x80000000 */
                        cmp f32Add2, #0                    /* Compares f32Add2 with 0 */
                        blt SatEnd                         /* If f32Add1 < 0, then goes to the end of function */
                        subs f32AddVal, f32AddVal, #1      /* f32AddVal = 0x7FFFFFFF */
                    SatEnd:};
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "adds %0, %1, %2 \n\t"               /* f32AddVal = f32Add1 + f32Add2 */
                        "movs %3, %1 \n\t"                   /* f32SatVal = f32Add1 */
                        "eors %3, %3, %2 \n\t"               /* f32SatVal = f32Add1 ^ f32Add2 */
                        "bmi MLIB_AddSat_F32_SatEnd \n\t"                      /* If f32SatVal < 0, then goes to the end of function */
                        "eors %1, %1, %0 \n\t"               /* f32Add1 = f32Add1 ^ (f32Add1 + f32Add2) */
                        "bpl MLIB_AddSat_F32_SatEnd \n\t"                      /* If f32Add1 >= 0, then goes to the end of function */
                        "movs %0, #128 \n\t"                 /* f32AddVal = 0x80 */
                        "lsls %0, %0, #24 \n\t"              /* f32AddVal = 0x80000000 */
                        "cmp %2, #0 \n\t"                    /* Compares f32Add2 with 0 */
                        "blt MLIB_AddSat_F32_SatEnd \n\t"                       /* If f32Add1 < 0, then goes to the end of function */
                        "subs %0, %0, #1 \n\t"               /* f32AddVal = 0x7FFFFFFF */
					"MLIB_AddSat_F32_SatEnd: \n\t"
                        : "+l"(f32AddVal), "+l"(f32Add1), "+l"(f32Add2), "+l"(f32SatVal):);
    #endif

    return f32AddVal;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD_F32_ASM_H_ */
