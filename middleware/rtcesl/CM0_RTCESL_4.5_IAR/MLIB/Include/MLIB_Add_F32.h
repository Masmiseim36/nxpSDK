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
