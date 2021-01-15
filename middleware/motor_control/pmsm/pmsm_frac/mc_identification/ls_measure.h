/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LS_MEASURE_H_
#define LS_MEASURE_H_

#include "mid_def.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    uint16_t ui16Active;             /* Inidicates whether Ls is being measured (true) or not (false) */
    uint16_t ui16LoopCounter;        /* Serves for timing to determine e.g. 300ms */
    int16_t i16AmplitudeOK;          /* Indicates that amplitude of the measuring signal was set */
    int16_t i16FrequencyOK;          /* Indicates that frequency of the measuring signal was set */
    frac16_t *pf16UdReq;             /* Pointer to required voltage Ud which is applied to the motor */
    frac16_t *pf16Idfbck;            /* Pointer to actual real measured current Id */
    frac16_t f16FreqStart;           /* Starting measuring frequency */
    frac16_t f16FreqDecrement;       /* Frequency decrement when tuning the measuring signal */
    frac16_t f16FreqMin;             /* Minimal measuring frequency */
    frac16_t f16FreqActual;          /* Actual Measuring Frequency */
    frac16_t f16Angle;               /* Angle for generating the measuring sine signal */
    frac16_t f16AngleIncrement;      /* Angle increment for generating the measuring sine signal */
    frac16_t f16AngleIncrementConst; /* Constant for calculating f16MeasAngleIncrement from f16MeasFreq */
    frac16_t f16IdAmplitudeReq;      /* Required amplitude of measuring sine current */
    frac16_t f16IdAmplitude;         /* Amplitude of measuring sine current */
    frac16_t f16UdMax;               /* Maximal amplitude of measuring voltage set as 1/2 DCbus before measurement */
    frac16_t f16UdAmplitude;         /* Amplitude of measuring sine voltage */
    frac16_t f16UdIncrement;         /* Voltage increment when tuning the measuring signal */
    frac16_t f16Rs;                  /* Measured Rs set before measurement */
    frac16_t f16Ls;                  /* Measured Ls return */
    frac16_t f16Ld;                  /* For storage Ld */
    frac16_t f16Lq;                  /* For storage Lq */
    int16_t i16ShiftRsMax;           /* Shift used when calculating R=U/I, the shift also expresses
                                        by how many bit is basic resistance scale extended */
    int16_t i16ShiftZsMax;           /* Shift used when calculating Z=U/I, the shift also expresses
                                        by how many bit is basic impedance scale extended */
    int16_t i16ShiftZdMax;           /* Shift used when calculating Z=U/I, the shift also expresses
                                        by how many bit is basic impedance scale extended */
    int16_t i16ShiftZqMax;           /* Shift used when calculating Z=U/I, the shift also expresses
                                        by how many bit is basic impedance scale extended */
    int16_t i16ShiftLsMax;           /* Shift used when calculating Ls=Zs/f, the shift also expresses
                                        by how many bit is basic inductance scale extended */
    int16_t i16ShiftLdMax;           /* Shift used when calculating Ld=Zd/f, the shift also expresses
                                        by how many bit is basic inductance scale extended */
    int16_t i16ShiftLqMax;           /* Shift used when calculating Lq=Zq/f, the shift also expresses
                                        by how many bit is basic inductance scale extended */
} mid_get_ls_a1_t;

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
extern void MID_getLs(mid_get_ls_a1_t *sLsMeasFcn);

#ifdef __cplusplus
}
#endif

#endif /* LS_MEASURE_H_ */
