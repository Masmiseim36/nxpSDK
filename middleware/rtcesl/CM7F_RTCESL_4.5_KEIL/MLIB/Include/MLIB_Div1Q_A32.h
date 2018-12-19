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
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIV1Q_A32_H_
#define _MLIB_DIV1Q_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Clb_F32.h"
#include "MLIB_BiShift_F32.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define MLIB_Div1Q_A32ss_Ci(f16Num, f16Denom)  MLIB_Div1Q_A32ss_FCi(f16Num, f16Denom)
#define MLIB_Div1Q_A32ls_Ci(f32Num, f16Denom)  MLIB_Div1Q_A32ls_FCi(f32Num, f16Denom)
#define MLIB_Div1Q_A32ll_Asm(f32Num, f32Denom) MLIB_Div1Q_A32ll_FAsm(f32Num, f32Denom)

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
 
 extern acc32_t MLIB_Div1Q_A32ll_FAsm(frac32_t f32Num, frac32_t f32Denom);
  
/***************************************************************************//*!
* @brief  16-bit numerator, 16-bit denominator inputs 32-bit accumulator output
*         1-quadrant division function
*
* @param  in  frac16_t f16Num  - Numerator in <0;1) in frac16_t
*             frac16_t f16Denom- Denominator in <0;1) in frac16_t
*                       
* @return This function returns - acc32_t value [0;65536.0 - (2^-15)]
*       
* @remarks  This function divides two positive fractional inputs:
*           result = f16Num / f16Denom.
*           The output is the accumulator type where the lower
*           word is the fractional part.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/ 
static inline acc32_t MLIB_Div1Q_A32ss_FCi(register frac16_t f16Num, register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return(INT32_MAX);
    }
    else
    {
        return(acc32_t)((frac32_t)(f16Num << 15) / (frac32_t)f16Denom);
    }
}
 
/***************************************************************************//*!
* @brief  32-bit numerator, 16-bit denominator inputs 32-bit accumulator output
*         1-quadrant division function
*
* @param  in  frac32_t f32Num  - Numerator in <0;1) in frac32_t
*             frac16_t f16Denom- Denominator in <0;1) in frac16_t
*                       
* @return This function returns- acc32_t value [0;65536.0 - (2^-15)]
*       
* @remarks  This function divides two non-negative fractional inputs:
*           result = f32Num / f16Denom.
*           The output is the accumulator type where the lower
*           word is the fractional part.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/
static inline acc32_t MLIB_Div1Q_A32ls_FCi(register frac32_t f32Num, register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return(INT32_MAX);
    }
    else
    {
        return(acc32_t)((frac32_t)(f32Num / (frac32_t)f16Denom) >> 1);
    }
}   
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_DIV1Q_A32_H_ */
