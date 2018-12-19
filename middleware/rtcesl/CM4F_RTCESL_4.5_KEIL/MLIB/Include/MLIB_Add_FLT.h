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
#ifndef _MLIB_ADD_FLT_H_
#define _MLIB_ADD_FLT_H_

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
#define MLIB_Add_FLT_Ci(fltAdd1, fltAdd2) MLIB_Add_FLT_FCi(fltAdd1, fltAdd2)  

/***************************************************************************//*!
*  Addition        
*  fltOut = fltAdd1 + fltAdd2
*  The output saturation is not implemented.
*******************************************************************************/ 
static inline float_t MLIB_Add_FLT_FCi(register float_t fltAdd1, register float_t fltAdd2)
{
    return((float_t)(fltAdd1 + fltAdd2));
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD_FLT_H_ */
