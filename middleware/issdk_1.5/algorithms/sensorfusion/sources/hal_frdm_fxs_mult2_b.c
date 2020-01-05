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

/*! \file hal_frdm_fxs_mult2_b.c
    \brief Hardware Abstraction layer for the FRDM-FXS-MULT2-B sensor shield.
*/

#include "sensor_fusion.h"  // top level magCal and sensor fusion interfaces

// This HAL (Hardware Abstraction Layer) is applicable to:
// FRDM_K64F +
// FRDM_FXS_MULT2_B utilizing the FX0S8700 and FXAS21002 on the shield
// It also works for the FRDM-STBC_AGM01 board.
// It also works for the FRDM-STBC_AGM02 board (the driver_MAG3110.c inverts
// the MAG3110 Z-axis reading to enforce +Z = up, so compatibility is maintained)

void ApplyAccelHAL(struct AccelSensor *Accel)
{
	int8 i;				// loop counter

	// apply HAL to all measurements read from FIFO buffer
	for (i = 0; i < Accel->iFIFOCount; i++)
	{
		// apply HAL mapping to coordinate system used
#if THISCOORDSYSTEM == NED
		int16 itmp16 = Accel->iGsFIFO[i][CHX];
		Accel->iGsFIFO[i][CHX] = Accel->iGsFIFO[i][CHY];
		Accel->iGsFIFO[i][CHY] = itmp16;
#endif // NED
#if THISCOORDSYSTEM == ANDROID
		Accel->iGsFIFO[i][CHX] = -Accel->iGsFIFO[i][CHX];
		Accel->iGsFIFO[i][CHY] = -Accel->iGsFIFO[i][CHY];
#endif // Android
#if (THISCOORDSYSTEM == WIN8)
		Accel->iGsFIFO[i][CHZ] = -Accel->iGsFIFO[i][CHZ];
#endif // Win8

	} // end of loop over FIFO count

	return;
}

// function applies the hardware abstraction layer to the magnetometer readings
void ApplyMagHAL(struct MagSensor *Mag)
{
	int8 i;				// loop counter

	// apply HAL to all measurements read from FIFO buffer
	for (i = 0; i < Mag->iFIFOCount; i++)
	{
		// apply HAL mapping to coordinate system used
#if THISCOORDSYSTEM == NED
		int16 itmp16 = Mag->iBsFIFO[i][CHX];
		Mag->iBsFIFO[i][CHX] = -Mag->iBsFIFO[i][CHY];
		Mag->iBsFIFO[i][CHY] = -itmp16;
		Mag->iBsFIFO[i][CHZ] = -Mag->iBsFIFO[i][CHZ];
#endif // NED
#if THISCOORDSYSTEM == ANDROID
		Mag->iBsFIFO[i][CHX] = -Mag->iBsFIFO[i][CHX];
		Mag->iBsFIFO[i][CHY] = -Mag->iBsFIFO[i][CHY];
#endif // Android
#if THISCOORDSYSTEM == WIN8
		Mag->iBsFIFO[i][CHX] = -Mag->iBsFIFO[i][CHX];
		Mag->iBsFIFO[i][CHY] = -Mag->iBsFIFO[i][CHY];
#endif
	} // end of loop over FIFO count

	return;
}

// function applies the hardware abstraction layer to the gyro readings
void ApplyGyroHAL(struct GyroSensor *Gyro)
{
	int8 i;				// loop counter

	// apply HAL to all measurements read from FIFO buffer
	for (i = 0; i < Gyro->iFIFOCount; i++)
	{
		// apply HAL mapping to coordinate system used
#if THISCOORDSYSTEM == NED
		int16 itmp16 = Gyro->iYsFIFO[i][CHX];
		Gyro->iYsFIFO[i][CHX] = -Gyro->iYsFIFO[i][CHY];
		Gyro->iYsFIFO[i][CHY] = -itmp16;
		Gyro->iYsFIFO[i][CHZ] = -Gyro->iYsFIFO[i][CHZ];
#endif // NED
#if THISCOORDSYSTEM == ANDROID
		Gyro->iYsFIFO[i][CHX] = -Gyro->iYsFIFO[i][CHX];
		Gyro->iYsFIFO[i][CHY] = -Gyro->iYsFIFO[i][CHY];
#endif // Android
#if THISCOORDSYSTEM == WIN8
		Gyro->iYsFIFO[i][CHX] = -Gyro->iYsFIFO[i][CHX];
		Gyro->iYsFIFO[i][CHY] = -Gyro->iYsFIFO[i][CHY];
#endif // Win8

	} // end of loop over FIFO count

	return;
}

