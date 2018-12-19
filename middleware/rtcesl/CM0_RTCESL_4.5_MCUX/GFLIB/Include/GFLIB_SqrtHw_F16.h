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
* @brief  Hardware square root
* 
*******************************************************************************/
#ifndef _GFLIB_SQRTHW_F16_H_
#define _GFLIB_SQRTHW_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "MLIB_MMDVSQ.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SqrtHw_F16_C(f16Val) GFLIB_SqrtHw_F16_FC(f16Val) 

/***************************************************************************//*!
* @brief        This function returns the square root of input value.
*
* @param[in]         f16Val      The input value.
*
* @return      The function returns the square root of the input value. The
*              return value is within the [0, 1) 32 bit fraction range.
*              If input is <= 0, then the function returns 0. 
*
*              The computations are made by hardware MMDSSQ
*
*
****************************************************************************/   
static inline frac32_t GFLIB_SqrtHw_F16_FC(frac16_t f16Val)
{
    uint16_t    u16InternalCount;
    frac32_t    f32Out;
    frac16_t    f16Out;
  
    if (f16Val <= 0)
    {
        f16Out = 0;
    }
    else
    {
        do
        {
            u16InternalCount = gu16CntMmdvsq + 1;
            gu16CntMmdvsq = u16InternalCount;
         
            RTCESL_MMDVSQ_CSR = 0x00000000;
            RTCESL_MMDVSQ_RCND = ((int32_t)f16Val)<<15; 
            while (((RTCESL_MMDVSQ_CSR)>>31) == 1);
        
            f32Out = (RTCESL_MMDVSQ_RES); 
        }
        while (u16InternalCount != gu16CntMmdvsq);
            f16Out = (frac16_t)(f32Out);
    }
    return (f16Out);
}
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRTHW_F16_H_ */

