/*
 * The Clear BSD License
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

/*! \file precisionAccelerometer.h
    \brief Implements accelerometer calibration routines
*/

#ifndef PRECISIONACCELEROMETER_H
#define PRECISIONACCELEROMETER_H

/// calibration constants
#define ACCEL_CAL_AVERAGING_SECS	2		///< calibration measurement averaging period (s)
#define MAX_ACCEL_CAL_ORIENTATIONS	12		///< number of stored precision accelerometer measurements

/// accelerometer measurement buffer
typedef struct AccelBuffer
{
	float fGsStored[MAX_ACCEL_CAL_ORIENTATIONS][3];	///< uncalibrated accelerometer measurements (g)
	float fSumGs[3];				///< averaging sum for current storage location
	int16_t iStoreCounter;				///< number of remaining iterations at FUSION_HZ to average measurement
	int16_t iStoreLocation;				///< -1 for none, 0 to 11 for the 12 storage locations
	int16_t iStoreFlags;				///< denotes which measurements are present
} AccelBuffer;

/// precision accelerometer calibration structure
typedef struct AccelCalibration
{
	// start of elements stored in flash memory
	float fV[3];					///< offset vector (g)
	float finvW[3][3];				///< inverse gain matrix
	float fR0[3][3];				///< forward rotation matrix for measurement 0
	// end of elements stored in flash memory
	float fmatA[10][10];			        ///< scratch 10x10 matrix used by calibration algorithms
	float fmatB[10][10];			        ///< scratch 10x10 matrix used by calibration algorithms
	float fvecA[10];				///< scratch 10x1 vector used by calibration algorithms
	float fvecB[4];					///< scratch 4x1 vector used by calibration algorithms
	float fA[3][3];					///< ellipsoid matrix A
	float finvA[3][3];				///< inverse of the ellipsoid matrix A
} AccelCalibration;

struct AccelSensor;  // actual typedef is located in sensor_fusion_types.h

// function prototypes for functions in precisionAcclerometer.c
/// Initialize the accelerometer calibration functions
void fInitializeAccelCalibration(
    struct AccelCalibration *pthisAccelCal,             ///< Accelerometer calibration parameter structure
    struct AccelBuffer *pthisAccelBuffer,               ///< Buffer of measurements used as input to the accel calibration functions
    volatile int8_t *AccelCalPacketOn                   ///< Used to coordinate calibration sample storage and communications
);
/// Update the buffer used to store samples used for accelerometer calibration.
void fUpdateAccelBuffer(
    struct AccelCalibration *pthisAccelCal,             ///< Accelerometer calibration parameter structure
    struct AccelBuffer *pthisAccelBuffer,               ///< Buffer of measurements used as input to the accel calibration functions
    struct AccelSensor* pthisAccel,                     ///< Pointer to the accelerometer input/state structure
    volatile int8_t *AccelCalPacketOn                   ///< Used to coordinate calibration sample storage and communications
);
/// function maps the accelerometer data fGs (g) onto precision calibrated and de-rotated data fGc (g), iGc (counts)
void fInvertAccelCal(
    struct AccelSensor *pthisAccel,                     ///< Pointer to the accelerometer input/state structure
    struct AccelCalibration *pthisAccelCal              ///< Accelerometer calibration parameter structure
);
/// function runs the precision accelerometer calibration
void fRunAccelCalibration(
    struct AccelCalibration *pthisAccelCal,             ///< Accelerometer calibration parameter structure
    struct AccelBuffer *pthisAccelBuffer,               ///< Buffer of measurements used as input to the accel calibration functions
    struct AccelSensor* pthisAccel                      ///< Pointer to the accelerometer input/state structure
);
/// calculate the 4 element calibration from the available measurements
void fComputeAccelCalibration4(
    struct AccelBuffer *pthisAccelBuffer,               ///< Buffer of measurements used as input to the accel calibration functions
    struct AccelCalibration *pthisAccelCal,             ///< Accelerometer calibration parameter structure
    struct AccelSensor* pthisAccel                      ///< Pointer to the accelerometer input/state structure
);
/// calculate the 7 element calibration from the available measurements
void fComputeAccelCalibration7(
    struct AccelBuffer *pthisAccelBuffer,               ///< Buffer of measurements used as input to the accel calibration functions
    struct AccelCalibration *pthisAccelCal,             ///< Accelerometer calibration parameter structure
    struct AccelSensor* pthisAccel                      ///< Pointer to the accelerometer input/state structure
);
/// calculate the 10 element calibration from the available measurements
void fComputeAccelCalibration10(
    struct AccelBuffer *pthisAccelBuffer,               ///< Buffer of measurements used as input to the accel calibration functions
    struct AccelCalibration *pthisAccelCal,             ///< Accelerometer calibration parameter structure
    struct AccelSensor* pthisAccel                      ///< Pointer to the accelerometer input/state structure
);

#endif // PRECISIONACCELEROMETER_H 
