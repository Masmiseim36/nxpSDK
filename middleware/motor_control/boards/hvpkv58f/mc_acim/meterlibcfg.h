/*******************************************************************************
*
* Copyright 2015 Freescale Semiconductor, Inc.
*
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
*
******************************************************************************/
/******************************************************************************
 * Filter-Based Metering Library Configuration File, 
 * Created: Wed Aug 26 08:19:45 2015
 ******************************************************************************
 * @TAGNAME       = METERLIB_CFG
 * @LOCKED        =         0
 * @FSAMPLE       =      1000
 * @DFACTOR       =         1
 * @IMAX          =    8.0000
 * @UMAX          =  250.0000
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
 * @LPF1_FCUT     =    0.5000
 * @LPF2_FCUT     =    3.0000
 * @KWIN_BETA     =    6.0672
 * @KWIN_GAIN     =    1.0000
 * @FIR_TAPS_CHG  =         0
 * @FIR_FREQ_MOD  =         0
 * @CUR_SENSOR    =         1
 * @LIB_TYPE      =         3
 * @MATH_TYPE     =         2
 * @KWH_ONLY      =         0
 * @SW_PH_CORR    =         0
 * @MCU_CORE      =         3
 ******************************************************************************/
#ifndef _METERLIBCFG_H_
#define _METERLIBCFG_H_

/******************************************************************************
 * general parameters and scaling coefficients
 ******************************************************************************/
#define POWER_METER        (3PH)                                                /* power meter topology */
#define CURRENT_SENSOR     (PROPORTIONAL)                                       /* current sensor output characteristic */
#define LIBRARY_PREFIX     (METERLIBLP)                                         /* library prefix; low-power library */
#define I_MAX              (8.000)                                              /* maximal current I-peak in amperes */
#define U_MAX              (250.000)                                            /* maximal voltage U-peak in volts */
#define F_NOM              (50)                                                 /* nominal frequency in Hz */
#define COUNTER_RES        (10000)                                              /* resolution of energy counters in inc/kWh */
#define IMP_PER_KWH        (50000)                                              /* Impulses per kWh */
#define IMP_PER_KVARH      (50000)                                              /* Impulses per kVARh */
#define DECIM_FACTOR       (1)                                                  /* auxiliary calculations decimation factor */
#define KWH_CALC_FREQ      (1000.000)                                           /* sample frequency in Hz */
#define KVARH_CALC_FREQ    (1000.000)                                           /* sample frequency in Hz */
/******************************************************************************
 * filter-based metering algorithm configuration structure
 ******************************************************************************/
#define METERLIB_CFG                                                                        \
{                                                                                           \
    U_MAX,                                                                                  \
    I_MAX,                                                                                  \
    FRAC32(((+0.5000)/(U_MAX*I_MAX/2.0))),                                                  \
    FRAC32((+0.0200)/I_MAX),                                                                \
    {{0l,0l,0l},{0l,0l,0l}},{{0l,0l,0l},{0l,0l,0l}},{{0l,0l,0l},{0l,0l,0l}},                \
    {{+32736,-32737,-32706}},                                                               \
    {{ +5189, +5189,-32768}},                                                               \
    {{0,0l},{0,0l}},                                                                        \
    {0,0l},                                                                                 \
    {{0,0l},{0,0l}},                                                                        \
    {{0,0l},{0,0l}},                                                                        \
    {0,0l},                                                                                 \
    {{0,0l},{0,0l}},                                                                        \
    {{0,0l},{0,0l}},                                                                        \
    {0,0l},                                                                                 \
    {{0,0l},{0,0l}},                                                                        \
    {  41,                                                                                  \
        {                                                                                   \
            FRAC16(0.0),FRAC16(-0.00099353190431),FRAC16(0.0),FRAC16(-0.00300094510483),    \
            FRAC16(0.0),FRAC16(-0.00678598226787),FRAC16(0.0),FRAC16(-0.01320271635306),    \
            FRAC16(0.0),FRAC16(-0.02341638841178),FRAC16(0.0),FRAC16(-0.03923647621935),    \
            FRAC16(0.0),FRAC16(-0.06413032414860),FRAC16(0.0),FRAC16(-0.10680982700323),    \
            FRAC16(0.0),FRAC16(-0.19931713703615),FRAC16(0.0),FRAC16(-0.63222258138991),    \
            FRAC16(0.0),FRAC16(+0.63222258138991),FRAC16(0.0),FRAC16(+0.19931713703615),    \
            FRAC16(0.0),FRAC16(+0.10680982700323),FRAC16(0.0),FRAC16(+0.06413032414860),    \
            FRAC16(0.0),FRAC16(+0.03923647621935),FRAC16(0.0),FRAC16(+0.02341638841178),    \
            FRAC16(0.0),FRAC16(+0.01320271635306),FRAC16(0.0),FRAC16(+0.00678598226787),    \
            FRAC16(0.0),FRAC16(+0.00300094510483),FRAC16(0.0),FRAC16(+0.00099353190431),    \
            FRAC16(0.0)                                                                     \
        },                                                                                  \
         21,                                                                                \
        {                                                                                   \
            FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
            FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
            FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),FRAC16(0.0),        \
            FRAC16(0.0),FRAC16(0.0),FRAC16(-1.0)                                            \
        }                                                                                   \
    },                                                                                      \
    {                                                                                       \
        {                                                                                   \
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  \
            0,0                                                                             \
        },                                                                                  \
        0l,                                                                                 \
        {                                                                                   \
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                                       \
        },                                                                                  \
        0l                                                                                  \
    },                                                                                      \
    {                                                                                       \
        {                                                                                   \
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  \
            0,0                                                                             \
        },                                                                                  \
        0l,                                                                                 \
        {                                                                                   \
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                                       \
        },                                                                                  \
        0l                                                                                  \
    },                                                                                      \
    {                                                                                       \
        {                                                                                   \
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  \
            0,0                                                                             \
        },                                                                                  \
        0l,                                                                                 \
        {                                                                                   \
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                                       \
        },                                                                                  \
        0l                                                                                  \
    },                                                                                      \
    {20,{0,0,0},{0,0,0}},                                                                   \
    {                                                                                       \
        {   +51,   +51,-32666},                                                             \
        {   +51,   +51,-32666},                                                             \
    },                                                                                      \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {0l,0l,0,0l},                                                                           \
    {                                                                                       \
        FRAC32((+0.0000/(U_MAX*I_MAX))),FRAC16(+1.0000),  METERLIBLP_KWH_DR(10000),      \
        {0ll,0ll,0ll},0ll,0l,FRAC16(-1.0)                                                   \
    },                                                                                      \
    {                                                                                       \
        FRAC32((+0.0000/(U_MAX*I_MAX))),FRAC16(+1.0000),METERLIBLP_KVARH_DR(10000),      \
        {0ll,0ll,0ll},0ll,0l,FRAC16(-1.0)                                                   \
    }                                                                                       \
}
#endif  /* _METERLIBCFG_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
