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
* @brief  Arcus tangent function based on the provided x,y coordinates as arguments
*         using division and piece-wise polynomial approximation
* 
*******************************************************************************/
#ifndef _GFLIB_ATANMAU_FLT_H_
#define _GFLIB_ATANMAU_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include "MLIB_MAU.h"

/*******************************************************************************
* Global variables
*******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/                         
#define GFLIB_AtanMAU_FLT_Ci(fltVal, u8ResReg)                                          \
        GFLIB_AtanMAU_FLT_FCi(fltVal, u8ResReg)

/****************************************************************************
* Inline functions 
****************************************************************************/              
/***************************************************************************//*!
*
* @brief    The GFLIB_Atan function computes the Atan(x)
*           using piece-wise polynomial approximation.          
*
* @param    ptr  *psParam - Pointer to the table
*
* @param    in   float_t fltVal - The input data value is 32-bit single precision 
*                                 floating-point value within the full type's range.
*                uint8_t u8ResReg - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                   Any value outside this range will trigger a CPU HardFault exception.
*
* @return   The function returns arctangent of the input as a 32-bit single
*           precision floating-point value in radians.              
*       
* @remarks  The GFLIB_Atan function computes the atan(x)/pi using MAU module.
*
*******************************************************************************/
static inline float_t GFLIB_AtanMAU_FLT_FCi(register float_t fltVal, register uint8_t u8ResReg)
{
    register uint32_t addr;
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif

    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_FLOAT, u8ResReg, RTCESL_MAU_MOPC_ATAN);
    RTCESL_MAU_REG_FLOAT(addr) = fltVal;
    return (RTCESL_MAU_REG_FLOAT((uint32_t)(rtcesl_mau_res_table[u8ResReg].res_addr))*FLOAT_PI);
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
}
        
#if defined(__cplusplus)
}
#endif
#endif /* _GFLIB_ATANMAU_FLT_H_ */
