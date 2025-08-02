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
* @brief  Multiply negate accumulate
* 
*******************************************************************************/
#ifndef _MLIB_MNAC_A32_H_
#define _MLIB_MNAC_A32_H_

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
#define MLIB_Mnac_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)                     \
        MLIB_Mnac_A32ass_FCi(a32Accum, f16Mult1, f16Mult2)  
  
/***************************************************************************//*!
*
* a32Out = - a32Accum + ( f16Mult1 * f16Mult2 )
*
*******************************************************************************/
static inline acc32_t MLIB_Mnac_A32ass_FCi(register acc32_t a32Accum, 
                                           register frac16_t f16Mult1,register frac16_t f16Mult2)
{   
	return (acc32_t)(MLIB_ShR_F32_Ci(((int32_t)f16Mult1 * (int32_t)f16Mult2), 15U) - a32Accum);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MNAC_A32_H_ */
