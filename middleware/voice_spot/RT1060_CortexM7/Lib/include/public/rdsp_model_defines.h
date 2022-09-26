/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_MODEL_DEFINES_H
#define RDSP_MODEL_DEFINES_H

// Public defines for RDSP Model

#if defined(__cplusplus) && !defined(CPP_LINKAGE)
extern "C" {
#endif

// Define error codes
#define RDSP_MODEL_OK						 0			// No errors detected
#define RDSP_MODEL_MALLOC_FAIL				-1			// Memory allocation failed
#define RDSP_MODEL_INVALID_POINTER		 	-2			// An invalid pointer detected
#define RDSP_MODEL_UNSUPPORTED_DATA_TYPE 	-3			// Unsupported data type requested
#define RDSP_MODEL_INVALID_HANDLE        	-4			// An invalid handle detected
#define RDSP_MODEL_NO_FREE_HANDLE       	-5			// There are no free handles available
#define RDSP_MODEL_OPEN_FAILED			 	-6			// Opening of the model failed
#define RDSP_MODEL_NOT_OPEN				 	-7			// The model instance is not open
#define RDSP_MODEL_NO_TRIGGER_FOUND		 	-8			// No valid trigger found for timing estimation
#define RDSP_MODEL_UNABLE_TO_CLOSE_MASTER 	-9			// Master can not be closed before all slaves have been closed
#define RDSP_MODEL_UNABLE_TO_OPEN_AS_SLAVE 	-10			// Slave model is not compatible with master
#define RDSP_MODEL_BLOB_NOT_VECTOR_ALIGNED 	-11			// The model blob is not aligned to the vector boundaries, typically 16 bytes
#define RDSP_MODEL_LICENSE_EXPIRED       	-12			// The SW license has expired, please renew
#define RDSP_MODEL_INVALID_ID       	 	-13			// An invalid ID was used
#define RDSP_MODEL_PARAMETER_NOT_AVAILABLE  -14         // The parameter is not available in the current state
#define RDSP_MODEL_INTEGRITY_CHECK_FAILED   -15         // The model integrity check failed
    
// Define data types
#define RDSP_DATA_TYPE__FLOAT32              0          // Use 32-bit floating-point data as input
#define RDSP_DATA_TYPE__INT32                1          // Use 32-bit integer data as input
    
// Define processing levels
#define RDSP_PROCESSING_LEVEL__FULL                     0           // Full processing
#define RDSP_PROCESSING_LEVEL__PREPROCESSING_ONLY       1           // Preprocessing only
#define RDSP_PROCESSING_LEVEL__SKIP_OUTPUT              2           // Do internal processing, but skip output generation
    

#if defined(__cplusplus) && !defined(CPP_LINKAGE)
}
#endif
#endif // RDSP_MODEL_DEFINES_H
