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
RAM_FUNC_LIB 
RTCESL_INLINE static inline float_t MLIB_Add_FLT_FCi(register float_t fltAdd1, register float_t fltAdd2)
{
    return((float_t)(fltAdd1 + fltAdd2));
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD_FLT_H_ */
