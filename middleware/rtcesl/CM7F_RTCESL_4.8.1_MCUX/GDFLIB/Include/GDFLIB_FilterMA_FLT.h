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
* @brief  Moving average filter 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERMA_FLT_H_
#define _GDFLIB_FILTERMA_FLT_H_

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
#define GDFLIB_FilterMAInit_FLT_Ci(fltInitVal, psParam)                       \
        GDFLIB_FilterMAInit_FLT_FCi(fltInitVal, psParam)                      
#define GDFLIB_FilterMA_FLT_Ci(fltInX, psParam)                               \
        GDFLIB_FilterMA_FLT_FCi(fltInX, psParam)
  
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct{
    float_t  fltAcc;     /* Float filter accumulator. */
    float_t  fltLambda;  /* Float value of averaging, if defines filter window size.*/
}GDFLIB_FILTER_MA_T_FLT;


/***************************************************************************//*!
* @brief  The function initializes the actual values of FilterMA_FLT.
*
* @param  in   float_t fltInitVal - Initial value
*         ptr  GDFLIB_FILTER_MA_T_FLT *psParam - pointer to filter structure
*
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline void GDFLIB_FilterMAInit_FLT_FCi(float_t fltInitVal,
                                               GDFLIB_FILTER_MA_T_FLT *psParam)
{
    psParam->fltAcc  = (float_t)fltInitVal;
}
 
/***************************************************************************//*!
*
* @brief  The function calculates recursive form of moving average filter
*
* @param  ptr   GDFLIB_FILTER_MA_T_FLT * psParam
*               - Pointer to filter structure
*               
*               GDFLIB_FILTER_MA_T_FLT structure:                    
*               - fltAcc - accumulator of the filter
*               - fltLambda -  float value of averaging, if defines filter window size.
*
* @param  in    fltx - input signal
*
* @return This function returns - float_t value
*        
* @remarks 
*
* Function does not divide sum(k) by 2^n but multiply sum(k) by float number Coef, 
* which is not determined by power of 2. For example if filter window size 4 is 
* required filter coef must be set to 0.25F value.  
*
* Filter Equation:
* 
* sum(k)   = sum(k-1) + input
* out(k)   = sum(k) * Coef
* sum(k-1) = sum(k) - out(k)
*
****************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline float_t GDFLIB_FilterMA_FLT_FCi(float_t fltInX,
                                              GDFLIB_FILTER_MA_T_FLT *psParam)
{
    register float fltTemp;
    register float fltAcc;

    fltAcc  = MLIB_Add_FLT (psParam->fltAcc, fltInX);      
    fltTemp = MLIB_Mul_FLT (fltAcc, psParam->fltLambda);
    fltAcc  = MLIB_Sub_FLT (fltAcc, fltTemp);

    /* Store new accumulator state */
    psParam->fltAcc = fltAcc;
    return(fltTemp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERMA_FLT_H_ */
