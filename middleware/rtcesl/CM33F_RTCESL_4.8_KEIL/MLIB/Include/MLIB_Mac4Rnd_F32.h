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
* @brief  Multiply accumulate of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MAC4RND_F32_H_
#define _MLIB_MAC4RND_F32_H_

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
#define MLIB_Mac4Rnd_F32_Ci(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)    \
        MLIB_Mac4Rnd_F32_FCi(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2) 
  
/***************************************************************************//*!
*
* f32Out = (f32Add1Mul1 * f32Add1Mul2) + (f32Add2Mul1 * f32Add2Mul2)
* Without saturation
*******************************************************************************/  
static inline frac32_t MLIB_Mac4Rnd_F32_FCi(register frac32_t f32Add1Mul1, register frac32_t f32Add1Mul2, 
                                            register frac32_t f32Add2Mul1, register frac32_t f32Add2Mul2)
{
	register uint64_t u64Temp;
    register frac64_t f64Temp;
	
    f64Temp = (((int64_t)f32Add1Mul1 * (int64_t)f32Add1Mul2) +
               ((int64_t)f32Add2Mul1 * (int64_t)f32Add2Mul2) + 0x40000000);
    u64Temp = (uint64_t)f64Temp >> 31U;    /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                       /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x200000000U;}          /* subtract the value 2^(data type bits number - shifted value) */
    return (frac32_t)u64Temp;              /* cast back the corrected values */

}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4RND_F32_H_ */
