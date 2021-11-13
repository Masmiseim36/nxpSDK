/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
*
****************************************************************************//*!
*
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_A32_ASM_H_
#define _MLIB_MULRND_A32_ASM_H_

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
#define MLIB_MulRndSat_F16as_Asmi(a32Accum, f16Mult)    MLIB_MulRndSat_F16as_FAsmi(a32Accum, f16Mult)                                                       
#define MLIB_MulRnd_A32_Asmi(a32Mult1, a32Mult2)        MLIB_MulRnd_A32_FAsmi(a32Mult1, a32Mult2)
#define MLIB_MulRndSat_A32_Asmi(a32Mult1, a32Mult2)     MLIB_MulRndSat_A32_FAsmi(a32Mult1, a32Mult2)
#define MLIB_MulNegRnd_A32_Asmi(a32Mult1, a32Mult2)     MLIB_MulNegRnd_A32_FAsmi(a32Mult1, a32Mult2)
#define MLIB_MulNegRndSat_A32_Asmi(a32Mult1, a32Mult2)  MLIB_MulNegRndSat_A32_FAsmi(a32Mult1, a32Mult2)
#define MLIB_MulNegRndSat_F16as_Asmi(a32Accum, f16Mult) MLIB_MulNegRndSat_F16as_FAsmi(a32Accum, f16Mult)

/***************************************************************************//*!
*
* f16Out = a32Accum * f16Mult
* With saturation
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_MulRndSat_F16as_FAsmi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    register frac32_t f32Val=0;
    #if defined(__CC_ARM)                                       /* For ARM Compiler */
        __asm volatile{ sxth f16Mult, f16Mult                   /* Converts 16-bit input to 32-bit */
                        uxth f32Val, a32Accum                   /* f32Val = a32Accum.L */
                        asrs a32Accum, a32Accum, #16            /* a32Accum = a32Accum.H */
    
                        muls f32Val, f32Val, f16Mult            /* f32Val = a32Accum.L * f16Mult */
                        asrs f32Val, f32Val, #7                 /* f32Val >> 7 */
                        adds f32Val, f32Val, #128               /* Rounding */
                        asrs f32Val, f32Val, #8                 /* f32Val >> 8 */
    
                        muls f16Mult, f16Mult, a32Accum         /* f16Mult = a32Accum.H * f16Mult */
                        lsls f16Mult, f16Mult, #1               /* f16Mult << 1 */
                        adds f16Mult, f16Mult, f32Val           /* f16Mult = f16Mult + f32Val */
    
                        movs f32Val, #128                       /* f32Val = 0x80 */
                        lsls f32Val, f32Val, #8                 /* f32Val = 0x8000 */
    
                        rev a32Accum, f16Mult                   /* Reverse byte */
                        cmp a32Accum, #0x80                     /* Compares with 0x80 */
                        bne PosTest                             /* If Result != 0x80000000, then goes to PosTest */
                        subs f16Mult, f32Val, #1                /* If Result = 0x80000000, then f16Mult = 0x7FFF */
                        b SatEnd                                /* Goes to SatEnd */
                    PosTest:    
                        cmp f32Val, f16Mult                     /* Compares f16Mult with 0x8000 */
                        bge NegTest                             /* If f16Mult < 0x8000, then goes to NegTest */
                        subs f16Mult, f32Val, #1                /* If f16Mult >= 0x8000, then f16Mult = 0x7FFF */
                        b SatEnd                                /* Goes to SatEnd */
                    NegTest:    
                        sxth f32Val, f32Val                     /* f32Val = 0xFFFF8000 */
                        cmp f32Val, f16Mult                     /* Compares f16Mult with 0xFFFF8000 */
                        ble SatEnd                              /* If f16Mult >= 0xFFFF8000, then goes to SatEnd */
                        mov f16Mult, f32Val                     /* If f16Mult < 0xFFFF8000, then f16Mult = 0xFFFF8000 */
                    SatEnd: };
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "sxth %0, %0 \n\t"                      /* Converts 16-bit input to 32-bit */
                        "uxth %2, %1 \n\t"                      /* f32Val = a32Accum.L */
                        "asrs %1, %1, #16 \n\t"                 /* a32Accum = a32Accum.H */
  
                        "muls %2, %2, %0 \n\t"                  /* f32Val = a32Accum.L * f16Mult */
                        "asrs %2, %2, #7 \n\t"                  /* f32Val >> 7 */
                        "adds %2, %2, #128 \n\t"                /* Rounding */
                        "asrs %2, %2, #8 \n\t"                  /* f32Val >> 8 */
  
                        "muls %0, %0, %1 \n\t"                  /* f16Mult = a32Accum.H * f16Mult */
                        "lsls %0, %0, #1 \n\t"                  /* f16Mult << 1 */
                        "adds %0, %0, %2 \n\t"                  /* f16Mult = f16Mult + f32Val */
  
                        "movs %2, #128 \n\t"                    /* f32Val = 0x80 */
                        "lsls %2, %2, #8 \n\t"                  /* f32Val = 0x8000 */
  
                        "rev %1, %0 \n\t"                       /* Reverse byte */
                        "cmp %1, #0x80 \n\t"                    /* Compares with 0x80 */
                        "bne MLIB_MulRndSat_F16as_PosTest%= \n\t" /* If Result != 0x80000000, then jumps through two commands */
                        "subs %0, %2, #1 \n\t"                  /* If Result = 0x80000000, then f16Mult = 0x7FFF */
                        "b MLIB_MulRndSat_F16as_SatEnd%= \n\t"    /* Jumps through eight commands */
					"MLIB_MulRndSat_F16as_PosTest%=: \n\t"  
                        "cmp %2, %0 \n\t"                       /* Compares f16Mult with 0x8000 */
                        "bge MLIB_MulRndSat_F16as_NegTest%= \n\t" /* If f16Mult < 0x8000, then jumps through two commands */
                        "subs %0, %2, #1 \n\t"                  /* If f16Mult >= 0x8000, then f16Mult = 0x7FFF */
                        "b MLIB_MulRndSat_F16as_SatEnd%= \n\t"    /* Jumps through four commands */
					"MLIB_MulRndSat_F16as_NegTest%=: \n\t"  
                        "sxth %2, %2 \n\t"                      /* f32Val = 0xFFFF8000 */
                        "cmp %2, %0 \n\t"                       /* Compares f16Mult with 0xFFFF8000 */
                        "ble MLIB_MulRndSat_F16as_SatEnd%= \n\t"  /* If f16Mult >= 0xFFFF8000, then jumps through next commands */
                        "mov %0, %2 \n\t"                       /* If f16Mult < 0xFFFF8000, then f16Mult = 0xFFFF8000 */
					"MLIB_MulRndSat_F16as_SatEnd%=: \n\t"
                        : "+l"(f16Mult), "+l"(a32Accum), "+l"(f32Val):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)                   /* For GCC compiler */
                            ".syntax unified \n"                /* Using unified asm syntax */
                        #endif    
                        "sxth %0, %0 \n"                        /* Converts 16-bit input to 32-bit */
                        "uxth %2, %1 \n"                        /* f32Val = a32Accum.L */
                        "asrs %1, %1, #16 \n"                   /* a32Accum = a32Accum.H */
    
                        "muls %2, %2, %0 \n"                    /* f32Val = a32Accum.L * f16Mult */
                        "asrs %2, %2, #7 \n"                    /* f32Val >> 7 */
                        "adds %2, %2, #128 \n"                  /* Rounding */
                        "asrs %2, %2, #8 \n"                    /* f32Val >> 8 */
    
                        "muls %0, %0, %1 \n"                    /* f16Mult = a32Accum.H * f16Mult */
                        "lsls %0, %0, #1 \n"                    /* f16Mult << 1 */
                        "adds %0, %0, %2 \n"                    /* f16Mult = f16Mult + f32Val */
    
                        "movs %2, #128 \n"                      /* f32Val = 0x80 */
                        "lsls %2, %2, #8 \n"                    /* f32Val = 0x8000 */
    
                        "rev %1, %0 \n"                         /* Reverse byte */
                        "cmp %1, #0x80 \n"                      /* Compares with 0x80 */
                        "bne .+6 \n"                            /* If Result != 0x80000000, then jumps through two commands */
                        "subs %0, %2, #1 \n"                    /* If Result = 0x80000000, then f16Mult = 0x7FFF */
                        "b .+18 \n"                             /* Jumps through eight commands */
    
                        "cmp %2, %0 \n"                         /* Compares f16Mult with 0x8000 */
                        "bge .+6 \n"                            /* If f16Mult < 0x8000, then jumps through two commands */
                        "subs %0, %2, #1 \n"                    /* If f16Mult >= 0x8000, then f16Mult = 0x7FFF */
                        "b .+10 \n"                             /* Jumps through four commands */
    
                        "sxth %2, %2 \n"                        /* f32Val = 0xFFFF8000 */
                        "cmp %2, %0 \n"                         /* Compares f16Mult with 0xFFFF8000 */
                        "ble .+4 \n"                            /* If f16Mult >= 0xFFFF8000, then jumps through next commands */
                        "mov %0, %2 \n"                         /* If f16Mult < 0xFFFF8000, then f16Mult = 0xFFFF8000 */
                        #if defined(__GNUC__)                   /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Mult), "+l"(a32Accum), "+l"(f32Val):);
    #endif
    return f16Mult;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* Output = a32Mult1 * a32Mult2
