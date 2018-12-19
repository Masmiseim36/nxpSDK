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
* @brief  Flex ramp functions with 16-bit fractional output 
* 
*******************************************************************************/
#ifndef _GFLIB_FLEXRAMP_A32_H_
#define _GFLIB_FLEXRAMP_A32_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "mlib.h"

/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_FlexRampInit_F16_C(f16InitVal, psParam)                         \
        GFLIB_FlexRampInit_F16_FC(f16InitVal, psParam)
#define GFLIB_FlexRampCalcIncr_F16_C(f16Target, a32Duration, psParam)         \
        GFLIB_FlexRampCalcIncr_F16_FC(f16Target, a32Duration, psParam)
#define GFLIB_FlexRamp_F16_C(psParam)                                         \
        GFLIB_FlexRamp_F16_FC(psParam)

/******************************************************************************
* Types
******************************************************************************/
/* Flex dynamic ramp structure */
typedef struct
{
    frac32_t f32State; 
    frac32_t f32Incr; 
    frac32_t f32Target; 
    frac32_t f32Ts;
    frac32_t f32IncrMax; 
    bool_t bReachFlag; 
} GFLIB_FLEXRAMP_T_F32;

/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern void GFLIB_FlexRampCalcIncr_F16_FC(frac16_t f16Target,
                                          acc32_t a32Duration,
                                          GFLIB_FLEXRAMP_T_F32 *psParam);

extern frac16_t GFLIB_FlexRamp_F16_FC(GFLIB_FLEXRAMP_T_F32 *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    Flex ramp initialization
*
* @param    in   frac16_t f16InitVal - Measured instant value in <-1;1) in frac16_t
*
* @param    ptr  GFLIB_FLEXRAMP_T_F32 *psParam
*                  - f32State: State variable keeping the last state, assigned by 
*                              GFLIB_FlexRampInit_F16
*                  - f32Incr: Increment - calculated by GFLIB_FlexRampCalcIncr_F16 function
*                  - f32Target: Target value - assigned by GFLIB_FlexRampCalcIncr_F16 function
*                  - f32Ts: Sample time [s] - assigned by user(positive value)
*                  - f32IncrMax: Maximum increment - assigned by user(positive value)
*                  - bReachFlag: Flag is set to TRUE if the desired value is achieved
*
* @return   void
*       
* @remarks  The initialization value is stored into the f32State variable and the
*           bReachFlag flag is cleared.
*
****************************************************************************/
static inline void GFLIB_FlexRampInit_F16_FC(register frac16_t f16InitVal,
                                             register GFLIB_FLEXRAMP_T_F32 *psParam)
{  
    psParam -> f32State = f16InitVal << 16;
    psParam -> bReachFlag = FALSE; 
}
 

#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_FLEXRAMP_A32_H_ */
