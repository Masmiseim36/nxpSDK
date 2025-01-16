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
* @brief  Conversion functions
* 
*******************************************************************************/
#ifndef _MLIB_CONV_F32_H_
#define _MLIB_CONV_F32_H_

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
#define MLIB_Conv_F32s_Ci(f16Val) MLIB_Conv_F32s_FCi(f16Val)
#define MLIB_Conv_F16l_Ci(f32Val) MLIB_Conv_F16l_FCi(f32Val)  

/***************************************************************************//*!
*
* @brief  16-bit fractional to 32-bit fractional convert function
*
* @param  in  frac16_t f16Val - Argument in <-1;1) in frac16_t
*
* @return This function returns - frac32_t value <-1;1)
*       
* @remarks  This function convert the lower 16-bits of the 32-bit input and
*           returns the upper 16-bit. 
*
*******************************************************************************/
static inline frac32_t MLIB_Conv_F32s_FCi(register frac16_t f16Val)
{
    register uint32_t ui32temp = (uint32_t)f16Val << 16U;
    return (frac32_t)ui32temp;
}
 
/***************************************************************************//*!
*
* @brief  32-bit fractional to 16-bit fractional convert function
*
* @param  in  frac32_t f32Val - Argument in <-1;1) in frac32_t
*
* @return This function returns - frac16_t value <-1;1)
*       
* @remarks  This function convert the lower 16-bits of the 32-bit input and
*           returns the upper 16-bit. 
*
*******************************************************************************/
static inline frac16_t MLIB_Conv_F16l_FCi(register frac32_t f32Val)
{
	register uint16_t u16Temp = (uint16_t)((uint32_t)f32Val >> 16U);
    return (frac16_t)u16Temp;
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_CONV_F32_H_ */
