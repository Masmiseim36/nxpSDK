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
* @brief  Exponential filter 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTEREXP_F32_H_
#define _GDFLIB_FILTEREXP_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gdflib_types.h"
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterExpInit_F16_Ci(f16InitVal, psParam)                       \
        GDFLIB_FilterExpInit_F16_FCi(f16InitVal, psParam)                      
#define GDFLIB_FilterExp_F16_Ci(f16InX, psParam)                               \
        GDFLIB_FilterExp_F16_FCi(f16InX, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct{
    frac32_t f32A;       /* is a filter constant <0; 1)assigned in form: 1-a */    
    frac32_t f32AccK_1;  /* filter accumulator value at step k-1 (last filter output)*/    
}GDFLIB_FILTER_EXP_T_F32;

/***************************************************************************//*!
* @brief  The function initializes the actual values of FilterExp_F16.
*
* @param  in  frac16_t f16InitVal  - Initial value
*         ptr  GDFLIB_FILTER_EXP_T_F32 *psParam - pointer to filter structure 
*
*******************************************************************************/
static inline void GDFLIB_FilterExpInit_F16_FCi(frac16_t f16InitVal,
                                                GDFLIB_FILTER_EXP_T_F32 *psParam)
{
    psParam->f32AccK_1 = ((frac32_t)f16InitVal) << 16; 
}
 
/***************************************************************************//*!
*
* @brief  The function calculates exponential filter
*
* @param  ptr   GDFLIB_FILTER_EXP_T_F32 * psParam
*               - Pointer to filter structure
*               
*               GDFLIB_FILTER_EXP_T_F32 structure:                    
*               - f32A - filter constant assign in form 1-a 
*                        (where a is exp. filter constant)
*               - f32AccK_1 - filter output value at step k-1
*
* @param  in    f16InX - input signal
*
* @return This function returns - frac16_t value
*        
* @remarks 
*
* Filter Equation:
* 
* Filter Equation:
* 
* y(k) = y(k-1) + A * (x(k) - y(k-1))
* 
* where
* x(k) is the filter input at time step k
* y(k) is the filtered output at time step k
* A    is the filter constant assign as 1-a from range (0; 1) (where a is exp. filter constant - smoothing constant).
*
****************************************************************************/
static inline frac16_t GDFLIB_FilterExp_F16_FCi(frac16_t f16InX,
                                                GDFLIB_FILTER_EXP_T_F32 *psParam)
{
    register frac32_t f32Temp;
    register frac32_t f32AccK_1;

    /* Store frequently read quantities to register */
    f32AccK_1 = psParam->f32AccK_1;
    
    /* Filter calculations  */ 
    f32Temp = MLIB_SubSat_F32(((frac32_t)f16InX)<<16, f32AccK_1);  /* calculation x(k) - y(k-1) */
    f32Temp = MLIB_MacSat_F32(f32AccK_1, psParam->f32A, f32Temp);  /* y(k-1) + A * (x(k) - y(k-1)) */
    psParam->f32AccK_1 = f32Temp;                                  /* store filter state value */
    
    return((frac16_t)(f32Temp >> 16));
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTEREXP_F32_H_ */
