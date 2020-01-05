/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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

  
/*! \file fusion.h 
    \brief Lower level sensor fusion interface
   
    This file can be used to "tune" the performance of specific algorithms
    within the sensor fusion library.  It also defines the lower level function
    definitions for specific algorithms.  Normally, the higher level hooks
    in sensor_fusion.h will be used, and those shown here will be left alone.
*/

#ifndef FUSION_H
#define FUSION_H

#include "sensor_fusion.h"

/// @name COMPUTE_1DOF_P_BASIC constants
///@{
#define FLPFSECS_1DOF_P_BASIC		1.5F            ///< pressure low pass filter time constant (s)
///@}

/// @name COMPUTE_3DOF_G_BASIC constants
///@{
#define FLPFSECS_3DOF_G_BASIC		1.0F            ///< tilt orientation low pass filter time constant (s)
///@}

/// @name COMPUTE_3DOF_B_BASIC constants
///@{
#define FLPFSECS_3DOF_B_BASIC		7.0F            ///< 2D eCompass orientation low pass filter time constant (s)
///@}

/// @name COMPUTE_6DOF_GB_BASIC constants
///@{
#define FLPFSECS_6DOF_GB_BASIC		7.0F            /// <3D eCompass orientation low pass filter time constant (s)
///@}

/// @name COMPUTE_6DOF_GY_KALMAN constants
///@{
#define FQVY_6DOF_GY_KALMAN			2E2     ///< gyro sensor noise variance units (deg/s)^2
#define FQVG_6DOF_GY_KALMAN			1.2E-3  ///< accelerometer sensor noise variance units g^2
#define FQWB_6DOF_GY_KALMAN			2E-2F   ///< gyro offset random walk units (deg/s)^2
#define FMIN_6DOF_GY_BPL			-7.0F   ///< minimum permissible power on gyro offsets (deg/s)
#define FMAX_6DOF_GY_BPL			7.0F    ///< maximum permissible power on gyro offsets (deg/s)
///@}

/// @name COMPUTE_9DOF_GBY_KALMAN constants
///@{
/// gyro sensor noise covariance units deg^2
/// increasing this parameter improves convergence to the geomagnetic field
#define FQVY_9DOF_GBY_KALMAN		2E2		///< gyro sensor noise variance units (deg/s)^2
#define FQVG_9DOF_GBY_KALMAN		1.2E-3	        ///< accelerometer sensor noise variance units g^2 defining minimum deviation from 1g sphere
#define FQVB_9DOF_GBY_KALMAN		5E0		///< magnetometer sensor noise variance units uT^2 defining minimum deviation from geomagnetic sphere.
#define FQWB_9DOF_GBY_KALMAN		2E-2F	        ///< gyro offset random walk units (deg/s)^2
#define FMIN_9DOF_GBY_BPL		-7.0F           ///< minimum permissible power on gyro offsets (deg/s)
#define FMAX_9DOF_GBY_BPL		7.0F            ///< maximum permissible power on gyro offsets (deg/s)
///@}

/// @name Fusion Function Prototypes
/// These functions comprise the core of the basic sensor fusion functions excluding
/// magnetic and acceleration calibration.  Parameter descriptions are not included here,
/// as details are provided in sensor_fusion.h.
///@{
void fInitializeFusion(SensorFusionGlobals *sfg);
void fFuseSensors(struct SV_1DOF_P_BASIC *pthisSV_1DOF_P_BASIC, struct SV_3DOF_G_BASIC *pthisSV_3DOF_G_BASIC,
		struct SV_3DOF_B_BASIC *pthisSV_3DOF_B_BASIC, struct SV_3DOF_Y_BASIC *pthisSV_3DOF_Y_BASIC,
		struct SV_6DOF_GB_BASIC *pthisSV_6DOF_GB_BASIC, struct SV_6DOF_GY_KALMAN *pthisSV_6DOF_GY_KALMAN,
		struct SV_9DOF_GBY_KALMAN *pthisSV_9DOF_GBY_KALMAN,
		struct AccelSensor *pthisAccel, struct MagSensor *pthisMag, struct GyroSensor *pthisGyro, 
		struct PressureSensor *pthisPressure, struct MagCalibration *pthisMagCal);
void fInit_1DOF_P_BASIC(struct SV_1DOF_P_BASIC *pthisSV, struct PressureSensor *pthisPressure,  float flpftimesecs);
void fInit_3DOF_G_BASIC(struct SV_3DOF_G_BASIC *pthisSV, struct AccelSensor *pthisAccel, float flpftimesecs);
void fInit_3DOF_B_BASIC(struct SV_3DOF_B_BASIC *pthisSV, struct MagSensor *pthisMag, float flpftimesecs);
void fInit_3DOF_Y_BASIC(struct SV_3DOF_Y_BASIC *pthisSV);
void fInit_6DOF_GB_BASIC(struct SV_6DOF_GB_BASIC *pthisSV, struct AccelSensor *pthisAccel, struct MagSensor *pthisMag, float flpftimesecs);
void fInit_6DOF_GY_KALMAN(struct SV_6DOF_GY_KALMAN *pthisSV, struct AccelSensor *pthisAccel, struct GyroSensor *pthisGyro);
void fInit_9DOF_GBY_KALMAN(struct SV_9DOF_GBY_KALMAN *pthisSV, struct AccelSensor *pthisAccel, struct MagSensor *pthisMag,
		struct GyroSensor *pthisGyro, struct MagCalibration *pthisMagCal);
void fRun_1DOF_P_BASIC(struct SV_1DOF_P_BASIC *pthisSV, struct PressureSensor *pthisPressure);
void fRun_3DOF_G_BASIC(struct SV_3DOF_G_BASIC *pthisSV, struct AccelSensor *pthisAccel);
void fRun_3DOF_B_BASIC(struct SV_3DOF_B_BASIC *pthisSV, struct MagSensor *pthisMag);
void fRun_3DOF_Y_BASIC(struct SV_3DOF_Y_BASIC *pthisSV, struct GyroSensor *pthisGyro);
void fRun_6DOF_GB_BASIC(struct SV_6DOF_GB_BASIC *pthisSV, struct MagSensor *pthisMag, struct AccelSensor *pthisAccel);
void fRun_6DOF_GY_KALMAN(struct SV_6DOF_GY_KALMAN *pthisSV, struct AccelSensor *pthisAccel, struct GyroSensor *pthisGyro);
void fRun_9DOF_GBY_KALMAN(struct SV_9DOF_GBY_KALMAN *pthisSV, struct AccelSensor *pthisAccel, struct MagSensor *pthisMag, struct GyroSensor *pthisGyro, struct MagCalibration *pthisMagCal);
///@}

#endif   // #ifndef FUSION_H
