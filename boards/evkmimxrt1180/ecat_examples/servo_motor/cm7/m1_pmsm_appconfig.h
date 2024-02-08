/*
	* Copyright 2022 NXP
	*
	* SPDX-License-Identifier: BSD-3-Clause
*/

/*
    * FILE NAME: ../boards/evkmimxrt1180/mc_pmsm/pmsm_enc/m1_pmsm_appconfig.h
    * DATE: Fri Dec 15 2023, 08:46:42
*/

/*
{
    "parameters": {
        "parametersPP": 4,
        "parametersRs": 0.345,
        "parametersLd": 0.0003665,
        "parametersLq": 0.0003665,
        "parametersKe": 0.0634,
        "parametersJ": 0.0000016,
        "parametersIphNom": 5,
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
        "parametersAlignVoltage": 0.8,
        "parametersAlignDuration": 0.4
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.0000625,
        "currentLoopF0": 350,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.002,
        "speedLoopF0": 45,
        "speedLoopKsi": 1,
        "speedLoopIncUp": 10000,
        "speedLoopIncDown": 10000,
        "speedLoopCutOffFreq": 100,
        "speedLoopUpperLimit": 6,
        "speedLoopLowerLimit": -6,
        "speedLoopSLKp": 0.01,
        "speedLoopSLKi": 0.00006,
        "speedLoopManualConstantTunning": true
    },
    "sensors": {
        "sensorEncPulseNumber": 2000,
        "sensorEncDir": 0,
        "sensorEncNmin": 0,
        "sensorObsrvParSampleTime": 0.0000625,
        "sensorObsrvParF0": 150,
        "sensorObsrvParKsi": 1,
        "positionLoopPLKp": 0.035
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 300,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 100,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessTrackObsrvIIRSpeedCutOff": 400,
        "sensorlessStartupRamp": 1000,
        "sensorlessStartupCurrent": 1,
        "sensorlessMergingSpeed": 400,
        "sensorlessMergingCoeff": 100
    }
}
*/

/*
{
    "motorName": "Teknic 2311P-LN-08D",
    "motorDescription": "Configuration for the Teknic motor."
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* PARAMETERS*/
#define M1_MOTOR_PP (4)
#define M1_I_PH_NOM (5.0F)
#define M1_N_NOM (1675.52F)
#define M1_I_MAX (8.25F)
#define M1_U_DCB_MAX (60.8F)
#define M1_U_DCB_TRIP (29.0F)
#define M1_U_DCB_UNDERVOLTAGE (16.0F)
#define M1_U_DCB_OVERVOLTAGE (32.0F)
#define M1_N_OVERSPEED (1842.65F)
#define M1_N_MIN (125.664F)
#define M1_E_BLOCK_TRH (0.8F)
#define M1_E_BLOCK_PER (2000)
#define M1_N_MAX (1843.07F)
#define M1_CALIB_DURATION (100)
#define M1_FAULT_DURATION (3000)
#define M1_FREEWHEEL_DURATION (750)
#define M1_SCALAR_UQ_MIN (1.0F)
#define M1_ALIGN_VOLTAGE (0.8F)
#define M1_ALIGN_DURATION (6400)
#define M1_U_MAX (35.1029F)
#define M1_FREQ_MAX (293.333F)
#define M1_N_ANGULAR_MAX (2.38732F)
#define M1_UDCB_IIR_B0 (0.0192568F)
#define M1_UDCB_IIR_B1 (0.0192568F)
#define M1_UDCB_IIR_A1 (0.961486F)
#define M1_SCALAR_VHZ_FACTOR_GAIN (0.0562500F)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0366667)
#define M1_SCALAR_RAMP_UP (0.0416667F)
#define M1_SCALAR_RAMP_DOWN (0.0416667F)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN (1.26695F)
#define M1_D_KI_GAIN (0.110777F)
#define M1_Q_KP_GAIN (1.26695F)
#define M1_Q_KI_GAIN (0.110777F)
#define M1_CLOOP_LIMIT (0.519615F)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP (8.37758F)
#define M1_SPEED_RAMP_DOWN (8.37758F)
#define M1_SPEED_LOOP_HIGH_LIMIT (6.0F)
#define M1_SPEED_LOOP_LOW_LIMIT (-6.0F)
#define M1_SPEED_PI_PROP_GAIN (0.01F)
#define M1_SPEED_PI_INTEG_GAIN (0.00006F)
#define M1_SPEED_IIR_B0 (0.0192568F)
#define M1_SPEED_IIR_B1 (0.0192568F)
#define M1_SPEED_IIR_A1 (0.961486F)
/* SENSORS*/
#define M1_POSPE_ENC_PULSES (2000)
#define M1_POSPE_ENC_DIRECTION (0)
#define M1_POSPE_ENC_N_MIN (0.0F)
#define M1_POSPE_MECH_POS_GAIN ACC32(8.192)
#define M1_POS_P_PROP_GAIN FRAC16(0.035)
#define M1_POSPE_TO_KP_GAIN (1884.96F)
#define M1_POSPE_TO_KI_GAIN (55.5165F)
#define M1_POSPE_TO_THETA_GAIN (0.0000198944F)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC (0.0261799F)
#define M1_OL_START_I (1.0F)
#define M1_MERG_SPEED_TRH (167.552F)
#define M1_MERG_COEFF FRAC16(0.00166667)
#define M1_I_SCALE (0.944435F)
#define M1_U_SCALE (0.161057F)
#define M1_E_SCALE (0.161057F)
#define M1_WI_SCALE (0.0000590272F)
#define M1_BEMF_DQ_KP_GAIN (1.03667F)
#define M1_BEMF_DQ_KI_GAIN (0.0813872F)
#define M1_TO_KP_GAIN (1256.64F)
#define M1_TO_KI_GAIN (24.6740F)
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
