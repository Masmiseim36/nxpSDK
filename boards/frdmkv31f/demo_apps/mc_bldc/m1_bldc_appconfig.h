/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************/
// File Name: {FM_project_loc}/bldc_appconfig.h
//
// Date:  April 29, 2019, 9:28:26
//
// Automatically generated file for static configuration of the BLDC application
/**********************************************************************/

#ifndef __M1_BLDC_APPCONFIG_H
#define __M1_BLDC_APPCONFIG_H

// Motor Parameters
//----------------------------------------------------------------------
// Pole-pair number                      = 2 [-]
// Back-EMF constant                     = 0.028648 [V.sec/rad]
// Phase current nominal                 = 1.67 [A]
// Phase voltage nominal                 = 12 [V]
//----------------------------------------------------------------------

// Application scales
#define M1_I_MAX (8.12)
#define M1_U_DCB_MAX (15.0)
#define M1_N_MAX (5000.0)
#define M1_I_DCB_OVERCURRENT FRAC16(0.862068965517)
#define M1_U_DCB_UNDERVOLTAGE FRAC16(0.4)
#define M1_U_DCB_OVERVOLTAGE FRAC16(0.933333333333)
#define M1_I_DCB_LIMIT FRAC16(0.369458128079)
#define M1_U_DCB_TRIP FRAC16(0.933333333333)
#define M1_N_NOM FRAC16(0.8)
#define M1_I_PH_NOM FRAC16(0.205665024631)
#define M1_U_PH_NOM FRAC16(0.8)
// Mechanical Alignemnt
#define M1_ALIGN_CURRENT FRAC16(0.184729064039)
#define M1_ALIGN_DURATION (500)
// Application counters
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (3000)
// Udcb and Idcb Filters
#define M1_UDCB_IIR_B0 FRAC32(0.007732519502)
#define M1_UDCB_IIR_B1 FRAC32(0.007732519502)
#define M1_UDCB_IIR_A1 FRAC32(0.484534960997)
#define M1_IDCB_IIR_B0 FRAC32(0.007732519502)
#define M1_IDCB_IIR_B1 FRAC32(0.007732519502)
#define M1_IDCB_IIR_A1 FRAC32(0.484534960997)

// BLDC Control Loop
//----------------------------------------------------------------------
// Loop sample time
//----------------------------------------------------------------------
// Control loop limits
#define M1_CTRL_LOOP_LIM_HIGH FRAC16(0.9)
#define M1_CTRL_LOOP_LIM_LOW FRAC16(0.0)

// Speed Controller - Parallel type
#define M1_SPEED_LOOP_KP_GAIN ACC32(0.0038689564699381697)
#define M1_SPEED_LOOP_KI_GAIN ACC32(0.0038689564699381697)

// Speed ramp increments
#define M1_SPEED_LOOP_RAMP_UP FRAC32(0.001)
#define M1_SPEED_LOOP_RAMP_DOWN FRAC32(0.001)

// Torque Controller - Parallel type
#define M1_TORQUE_LOOP_KP_GAIN ACC32(0.009635733333333333)
#define M1_TORQUE_LOOP_KI_GAIN ACC32(0.009635733333333333)

// Sensoroless Control Module
//----------------------------------------------------------------------
// Timer Frequency                       = 312500 [Hz]
//----------------------------------------------------------------------
#define M1_N_MIN FRAC16(0.05)
#define M1_N_START_TRH FRAC16(0.06)
#define M1_STARTUP_CMT_CNT (2)
#define M1_STARTUP_CMT_PER (15625)
#define M1_CMT_T_OFF FRAC16(0.22)
#define M1_FREEWHEEL_T_LONG (1000)
#define M1_FREEWHEEL_T_SHORT (500)
#define M1_SPEED_SCALE_CONST (1875)
#define M1_CMT_PER_MIN (313)
#define M1_START_CMT_ACCELER FRAC16(0.333333333333)
#define M1_INTEG_TRH (163841)

#endif

// End of generated file
/**********************************************************************/
