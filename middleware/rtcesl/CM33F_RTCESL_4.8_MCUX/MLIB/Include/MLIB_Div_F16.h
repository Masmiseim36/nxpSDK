/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
* 
*
****************************************************************************//*!
*
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIV_F16_H_
#define _MLIB_DIV_F16_H_

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
#define MLIB_Div_F16_Ci(f16Num, f16Denom)    MLIB_Div_F16_FCi(f16Num, f16Denom) 
#define MLIB_DivSat_F16_Ci(f16Num, f16Denom) MLIB_DivSat_F16_FCi(f16Num, f16Denom)  

/***************************************************************************//*!
* @brief  16-bit inputs inputs 16-output 4-quadrant division
*
* @param  in  frac16_t f16Num  - Numerator in <-1;1) in frac16_t
*             frac16_t f16Denom- Denominator in <-1;1) in frac16_t
*                       
* @return This function returns - frac16_t value <-1;1)
*       
* @remarks  This function divides two fractional inputs:
*           result = f16Num / f16Denom.
*           The function does not saturate the output.
*           If the denominator is 0, the output is 0x7FFF or 0x8000
*           depending on the numerator's sign. For the both are 0, the output is
*           0x7FFF.
*
****************************************************************************/
static inline frac16_t MLIB_Div_F16_FCi(register frac16_t f16Num, register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return((f16Num >= 0) ? INT16_MAX : INT16_MIN);
    }
    else
    {
        return (frac16_t)(MLIB_ShL_F32_Ci(f16Num,  15U) / (frac32_t)f16Denom);

    }
}
 
/***************************************************************************//*!
* @brief  16-bit inputs inputs 16-output 4-quadrant division function with saturation
*
* @param  in  frac16_t f16Num   - Numerator in <-1;1) in frac16_t
*             frac16_t f16Denom - Denominator in <-1;1) in frac16_t
*                       
* @return This function returns - frac16_t value <-1;1)
*       
* @remarks  This function divides two fractional inputs:
*           result = f16Num / f16Denom.
*           The function saturates the output if |f16Num| > |f16Denom|
*           to 0x7FFF or 0x8000 depending on the signs. 
*
****************************************************************************/
static inline frac16_t MLIB_DivSat_F16_FCi(register frac16_t f16Num, register frac16_t f16Denom)
{
    register frac32_t f32Return;

    if (f16Denom == (frac16_t)0)
    {
        return ((f16Num >= (frac16_t)0) ? (frac16_t)0x7fff : (frac16_t)0x8000U);
    }
    else
    {
        f32Return = (frac32_t)(MLIB_ShL_F32_Ci(f16Num, 15U) / (frac32_t)f16Denom);
        return(frac16_t)(f32Return > (frac16_t)(INT16_MAX) ? (frac16_t)(INT16_MAX) : f32Return < (frac16_t)(INT16_MIN) ? (frac16_t)(INT16_MIN) : f32Return);
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIV_F16_H_ */
