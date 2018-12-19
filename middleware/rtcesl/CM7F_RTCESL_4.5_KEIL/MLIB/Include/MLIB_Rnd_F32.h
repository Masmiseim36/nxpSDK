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
* @brief  Rounding
* 
*******************************************************************************/
#ifndef _MLIB_RND_F32_H_
#define _MLIB_RND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Add_F32.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Rnd_F16l_Ci(f32Val)    MLIB_Rnd_F16l_FCi(f32Val) 
  
/***************************************************************************//*!
*
* Converts 32-bit number into 16-bit with rounding
* Without saturation
*******************************************************************************/   
static inline frac16_t MLIB_Rnd_F16l_FCi(register frac32_t f32Val)
{
    return((frac16_t)(MLIB_Add_F32_Ci(f32Val, (frac32_t)0x00008000)>>16));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_RND_F32_H_ */
