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
* @brief  Binary logarithm
* 
*******************************************************************************/
#ifndef _MLIB_LOG2_U16_H_
#define _MLIB_LOG2_U16_H_

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
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler   */
#define _clz(x) __CLZ(x)
#else
#define _clz(x) __builtin_clz(x)
#endif

#define MLIB_Log2_U16_Ci(u16Val) MLIB_Log2_U16_FCi(u16Val)
  
/***************************************************************************//*!
*
* @brief  Binary logarithm of 16-bit input
*
* @param  in  uint16_t u16Val - Argument in uint16_t
*
* @return This function returns - uint16_t value
*       
* @remarks 	This function returns the 16-bit integer part of binary logarithm of the input.
* 			Returns 0 for input u16Val == 0.
*
*******************************************************************************/
static inline uint16_t MLIB_Log2_U16_FCi(register uint16_t u16Val)
{
	register uint16_t u16Temp;
    if ((bool_t)(u16Val == 0U))
    {   return (uint16_t)0;
    }
    else
    {
        u16Temp = 0x1FU - (uint16_t)_clz(u16Val);
        return (uint16_t)u16Temp;
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_LOG2_U16_H_ */
