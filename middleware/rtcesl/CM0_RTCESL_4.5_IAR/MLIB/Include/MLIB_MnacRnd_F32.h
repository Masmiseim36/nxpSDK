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
* @brief  Multiply negate accumulate with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MNACRND_F32_H_
#define _MLIB_MNACRND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Add_F32.h"
#include "MLIB_MulRnd_F32.h"
#include "MLIB_Sub_F32.h"

/*******************************************************************************
* Macros
*******************************************************************************/                       
#define MLIB_MnacRnd_F32lls_Ci(f32Accum, f32Mult1, f16Mult2)                    \
        MLIB_MnacRnd_F32lls_FCi(f32Accum, f32Mult1, f16Mult2)                   

/***************************************************************************//*!
*
* f32Out = - f32Accum + ( f32Mult1 * f16Mult2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_MnacRnd_F32lls_FCi(register frac32_t f32Accum, 
                                               register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    return(MLIB_Sub_F32_Ci(MLIB_MulRnd_F32ls_Ci(f32Mult1,f16Mult2), f32Accum));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MNACRND_F32_H_ */
