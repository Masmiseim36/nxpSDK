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
#ifndef _MLIB_CLB_F32_H_
#define _MLIB_CLB_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Abs_F32_Asmi.h"

#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler   */
#define _clzl(x) __CLZ(x)
#else
#define _clzl(x) __builtin_clzl(x)
#endif
/*******************************************************************************
* Macros 
*******************************************************************************/
#define MLIB_Clb_U16l_Ci(f32Val) MLIB_Clb_U16l_FCi(f32Val)
  
/***************************************************************************//*!
*
* @brief  This function returns the number of left shifts needed to normalize the 16-bit input.
*
* @param  in    frac32_t f32Num
*               - Argument in [-1;1) in frac32_t
*
* @return This function returns
*     - uint16_t value
*
* @remarks 	Depending on the sign of the input value the function counts and returns the
* 			number of the left shift needed to get an equality between input value and
* 			the maximum fractional values "1" or "-1". For the input "0" returns "0".
*
****************************************************************************/
static inline uint16_t MLIB_Clb_U16l_FCi(register frac32_t f32Val)
{
    register frac32_t f32Temp;
    register uint32_t u32Temp;

    if (f32Val == 0)
    {   return 0U;
    }
    else
    {   f32Temp = (frac32_t)MLIB_AbsSat_F32_Asmi(f32Val);
        u32Temp = (uint32_t)f32Temp;
        return (uint16_t)_clzl(u32Temp) - 0x1U;
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_CLB_F32_H_ */
