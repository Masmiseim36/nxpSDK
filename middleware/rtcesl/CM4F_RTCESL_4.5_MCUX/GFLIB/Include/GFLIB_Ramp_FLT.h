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

