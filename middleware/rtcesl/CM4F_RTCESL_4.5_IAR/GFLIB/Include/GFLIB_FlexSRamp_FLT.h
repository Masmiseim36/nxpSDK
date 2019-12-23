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
* @brief  Float flex s-ramp functions
* 
*******************************************************************************/
#ifndef _GFLIB_FLEXSRAMP_FLT_H_
#define _GFLIB_FLEXSRAMP_FLT_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "mlib_fp.h"
#include "gflib_fp.h"    

/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_FlexSRampInit_FLT_C(fltInitVal, psParam)                         \
        GFLIB_FlexSRampInit_FLT_FC(fltInitVal, psParam)
#define GFLIB_FlexSRampCalcIncr_FLT_C(fltTarget, fltDuration, psParam)         \
        GFLIB_FlexSRampCalcIncr_FLT_FC(fltTarget, fltDuration, psParam)
#define GFLIB_FlexSRamp_FLT_C(psParam)                                         \
        GFLIB_FlexSRamp_FLT_FC(psParam)

/******************************************************************************
* Types
******************************************************************************/
/* Flex dynamic ramp structure */
typedef struct
{
    float_t fltState;    /* state variable keeping the last state x  */
    float_t fltIncr;     /* derivative of x (dX) - acceleration */
    float_t fltAIncr;    /* derivative of a (dA) - der. of acceleration */
    float_t fltADes;     /* desired acceleration - must be <= f32Incrmax */
    float_t fltTarget;   /* target value x */
    float_t fltTs;       /* sample time [s] */
    float_t fltIncrMax;  /* maximum increment of x */
    float_t fltXT1;      /* X(T1) - x at the instant where the ramp got the desired acceleration (f32Incr = const) */  
    float_t fltXT2;      /* X(T2) - x at the instant where the ramp started to decelerate */
    float_t fltDA;       /* defined derivative of acceleration: dA = A / T */
    uint16_t u16AccState;/* state of the ramp process 0: f32Incr rising, 1: f32Incr = const, 2: f32Incr falling */
    bool_t bReachFlag;   /* flag is set to TRUE if the desired value is achieved */
} GFLIB_FLEXSRAMP_T_FLT;

/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern bool_t GFLIB_FlexSRampCalcIncr_FLT_FC(float_t fltTarget, float_t fltDur, 
                                             GFLIB_FLEXSRAMP_T_FLT *psParam);

extern float_t GFLIB_FlexSRamp_FLT_FC(GFLIB_FLEXSRAMP_T_FLT *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief  Float flex s-ramp initialization
*
* @param  in  float_t fltInitVal - Measured instant value in float_t
*
* @param  ptr  GFLIB_FLEXSRAMP_T_FLT *psParam
*            - State variable keeping the last state x  
*            - derivative of x (dX) - acceleration 
*            - derivative of a (dA) - der. of acceleration 
*            - desired acceleration - must be <= fltIncrmax 
*            - Target value x 
*            - Sample time [s] 
*            - Maximum increment of x  
*            - X(T1) - x at the instant where the ramp got the desired acceleration (fltIncr = const)  
*            - X(T2) - x at the instant where the ramp started to decelerate  
*            - Defined derivative of acceleration: dA = A / T  
*            - State of the ramp process 0: fltIncr rising, 1: fltIncr = const, 2: fltIncr falling  
*            - Flag is set to TRUE if the desired value is achieved  
*
* @return void
*       
* @remarks  The init value is stored into the fltState variable. The fltIncr is
*           reset to 0 and the state variable u16AccState is set to 0. The
*           bReachFlag flag is cleared.
*
****************************************************************************/
static inline void GFLIB_FlexSRampInit_FLT_FC(register float_t fltInitVal,
                                              register GFLIB_FLEXSRAMP_T_FLT *psParam)
{  
    psParam -> fltState = fltInitVal;
    psParam -> bReachFlag = FALSE; 
    psParam -> fltAIncr    = 0.0F;
    psParam -> u16AccState = 0;
}
 
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_FLEXSRAMP_FLT_H_ */
