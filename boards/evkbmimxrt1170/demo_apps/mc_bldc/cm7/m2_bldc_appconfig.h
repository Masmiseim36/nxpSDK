/*
    * Copyright 2023 NXP 
    *
    * SPDX-License-Identifier: BSD-3-Clause 
*/


/*
    * FILE NAME: m2_bldc_appconfig.h
    * DATE: Thu Nov 02 2023, 09:00:37
*/

/*
{
    "parameters": {
        "parametersPP": 4,
        "parametersIphNom": 2.8,
        "parametersUphNom": 240,
        "parametersNnom": 3000,
        "parametersImax": 8,
        "parametersUdcbMax": 443,
        "parametersUdcbUnder": 15,
        "parametersUdcbOver": 400,
        "parametersUdcbTrip": 220,
        "parametersNover": 3200,
        "parametersNmin": 200,
        "parametersUdcbIrrF0": 100,
        "parametersIdcbIrrF0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 3,
        "parametersNmax": 3500,
        "parametersKe": 0.068,
        "parametersAlignVoltage": 2.4,
        "parametersAlignCurrent": 0.8,
        "parametersAlignDuration": 0.5
    },
    "controlLoop": {
        "controlLoopSampleTime": 0.001,
        "controlLoopPwmFreq": 20000,
        "controlLoopLimitHigh": 90,
        "controlLoopLimitLow": 0,
        "controlLoopIncUp": 1000,
        "controlLoopIncDown": 1000,
        "controlLoopSLKp": 0.00008,
        "controlLoopSLKi": 0.0266,
        "controlLoopTLKp": 1.375,
        "controlLoopTLKi": 82.5
    },
    "sensorless": {
        "sensorlessTimerFreq": 312500,
        "sensorlessSpeedMin": 100,
        "sensorlessFreewheelTime": 1,
        "sensorlessOLspeedLim": 150,
        "sensorlessCmtCount": 8,
        "sensorlessCmtPeriod": 0.05,
        "sensorlessTimeOff": 22,
        "sensorlessIntegThrCorr": 100
    }
}
*/

/*
{
    "motorName": "motor",
    "motorDescription": "COMPARE_SET_2"
}
*/

#ifndef __M1_BLDC_APPCONFIG_H 
#define __M1_BLDC_APPCONFIG_H 

/* PARAMETERS*/
#define M1_I_PH_NOM FRAC16(0.35)
#define M1_U_PH_NOM FRAC16(0.541761)
#define M1_N_NOM FRAC16(0.857143)
#define M1_I_MAX (8)
#define M1_U_DCB_MAX (443)
#define M1_I_DCB_OVERCURRENT FRAC16(0.875)
#define M1_U_DCB_UNDERVOLTAGE FRAC16(0.0338600)
#define M1_U_DCB_OVERVOLTAGE FRAC16(0.902935)
#define M1_I_DCB_LIMIT FRAC16(0.375)
#define M1_U_DCB_TRIP FRAC16(0.496614)
#define M1_UDCB_IIR_B0 FRAC32(0.00773252)
#define M1_UDCB_IIR_B1 FRAC32(0.00773252)
#define M1_UDCB_IIR_A1 FRAC32(0.484535)
#define M1_IDCB_IIR_B0 FRAC32(0.00773252)
#define M1_IDCB_IIR_B1 FRAC32(0.00773252)
#define M1_IDCB_IIR_A1 FRAC32(0.484535)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_N_MAX (3500)
#define M1_ALIGN_VOLTAGE FRAC16(0.00541761)
#define M1_ALIGN_CURRENT FRAC16(0.1)
#define M1_ALIGN_DURATION (500)
/* CONTROLLOOP*/
#define M1_CTRL_LOOP_LIM_HIGH FRAC16(0.9)
#define M1_CTRL_LOOP_LIM_LOW FRAC16(0.0)
#define M1_SPEED_LOOP_RAMP_UP FRAC32(0.000285714)
#define M1_SPEED_LOOP_RAMP_DOWN FRAC32(0.000285714)
#define M1_SPEED_LOOP_KP_GAIN ACC32(0.0146608)
#define M1_SPEED_LOOP_KI_GAIN ACC32(0.00487470)
#define M1_TORQUE_LOOP_KP_GAIN ACC32(0.0248307)
#define M1_TORQUE_LOOP_KI_GAIN ACC32(0.00148984)
/* SENSORLESS*/
#define M1_N_MIN FRAC16(0.0571429)
#define M1_FREEWHEEL_T_LONG FRAC16(1000.0)
#define M1_FREEWHEEL_T_SHORT FRAC16(500.0)
#define M1_N_START_TRH FRAC16(0.0428571)
#define M1_STARTUP_CMT_CNT (8)
#define M1_CMT_T_OFF FRAC16(0.22)
#define M1_CMT_PER_MIN (335)
#define M1_STARTUP_CMT_PER (23437.5)
#define M1_SPEED_SCALE_CONST (2009)
#define M1_START_CMT_ACCELER FRAC16(0.854751)
#define M1_INTEG_TRH (13168)
/* USER INPUT START */
/* USER INPUT END */

#endif /* __M1_BLDC_APPCONFIG_H */