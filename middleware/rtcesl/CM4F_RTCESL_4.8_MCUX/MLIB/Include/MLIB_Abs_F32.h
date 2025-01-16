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
* @brief  Absolute Value
* 
*******************************************************************************/
#ifndef _MLIB_ABS_F32_H_
#define _MLIB_ABS_F32_H_

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
#define MLIB_Abs_F32_Ci(f32Val)    MLIB_Abs_F32_FCi(f32Val)
  
/***************************************************************************//*!
*  Absolute value      
*  f32Out = |f32In|
*  not saturated
*******************************************************************************/
static inline frac32_t MLIB_Abs_F32_FCi(register frac32_t f32Val)
{
    return((f32Val < (frac32_t)0) ? (-f32Val) : (f32Val));
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ABS_F32_H_*/
