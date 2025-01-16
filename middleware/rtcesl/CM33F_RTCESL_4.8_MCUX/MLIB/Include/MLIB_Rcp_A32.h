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
* @brief  Reciprocal value
* 
*******************************************************************************/
#ifndef _MLIB_RCP_A32_H_
#define _MLIB_RCP_A32_H_

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
#define MLIB_Rcp1_A32s_Ci(f16Denom) MLIB_Rcp1_A32s_FCi(f16Denom)  
#define MLIB_Rcp_A32s_Ci(f16Denom)  MLIB_Rcp_A32s_FCi(f16Denom)  

/***************************************************************************//*!
* @brief  16-bit input 32-output 16-bit precision reciprocal function
*
* @param  in  frac16_t f16Denom - Denominator in <-1;1) in frac16_t
* @return This function returns - acc32_t value [-65536.0;65536.0 - (2^-15)]
*       
* @remarks  This function calculates the multiplicative inverse value of
*           the fractional input:
*           result = FRAC16(1) / f16Denom. The function calculates
*           the result with 16-bit division precision.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/
static inline acc32_t MLIB_Rcp1_A32s_FCi(register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return(frac32_t)(INT32_MAX);
    }
    else
    {
        return(acc32_t)((frac32_t)0x40000000 / (frac32_t)f16Denom);
    }
}
 
/***************************************************************************//*!
* @brief  16-bit input 32-output 16-bit precision reciprocal function
*
* @param  in  frac16_t f16Denom - Denominator in <-1;1) in frac16_t
* @return This function returns - acc32_t value [-65536.0;65536.0 - (2^-15)]
*       
* @remarks  This function calculates the multiplicative inverse value of
*           the fractional input:
*           result = FRAC16(1) / f16Denom. The function calculates
*           the result with 16-bit division precision.
*           If the denominator is 0, the output is 0x7FFF FFFF.
*
*******************************************************************************/
static inline acc32_t MLIB_Rcp_A32s_FCi(register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return(frac32_t)(INT32_MAX);
    }
    else
    {
        return(acc32_t)((frac32_t)0x40000000 / (frac32_t)f16Denom);
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_RCP_A32_H_ */
