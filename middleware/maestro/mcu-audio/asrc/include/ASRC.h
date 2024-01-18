/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
@file
Header file for the application layer interface of the ASRC module.
This file includes all definitions, types, structures and function prototypes
required by the calling layer. All other types, structures and functions are
private.
*/
#ifndef ASRC_LIB_H_
#define ASRC_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "PL_platformTypesPrivate.h"

/* ASRC_Lib_Info_st structure */
typedef struct
{
    PL_UINT8 ASRC_LIB_Release_MAJOR;
    PL_UINT8 ASRC_LIB_Release_MEDIUM;
    PL_UINT8 ASRC_LIB_Release_MINOR;

} ASRC_LibInfo_st;

// Error type
typedef enum
{
    ASRC_SUCCESS, /// Successful return from a routine
    // invalid parameters
    ASRC_INVALID_BUFFER_MEMORY_ALIGNMENT, /// Memory alignment error
    ASRC_INVALID_NULL_ADDRESS,            /// Memory alignment error
    // Error in given ASRC_InstanceParams_st
    ASRC_INVALID_INPUT_FRAME_SIZE,  /// InputSamplesPerFrame parameter not supported
    ASRC_INVALID_OUTPUT_FRAME_SIZE, /// OutputSamplesPerFrame parameter not supported
    ASRC_INVALID_FRAME_SIZE,
    ASRC_INVALID_TARGET_FREQUENCY,  /// targetFs not supported
    ASRC_INVALID_PI_CONTROLLER_ON,  /// PIControllerON parameter is not boolean value
    ASRC_INVALID_ASRC_ENABLE,       /// ASRC_Enable parameter is not boolean value
    ASRC_INVALID_ALPHA,             /// alpha parameter not supported
    ASRC_INVALID_BETA,              /// beta parameter not supported
    ASRC_INVALID_LAMBDA,            /// lambda parameter not supported
    ASRC_INVALID_OPERATING_MODE,    /// OperatingMode parameter not supported
    ASRC_INVALID_SYSTEM_CLOCK,      /// systemClock parameter not supported
    // circular buffer error cases
    ASRC_CIRCULAR_BUFFER_OVERFLOW, /// read and write pointer equals
    ASRC_CIRCULAR_BUFFER_FULL,     /// not space to write  in circular buffer
    ASRC_CIRCULAR_BUFFER_EMPTY,    /// not enough data to read  in circular buffer
    ASRC_CIRCULAR_BUFFER_ERROR,
    /// general errors
    ASRC_ERROR_UNDEFINED, /// undefined error
    ASRC_SYSTEM_ERROR,    /// Unknow error
    ASRC_NB_ERROR         /// number of error

} ASRC_ReturnStatus_en;

/* ASRC Operating Mode
 */
typedef enum
{
    ASRC_DISABLE,     ///  module deactivated
    ASRC_MASTER_MODE, ///  evaluate and fix gamma value within ASRC
    ASRC_SLAVE_MODE,  ///  user fix gamma value
    ASRC_NUMBER_OF_OPERATING_MODE

} ASRC_OperatingMode_en;

typedef enum
{
    ASRC_FS_16000 = 0, // 16kHz sampling rate
    ASRC_FS_INVALID

} ASRC_Fs_en;

typedef struct
{
    ASRC_OperatingMode_en OperatingMode; /// see enum
    PL_UINT16 InputSamplesPerFrame;      /// [| 41 - 120 |]     Number of input samples per frame
    PL_UINT16 OutputSamplesPerFrame;     /// [| 41 - 120 |]     Number of output samples per frame
    ASRC_Fs_en targetFs;                 /// { 16000 }          Estimate frequency of input and output
    PL_UINT32 systemClock;               /// [| 50MHz - 2GHz |] System Platform clock rate

} ASRC_InstanceParams_st;

