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

#ifndef _GFLIB_SQRTMAU_F16_H_
#define _GFLIB_SQRTMAU_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SqrtMAU_F16_Ci(f16Val, u8ResReg) GFLIB_SqrtMAU_F16_FCi(f16Val, u8ResReg)
  
/****************************************************************************
* Inline functions 
****************************************************************************/

/***************************************************************************//*!
*
* @brief  Calculates the square root of the argument by Math Accelerator Unit.
*
* @param    ptr  GFLIB_SQRT_TABLE_T_F32 *psParam - Pointer to the polynomial table 
* @param    in   frac16_t f16Val  - Argument in <0;1) in frac16_t
*                uint8_t u8ResReg - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                   Any value outside this range will trigger a CPU HardFault exception.
*
* @return This function returns - frac16_t value <0;1)
*		
* @remarks 	This function calculates square root. The argument must be in the
*		range [0;1]. The algorithm calculates square root using MAU module.
*		If the value is negative the function returns zero value.
*
****************************************************************************/
static inline frac16_t GFLIB_SqrtMAU_F16_FCi(register frac16_t f16Val, register uint8_t u8ResReg)
{
    register frac16_t f16Temp;
    register uint32_t addr;
    
    if(f16Val > 0)
    {
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
        
        addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, u8ResReg, RTCESL_MAU_MOPC_SQRT);
        RTCESL_MAU_REG_Q15(addr) = f16Val;
        f16Temp = rtcesl_mau_res_table[u8ResReg].res_addr;        
	    
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif     
    }
    else
    {
        f16Temp = 0;
    }   
    return (frac16_t)f16Temp;  
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRTMAU_F16_H_ */
