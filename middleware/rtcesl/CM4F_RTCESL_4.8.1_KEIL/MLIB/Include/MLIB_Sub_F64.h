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
    register uint64_t u64satmin, u64satmax;

    i64Temp = MLIB_Sub_F64_FCi(f64Min, f64Sub);
    u64satmax = (~(uint64_t)f64Min) & (uint64_t)f64Sub;
    u64satmax = u64satmax & (uint64_t)i64Temp;
    u64satmin = (uint64_t)f64Min & (~(uint64_t)f64Sub);
    u64satmin = u64satmin & (~(uint64_t)i64Temp);

    i64Temp = ((int64_t)u64satmin < 0) ? (int64_t)0x8000000000000000U : i64Temp;
    i64Temp = ((int64_t)u64satmax < 0) ? (int64_t)0x7FFFFFFFFFFFFFFF  : i64Temp;
    return(i64Temp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_F64_H_ */ 
