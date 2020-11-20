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
* @brief  Dynamic ramp
* 
*******************************************************************************/
#ifndef _GFLIB_DRAMP_FLT_H_
#define _GFLIB_DRAMP_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_DRampInit_FLT_Ci(fltInitVal, psParam)                            \
        GFLIB_DRampInit_FLT_FCi(fltInitVal, psParam)
#define GFLIB_DRamp_FLT_C(fltTarget, fltInstant, pbStopFlag, psParam)          \
        GFLIB_DRamp_FLT_FC(fltTarget, fltInstant, pbStopFlag, psParam)
#define GFLIB_DRamp_FLT_CRam(fltTarget, fltInstant, pbStopFlag, psParam)       \
        GFLIB_DRamp_FLT_FCRam(fltTarget, fltInstant, pbStopFlag, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t fltRampUp;      /* Ramp-up increment  */
    float_t fltRampDown;    /* Ramp-down increment */
    float_t fltRampUpSat;   /* Ramp-up increment used in case of saturation */
    float_t fltRampDownSat; /* Ramp-down increment used in case of saturation */
    float_t fltState;       /* Previous ramp value */
    bool_t  bReachFlag;     /* Flag is set to 1 if the desired value is achieved */
} GFLIB_DRAMP_T_FLT;

/***************************************************************************//*!
* @brief    The function initializes the actual value of DRamp_FLT.
*
* @param    ptr  GFLIB_DRAMP_T_FLT *psParam
*                  - fltRampUp: Ramp-up increment
*                  - fltRampDown: Ramp-down increment
*                  - fltRampUpSat: Ramp-up increment used in case of saturation
*                  - fltRampDownSat: Ramp-down increment used in case of saturation
*                  - fltState: Previous ramp value
*                  - bReachFlag: Flag is set to 1 if the desired value is achieved
* @param    in   float_t fltInitVal - Initial value
* @remarks  The initialization value is stored into the psParam->fltState and the
*           bReachFlag flag is cleared.
*******************************************************************************/
RAM_FUNC_LIB 
static inline void GFLIB_DRampInit_FLT_FCi(float_t fltInitVal, GFLIB_DRAMP_T_FLT *psParam)
{
    psParam->fltState = fltInitVal;
    psParam->bReachFlag = 0;
}
 
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_DRamp_FLT_FC(float_t fltTarget, float_t fltInstant, 
                                  const bool_t *pbStopFlag, GFLIB_DRAMP_T_FLT *psParam);

RAM_FUNC_LIB 
extern float_t GFLIB_DRamp_FLT_FCRam(float_t fltTarget, float_t fltInstant, 
                                     const bool_t *pbStopFlag, GFLIB_DRAMP_T_FLT *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_DRAMP_FLT_H_ */
