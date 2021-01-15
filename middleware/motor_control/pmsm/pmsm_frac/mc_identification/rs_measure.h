/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RS_MEASURE_H_
#define RS_MEASURE_H_

#include "mid_def.h"
#include "mlib_types.h"
#include "gdflib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    GDFLIB_FILTER_MA_T_A32 sUdReqMA32Filter;  /* Ud required filter */
    GDFLIB_FILTER_MA_T_A32 sIdfbckMA32Filter; /* Id feedback filter */
    uint16_t ui16Active;                      /* Inidicates whether Rs is being measured (true) or not (false) */
    uint16_t ui16LoopCounter;                 /* Serves for timing to determine e.g. 600ms */
    frac16_t *pf16IdReq;                      /* Pointer to required current Id (PI current controller's input) */
    frac16_t f16IdMeas;                       /* User defined measuring DC current */
    frac16_t *pf16UdReq;                      /* Pointer to required voltage Ud (PI current controller's output) */
    frac16_t *pf16Idfbck;                     /* Pointer to actual real measured current Id */
    frac16_t f16Rs;                           /* Measured Rs return */
    frac16_t *pf16UdErrorLookUp;  /* Pointer to Look Up table with the transfer characteristic coefficients */
    int16_t i16RescaleIdLUTShift; /* Shift amount when re-scaling current Id into range -2A to 2A for LUT */
    frac16_t f16RescaleIdLUTGain; /* Gain when re-scaling current Id into range -2A to 2A for LUT */
    int16_t i16ShiftRsMax;        /* Shift used when calculating U/I and Rs*I, the shift also expresses
      `                              by how many bit is basic resistance scale extended */
} mid_get_rs_a1_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern frac16_t f16TransferCharError[];
extern uint16_t ui16FaultMID;

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
extern void MID_getRs(mid_get_rs_a1_t *sRsMeasFcn);

#ifdef __cplusplus
}
#endif

#endif /* RS_MEASURE_H_ */
