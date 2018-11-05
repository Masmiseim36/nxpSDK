/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
* @brief  Digital IIR Filter, 1st order 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR1_F32_H_
#define _GDFLIB_FILTERIIR1_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gdflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterIIR1Init_F16_Ci(psParam)                                  \
        GDFLIB_FilterIIR1Init_F16_FCi(psParam)                                 
#define GDFLIB_FilterIIR1_F16_Ci(f16InX, psParam)                              \
        GDFLIB_FilterIIR1_F16_FCi(f16InX, psParam)                             
#define GDFLIB_FilterIIR1_F16_Asm(f16InX, psParam)                             \
        GDFLIB_FilterIIR1_F16_FAsm(f16InX, psParam)
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32B0; /* B0 coefficient of an IIR1 filter */
    frac32_t f32B1; /* B1 coefficient of an IIR1 filter */
    frac32_t f32A1; /* A1 coefficient of an IIR1 filter */
} GDFLIB_FILTER_IIR1_COEFF_T_F32;

typedef struct
{
    GDFLIB_FILTER_IIR1_COEFF_T_F32 sFltCoeff; /* Sub-structure containing filter coefficients. */
    frac32_t f32FltBfrY[1]; /* Internal accumulator buffer */    
    frac16_t f16FltBfrX[1]; /* Input buffer of an IIR1 filter */
} GDFLIB_FILTER_IIR1_T_F32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GDFLIB_FilterIIR1_F16_FAsm(frac16_t f16InX, GDFLIB_FILTER_IIR1_T_F32 *psParam);

/***************************************************************************//*!
*
* @brief  The function clears buffer of 2nd order IIR filter pointed to by argument
*
* @param  ptr   GDFLIB_FILTER_IIR1_T_F16 *pudtFilter - Pointer to filter structure
*
* @return N/A 
*        
*******************************************************************************/
 static inline void GDFLIB_FilterIIR1Init_F16_FCi(GDFLIB_FILTER_IIR1_T_F32 *psParam)
 {
    psParam->f32FltBfrY[0] = (frac32_t)0;    
    psParam->f16FltBfrX[0] = (frac16_t)0;
 }
/***************************************************************************//*!
*
* @brief  16-bit input and output 1st order IIR filter
*
* @param  ptr   GDFLIB_FILTER_IIR1_T_F32 *psParam
*               - pointer to the structure of the filter
*
*               GDFLIB_FILTER_IIR1_T_F32 data structure:
*               GDFLIB_FILTER_IIR1_COEFF_T_F32    sFltCoeff   
*               - filter coefficients in frac32_t divided by 2
*                 in the following range: -1.0 <= coef < 1.0
*               - B coefficients:
*                   frac32_t  f32B0
*                   frac32_t  f32B1
*               -A coefficient, the sign is negated
*                   frac32_t  f32A1 
*               frac32_t f32FltBfrY[1] - y history buffer in frac32_t
*               frac16_t f16FltBfrX[1] - x history buffer in frac16_t 
* @param  in   frac16_t f16InX - Input signal at the step K step
*                                fractional value in the range <-1;1)
*                                                  
* @return This function filtered value - frac16_t value <-1;1)         
*
*            -----------                 
*     x(k)  |           |  y(k)           
*   --------|   IIR1    |---------        
*           |           |                 
*            -----------                  
*                                                                   
*   x(k) - input signal                    
*   y(k) - filtered output signal 
*
*   Filter equation:
*   y(n) = b0*x(n) + b1*x(n-1) - a1*y(n-1)
*                                                                      
*   All coefficients must be divided by 2, i.e. if the desired b1 coefficient
*   is 1.756 the value is then:  b1 = FRAC32(1.786 / 2.0);
*   
*   All A coefficients must be negated, i.e. if the desired a2 coefficient 
*   is 3.789, the value is then: a2 = FRAC32(-3.789 / 2.0);
*
*******************************************************************************/
static inline frac16_t GDFLIB_FilterIIR1_F16_FCi(frac16_t f16InX,
                                                 GDFLIB_FILTER_IIR1_T_F32 *psParam)
{
    register frac32_t f32Acc;
    register frac64_t f64M1;
    frac64_t f64M2;
    frac64_t f64M3;
    frac64_t f64Acc;
    /*
    * Implemented equation assuming adjusted(divided and sign-negative for A) coefficients:
    * y(k) = b0*x(k) + b1*x(k-1) + a1*y(k-1)
    *
    * Calculation steps:
    * M1 = b0 * x(k)
    * M2 = b1 * x(k-1)
    * M3 = a1 * y(k-1)
    * Acc = M1 + M2 + M3
    * y(k) = Acc * 2
    */
             
    /* M1 = b0 * x(k) */       
    f64M1 = ((frac64_t)(((((int32_t)(psParam->sFltCoeff.f32B0)&0xffff)*(int32_t)(f16InX))>>15)+
                        ((((int32_t)(psParam->sFltCoeff.f32B0)>>16)   *(int32_t)(f16InX))<<1))<<15);
                                                    
    /* M2 = b1 * x(k-1) */     
    f64M2 = ((frac64_t)(((((int32_t)(psParam->sFltCoeff.f32B1)&0xffff)*(int32_t)(psParam->f16FltBfrX[0]))>>15)+
                        ((((int32_t)(psParam->sFltCoeff.f32B1)>>16)   *(int32_t)(psParam->f16FltBfrX[0]))<<1))<<15);
    
    /* M3 = a1 * y(k-1) */
    f64M3 = ((frac64_t)psParam->sFltCoeff.f32A1 * (frac64_t)psParam->f32FltBfrY[0]) >> 16;

    /* Acc = M1 + M2 + M3, M3 is negated */
    f64Acc = f64M1 + f64M2 + f64M3 ;

    f64Acc = (frac64_t) f64Acc >> 14 ;  /*  <<1 coeff. are divided by 2, <<1 signed mult., >>16 convert */
    f64Acc = (f64Acc > (frac64_t)INT32_MAX) ? (frac64_t)INT32_MAX : f64Acc;     /* saturate f64Acc   */
    f64Acc = (f64Acc < (frac64_t)INT32_MIN) ? (frac64_t)INT32_MIN : f64Acc;     /* into frac32_t range */
    f32Acc = (frac32_t)f64Acc;

    /* Storing filter states in the buffers */
    psParam->f16FltBfrX[0] = f16InX;
    psParam->f32FltBfrY[0] = f32Acc;

    /* Returning de-scaled value of internal accumulator */
    return((frac16_t)(f32Acc >> 16));     
}  
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR1_F32_H_ */
