/*
    * Copyright 2025 NXP 
    *
    * SPDX-License-Identifier: BSD-3-Clause 
*/

/*
    * FILE NAME: ../../../examples/_boards/imx943evk/demo_apps/mc_pmsm/pmsm_enc_dual/cm7_core1/m1_pmsm_appconfig.h
    * DATE: Fri Nov 21 2025, 10:22:07
*/

/*
{
    "mid": {
        "midInParamINom": 2,
        "midInParamNNom": 3000,
        "midPolePairIAPp": 5,
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
        "parametersPp": 5,
        "parametersRs": 0.2,
        "parametersLd": 0.00022,
        "parametersLq": 0.000218,
        "parametersKt": 0.12,
        "parametersJ": 0.0000268,
        "parametersIphNom": 8,
        "parametersUphNom": 15,
        "parametersNnom": 4000,
        "parametersImax": 34.2,
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
        "parametersScalarUqMin": 0.7,
        "parametersAlignVoltage": 0.6,
        "parametersAlignDuration": 1,
        "parametersScalarVHzRatio": 100
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.00003125,
        "currentLoopF0": 300,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.00025,
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
        "positionLoopSampleTime": 0.00025,
        "positionLoopF0": 10,
        "positionLoopKsi": 1,
        "servo_positionLoopUpperLimit": 3000,
        "servo_positionLoopLowerLimit": -3000,
        "servo_speedLoopUpperLimit": 4,
        "servo_speedLoopLowerLimit": -4
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 150,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 35,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessTrackObsrvIIRSpeedCutOff": 400,
        "sensorlessStartupRamp": 3000,
        "sensorlessStartupCurrent": 0.6,
        "sensorlessMergingSpeed": 500,
        "sensorlessMergingCoeff": 100
    }
}
*/

/*
{
    "motorName": "HEIDRIVE HMD06-011-048-60-OPB1MW230",
    "motorDescription": "64kHz - constants are not tuned for pmsm_servo example"
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* MID*/
/* PARAMETERS*/
#define M1_MOTOR_PP (5)
#define M1_I_PH_NOM (8.0F)
#define M1_N_NOM (2094.40F)
#define M1_I_MAX (34.2F)
#define M1_U_DCB_MAX (60.8F)
#define M1_U_DCB_TRIP (28.0F)
#define M1_U_DCB_UNDERVOLTAGE (16.0F)
#define M1_U_DCB_OVERVOLTAGE (30.0F)
#define M1_N_OVERSPEED (2303.31F)
#define M1_N_MIN (157.080F)
#define M1_E_BLOCK_TRH (0.2F)
#define M1_E_BLOCK_PER (2000)
#define M1_N_MAX (2303.83F)
#define M1_CALIB_DURATION (800)
#define M1_FAULT_DURATION (24000)
#define M1_FREEWHEEL_DURATION (6000)
#define M1_SCALAR_UQ_MIN (1.0F)
#define M1_ALIGN_VOLTAGE (0.6F)
#define M1_ALIGN_DURATION (32000)
#define M1_U_MAX (35.1029F)
#define M1_FREQ_MAX (366.667F)
#define M1_N_ANGULAR_MAX (1.90986F)
#define M1_UDCB_IIR_B0 (0.00972203F)
#define M1_UDCB_IIR_B1 (0.00972203F)
#define M1_UDCB_IIR_A1 (0.980556F)
#define M1_SCALAR_VHZ_FACTOR_GAIN (0.250000F)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0229167)
#define M1_SCALAR_RAMP_UP (0.0130208F)
#define M1_SCALAR_RAMP_DOWN (0.0130208F)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN (0.629380F)
#define M1_D_KI_GAIN (0.0122136F)
#define M1_Q_KP_GAIN (0.621841F)
#define M1_Q_KI_GAIN (0.0121026F)
#define M1_Q_IIR_ZC_B0 (0.0190910F)
#define M1_Q_IIR_ZC_B1 (0.0190910F)
#define M1_Q_IIR_ZC_A1 (0.961818F)
#define M1_CLOOP_LIMIT (0.519615F)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP (0.654498F)
#define M1_SPEED_RAMP_DOWN (0.654498F)
#define M1_SPEED_LOOP_HIGH_LIMIT (4.0F)
#define M1_SPEED_LOOP_LOW_LIMIT (-4.0F)
#define M1_SPEED_PI_PROP_GAIN (0.00561298F)
#define M1_SPEED_PI_INTEG_GAIN (0.0000220421F)
#define M1_SPEED_IIR_B0 (0.0728205F)
#define M1_SPEED_IIR_B1 (0.0728205F)
#define M1_SPEED_IIR_A1 (0.854359F)
#define M1_SPEED_IIR_ZC_B0 (0.00391163F)
#define M1_SPEED_IIR_ZC_B1 (0.00391163F)
#define M1_SPEED_IIR_ZC_A1 (0.992177F)
/* POSITIONLOOP*/
#define M1_SERVO_POSITION_P_HIGH_LIMIT (1570.80F)
#define M1_SERVO_POSITION_P_LOW_LIMIT (-1570.80F)
#define M1_SERVO_POSITION_P_PROP_GAIN (657.974F)
#define M1_SERVO_FEED_FRWD_K1 (20.9440F)
#define M1_SERVO_FEED_FRWD_K2 (0.166667F)
#define M1_SERVO_IIR_ZC_B0 (0.00779278F)
#define M1_SERVO_IIR_ZC_B1 (0.00779278F)
#define M1_SERVO_IIR_ZC_A1 (0.984414F)
#define M1_SERVO_SPEED_PI_PROP_GAIN (0.00841947F)
#define M1_SERVO_SPEED_PI_INTEG_GAIN (0.0000661263F)
#define M1_SERVO_SPEED_PI_HIGH_LIMIT (4.0F)
#define M1_SERVO_SPEED_PI_LOW_LIMIT (-4.0F)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC (0.0490874F)
#define M1_OL_START_I (0.6F)
#define M1_MERG_SPEED_TRH (261.799F)
#define M1_MERG_COEFF FRAC16(0.00131226)
#define M1_I_SCALE (0.972376F)
#define M1_U_SCALE (0.138122F)
#define M1_E_SCALE (0.138122F)
#define M1_WI_SCALE (0.0000301105F)
#define M1_BEMF_DQ_KP_GAIN (0.214690F)
#define M1_BEMF_DQ_KI_GAIN (0.00610682F)
#define M1_TO_KP_GAIN (439.823F)
#define M1_TO_KI_GAIN (1.51128F)
#define M1_TO_THETA_GAIN (0.00000994718F)
#define M1_TO_SPEED_IIR_B0 (0.0377861F)
#define M1_TO_SPEED_IIR_B1 (0.0377861F)
#define M1_TO_SPEED_IIR_A1 (0.924428F)
/* USER INPUT START */
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
