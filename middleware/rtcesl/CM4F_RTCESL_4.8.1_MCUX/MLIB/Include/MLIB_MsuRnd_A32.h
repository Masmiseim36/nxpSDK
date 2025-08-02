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
* @brief  Multiply subtract with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MSURND_A32_H_
#define _MLIB_MSURND_A32_H_

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
#define MLIB_MsuRnd_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)                   \
        MLIB_MsuRnd_A32ass_FCi(a32Accum, f16Mult1, f16Mult2)
    
/***************************************************************************//*!
*
* a32Out = a32Accum - ( f16Mult1 * f16Mult2)
*
*******************************************************************************/
static inline acc32_t MLIB_MsuRnd_A32ass_FCi(register acc32_t a32Accum, 
                                             register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register frac64_t f64Temp;
    register uint64_t u64Temp;

    u64Temp = (uint64_t)a32Accum << 15U;
    if (a32Accum < 0)                    /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x0800000000000U;}    /* subtract the value 2^(data type bits number - shifted value) */
    f64Temp = (frac64_t)u64Temp;         /* cast back the corrected values */
    f64Temp += (frac64_t)((int64_t)f16Mult1 * (int64_t)(-f16Mult2)) + 0x00004000;
    u64Temp = (uint64_t)f64Temp >> 15U;  /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                     /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x2000000000000U;}    /* subtract the value 2^(data type bits number - shifted value) */
    return (acc32_t)u64Temp;             /* cast back the corrected values */
}

 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSURND_A32_H_ */
