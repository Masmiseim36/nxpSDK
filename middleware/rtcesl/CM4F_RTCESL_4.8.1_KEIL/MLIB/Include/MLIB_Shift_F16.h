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
#ifndef _MLIB_SHIFT_F16_H_
#define _MLIB_SHIFT_F16_H_

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
#define MLIB_Sh1L_F16_Ci(f16Val)          MLIB_Sh1L_F16_FCi(f16Val)
#define MLIB_Sh1R_F16_Ci(f16Val)          MLIB_Sh1R_F16_FCi(f16Val)  
#define MLIB_Sh1R_F16_Ci(f16Val)          MLIB_Sh1R_F16_FCi(f16Val)  
#define MLIB_ShL_F16_Ci(f16Val, u16Sh)    MLIB_ShL_F16_FCi(f16Val, u16Sh)
#define MLIB_ShR_F16_Ci(f16Val, u16Sh)    MLIB_ShR_F16_FCi(f16Val, u16Sh)  
  
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by 1 to the left. 
* The function does not saturate the output.
*              
*******************************************************************************/ 
static inline frac16_t MLIB_Sh1L_F16_FCi(register frac16_t f16Val)
{
    register uint32_t ui32temp = (uint32_t)f16Val << 1U;
    return (frac16_t)ui32temp;
}
 
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by 1 to the right. 
*              
*******************************************************************************/
static inline frac16_t MLIB_Sh1R_F16_FCi(register frac16_t f16Val)
{
    register uint32_t ui32temp = (uint32_t)f16Val >> 1U;
    return (frac16_t)ui32temp;
}
 
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of u16Sh to the left. 
* The function does not saturate the output. 
*             
*******************************************************************************/ 
static inline frac16_t MLIB_ShL_F16_FCi(register frac16_t f16Val, register uint16_t u16Sh)
{
    register uint32_t ui32temp = (uint32_t)f16Val << u16Sh;
    return (frac16_t)ui32temp;
}
 
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of u16Sh to the right.  
*             
*******************************************************************************/ 
static inline frac16_t MLIB_ShR_F16_FCi(register frac16_t f16Val, register uint16_t u16Sh)
{
    register uint32_t ui32temp = (uint32_t)f16Val >> u16Sh;
    return (frac16_t)ui32temp;
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SHIFT_F16_H_ */
