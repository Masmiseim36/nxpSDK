/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CALC_POWER_H_
#define _CALC_POWER_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MID_ONETHIRD (0.333333333333333333333)

typedef struct
{
    float fltUphA;
    float fltUphB;
    float fltUphC;
    float *pfltIphA;
    float *pfltIphB;
    float *pfltIphC;
    float fltUrmsAvg; /* stator average phase rms voltage [V] */
    float fltIrmsAvg; /* stator average phase rms current [A] */
    float fltPAvg;    /* average stator real power [W] */
    float fltQAvg;    /* average reactive stator power [var] */
    float fltSAvg;    /* average stator apparent power [VA] */
    int bCalcPwr;     /* parameter calculation flag */
} mid_calc_power_a1_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void MID_CalcPwr(mid_calc_power_a1_t *sCalcPwr);
void MID_ReadPwr(mid_calc_power_a1_t *sCalcPwr);

#ifdef __cplusplus
}
#endif

#endif /* _CALC_POWER_H_ */
