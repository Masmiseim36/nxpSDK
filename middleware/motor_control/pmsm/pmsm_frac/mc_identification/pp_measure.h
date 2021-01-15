/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PP_MEASURE_H_
#define PP_MEASURE_H_

#include "mid_def.h"
#include "gflib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    uint16_t ui16Active;        /* Indicates whether Ke is being measured (true) or not (false) */
    uint16_t ui16PpDetermined;  /* Indicates whether the user already set pp in MCAT (true) or not yet (false) */
    uint16_t ui16WaitingSteady; /* Indicates that motor is waiting in steady state (when electrical position is zero) */
    uint16_t ui16LoopCounter;   /* Serves for timing to determine e.g. 300ms */
    frac16_t *pf16PosEl;        /* Pointer to electrical position for Park transformations */
    frac16_t *pf16IdReq;        /* Pointer to required current Id (PI current controller's input) */
    frac16_t f16PosElCurrent;   /* Current value of electrical position */
    frac16_t f16PosElLast;      /* Last value of electrical position */
    frac16_t f16IdReqOpenLoop;  /* Openloop current */
    frac16_t f16SpeedElReq;     /* Required Electrical Speed */
    frac16_t f16SpeedElRamp;    /* Ramped f16SpeedElReq, this speed is integrated to get position */
    GFLIB_RAMP_T_F16 sSpeedElRampParam;      /* Ramp Up + Down coefficients for f16Speed */
    GFLIB_INTEGRATOR_T_A32 sSpeedIntegrator; /* Speed integrator coefficients */
} mid_get_pp_a1_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint16_t ui16FaultMID;

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
extern void MID_getPp(mid_get_pp_a1_t *sPpMeasFcn);

#ifdef __cplusplus
}
#endif

#endif /* PP_MEASURE_H_ */
