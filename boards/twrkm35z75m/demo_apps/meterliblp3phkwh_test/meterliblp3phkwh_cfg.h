/******************************************************************************
 *
 * Copyright 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 **************************************************************************************
 * Filter-Based Metering Library Configuration File, Created: Sun May 31 09:40:13 2015
 **************************************************************************************
 * @TAGNAME       = METERLIBLP3PH_CFG
 * @LOCKED        =         0
 * @FSAMPLE       =      1200
 * @DFACTOR       =         1
 * @IMAX          =  141.4214
 * @UMAX          =  350.0000
 * @FREQ          =        50
 * @COUNTERS_RES  =     10000
 * @PWR_THRESHOLD =    0.5000
 * @I_STARTING    =    0.0200
 * @APWR_OFS      =    0.0000
 * @RPWR_OFS      =    0.0000
 * @ENERGY_ATT    =    0.0000
 * @IMP_PER_KWH   =     50000
 * @IMP_PER_KVARH =     50000
 * @HPF_FCUT      =    0.3000
 * @LPF1_FCUT     =    1.5000
 * @LPF2_FCUT     =    3.0000
 * @KWIN_BETA     =    6.0672
 * @KWIN_GAIN     =    1.0000
 * @FIR_TAPS_CHG  =         0
 * @FIR_FREQ_MOD  =         0
 * @CUR_SENSOR    =         1
 * @LIB_TYPE      =         3
 * @MATH_TYPE     =         2
 * @KWH_ONLY      =         1
 * @SW_PH_CORR    =         0
 * @MCU_CORE      =         1
 **************************************************************************************/
#ifndef __METERLIBLP3PHKWH_CFG_H
#define __METERLIBLP3PHKWH_CFG_H

/**************************************************************************************
 * General parameters and scaling coefficients
 **************************************************************************************/
#define POWER_METER               3PH  /*!< Power meter topology                      */
#define CURRENT_SENSOR   PROPORTIONAL  /*!< Current sensor output characteristic      */
#define LIBRARY_PREFIX     METERLIBLP  /*!< Library prefix; low-power library         */
#define I_MAX                 141.421  /*!< Maximal current I-peak in amperes         */
#define U_MAX                 350.000  /*!< Maximal voltage U-peak in volts           */
#define F_NOM                      50  /*!< Nominal frequency in Hz                   */
#define COUNTER_RES             10000  /*!< Resolution of energy counters in inc/kWh  */
#define IMP_PER_KWH             50000  /*!< Impulses per kWh                          */
#define IMP_PER_KVARH           50000  /*!< Impulses per kVARh                        */
#define DECIM_FACTOR                1  /*!< Auxiliary calculations decimation factor  */
#define KWH_CALC_FREQ        1200.000  /*!< Sample frequency in Hz                    */
#define KVARH_CALC_FREQ      1200.000  /*!< Sample frequency in Hz                    */
/**************************************************************************************
 * Filter-based metering algorithm configuration structure
 **************************************************************************************/
