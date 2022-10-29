/*
	* Copyright 2021 NXP
	*
	* SPDX-License-Identifier: BSD-3-Clause
*/

/*
    * FILE NAME: ../../source/m2_pmsm_appconfig.h
    * DATE: Wed Jun 22 2022, 07:43:57
*/

/*
{
    "parameters": {
        "parametersPP": 4,
        "parametersRs": 1.45,
        "parametersLd": 0.00606,
        "parametersLq": 0.005762,
        "parametersKe": 0.1135,
        "parametersJ": 0.0000016,
        "parametersIphNom": 2,
        "parametersUphNom": 200,
        "parametersNnom": 4000,
        "parametersImax": 8,
        "parametersUdcbMax": 433,
        "parametersUdcbTrip": 346.3,
        "parametersUdcbUnder": 173.2,
        "parametersUdcbOver": 346.4,
        "parametersNover": 4180,
        "parametersNmin": 400,
        "parametersEblock": 7,
        "parametersEblockPer": 2000,
        "parametersNmax": 4400,
        "parametersUdcbIIRf0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 6,
        "parametersFreewheelDuration": 1.5,
        "parametersScalarUqMin": 4,
        "parametersAlignVoltage": 6,
        "parametersAlignDuration": 0.8
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.0001,
        "currentLoopF0": 280,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.001,
        "speedLoopF0": 28,
        "speedLoopKsi": 1,
        "speedLoopIncUp": 4000,
        "speedLoopIncDown": 4000,
        "speedLoopCutOffFreq": 100,
        "speedLoopUpperLimit": 2,
        "speedLoopLowerLimit": -2,
        "speedLoopSLKp": 0.00035,
        "speedLoopSLKi": 0.000001,
        "speedLoopManualConstantTunning": true,
        "speedLoopPLKp": 0.12
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 280,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 25,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessStartupRamp": 1500,
        "sensorlessStartupCurrent": 0.2,
        "sensorlessMergingSpeed": 500,
        "sensorlessMergingCoeff": 100
    }
}
*/

/*
{
    "motorName": "Mige 60CST",
    "motorDescription": "Default config for Mige motor."
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* PARAMETERS*/
#define M1_MOTOR_PP (4)
#define M1_I_PH_NOM (2.0F)
#define M1_N_NOM (1675.52F)
#define M1_I_MAX (8.0F)
#define M1_U_DCB_MAX (433.0F)
#define M1_U_DCB_TRIP (346.3F)
#define M1_U_DCB_UNDERVOLTAGE (173.2F)
#define M1_U_DCB_OVERVOLTAGE (346.4F)
#define M1_N_OVERSPEED (1750.91F)
#define M1_N_MIN (167.552F)
#define M1_E_BLOCK_TRH (7.0F)
#define M1_E_BLOCK_PER (2000)
#define M1_N_MAX (1843.07F)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_FREEWHEEL_DURATION (1500)
#define M1_SCALAR_UQ_MIN (4.0F)
#define M1_ALIGN_VOLTAGE (6.0F)
#define M1_ALIGN_DURATION (800)
#define M1_U_MAX (249.993F)
#define M1_FREQ_MAX (293.333F)
#define M1_N_ANGULAR_MAX (2.38732F)
#define M1_UDCB_IIR_B0 (0.0304590F)
#define M1_UDCB_IIR_B1 (0.0304590F)
#define M1_UDCB_IIR_A1 (0.939082F)
#define M1_SCALAR_VHZ_FACTOR_GAIN (0.75F)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0586667)
#define M1_SCALAR_RAMP_UP (0.0266667F)
#define M1_SCALAR_RAMP_DOWN (0.0266667F)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN (19.8726F)
#define M1_D_KI_GAIN (1.87564F)
#define M1_Q_KP_GAIN (18.8241F)
#define M1_Q_KI_GAIN (1.78340F)
#define M1_CLOOP_LIMIT (0.519615F)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP (1.67552F)
#define M1_SPEED_RAMP_DOWN (1.67552F)
#define M1_SPEED_LOOP_HIGH_LIMIT (2.0F)
#define M1_SPEED_LOOP_LOW_LIMIT (-2.0F)
#define M1_SPEED_PI_PROP_GAIN (0.00035F)
#define M1_SPEED_PI_INTEG_GAIN (0.00000100000F)
#define M1_POS_P_PROP_GAIN FRAC16(0.12)
#define M1_SPEED_IIR_B0 (0.0304590F)
#define M1_SPEED_IIR_B1 (0.0304590F)
#define M1_SPEED_IIR_A1 (0.939082F)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC (0.0628319F)
#define M1_OL_START_I (0.2F)
#define M1_MERG_SPEED_TRH (209.440F)
#define M1_MERG_COEFF FRAC16(0.00457764)
#define M1_I_SCALE (0.976632F)
#define M1_U_SCALE (0.0161160F)
#define M1_E_SCALE (0.0161160F)
#define M1_WI_SCALE (0.0000928606F)
#define M1_BEMF_DQ_KP_GAIN (19.8726F)
#define M1_BEMF_DQ_KI_GAIN (1.87564F)
#define M1_TO_KP_GAIN (314.159F)
#define M1_TO_KI_GAIN (2.46740F)
#define M1_TO_THETA_GAIN (0.0000318310F)
#define M1_TO_SPEED_IIR_B0 (0.111635F)
#define M1_TO_SPEED_IIR_B1 (0.111635F)
#define M1_TO_SPEED_IIR_A1 (0.776730F)
/* USER INPUT START */
#define AA 1
#define BB 3
#define CC 4
#define DD 2
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
