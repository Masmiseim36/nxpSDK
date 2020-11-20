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
* @brief  Ramp 
* 
*******************************************************************************/
#ifndef _GFLIB_RAMP_F16_H_
#define _GFLIB_RAMP_F16_H_

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
#define GFLIB_RampInit_F16_Ci(f16InitVal, psParam)                             \
        GFLIB_RampInit_F16_FCi(f16InitVal, psParam)                            
#define GFLIB_Ramp_F16_Ci(f16Target, psParam)                                  \
        GFLIB_Ramp_F16_FCi(f16Target, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac16_t f16RampUp;     /* Ramp-up increment */
    frac16_t f16RampDown;   /* Ramp-down increment */
    frac16_t f16State;      /* Previous ramp value */
} GFLIB_RAMP_T_F16;

/***************************************************************************//*!
* @brief  The function initializes the actual value of Ramp_F16.
*
* @param  ptr   GFLIB_RAMP_T_F16 *psParam
*                   - rampUp: Ramp-up increment
*                   - rampDown: Ramp-down increment
*                   - state: Actual state
*                 
* @param  in    frac16_t f16InitVal - Initial value
*******************************************************************************/
RAM_FUNC_LIB 
static inline void GFLIB_RampInit_F16_FCi(frac16_t f16InitVal, GFLIB_RAMP_T_F16 *psParam)
{
  psParam->f16State = f16InitVal;
}
 
/***************************************************************************//*!
* @brief    Ramp function
*
* @param    ptr   GFLIB_RAMP_T_F16 *psParam
*                  - rampUp: Ramp-up increment
*                  - rampDown: Ramp-down increment
*                  - state: Actual state
* @param    in    frac16_t f16Target - Desired value in <-1;1) in frac16_t
*
* @return   This function returns - frac16_t value <-1;1)
*       
* @remarks  This function ramps the value from the f16State value up/down to
*           the f16Target value using the up/down increments defined in
*           the psParam structure.
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t GFLIB_Ramp_F16_FCi(frac16_t f16Target, GFLIB_RAMP_T_F16 *psParam)
{
    register frac16_t   f16Result = 0;        /* Result of the ramp */    

    if (f16Target > psParam->f16State)        /* If desired > actual */
    {   
        /* The algorithm adds the rampUp to actual */ 
        f16Result = (int16_t)(uint16_t)MLIB_AddSat_F16(psParam->f16State, psParam -> f16RampUp);  
        if (f16Result > f16Target)            /* If it overshoots */
        {           
            f16Result = f16Target;            /* it uses the desired */
        }
    } 
    
    if (f16Target < psParam->f16State)   /* If desired < actual */
    {       
        /* The algorithm subtracts the rampDown from actual */
        f16Result = (int16_t)(uint16_t)MLIB_SubSat_F16(psParam->f16State, psParam -> f16RampDown);     
        if (f16Result < f16Target)            /* If it undershoots */
        {           
            f16Result = f16Target;            /* it uses the desired */
        }
    }
    
    if (f16Target == psParam->f16State)
    {
        f16Result = f16Target;    
    }
    
    psParam->f16State = f16Result;
    return(f16Result); 
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_RAMP_F16_H_*/

