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
#ifndef _MLIB_DIVHW1Q_F32_ASM_H_
#define _MLIB_DIVHW1Q_F32_ASM_H_

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
#define MLIB_DivHw1Q_F32_Asm(f32Num, f32Denom)       MLIB_DivHw1Q_F32_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)
#define MLIB_DivHw1Q_F32ls_Asm(f32Num, f16Denom)     MLIB_DivHw1Q_F32ls_FAsm(f32Num, f16Denom, &gu16CntMmdvsq)
#define MLIB_DivHw1Q_F16ll_Asm(f32Num, f32Denom)     MLIB_DivHw1Q_F16ll_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)
#define MLIB_DivHw1Q_F16ls_Asm(f32Num, f16Denom)     MLIB_DivHw1Q_F16ls_FAsm(f32Num, f16Denom, &gu16CntMmdvsq)
                                                     
#define MLIB_DivHw1QSat_F32_Asm(f32Num, f32Denom)    MLIB_DivHw1QSat_F32_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)
#define MLIB_DivHw1QSat_F32ls_Asm(f32Num, f16Denom)  MLIB_DivHw1QSat_F32ls_FAsm(f32Num, f16Denom, &gu16CntMmdvsq)
#define MLIB_DivHw1QSat_F16ll_Asm(f32Num, f32Denom)  MLIB_DivHw1QSat_F16ll_FAsm(f32Num, f32Denom, &gu16CntMmdvsq)
#define MLIB_DivHw1QSat_F16ls_Asm(f32Num, f16Denom)  MLIB_DivHw1QSat_F16ls_FAsm(f32Num, f16Denom, &gu16CntMmdvsq)

