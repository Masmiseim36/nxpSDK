/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_VOICESPOT_H
#define RDSP_VOICESPOT_H

#include <stdint.h>

#include "public/rdsp_voicespot_defines.h"
#include "public/rdsp_voicespot_utils.h"
#include "public/rdsp_voicespot_sleep_cbuffer.h"

// External API header for RDSP VoiceSpot
#if defined(__cplusplus) && !defined(CPP_LINKAGE)
extern "C" {
#endif

typedef struct rdsp_voicespot_ver_struct {
    int32_t major;
    int32_t minor;
    int32_t patch;
    uint32_t build;
} rdsp_voicespot_version;

// Forward declaration of rdsp_voicespot_control
typedef struct rdsp_voicespot_control_struct rdsp_voicespot_control;

// Check model integrity
int32_t rdspVoiceSpot_CheckModelIntegrity(uint32_t model_blob_size, uint8_t* model_blob);

// Get version of the voicespot library
extern int32_t rdspVoiceSpot_GetLibVersion(rdsp_voicespot_control* voicespot_control, rdsp_voicespot_version* voicespot_version);

// Get version of the voicespot library used to create model specified by voicespot_handle + model string
extern int32_t rdspVoiceSpot_GetModelInfo(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, rdsp_voicespot_version* voicespot_version, char** voicespot_model_string, char*** voicespot_class_string, int32_t *num_inputs, int32_t *num_outputs);

// Set internal parameter
extern int32_t rdspVoiceSpot_SetParameter(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t parameter_id, uint8_t* parameter_value, uint32_t parameter_size);

// Set internal parameters from parameter blob
extern int32_t rdspVoiceSpot_SetParametersFromBlob(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, uint8_t* parameter_blob);

// Create voicespot control
extern int32_t rdspVoiceSpot_CreateControl(rdsp_voicespot_control** voicespot_control, int32_t data_type);

// Release voicespot control and all instances
extern int32_t rdspVoiceSpot_ReleaseControl(rdsp_voicespot_control* voicespot_control);

// Create voicespot instance and return voicespot handle
extern int32_t rdspVoiceSpot_CreateInstance(rdsp_voicespot_control* voicespot_control, int32_t* voicespot_handle, int32_t enable_hp, int32_t generate_debug_audio);

// Create voicespot slave instance and return voicespot handle
extern int32_t rdspVoiceSpot_CreateSlaveInstance(rdsp_voicespot_control* voicespot_control, int32_t* voicespot_handle, int32_t voicespot_handle_master);

// Release voicespot instance
extern int32_t rdspVoiceSpot_ReleaseInstance(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle);

// Open a voicespot instance and return a potential error code
extern int32_t rdspVoiceSpot_OpenInstance(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, uint32_t model_blob_size, uint8_t* model_blob, int32_t model_blob_is_already_open, int32_t additional_framebuffer_duration);

// Close a voicespot instance
extern int32_t rdspVoiceSpot_CloseInstance(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle);

// Process a frame of audio data in "frame_data" using a voicespot instance. Returns the number of scores "num_scores" available in "scores"
extern int32_t rdspVoiceSpot_Process(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t processing_level, uint8_t* frame_data, int32_t* num_scores, int32_t* scores, uint8_t** recon_frame_data);

// Process a block of data stored in the internal frame buffer using a voicespot instance. Returns the number of scores "num_scores" available in "scores"
extern int32_t rdspVoiceSpot_ProcessBuffered(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t processing_level, int32_t* num_scores, int32_t* scores);

// Estimate start of trigger and return offset in samples from the end of latest processed input frame
// score_index:         Class index of the trigger for which the timing estimate should be performed
// score_threshold:     Threshold to apply for timing estimation
// timing_accuracy:     Temporal resolution of the timing estimate, in frames
// max_num_parts:       Maximum number of parts to split the work into. Continue work by calling rdspVoiceSpot_EstimateStartSplitContinue() until a non-negative timing estimate is returned.
// start_offset:        Offset back in time for the location of the start of the trigger, in samples
extern int32_t rdspVoiceSpot_EstimateStartSplit(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t score_index, int32_t score_threshold, int32_t timing_accuracy, int32_t max_num_parts, int32_t* start_offset);

// Estimate start of trigger and return offset in samples from the end of latest processed input frame
// score_index:         Class index of the trigger for which the timing estimate should be performed
// score_threshold:     Threshold to apply for timing estimation
// timing_accuracy:     Temporal resolution of the timing estimate, in frames
// start_offset:        Offset back in time for the location of the start of the trigger, in samples
extern int32_t rdspVoiceSpot_EstimateStart(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t score_index, int32_t score_threshold, int32_t timing_accuracy, int32_t* start_offset);

// Continue work to get timing estimate. This must be called a number of times until a non-negative timing estimate is returned.
extern int32_t rdspVoiceSpot_EstimateStartSplitContinue(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t score_index, int32_t timing_accuracy, int32_t* start_offset);

// Estimate stop of trigger and return offset in samples from the end of latest processed input frame
// score_index:         Class index of the trigger for which the timing estimate should be performed
// score_threshold:     Threshold to apply for timing estimation
// timing_accuracy:     Temporal resolution of the timing estimate, in frames
// stop_offset:         Offset back in time for the location of the stop of the trigger, in samples
extern int32_t rdspVoiceSpot_EstimateStop(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t score_index, int32_t score_threshold, int32_t* stop_offset);

// Estimate start and stop of trigger and return offset in samples from the end of latest processed input frame
extern int32_t rdspVoiceSpot_EstimateStartAndStop(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t score_index, int32_t score_threshold, int32_t timing_accuracy, int32_t* start_offset, int32_t* stop_offset);

// Reset processing of a voicespot instance
extern int32_t rdspVoiceSpot_ResetProcessing(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle);

// Reset the Adaptive Power State of a voicespot instance
extern int32_t rdspVoiceSpot_ResetAdaptivePowerState(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle);

// Get pointer to memory location
extern int32_t rdspVoiceSpot_GetPointer(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t pointer_id, uint8_t** pointer);

// Create VAD
extern int32_t rdspVoiceSpot_EnableAdaptiveThreshold(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t Aadapt_threshold_mode);

// Create Power state handler
// If external buffer is used it must be allocated as desired length + 1 frame and the buffer length reported must be the full buffer.
// The reason is that the buffer is assumed empty when read_index == write_index
extern int32_t rdspVoiceSpot_EnableAdaptivePowerState(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t Aadapt_power_state_mode, rdsp_voicespot_sleep_cbuffer* voicespot_buffer);

// Check if triggered
extern int32_t rdspVoiceSpot_CheckIfTriggered(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, int32_t* Ascores, int32_t Aallow_trigger, int32_t* Acurrent_thresholds, int32_t processing_period);

typedef void (funcVoiceSpotProcessingStatusCallback)(int32_t voicespot_handle, rdsp_voicespot_processing_status processing_status);

// Function to register a function as callback for events.
// The callback is called with a rdsp_voicespot_processing_status enum.
// The register function returns 0 if successful and -1 otherwise.
extern int32_t rdspVoiceSpot_RegisterPowerStateStatusCallback(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, funcVoiceSpotProcessingStatusCallback* callback);

typedef void (funcVoiceSpotReadFrameCallback)(int32_t voicespot_handle, uint8_t** frame_pointer);

// Function to register a function as callback for buffer reads.
// The callback returns is called with a uint8_t**. The pointer at that address must be set to point at at frame of data in the correct input format.
// The register function returns 0 if successful and -1 otherwise.
extern int32_t rdspVoiceSpot_RegisterReadFrameCallback(rdsp_voicespot_control* voicespot_control, int32_t voicespot_handle, funcVoiceSpotReadFrameCallback* callback);

#if defined(__cplusplus) && !defined(CPP_LINKAGE)
}
#endif

#endif // RDSP_VOICESPOT_H
