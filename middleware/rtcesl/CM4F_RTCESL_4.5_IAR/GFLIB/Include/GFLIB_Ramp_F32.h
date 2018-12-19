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
#ifndef _GFLIB_RAMP_F32_H_
#define _GFLIB_RAMP_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "mlib.h"
#include "gflib_types.h"
  
/*******************************************************************************
* Macros 
******************************************************************************/  
#define GFLIB_RampInit_F32_Ci(f32InitVal, psParam)                            \
        GFLIB_RampInit_F32_FCi(f32InitVal, psParam)                           
#define GFLIB_Ramp_F32_Ci(f32Target, psParam)                                 \
        GFLIB_Ramp_F32_FCi(f32Target, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32RampUp;    /* Ramp-up increment */
    frac32_t f32RampDown;  /* Ramp-down increment */
    frac32_t f32State;     /* Previous ramp value */
} GFLIB_RAMP_T_F32;

/***************************************************************************//*!
* @brief  The function initializes the actual value of Ramp_F32.
*
* @param  ptr   GFLIB_RAMP_T_F32 *psParam
*                     - rampUp: Ramp-up increment
*                     - rampDown: Ramp-down increment
*                     - state: Actual state
*                     
* @param  in    frac32_t f32InitVal - Initial value
*******************************************************************************/
static inline void GFLIB_RampInit_F32_FCi(frac32_t f32InitVal, GFLIB_RAMP_T_F32 *psParam)
{
    psParam->f32State = f32InitVal;
}
 
/***************************************************************************//*!
* @brief    Ramp function
*
* @param    ptr  GFLIB_RAMP_T_F32 *psParam
*                  - rampUp: Ramp-up increment
*                  - rampDown: Ramp-down increment
*                  - state: Actual state
* @param    in   frac32_t f32Target - Desired value in <-1;1) in frac32_t
*
* @return   This function returns - frac32_t value <-1;1)
*       
* @remarks  This function ramps the value from the f32State value up/down to
*           the f32Target value using the up/down increments defined in
*           the psParam structure.
*******************************************************************************/
static inline frac32_t GFLIB_Ramp_F32_FCi(frac32_t f32Target, GFLIB_RAMP_T_F32 *psParam)
{
    register frac32_t   f32Result = 0;       /* Result of the ramp */    

    if (f32Target > psParam->f32State)       /* If desired > actual */
    {   
        /* The algorithm adds the rampUp to actual */   
        f32Result = MLIB_AddSat_F32(psParam->f32State, psParam -> f32RampUp);  
        if (f32Result > f32Target)           /* If it overshoots */
        {           
            f32Result = f32Target;           /* it uses the desired */
        }
    } 
    
    if (f32Target < psParam->f32State)  /* If desired < actual */
    {   
        /* The algorithm subtracts the rampDown from actual */
        f32Result = MLIB_SubSat_F32(psParam->f32State, psParam -> f32RampDown);         
        if (f32Result < f32Target)           /* If it undershoots */
        {           
            f32Result = f32Target;           /* it uses the desired */
        }
    }
    
    if (f32Target == psParam->f32State) 
    {
        f32Result = f32Target; 
    }
    
    psParam->f32State = f32Result;   
    return(f32Result); 
}
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_RAMP_F32_H */

