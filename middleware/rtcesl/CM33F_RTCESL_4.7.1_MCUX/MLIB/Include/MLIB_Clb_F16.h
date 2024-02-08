/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
