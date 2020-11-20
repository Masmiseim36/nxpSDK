/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
static inline void GDFLIB_FilterMAInit_FLT_FCi(float_t fltInitVal,
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
static inline float_t GDFLIB_FilterMA_FLT_FCi(float_t fltInX,
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
