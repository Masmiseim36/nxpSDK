/******************************************************************************
 *
 * Copyright 2010-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************
 * meterliblp2phkwh_test.c
 ******************************************************************************/
#include <math.h>

#include "fraclib.h"
#include "meterliblp.h"
#include "meterliblp2phkwh_cfg.h"
#include "pin_mux.h"
#include "board.h"

#define _PI   3.14159265358979323846	/* pi */

/* static data definitions                                                    */
static tMETERLIBLP2PH_DATA mlib = METERLIBLP2PH_CFG;
static volatile frac32  u24_samplePh1, i24_samplePh1, u24_samplePh2, i24_samplePh2;
static tENERGY_CNT wh_cnt;
static double  time = 0.0, U_RMS_Ph1, I_RMS_Ph1, P_Ph1, S_Ph1, U_RMS_Ph2,
                        I_RMS_Ph2, P_Ph2, S_Ph2,
                        U_ANGLE = (  45.0/180.0)*_PI,
                        I_SHIFT = (  -5.5/180.0)*_PI,
                        DEG_180 = ( 180.0/180.0)*_PI;
static int     cycle = 0;

static frac16  shift[] = { METERLIBLP_DEG2SH(-5.5, 50.0), METERLIBLP_DEG2SH(-5.5, 50.0) };

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
    i24_samplePh1 = FRAC24(((sin(2*_PI*50.0*time+I_SHIFT)*5.0*sqrt(2)+0.0)/I_MAX));
    u24_samplePh2 = FRAC24(((sin(2*_PI*50.0*time+DEG_180+U_ANGLE)*230.0*sqrt(2)+0.0)/U_MAX));
    i24_samplePh2 = FRAC24(((sin(2*_PI*50.0*time+DEG_180+I_SHIFT)*10.0*sqrt(2)+0.0)/I_MAX));

    METERLIBLP2PH_ProcSamples(&mlib,u24_samplePh1,i24_samplePh1,u24_samplePh2,i24_samplePh2,shift);
    METERLIBLP2PH_CalcWattHours(&mlib,&wh_cnt,METERLIBLP_KWH_PR(IMP_PER_KWH));

    /* functions below might be called less frequently - please refer to      */
    /* KWH_CALC_FREQ, KVARH_CALC_FREQ and DECIM_FACTOR constants              */
    if (!(cycle % DECIM_FACTOR))
    {
      METERLIBLP2PH_CalcURMS (&mlib);
      METERLIBLP2PH_CalcIRMS (&mlib);
      METERLIBLP2PH_CalcPAVG (&mlib);
    }

    METERLIBLP2PH_ReadURMS  (&mlib,&U_RMS_Ph1,&U_RMS_Ph2);
    METERLIBLP2PH_ReadIRMS  (&mlib,&I_RMS_Ph1,&I_RMS_Ph2);
    METERLIBLP2PH_ReadPAVG  (&mlib,&P_Ph1,&P_Ph2);
    METERLIBLP2PH_ReadS     (&mlib,&S_Ph1,&S_Ph2);
    cycle++;
  }
}
