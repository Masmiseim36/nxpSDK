/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
* @brief  Digital Float IIR Filter, 2nd order 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR2PQ_FLT_H_
#define _GDFLIB_FILTERIIR2PQ_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gdflib_types.h"
#include "MLIB_ConvSc_FLT_Asmi.h"
#include "GDFLIB_FilterIIR2_F32.h"
  
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterIIR2InitPQ_F16_C(psParam)                                 \
        GDFLIB_FilterIIR2InitPQ_F16_FCi(psParam)                               
#define GDFLIB_FilterIIR2PQ_F16_Ci(f16InX, psParam)                            \
        GDFLIB_FilterIIR2PQ_F16_FCi(f16InX, psParam)                           
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    GDFLIB_FILTER_IIR2_COEFF_T_F32 sFltCoeff;   /* Sub-structure containing filter coefficients. */    
    GDFLIB_FILTER_IIR2_COEFF_T_FLT sFltCoeffFLT;/* Sub-structure containing filter coefficients. */    
    float_t  fltFltBfrv[2];                     /* Internal accumulator buffer */     
} GDFLIB_FILTER_IIR2PQ_T_F32;

typedef struct
{
    GDFLIB_FILTER_IIR2_COEFF_T_FLT sFltCoeff;   /* Sub-structure containing filter coefficients. */    
    float_t  fltFltBfrv[2];                     /* Internal accumulator buffer */     
} GDFLIB_FILTER_IIR2PQ_T_FLT;
/****************************************************************************
* Inline functions 
****************************************************************************/

/***************************************************************************//*!
*
* @brief  The function clears buffer of 2nd order IIR filter with PowerQuad support pointed to by argument
*
* @param  ptr   GDFLIB_FILTER_IIR2PQ_T_F32 *psParam - Pointer to filter structure
*
* @return N/A
*
* @remarks This functions transfer filter coeficient from frac32 form to float type 
*          with respecting the coeffiecents scaling: all filter coef. are devided by 2
*          and a1,a2 coef. are sign inverted. It is necessary to call this function 
*          each time the filer coef. were changed to update filter the float filter coef.  
*
****************************************************************************/
static inline void GDFLIB_FilterIIR2InitPQ_F16_FCi(GDFLIB_FILTER_IIR2PQ_T_F32 *psParam)
{
    /* Coeficients relaculation from frac32 to float +  multiply by +2 or -2 make the coef. */
    psParam->sFltCoeffFLT.fltA1 = MLIB_ConvSc_FLTlf_Asmi(psParam->sFltCoeff.f32A1, -2.0f);  
    psParam->sFltCoeffFLT.fltA2 = MLIB_ConvSc_FLTlf_Asmi(psParam->sFltCoeff.f32A2, -2.0f);      
    psParam->sFltCoeffFLT.fltB0 = MLIB_ConvSc_FLTlf_Asmi(psParam->sFltCoeff.f32B0,  2.0f);      
    psParam->sFltCoeffFLT.fltB1 = MLIB_ConvSc_FLTlf_Asmi(psParam->sFltCoeff.f32B1,  2.0f);      
    psParam->sFltCoeffFLT.fltB2 = MLIB_ConvSc_FLTlf_Asmi(psParam->sFltCoeff.f32B2,  2.0f);     
    
    /* Internal states initialization */
    psParam->fltFltBfrv[0] = (float_t)0.0f;
    psParam->fltFltBfrv[1] = (float_t)0.0f;   
}

/***************************************************************************//*!
*
* @brief    The function clears the buffer of 2nd order IIR filter with PowerQuad support
*
* @param    ptr   r1 - pointer to the structure of the filter
*                      GDFLIB_FILTER_IIR2_COEFF_T_F32  sFltCoeff
*                      - filter coefficients in frac32_t divided by 2
*                        in the following range
*                        -1.0 <= f16Gain < 1.0
*                      - B coefficients:
*                          frac32_t            f32B0
*                          frac32_t            f32B1
*                          frac32_t            f32B2
*                      - (-A) coefficient, the sign is negated
*                          frac32_t            f32A1
*                          frac32_t            f32A2
* @param    in      r0 - Input signal at the step K step
*                      fractional value in the range <-1;1)
*
* @return   This function filtered value
*                   r0 - frac16_t value <-1;1)
*
* @remarks  Filter algorithm:
*
*             -----------
*      x(k)  |           |  y(k)
*    --------|    IIR2   |---------
*            |           |
*             -----------
*
*    x(k) - input signal
*    v(k) - filter internal signal
*    y(k) - filtered output signal
*
*    Filter equation:
*    v(k) = b0*x(n) + a1*v(n-1) + a2*v(n-2)
*    y(n) = b0*v(n) + b1*v(n-1) + b2*v(n-2)
*
*    All coefficients must be divided by 2, i.e. if the desired b1 coefficient
*    is 0.91363 the value is then:  b1 = FRAC32(0.91363 / 2.0);
*
*    All A coefficients must be negated, i.e. if the desired a2 coefficient
*    is 0.82727, the value is then: a2 = FRAC32(0.82727 / -2.0);
*
*******************************************************************************/
static inline frac16_t GDFLIB_FilterIIR2PQ_F16_FCi(frac16_t f16InX, GDFLIB_FILTER_IIR2PQ_T_F32 *psParam)
{
    frac16_t f16Result;
    
    /* copy filter parameters and state data to PQ registers */
    #if defined(__GNUC__) && (__ARMCC_VERSION == 0)       /* For ARMGCC compiler */
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
    RTCESL_PQ_GPREG0 = *(uint32_t*)(void *)& psParam->fltFltBfrv[0]; /* Restore state value for n step from buffer */
    RTCESL_PQ_GPREG1 = *(uint32_t*)(void *)& psParam->fltFltBfrv[1]; /* Restore state value for n-1 step from buffer */ 
    RTCESL_PQ_GPREG2 = *(uint32_t*)(void *)& psParam->sFltCoeffFLT.fltA1; /* Transfer coef A1 for IIR calculation */
    RTCESL_PQ_GPREG3 = *(uint32_t*)(void *)& psParam->sFltCoeffFLT.fltA2; /* Transfer coef A2 for IIR calculation */     
    RTCESL_PQ_GPREG4 = *(uint32_t*)(void *)& psParam->sFltCoeffFLT.fltB0; /* Transfer coef B0 for IIR calculation */
    RTCESL_PQ_GPREG5 = *(uint32_t*)(void *)& psParam->sFltCoeffFLT.fltB1; /* Transfer coef B1 for IIR calculation */ 
    RTCESL_PQ_GPREG6 = *(uint32_t*)(void *)& psParam->sFltCoeffFLT.fltB2; /* Transfer coef B2 for IIR calculation */ 
    #if defined(__GNUC__) && (__ARMCC_VERSION == 0)       /* For ARMGCC compiler */
    #pragma GCC diagnostic pop
    #endif
    
    /* move filter input do PQ coprocesor */
    RTCESL_PQ_BIQUAD0_FLT((uint32_t)f16InX);                   /* Move input value to coprocesor for calculation */
    f16Result = (frac16_t)RTCESL_PQ_READ_ADD0_FLT();  /* Move result value from coprocesor */

    /* store state data from PQ register to filter structure for next steps*/
    psParam->fltFltBfrv[0] = *(float_t*)(void *)& RTCESL_PQ_GPREG0; /* Store state value for n step to buffer */
    psParam->fltFltBfrv[1] = *(float_t*)(void *)& RTCESL_PQ_GPREG1; /* Store state value for n-1 step to buffer */    

    /* return result */
    return (f16Result);
}

#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR2PQ_FLT_H_ */
