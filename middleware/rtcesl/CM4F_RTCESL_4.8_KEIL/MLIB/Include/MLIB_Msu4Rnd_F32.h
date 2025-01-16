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
* @brief  Multiply subtract of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MSU4RND_F32_H_
#define _MLIB_MSU4RND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Sub_F64.h" 
#include "MLIB_Sub_F32.h"  
#include "MLIB_MulRnd_F32.h"   
  
/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Msu4Rnd_F32_Ci( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)    \
        MLIB_Msu4Rnd_F32_FCi( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2) 

/***************************************************************************//*!
*
* f32Out = (f32MinMul1 * f32MinMul2) - (f32SubMul1 * f32SubMul2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_Msu4Rnd_F32_FCi(register frac32_t f32MinMul1, register frac32_t f32MinMul2,
                                            register frac32_t f32SubMul1, register frac32_t f32SubMul2)
{
	frac64_t f64Temp;
	uint64_t u64Temp;

	f64Temp = (((int64_t)f32MinMul1 * (int64_t)f32MinMul2) -
               ((int64_t)f32SubMul1 * (int64_t)f32SubMul2) + 0x40000000);
    u64Temp = (uint64_t)f64Temp >> 31U;    /* Misra compliance signed shift using unsigned type*/
    if (f64Temp < 0)                       /* to compensate unsigned shift error with signed type need to */
    {   u64Temp -= 0x200000000U;}          /* subtract the value 2^(data type bits number - shifted value) */
    return (frac32_t)u64Temp;              /* cast back the corrected values */
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSU4RND_F32_H_ */
