/*
    * Copyright 2024 NXP 
    *
    * SPDX-License-Identifier: BSD-3-Clause 
*/


/*
    * FILE NAME: m1_bldc_appconfig.h
    * DATE: Wed Mar 20 2024, 13:54:21
*/

/*
{
    "parameters": {
        "parametersPP": 2,
        "parametersNmax": 5000,
        "parametersImax": 8.25,
        "parametersNnom": 4000,
        "parametersUdcbMax": 15,
        "parametersUdcbUnder": 6,
        "parametersUdcbOver": 14,
        "parametersNover": 4200,
        "parametersNmin": 250,
        "parametersUdcbIrrF0": 100,
        "parametersIdcbIrrF0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 3,
        "parametersKe": 0.028648,
        "parametersAlignCurrent": 1.5,
        "parametersAlignDuration": 0.5
    },
    "controlLoop": {
        "controlLoopSampleTime": 0.001,
        "controlLoopPwmFreq": 20000,
        "controlLoopLimitHigh": 90,
        "controlLoopLimitLow": 0,
        "controlLoopIncUp": 5000,
        "controlLoopIncDown": 5000,
        "controlLoopSLKp": 0.00003,
        "controlLoopSLKi": 0.03,
        "controlLoopTLKp": 0.0178,
        "controlLoopTLKi": 17.8
    },
    "sensorless": {
        "sensorlessTimerFreq": 312500,
        "sensorlessFreewheelTime": 1,
        "sensorlessOLspeedLim": 300,
        "sensorlessCmtCount": 2,
        "sensorlessCmtPeriod": 0.05,
        "sensorlessTimeOff": 22,
        "sensorlessIntegThrCorr": 100
    }
}
*/

/*
{
    "motorName": "linix",
    "motorDescription": "Configuration for LINIX motor (default)."
}
*/

#ifndef __M1_BLDC_APPCONFIG_H 
#define __M1_BLDC_APPCONFIG_H 

/* PARAMETERS*/
#define M1_N_MAX (5000.0F)
#define M1_I_MAX (8.25F)
#define M1_N_NOM FRAC16(0.8)
#define M1_U_DCB_MAX (15.0F)
#define M1_U_DCB_UNDERVOLTAGE FRAC16(0.4)
#define M1_U_DCB_OVERVOLTAGE FRAC16(0.933333)
#define M1_I_DCB_LIMIT FRAC16(0.363636)
#define M1_N_MIN FRAC16(0.05)
#define M1_UDCB_IIR_B0 FRAC32(0.00773252)
#define M1_UDCB_IIR_B1 FRAC32(0.00773252)
#define M1_UDCB_IIR_A1 FRAC32(0.484535)
#define M1_IDCB_IIR_B0 FRAC32(0.00773252)
#define M1_IDCB_IIR_B1 FRAC32(0.00773252)
#define M1_IDCB_IIR_A1 FRAC32(0.484535)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (3000)
#define M1_ALIGN_CURRENT FRAC16(0.181818)
#define M1_ALIGN_DURATION (500)
/* CONTROLLOOP*/
#define M1_CTRL_LOOP_LIM_HIGH FRAC16(0.9)
#define M1_CTRL_LOOP_LIM_LOW FRAC16(0.0)
#define M1_SPEED_LOOP_RAMP_UP FRAC32(0.001)
#define M1_SPEED_LOOP_RAMP_DOWN FRAC32(0.001)
#define M1_SPEED_LOOP_KP_GAIN ACC32(0.00380799)
#define M1_SPEED_LOOP_KI_GAIN ACC32(0.00380799)
#define M1_TORQUE_LOOP_KP_GAIN ACC32(0.00979)
#define M1_TORQUE_LOOP_KI_GAIN ACC32(0.00979)
/* SENSORLESS*/
#define M1_FREEWHEEL_T_LONG (1000)
#define M1_FREEWHEEL_T_SHORT (500)
#define M1_N_START_TRH FRAC16(0.06)
#define M1_STARTUP_CMT_CNT (2)
#define M1_CMT_T_OFF FRAC16(0.22)
#define M1_CMT_PER_MIN (313)
#define M1_STARTUP_CMT_PER (15625)
#define M1_SPEED_SCALE_CONST (1875)
#define M1_START_CMT_ACCELER FRAC16(0.333333)
#define M1_INTEG_TRH (163841)
/* USER INPUT START */
/* USER INPUT END */

#endif /* __M1_BLDC_APPCONFIG_H */
