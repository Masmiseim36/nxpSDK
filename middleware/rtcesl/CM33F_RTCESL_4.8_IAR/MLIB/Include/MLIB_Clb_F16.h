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
* @brief  Count of leading bits
* 
*******************************************************************************/
#ifndef _MLIB_CLB_F16_H_
#define _MLIB_CLB_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler   */
#define _clz(x) __CLZ(x)
#else
#define _clz(x) __builtin_clz(x)
#endif
/*******************************************************************************
* Macros 
*******************************************************************************/  
#define MLIB_Clb_U16s_Ci(f16Val) MLIB_Clb_U16s_FCi(f16Val)
  
/***************************************************************************//*!
*
* @brief  This function returns the number of left shifts needed to normalize the 16-bit input.
*
* @param  in    frac16_t f16Num
*               - Argument in [-1;1) in frac16_t
*
* @return This function returns
*     - uint16_t value
*
* @remarks 	Depending on the sign of the input value the function counts and returns the
* 			number of the left shift needed to get an equality between input value and
* 			the maximum fractional values "1" or "-1". For the input "0" returns "0".
*
****************************************************************************/
static inline uint16_t MLIB_Clb_U16s_FCi(register frac16_t f16Val)
{
    register frac16_t f16Temp;
    register uint16_t u16Temp;

    if (f16Val == 0)
    {   return 0U;
    }
    else
    {   f16Temp = (frac16_t)MLIB_AbsSat_F16_Asmi(f16Val);
        u16Temp = (uint16_t)f16Temp;
        u16Temp = (uint16_t)_clz(u16Temp) - 0x11U;
        return u16Temp;
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_CLB_F16_H_ */
