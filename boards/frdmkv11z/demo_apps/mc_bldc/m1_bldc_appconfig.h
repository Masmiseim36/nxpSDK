/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**********************************************************************/
// File Name: {FM_project_loc}/../../../src/projects/frdm-kv11z/bldc_appconfig.h
//
// Date:  February 16, 2017, 13:22:19
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
#define M1_N_MAX (4400.0)
#define M1_I_DCB_OVERCURRENT FRAC16(0.862068965517)
#define M1_U_DCB_UNDERVOLTAGE FRAC16(0.4)
#define M1_U_DCB_OVERVOLTAGE FRAC16(0.933333333333)
#define M1_I_DCB_LIMIT FRAC16(0.369458128079)
#define M1_U_DCB_TRIP FRAC16(0.933333333333)
#define M1_N_NOM FRAC16(0.909090909091)
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
#define M1_SPEED_LOOP_KP_GAIN ACC32(0.003404681693545589)
#define M1_SPEED_LOOP_KI_GAIN ACC32(0.003404681693545589)

// Speed ramp increments
#define M1_SPEED_LOOP_RAMP_UP FRAC32(0.000454545455)
#define M1_SPEED_LOOP_RAMP_DOWN FRAC32(0.000454545455)

// Torque Controller - Parallel type
#define M1_TORQUE_LOOP_KP_GAIN ACC32(0.009635733333333333)
#define M1_TORQUE_LOOP_KI_GAIN ACC32(0.009635733333333333)

// Sensoroless Control Module
//----------------------------------------------------------------------
// Timer Frequency                       = 585937 [Hz]
//----------------------------------------------------------------------
#define M1_N_MIN FRAC16(0.056818181818)
#define M1_N_START_TRH FRAC16(0.081818181818)
#define M1_STARTUP_CMT_CNT (2)
#define M1_STARTUP_CMT_PER (29297)
#define M1_CMT_T_OFF FRAC16(0.22)
#define M1_FREEWHEEL_T_LONG (1000)
#define M1_FREEWHEEL_T_SHORT (500)
#define M1_SPEED_SCALE_CONST (3995)
#define M1_CMT_PER_MIN (666)
#define M1_START_CMT_ACCELER FRAC16(0.277777777778)
#define M1_INTEG_TRH (163841)

#endif

// End of generated file
/**********************************************************************/