#define MLIB_DivHw1Q_F16ls_Asmi(f32Num, f16Denom)    MLIB_DivHw1Q_F16ls_FAsmi(f32Num, f16Denom)
#define MLIB_DivHw1QSat_F16ls_Asmi(f32Num, f16Denom) MLIB_DivHw1QSat_F16ls_FAsmi(f32Num, f16Denom)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/  
extern frac32_t MLIB_DivHw1Q_F32_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
extern frac32_t MLIB_DivHw1Q_F32ls_FAsm(frac32_t f32Num, frac16_t f16Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHw1Q_F16ll_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHw1Q_F16ls_FAsm(frac32_t f32Num, frac16_t f16Denom, const volatile uint16_t *pVal);

extern frac32_t MLIB_DivHw1QSat_F32_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
extern frac32_t MLIB_DivHw1QSat_F32ls_FAsm(frac32_t f32Num, frac16_t f16Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHw1QSat_F16ll_FAsm(frac32_t f32Num, frac32_t f32Denom, const volatile uint16_t *pVal);
extern frac16_t MLIB_DivHw1QSat_F16ls_FAsm(frac32_t f32Num, frac16_t f16Denom, const volatile uint16_t *pVal);

/*******************************************************************************
* @brief     32-bit numerator, 16-bit denominator inputs 16-output one quadrant
*            division function
*
* @param     in  f32Num - Numerator in <0;1) in frac32_t
*                f16Denom - Denominator in <0;1) in frac16_t
*
* @return    This function returns
*                - frac16_t value in <0;1)
*
* @remarks   This function divides two fractional inputs:
*            result = f32Num / f16Denom.
*            The function does not saturate the output.
*            If the denominator is 0, the output is 0x7FFF.
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#elif defined(__CC_ARM) 
#pragma push                               /* Save current optimization level */
#pragma optimize O0                        /* Prevent common subroutine optimization */
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_DivHw1Q_F16ls_FAsmi(register frac32_t f32Num, register frac16_t f16Denom)
{
  register frac16_t f16Result = 0x7FFF;
  register frac32_t f32BasePtr = 0xF0004000;
  register frac32_t f32Temp1, f32Temp2;

    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ lsls    f16Denom, f16Denom, #16      /* Denominator << 16 */
                        beq     DivHw1Q_F16ls_End            /* If Denominator = 0, then goes to DivHw1Q_F16ls_End*/
                        asrs    f16Denom, f16Denom, #15      /* Denominator >> 15 */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                    DivHw1Q_F16ls_Recount:
                        adds    f32Temp2, f32Temp1, #1       /* f32Temp2 = gu16CntMmdvsq + 1 */
                        strh    f32Temp2, [&gu16CntMmdvsq]   /* gu16CntMmdvsq = f32Temp2 */
                        movs    f32Temp1, #0                 /* f32TestVal = 0 */
                        str     f32Temp1, [f32BasePtr, #8]   /* MMDVSQ_CSR = 0x00000000 */
                        str     f32Num, [f32BasePtr]         /* MMDVSQ_DEND = f32Num */
                        str     f16Denom, [f32BasePtr, #4]   /* MMDVSQ_DSOR = f16Denom */
                        ldr     f16Result, [f32BasePtr, #12] /* f16Result = MMDVSQ_RES */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                        cmp     f32Temp1, f32Temp2           /* Compares f32Temp1 and f32Temp2 */
                        bne     DivHw1Q_F16ls_Recount        /* If f32Temp1 != f32Temp2, then goes to DivHw1Q_F16ls_Recount */
                    DivHw1Q_F16ls_End: };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "lsls    %0, %0, #16 \n\t"             /* Denominator << 16 */
                        "beq     MLIB_DivHw1Q_F16ls_End \n\t"  /* If Denominator = 0, then goes to DivHw1Q_F16ls_End*/
                        "asrs    %0, %0, #15  \n\t"            /* Denominator >> 15 */
                        "ldrh    %3, [%5] \n\t"                /* f32Temp1 = gu16CntMmdvsq */
                    "MLIB_DivHw1Q_F16ls_Recount: \n\t"
                        "adds    %4, %3, #1 \n\t"              /* f32Temp2 = gu16CntMmdvsq + 1 */
                        "strh    %4, [%5] \n\t"                /* gu16CntMmdvsq = f32Temp2 */
                        "movs    %3, #0 \n\t"                  /* f32TestVal = 0 */
                        "str     %3, [%6, #8] \n\t"            /* MMDVSQ_CSR = 0x00000000 */
                        "str     %1, [%6] \n\t"                /* MMDVSQ_DEND = f32Num */
                        "str     %0, [%6, #4] \n\t"            /* MMDVSQ_DSOR = f16Denom */
                        "ldr     %2, [%6, #12] \n\t"           /* f16Result = MMDVSQ_RES */
                        "ldrh    %3, [%5] \n\t"                /* f32Temp1 = gu16CntMmdvsq */
                        "cmp     %3, %4 \n\t"                  /* Compares f32Temp1 and f32Temp2 */
                        "bne     MLIB_DivHw1Q_F16ls_Recount \n\t"   /* If f32Temp1 != f32Temp2, then goes to DivHw1Q_F16ls_Recount */
                    "MLIB_DivHw1Q_F16ls_End: \n\t"
                        : "+l"(f16Denom), "+l"(f32Num), "+l"(f16Result), "+l"(f32Temp1), "+l"(f32Temp2): "l"(&gu16CntMmdvsq), "l"(f32BasePtr):);
    #endif
    return f16Result;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#elif defined(__CC_ARM) 
#pragma pop                            /* Restore previous optimization level */
#endif

/*******************************************************************************
* @brief     32-bit numerator, 16-bit denominator inputs 16-output one quadrant
*            division function
*
* @param     in  f32Num - Numerator in <0;1) in frac32_t
*                f16Denom - Denominator in <0;1) in frac16_t
*
* @return    This function returns
*                - frac16_t value in <0;1)
*
* @remarks   This function divides two fractional inputs:
*            result = f32Num / f16Denom.
*            The function saturates the output if f32Num > f16Denom to 0x7FFF.
*            If the denominator is 0, the output is 0x7FFF.
*
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#elif defined(__CC_ARM) 
#pragma push                               /* Save current optimization level */
#pragma optimize O0                        /* Prevent common subroutine optimization */
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_DivHw1QSat_F16ls_FAsmi(register frac32_t f32Num, register frac16_t f16Denom)
{
  register frac16_t f16Result = 0x7FFF;
  register frac32_t f32BasePtr = 0xF0004000;
  register frac32_t f32Temp1, f32Temp2;
    #if defined(__CC_ARM)                                    /* For ARM Compiler */
        __asm volatile{ lsls    f16Denom, f16Denom, #16      /* Denominator << 16 */
                        cmp     f16Denom, f32Num             /* Compares Denominator with Numerator */
                        ble     DivHw1Q_F16ls_End            /* If Denominator >= Numerator, then goes to DivHw1QSat_F16ls_End */
                        asrs    f16Denom, f16Denom, #15      /* Denominator >> 15 */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                    DivHw1QSat_F16ls_Recount:
                        adds    f32Temp2, f32Temp1, #1       /* f32Temp2 = gu16CntMmdvsq + 1 */
                        strh    f32Temp2, [&gu16CntMmdvsq]   /* gu16CntMmdvsq = f32Temp2 */
                        movs    f32Temp1, #0                 /* f32TestVal = 0 */
                        str     f32Temp1, [f32BasePtr, #8]   /* MMDVSQ_CSR = 0x00000000 */
                        str     f32Num, [f32BasePtr]         /* MMDVSQ_DEND = f32Num */
                        str     f16Denom, [f32BasePtr, #4]   /* MMDVSQ_DSOR = f16Denom */
                        ldr     f16Result, [f32BasePtr, #12] /* f16Result = MMDVSQ_RES */
                        ldrh    f32Temp1, [&gu16CntMmdvsq]   /* f32Temp1 = gu16CntMmdvsq */
                        cmp     f32Temp1, f32Temp2           /* Compares f32Temp1 and f32Temp2 */
                        bne     DivHw1QSat_F16ls_Recount     /* If f32Temp1 != f32Temp2, then goes to DivHw1QSat_F16ls_Recount */
                    DivHw1Q_F16ls_End: };
    #elif defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050) 
        __asm volatile(
                        "lsls    %0, %0, #16 \n\t"             /* Denominator << 16 */
                        "cmp     %0, %1 \n\t"                  /* Compares Denominator with Numerator */
                        "ble     MLIB_DivHw1QSat_F16ls_End \n\t"/*  If Denominator >= Numerator, then goes to DivHw1QSat_F16ls_End */
                        "asrs    %0, %0, #15  \n\t"            /* Denominator >> 15 */
                        "ldrh    %3, [%5] \n\t"                /* f32Temp1 = gu16CntMmdvsq */
                    "MLIB_DivHw1QSat_F16ls_ecount: \n\t"
                        "adds    %4, %3, #1 \n\t"              /* f32Temp2 = gu16CntMmdvsq + 1 */
                        "strh    %4, [%5] \n\t"                /* gu16CntMmdvsq = f32Temp2 */
                        "movs    %3, #0 \n\t"                  /* f32TestVal = 0 */
                        "str     %3, [%6, #8] \n\t"            /* MMDVSQ_CSR = 0x00000000 */
                        "str     %1, [%6] \n\t"                /* MMDVSQ_DEND = f32Num */
                        "str     %0, [%6, #4] \n\t"            /* MMDVSQ_DSOR = f16Denom */
                        "ldr     %2, [%6, #12] \n\t"           /* f16Result = MMDVSQ_RES */
                        "ldrh    %3, [%5] \n\t"                /* f32Temp1 = gu16CntMmdvsq */
                        "cmp     %3, %4 \n\t"                  /* Compares f32Temp1 and f32Temp2 */
                        "bne     MLIB_DivHw1QSat_F16ls_Recount \n\t"/* If f32Temp1 != f32Temp2, then goes to DivHw1QSat_F16ls_Recount */
                    "MLIB_DivHw1QSat_F16ls_End: \n\t"
                        : "+l"(f16Denom), "+l"(f32Num), "+l"(f16Result), "+l"(f32Temp1), "+l"(f32Temp2): "l"(&gu16CntMmdvsq), "l"(f32BasePtr):);
    #endif
    return f16Result;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#elif defined(__CC_ARM) 
#pragma pop                            /* Restore previous optimization level */
#endif


#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIVHW1Q_F32_ASM_H_ */
