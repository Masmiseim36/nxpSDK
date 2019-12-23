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
* @brief  Unsigned division
* 
*******************************************************************************/
#ifndef _MLIB_DIVHW1Q_A32_ASM_H_
#define _MLIB_DIVHW1Q_A32_ASM_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "MLIB_MMDVSQ.h"

/*******************************************************************************
* Macros 
*******************************************************************************/ 
#define MLIB_DivHw1Q_A32ss_Asmi(f16Num, f16Denom) MLIB_DivHw1Q_A32ss_FAsmi(f16Num, f16Denom)
#define MLIB_DivHw1Q_A32ls_Asmi(f32Num, f16Denom) MLIB_DivHw1Q_A32ls_FAsmi(f32Num, f16Denom)
#define MLIB_DivHw1Q_A32ll_Asm(f32Num, f32Denom)  MLIB_DivHw1Q_A32ll_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/  
extern acc32_t MLIB_DivHw1Q_A32ll_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
  
/*******************************************************************************
* @brief    16-bit numerator, 16-bit denominator inputs 32-bit accumulator 
*           output 1-quadrant division function
*
* @param    in  frac16_t f16Num
*                 - Numerator in <0;1) in frac16_t
*               frac16_t f16Denom
*                 - Denominator in <0;1) in frac16_t
*                       
* @return   This function returns
*               acc32_t value [0;65536.0)
*
* @remarks  This function divides two positive fractional inputs:
*           result = f16Num / f16Denom.
*           The output is the accumulator type where the lower
*           word is the fractional part.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline acc32_t MLIB_DivHw1Q_A32ss_FAsmi(register frac16_t f16Num, register frac16_t f16Denom)
{
  register acc32_t a32Result = 0x7FFFFFFF;
  register frac32_t f32BasePtr = 0xF0004000;
  register frac32_t f32Temp1, f32Temp2;

    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ lsls    f16Num, f16Num, #16          /* Numerator << 16 */
                        lsls    f16Denom, f16Denom, #16      /* Denominator << 16 */
                        beq     DivHw1Q_A32ss_End            /* If Denominator = 0, then goes to DivHw1Q_A32ss_End */
                        asrs    f16Denom, f16Denom, #15      /* Denominator >> 15 */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                    DivHw1Q_A32ss_Recount:
                        adds    f32Temp2, f32Temp1, #1       /* f32Temp2 = gu16CntMmdvsq + 1 */
                        strh    f32Temp2, [&gu16CntMmdvsq]   /* gu16CntMmdvsq = f32Temp2 */
                        movs    f32Temp1, #0                 /* f32TestVal = 0 */
                        str     f32Temp1, [f32BasePtr, #8]   /* MMDVSQ_CSR = 0x00000000 */
                        str     f16Num, [f32BasePtr]         /* MMDVSQ_DEND = f16Num */
                        str     f16Denom, [f32BasePtr, #4]   /* MMDVSQ_DSOR = f16Denom */
                        ldr     a32Result, [f32BasePtr, #12] /* a32Result = MMDVSQ_RES */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                        cmp     f32Temp1, f32Temp2           /* Compares f32Temp1 and f32Temp2 */
                        bne     DivHw1Q_A32ss_Recount        /* If f32Temp1 != f32Temp2, then goes to DivHw1Q_A32ss_Recount */
                    DivHw1Q_A32ss_End:  };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "lsls    %1, %1, #16 \n\t"             /* Numerator << 16 */
                        "lsls    %0, %0, #16 \n\t"             /* Denominator << 16 */
                        "beq     MLIB_DivHw1Q_A32ss_End \n\t"  /* If Denominator = 0, then goes to DivHw1Q_A32ss_End*/
                        "asrs    %0, %0, #15  \n\t"            /* Denominator >> 15 */
                        "ldrh    %3, [%5] \n\t"                /* f32Temp1 = gu16CntMmdvsq */
                    "MLIB_DivHw1Q_A32ss_Recount: \n\t"
                        "adds    %4, %3, #1 \n\t"              /* f32Temp2 = gu16CntMmdvsq + 1 */
                        "strh    %4, [%5] \n\t"                /* gu16CntMmdvsq = f32Temp2 */
                        "movs    %3, #0 \n\t"                  /* f32TestVal = 0 */
                        "str     %3, [%6, #8] \n\t"            /* MMDVSQ_CSR = 0x00000000 */
                        "str     %1, [%6] \n\t"                /* MMDVSQ_DEND = f16Num */
                        "str     %0, [%6, #4] \n\t"            /* MMDVSQ_DSOR = f16Denom */
                        "ldr     %2, [%6, #12] \n\t"           /* f16Result = MMDVSQ_RES */
                        "ldrh    %3, [%5] \n\t"                /* f32Temp1 = gu16CntMmdvsq */
                        "cmp     %3, %4 \n\t"                  /* Compares f32Temp1 and f32Temp2 */
                        "bne     MLIB_DivHw1Q_A32ss_Recount \n\t"   /* If f32Temp1 != f32Temp2, then goes to DivHw1Q_A32ss_Recount */
                    "MLIB_DivHw1Q_A32ss_End: \n\t"
                        : "+l"(f16Denom), "+l"(f16Num), "+l"(a32Result), "+l"(f32Temp1), "+l"(f32Temp2): "l"(&gu16CntMmdvsq), "l"(f32BasePtr):);
    #endif
    return a32Result;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/*******************************************************************************
* @brief    32-bit numerator, 16-bit denominator inputs 32-bit accumulator 
*           output 1-quadrant division function
*
* @param    in  frac32_t f32Num
*                 - Numerator in <0;1) in frac32_t
*               frac16_t f16Denom
*                 - Denominator in <0;1) in frac16_t
*                       
* @return   This function returns
*               acc32_t value [0;65536.0)
*
* @remarks  This function divides two non-negative fractional inputs:
*           result = f32Num / f16Denom.
*           The output is the accumulator type where the lower
*           word is the fractional part.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline acc32_t MLIB_DivHw1Q_A32ls_FAsmi(register frac32_t f32Num, register frac16_t f16Denom)
{
  register acc32_t a32Result = 0x7FFFFFFF;
  register frac32_t f32BasePtr = 0xF0004000;
  register frac32_t f32Temp1, f32Temp2;

    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ lsls    f16Denom, f16Denom, #16      /* Denominator << 16 */
                        beq     DivHw1Q_A32ls_End            /* If Denominator = 0, then goes to DivHw1Q_A32ls_End*/
                        asrs    f16Denom, f16Denom, #15      /* Denominator >> 15 */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                    DivHw1Q_A32ls_Recount:
                        adds    f32Temp2, f32Temp1, #1       /* f32Temp2 = gu16CntMmdvsq + 1 */
                        strh    f32Temp2, [&gu16CntMmdvsq]   /* gu16CntMmdvsq = f32Temp2 */
                        movs    f32Temp1, #0                 /* f32TestVal = 0 */
                        str     f32Temp1, [f32BasePtr, #8]   /* MMDVSQ_CSR = 0x00000000 */
                        str     f32Num, [f32BasePtr]         /* MMDVSQ_DEND = f32Num */
                        str     f16Denom, [f32BasePtr, #4]   /* MMDVSQ_DSOR = f16Denom */
                        ldr     a32Result, [f32BasePtr, #12] /* a32Result = MMDVSQ_RES */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                        cmp     f32Temp1, f32Temp2           /* Compares f32Temp1 and f32Temp2 */
                        bne     DivHw1Q_A32ls_Recount        /* If f32Temp1 != f32Temp2, then goes to DivHw1Q_A32ls_Recount */
                    DivHw1Q_A32ls_End:  };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "lsls    %0, %0, #16 \n\t"             /* Denominator << 16 */
                        "beq     MLIB_DivHw1Q_A32ls_End \n\t"  /* If Denominator = 0, then goes to DivHw1Q_A32ls_End*/
                        "asrs    %0, %0, #15  \n\t"            /* Denominator >> 15 */
                        "ldrh    %3, [%5] \n\t"                /* f32Temp1 = gu16CntMmdvsq */
                    "MLIB_DivHw1Q_A32ls_Recount: \n\t"
                        "adds    %4, %3, #1 \n\t"              /* f32Temp2 = gu16CntMmdvsq + 1 */
                        "strh    %4, [%5] \n\t"                /* gu16CntMmdvsq = f32Temp2 */
                        "movs    %3, #0 \n\t"                  /* f32TestVal = 0 */
                        "str     %3, [%6, #8] \n\t"            /* MMDVSQ_CSR = 0x00000000 */
                        "str     %1, [%6] \n\t"                /* MMDVSQ_DEND = f32Num */
                        "str     %0, [%6, #4] \n\t"            /* MMDVSQ_DSOR = f16Denom */
                        "ldr     %2, [%6, #12] \n\t"           /* f16Result = MMDVSQ_RES */
                        "ldrh    %3, [%5] \n\t"                /* f32Temp1 = gu16CntMmdvsq */
                        "cmp     %3, %4 \n\t"                  /* Compares f32Temp1 and f32Temp2 */
                        "bne     MLIB_DivHw1Q_A32ls_Recount \n\t"   /* If f32Temp1 != f32Temp2, then goes to DivHw1Q_A32ls_Recount */
                    "MLIB_DivHw1Q_A32ls_End: \n\t"
                        : "+l"(f16Denom), "+l"(f32Num), "+l"(a32Result), "+l"(f32Temp1), "+l"(f32Temp2): "l"(&gu16CntMmdvsq), "l"(f32BasePtr):);
    #endif
    return a32Result;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_DIVHW1Q_A32_ASM_H_ */
