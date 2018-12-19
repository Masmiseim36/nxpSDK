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
#ifndef _MLIB_SUB_A32_H_
#define _MLIB_SUB_A32_H_

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
#define MLIB_Sub_A32ss_Ci(f16Min, f16Sub)   MLIB_Sub_A32ss_FCi(f16Min, f16Sub) 
#define MLIB_Sub_A32as_Ci(a32Accum, f16Sub) MLIB_Sub_A32as_FCi(a32Accum, f16Sub)  
  
/***************************************************************************//*!
*
* a32Out = f16Min - f16Sub
* 
*******************************************************************************/   
static inline acc32_t MLIB_Sub_A32ss_FCi(register frac16_t f16Min, register frac16_t f16Sub)
{
    return((acc32_t)((acc32_t)f16Min - (acc32_t)f16Sub));              
}
 
/***************************************************************************//*!
*
* a32Out = a32Accum - f16Sub
* 
*******************************************************************************/   
static inline acc32_t MLIB_Sub_A32as_FCi(register acc32_t a32Accum, register frac16_t f16Sub)
{
    return ((acc32_t)((acc32_t)a32Accum -(acc32_t)f16Sub));             
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_A32_H_ */
