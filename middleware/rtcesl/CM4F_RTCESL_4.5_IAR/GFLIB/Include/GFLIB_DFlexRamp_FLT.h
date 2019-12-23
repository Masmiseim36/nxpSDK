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
* @brief  Float dynamic flex ramp functions 
* 
*******************************************************************************/
#ifndef _GFLIB_DFLEXRAMP_FLT_H_
#define _GFLIB_DFLEXRAMP_FLT_H_

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
#define GFLIB_DFlexRampInit_FLT_C(fltInitVal, psParam)                                                 \
        GFLIB_DFlexRampInit_FLT_FC(fltInitVal, psParam)
#define GFLIB_DFlexRampCalcIncr_FLT_C(fltTarget, fltDuration, fltIncrSatMot, fltIncrSatGen, psParam)   \
        GFLIB_DFlexRampCalcIncr_FLT_FC(fltTarget, fltDuration, fltIncrSatMot, fltIncrSatGen, psParam)
#define GFLIB_DFlexRamp_FLT_C(fltInstant, pbStopFlagMot, pbStopFlagGen, psParam)                       \
        GFLIB_DFlexRamp_FLT_FC(fltInstant, pbStopFlagMot, pbStopFlagGen, psParam)

/******************************************************************************
* Types
******************************************************************************/
/* Flex dynamic ramp structure */
typedef struct
{
    float_t fltState; 
    float_t fltIncr; 
    float_t fltIncrSatMot;
    float_t fltIncrSatGen;
    float_t fltTarget; 
    float_t fltTs;
    float_t fltIncrMax; 
    bool_t bReachFlag; 
} GFLIB_DFLEXRAMP_T_FLT;

/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern void GFLIB_DFlexRampCalcIncr_FLT_FC(float_t fltTarget,
                                           float_t fltDuration,
                                           float_t fltIncrSatMot,
                                           float_t fltIncrSatGen,
                                           GFLIB_DFLEXRAMP_T_FLT *psParam);

extern float_t GFLIB_DFlexRamp_FLT_FC(float_t fltInstant,
                                       const bool_t *pbStopFlagMot,
                                       const bool_t *pbStopFlagGen,
                                       GFLIB_DFLEXRAMP_T_FLT *psParam);


/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief  Float dynamic flex ramp initialization
*         
* @param  in  float_t fltInitVal - Measured instant value in float_t
*         
* @param  ptr GFLIB_FLEXRAMP_T_FLT *psParam
*               - fltState: State variable keeping the last state, assigned by GFLIB_DFlexRampInit_FLT
*               - fltIncr: Increment - calculated by GFLIB_DFlexRampCalcIncr_FLT function
*               - fltIncrSatMot: Motor mode saturation mode increment - assigned by user(positive value), 
*                                sign can be changed by GFLIB_DFlexRampCalcIncr_FLT
*               - fltIncrSatGen: Generator mode saturation mode increment - assigned by user(positive value),
*                                sign can be changed by GFLIB_DFlexRampCalcIncr_FLT
*               - fltTarget: Target value - assigned by GFLIB_DFlexRampCalcIncr_FLT function
*               - fltTs: Sample time [s] - assigned by user(positive value)
*               - fltIncrMax: Maximum increment - assigned by user(positive value)
*               - bReachFlag: Flag is set to TRUE if the desired value is achieved
*         
* @return void
*       
* @remarks The initialization value is stored into the fltState variable and the bReachFlag flag is cleared.
*
****************************************************************************/
static inline void GFLIB_DFlexRampInit_FLT_FC(register float_t fltInitVal,
                                              register GFLIB_DFLEXRAMP_T_FLT *psParam)
{
    psParam -> fltState = fltInitVal;
    psParam -> bReachFlag = FALSE;   
}
 
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_DFLEXRAMP_FLT_H_ */
