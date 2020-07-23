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
