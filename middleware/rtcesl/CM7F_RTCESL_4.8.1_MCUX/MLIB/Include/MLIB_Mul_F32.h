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
#ifndef _MLIB_MUL_F32_H_
#define _MLIB_MUL_F32_H_

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
#define MLIB_Mul_F32_Ci(f32Mult1, f32Mult2)      MLIB_Mul_F32_FCi(f32Mult1, f32Mult2)
#define MLIB_Mul_F32ss_Ci(f16Mult1, f16Mult2)    MLIB_Mul_F32ss_FCi(f16Mult1, f16Mult2)
#define MLIB_MulNeg_F32_Ci(f32Mult1, f32Mult2)   MLIB_MulNeg_F32_FCi(f32Mult1, f32Mult2)
#define MLIB_MulNeg_F32ss_Ci(f16Mult1, f16Mult2) MLIB_MulNeg_F32ss_FCi(f16Mult1, f16Mult2)
  
/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* Without saturation
*******************************************************************************/   
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_Mul_F32_FCi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    register frac64_t f64Temp;
    register uint64_t u64Temp;
    
    f64Temp = (frac64_t)f32Mult1 * f32Mult2;
    u64Temp = (uint64_t)f64Temp >> 31U;    /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                       /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x200000000U;}          /* subtract the value 2^(data type bits number - shifted value) */
    return (frac32_t)u64Temp;              /* cast back the corrected values */
}
 
/***************************************************************************//*!
*
* f32Out = f16Mult1 * f16Mult2
* Without saturation
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_Mul_F32ss_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return(frac32_t) MLIB_Sh1L_F32_Ci((int32_t)f16Mult1 * (int32_t)f16Mult2);
}
 
/***************************************************************************//*!
*
* f32Out = f32Mult1 * -f32Mult2
* 
*******************************************************************************/  
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_MulNeg_F32_FCi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    register frac64_t f64Temp;
	register uint64_t u64Temp;

    f64Temp = (frac64_t)f32Mult1 * f32Mult2;
    u64Temp = (uint64_t)(f64Temp) >> 31U;       /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                            /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x200000000U;}               /* subtract the value 2^(data type bits number - shifted value) */
    return MLIB_Neg_F32_FCi((frac32_t)u64Temp); /* cast back the corrected values */
}
 
/***************************************************************************//*!
*
* f32Out = f16Mult1 * -f16Mult2
* 
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac32_t MLIB_MulNeg_F32ss_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return(frac32_t)(MLIB_Sh1L_F32_Ci((int32_t)f16Mult1 * (int32_t)(-f16Mult2)));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_F32_H_ */
