/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_MODEL_H
#define RDSP_MODEL_H

// External API header for RDSP Model
#if defined(__cplusplus) && !defined(CPP_LINKAGE)
extern "C" {
#endif
#ifdef K32
#include "rdsp_k32_types.h"
#else
#include <stdint.h>
#endif

#ifdef K32
#include "rdsp_model_internal.h"
#include "rdsp_vt_model.h"
#include "rdsp_vt_utils.h"
#include "rdsp_model.h"
#endif
    
#include "rdsp_model_defines.h"
    
typedef struct rdsp_model_ver_struct {
    int32_t major;
    int32_t minor;
    int32_t patch;
    uint32_t build;
}rdsp_model_version;

// Forward declaration of rdsp_model_control
typedef struct rdsp_model_control_struct rdsp_model_control;

// Check model integrity
int32_t rdspModel_CheckModelIntegrity(uint32_t model_blob_size, uint8_t* model_blob);

// Get version of the model library
extern int32_t rdspModel_GetLibVersion(rdsp_model_control* model_control, rdsp_model_version* model_version);

// Get version of the model library used to create model specified by model_handle + model string
extern int32_t rdspModel_GetModelInfo(rdsp_model_control* model_control, int32_t model_handle, rdsp_model_version* model_version, char** model_string, char*** class_string, int32_t *num_inputs, int32_t *num_outputs);

// Set internal parameter
extern int32_t rdspModel_SetParameter(rdsp_model_control* model_control, int32_t model_handle, int32_t parameter_id, uint8_t* parameter_value);

// Create model control
extern int32_t rdspModel_CreateControl(rdsp_model_control** model_control, int32_t data_type);

// Release model control and all model instances
extern int32_t rdspModel_ReleaseControl(rdsp_model_control* model_control);

// Create model instance and return model handle
extern int32_t rdspModel_CreateInstance(rdsp_model_control* model_control, int32_t* model_handle);

// Create model slave instance and return model handle
extern int32_t rdspModel_CreateSlaveInstance(rdsp_model_control* model_control, int32_t* model_handle, int32_t model_handle_master);

// Release model instance
extern int32_t rdspModel_ReleaseInstance(rdsp_model_control* model_control, int32_t model_handle);

// Open a model instance
extern int32_t rdspModel_OpenInstance(rdsp_model_control* model_control, int32_t model_handle, uint32_t model_blob_size, uint8_t* model_blob, int32_t model_blob_is_already_open, int32_t additional_framebuffer_duration);

// Close a model instance
extern int32_t rdspModel_CloseInstance(rdsp_model_control* model_control, int32_t model_handle);

// Process a frame of audio data in "frame_data" using a model instance. Returns the number of scores written into "scores"
extern int32_t rdspModel_Process(rdsp_model_control* model_control, int32_t model_handle, int32_t processing_level, uint8_t** frame_data, int32_t input_format, int32_t* num_scores, int32_t* scores);

// Process a block of data stored in the internal frame buffer using a voicespot instance. Returns the number of scores "num_scores" available in "scores"
extern int32_t rdspModel_ProcessBuffered(rdsp_model_control* model_control, int32_t model_handle, int32_t processing_level, int32_t* num_scores, int32_t* scores);

// Estimate start of trigger and return offset in frames from latest processed input frame
// score_index:         Class index of the trigger for which the timing estimate should be performed
// score_threshold:     Threshold to apply for timing estimation
// timing_accuracy:     Temporal resolution of the timing estimate, in frames
// max_num_parts:       Maximum number of parts to split the work into. Continue work by calling rdspModel_EstimateStartSplitContinue() until a non-negative timing estimate is returned.
// start_offset:        Offset back in time for the location of the start of the trigger, in frames
extern int32_t rdspModel_EstimateStartSplit(rdsp_model_control* model_control, int32_t model_handle, int32_t score_index, int32_t score_threshold, int32_t timing_accuracy, int32_t max_num_parts, int32_t* start_offset);

// Estimate start of trigger and return offset in frames from latest processed input frame
// score_index:         Class index of the trigger for which the timing estimate should be performed
// score_threshold:     Threshold to apply for timing estimation
// timing_accuracy:     Temporal resolution of the timing estimate, in frames
// start_offset:        Offset back in time for the location of the start of the trigger, in frames
extern int32_t rdspModel_EstimateStart(rdsp_model_control* model_control, int32_t model_handle, int32_t score_index, int32_t score_threshold, int32_t timing_accuracy, int32_t* start_offset);

// Continue work to get timing estimate. This must be called a number of times until a non-negative timing estimate is returned.
extern int32_t rdspModel_EstimateStartSplitContinue(rdsp_model_control* model_control, int32_t model_handle, int32_t* start_offset);

// Estimate stop of trigger and return offset in frames from latest processed input frame
// score_index:         Class index of the trigger for which the timing estimate should be performed
// score_threshold:     Threshold to apply for timing estimation
// stop_offset:         Offset back in time for the location of the stop of the trigger, in frames
extern int32_t rdspModel_EstimateStop(rdsp_model_control* model_control, int32_t model_handle, int32_t score_index, int32_t score_threshold, int32_t* stop_offset);

// Reset processing of a model instance
extern int32_t rdspModel_ResetProcessing(rdsp_model_control* model_control, int32_t model_handle);

// Get pointer to memory location
extern int32_t rdspModel_GetPointer(rdsp_model_control* model_control, int32_t model_handle, int32_t pointer_id, uint8_t** pointer);
#if defined(__cplusplus) && !defined(CPP_LINKAGE)
}
#endif
#endif // RDSP_MODEL_H
