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
* @brief  Subtraction of three input values from the first input
* 
*******************************************************************************/
#ifndef _MLIB_SUB4_FLT_H_
#define _MLIB_SUB4_FLT_H_

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
#define MLIB_Sub4_FLT_Ci(fltMin, fltSub1, fltSub2, fltSub3)                    \
        MLIB_Sub4_FLT_FCi(fltMin, fltSub1, fltSub2, fltSub3)  

/***************************************************************************//*!
*  Subtraction of three input values from the first input      
*  fltOut = fltMin - fltSub1 - fltSub2 - fltSub3
*  The output saturation is not implemented, thus in case 
*  the subtraction of input value is outside the (-2^128, 2^128) 
*  interval, the output value will overflow without any detection.
*******************************************************************************/  
static inline float_t MLIB_Sub4_FLT_FCi(register float_t fltMin, 
                                        register float_t fltSub1, 
                                        register float_t fltSub2, 
                                        register float_t fltSub3)
{
    return((float_t)(fltMin - fltSub1 - fltSub2 - fltSub3)); 
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_SUB4_FLT_H_ */
