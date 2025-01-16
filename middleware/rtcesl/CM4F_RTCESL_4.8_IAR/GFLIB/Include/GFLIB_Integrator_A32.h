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
* @brief  Integrator with bilinear transformation
* 
*******************************************************************************/
#ifndef _GFLIB_INTEGRATOR_A32_H_
#define _GFLIB_INTEGRATOR_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_IntegratorInit_F16_Ci(f16InitVal, psParam)                       \
        GFLIB_IntegratorInit_F16_FCi(f16InitVal, psParam)                      
#define GFLIB_Integrator_F16_Ci(f16InVal, psParam)                             \
        GFLIB_Integrator_F16_FCi(f16InVal, psParam)
    
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
  acc32_t a32Gain;          /* Integration Gain */
  frac32_t f32IAccK_1;      /* Output Value from the previous step */
  frac16_t f16InValK_1;     /* Input Value from the previous step */
} GFLIB_INTEGRATOR_T_A32;

/***************************************************************************//*!
* @brief  The function initializes the actual value of Integrator.
*
* @param  ptr GFLIB_INTEGRATOR_T *psParam - Pointer to integrator structure
*             GFLIB_INTEGRATOR_T data structure:
*                acc32 a32Gain - gain; input parameter
*                                in the following range: -65536.0 <= a32Gain < 65536.0    
*                frac32_t f32IAccK_1 - state variable; fractional part (lower long)
*                                      of integral part at step k-1;
*                                      can be modified outside of the function;
*                                      input/output parameter
*                                      in the following range: -1.0 <= f32IAccK_1 < 1.0                               
*                frac16_t f16InValK_1 - input value at the step k-1;
*                                       input/output parameter
*                                       in the following range: -1.0 <= f16InVal_1 < 1.0   
* @param  in    frac16_t f16InitVal - Initial value
*
*******************************************************************************/
static inline void GFLIB_IntegratorInit_F16_FCi(register frac16_t f16InitVal, 
                                                register GFLIB_INTEGRATOR_T_A32 *psParam)
{
    psParam->f32IAccK_1 = MLIB_Conv_F32s(f16InitVal); /* Stores the integral portion */
    psParam->f16InValK_1 = 0;                         /* Resets the previous step error */
}
 
/***************************************************************************//*!
*
* @brief   Scaled integrator with the bilinear transformation.
*
* @param  ptr GFLIB_INTEGRATOR_T *psParam - Pointer to integrator structure
*             GFLIB_INTEGRATOR_T data structure:
*               acc32 a32Gain - gain; input parameter
*                               in the following range: -65536.0 <= a32Gain < 65536.0    
*               frac32_t f32IAccK_1 - state variable; fractional part (lower long)
*                                     of integral part at step k-1;
*                                     can be modified outside of the function;
*                                     input/output parameter
*                                     in the following range: -1.0 <= f32IAccK_1 < 1.0                               
*               frac16_t f16InValK_1 - input value at the step k-1;
*                                      input/output parameter
*                                      in the following range: -1.0 <= f16InVal_1 < 1.0 
* @param   in    Frac16 f16InVal - input value
*
* @return  out   This function returns the integrated value. - frac16_t value <-1;1)
*
* @remarks Integrated input value according to equation:
* 
*          f32IAccK_1 = [(f16InVal + f16InValK_1) / 2]  * a32Gain) + f32IAccK_1
*          f16InValK_1 = f16InVal
*          output = (frac16_t)f32IAccK_1
* 
*******************************************************************************/
static inline frac16_t GFLIB_Integrator_F16_FCi(register frac16_t f16InVal, register GFLIB_INTEGRATOR_T_A32 *psParam)
{
    register acc32_t a32Temp;
    register frac32_t f32A1; 
    frac32_t f32Temp2;
   
    a32Temp = ((acc32_t)f16InVal + (acc32_t)psParam->f16InValK_1);  /* input + previous input */
    f32A1 = psParam->a32Gain * a32Temp;                             /* multiply by 0.5 because of bilinear transformation */
  
    f32Temp2 = MLIB_Add_F32(psParam->f32IAccK_1,f32A1);             /* summed with previous output*/
    psParam->f32IAccK_1  = f32Temp2;
    psParam->f16InValK_1 = f16InVal;                                /* set value of previous input for next execution */
    return(frac16_t)MLIB_Conv_F16l(f32Temp2 + 0x8000);
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_INTEGRATOR_A32_H_ */
