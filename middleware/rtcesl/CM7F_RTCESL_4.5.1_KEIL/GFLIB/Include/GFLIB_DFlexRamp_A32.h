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
* @brief  Dynamic flex ramp functions with 16-bit fractional output 
* 
*******************************************************************************/
#ifndef _GFLIB_DFLEXRAMP_A32_H_
#define _GFLIB_DFLEXRAMP_A32_H_

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
#define GFLIB_DFlexRampInit_F16_C(f16InitVal, psParam)                                                    \
        GFLIB_DFlexRampInit_F16_FC(f16InitVal, psParam)                                                   
#define GFLIB_DFlexRampCalcIncr_F16_C(f16Target, a32Duration, f32IncrSatMot, f32IncrSatGen, psParam)      \
        GFLIB_DFlexRampCalcIncr_F16_FC(f16Target, a32Duration, f32IncrSatMot, f32IncrSatGen, psParam)     
#define GFLIB_DFlexRamp_F16_C(f16Instant, pbStopFlagMot, pbStopFlagGen, psParam)                          \
        GFLIB_DFlexRamp_F16_FC(f16Instant, pbStopFlagMot, pbStopFlagGen, psParam)
#define GFLIB_DFlexRampCalcIncr_F16_CRam(f16Target, a32Duration, f32IncrSatMot, f32IncrSatGen, psParam)   \
        GFLIB_DFlexRampCalcIncr_F16_FCRam(f16Target, a32Duration, f32IncrSatMot, f32IncrSatGen, psParam)
#define GFLIB_DFlexRamp_F16_CRam(f16Instant, pbStopFlagMot, pbStopFlagGen, psParam)                       \
        GFLIB_DFlexRamp_F16_FCRam(f16Instant, pbStopFlagMot, pbStopFlagGen, psParam)

/******************************************************************************
* Types
******************************************************************************/
/* Flex dynamic ramp structure */
typedef struct
{
    frac32_t f32State; 
    frac32_t f32Incr; 
    frac32_t f32IncrSatMot;
    frac32_t f32IncrSatGen;
    frac32_t f32Target; 
    frac32_t f32Ts;
    frac32_t f32IncrMax; 
    bool_t bReachFlag; 
} GFLIB_DFLEXRAMP_T_F32;

/******************************************************************************
* Exported function prototypes
******************************************************************************/
extern void GFLIB_DFlexRampCalcIncr_F16_FC(frac16_t f16Target,
                                           acc32_t a32Duration,
                                           frac32_t f32IncrSatMot,
                                           frac32_t f32IncrSatGen,
                                           GFLIB_DFLEXRAMP_T_F32 *psParam);
RAM_FUNC_LIB 
extern void GFLIB_DFlexRampCalcIncr_F16_FCRam(frac16_t f16Target,
                                              acc32_t a32Duration,
                                              frac32_t f32IncrSatMot,
                                              frac32_t f32IncrSatGen,
                                              GFLIB_DFLEXRAMP_T_F32 *psParam);
										   
extern frac16_t GFLIB_DFlexRamp_F16_FC(frac16_t f16Instant,
                                       const bool_t *pbStopFlagMot,
                                       const bool_t *pbStopFlagGen,
                                       GFLIB_DFLEXRAMP_T_F32 *psParam);										   
RAM_FUNC_LIB 
extern frac16_t GFLIB_DFlexRamp_F16_FCRam(frac16_t f16Instant,
                                          const bool_t *pbStopFlagMot,
                                          const bool_t *pbStopFlagGen,
                                          GFLIB_DFLEXRAMP_T_F32 *psParam);


/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief   Dynamic flex ramp initialization
*
* @param   in   frac16_t f16InitVal - Measured instant value in <-1;1) in frac16_t
*
* @param   ptr  GFLIB_FLEXRAMP_T_F32 *psParam
*                 - f32State: State variable keeping the last state, assigned by GFLIB_DFlexRampInit_F16
*                 - f32Incr: Increment - calculated by GFLIB_DFlexRampCalcIncr_F16 function
*                 - f32IncrSatMot: Motor mode saturation mode increment - assigned by user(positive value), 
*                                  sign can be changed by GFLIB_DFlexRampCalcIncr_F16
*                 - f32IncrSatGen: Generator mode saturation mode increment - assigned by user(positive value),
*                                  sign can be changed by GFLIB_DFlexRampCalcIncr_F16
*                 - f32Target: Target value - assigned by GFLIB_DFlexRampCalcIncr_F16 function
*                 - f32Ts: Sample time [s] - assigned by user(positive value)
*                 - f32IncrMax: Maximum increment - assigned by user(positive value)
*                 - bReachFlag: Flag is set to TRUE if the desired value is achieved
*
* @return  void
*       
* @remarks The initialization value is stored into the f32State variable and the bReachFlag flag is cleared.
*
****************************************************************************/
RAM_FUNC_LIB 
static inline void GFLIB_DFlexRampInit_F16_FC(register frac16_t f16InitVal,
                                              register GFLIB_DFLEXRAMP_T_F32 *psParam)
{
    psParam -> f32State = f16InitVal << 16;
    psParam -> bReachFlag = FALSE;   
}
 
#if defined(__cplusplus) 
} 
#endif 

#endif /* _GFLIB_DFLEXRAMP_A32_H_ */
