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
#ifndef _GFLIB_SQRTMAU_FLT_H_
#define _GFLIB_SQRTMAU_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib_FP.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SqrtMAU_FLT_Ci(fltVal) GFLIB_SqrtMAU_FLT_FCi(fltVal)
    
/****************************************************************************
* Inline functions 
****************************************************************************/
  

/***************************************************************************//*!
* @brief  Calculates the square root of the argument by Math Accelerator Unit.
*
* @param    in   float_t fltVal - Argument in <0;1> in float_t
*
* @return   This function returns - float_t value <0;1>
*
* @remarks  This function calculates square root. The algorithm calculates 
*           square root using MAU module. If the value is negative 
*           the function returns zero value.
*
****************************************************************************/  
static inline float_t GFLIB_SqrtMAU_FLT_FCi(register float_t fltVal)
{
    register uint32_t addr;

    if(fltVal > 0)
    {        
        #if defined(__GNUC__)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #endif
      
        addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_FLOAT, 3, RTCESL_MAU_MOPC_SQRT);
        RTCESL_MAU_REG_FLOAT(addr) = fltVal;
        fltVal = RTCESL_MAU_REG_FLOAT((uint32_t)(RTCESL_MAU_RES3_ADDR));       
        
        #if defined(__GNUC__)
        #pragma GCC diagnostic pop
        #endif
    }
    else
    {
        fltVal = RTCESL_NAN;
    }
    return fltVal;  
}


#if defined(__cplusplus)
}
#endif

#endif  /* _GFLIB_SQRTMAU_FLT_H_*/