* Without saturation
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline acc32_t MLIB_MulRnd_A32_FAsmi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register frac32_t a32Val1=0, a32Val2=0, a32Val3=0;
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ uxth a32Val1, a32Mult1              /* a32Val1 = a32Mult1.L */
                        uxth a32Val2, a32Mult2              /* a32Val2 = a32Mult2.L */

                        asrs a32Mult1, a32Mult1, #16        /* a32Mult1 = a32Mult1.H */
                        asrs a32Mult2, a32Mult2, #16        /* a32Mult2 = a32Mult2.H */

                        movs a32Val3, a32Val1               /* a32Val3 = a32Mult1.L */
                        muls a32Val3, a32Val3, a32Val2      /* a32Val3 = a32Mult1.L * a32Mult2.L */
                        lsrs a32Val3, a32Val3, #7           /* a32Val3 >> 7 */
                        adds a32Val3, a32Val3, #0x80        /* Rounding */
                        lsrs a32Val3, a32Val3, #8           /* a32Val3 >> 8 */

                        muls a32Val1, a32Val1, a32Mult2     /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        muls a32Val2, a32Val2, a32Mult1     /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        adds a32Val2, a32Val2, a32Val1      /* a32Val2 = a32Val2 + a32Val1 */

                        lsls a32Val2, a32Val2, #1           /* a32Val2 << 1 */
                        adds a32Val2, a32Val2, a32Val3      /* a32Val2 = a32Val2 + a32Val3 */

                        muls a32Mult1, a32Mult1, a32Mult2   /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        lsls a32Mult1, a32Mult1, #17        /* a32Mult1 << 17 */
                        adds a32Mult1, a32Mult1, a32Val2 }; /* a32Mult1 = a32Mult1 + a32Val2 */
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "uxth %2, %0 \n\t"                  /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n\t"                  /* a32Val2 = a32Mult2.L */

                        "asrs %0, %0, #16 \n\t"             /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n\t"             /* a32Mult2 = a32Mult2.H */

                        "movs %4, %2 \n\t"                  /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n\t"              /* a32Val3 = a32Mult1.L * a32Mult2.L */
                        "lsrs %4, %4, #7 \n\t"              /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n\t"           /* Rounding */
                        "lsrs %4, %4, #8 \n\t"              /* a32Val3 >> 8 */

                        "muls %2, %2, %1 \n\t"              /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n\t"              /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n\t"              /* a32Val2 = a32Val2 + a32Val1 */

                        "lsls %3, %3, #1 \n\t"              /* a32Val2 << 1 */
                        "adds %3, %3, %4 \n\t"              /* a32Val2 = a32Val2 + a32Val3 */

                        "muls %0, %0, %1 \n\t"              /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "lsls %0, %0, #17 \n\t"             /* a32Mult1 << 17 */
                        "adds %0, %0, %3 \n\t"              /* a32Mult1 = a32Mult1 + a32Val2 */
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "uxth %2, %0 \n"                    /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n"                    /* a32Val2 = a32Mult2.L */

                        "asrs %0, %0, #16 \n"               /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n"               /* a32Mult2 = a32Mult2.H */

                        "movs %4, %2 \n"                    /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n"                /* a32Val3 = a32Mult1.L * a32Mult2.L */
                        "lsrs %4, %4, #7 \n"                /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n"             /* Rounding */
                        "lsrs %4, %4, #8 \n"                /* a32Val3 >> 8 */

                        "muls %2, %2, %1 \n"                /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n"                /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n"                /* a32Val2 = a32Val2 + a32Val1 */

                        "lsls %3, %3, #1 \n"                /* a32Val2 << 1 */
                        "adds %3, %3, %4 \n"                /* a32Val2 = a32Val2 + a32Val3 */

                        "muls %0, %0, %1 \n"                /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "lsls %0, %0, #17 \n"               /* a32Mult1 << 17 */
                        "adds %0, %0, %3 \n"                /* a32Mult1 = a32Mult1 + a32Val2 */
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #endif

    return a32Mult1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* Output = a32Mult1 * a32Mult2
