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
* @brief  Integrator with bilinear transformation
* 
*******************************************************************************/
#ifndef _GFLIB_INTEGRATOR_FLT_H_
#define _GFLIB_INTEGRATOR_FLT_H_

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
#define GFLIB_IntegratorInit_FLT_Ci(fltInitVal, psParam)                      \
        GFLIB_IntegratorInit_FLT_FCi(fltInitVal, psParam)
#define GFLIB_Integrator_FLT_Ci(fltInVal, psParam)                            \
        GFLIB_Integrator_FLT_FCi(fltInVal, psParam)
    
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t fltGain;         /* Integration Gain */
    float_t fltIAccK_1;      /* Output Value from the previous step */
    float_t fltInValK_1;     /* Input Value from the previous step */
    float_t fltUpperLim;     /* Upper Limit */ 
    float_t fltLowerLim;     /* Lower Limit */
} GFLIB_INTEGRATOR_T_FLT;

/***************************************************************************//*!
* @brief  The function initializes the actual value of float Integrator.
*
* @param  ptr   GFLIB_INTEGRATOR_T *psParam -  Pointer to integrator structure
*               GFLIB_INTEGRATOR_T data structure:
*                     float_t gain fltGain - gain; input parameter
*                           in the float range   
*                     float_t fltIAccK_1 - state variable; fractional part 
*                           (lower long) of integral part at step k-1;
*                           can be modified outside of the function;
*                           input/output parameter
*                           in the float range                               
*                     float_t fltInValK_1 - input value at the step k-1;
*                           input/output parameter in the float range 
*                     float_t fltUpperLim - upper limit
*                     float_t fltLowerLim - lower limit                    
* @param  in    float_t fltInitVal - Initial value
*
*******************************************************************************/
RAM_FUNC_LIB 
static inline void GFLIB_IntegratorInit_FLT_FCi(register float_t fltInitVal,
                                                register GFLIB_INTEGRATOR_T_FLT *psParam)
{
    psParam->fltIAccK_1 = fltInitVal;     /* Stores the integral portion */
    psParam->fltInValK_1 = 0.0F;          /* Resets the previous step error */
}
 
/***************************************************************************//*!
* @brief   Float integrator with the bilinear transformation.
*          
* @brief   The function initializes the actual value of Integrator.
*
* @param  ptr   GFLIB_INTEGRATOR_T *psParam -  Pointer to integrator structure
*               GFLIB_INTEGRATOR_T data structure:
*                     float_t gain fltGain - gain; input parameter
*                           in the float range   
*                     float_t fltIAccK_1 - state variable; fractional part 
*                           (lower long) of integral part at step k-1;
*                           can be modified outside of the function;
*                           input/output parameter
*                           in the float range                               
*                     float_t fltInValK_1 - input value at the step k-1;
*                           input/output parameter in the float range 
*                     float_t fltUpperLim - upper limit
*                     float_t fltLowerLim - lower limit
* @param   in   float_t fltInitVal - Initial value
*
* @return  out  This function returns the integrated value - float_t value
*       
* @remarks Integrated input value according to equation:    
* 
*          fltIAccK_1 = [(fltInVal + fltInValK_1) / 2]  * fltGain) + fltIAccK_1
*          fltInValK_1 = fltInVal
*          output = fltIAccK_1
* 
*******************************************************************************/
RAM_FUNC_LIB 
static inline float_t GFLIB_Integrator_FLT_FCi(register float_t fltInVal, 
                                               register GFLIB_INTEGRATOR_T_FLT *psParam)
{   
    register float_t fltTemp;
    register float_t fltTemp1;
    
    /* Move frequently used quantities to register */
    fltTemp1 = psParam->fltIAccK_1;
    
    fltTemp = MLIB_Add_FLT (fltInVal, psParam->fltInValK_1); /* input + previous input */
    fltTemp = MLIB_Mul_FLT (fltTemp, 0.5F);                  /* multiply by 0.5 because of bilinear transformation */

    fltTemp = MLIB_Mac_FLT (fltTemp1, psParam->fltGain, fltTemp);      /* multiply a32Gain with  inputs */                                                                                                         
    psParam->fltIAccK_1  = fltTemp;                          /* store actual value to accumulator */ 
    psParam->fltInValK_1 = fltInVal;                         /* set value of previous input for next execution */
    if(fltTemp > psParam->fltUpperLim) 
    {
        return(psParam->fltUpperLim);
    }
    if(fltTemp < psParam->fltLowerLim) 
    {
        return(psParam->fltLowerLim);
    }
    
    return (fltTemp);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_INTEGRATOR_FLT_H_ */
