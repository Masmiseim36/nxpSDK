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
* @brief  Shift
* 
*******************************************************************************/
#ifndef _MLIB_BISHIFT_F32_H_
#define _MLIB_BISHIFT_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Shift_F32.h"
#include "MLIB_Shift_F32_Asmi.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_ShLBi_F32_Ci(f32Val, i16Sh)    MLIB_ShLBi_F32_FCi(f32Val, i16Sh)
#define MLIB_ShLBiSat_F32_Ci(f32Val, i16Sh) MLIB_ShLBiSat_F32_FCi(f32Val, i16Sh)
#define MLIB_ShRBi_F32_Ci(f32Val, i16Sh)    MLIB_ShRBi_F32_FCi(f32Val, i16Sh)
#define MLIB_ShRBiSat_F32_Ci(f32Val, i16Sh) MLIB_ShRBiSat_F32_FCi(f32Val, i16Sh)  
  
/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of i16Sh to the left. 
* If the i16Sh is negative, the input is shifted to the right. The function
* does not saturate the output.  
* 
*******************************************************************************/ 
static inline frac32_t MLIB_ShLBi_F32_FCi(register frac32_t f32Val, register int16_t i16Sh)
{
    return (i16Sh<(int16_t)0) ? MLIB_ShR_F32_Ci(f32Val, (uint16_t)(-i16Sh)) : 
                                MLIB_ShL_F32_Ci(f32Val, (uint16_t)(i16Sh));
}
 
/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of i16Sh to the left. 
* If the i16Sh is negative, the input is shifted to the right. The function
* saturates the output.  
* 
*******************************************************************************/ 
static inline frac32_t MLIB_ShLBiSat_F32_Ci(register frac32_t f32Val, register int16_t i16Sh)
{
    return (i16Sh<(int16_t)0) ? MLIB_ShR_F32_Ci(f32Val, (uint16_t)(-i16Sh)) : 
                                MLIB_ShLSat_F32_Asmi(f32Val, (uint16_t)(i16Sh));
}
 
/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of i16Sh to the right. 
* If the i16Sh is negative, the input is shifted to the left. The function
* does not saturate the output.  
* 
*******************************************************************************/  
static inline frac32_t MLIB_ShRBi_F32_FCi(register frac32_t f32Val, register int16_t i16Sh)
{
    return (i16Sh<(int16_t)0) ? MLIB_ShL_F32_Ci(f32Val, (uint16_t)(-i16Sh)) : 
                                MLIB_ShR_F32_Ci(f32Val, (uint16_t)(i16Sh));
}
 
/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of i16Sh to the right. 
* If the i16Sh is negative, the input is shifted to the left. The function
* saturates the output.  
* 
*******************************************************************************/
static inline frac32_t MLIB_ShRBiSat_F32_FCi(register frac32_t f32Val, register int16_t i16Sh)
{

    return (i16Sh<(int16_t)0) ? MLIB_ShLSat_F32_Asmi(f32Val, (uint16_t)(-i16Sh)) : 
                                MLIB_ShR_F32_Ci(f32Val, (uint16_t)(i16Sh));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_BISHIFT_F32_H_ */
