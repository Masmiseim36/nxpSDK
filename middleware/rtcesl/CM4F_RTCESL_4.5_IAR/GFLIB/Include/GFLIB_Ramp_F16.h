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

