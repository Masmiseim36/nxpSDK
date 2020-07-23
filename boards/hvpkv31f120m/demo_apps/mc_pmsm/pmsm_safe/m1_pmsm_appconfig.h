/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!
 * @file  {FM_project_loc}/pmsm_appconfig.h
 *
 * @date  March 28, 2020, 17:00:01
 *
 * @brief Automatically generated file for static configuration of the PMSM FOC
 *        application
 */

#ifndef _M1_PMSM_APPCONFIG_H_
#define _M1_PMSM_APPCONFIG_H_

/* Motor parameters:
 *----------------------------------------------------------------------
 * Pole-pair number                      = 4 [-]
 * Stator resistance                     = 1.45 [Ohms]
 * Direct axis inductance                = 0.00606 [H]
 * Quadrature axis inductance            = 0.005762 [H]
 * Back-EMF constant                     = 0.1135 [V.sec/rad]
 * Drive inertia                         = 0.0000016 [kg.m2]
 * Nominal current                       = 2 [A]
 */
#define M1_MOTOR_PP                        (4)

/*
 * Application scales:
 *----------------------------------------------------------------------
 */
#define M1_I_MAX                           (8.0F)
#define M1_U_DCB_MAX                       (433.0F)
#define M1_U_MAX                           (250.0F)
#define M1_N_MAX                           (4400.0F)
#define M1_FREQ_MAX                        (293.0F)
#define M1_E_MAX                           (50.0F)
#define M1_U_DCB_TRIP                      FRAC16(0.8)
#define M1_U_DCB_UNDERVOLTAGE              FRAC16(0.1270207852)
#define M1_U_DCB_OVERVOLTAGE               FRAC16(0.8)
#define M1_N_OVERSPEED                     FRAC16(0.95)
#define M1_N_MIN                           FRAC16(0.090909090909)
#define M1_N_NOM                           FRAC16(0.909090909091)
#define M1_I_PH_NOM                        FRAC16(0.5)
/* DC-bus voltage filter. */
#define M1_UDCB_IIR_B0                     FRAC32(0.015229513976)
#define M1_UDCB_IIR_B1                     FRAC32(0.015229513976)
#define M1_UDCB_IIR_A1                     FRAC32(0.469540972049)
/* Mechanical alignment. */
#define M1_ALIGN_VOLTAGE                   FRAC16(0.028)
#define M1_ALIGN_DURATION                  (800U)
/* Application counters. */
#define M1_FAULT_DURATION                  (3000U)
#define M1_FREEWHEEL_DURATION              (1250U)
/* Miscellaneous. */
#define M1_E_BLOCK_TRH                     FRAC16(0.14)
#define M1_E_BLOCK_PER                     (2000U)

/* Current loop control:
 *----------------------------------------------------------------------
 * Loop bandwidth                        = 280 [Hz]
 * Loop attenuation                      = 1 [-]
 * Loop sample time                      = 0.0001 [sec]
 */
#define M1_CLOOP_SAMPLE_TIME              (0.0001F)
/* Current controller output limit. */
#define M1_CLOOP_LIMIT                     FRAC16(0.9)
/* D-axis controller - PI parallel type. */
#define M1_D_KP_GAIN                       ACC32(0.6359237650702286)
#define M1_D_KI_GAIN                       ACC32(0.030010166588001014)
/* Q-axis controller - PI parallel type. */
#define M1_Q_KP_GAIN                       ACC32(0.6023705502202404)
#define M1_Q_KI_GAIN                       ACC32(0.02853441912212241)

/* Speed loop control:
 *----------------------------------------------------------------------
 * Loop bandwidth                        = 10 [Hz]
 * Loop attenuation
 * Loop sample time
 */
#define M1_SLOOP_SAMPLE_TIME              (0.001F)
/* Speed controller - PI parallel type. */
#define M1_SPEED_PI_PROP_GAIN              ACC32(0.1843067690106012)
#define M1_SPEED_PI_INTEG_GAIN             ACC32(0.00020734511513692633)
#define M1_SPEED_LOOP_HIGH_LIMIT           FRAC16(0.25)
#define M1_SPEED_LOOP_LOW_LIMIT            FRAC16(-0.25)

#define M1_SPEED_RAMP_UP                   FRAC32(0.000113636364)
#define M1_SPEED_RAMP_DOWN                 FRAC32(0.000113636364)

#define M1_SPEED_IIR_B0                    FRAC32(0.119528611806)
#define M1_SPEED_IIR_B1                    FRAC32(0.119528611806)
#define M1_SPEED_IIR_A1                    FRAC32(0.260942776389)

/* Sensorless BEMF DQ and tracking observer:
 *----------------------------------------------------------------------
 * Loop bandwidth                        = 280 [Hz]
 * Loop attenuation                      = 1 [-]
 * Loop sample time                      = 0.0001 [sec]
 */
/* BEMF DQ observer. */
#define M1_I_SCALE                         ACC32(0.97663174859)
#define M1_U_SCALE                         ACC32(0.503626107978)
#define M1_E_SCALE                         ACC32(0.100725221596)
#define M1_WI_SCALE                        ACC32(0.171148364713)
#define M1_BEMF_DQ_KP_GAIN                 ACC32(3.1796188253511435)
#define M1_BEMF_DQ_KI_GAIN                 ACC32(0.30010166588001014)

/* Tracking observer. */
#define M1_TO_KP_GAIN                      FRAC16(0.681818181818)
#define M1_TO_KP_SHIFT                     (-2)
#define M1_TO_KI_GAIN                      FRAC16(0.685438397147)
#define M1_TO_KI_SHIFT                     (-9)
#define M1_TO_THETA_GAIN                   FRAC16(0.938666666667)
#define M1_TO_THETA_SHIFT                  (-4)
/* Observer speed output filter. */
#define M1_TO_SPEED_IIR_B0                 FRAC32(0.055817605853)
#define M1_TO_SPEED_IIR_B1                 FRAC32(0.055817605853)
#define M1_TO_SPEED_IIR_A1                 FRAC32(0.388364788296)
/* Open loop start-up. */
#define M1_OL_START_RAMP_INC               FRAC32(0.000011363636)
#define M1_OL_START_I                      FRAC16(0.0875)
#define M1_MERG_SPEED_TRH                  FRAC16(0.113636363636)
#define M1_MERG_COEFF                      FRAC16(0.004577636719)

/* Control structure module - scalar control:
 *----------------------------------------------------------------------
 */
#define M1_SCALAR_VHZ_FACTOR_GAIN          ACC32(0.88)
#define M1_SCALAR_INTEG_GAIN               ACC32(0.058666666667)
#define M1_SCALAR_RAMP_UP                  FRAC32(0.000011363636)
#define M1_SCALAR_RAMP_DOWN                FRAC32(0.000227272727)


#endif /* _M1_PMSM_APPCONFIG_H_ */
