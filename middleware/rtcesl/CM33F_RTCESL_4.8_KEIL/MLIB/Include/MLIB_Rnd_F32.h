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
#include "MLIB_Conv_F32.h"

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
    return(frac16_t)MLIB_Conv_F16l_Ci(MLIB_Add_F32_Ci(f32Val, (frac32_t)0x00008000));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_RND_F32_H_ */
