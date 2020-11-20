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
static inline void GDFLIB_FilterExpInit_FLT_FCi(float_t fltInitVal,
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
static inline float_t GDFLIB_FilterExp_FLT_FCi(float_t fltInX,
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
