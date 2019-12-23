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
* @brief  Hardware square root
* 
*******************************************************************************/
#ifndef _GFLIB_SQRTHW_F16_ASMI_H_
#define _GFLIB_SQRTHW_F16_ASMI_H_

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
#define GFLIB_SqrtHw_F16_Asmi(f16Val) GFLIB_SqrtHw_F16_FAsmi(f16Val)

/***************************************************************************//*!
* @brief       This function returns the square root of input value.
*
* @param[in]         f16Val      The input value.
*
* @return      The function returns the square root of the input value. The
*              return value is within the [0, 1) 32 bit fraction range.
*              If input is <= 0, then the function returns 0. 
*
*              The computations are made by hardware MMDSSQ
*
*
****************************************************************************/   
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549  /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t GFLIB_SqrtHw_F16_FAsmi(register frac16_t f16Val)
{
  register frac32_t f32BasePtr = 0xF0004000;
  register frac16_t f16Result=0;
  register frac32_t f32Temp1=0, f32Temp2=0;

    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ movs    f16Result, #0                /* f16Result = 0 */
                        lsls    f16Val, f16Val, #16          /* f16Val << 16 */
                        bmi     SqrtHw_F16_End               /* If f16Val < 0, then goes to SqrtHw_F16_End */
                        asrs    f16Val, #1                   /* f16Val >> 1 */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                    SqrtHw_F16_Recount:
                        adds    f32Temp2, f32Temp1, #1       /* f32Temp2 = gu16CntMmdvsq + 1 */
                        strh    f32Temp2, [&gu16CntMmdvsq]   /* gu16CntMmdvsq = f32Temp2 */
                        movs    f32Temp1, #0                 /* f32TestVal = 0 */
                        str     f32Temp1, [f32BasePtr, #8]   /* MMDVSQ_CSR = 0x00000000 */
                        str     f16Val, [f32BasePtr, #16]    /* RTCESL_MMDVSQ_RCND = f16Val */
                    SqrtHw_F16_Wait:
                        ldr     f32Temp1, [f32BasePtr, #8]   /* f32Temp1 = MMDVSQ_CSR */
                        lsrs    f32Temp1, f32Temp1, #31      /* f32Temp1 >> 31*/
                        bne     SqrtHw_F16_Wait              /* If f32Temp1 != 0, then goes to SqrtHw_F16_Wait */
                        ldr     f16Result, [f32BasePtr, #12] /* f16Result = MMDVSQ_RES */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                        cmp     f32Temp1, f32Temp2           /* Compares f32Temp1 and f32Temp2 */
                        bne     SqrtHw_F16_Recount           /* If f32Temp1 != f32Temp2, then goes to SqrtHw_F16_Recount */
                    SqrtHw_F16_End: };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(       
                        "movs    %1, #0 \n\t"                   /* f16Result = 0 */
                        "lsls    %0, %0, #16 \n\t"              /* f16Val << 16 */
                        "bmi     GFLIB_SqrtHw_F16_End \n\t"     /* If f16Val < 0, then goes to GFLIB_SqrtHw_F16_End */
                        "asrs    %0, #1 \n\t"                   /* f16Val >> 1 */
                        "ldrh    %2, [%4] \n\t"                 /* f32Temp1 = gu16CntMmdvsq */
                    "GFLIB_SqrtHw_F16_Recount: \n\t"         
                        "adds    %3, %2, #1 \n\t"               /* f32Temp2 = gu16CntMmdvsq + 1 */
                        "strh    %3, [%4] \n\t"                 /* gu16CntMmdvsq = f32Temp2 */
                        "movs    %2, #0 \n\t"                   /* f32TestVal = 0 */
                        "str     %2, [%5, #8] \n\t"             /* MMDVSQ_CSR = 0x00000000 */
                        "str     %0, [%5, #16] \n\t"            /* RTCESL_MMDVSQ_RCND = f16Val */
                    "GFLIB_SqrtHw_F16_Wait: \n\t"         
                        "ldr     %2, [%5, #8] \n\t"             /* f32Temp1 = MMDVSQ_CSR */
                        "lsrs    %2, %2, #31 \n\t"              /* f32Temp1 >> 31*/
                        "bne     GFLIB_SqrtHw_F16_Wait \n\t"    /* If f32Temp1 != 0, then goes to GFLIB_SqrtHw_F16_Wait */
                        "ldr     %1, [%5, #12] \n\t"            /* f16Result = MMDVSQ_RES */
                        "ldrh    %2, [%4] \n\t"                 /* f32Temp1 = gu16CntMmdvsq */
                        "cmp     %2, %3 \n\t"                   /* Compares f32Temp1 and f32Temp2 */
                        "bne     GFLIB_SqrtHw_F16_Recount \n\t" /* If f32Temp1 != f32Temp2, then goes to GFLIB_SqrtHw_F16_Recount */
                    "GFLIB_SqrtHw_F16_End: \n\t"
                        : "+l"(f16Val), "+l"(f16Result), "+l"(f32Temp1), "+l"(f32Temp2): "l"(&gu16CntMmdvsq), "l"(f32BasePtr):);
    #endif
    return f16Result;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRTHW_F16_ASMI_H_ */

