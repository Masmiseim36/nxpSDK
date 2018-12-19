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
* @brief  Digital Float IIR Filter, 1st order 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR1_FLT_H_
#define _GDFLIB_FILTERIIR1_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gdflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterIIR1Init_FLT_Ci(psParam)                                  \
        GDFLIB_FilterIIR1Init_FLT_FCi(psParam)                                 
#define GDFLIB_FilterIIR1_FLT_Ci(fltInX, psParam)                              \
        GDFLIB_FilterIIR1_FLT_FCi(fltInX, psParam)                             
#define GDFLIB_FilterIIR1_FLT_C(fltInX, psParam)                               \
        GDFLIB_FilterIIR1_FLT_FC(fltInX, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t fltB0; /* B0 coefficient of an IIR1 filter */
    float_t fltB1; /* B1 coefficient of an IIR1 filter */
    float_t fltA1; /* A1 coefficient of an IIR1 filter */
} GDFLIB_FILTER_IIR1_COEFF_T_FLT;

typedef struct
{
    GDFLIB_FILTER_IIR1_COEFF_T_FLT sFltCoeff; /* Sub-structure containing filter coeff. */
    float_t fltFltBfrY[1]; /* Internal accumulator buffer */    
    float_t fltFltBfrX[1]; /* Input buffer of an IIR1 filter */
} GDFLIB_FILTER_IIR1_T_FLT;

/***************************************************************************//*!
*
* @brief  The function clears buffer of 2nd order float IIR filter  pointed to by argument
*
* @param  ptr   GDFLIB_FILTER_IIR1_T_FLT *p- Pointer to filter structure
*
* @return N/A
*
****************************************************************************/
static inline void GDFLIB_FilterIIR1Init_FLT_FCi(GDFLIB_FILTER_IIR1_T_FLT *psParam)
{    
    psParam->fltFltBfrY[0] = (float_t)0.0F;       
    psParam->fltFltBfrX[0] = (float_t)0.0F; 
}
 
/***************************************************************************//*!
*
* @brief  The function clears the buffer of 2nd order float IIR filter 
*
* @param  ptr   GDFLIB_FILTER_IIR1_T_FLT *psParam
*               - pointer to the structure of the filter
*
*                 GDFLIB_FILTER_IIR1_T_FLT data structure:
*                 GDFLIB_FILTER_IIR1_COEFF_T_FLT  sFltCoeff   
*                 - filter coefficients in float_t as real value
*                   - B coefficients:
*                       float_t  fltB0
*                       float_t  fltB1
*                   - A coefficient, the sign is negated
*                       float_t  fltA1
*                   float_t fltFltBfrY[1] - y history buffer in float_t
*                   float_t fltFltBfrX[1] - x history buffer in float_t
* @param  in    float_t fltInX - Input signal at the step K step
*
* @return   This function filtered value - float_t value 
*
* @remarks      Filter algorithm:                                           
*                                                                       
*             -----------                 
*      x(k)  |           |  y(k)           
*    --------|    IIR1   |---------        
*            |           |                 
*             -----------                  
*                                                            
*    x(k) - input signal                    
*    y(k) - filtered output signal              
*                                                                    
*    Filter equation:
*    y(n) = b0*x(n) + b1*x(n-1) - a1*y(n-1)
*                                                                        
*    All B coefficients are not scaled, i.e. if the desired b1 coefficient
*    is 1.756 the value is then:  b1 = 1.786F ;
*    
*    All A coefficients must be negated but are not scaled, i.e. if the desired a2 coefficient 
*    is 3.789, the value is then: a2 = -3.789F ; 
*   
*******************************************************************************/
static inline float_t GDFLIB_FilterIIR1_FLT_FCi(float_t fltInX,
                                                GDFLIB_FILTER_IIR1_T_FLT *psParam)
{
    register float_t fltAcc;
    
    /*
    * Implemented equation assuming real (not devided) filter coeficients:
    * y(k) = b0*x(k) + b1*x(k-1) + a1*y(k-1)
    *
    * Calculation steps:
    * Acc =       b0 * x(k)
    * Acc = Acc + b1 * x(k-1)     
    * Acc = Acc + a1 * y(k-1)    
    * y(k)= Acc 
    */
    
    /* Acc =  b0 * x(k) */  
    fltAcc = MLIB_Mul_FLT ( psParam->sFltCoeff.fltB0, fltInX);             
    
    /* Acc = Acc + b1 * x(k-1) */ 
    fltAcc = MLIB_Mac_FLT (fltAcc, psParam->sFltCoeff.fltB1, psParam->fltFltBfrX[0]); 
    
    /* Acc = Acc + a1 * y(k-1) */   
    fltAcc = MLIB_Mac_FLT (fltAcc, psParam->sFltCoeff.fltA1, psParam->fltFltBfrY[0]);  
                     
    /* Storing filter states in the buffers */
    psParam->fltFltBfrX[0] = fltInX;
    psParam->fltFltBfrY[0] = fltAcc;
        
    /* Returning float value of internal accumulator */
    return(fltAcc);    
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR1_FLT_H_ */
