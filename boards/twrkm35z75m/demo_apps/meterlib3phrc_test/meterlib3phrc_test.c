/******************************************************************************
 *
 * Copyright 2010-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************
 * meterlib3phrc_test.c
 ******************************************************************************/
#include <math.h>

#include "fraclib.h"
#include "meterlib.h"
#include "meterlib3phrc_cfg.h"
#include "pin_mux.h"
#include "board.h"

#define _PI   3.14159265358979323846	/* pi */

/* static data definitions                                                    */
static tMETERLIB3PH_DATA mlib = METERLIB3PHRC_CFG;
static volatile frac32  u24_samplePh1, i24_samplePh1, u24_samplePh2, i24_samplePh2,
                        u24_samplePh3, i24_samplePh3;
static tENERGY_CNT wh_cnt, varh_cnt;
static double  time = 0.0, U_RMS_Ph1, I_RMS_Ph1, P_Ph1, Q_Ph1, S_Ph1,
                        U_RMS_Ph2, I_RMS_Ph2, P_Ph2, Q_Ph2, S_Ph2,
                        U_RMS_Ph3, I_RMS_Ph3, P_Ph3, Q_Ph3, S_Ph3,
                        U_ANGLE = (  45.0/180.0)*_PI,
                        I_SHIFT = (  -5.5/180.0)*_PI,
                        DEG_120 = (-120.0/180.0)*_PI,
                        DEG_240 = (-240.0/180.0)*_PI,
                        I_90DEG = (  90.0/180.0)*_PI;

static volatile int cycle = 0, rotation;

static frac16  shift[] = { METERLIB_DEG2SH(-5.5, 50.0),
                           METERLIB_DEG2SH(-5.5, 50.0),
                           METERLIB_DEG2SH(-5.5, 50.0) };

#if defined(__ICCARM__)
  #pragma diag_suppress=Pa082
#endif
void main (void)
{
  BOARD_InitPins();
  BOARD_BootClockRUN(); 
  
  while (1)
  {
    /* calculate phase voltage and phase current waveforms                    */
    time = time+(1.0/KWH_CALC_FREQ);
    u24_samplePh1 = FRAC24(((sin(2*_PI*50.0*time+U_ANGLE)*230.0*sqrt(2)+0.0)/U_MAX));
    i24_samplePh1 = FRAC24(((sin(2*_PI*50.0*time+I_SHIFT+I_90DEG)*5.0*sqrt(2)+0.0)/I_MAX));
    u24_samplePh2 = FRAC24(((sin(2*_PI*50.0*time+DEG_120+U_ANGLE)*230.0*sqrt(2)+0.0)/U_MAX));
    i24_samplePh2 = FRAC24(((sin(2*_PI*50.0*time+DEG_120+I_SHIFT+I_90DEG)*10.0*sqrt(2)+0.0)/I_MAX));
    u24_samplePh3 = FRAC24(((sin(2*_PI*50.0*time+DEG_240+U_ANGLE)*230.0*sqrt(2)+0.0)/U_MAX));
    i24_samplePh3 = FRAC24(((sin(2*_PI*50.0*time+DEG_240+I_SHIFT+I_90DEG)*15.0*sqrt(2)+0.0)/I_MAX));

    rotation = METERLIB3PH_ProcSamples(&mlib,u24_samplePh1,i24_samplePh1,u24_samplePh2,
                                        i24_samplePh2,u24_samplePh3,i24_samplePh3,shift);
    METERLIB3PH_CalcWattHours(&mlib,&wh_cnt,METERLIB_KWH_PR(IMP_PER_KWH));

    /* functions below might be called less frequently - please refer to      */
    /* KWH_CALC_FREQ, KVARH_CALC_FREQ and DECIM_FACTOR constants              */
    if (!(cycle % (int)(KWH_CALC_FREQ/KVARH_CALC_FREQ)))
    {
      METERLIB3PH_CalcVarHours(&mlib,&varh_cnt,METERLIB_KVARH_PR(IMP_PER_KVARH));
    }

    if (!(cycle % DECIM_FACTOR))
    {
      METERLIB3PH_CalcAuxiliary(&mlib);
    }

    METERLIB3PH_ReadResultsPh1(&mlib,&U_RMS_Ph1,&I_RMS_Ph1,&P_Ph1,&Q_Ph1,&S_Ph1);
    METERLIB3PH_ReadResultsPh2(&mlib,&U_RMS_Ph2,&I_RMS_Ph2,&P_Ph2,&Q_Ph2,&S_Ph2);
    METERLIB3PH_ReadResultsPh3(&mlib,&U_RMS_Ph3,&I_RMS_Ph3,&P_Ph3,&Q_Ph3,&S_Ph3);
    cycle++;
  }
}
