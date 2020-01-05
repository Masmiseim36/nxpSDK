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

/*! \file motionCheck.c
    \brief check to see if the board is moving.

    This function would normally be called from your fusion_tasks in your main.c.
    See main_freertos_two_tasks_power_cycling.c for example usage.
*/

// Sensor Fusion Headers
#include "sensor_fusion.h"      // top level magCal and sensor fusion interfaces

/// The motionCheck() function is not a sensor fusion function.  It is a function
/// that simply monitors an accelerometer or magnetometer tri-axial sensor output,
/// returning Boolean true if the sensor appears to be stationary, and false otherwise.
/// This function would normally be called from your fusion_tasks in your main().
bool motionCheck(
    float sample[3],            ///< processed triaxial sensor sample (accel or mag)
    float baseline[3],          ///< previous value to compare to
    float tolerance,            ///< how much tolerance you can stand
    uint32_t winLength,         ///< how many samples need to be stable to assert "noMotion"
    uint32_t *count             ///< how many samples so far we've been not moving
)
{
    float change[3];
    bool changed;
    change[CHX] = fabs(baseline[CHX] - sample[CHX]);
    change[CHY] = fabs(baseline[CHY] - sample[CHY]);
    change[CHZ] = fabs(baseline[CHZ] - sample[CHZ]);
    changed = (change[CHX]>tolerance) ||
              (change[CHY]>tolerance) ||
              (change[CHZ]>tolerance);
    if (changed) {
        baseline[CHX] = sample[CHX];
        baseline[CHY] = sample[CHY];
        baseline[CHZ] = sample[CHZ];
        *count = 0;
    } else {
        if ((*count) <= winLength) (*count) += 1;
    }
    return(*count > winLength);
}
