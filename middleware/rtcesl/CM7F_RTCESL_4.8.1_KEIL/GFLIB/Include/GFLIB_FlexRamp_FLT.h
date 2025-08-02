/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
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
#include "mlib_FP.h"

/******************************************************************************
* Macros 
******************************************************************************/
#define GFLIB_FlexRampInit_FLT_C(fltInitVal, psParam)                         \
        GFLIB_FlexRampInit_FLT_FC(fltInitVal, psParam)
#define GFLIB_FlexRampCalcIncr_FLT_C(fltTarget, fltDuration, psParam)         \
        GFLIB_FlexRampCalcIncr_FLT_FC(fltTarget, fltDuration, psParam)
#define GFLIB_FlexRamp_FLT_C(psParam)                                         \
        GFLIB_FlexRamp_FLT_FC(psParam)
#define GFLIB_FlexRampCalcIncr_FLT_CRam(fltTarget, fltDuration, psParam)      \
        GFLIB_FlexRampCalcIncr_FLT_FCRam(fltTarget, fltDuration, psParam)
#define GFLIB_FlexRamp_FLT_CRam(psParam)                                      \
        GFLIB_FlexRamp_FLT_FCRam(psParam)

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
RAM_FUNC_LIB 
extern void GFLIB_FlexRampCalcIncr_FLT_FCRam(float_t fltTarget,
                                             float_t fltDuration,
                                             GFLIB_FLEXRAMP_T_FLT *psParam);
										  
extern float_t GFLIB_FlexRamp_FLT_FC(GFLIB_FLEXRAMP_T_FLT *psParam);
RAM_FUNC_LIB 
extern float_t GFLIB_FlexRamp_FLT_FCRam(GFLIB_FLEXRAMP_T_FLT *psParam);

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
RAM_FUNC_LIB 
RTCESL_INLINE static inline void GFLIB_FlexRampInit_FLT_FC(register float_t fltInitVal,
                                             register GFLIB_FLEXRAMP_T_FLT *psParam)
{  
    psParam -> fltState = fltInitVal;
    psParam -> bReachFlag = FALSE; 
}
 
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_FLEXRAMP_FLT_H_ */