* With saturation
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline acc32_t MLIB_MulRndSat_A32_FAsmi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Val1=0, a32Val2=0, a32Val3=0;
    #if defined(__CC_ARM)                                     /* For ARM Compiler */
        __asm volatile{ uxth a32Val1, a32Mult1                /* a32Val1 = a32Mult1.L */
                        uxth a32Val2, a32Mult2                /* a32Val2 = a32Mult2.L */
                        asrs a32Mult1, a32Mult1, #16          /* a32Mult1 = a32Mult1.H */
                        asrs a32Mult2, a32Mult2, #16          /* a32Mult2 = a32Mult2.H */
  
                        movs a32Val3, a32Val1                 /* a32Val3 = a32Mult1.L */
                        muls a32Val3, a32Val3, a32Val2        /* a32Val3 = a32Mult1.L * a32Mult2.L */
  
                        muls a32Val1, a32Val1, a32Mult2       /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        muls a32Val2, a32Val2, a32Mult1       /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        adds a32Val2, a32Val2, a32Val1        /* a32Val2 = a32Val2 + a32Val1 */
  
                        lsrs a32Val1, a32Val3, #16            /* a32Val1 = a32Val3 >> 16 */
                        adds a32Val2, a32Val2, a32Val1        /* a32Val2 = a32Val2 + a32Val3 */
                        asrs a32Val1, a32Val2, #16            /* a32Val1 = a32Val2 >> 16 */
                        uxth a32Val2, a32Val2                 /* Clears higher 16 bits */
  
                        muls a32Mult1, a32Mult1, a32Mult2     /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        adds a32Mult1, a32Mult1, a32Val1      /* a32Mult1 = a32Mult1 + a32Val1 */
  
                        asrs a32Mult2, a32Mult1, #14          /* a32Mult2 = higher 18 bits of multiplication */
                        beq MLIB_MulRndSat_A32_NotSat         /* If a32Mult2 = 0, then goes to the MLIB_MulRndSat_A32_NotSat */
                        mvns a32Mult2, a32Mult2               /* a32Mult2 = ~ a32Mult2 */
                        beq MLIB_MulRndSat_A32_NotSat         /* If a32Mult2 = 0, then goes to the MLIB_MulRndSat_A32_NotSat */
                        asrs a32Mult2, a32Mult1, #31          /* a32Mult2 = sign of result */
                        movs a32Mult1, #0x80                  /* a32Mult1 = 0x80 */
                        lsls a32Mult1, a32Mult1, #24          /* a32Mult1 = 0x80000000 */
                        subs a32Mult1, a32Mult1, #1           /* a32Mult1 = 0x7FFFFFFF */
                        subs a32Mult1, a32Mult1, a32Mult2     /* a32Mult1 = 0x7FFFFFFF - Sign of result */
                        b MLIB_MulRndSat_A32_End              /* Goes to the MLIB_MulRndSat_A32_End*/
  
                    MLIB_MulRndSat_A32_NotSat:  
                        lsls a32Mult1, a32Mult1, #16          /* a32Mult1 << 16 */
                        adds a32Mult1, a32Mult1, a32Val2      /* a32Mult1 = a32Mult1 + a32Val2 */
                        lsls a32Mult1, a32Mult1, #1           /* Result << 1 */
                        uxth a32Val3, a32Val3                 /* Clears higher 16 bits */
                        lsrs a32Val3, a32Val3, #7             /* a32Val3 >> 7 */
                        adds a32Val3, a32Val3, #0x80          /* Rounding */
                        lsrs a32Val3, a32Val3, #8             /* a32Val3 >> 8 */
                        adds a32Mult1, a32Mult1, a32Val3      /* a32Mult1 = a32Mult1 + a32Val3 */
                    MLIB_MulRndSat_A32_End: };
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "uxth %2, %0 \n\t"                    /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n\t"                    /* a32Val2 = a32Mult2.L */
                        "asrs %0, %0, #16 \n\t"               /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n\t"               /* a32Mult2 = a32Mult2.H */

                        "movs %4, %2 \n\t"                    /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n\t"                /* a32Val3 = a32Mult1.L * a32Mult2.L */

                        "muls %2, %2, %1 \n\t"                /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n\t"                /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n\t"                /* a32Val2 = a32Val2 + a32Val1 */

                        "lsrs %2, %4, #16 \n\t"               /* a32Val1 = a32Val3 >> 16 */
                        "adds %3, %3, %2 \n\t"                /* a32Val2 = a32Val2 + a32Val3 */
                        "asrs %2, %3, #16 \n\t"               /* a32Val1 = a32Val2 >> 16 */
                        "uxth %3, %3 \n\t"                    /* Clears higher 16 bits */

                        "muls %0, %0, %1 \n\t"                /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "adds %0, %0, %2 \n\t"                /* a32Mult1 = a32Mult1 + a32Val1 */

                        "asrs %1, %0, #14 \n\t"               /* a32Mult2 = higher 18 bits of multiplication */
                        "beq MLIB_MulRndSat_A32_NotSat1%= \n\t" /* If a32Mult2 = 0, then goes to the MLIB_MulRndSat_A32_NotSat */
                        "mvns %1, %1 \n\t"                    /* a32Mult2 = ~ a32Mult2 */
                        "beq MLIB_MulRndSat_A32_NotSat1%= \n\t" /* If a32Mult2 = 0, then goes to the MLIB_MulRndSat_A32_NotSat */
                        "asrs %1, %0, #31 \n\t"               /* a32Mult2 = sign of result */
                        "movs %0, #0x80 \n\t"                 /* a32Mult1 = 0x80 */
                        "lsls %0, %0, #24 \n\t"               /* a32Mult1 = 0x80000000 */
                        "subs %0, %0, #1 \n\t"                /* a32Mult1 = 0x7FFFFFFF */
                        "subs %0, %0, %1 \n\t"                /* a32Mult1 = 0x7FFFFFFF - Sign of result */
                        "b MLIB_MulRndSat_A32_End1%= \n\t"      /* Goes to the MLIB_MulRndSat_A32_End*/

                    "MLIB_MulRndSat_A32_NotSat1%=: \n\t"
                        "lsls %0, %0, #16 \n\t"               /* a32Mult1 << 16 */
                        "adds %0, %0, %3 \n\t"                /* a32Mult1 = a32Mult1 + a32Val2 */
                        "lsls %0, %0, #1 \n\t"                /* Result << 1 */
                        "uxth %4, %4 \n\t"                    /* Clears higher 16 bits */
                        "lsrs %4, %4, #7 \n\t"                /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n\t"             /* Rounding */
                        "lsrs %4, %4, #8 \n\t"                /* a32Val3 >> 8 */
                        "adds %0, %0, %4 \n\t"                /* a32Mult1 = a32Mult1 + a32Val3 */

                    "MLIB_MulRndSat_A32_End1%=: \n\t"
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #elif defined(__GNUC__)
        __asm volatile(
                        #if defined(__GNUC__)                 /* For GCC compiler */
                            ".syntax unified \n"              /* Using unified asm syntax */
                        #endif  
                        "uxth %2, %0 \n"                      /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n"                      /* a32Val2 = a32Mult2.L */
                        "asrs %0, %0, #16 \n"                 /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n"                 /* a32Mult2 = a32Mult2.H */
  
                        "movs %4, %2 \n"                      /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n"                  /* a32Val3 = a32Mult1.L * a32Mult2.L */
  
                        "muls %2, %2, %1 \n"                  /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n"                  /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n"                  /* a32Val2 = a32Val2 + a32Val1 */
  
                        "lsrs %2, %4, #16 \n"                 /* a32Val1 = a32Val3 >> 16 */
                        "adds %3, %3, %2 \n"                  /* a32Val2 = a32Val2 + a32Val3 */
                        "asrs %2, %3, #16 \n"                 /* a32Val1 = a32Val2 >> 16 */
                        "uxth %3, %3 \n"                      /* Clears higher 16 bits */
  
                        "muls %0, %0, %1 \n"                  /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "adds %0, %0, %2 \n"                  /* a32Mult1 = a32Mult1 + a32Val1 */
  
                        "asrs %1, %0, #14 \n"                 /* a32Mult2 = higher 18 bits of multiplication */
                        "beq MLIB_MulRndSat_A32_NotSat \n"    /* If a32Mult2 = 0, then goes to the MLIB_MulRndSat_A32_NotSat */
                        "mvns %1, %1 \n"                      /* a32Mult2 = ~ a32Mult2 */
                        "beq MLIB_MulRndSat_A32_NotSat \n"    /* If a32Mult2 = 0, then goes to the MLIB_MulRndSat_A32_NotSat */
                        "asrs %1, %0, #31 \n"                 /* a32Mult2 = sign of result */
                        "movs %0, #0x80 \n"                   /* a32Mult1 = 0x80 */
                        "lsls %0, %0, #24 \n"                 /* a32Mult1 = 0x80000000 */
                        "subs %0, %0, #1 \n"                  /* a32Mult1 = 0x7FFFFFFF */
                        "subs %0, %0, %1 \n"                  /* a32Mult1 = 0x7FFFFFFF - Sign of result */
                        "b MLIB_MulRndSat_A32_End \n"         /* Goes to the MLIB_MulRndSat_A32_End*/
  
                    "MLIB_MulRndSat_A32_NotSat: \n"  
                        "lsls %0, %0, #16 \n"                 /* a32Mult1 << 16 */
                        "adds %0, %0, %3 \n"                  /* a32Mult1 = a32Mult1 + a32Val2 */
                        "lsls %0, %0, #1 \n"                  /* Result << 1 */
                        "uxth %4, %4 \n"                      /* Clears higher 16 bits */
                        "lsrs %4, %4, #7 \n"                  /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n"               /* Rounding */
                        "lsrs %4, %4, #8 \n"                  /* a32Val3 >> 8 */
                        "adds %0, %0, %4 \n"                  /* a32Mult1 = a32Mult1 + a32Val3 */
  
                    "MLIB_MulRndSat_A32_End: \n"  
                        #if defined(__GNUC__)                 /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)                 /* For GCC compiler */
                            ".syntax unified \n"              /* Using unified asm syntax */
                        #endif  
                        "uxth %2, %0 \n"                      /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n"                      /* a32Val2 = a32Mult2.L */
                        "asrs %0, %0, #16 \n"                 /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n"                 /* a32Mult2 = a32Mult2.H */
  
                        "movs %4, %2 \n"                      /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n"                  /* a32Val3 = a32Mult1.L * a32Mult2.L */
  
                        "muls %2, %2, %1 \n"                  /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n"                  /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n"                  /* a32Val2 = a32Val2 + a32Val1 */
  
                        "lsrs %2, %4, #16 \n"                 /* a32Val1 = a32Val3 >> 16 */
                        "adds %3, %3, %2 \n"                  /* a32Val2 = a32Val2 + a32Val3 */
                        "asrs %2, %3, #16 \n"                 /* a32Val1 = a32Val2 >> 16 */
                        "uxth %3, %3 \n"                      /* Clears higher 16 bits */
  
                        "muls %0, %0, %1 \n"                  /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "adds %0, %0, %2 \n"                  /* a32Mult1 = a32Mult1 + a32Val1 */
  
                        "asrs %1, %0, #14 \n"                 /* a32Mult2 = higher 18 bits of multiplication */
                        "beq MLIB_MulRndSat_A32_NotSat \n"    /* If a32Mult2 = 0, then goes to the MLIB_MulRndSat_A32_NotSat */
                        "mvns %1, %1 \n"                      /* a32Mult2 = ~ a32Mult2 */
                        "beq MLIB_MulRndSat_A32_NotSat \n"    /* If a32Mult2 = 0, then goes to the MLIB_MulRndSat_A32_NotSat */
                        "asrs %1, %0, #31 \n"                 /* a32Mult2 = sign of result */
                        "movs %0, #0x80 \n"                   /* a32Mult1 = 0x80 */
                        "lsls %0, %0, #24 \n"                 /* a32Mult1 = 0x80000000 */
                        "subs %0, %0, #1 \n"                  /* a32Mult1 = 0x7FFFFFFF */
                        "subs %0, %0, %1 \n"                  /* a32Mult1 = 0x7FFFFFFF - Sign of result */
                        "b MLIB_MulRndSat_A32_End \n"         /* Goes to the MLIB_MulRndSat_A32_End*/
  
                    "MLIB_MulRndSat_A32_NotSat: \n"  
                        "lsls %0, %0, #16 \n"                 /* a32Mult1 << 16 */
                        "adds %0, %0, %3 \n"                  /* a32Mult1 = a32Mult1 + a32Val2 */
                        "lsls %0, %0, #1 \n"                  /* Result << 1 */
                        "uxth %4, %4 \n"                      /* Clears higher 16 bits */
                        "lsrs %4, %4, #7 \n"                  /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n"               /* Rounding */
                        "lsrs %4, %4, #8 \n"                  /* a32Val3 >> 8 */
                        "adds %0, %0, %4 \n"                  /* a32Mult1 = a32Mult1 + a32Val3 */
  
                    "MLIB_MulRndSat_A32_End: \n"  
                        #if defined(__GNUC__)                 /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #endif

    return a32Mult1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* Output = a32Mult1 * a32Mult2
* Without saturation
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline acc32_t MLIB_MulNegRnd_A32_FAsmi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register frac32_t a32Val1=0, a32Val2=0, a32Val3=0;
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ uxth a32Val1, a32Mult1              /* a32Val1 = a32Mult1.L */
                        uxth a32Val2, a32Mult2              /* a32Val2 = a32Mult2.L */

                        asrs a32Mult1, a32Mult1, #16        /* a32Mult1 = a32Mult1.H */
                        asrs a32Mult2, a32Mult2, #16        /* a32Mult2 = a32Mult2.H */

                        movs a32Val3, a32Val1               /* a32Val3 = a32Mult1.L */
                        muls a32Val3, a32Val3, a32Val2      /* a32Val3 = a32Mult1.L * a32Mult2.L */
                        lsrs a32Val3, a32Val3, #7           /* a32Val3 >> 7 */
                        adds a32Val3, a32Val3, #0x80        /* Rounding */
                        lsrs a32Val3, a32Val3, #8           /* a32Val3 >> 8 */

                        muls a32Val1, a32Val1, a32Mult2     /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        muls a32Val2, a32Val2, a32Mult1     /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        adds a32Val2, a32Val2, a32Val1      /* a32Val2 = a32Val2 + a32Val1 */

                        lsls a32Val2, a32Val2, #1           /* a32Val2 << 1 */
                        adds a32Val2, a32Val2, a32Val3      /* a32Val2 = a32Val2 + a32Val3 */

                        muls a32Mult1, a32Mult1, a32Mult2   /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        lsls a32Mult1, a32Mult1, #17        /* a32Mult1 << 17 */
                        adds a32Mult1, a32Mult1, a32Val2    /* a32Mult1 = a32Mult1 + a32Val2 */
                        rsbs a32Mult1, a32Mult1, #0 };      /* Negation */
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "uxth %2, %0 \n\t"                  /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n\t"                  /* a32Val2 = a32Mult2.L */

                        "asrs %0, %0, #16 \n\t"             /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n\t"             /* a32Mult2 = a32Mult2.H */

                        "movs %4, %2 \n\t"                  /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n\t"              /* a32Val3 = a32Mult1.L * a32Mult2.L */
                        "lsrs %4, %4, #7 \n\t"              /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n\t"           /* Rounding */
                        "lsrs %4, %4, #8 \n\t"              /* a32Val3 >> 8 */

                        "muls %2, %2, %1 \n\t"              /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n\t"              /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n\t"              /* a32Val2 = a32Val2 + a32Val1 */

                        "lsls %3, %3, #1 \n\t"              /* a32Val2 << 1 */
                        "adds %3, %3, %4 \n\t"              /* a32Val2 = a32Val2 + a32Val3 */

                        "muls %0, %0, %1 \n\t"              /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "lsls %0, %0, #17 \n\t"             /* a32Mult1 << 17 */
                        "adds %0, %0, %3 \n\t"              /* a32Mult1 = a32Mult1 + a32Val2 */
                        "rsbs %0, %0, #0 \n\t"              /* Negation */
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax unified \n"            /* Using unified asm syntax */
                        #endif
                        "uxth %2, %0 \n"                    /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n"                    /* a32Val2 = a32Mult2.L */

                        "asrs %0, %0, #16 \n"               /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n"               /* a32Mult2 = a32Mult2.H */

                        "movs %4, %2 \n"                    /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n"                /* a32Val3 = a32Mult1.L * a32Mult2.L */
                        "lsrs %4, %4, #7 \n"                /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n"             /* Rounding */
                        "lsrs %4, %4, #8 \n"                /* a32Val3 >> 8 */

                        "muls %2, %2, %1 \n"                /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n"                /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n"                /* a32Val2 = a32Val2 + a32Val1 */

                        "lsls %3, %3, #1 \n"                /* a32Val2 << 1 */
                        "adds %3, %3, %4 \n"                /* a32Val2 = a32Val2 + a32Val3 */

                        "muls %0, %0, %1 \n"                /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "lsls %0, %0, #17 \n"               /* a32Mult1 << 17 */
                        "adds %0, %0, %3 \n"                /* a32Mult1 = a32Mult1 + a32Val2 */
                        "rsbs %0, %0, #0 \n"                /* Negation */
                        #if defined(__GNUC__)               /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #endif

    return a32Mult1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* Output = a32Mult1 * a32Mult2
* With saturation
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline acc32_t MLIB_MulNegRndSat_A32_FAsmi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Val1=0, a32Val2=0, a32Val3=0;
    #if defined(__CC_ARM)                                        /* For ARM Compiler */
        __asm volatile{ uxth a32Val1, a32Mult1                   /* a32Val1 = a32Mult1.L */
                        uxth a32Val2, a32Mult2                   /* a32Val2 = a32Mult2.L */
                        asrs a32Mult1, a32Mult1, #16             /* a32Mult1 = a32Mult1.H */
                        asrs a32Mult2, a32Mult2, #16             /* a32Mult2 = a32Mult2.H */
     
                        movs a32Val3, a32Val1                    /* a32Val3 = a32Mult1.L */
                        muls a32Val3, a32Val3, a32Val2           /* a32Val3 = a32Mult1.L * a32Mult2.L */
     
                        muls a32Val1, a32Val1, a32Mult2          /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        muls a32Val2, a32Val2, a32Mult1          /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        adds a32Val2, a32Val2, a32Val1           /* a32Val2 = a32Val2 + a32Val1 */
     
                        lsrs a32Val1, a32Val3, #16               /* a32Val1 = a32Val3 >> 16 */
                        adds a32Val2, a32Val2, a32Val1           /* a32Val2 = a32Val2 + a32Val3 */
                        asrs a32Val1, a32Val2, #16               /* a32Val1 = a32Val2 >> 16 */
                        uxth a32Val2, a32Val2                    /* Clears higher 16 bits */
     
                        muls a32Mult1, a32Mult1, a32Mult2        /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        adds a32Mult1, a32Mult1, a32Val1         /* a32Mult1 = a32Mult1 + a32Val1 */
     
                        asrs a32Mult2, a32Mult1, #14             /* a32Mult2 = higher 18 bits of multiplication */
                        beq MLIB_MulNegRndSat_A32_NotSat         /* If a32Mult2 = 0, then goes to the MLIB_MulNegRndSat_A32_NotSat */
                        mvns a32Mult2, a32Mult2                  /* a32Mult2 = ~ a32Mult2 */
                        beq MLIB_MulNegRndSat_A32_NotSat         /* If a32Mult2 = 0, then goes to the MLIB_MulNegRndSat_A32_NotSat */
                        asrs a32Mult2, a32Mult1, #31             /* a32Mult2 = sign of result */
                        movs a32Mult1, #0x80                     /* a32Mult1 = 0x80 */
                        lsls a32Mult1, a32Mult1, #24             /* a32Mult1 = 0x80000000 */
                        subs a32Mult1, a32Mult1, a32Mult2        /* a32Mult1 = 0x80000000 - Sign of result */
                        b MLIB_MulNegRndSat_A32_End              /* Goes to the MLIB_MulNegRndSat_A32_End*/
     
                    MLIB_MulNegRndSat_A32_NotSat:     
                        lsls a32Mult1, a32Mult1, #16             /* a32Mult1 << 16 */
                        adds a32Mult1, a32Mult1, a32Val2         /* a32Mult1 = a32Mult1 + a32Val2 */
                        lsls a32Mult1, a32Mult1, #1              /* Result << 1 */
                        uxth a32Val3, a32Val3                    /* Clears higher 16 bits */
                        lsrs a32Val3, a32Val3, #7                /* a32Val3 >> 7 */
                        adds a32Val3, a32Val3, #0x80             /* Rounding */
                        lsrs a32Val3, a32Val3, #8                /* a32Val3 >> 8 */
                        adds a32Mult1, a32Mult1, a32Val3         /* a32Mult1 = a32Mult1 + a32Val3 */
                    MLIB_MulNegRndSat_A32_End:    
                        rsbs a32Mult1, a32Mult1, #0 };           /* a32Mult1 = - a32Mult1 */
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "uxth %2, %0 \n\t"                       /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n\t"                       /* a32Val2 = a32Mult2.L */
                        "asrs %0, %0, #16 \n\t"                  /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n\t"                  /* a32Mult2 = a32Mult2.H */
   
                        "movs %4, %2 \n\t"                       /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n\t"                   /* a32Val3 = a32Mult1.L * a32Mult2.L */
   
                        "muls %2, %2, %1 \n\t"                   /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n\t"                   /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n\t"                   /* a32Val2 = a32Val2 + a32Val1 */
   
                        "lsrs %2, %4, #16 \n\t"                  /* a32Val1 = a32Val3 >> 16 */
                        "adds %3, %3, %2 \n\t"                   /* a32Val2 = a32Val2 + a32Val3 */
                        "asrs %2, %3, #16 \n\t"                  /* a32Val1 = a32Val2 >> 16 */
                        "uxth %3, %3 \n\t"                       /* Clears higher 16 bits */
   
                        "muls %0, %0, %1 \n\t"                   /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "adds %0, %0, %2 \n\t"                   /* a32Mult1 = a32Mult1 + a32Val1 */
   
                        "asrs %1, %0, #14 \n\t"                  /* a32Mult2 = higher 18 bits of multiplication */
                        "beq MLIB_MulNegRndSat_A32_NotSat1%= \n\t" /* If a32Mult2 = 0, then goes to the MLIB_MulNegRndSat_A32_NotSat */
                        "mvns %1, %1 \n\t"                       /* a32Mult2 = ~ a32Mult2 */
                        "beq MLIB_MulNegRndSat_A32_NotSat1%= \n\t" /* If a32Mult2 = 0, then goes to the MLIB_MulNegRndSat_A32_NotSat */
                        "asrs %1, %0, #31 \n\t"                  /* a32Mult2 = sign of result */
                        "movs %0, #0x80 \n\t"                    /* a32Mult1 = 0x80 */
                        "lsls %0, %0, #24 \n\t"                  /* a32Mult1 = 0x80000000 */
                        "subs %0, %0, %1 \n\t"                   /* a32Mult1 = 0x80000000 - Sign of result */
                        "b MLIB_MulNegRndSat_A32_End1%= \n\t"    /* Goes to the MLIB_MulNegRndSat_A32_End*/

                    "MLIB_MulNegRndSat_A32_NotSat1%=: \n\t"
                        "lsls %0, %0, #16 \n\t"                  /* a32Mult1 << 16 */
                        "adds %0, %0, %3 \n\t"                   /* a32Mult1 = a32Mult1 + a32Val2 */
                        "lsls %0, %0, #1 \n\t"                   /* Result << 1 */
                        "uxth %4, %4 \n\t"                       /* Clears higher 16 bits */
                        "lsrs %4, %4, #7 \n\t"                   /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n\t"                /* Rounding */
                        "lsrs %4, %4, #8 \n\t"                   /* a32Val3 >> 8 */
                        "adds %0, %0, %4 \n\t"                   /* a32Mult1 = a32Mult1 + a32Val3 */

                    "MLIB_MulNegRndSat_A32_End1%=: \n\t"
                        "rsbs %0, %0, #0 \n\t"                   /* a32Mult1 = - a32Mult1 */
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #elif defined(__GNUC__)
        __asm volatile(
                        #if defined(__GNUC__)                    /* For GCC compiler */
                            ".syntax unified \n"                 /* Using unified asm syntax */
                        #endif     
                        "uxth %2, %0 \n"                         /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n"                         /* a32Val2 = a32Mult2.L */
                        "asrs %0, %0, #16 \n"                    /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n"                    /* a32Mult2 = a32Mult2.H */
     
                        "movs %4, %2 \n"                         /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n"                     /* a32Val3 = a32Mult1.L * a32Mult2.L */
     
                        "muls %2, %2, %1 \n"                     /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n"                     /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n"                     /* a32Val2 = a32Val2 + a32Val1 */
     
                        "lsrs %2, %4, #16 \n"                    /* a32Val1 = a32Val3 >> 16 */
                        "adds %3, %3, %2 \n"                     /* a32Val2 = a32Val2 + a32Val3 */
                        "asrs %2, %3, #16 \n"                    /* a32Val1 = a32Val2 >> 16 */
                        "uxth %3, %3 \n"                         /* Clears higher 16 bits */
     
                        "muls %0, %0, %1 \n"                     /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "adds %0, %0, %2 \n"                     /* a32Mult1 = a32Mult1 + a32Val1 */
     
                        "asrs %1, %0, #14 \n"                    /* a32Mult2 = higher 18 bits of multiplication */
                        "beq MLIB_MulNegRndSat_A32_NotSat%= \n"    /* If a32Mult2 = 0, then goes to the MLIB_MulNegRndSat_A32_NotSat */
                        "mvns %1, %1 \n"                         /* a32Mult2 = ~ a32Mult2 */
                        "beq MLIB_MulNegRndSat_A32_NotSat%= \n"    /* If a32Mult2 = 0, then goes to the MLIB_MulNegRndSat_A32_NotSat */
                        "asrs %1, %0, #31 \n"                    /* a32Mult2 = sign of result */
                        "movs %0, #0x80 \n"                      /* a32Mult1 = 0x80 */
                        "lsls %0, %0, #24 \n"                    /* a32Mult1 = 0x80000000 */
                        "subs %0, %0, %1 \n"                     /* a32Mult1 = 0x80000000 - Sign of result */
                        "b MLIB_MulNegRndSat_A32_End%= \n"         /* Goes to the MLIB_MulNegRndSat_A32_End*/
     
                    "MLIB_MulNegRndSat_A32_NotSat%=: \n"     
                        "lsls %0, %0, #16 \n"                    /* a32Mult1 << 16 */
                        "adds %0, %0, %3 \n"                     /* a32Mult1 = a32Mult1 + a32Val2 */
                        "lsls %0, %0, #1 \n"                     /* Result << 1 */
                        "uxth %4, %4 \n"                         /* Clears higher 16 bits */
                        "lsrs %4, %4, #7 \n"                     /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n"                  /* Rounding */
                        "lsrs %4, %4, #8 \n"                     /* a32Val3 >> 8 */
                        "adds %0, %0, %4 \n"                     /* a32Mult1 = a32Mult1 + a32Val3 */
     
                    "MLIB_MulNegRndSat_A32_End%=: \n"     
                        "rsbs %0, %0, #0 \n"                     /* a32Mult1 = - a32Mult1 */
                        #if defined(__GNUC__)                    /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)                    /* For GCC compiler */
                            ".syntax unified \n"                 /* Using unified asm syntax */
                        #endif     
                        "uxth %2, %0 \n"                         /* a32Val1 = a32Mult1.L */
                        "uxth %3, %1 \n"                         /* a32Val2 = a32Mult2.L */
                        "asrs %0, %0, #16 \n"                    /* a32Mult1 = a32Mult1.H */
                        "asrs %1, %1, #16 \n"                    /* a32Mult2 = a32Mult2.H */
     
                        "movs %4, %2 \n"                         /* a32Val3 = a32Mult1.L */
                        "muls %4, %4, %3 \n"                     /* a32Val3 = a32Mult1.L * a32Mult2.L */
     
                        "muls %2, %2, %1 \n"                     /* a32Val1 = a32Mult1.L * a32Mult2.H */
                        "muls %3, %3, %0 \n"                     /* a32Val2 = a32Mult2.L * a32Mult1.H */
                        "adds %3, %3, %2 \n"                     /* a32Val2 = a32Val2 + a32Val1 */
     
                        "lsrs %2, %4, #16 \n"                    /* a32Val1 = a32Val3 >> 16 */
                        "adds %3, %3, %2 \n"                     /* a32Val2 = a32Val2 + a32Val3 */
                        "asrs %2, %3, #16 \n"                    /* a32Val1 = a32Val2 >> 16 */
                        "uxth %3, %3 \n"                         /* Clears higher 16 bits */
     
                        "muls %0, %0, %1 \n"                     /* a32Mult1 = a32Mult1.H * a32Mult2.H */
                        "adds %0, %0, %2 \n"                     /* a32Mult1 = a32Mult1 + a32Val1 */
     
                        "asrs %1, %0, #14 \n"                    /* a32Mult2 = higher 18 bits of multiplication */
                        "beq MLIB_MulNegRndSat_A32_NotSat \n"    /* If a32Mult2 = 0, then goes to the MLIB_MulNegRndSat_A32_NotSat */
                        "mvns %1, %1 \n"                         /* a32Mult2 = ~ a32Mult2 */
                        "beq MLIB_MulNegRndSat_A32_NotSat \n"    /* If a32Mult2 = 0, then goes to the MLIB_MulNegRndSat_A32_NotSat */
                        "asrs %1, %0, #31 \n"                    /* a32Mult2 = sign of result */
                        "movs %0, #0x80 \n"                      /* a32Mult1 = 0x80 */
                        "lsls %0, %0, #24 \n"                    /* a32Mult1 = 0x80000000 */
                        "subs %0, %0, %1 \n"                     /* a32Mult1 = 0x80000000 - Sign of result */
                        "b MLIB_MulNegRndSat_A32_End \n"         /* Goes to the MLIB_MulNegRndSat_A32_End*/
     
                    "MLIB_MulNegRndSat_A32_NotSat: \n"     
                        "lsls %0, %0, #16 \n"                    /* a32Mult1 << 16 */
                        "adds %0, %0, %3 \n"                     /* a32Mult1 = a32Mult1 + a32Val2 */
                        "lsls %0, %0, #1 \n"                     /* Result << 1 */
                        "uxth %4, %4 \n"                         /* Clears higher 16 bits */
                        "lsrs %4, %4, #7 \n"                     /* a32Val3 >> 7 */
                        "adds %4, %4, #0x80 \n"                  /* Rounding */
                        "lsrs %4, %4, #8 \n"                     /* a32Val3 >> 8 */
                        "adds %0, %0, %4 \n"                     /* a32Mult1 = a32Mult1 + a32Val3 */
     
                    "MLIB_MulNegRndSat_A32_End: \n"     
                        "rsbs %0, %0, #0 \n"                     /* a32Mult1 = - a32Mult1 */
                        #if defined(__GNUC__)                    /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(a32Mult1), "+l"(a32Mult2), "+l"(a32Val1), "+l"(a32Val2), "+l"(a32Val3):);
    #endif

    return a32Mult1;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

/***************************************************************************//*!
*
* f16Out = -a32Accum * f16Mult
* With saturation
*******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_suppress=Pe549                /* Suppresses the Pe549 warning for IAR compiler*/
#endif
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET
static inline frac16_t MLIB_MulNegRndSat_F16as_FAsmi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    register frac32_t f32Val=0;
    #if defined(__CC_ARM)                                          /* For ARM Compiler */
        __asm volatile{ sxth f16Mult, f16Mult                      /* Converts 16-bit input to 32-bit */
                        uxth f32Val, a32Accum                      /* f32Val = a32Accum.L */
                        asrs a32Accum, a32Accum, #16               /* a32Accum = a32Accum.H */
       
                        muls f32Val, f32Val, f16Mult               /* f32Val = a32Accum.L * f16Mult */
                        asrs f32Val, f32Val, #7                    /* f32Val >> 7 */
                        adds f32Val, f32Val, #128                  /* Rounding */
                        asrs f32Val, f32Val, #8                    /* f32Val >> 8 */
       
                        muls f16Mult, f16Mult, a32Accum            /* f16Mult = a32Accum.H * f16Mult */
                        lsls f16Mult, f16Mult, #1                  /* f16Mult << 1 */
                        adds f16Mult, f16Mult, f32Val              /* f16Mult = f16Mult + f32Val */
       
                        rsbs f16Mult, f16Mult, #0                  /* Negation */
       
                        movs f32Val, #128                          /* f32Val = 0x80 */
                        lsls f32Val, #8                            /* f32Val = 0x8000 */
       
                        cmp f32Val, f16Mult                        /* Compares f16Mult with 0x8000 */
                        bge NegTest                                /* If f16Mult < 0x8000, then goes to NegTest */
                        subs f16Mult, f32Val, #1                   /* If f16Mult >= 0x8000, then f16Mult = 0x7FFF */
                        b SatEnd                                   /* Goes to SatEnd */
                    NegTest:       
                        sxth f32Val, f32Val                        /* f32Val = 0xFFFF8000 */
                        cmp f32Val, f16Mult                        /* Compares f16Mult with 0xFFFF8000 */
                        ble SatEnd                                 /* If f16Mult >= 0xFFFF8000, then goes to SatEnd */
                        mov f16Mult, f32Val                        /* If f16Mult < 0xFFFF8000, then f16Mult = 0xFFFF8000 */
                    SatEnd: };
    #elif defined(__GNUC__) && defined(__ARMCC_VERSION) 
        __asm volatile(
                        "sxth %0, %0 \n\t"                         /* Converts 16-bit input to 32-bit */
                        "uxth %2, %1 \n\t"                         /* f32Val = a32Accum.L */
                        "asrs %1, %1, #16 \n\t"                    /* a32Accum = a32Accum.H */
     
                        "muls %2, %2, %0 \n\t"                     /* f32Val = a32Accum.L * f16Mult */
                        "asrs %2, %2, #7 \n\t"                     /* f32Val >> 7 */
                        "adds %2, %2, #128 \n\t"                   /* Rounding */
                        "asrs %2, %2, #8 \n\t"                     /* f32Val >> 8 */
     
                        "muls %0, %0, %1 \n\t"                     /* f16Mult = a32Accum.H * f16Mult */
                        "lsls %0, %0, #1 \n\t"                     /* f16Mult << 1 */
                        "adds %0, %0, %2 \n\t"                     /* f16Mult = f16Mult + f32Val */
     
                        "rsbs %0, %0, #0 \n\t"                     /* Negation */
     
                        "movs %2, #128 \n\t"                       /* f32Val = 0x80 */
                        "lsls %2, %2, #8 \n\t"                     /* f32Val = 0x8000 */
     
                        "cmp %2, %0 \n\t"                          /* Compares f16Mult with 0x8000 */
                        "bge MLIB_MulNegRndSat_F16as_NegTest%= \n\t" /* If f16Mult < 0x8000, then jumps through two commands */
                        "subs %0, %2, #1 \n\t"                     /* If f16Mult >= 0x8000, then f16Mult = 0x7FFF */
                        "b MLIB_MulNegRndSat_F16as_SatEnd%= \n\t"  /* Jumps through four commands */
					"MLIB_MulNegRndSat_F16as_NegTest%=: \n\t"
                        "sxth %2, %2 \n\t"                         /* f32Val = 0xFFFF8000 */
                        "cmp %2, %0 \n\t"                          /* Compares f16Mult with 0xFFFF8000 */
                        "ble MLIB_MulNegRndSat_F16as_SatEnd%= \n\t"/* If f16Mult >= 0xFFFF8000, then jumps through next commands */
                        "mov %0, %2 \n\t"                          /* If f16Mult < 0xFFFF8000, then f16Mult = 0xFFFF8000 */
					"MLIB_MulNegRndSat_F16as_SatEnd%=: \n\t"
                        : "+l"(f16Mult), "+l"(a32Accum), "+l"(f32Val):);
    #else
        __asm volatile(
                        #if defined(__GNUC__)                      /* For GCC compiler */
                            ".syntax unified \n"                   /* Using unified asm syntax */
                        #endif       
                        "sxth %0, %0 \n"                           /* Converts 16-bit input to 32-bit */
                        "uxth %2, %1 \n"                           /* f32Val = a32Accum.L */
                        "asrs %1, %1, #16 \n"                      /* a32Accum = a32Accum.H */
       
                        "muls %2, %2, %0 \n"                       /* f32Val = a32Accum.L * f16Mult */
                        "asrs %2, %2, #7 \n"                       /* f32Val >> 7 */
                        "adds %2, %2, #128 \n"                     /* Rounding */
                        "asrs %2, %2, #8 \n"                       /* f32Val >> 8 */
       
                        "muls %0, %0, %1 \n"                       /* f16Mult = a32Accum.H * f16Mult */
                        "lsls %0, %0, #1 \n"                       /* f16Mult << 1 */
                        "adds %0, %0, %2 \n"                       /* f16Mult = f16Mult + f32Val */
       
                        "rsbs %0, %0, #0 \n"                       /* Negation */
       
                        "movs %2, #128 \n"                         /* f32Val = 0x80 */
                        "lsls %2, %2, #8 \n"                       /* f32Val = 0x8000 */
       
                        "cmp %2, %0 \n"                            /* Compares f16Mult with 0x8000 */
                        "bge .+6 \n"                               /* If f16Mult < 0x8000, then jumps through two commands */
                        "subs %0, %2, #1 \n"                       /* If f16Mult >= 0x8000, then f16Mult = 0x7FFF */
                        "b .+10 \n"                                /* Jumps through four commands */
       
                        "sxth %2, %2 \n"                           /* f32Val = 0xFFFF8000 */
                        "cmp %2, %0 \n"                            /* Compares f16Mult with 0xFFFF8000 */
                        "ble .+4 \n"                               /* If f16Mult >= 0xFFFF8000, then jumps through next commands */
                        "mov %0, %2 \n"                            /* If f16Mult < 0xFFFF8000, then f16Mult = 0xFFFF8000 */
                        #if defined(__GNUC__)                      /* For GCC compiler */
                            ".syntax divided \n"
                        #endif
                        : "+l"(f16Mult), "+l"(a32Accum), "+l"(f32Val):);
    #endif
    return f16Mult;
}
/* inline function without any optimization (compilation issue) */ 
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler */
#pragma diag_default=Pe549
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_A32_ASM_H_ */
