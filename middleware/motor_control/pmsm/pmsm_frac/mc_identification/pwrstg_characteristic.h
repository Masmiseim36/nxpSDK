/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PWRSTG_CHARASTERISTIC_H_
#define PWRSTG_CHARASTERISTIC_H_

#include "mid_def.h"
#include "gdflib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    GDFLIB_FILTER_MA_T_A32 sUdReqMA32Filter;  /* Ud required filter */
    GDFLIB_FILTER_MA_T_A32 sIdfbckMA32Filter; /* Id feedback filter */
    uint16_t ui16Active;                      /* Inidicates whether Transfer characteristic is being measured
                                                 (true) or not (false) */
    uint16_t ui16LoopCounter;                 /* Serves for timing to determine e.g. 600ms */
    frac16_t f16Rs;                           /* Stator resistance of connected motor */
    frac16_t *pf16IdReq;                      /* Pointer to required current Id (PI current controller's input) */
    frac16_t *pf16UdReq;                      /* Pointer to required voltage Ud (PI current controller's output) */
    frac16_t *pf16Idfbck;                     /* Pointer to actual real measured current Id */
    frac16_t f16IdCalib;                      /* Range of the characterisation is from -f16IdCharMax to +f16IdCharMax */
    frac16_t f16IdReqActual; /* Actual current at which is the characteristic being measured at the moment */
    frac16_t f16UdErrorLookUp[M1_CHAR_CURRENT_POINT_NUMBERS]; /* Ud error voltages */
    uint16_t ui16LUTIndex;   /* Lookup table index, counts up to 65 until a whole characteristic is measured */
    frac16_t f16IdIncrement; /* Id increment after each 300ms */
    int16_t i16ShiftRsMax;   /* Shift used when calculating U/I and Rs*I, the shift also
                                expresses by how many bit is basic resistance scale extended */
} mid_get_char_a1_t;

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
extern void MID_GetTransferCharacteristic(mid_get_char_a1_t *sTransferCharFcn);

#ifdef __cplusplus
}
#endif

#endif /* PWRSTG_CHARASTERISTIC_H_ */
