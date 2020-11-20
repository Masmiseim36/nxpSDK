/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
* @brief  Shift
* 
*******************************************************************************/
#ifndef _MLIB_BISHIFT_F16_H_
#define _MLIB_BISHIFT_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Shift_F16.h"
#include "MLIB_Shift_F16_Asmi.h"

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_ShLBi_F16_Ci(f16Val, i16Sh)    MLIB_ShLBi_F16_FCi(f16Val, i16Sh) 
#define MLIB_ShLBiSat_F16_Ci(f16Val, i16Sh) MLIB_ShLBiSat_F16_FCi(f16Val, i16Sh)   
#define MLIB_ShRBi_F16_Ci(f16Val, i16Sh)    MLIB_ShRBi_F16_FCi(f16Val, i16Sh) 
#define MLIB_ShRBiSat_F16_Ci(f16Val, i16Sh) MLIB_ShRBiSat_F16_FCi(f16Val, i16Sh) 
  
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of i16Sh to the left. 
* If the i16Sh is negative, the input is shifted to the right. The function
* does not saturate the output.  
* 
*******************************************************************************/ 
RAM_FUNC_LIB 
static inline frac16_t MLIB_ShLBi_F16_FCi(register frac16_t f16Val, register int16_t i16Sh)
{
    return (i16Sh<(int16_t)0) ? MLIB_ShR_F16_Ci(f16Val, (uint16_t)(-i16Sh)) :
                                MLIB_ShL_F16_Ci(f16Val, (uint16_t)(i16Sh));
}
 
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of i16Sh to the left. 
* If the i16Sh is negative, the input is shifted to the right. The function
* saturates the output.  
* 
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t MLIB_ShLBiSat_F16_FCi(register frac16_t f16Val, register int16_t i16Sh)
{
    return (i16Sh<(int16_t)0) ? MLIB_ShR_F16_Ci(f16Val, (uint16_t)(-i16Sh)) : 
                                MLIB_ShLSat_F16_Asmi(f16Val, (uint16_t)(i16Sh));
}
 
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of i16Sh to the right. 
* If the i16Sh is negative, the input is shifted to the left. The function
* does not saturate the output.  
* 
*******************************************************************************/ 
RAM_FUNC_LIB 
static inline frac16_t MLIB_ShRBi_F16_FCi(register frac16_t f16Val, register int16_t i16Sh)
{
    return (i16Sh<(int16_t)0) ? MLIB_ShL_F16_Ci(f16Val, (uint16_t)(-i16Sh)) : 
                                MLIB_ShR_F16_Ci(f16Val, (uint16_t)(i16Sh));
}
 
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of i16Sh to the right. 
* If the i16Sh is negative, the input is shifted to the left. The function
* saturates the output.  
* 
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t MLIB_ShRBiSat_F16_FCi(register frac16_t f16Val, register int16_t i16Sh)
{
    return (i16Sh<(int16_t)0) ? MLIB_ShLSat_F16_Asmi(f16Val, (uint16_t)(-i16Sh)) :
                                MLIB_ShR_F16_Ci(f16Val, (uint16_t)(i16Sh));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_BISHIFT_F16_H_ */
