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
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_F32_H_
#define _MLIB_MULRND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Neg_F32.h"  

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_MulRnd_F32ls_Ci(f32Mult1, f16Mult2)    MLIB_MulRnd_F32ls_FCi(f32Mult1, f16Mult2)
#define MLIB_MulRndSat_F32ls_Ci(f32Mult1, f16Mult2) MLIB_MulRndSat_F32ls_FCi(f32Mult1, f16Mult2)
#define MLIB_MulNegRnd_F32_Ci(f32Mult1, f32Mult2)   MLIB_MulNegRnd_F32_FCi(f32Mult1, f32Mult2)
#define MLIB_MulNegRnd_F32ls_Ci(f32Mult1, f16Mult2) MLIB_MulNegRnd_F32ls_FCi(f32Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f16Mult2
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_MulRnd_F32ls_FCi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    return((frac32_t)((((((int32_t)(f32Mult1)&0xffff)*(int32_t)(f16Mult2)) + 0x4000)>>15) + 
                       ((((int32_t)(f32Mult1)>>16)*(int32_t)(f16Mult2))<<1)));
}
/***************************************************************************//*!
*
* f32Out = f32Mult1 * f16Mult2
* With saturation
*******************************************************************************/
static inline frac32_t MLIB_MulRndSat_F32ls_FCi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    register int32_t i32Temp;
    
    i32Temp = (int32_t)(MLIB_MulRnd_F32ls_Ci(f32Mult1, f16Mult2));
    i32Temp = (i32Temp == INT32_MIN) ? INT32_MAX : i32Temp;
    return((frac32_t)(i32Temp));
}
/***************************************************************************//*!
*
* f32Out = f32Mult1 * -f32Mult2
* 
*******************************************************************************/ 
static inline frac32_t MLIB_MulNegRnd_F32_FCi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    int64_t i64Mult1; 
    int64_t i64Mult2; 
    
    i64Mult1 = (int64_t)f32Mult1;
    i64Mult2 = (int64_t)f32Mult2;
    return((frac32_t) (((i64Mult1*(-i64Mult2))+0x40000000)>>31));
}
/***************************************************************************//*!
*
* f32Out = f32Mult1 * -f16Mult2
* 
*******************************************************************************/
static inline frac32_t MLIB_MulNegRnd_F32ls_FCi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    frac32_t f32Temp1;
    f32Temp1 = MLIB_NegSat_F32_Ci((int32_t)(f16Mult2));    
   
    return((frac32_t)((((((int32_t)(f32Mult1)&0xffff)*((int32_t)(f32Temp1)))+0x4000)>>15) + 
          ((((int32_t)(f32Mult1)>>16)*((f32Temp1)))<<1)));
}
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_F32_H_ */
