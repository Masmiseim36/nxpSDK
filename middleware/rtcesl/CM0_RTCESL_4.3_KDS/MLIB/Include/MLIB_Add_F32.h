/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
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
 */

/*******************************************************************************
*
* @brief  Addition
* 
*******************************************************************************/
#ifndef _MLIB_ADD_F32_H_
#define _MLIB_ADD_F32_H_

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
#define MLIB_Add_F32_Ci(f32Add1, f32Add2)    MLIB_Add_F32_FCi(f32Add1, f32Add2) 
#define MLIB_AddSat_F32_Ci(f32Add1, f32Add2) MLIB_AddSat_F32_FCi(f32Add1, f32Add2)   

/***************************************************************************//*!
*  Addition        
*  32Out = f32Add1 + f32Add2
*  function is not saturated
*******************************************************************************/ 
static inline frac32_t MLIB_Add_F32_FCi(register frac32_t f32Add1, register frac32_t f32Add2)
{
    return((frac32_t)(f32Add1 + f32Add2));
}
/***************************************************************************//*!
*  Saturated Addition      
*  f32Out = f32Add1 + f32Add2
*******************************************************************************/
static inline frac32_t MLIB_AddSat_F32_FCi(register frac32_t f32Add1, register frac32_t f32Add2)
{
    register int32_t i32z;
    register int32_t i32SatMin;
    register int32_t i32SatMax;
 
    i32z = f32Add1 + f32Add2;    /* Addition without saturation to find saturation max. and min. */
    i32SatMax = ~(f32Add1 | f32Add2) & i32z;   /* Saturation max. from addends and addition without sat. */
    i32SatMin = (f32Add1 & f32Add2) & ~i32z;   /* Saturation min. from addends and addition without sat. */
    i32z = (i32SatMin < 0) ? INT32_MIN : i32z; /* Low limit restriction */
    i32z = (i32SatMax < 0) ? INT32_MAX : i32z; /* Hight limit restriction */
    return(i32z);       
}
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD_F32_H_ */
