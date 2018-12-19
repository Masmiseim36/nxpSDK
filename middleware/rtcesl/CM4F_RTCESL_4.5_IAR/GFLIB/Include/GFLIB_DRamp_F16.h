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

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_DRAMP_F16_H_ */

