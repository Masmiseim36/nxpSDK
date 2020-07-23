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
#define GFLIB_FlexSRampCalcIncr_F16_CRam(f16Target, a32Duration, psParam)     \
        GFLIB_FlexSRampCalcIncr_F16_FCRam(f16Target, a32Duration, psParam)       
#define GFLIB_FlexSRamp_F16_CRam(psParam)                                     \
        GFLIB_FlexSRamp_F16_FCRam(psParam)		

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
RAM_FUNC_LIB 
extern bool_t GFLIB_FlexSRampCalcIncr_F16_CRam(frac16_t f16Target, acc32_t a32Duration, 
                                               GFLIB_FLEXSRAMP_T_F32 *psParam);
											
extern frac16_t GFLIB_FlexSRamp_F16_FC(GFLIB_FLEXSRAMP_T_F32 *psParam);											
RAM_FUNC_LIB 
extern frac16_t GFLIB_FlexSRamp_F16_FCRam(GFLIB_FLEXSRAMP_T_F32 *psParam);

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
RAM_FUNC_LIB 
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
