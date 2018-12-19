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
* @brief  Subtraction
* 
*******************************************************************************/
#ifndef _MLIB_SUB_F64_H_
#define _MLIB_SUB_F64_H_

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
#define MLIB_Sub_F64_Ci(f64Min, f64Sub)    MLIB_Sub_F64_FCi(f64Min, f64Sub) 
#define MLIB_SubSat_F64_Ci(f64Min, f64Sub) MLIB_SubSat_F64_FCi(f64Min, f64Sub)  
  
/***************************************************************************//*!
*
* f64Out = f64Min - f64Sub
* Without saturation
*******************************************************************************/ 
static inline frac64_t MLIB_Sub_F64_FCi(register frac64_t f64Min, register frac64_t f64Sub)
{
    return(f64Min - f64Sub);
}
 
/***************************************************************************//*!
*
* f64Out = f64Min - f64Sub
* With saturation
*******************************************************************************/
static inline frac64_t MLIB_SubSat_F64_FCi(register frac64_t f64Min, register frac64_t f64Sub)
{
    register int64_t i64Temp;
    register int64_t i64satmin, i64satmax;

    i64Temp = MLIB_Sub_F64_FCi(f64Min, f64Sub);
    i64satmax = (~f64Min & f64Sub) & i64Temp;
    i64satmin = f64Min & (~f64Sub) & (~i64Temp);

    i64Temp = (i64satmin < 0) ? INT64_MIN : i64Temp;
    i64Temp = (i64satmax < 0) ? INT64_MAX : i64Temp;
    return(i64Temp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_F64_H_ */ 
