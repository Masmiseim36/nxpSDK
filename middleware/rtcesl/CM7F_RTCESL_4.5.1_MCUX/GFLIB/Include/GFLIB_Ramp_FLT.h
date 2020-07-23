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
#ifndef _GFLIB_RAMP_FLT_H_
#define _GFLIB_RAMP_FLT_H_

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
#define GFLIB_RampInit_FLT_Ci(fltInitVal, psParam)                             \
        GFLIB_RampInit_FLT_FCi(fltInitVal, psParam)                            
#define GFLIB_Ramp_FLT_Ci(fltTarget, psParam)                                  \
        GFLIB_Ramp_FLT_FCi(fltTarget, psParam)

/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    float_t fltRampUp;     /* Ramp-up increment */
    float_t fltRampDown;   /* Ramp-down increment */
    float_t fltState;      /* Previous ramp value */
} GFLIB_RAMP_T_FLT;

/***************************************************************************//*!
* @brief  The function initializes the actual value of Ramp_FLT.
*
* @param  ptr  GFLIB_RAMP_T_FLT *psParam
*                - rampUp: Ramp-up increment
*                - rampDown: Ramp-down increment
*                - state: Actual state
*                    
* @param  in   float_t fltInitVal - Initial value
*******************************************************************************/
RAM_FUNC_LIB 
static inline void GFLIB_RampInit_FLT_FCi(float_t fltInitVal, GFLIB_RAMP_T_FLT *psParam)
{
    psParam->fltState = fltInitVal;
}
 
/***************************************************************************//*!
* @brief    Ramp function
*
* @param    ptr   GFLIB_RAMP_T_FLT *psParam
*                   - rampUp: Ramp-up increment
*                   - rampDown: Ramp-down increment
*                   - state: Actual state
* @param    in    float_t fltTarget - Desired value in float_t
*
* @return   This function returns - float_t value 
*       
* @remarks  This function ramps the value from the fltState value up/down to
*           the fltTarget value using the up/down increments defined in
*           the psParam structure.
*******************************************************************************/
RAM_FUNC_LIB 
static inline float_t GFLIB_Ramp_FLT_FCi(float_t fltTarget, GFLIB_RAMP_T_FLT *psParam)
{
    register float_t    fltResult;        /* Result of the ramp */    

    if (fltTarget > psParam->fltState)    /* If desired > actual */
    {   
        /* The algorithm adds the rampUp to actual */   
        fltResult = MLIB_Add_FLT(psParam->fltState, psParam -> fltRampUp);  
        if (fltResult > fltTarget)        /* If it overshoots */
        {           
            fltResult = fltTarget;        /* it uses the desired */
        }
    }   
    else if (fltTarget < psParam->fltState) /* If desired < actual */
    {       
        /* The algorithm subtracts the rampDown from actual */
        fltResult = MLIB_Sub_FLT(psParam->fltState, psParam -> fltRampDown);    
        if (fltResult < fltTarget)        /* If it undershoots */
        {           
            fltResult = fltTarget;        /* it uses the desired */
        }
    }
    else 
    {
        fltResult = fltTarget;             
    }
    psParam->fltState = fltResult;
    return(fltResult); 
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_RAMP_FLT_H */

