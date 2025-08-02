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
* @brief  Exponential filter 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTEREXP_FLT_H_
#define _GDFLIB_FILTEREXP_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gdflib_types.h"
#include "mlib_FP.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterExpInit_FLT_Ci(fltInitVal, psParam)                       \
        GDFLIB_FilterExpInit_FLT_FCi(fltInitVal, psParam)                      
#define GDFLIB_FilterExp_FLT_Ci(fltInX, psParam)                               \
        GDFLIB_FilterExp_FLT_FCi(fltInX, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct{
    float_t fltA;       /* is a filter constant <0; 1> assigned in form: 1-a */    
    float_t fltAccK_1;  /* filter input value at step k-1 (last filter output)*/    
}GDFLIB_FILTER_EXP_T_FLT;

/***************************************************************************//*!
* @brief  The function initializes the actual values of FilterExp_FLT.
*
* @param  in  float_t fltInitVal  - Initial value
*         ptr  GDFLIB_FILTER_EXP_T_FLT *psParam - pointer to filter structure 
*
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline void GDFLIB_FilterExpInit_FLT_FCi(float_t fltInitVal,
                                                GDFLIB_FILTER_EXP_T_FLT *psParam)
{
    psParam->fltAccK_1 = fltInitVal; 
}
 
/***************************************************************************//*!
*
* @brief  The function calculates exponential filter
*
* @param  ptr   GDFLIB_FILTER_EXP_T_FLT * psParam
*               - Pointer to filter structure
*               
*               GDFLIB_FILTER_EXP_T_FLT structure:                    
*               - fltA - filter constant assign in form 1-a 
*                        (where a is exp. filter constant)
*               - fltAccK_1 - filter output value at step k-1
*
* @param  in    fltInX - input signal
*
* @return This function returns - float_t value
*        
* @remarks 
*
* Filter Equation:
* 
* y(k) = y(k-1) + A * (x(k) - y(k-1))
* 
* where
* x(k) is the filter input at time step k
* y(k) is the filtered output at time step k
* A    is the filter constant assign as 1-a from range <0; 1) (where a is exp. filter constant - smoothing constant).
*
****************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline float_t GDFLIB_FilterExp_FLT_FCi(float_t fltInX,
                                                GDFLIB_FILTER_EXP_T_FLT *psParam)
{
    register float_t fltTemp;
    register float_t fltAccK_1;

    /* Store frequently read quantities to register */
    fltAccK_1 = psParam->fltAccK_1;
    
    /* Filter calculations  */
    fltTemp  = MLIB_Sub_FLT(fltInX, fltAccK_1);                 /* calculation x(k) - y(k-1) */
    fltTemp  = MLIB_Mac_FLT(fltAccK_1, psParam->fltA, fltTemp); /* y(k-1) + A * (x(k) - y(k-1)) */
    psParam->fltAccK_1 = fltTemp ;                              /* store filter state value */
    
    return(fltTemp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTEREXP_FLT_H_ */
