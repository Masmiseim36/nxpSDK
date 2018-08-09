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

#ifndef SENSOR_FUSION_STATUS_H_
#define SENSOR_FUSION_STATUS_H_


/*! \file status.h
    \brief Application-specific status subsystem

    Applications may change how they choose to display status information.
    The default implementation here uses LEDs on NXP Freedom boards.
    You may swap out implementations as long as the "Required" methods and states
    are retained.
*/
/// StatusSubsystem() provides an object-like interface for communicating status to the user
typedef struct StatusSubsystem {
	// Required internal states
	fusion_status_t		previous;       ///< Previous status state - fusion_status_t is defined in sensor_fusion.h
	fusion_status_t		status;         ///< Current status
	fusion_status_t		next;           ///< Pending status change
	// Required methods
	ssSetStatus_t           *set;	        ///< change status immediately - no delay
	ssSetStatus_t           *queue;         ///< queue status change for next regular interval
	ssUpdateStatus_t        *update;        ///< make pending status active/visible
	ssUpdateStatus_t        *test ;         ///< unit test which simply increments to next state
	// application-specific internal variables
	uint8_t toggle;                         ///< This implementation can change LED color and have either solid/toggle
} StatusSubsystem ;

/// initializeStatusSubsystem() should be called once at startup to initialize the
/// data structure and to put hardware into the proper state for communicating status.
void initializeStatusSubsystem (
  StatusSubsystem *pStatus                      ///< pointer to the status subsystem
);

#endif /* SENSOR_FUSION_STATUS_H_ */
