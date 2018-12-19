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
* @brief  Multiply accumulate with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MACRND_A32_H_
#define _MLIB_MACRND_A32_H_

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
#define MLIB_MacRnd_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)                    \
        MLIB_MacRnd_A32ass_FCi(a32Accum, f16Mult1, f16Mult2)  
  
/***************************************************************************//*!
*
* a32Out = a32Accum + ( f16Mult1 * f16Mult2)
*
*******************************************************************************/  
static inline acc32_t MLIB_MacRnd_A32ass_FCi(register acc32_t a32Accum, 
                                             register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register acc32_t a32Temp;
    
    a32Temp = ((int32_t)f16Mult1 * (int32_t)f16Mult2);
    a32Temp = (a32Temp + 0x00004000) >> 15;
    return (a32Accum + a32Temp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MACRND_A32_H_ */
