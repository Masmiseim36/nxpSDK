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
* @brief  Float flex ramp functions
* 
*******************************************************************************/
#ifndef _GFLIB_FLEXRAMP_FLT_H_
#define _GFLIB_FLEXRAMP_FLT_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "mlib_fp.h"

/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_FlexRampInit_FLT_C(fltInitVal, psParam)                         \
        GFLIB_FlexRampInit_FLT_FC(fltInitVal, psParam)
#define GFLIB_FlexRampCalcIncr_FLT_C(fltTarget, fltDuration, psParam)         \
        GFLIB_FlexRampCalcIncr_FLT_FC(fltTarget, fltDuration, psParam)
#define GFLIB_FlexRamp_FLT_C(psParam)                                         \
        GFLIB_FlexRamp_FLT_FC(psParam)

/******************************************************************************
* Types
******************************************************************************/
/* Flex dynamic ramp structure */
typedef struct
{
    float_t fltState; 
    float_t fltIncr; 
    float_t fltTarget; 
    float_t fltTs;
    float_t fltIncrMax; 
    bool_t  bReachFlag; 
} GFLIB_FLEXRAMP_T_FLT;

/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern void GFLIB_FlexRampCalcIncr_FLT_FC(float_t fltTarget,
                                          float_t fltDuration,
                                          GFLIB_FLEXRAMP_T_FLT *psParam);

extern float_t GFLIB_FlexRamp_FLT_FC(GFLIB_FLEXRAMP_T_FLT *psParam);

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief  Float Flex ramp initialization
*
* @param  in  float_t fltInitVal - Measured instant value in float_t
*
* @param  ptr GFLIB_FLEXRAMP_T_FLT *psParam
*               - fltState: State variable keeping the last state, assigned by 
*                           GFLIB_FlexRampInit_FLT
*               - fltIncr: Increment - calculated by GFLIB_FlexRampCalcIncr_FLT function
*               - fltTarget: Target value - assigned by GFLIB_FlexRampCalcIncr_FLT function
*               - fltTs: Sample time [s] - assigned by user(positive value)
*               - fltIncrMax: Maximum increment - assigned by user(positive value)
*               - bReachFlag: Flag is set to TRUE if the desired value is achieved
*
* @return void
*       
* @remarks  The initialization value is stored into the fltState variable and the
*           bReachFlag flag is cleared.
*
****************************************************************************/
static inline void GFLIB_FlexRampInit_FLT_FC(register float_t fltInitVal,
                                             register GFLIB_FLEXRAMP_T_FLT *psParam)
{  
    psParam -> fltState = fltInitVal;
    psParam -> bReachFlag = FALSE; 
}
 
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_FLEXRAMP_FLT_H_ */
