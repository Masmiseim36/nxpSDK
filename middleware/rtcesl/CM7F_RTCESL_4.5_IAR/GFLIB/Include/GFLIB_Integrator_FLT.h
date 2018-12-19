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
