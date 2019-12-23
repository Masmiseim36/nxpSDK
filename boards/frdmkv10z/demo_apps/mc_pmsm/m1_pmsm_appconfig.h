/**********************************************************************/
// File Name: {FM_project_loc}/../../../src/projects/frdm-kv10z/pmsm_appconfig.h
//
// Date:  February 22, 2016, 9:41:24
//
// Automatically generated file for static configuration of the PMSM FOC application
/**********************************************************************/

#ifndef __M1_PMSM_APPCONFIG_H
#define __M1_PMSM_APPCONFIG_H

// Motor Parameters
//----------------------------------------------------------------------
// Pole-pair number                      = 2 [-]
// Stator resistance                     = 0.55 [Ohms]
// Direct axis inductance                = 0.000403 [H]
// Quadrature axis inductance            = 0.000512 [H]
// Back-EMF constant                     = 0.0154 [V.sec/rad]
// Drive inertia                         = 0.0000016 [kg.m2]
// Nominal current                       = 2 [A]

#define M1_MOTOR_PP (2)
//----------------------------------------------------------------------

// Application scales
//----------------------------------------------------------------------
#define M1_I_MAX (8.25F)
#define M1_U_DCB_MAX (60.8F)
#define M1_U_MAX (35.1F)
#define M1_N_MAX (4400.0F)
#define M1_FREQ_MAX (147.0F)
#define M1_E_MAX (15.1F)
#define M1_U_DCB_TRIP FRAC16(0.526315789474)
#define M1_U_DCB_UNDERVOLTAGE FRAC16(0.263157894737)
#define M1_U_DCB_OVERVOLTAGE FRAC16(0.476973684211)
#define M1_N_OVERSPEED FRAC16(0.977272727273)
#define M1_N_MIN FRAC16(0.090909090909)
#define M1_N_NOM FRAC16(0.909090909091)
#define M1_I_PH_NOM FRAC16(0.242424242424)
// DCB Voltage Filter
#define M1_UDCB_IIR_B0 FRAC32(0.015229513976)
#define M1_UDCB_IIR_B1 FRAC32(0.015229513976)
#define M1_UDCB_IIR_A1 FRAC32(0.469540972049)
// Mechanical Alignment
#define M1_ALIGN_VOLTAGE FRAC16(0.02849002849)
#define M1_ALIGN_DURATION (400)
// Application counters
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_FREEWHEEL_DURATION (1500)
// Miscellaneous
#define M1_U_BLOCK_TRH FRAC16(0.092715231788)

// Current Loop Control
//----------------------------------------------------------------------
// Loop Bandwidth                        = 280 [Hz]
// Loop Attenuation
// Loop sample time
//----------------------------------------------------------------------
// Current Controller Output Limit
#define M1_CLOOP_LIMIT FRAC16(0.9)
// D-axis Controller - Parallel type
#define M1_D_KP_GAIN ACC32(0.20401456968733286)
#define M1_D_KI_GAIN ACC32(0.014658775105582406)
// Q-axis Controller - Parallel type
#define M1_Q_KP_GAIN ACC32(0.2941594829918769)
#define M1_Q_KI_GAIN ACC32(0.018623555469127025)

// Speed Loop Control
//----------------------------------------------------------------------
// Loop Bandwidth                        = 20 [Hz]
// Loop Attenuation
// Loop sample time
//----------------------------------------------------------------------
// Speed Controller - Parallel type
#define M1_SPEED_PI_PROP_GAIN ACC32(0.33510321638291124)
#define M1_SPEED_PI_INTEG_GAIN ACC32(0.001005309649148734)
#define M1_SPEED_LOOP_HIGH_LIMIT FRAC16(0.363636363636)
#define M1_SPEED_LOOP_LOW_LIMIT FRAC16(-0.363636363636)

#define M1_SPEED_RAMP_UP FRAC32(0.002272727273)
#define M1_SPEED_RAMP_DOWN FRAC32(0.002272727273)

#define M1_SPEED_LOOP_CNTR (10)
#define M1_SPEED_LOOP_FREQ (1000)

#define M1_SPEED_IIR_B0 FRAC32(0.119528611806)
#define M1_SPEED_IIR_B1 FRAC32(0.119528611806)
#define M1_SPEED_IIR_A1 FRAC32(0.260942776389)

// Sensorless BEMF DQ nad Tracking Observer
//----------------------------------------------------------------------
// Loop Bandwidth                        = 300 [Hz]
// Loop Attenuation
// Loop sample time
//----------------------------------------------------------------------
// Bemf DQ Observer
#define M1_I_SCALE ACC32(0.879912663756)
#define M1_U_SCALE ACC32(0.928940055578)
#define M1_E_SCALE ACC32(0.399629482599)
#define M1_WI_SCALE ACC32(0.103018630714)
#define M1_BEMF_DQ_KP_GAIN ACC32(0.5295703450349137)
#define M1_BEMF_DQ_KI_GAIN ACC32(0.07823197485899247)

// Bemf DQ Observer
#define M1_TO_KP_GAIN FRAC16(0.545454545455)
#define M1_TO_KP_SHIFT (-1)
#define M1_TO_KI_GAIN FRAC16(0.877361148348)
#define M1_TO_KI_SHIFT (-9)
#define M1_TO_THETA_GAIN FRAC16(0.938666666667)
#define M1_TO_THETA_SHIFT (-5)
// Observer speed output filter
#define M1_TO_SPEED_IIR_B0 FRAC32(0.055817605853)
#define M1_TO_SPEED_IIR_B1 FRAC32(0.055817605853)
#define M1_TO_SPEED_IIR_A1 FRAC32(0.388364788296)
// Open loop start-up
#define M1_OL_START_RAMP_INC FRAC32(0.000068181818)
#define M1_OL_START_I FRAC16(0.072727272727)
#define M1_MERG_SPEED_TRH FRAC16(0.113636363636)
#define M1_MERG_COEFF FRAC16(0.013732910156)

// Control Structure Module - Scalar Control
//----------------------------------------------------------------------
#define M1_SCALAR_VHZ_FACTOR_GAIN FRAC16(0.4700854700854701)
#define M1_SCALAR_VHZ_FACTOR_SHIFT (0)
#define M1_SCALAR_INTEG_GAIN ACC32(0.029333333333)
#define M1_SCALAR_RAMP_UP FRAC32(0.000227272727)
#define M1_SCALAR_RAMP_DOWN FRAC32(0.000227272727)

// Motor Identification Module
//----------------------------------------------------------------------
#define M1_CHAR_NUMBER_OF_POINTS_BASE (6)
#define M1_CHAR_CURRENT_POINT_NUMBERS (65)
#define M1_CHAR_NUMBER_OF_POINTS_HALF (32)
#define M1_TIME_50MS (500)
#define M1_TIME_100MS (1000)
#define M1_TIME_300MS (3000)
#define M1_TIME_600MS (6000)
#define M1_TIME_1200MS (12000)
#define M1_TIME_2400MS (24000)
#define M1_K_RESCALE_DCB_TO_PHASE_HALF FRAC16(0.866096866096866)
#define M1_K_ANGLE_INCREMENT FRAC16(0.2)
#define M1_INV_MOD_INDEX FRAC16(0.4999587446189769)
#define M1_K_I_50MA FRAC16(0.006060606060606061)

#endif

// End of generated file
/**********************************************************************/
