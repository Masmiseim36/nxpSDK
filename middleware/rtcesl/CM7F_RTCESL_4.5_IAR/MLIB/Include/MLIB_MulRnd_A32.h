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
#ifndef _MLIB_MULRND_A32_H_
#define _MLIB_MULRND_A32_H_

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
#define MLIB_MulRnd_F16as_Ci(a32Accum, f16Mult)    MLIB_MulRnd_F16as_FCi(a32Accum, f16Mult)
#define MLIB_MulNegRnd_F16as_Ci(a32Accum, f16Mult) MLIB_MulNegRnd_F16as_FCi(a32Accum, f16Mult)
#define MLIB_MulRnd_A32_Ci(a32Mult1, a32Mult2)     MLIB_MulRnd_A32_FCi(a32Mult1, a32Mult2)
#define MLIB_MulNegRnd_A32_Ci(a32Mult1, a32Mult2)  MLIB_MulNegRnd_A32_FCi(a32Mult1, a32Mult2)

/***************************************************************************//*!
*
* f16Out = a32Accum * f16Mult
* Without saturation
*******************************************************************************/
static inline frac16_t MLIB_MulRnd_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{                                                                              
    return(frac16_t)((((frac64_t)a32Accum*f16Mult)+0x4000)>>15);
}
 
/***************************************************************************//*!
*
* f16Out = a32Accum * -f16Mult
* 
*******************************************************************************/
static inline frac16_t MLIB_MulNegRnd_F16as_FCi(register acc32_t a32Accum, register frac16_t f16Mult)
{
    return (frac16_t)((((frac64_t)a32Accum*(-f16Mult))+0x4000)>>15);
}
 
/***************************************************************************//*!
*
* a32Out = a32Mult1 * a32Mult2
* Without saturation
*******************************************************************************/
static inline acc32_t MLIB_MulRnd_A32_FCi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Temp;

    a32Temp = ((acc64_t)a32Mult1*a32Mult2 + (acc64_t)0x4000) >> 15;
    return (acc32_t)a32Temp;
}
 
/***************************************************************************//*!
*
* a32Out = - a32Mult1 * a32Mult2
* Without saturation
*******************************************************************************/
static inline acc32_t MLIB_MulNegRnd_A32_FCi(register acc32_t a32Mult1, register acc32_t a32Mult2)
{
    register acc32_t a32Temp;

    a32Temp = ((acc64_t)a32Mult1*a32Mult2 + (acc64_t)0x4000) >> 15;
    return (acc32_t)(-a32Temp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_A32_H_ */
