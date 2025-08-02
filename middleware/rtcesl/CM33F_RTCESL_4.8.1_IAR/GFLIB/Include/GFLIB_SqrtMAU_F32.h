/*******************************************************************************
*
* Copyright 2013-2016, Freescale Semiconductor, Inc.
* Copyright 2024 NXP
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
* @brief  Square root 
* 
*******************************************************************************/

#ifndef _GFLIB_SQRTMAU_F32_H_
#define _GFLIB_SQRTMAU_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "MLIB_MAU.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SqrtMAU_F16l_Ci(f32Val)  GFLIB_SqrtMAU_F16l_FCi(f32Val)

/****************************************************************************
* Inline functions 
****************************************************************************/

/***************************************************************************//*!
* @brief  Calculates the square root of the argument by Math Accelerator Unit.
*
* @param    in   frac16_t f16Val - Argument in <0;1) in frac16_t
*
* @return   This function returns - frac16_t value <0;1)
*       
* @remarks 	This function calculates square root. The argument must be in the
*		range [0;1]. The algorithm calculates square root using MAU module.
*		If the value is negative the function returns zero value.
*
****************************************************************************/ 
static inline frac16_t GFLIB_SqrtMAU_F16l_FCi(register frac32_t f32Val)
{
    register frac32_t f32Temp;
    register uint32_t addr;

    f32Temp = MLIB_Sh1R_F32(f32Val);
    if(f32Temp > 0)
    {        
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
    
        addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, 3, RTCESL_MAU_MOPC_SQRT);
        RTCESL_MAU_REG_Q31(addr) = f32Val;
        f32Temp = RTCESL_MAU_RES3 >> 16U;
    
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif    
    }
    else
    {
        f32Temp = 0;
    }   
    return (frac16_t)f32Temp;  
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRTMAU_F32_H_ */

