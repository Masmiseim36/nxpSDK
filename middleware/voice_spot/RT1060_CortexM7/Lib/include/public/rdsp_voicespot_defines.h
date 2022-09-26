/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_VOICESPOT_DEFINES_H
#define RDSP_VOICESPOT_DEFINES_H

// Public defines for RDSP VoiceSpot
#include "public/rdsp_model_defines.h"

// Define error codes
#define RDSP_VOICESPOT_OK                               RDSP_MODEL_OK							// No errors detected
#define RDSP_VOICESPOT_MALLOC_FAIL                      RDSP_MODEL_MALLOC_FAIL					// Memory allocation failed
#define RDSP_VOICESPOT_INVALID_POINTER                  RDSP_MODEL_INVALID_POINTER				// An invalid pointer detected
#define RDSP_VOICESPOT_UNSUPPORTED_DATA_TYPE            RDSP_MODEL_UNSUPPORTED_DATA_TYPE		// Unsupported data type requested
#define RDSP_VOICESPOT_INVALID_HANDLE                   RDSP_MODEL_INVALID_HANDLE				// An invalid handle detected
#define RDSP_VOICESPOT_NO_FREE_HANDLE                   RDSP_MODEL_NO_FREE_HANDLE				// There are no free handles available
#define RDSP_VOICESPOT_OPEN_FAILED                      RDSP_MODEL_OPEN_FAILED					// Opening of the model failed
#define RDSP_VOICESPOT_NOT_OPEN                         RDSP_MODEL_NOT_OPEN						// The model instance is not open
#define RDSP_VOICESPOT_NO_TRIGGER_FOUND                 RDSP_MODEL_NO_TRIGGER_FOUND				// No valid trigger found for timing estimation
#define RDSP_VOICESPOT_UNABLE_TO_CLOSE_MASTER           RDSP_MODEL_UNABLE_TO_CLOSE_MASTER		// Master can not be closed before all slaves have been closed
#define RDSP_VOICESPOT_UNABLE_TO_OPEN_AS_SLAVE          RDSP_MODEL_UNABLE_TO_OPEN_AS_SLAVE		// Slave model is not compatible with master
#define RDSP_VOICESPOT_BLOB_NOT_VECTOR_ALIGNED          RDSP_MODEL_BLOB_NOT_VECTOR_ALIGNED		// The model blob is not aligned to the vector boundaries, typically 16 bytes
#define RDSP_VOICESPOT_LICENSE_EXPIRED                  RDSP_MODEL_LICENSE_EXPIRED              // The SW license has expired, please renew
#define RDSP_VOICESPOT_INVALID_ID                       RDSP_MODEL_INVALID_ID       	 		// An invalid ID was used
#define RDSP_VOICESPOT_PARAMETER_NOT_AVAILABLE          RDSP_MODEL_PARAMETER_NOT_AVAILABLE  	// The parameter is not available in the current state
#define RDSP_VOICESPOT_INTEGRITY_CHECK_FAILED           RDSP_MODEL_INTEGRITY_CHECK_FAILED       // The model integrity check failed

#define RDSP_VOICESPOT_VALID_THRESHOLD_NOT_AVAILABLE    -0x100                                  // No valid threshold value available for event detection
#define RDSP_VOICESPOT_BUFFER_UNDERFLOW                 -0x101									// The input buffer has experienced underflow
#define RDSP_VOICESPOT_UNABLE_TO_REGISTER_CALLBACK      -0x102									// The callback function could not be registered

#endif // RDSP_VOICESPOT_DEFINES_H
