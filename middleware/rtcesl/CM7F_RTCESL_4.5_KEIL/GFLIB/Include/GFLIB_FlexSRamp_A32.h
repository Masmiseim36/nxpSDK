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
* @brief  Flex s-ramp functions with 16-bit fractional output 
* 
*******************************************************************************/
#ifndef _GFLIB_FLEXSRAMP_A32_H_
#define _GFLIB_FLEXSRAMP_A32_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "gflib.h"
    
/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_FlexSRampInit_F16_C(f16InitVal, psParam)                        \
        GFLIB_FlexSRampInit_F16_FC(f16InitVal, psParam)                       
#define GFLIB_FlexSRampCalcIncr_F16_C(f16Target, a32Duration, psParam)        \
        GFLIB_FlexSRampCalcIncr_F16_FC(f16Target, a32Duration, psParam)       
#define GFLIB_FlexSRamp_F16_C(psParam)                                        \
        GFLIB_FlexSRamp_F16_FC(psParam)

/******************************************************************************
* Types
******************************************************************************/
/* Flex dynamic S ramp structure */
typedef struct
{
    frac32_t f32State;    /* state variable keeping the last state x  */
    frac32_t f32Incr;     /* derivative of x (dX) - acceleration */
    frac32_t f32AIncr;    /* derivative of a (dA) - der. of acceleration */
    frac32_t f32ADes;     /* desired acceleration - must be <= f32Incrmax */
    frac32_t f32Target;   /* target value x */
    frac32_t f32Ts;       /* sample time [s] */
    frac32_t f32IncrMax;  /* maximum increment of x */
    frac32_t f32XT1;      /* X(T1) - x at the instant where the ramp got the desired acceleration (f32Incr = const) */                           
    frac32_t f32XT2;      /* X(T2) - x at the instant where the ramp started to decelerate */
    frac16_t f16DA;       /* defined derivative of acceleration: dA = A / T */
    uint16_t u16AccState; /* state of the ramp process 0: f32Incr rising, 1: f32Incr = const, 2: f32Incr falling */
    bool_t bReachFlag;    /* flag is set to TRUE if the desired value is achieved */
} GFLIB_FLEXSRAMP_T_F32 ;
    
/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern bool_t GFLIB_FlexSRampCalcIncr_F16_C(frac16_t f16Target, acc32_t a32Duration, 
                                            GFLIB_FLEXSRAMP_T_F32 *psParam);

extern frac16_t GFLIB_FlexSRamp_F16_FC(GFLIB_FLEXSRAMP_T_F32 *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    Flex s-ramp initialization
*
* @param    in   frac16_t f16InitVal - Measured instant value in <-1;1) in frac16_t
*
* @param  ptr  GFLIB_FLEXSRAMP_T_F32 *psParam
*           - state variable keeping the last state x  
*           - derivative of x (dX) - acceleration 
*           - derivative of a (dA) - der. of acceleration 
*           - desired acceleration - must be <= f32Incrmax
*           - target value x 
*           - sample time [s] 
*           - maximum increment of x 
*           - X(T1) - x at the instant where the ramp got the desired acceleration (f32Incr = const) 
*             X(T2) - x at the instant where the ramp started to decelerate 
*           - defined derivative of acceleration: dA = A / T 
*           - state of the ramp process 0: f32Incr rising, 1: f32Incr = const, 2: f32Incr falling 
*           - flag is set to TRUE if the desired value is achieved 
*
* @return   void
*       
* @remarks  The init value is stored into the f32State variable. The f32Incr is
*           reset to 0 and the state variable u16AccState is set to 0. The
*           bReachFlag flag is cleared.
*
****************************************************************************/
static inline void GFLIB_FlexSRampInit_F16_FC(register frac16_t f16InitVal,
                                              register GFLIB_FLEXSRAMP_T_F32 *psParam)
{  
    psParam -> f32State    = f16InitVal << 16;
    psParam -> bReachFlag  = FALSE; 
    psParam -> f32AIncr    = 0;
    psParam -> u16AccState = 0;
}
 
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_FLEXSRAMP_A32_H_ */
