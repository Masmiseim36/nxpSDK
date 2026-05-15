/*
    * Copyright 2025 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

/*
    * FILE NAME: ../../../boards/frdmimxrt1186/demo_apps/mc_pmsm/pmsm_enc/cm7/m2_pmsm_appconfig.h
    * DATE: Fri Nov 21 2025, 10:22:07
*/

/*
{
    "mid": {
        "midInParamINom": 2,
        "midInParamNNom": 3000,
        "midPolePairIAPp": 2,
        "midElParamMeasRs": 0,
        "midElParamMeasLd": 0,
        "midElParamMeasLq": 0,
        "midElParamMeasUdt": 0,
        "midMechParamMeasKe": 0,
        "midMechParamMeasKt": 0,
        "midMechParamMeasJ": 0,
        "midMechParamMeasB": 0,
        "midMechParamMeasA": 0
    },
    "parameters": {
        "parametersPp": 2,
        "parametersRs": 0.45,
        "parametersLd": 0.000375,
        "parametersLq": 0.000429,
        "parametersKt": 0.0138,
        "parametersJ": 0.0000016,
        "parametersIphNom": 2,
        "parametersUphNom": 15,
        "parametersNnom": 4000,
        "parametersImax": 8.25,
        "parametersUdcbMax": 60.8,
        "parametersUdcbTrip": 29,
        "parametersUdcbUnder": 16,
        "parametersUdcbOver": 32,
        "parametersNover": 4399,
        "parametersNmin": 300,
        "parametersEblock": 0.8,
        "parametersEblockPer": 2000,
        "parametersNmax": 4400,
        "parametersUdcbIIRf0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 6,
        "parametersFreewheelDuration": 1.5,
        "parametersScalarUqMin": 1,
        "parametersAlignVoltage": 1.2,
        "parametersAlignDuration": 0.4,
        "parametersScalarVHzRatio": 100
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.0000625,
        "currentLoopF0": 288,
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
        "speedLoopUpperLimit": 2,
        "speedLoopLowerLimit": -2,
        "speedLoopSLKp": 0.003,
        "speedLoopSLKi": 0.09,
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
        "sensorlessStartupRamp": 5000,
        "sensorlessStartupCurrent": 0.35,
        "sensorlessMergingSpeed": 300,
        "sensorlessMergingCoeff": 100
    }
}
*/

/*
{
    "motorName": "Linix",
    "motorDescription": "Configuration for Linix motor."
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* MID*/
/* PARAMETERS*/
#define M1_MOTOR_PP (2)
#define M1_I_PH_NOM (2.0F)
#define M1_N_NOM (837.758F)
#define M1_I_MAX (8.25F)
#define M1_U_DCB_MAX (60.8F)
#define M1_U_DCB_TRIP (29.0F)
#define M1_U_DCB_UNDERVOLTAGE (16.0F)
#define M1_U_DCB_OVERVOLTAGE (32.0F)
#define M1_N_OVERSPEED (921.324F)
#define M1_N_MIN (62.8319F)
#define M1_E_BLOCK_TRH (0.8F)
#define M1_E_BLOCK_PER (2000)
#define M1_N_MAX (921.534F)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_FREEWHEEL_DURATION (1500)
#define M1_SCALAR_UQ_MIN (1.0F)
#define M1_ALIGN_VOLTAGE (1.2F)
#define M1_ALIGN_DURATION (6400)
#define M1_U_MAX (35.1029F)
#define M1_FREQ_MAX (146.667F)
#define M1_N_ANGULAR_MAX (4.77465F)
#define M1_UDCB_IIR_B0 (0.0192568F)
#define M1_UDCB_IIR_B1 (0.0192568F)
#define M1_UDCB_IIR_A1 (0.961486F)
#define M1_SCALAR_VHZ_FACTOR_GAIN (0.112500F)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0183333)
#define M1_SCALAR_RAMP_UP (0.0104167F)
#define M1_SCALAR_RAMP_DOWN (0.0104167F)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN (0.907168F)
#define M1_D_KI_GAIN (0.0383730F)
#define M1_Q_KP_GAIN (1.10260F)
#define M1_Q_KI_GAIN (0.0438987F)
#define M1_Q_IIR_ZC_B0 (0.0382894F)
#define M1_Q_IIR_ZC_B1 (0.0382894F)
#define M1_Q_IIR_ZC_A1 (0.923421F)
#define M1_CLOOP_LIMIT (0.519615F)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP (1.04720F)
#define M1_SPEED_RAMP_DOWN (1.04720F)
#define M1_SPEED_LOOP_HIGH_LIMIT (2.0F)
#define M1_SPEED_LOOP_LOW_LIMIT (-2.0F)
#define M1_SPEED_PI_PROP_GAIN (0.00728485F)
#define M1_SPEED_PI_INTEG_GAIN (0.000114430F)
#define M1_SPEED_IIR_B0 (0.239057F)
#define M1_SPEED_IIR_B1 (0.239057F)
#define M1_SPEED_IIR_A1 (0.521886F)
#define M1_SPEED_IIR_ZC_B0 (0.0154650F)
#define M1_SPEED_IIR_ZC_B1 (0.0154650F)
#define M1_SPEED_IIR_ZC_A1 (0.969070F)
/* POSITIONLOOP*/
#define M1_SERVO_POSITION_P_HIGH_LIMIT (628.319F)
#define M1_SERVO_POSITION_P_LOW_LIMIT (-628.319F)
#define M1_SERVO_POSITION_P_PROP_GAIN (526.379F)
#define M1_SERVO_FEED_FRWD_K1 (8.37758F)
#define M1_SERVO_FEED_FRWD_K2 (0.0333333F)
#define M1_SERVO_IIR_ZC_B0 (0.0591174F)
#define M1_SERVO_IIR_ZC_B1 (0.0591174F)
#define M1_SERVO_IIR_ZC_A1 (0.881765F)
#define M1_SERVO_SPEED_PI_PROP_GAIN (0.0218546F)
#define M1_SERVO_SPEED_PI_INTEG_GAIN (0.00137316F)
#define M1_SERVO_SPEED_PI_HIGH_LIMIT (4.0F)
#define M1_SERVO_SPEED_PI_LOW_LIMIT (-4.0F)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC (0.0654498F)
#define M1_OL_START_I (0.35F)
#define M1_MERG_SPEED_TRH (62.8319F)
#define M1_MERG_COEFF FRAC16(0.000610352)
#define M1_I_SCALE (0.930233F)
#define M1_U_SCALE (0.155039F)
#define M1_E_SCALE (0.155039F)
#define M1_WI_SCALE (0.0000665116F)
#define M1_BEMF_DQ_KP_GAIN (0.963717F)
#define M1_BEMF_DQ_KI_GAIN (0.0832748F)
#define M1_TO_KP_GAIN (879.646F)
#define M1_TO_KI_GAIN (12.0903F)
#define M1_TO_THETA_GAIN (0.0000198944F)
#define M1_TO_SPEED_IIR_B0 (0.0728205F)
#define M1_TO_SPEED_IIR_B1 (0.0728205F)
#define M1_TO_SPEED_IIR_A1 (0.854359F)
/* USER INPUT START */
#define AA 1
#define BB 3
#define CC 4
#define DD 2
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
