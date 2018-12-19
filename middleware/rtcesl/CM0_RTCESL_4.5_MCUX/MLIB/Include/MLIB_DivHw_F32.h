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
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIVHW_F32_H_
#define _MLIB_DIVHW_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/ 
#include "MLIB_MMDVSQ.h"
#include "MLIB_Shift_F32.h"
#include "MLIB_Shift_F16.h"  
#include "MLIB_BiShift_F32.h"
#include "MLIB_Add_F32.h"
#include "MLIB_Conv_F32.h"
#include "MLIB_Mul_F32.h"
#include "MLIB_Neg_F16.h"
#include "MLIB_Neg_F32.h"
#include "MLIB_Abs_F32.h"  
 
/*******************************************************************************
* Macros 
*******************************************************************************/    
#define MLIB_DivHw_F32_C(f32Num, f32Denom)      MLIB_DivHw_F32_FC(f32Num, f32Denom) 

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/  
extern frac32_t MLIB_DivHw_F32_FC(frac32_t f32Num, frac32_t f32Denom);

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIVHW_F16_H_ */
