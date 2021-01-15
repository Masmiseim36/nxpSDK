/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KE_MEASURE_H_
#define KE_MEASURE_H_

#include "mid_def.h"

#include "gflib.h"
#include "gdflib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    uint16_t ui16Active;                     /* Inidicates whether Ke is being measured
                                                (true) or not (false) */
    uint16_t ui16MCATObsrvDone;              /* Inidicates whether MCAT has calculated Befm
                                                observer (true) or not yet (false) */
    uint16_t ui16LoopCounter;                /* Serves for timing to determine e.g. 300ms */
    frac16_t *pf16PosEl;                     /* Pointer to electrical position for Park transformations */
    frac16_t *pf16UdReq;                     /* Pointer to required voltage Ud which is applied to the motor */
    frac16_t *pf16IdReq;                     /* Pointer to required current Id (PI current controller's input) */
    frac16_t *pf16Idfbck;                    /* Pointer to actual real measured current Id */
    frac32_t *pf32Ed;                        /* Pointer to Befm voltage in d-axis calculated by Bemf observer */
    frac32_t *pf32Eq;                        /* Pointer to Befm voltage in q-axis calculated by Bemf observer */
    frac16_t f16IdReqOpenLoop;               /* Openloop current */
    frac16_t f16SpeedElReq;                  /* Required Electrical Speed */
    frac16_t f16SpeedElRamp;                 /* Ramped f16SpeedElReq, this speed is integrated to get position */
    frac16_t f16Ke;                          /* Resulting electrical constant */
    int16_t i16ShiftKeMax;                   /* Shift used when calculating e/w, the shift also
                                                expresses by how many bit is basic ke scale extended */
    GFLIB_RAMP_T_F16 sSpeedElRampParam;      /* Ramp Up + Down coefficients for f16Speed */
    GFLIB_INTEGRATOR_T_A32 sSpeedIntegrator; /* Speed integrator coefficients */
    GDFLIB_FILTER_MA_T_A32 sEdMA32Filter;    /* Bemf Ed MA filter */
    GDFLIB_FILTER_MA_T_A32 sEqMA32Filter;    /* Bemf Eq MA filter */
} mid_get_ke_a1_t;

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
extern void MID_getKe(mid_get_ke_a1_t *sKeMeasFcn);

#ifdef __cplusplus
}
#endif

#endif /* KE_MEASURE_H_ */