/* Control Parameter structure */
typedef struct
{
    PL_BOOL PIControllerON; /// { true, false } Enable analysis part of ASRC
    PL_BOOL ASRC_Enable;    /// { true, false } Enable ASRC PIController and synthesis : if false ASRC is transparent

    PL_FLOAT alpha_copy;    /// [ -0.01 - 0.01]  Copy of the Proportional gain of the PI Controller
                            /// Default :  -0.00015, if increased, faster convergence but larger oscillation

    PL_FLOAT beta_copy;     /// [ -0.01 - 0.01] Copy of the Integral gain of the PI Controller
                        /// Default : -0.00000005, if increased, the history get more importance in the calculation but
                        /// can cause overshoot

    PL_FLOAT lambda_copy; /// [ 0 - 1]  Copy of the smoothing of parameter for AOD_corrected computation
                          /// Default :  0.08, if increased, faster convergence but larger oscillation
} ASRC_ControlParams_st;

/*
 * ASRC Instance Handle
 * This handle is used by most of the ASRC APIs
 * @see ASRC_GetInstanceHandle
 * @ingroup ASRC
 */

typedef void *ASRC_Handle_t; ///< ASRC handle

/**
 * @brief
 *
 * @param phInstance
 * @param pMemoryTable
 * @param pInstanceParams
 * @return ASRC_ReturnStatus_en
 */
ASRC_ReturnStatus_en ASRC_GetInstanceHandle(ASRC_Handle_t *phInstance,
                                            PL_MemoryTable_st *pMemoryTable,
                                            ASRC_InstanceParams_st *pInstanceParams);
/**
 * @brief
 *
 * @param phInstance
 * @param pControlParams
 * @return ASRC_ReturnStatus_en
 */
ASRC_ReturnStatus_en ASRC_GetControlParameters(ASRC_Handle_t phInstance, ASRC_ControlParams_st *pControlParams);

/**
 * @brief
 *
 * @param phInstance
 * @param pNewParams
 * @return ASRC_ReturnStatus_en
 */
ASRC_ReturnStatus_en ASRC_SetControlParameters(ASRC_Handle_t phInstance, ASRC_ControlParams_st *const pNewParams);

/**
 * @brief
 *
 * @param hInstance
 * @param pMemoryTable
 * @param pInstanceParams
 * @return ASRC_ReturnStatus_en
 */
ASRC_ReturnStatus_en ASRC_GetMemoryTable(ASRC_Handle_t hInstance,
                                         PL_MemoryTable_st *pMemoryTable,
                                         ASRC_InstanceParams_st *pInstanceParams);

/**
 * @brief
 *
 * @param pLib_Info
 * @return ASRC_ReturnStatus_en
 */
ASRC_ReturnStatus_en ASRC_GetLibInfo(ASRC_LibInfo_st *pLib_Info);

/**
 * @brief
 *
 * @param asrcHandler
 * @param gamma
 * @return ASRC_ReturnStatus_en
 */

ASRC_ReturnStatus_en ASRC_GetRatio(ASRC_Handle_t asrcHandler, PL_FLOAT *gamma);

/**
 * @brief push input frame
 *
 * @param asrcHandler
 * @param inputFrame
 * @return ASRC_ReturnStatus_en
 */
ASRC_ReturnStatus_en ASRC_Push(ASRC_Handle_t asrcHandler, const PL_FLOAT *inputFrame);

/**
 * @brief
 *
 * @param asrcHandler
 * @param outputFrame
 * @param framesize
 * @param timestamp
 * @return ASRC_ReturnStatus_en
 */
ASRC_ReturnStatus_en ASRC_Pull(ASRC_Handle_t asrcHandler,
                               const PL_FLOAT *outputFrame,
                               PL_UINT16 framesize,
                               PL_UINT32 timestamp); // according to systemClock

/**
 * @brief Interpolate output sample if delay line ready
 *
 * @param asrcHandler
 * @param timestamp
 * @param userGammaRatio
 * @return ASRC_ReturnStatus_en
 */
ASRC_ReturnStatus_en ASRC_Process(ASRC_Handle_t asrcHandler,
                                  PL_UINT32 timestamp, // according to systemClock
                                  PL_FLOAT userGammaRatio);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // ASRC_LIB_H_
