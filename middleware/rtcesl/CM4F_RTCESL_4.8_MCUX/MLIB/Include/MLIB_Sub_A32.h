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
