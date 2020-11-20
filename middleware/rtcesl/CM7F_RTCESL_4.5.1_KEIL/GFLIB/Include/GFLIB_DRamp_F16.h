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
* @brief  Dynamic Ramp 
* 
*******************************************************************************/
#ifndef _GFLIB_DRAMP_F16_H_
#define _GFLIB_DRAMP_F16_H_

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
#define GFLIB_DRampInit_F16_Ci(f16InitVal, psParam)                            \
        GFLIB_DRampInit_F16_FCi(f16InitVal, psParam)                                 
#define GFLIB_DRamp_F16_Asm(f16Target, f16Instant, pbStopFlag, psParam)        \
        GFLIB_DRamp_F16_FAsm(f16Target, f16Instant, pbStopFlag, psParam)
#define GFLIB_DRamp_F16_AsmRam(f16Target, f16Instant, pbStopFlag, psParam)      \
        GFLIB_DRamp_F16_FAsmRam(f16Target, f16Instant, pbStopFlag, psParam)
        
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac16_t f16RampUp;      /* Ramp-up increment  */
    frac16_t f16RampDown;    /* Ramp-down increment */
    frac16_t f16RampUpSat;   /* Ramp-up increment used in case of saturation */
    frac16_t f16RampDownSat; /* Ramp-down increment used in case of saturation */
    frac16_t f16State;       /* Previous ramp value */
    bool_t   bReachFlag;     /* Flag is set to 1 if the desired value is achieved */
} GFLIB_DRAMP_T_F16;

/***************************************************************************//*!
* @brief  The function initializes the actual value of DRamp_F16.
*
* @param    ptr  GFLIB_DRAMP_T_F16 *psParam
*                   - f16RampUp: Ramp-up increment
*                   - f16RampDown: Ramp-down increment
*                   - f16RampUpSat: Ramp-up increment used in case of saturation
*                   - f16RampDownSat: Ramp-down increment used in case of saturation
*                   - f16State: Previous ramp value
*                   - bReachFlag: Flag is set to 1 if the desired value is achieved
* @param    in   frac16_t f16InitVal - Initial value
* @remarks  The initialization value is stored into the psParam->f16State and the
*           bReachFlag flag is cleared.
*******************************************************************************/
RAM_FUNC_LIB 
static inline void GFLIB_DRampInit_F16_FCi(frac16_t f16InitVal, GFLIB_DRAMP_T_F16 *psParam)
{
    psParam->f16State = f16InitVal;
    psParam->bReachFlag = 0;
}
 
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/

extern frac16_t GFLIB_DRamp_F16_FAsm(frac16_t f16Target, frac16_t f16Instant, 
                                     const bool_t *pbStopFlag, GFLIB_DRAMP_T_F16 *psParam);

RAM_FUNC_LIB 
extern frac16_t GFLIB_DRamp_F16_FAsmRam(frac16_t f16Target, frac16_t f16Instant, 
                                        const bool_t *pbStopFlag, GFLIB_DRAMP_T_F16 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_DRAMP_F16_H_ */

