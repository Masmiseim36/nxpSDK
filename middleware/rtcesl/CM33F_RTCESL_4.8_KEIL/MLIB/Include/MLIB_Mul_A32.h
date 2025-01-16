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
* @brief  Multiply 
* 
*******************************************************************************/
#ifndef _MLIB_MUL_A32_H_
#define _MLIB_MUL_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Shift_F32.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Mul_F16as_Ci(a32Accum, f16Mult)    MLIB_Mul_F16as_FCi(a32Accum, f16Mult)
#define MLIB_MulNeg_F16as_Ci(a32Accum, f16Mult) MLIB_MulNeg_F16as_FCi(a32Accum, f16Mult)
#define MLIB_Mul_A32_Ci(a32Mult1, a32Mult2)     MLIB_Mul_A32_FCi( a32Mult1, a32Mult2)
#define MLIB_MulNeg_A32_Ci(a32Mult1, a32Mult2)  MLIB_MulNeg_A32_FCi( a32Mult1, a32Mult2)

/***************************************************************************//*!
*
* f16Out = a32Accum * f16Mult
* Without saturation
*******************************************************************************/  
static inline frac16_t MLIB_Mul_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{
	register frac64_t f64Temp;
	register uint64_t u64Temp;

    f64Temp = (frac64_t)a32Accum * f16Mult;
    u64Temp = (uint64_t)f64Temp >> 15U;  /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                     /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x2000000000000U;}    /* subtract the value 2^(data type bits number - shifted value) */
    return (frac16_t)u64Temp;            /* cast back the corrected values */
}
 
/***************************************************************************//*!
*
* f16Out = a32Accum * -f16Mult

*******************************************************************************/
static inline frac16_t MLIB_MulNeg_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{
	register frac64_t f64Temp;
	register uint64_t u64Temp;

    f64Temp = (frac64_t)a32Accum * (-f16Mult);
    u64Temp = (uint64_t)f64Temp >> 15U;  /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                     /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x2000000000000U;}    /* subtract the value 2^(data type bits number - shifted value) */
    return (frac16_t)u64Temp;            /* cast back the corrected values */
}
 
/***************************************************************************//*!
*
* a32Out = a32Mult1 * a32Mult2

*******************************************************************************/
static inline acc32_t MLIB_Mul_A32_FCi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
	register frac64_t f64Temp;
	register uint64_t u64Temp;

    f64Temp = (acc64_t)a32Mult1 * a32Mult2;
    u64Temp = (uint64_t)f64Temp >> 15U;  /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                     /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x2000000000000U;}    /* subtract the value 2^(data type bits number - shifted value) */

    return (acc32_t)u64Temp;             /* cast back the corrected values */
}
 
/***************************************************************************//*!
*
* a32Out = - a32Mult1 * a32Mult2

*******************************************************************************/
static inline acc32_t MLIB_MulNeg_A32_FCi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
	register frac64_t f64Temp;
	register uint64_t u64Temp;
    
    f64Temp = (acc64_t)a32Mult1 * (-a32Mult2);
    u64Temp = (uint64_t)f64Temp >> 15U;  /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                     /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x2000000000000U;}    /* subtract the value 2^(data type bits number - shifted value) */

    return (acc32_t)u64Temp;             /* cast back the corrected values */
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_A32_H_ */
