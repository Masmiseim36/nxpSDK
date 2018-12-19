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
* @brief  Dynamic ramp
* 
*******************************************************************************/
#ifndef _GFLIB_DRAMP_F32_H_
#define _GFLIB_DRAMP_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/  
#include "mlib.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_DRampInit_F32_Ci(f32InitVal, psParam)                            \
        GFLIB_DRampInit_F32_FCi(f32InitVal, psParam)                                   
#define GFLIB_DRamp_F32_Asm(f32Target, f32Instant, pbStopFlag, psParam)        \
        GFLIB_DRamp_F32_FAsm(f32Target, f32Instant, pbStopFlag, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32RampUp;      /* Ramp-up increment  */
    frac32_t f32RampDown;    /* Ramp-down increment */
    frac32_t f32RampUpSat;   /* Ramp-up increment used in case of saturation */
    frac32_t f32RampDownSat; /* Ramp-down increment used in case of saturation */
    frac32_t f32State;       /* Previous ramp value */
    bool_t   bReachFlag;     /* Flag is set to 1 if the desired value is achieved */
} GFLIB_DRAMP_T_F32;

/***************************************************************************//*!
* @brief  The function initializes the actual value of DRamp_F32.
*
* @param    ptr   GFLIB_DRAMP_T_F32 *psParam
*                    - f32RampUp: Ramp-up increment
*                    - f32RampDown: Ramp-down increment
*                    - f32RampUpSat: Ramp-up increment used in case of saturation
*                    - f32RampDownSat: Ramp-down increment used in case of saturation
*                    - f32State: Previous ramp value
*                    - bReachFlag: Flag is set to 1 if the desired value is achieved
* @param    in    frac32_t f32InitVal - Initial value
* @remarks  The initialization value is stored into the psParam->f32State and the
*           bReachFlag flag is cleared.
*******************************************************************************/
static inline void GFLIB_DRampInit_F32_FCi(frac32_t f32InitVal, GFLIB_DRAMP_T_F32 *psParam)
{
    psParam->f32State = f32InitVal;
    psParam->bReachFlag = 0;
}
 
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac32_t GFLIB_DRamp_F32_FAsm(frac32_t f32Target, frac32_t f32Instant, 
                                     const bool_t *pbStopFlag, GFLIB_DRAMP_T_F32 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_DRAMP_F32_H_ */

