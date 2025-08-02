/*
    * Copyright 2025 NXP 
    *
    * SPDX-License-Identifier: BSD-3-Clause 
*/

/*
    * FILE NAME: ../../../examples/_boards/evkmimxrt1180/demo_apps/mc_pmsm/pmsm_enc/cm7/m1_pmsm_appconfig.h
    * DATE: Thu Apr 24 2025, 09:22:56
*/

/*
{
    "parameters": {
        "parametersPP": 4,
        "parametersRs": 0.54,
        "parametersLd": 0.0003356,
        "parametersLq": 0.000218,
        "parametersKe": 0.05477461,
        "parametersJ": 0.00001,
        "parametersIphNom": 8,
        "parametersUphNom": 15,
        "parametersNnom": 4000,
        "parametersImax": 8.25,
        "parametersUdcbMax": 60.8,
        "parametersUdcbTrip": 28,
        "parametersUdcbUnder": 16,
        "parametersUdcbOver": 30,
        "parametersNover": 4399,
        "parametersNmin": 300,
        "parametersEblock": 0.2,
        "parametersEblockPer": 2000,
        "parametersNmax": 4400,
        "parametersUdcbIIRf0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 6,
        "parametersFreewheelDuration": 1.5,
        "parametersScalarUqMin": 1,
        "parametersAlignVoltage": 1.2,
        "parametersAlignDuration": 1,
        "parametersScalarVHzRatio": 100
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.0000625,
        "currentLoopF0": 1000,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.001,
        "speedLoopF0": 10,
        "speedLoopKsi": 1,
        "speedLoopIncUp": 5000,
        "speedLoopIncDown": 5000,
        "speedLoopCutOffFreq": 100,
        "speedLoopUpperLimit": 4,
        "speedLoopLowerLimit": -4,
        "speedLoopSLKp": 0.009202212,
        "speedLoopSLKi": 0.00008095,
        "speedLoopManualConstantTunning": false
    },
    "positionLoop": {
        "positionLoopSampleTime": 0.001,
        "positionLoopF0": 20,
        "positionLoopKsi": 1,
        "servo_positionLoopUpperLimit": 3000,
        "servo_positionLoopLowerLimit": -3000,
        "servo_speedLoopUpperLimit": 4,
        "servo_speedLoopLowerLimit": -4
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 300,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 70,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessTrackObsrvIIRSpeedCutOff": 400,
        "sensorlessStartupRamp": 3000,
        "sensorlessStartupCurrent": 0.65,
        "sensorlessMergingSpeed": 500,
        "sensorlessMergingCoeff": 100
    }
}
*/

/*
{
    "motorName": "Teknic M-2311P-LN-08D",
    "motorDescription": "Curent loop sample frequency 16KHz"
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* PARAMETERS*/
#define M1_MOTOR_PP (4)
#define M1_I_PH_NOM (8.0F)
#define M1_N_NOM (1675.52F)
#define M1_I_MAX (8.25F)
#define M1_U_DCB_MAX (60.8F)
#define M1_U_DCB_TRIP (28.0F)
#define M1_U_DCB_UNDERVOLTAGE (16.0F)
#define M1_U_DCB_OVERVOLTAGE (30.0F)
#define M1_N_OVERSPEED (1842.65F)
#define M1_N_MIN (125.664F)
#define M1_E_BLOCK_TRH (0.2F)
#define M1_E_BLOCK_PER (2000)
#define M1_N_MAX (1843.07F)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_FREEWHEEL_DURATION (1500)
#define M1_SCALAR_UQ_MIN (1.0F)
#define M1_ALIGN_VOLTAGE (1.2F)
#define M1_ALIGN_DURATION (16000)
#define M1_U_MAX (35.1029F)
#define M1_FREQ_MAX (293.333F)
#define M1_N_ANGULAR_MAX (2.38732F)
#define M1_UDCB_IIR_B0 (0.0192568F)
#define M1_UDCB_IIR_B1 (0.0192568F)
#define M1_UDCB_IIR_A1 (0.961486F)
#define M1_SCALAR_VHZ_FACTOR_GAIN (0.0562500F)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0366667)
#define M1_SCALAR_RAMP_UP (0.0208333F)
#define M1_SCALAR_RAMP_DOWN (0.0208333F)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN (3.67727F)
#define M1_D_KI_GAIN (0.414030F)
#define M1_Q_KP_GAIN (2.19947F)
#define M1_Q_KI_GAIN (0.268947F)
#define M1_Q_IIR_ZC_B0 (0.108955F)
#define M1_Q_IIR_ZC_B1 (0.108955F)
#define M1_Q_IIR_ZC_A1 (0.782090F)
#define M1_CLOOP_LIMIT (0.519615F)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP (2.09440F)
#define M1_SPEED_RAMP_DOWN (2.09440F)
#define M1_SPEED_LOOP_HIGH_LIMIT (4.0F)
#define M1_SPEED_LOOP_LOW_LIMIT (-4.0F)
#define M1_SPEED_PI_PROP_GAIN (0.00573549F)
#define M1_SPEED_PI_INTEG_GAIN (0.0000900929F)
#define M1_SPEED_IIR_B0 (0.239057F)
#define M1_SPEED_IIR_B1 (0.239057F)
#define M1_SPEED_IIR_A1 (0.521886F)
#define M1_SPEED_IIR_ZC_B0 (0.0154650F)
#define M1_SPEED_IIR_ZC_B1 (0.0154650F)
#define M1_SPEED_IIR_ZC_A1 (0.969070F)
/* POSITIONLOOP*/
#define M1_SERVO_POSITION_P_HIGH_LIMIT (1256.64F)
#define M1_SERVO_POSITION_P_LOW_LIMIT (-1256.64F)
#define M1_SERVO_POSITION_P_PROP_GAIN (1052.76F)
#define M1_SERVO_FEED_FRWD_K1 (16.7552F)
#define M1_SERVO_FEED_FRWD_K2 (0.0666667F)
#define M1_SERVO_IIR_ZC_B0 (0.0591174F)
#define M1_SERVO_IIR_ZC_B1 (0.0591174F)
#define M1_SERVO_IIR_ZC_A1 (0.881765F)
#define M1_SERVO_SPEED_PI_PROP_GAIN (0.0172065F)
#define M1_SERVO_SPEED_PI_INTEG_GAIN (0.00108111F)
#define M1_SERVO_SPEED_PI_HIGH_LIMIT (4.0F)
#define M1_SERVO_SPEED_PI_LOW_LIMIT (-4.0F)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC (0.0785398F)
#define M1_OL_START_I (0.65F)
#define M1_MERG_SPEED_TRH (209.440F)
#define M1_MERG_COEFF FRAC16(0.00207520)
#define M1_I_SCALE (0.908623F)
#define M1_U_SCALE (0.169216F)
#define M1_E_SCALE (0.169216F)
#define M1_WI_SCALE (0.0000368891F)
#define M1_BEMF_DQ_KP_GAIN (0.725182F)
#define M1_BEMF_DQ_KI_GAIN (0.0745254F)
#define M1_TO_KP_GAIN (879.646F)
#define M1_TO_KI_GAIN (12.0903F)
#define M1_TO_THETA_GAIN (0.0000198944F)
#define M1_TO_SPEED_IIR_B0 (0.0728205F)
#define M1_TO_SPEED_IIR_B1 (0.0728205F)
#define M1_TO_SPEED_IIR_A1 (0.854359F)
/* USER INPUT START */
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
