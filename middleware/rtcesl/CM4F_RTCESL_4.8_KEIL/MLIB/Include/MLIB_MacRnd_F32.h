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
* @brief  Multiply accumulate with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MACRND_F32_H_
#define _MLIB_MACRND_F32_H_

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
#define MLIB_MacRnd_F32lls_Ci(f32Accum, f32Mult1, f16Mult2)                    \
        MLIB_MacRnd_F32lls_FCi(f32Accum, f32Mult1, f16Mult2)                   

/***************************************************************************//*!
*
* f32Out = f32Accum + ( f32Mult1 * f16Mult2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_MacRnd_F32lls_FCi(register frac32_t f32Accum, 
                                              register frac32_t f32Mult1, register frac16_t f16Mult2)
{   
    register uint64_t u64Temp;
    register frac64_t f64Temp;
    
    f64Temp = (frac64_t)f32Mult1 * (frac16_t)f16Mult2 + 0x4000;
    u64Temp = (uint64_t)f64Temp >> 15U;              /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                                 /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x2000000000000U;}                /* subtract the value 2^(data type bits number - shifted value) */
    return ((frac32_t)u64Temp + (frac32_t)f32Accum); /* cast back the corrected values and add accumulator */
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MACRND_F32_H_ */
