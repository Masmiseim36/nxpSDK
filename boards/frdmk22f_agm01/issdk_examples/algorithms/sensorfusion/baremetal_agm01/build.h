/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! \file build.h
    \brief Build configuration file

    This file contains only those parameters that directly relate to fusion
    implementation choices.  Board dependencies are in hal.h.  Consult the
    Sensor Fusion User Guide for guidance and details.
*/

#ifndef BUILD_H
#define BUILD_H

#define THISBUILD  		720     ///< define build number sent in debug packet for display purposes only

/// @name CoordinateSystemBitFields
/// These defines determine the frame of reference (x, y, z axes and Euler angles) standard
/// to be used for a particular build.  Change THISCOORDSYSTEM to whichever of NED, ANDROID
/// or WIN8 you prefer.
///@{
#define NED 0                           ///< identifier for NED (Aerospace) axes and angles
#define ANDROID 1                       ///< identifier for Android axes and angles
#define WIN8 2			        ///< identifier for Windows 8 axes and angles
#define THISCOORDSYSTEM NED		///< the coordinate system to be used
///@}

///@{
/// @name SensorBitFields
/// These bit-field values are used to declare which sensor types are used in the application.
/// Change bit-field values to 0x0000 for any features NOT USED
#define F_USING_ACCEL           0x0001  ///< nominally 0x0001 if an accelerometer is to be used, 0x0000 otherwise
#define F_USING_MAG             0x0002  ///< nominally 0x0002 if an magnetometer  is to be used, 0x0000 otherwise
#define F_USING_GYRO            0x0004  ///< nominally 0x0004 if a gyro           is to be used, 0x0000 otherwise
#define F_USING_PRESSURE        0x0000  ///< nominally 0x0008 if altimeter        is to be used, 0x0000 otherwise
#define F_USING_TEMPERATURE     0x0000  ///< nominally 0x0010 if temp sensor      is to be used, 0x0000 otherwise
#define F_ALL_SENSORS           0x001F  ///< refers to all applicable sensor types for the given physical unit
///@}
/// @name FusionSelectionBitFields
/// These bit-field values are used to declare which sensor fusion algorithms are used
/// in the application.  You can use more than one, altough they all run from the same data.
/// Change individual bit-field values to 0x0000 for any features NOT USED.
///@{
#define F_1DOF_P_BASIC	        0x0000	///< 1DOF pressure (altitude) and temperature algorithm selector  - 0x0100 to include, 0x0000 otherwise
#define F_3DOF_G_BASIC	        0x0200	///< 3DOF accel tilt (accel) algorithm selector                   - 0x0200 to include, 0x0000 otherwise
#define F_3DOF_B_BASIC	        0x0400	///< 3DOF mag eCompass (vehicle/mag) algorithm selector           - 0x0400 to include, 0x0000 otherwise
#define F_3DOF_Y_BASIC	        0x0800	///< 3DOF gyro integration algorithm selector                     - 0x0800 to include, 0x0000 otherwise
#define F_6DOF_GB_BASIC	        0x1000	///< 6DOF accel and mag eCompass algorithm selector               - 0x1000 to include, 0x0000 otherwise
#define F_6DOF_GY_KALMAN        0x2000	///< 6DOF accel and gyro (Kalman) algorithm selector              - 0x2000 to include, 0x0000 otherwise
#define F_9DOF_GBY_KALMAN	0x4000	///< 9DOF accel, mag and gyro algorithm selector                  - 0x4000 to include, 0x0000 otherwise
///@}

/// @name SensorParameters
/// FIFO sizes effect the size of the sensor data structures.  ODR refers to "Output Data Rate"
///@{
#define ACCEL_FIFO_SIZE 	32	///< FXOS8700 (accel), MMA8652, FXLS8952 all have 32 element FIFO
#define MAG_FIFO_SIZE 		16	///< FXOS8700 (mag), MAG3110 have no FIFO so equivalent to 1 element FIFO
#define GYRO_FIFO_SIZE 		32	///< FXAX21000, FXAS21002 have 32 element FIFO
#define ACCEL_ODR_HZ 		200    	///< (int) requested accelerometer ODR Hz (over-rides MAG_ODR_HZ for FXOS8700)
#define MAG_ODR_HZ 		200    	///< (int) requested magnetometer ODR Hz (over-ridden by ACCEL_ODR_HZ for FXOS8700)
#define GYRO_ODR_HZ 		400    	///< (int) requested gyroscope ODR Hz
#define FUSION_HZ 		40	///< (int) actual rate of fusion algorithm execution and sensor FIFO reads
#define FAST_LOOP_HZ		40	///< Over Sample Ratio * FUSION_HZ when using no FIFO
#define OVERSAMPLE_RATE         FAST_LOOP_HZ/FUSION_HZ
///@}
//#define F_USE_WIRELESS_UART     0x0000	///< 0x0001 to include, 0x0000 otherwise
//#define F_USE_WIRED_UART        0x0000	///< 0x0002 to include, 0x0000 otherwise

#define INCLUDE_DEBUG_FUNCTIONS   // Comment this line to disable the ApplyPerturbation function

#endif // BUILD_H