#define METERLIBLP3PH_CFG                                                             \
{                                                                                     \
  U_MAX,                                                                              \
  I_MAX,                                                                              \
  FRAC32(((+0.5000)/(U_MAX*I_MAX/2.0))),                                              \
  FRAC32((+0.0200)/I_MAX),                                                            \
  {{0l,0l,0l},{0l,0l,0l}},{{0l,0l,0l},{0l,0l,0l}},{{0l,0l,0l},{0l,0l,0l}},            \
  {{+32741,-32742,-32716}},                                                           \
  {{ +4313, +4313,-32768}},                                                           \
  {{0,0l},{0,0l}},                                                                    \
  {0,0l},                                                                             \
  {{0,0l},{0,0l}},                                                                    \
  {{0,0l},{0,0l}},                                                                    \
  {0,0l},                                                                             \
  {{0,0l},{0,0l}},                                                                    \
  {{0,0l},{0,0l}},                                                                    \
  {0,0l},                                                                             \
  {{0,0l},{0,0l}},                                                                    \
  {  49,                                                                              \
    {                                                                                 \
      FRAC16(0.0),FRAC16(-0.00073728465714),FRAC16(0.0),FRAC16(-0.00196750272687),    \
      FRAC16(0.0),FRAC16(-0.00411945802255),FRAC16(0.0),FRAC16(-0.00756839142185),    \
      FRAC16(0.0),FRAC16(-0.01278720365088),FRAC16(0.0),FRAC16(-0.02040684105768),    \
      FRAC16(0.0),FRAC16(-0.03136483560542),FRAC16(0.0),FRAC16(-0.04728105184137),    \
      FRAC16(0.0),FRAC16(-0.07151114503989),FRAC16(0.0),FRAC16(-0.11276139617420),    \
      FRAC16(0.0),FRAC16(-0.20318408017719),FRAC16(0.0),FRAC16(-0.63356345988777),    \
      FRAC16(0.0),FRAC16(+0.63356345988777),FRAC16(0.0),FRAC16(+0.20318408017719),    \
      FRAC16(0.0),FRAC16(+0.11276139617420),FRAC16(0.0),FRAC16(+0.07151114503989),    \
      FRAC16(0.0),FRAC16(+0.04728105184137),FRAC16(0.0),FRAC16(+0.03136483560542),    \
      FRAC16(0.0),FRAC16(+0.02040684105768),FRAC16(0.0),FRAC16(+0.01278720365088),    \
      FRAC16(0.0),FRAC16(+0.00756839142185),FRAC16(0.0),FRAC16(+0.00411945802255),    \
      FRAC16(0.0),FRAC16(+0.00196750272687),FRAC16(0.0),FRAC16(+0.00073728465714),    \
      FRAC16(0.0)                                                                     \
    },                                                                                \
     25,                                                                              \
    {                                                                                 \
      FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
      FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
      FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
      FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
      FRAC16(-1.0)                                                                    \
    }                                                                                 \
  },                                                                                  \
  {                                                                                   \
    {                                                                                 \
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  \
      0,0,0,0,0,0,0,0,0,0                                                             \
    },                                                                                \
    0l,                                                                               \
    {                                                                                 \
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                               \
    },                                                                                \
    0l                                                                                \
  },                                                                                  \
  {                                                                                   \
    {                                                                                 \
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  \
      0,0,0,0,0,0,0,0,0,0                                                             \
    },                                                                                \
    0l,                                                                               \
    {                                                                                 \
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                               \
    },                                                                                \
    0l                                                                                \
  },                                                                                  \
  {                                                                                   \
    {                                                                                 \
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  \
      0,0,0,0,0,0,0,0,0,0                                                             \
    },                                                                                \
    0l,                                                                               \
    {                                                                                 \
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                               \
    },                                                                                \
    0l                                                                                \
  },                                                                                  \
  {  24,{0,0,0},{0,0,0}},                                                             \
  {                                                                                   \
    {  +128,  +128,-32512},                                                           \
    {  +128,  +128,-32512},                                                           \
  },                                                                                  \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {0l,0l,0,0l},                                                                       \
  {                                                                                   \
    FRAC32((+0.0000/(U_MAX*I_MAX))),FRAC16(+1.0000),  METERLIBLP_KWH_DR(   10000),    \
    {0ll,0ll,0ll},0ll,0l,FRAC16(-1.0)                                                 \
  },                                                                                  \
  {                                                                                   \
    FRAC32((+0.0000/(U_MAX*I_MAX))),FRAC16(+1.0000),METERLIBLP_KVARH_DR(   10000),    \
    {0ll,0ll,0ll},0ll,0l,FRAC16(-1.0)                                                 \
  }                                                                                   \
}
#endif  /* __METERLIBLP3PHKWH_CFG_H */