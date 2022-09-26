/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "string.h"

#if defined(HIFI4)
#include <xtensa/tie/xt_hifi4.h>
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#elif defined(HIFI3)
#include <xtensa/tie/xt_hifi3.h>
#include "NatureDSP_Signal_math.h"
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#elif defined(FUSIONF1)
#include <xtensa/tie/xt_fusion.h>
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#elif defined(HEMILITE)
#include "hemilite_fr32_utils.h"
#elif defined(HMD)
#include "fr32_utils.h"
#elif defined(HMD1A)
#else
#include <math.h>
#endif

#if !defined(SIMULATED) && (defined(HEMILITE) || defined(HMD1A) || defined(DMX1A))
#include "rdsp_plugin_utils.h"
#endif


#ifdef LOG_CC
FILE* fid_cc_log;
#endif

/* Pick Model */
#include "public/rdsp_voicespot.h"
#include "public/rdsp_voicespot_utils.h"
#include "models/Alexa/EN-US/Alexa-en_US_5-rc.h"
#include "models/Alexa/EN-US/Alexa-en_US_5-rc_params.h"
#define VOICESPOT_NUM_SAMPLES_PER_FRAME 200
#define VOICESPOT_SAMPLE_RATE 16000
#define VOICESPOT_SLEEP_BUFFER_LENGTH (41 * VOICESPOT_NUM_SAMPLES_PER_FRAME)     // Specify the number of samples in the sleep buffer you wish to use here
float sleep_buffer[VOICESPOT_SLEEP_BUFFER_LENGTH];     // Memory for circular sleep buffer. The type of this buffer can be whatever as long as the data type is correctly converted to the correct VoiceSpot input type in voicespot_read_frame_callback()
float read_frame_buffer[VOICESPOT_NUM_SAMPLES_PER_FRAME];
int32_t* read_index_ptr;
volatile int32_t* write_index_ptr;

int64_t frame_count = 0; // Frame counter
int64_t frame_count_trigger = -1;           // Frame count where last trigger occurred

// WAVE PCM soundfile format, see e.g. http://soundfile.sapp.org/doc/WaveFormat/
typedef struct header_file {
    char chunk_id[4];
    int32_t chunk_size;
    char format[4];
    char subchunk1_id[4];
    int32_t subchunk1_size;
    int16_t audio_format;
    int16_t num_channels;
    int32_t sample_rate; // sample_rate denotes the sampling rate.
    int32_t byte_rate;
    int16_t block_align;
    int16_t bits_per_sample;
} wav_header;
#define WAVE_FORMAT_PCM 0x0001
#define WAVE_FORMAT_IEEE_FLOAT 0x0003


static int usage(int argc, char **argv) {
    printf("Command:");
    for (int i = 0; i < argc; i++) {
        printf(" %s", argv[i]);
    }
    printf("\n");

    printf("Usage: %s [-m model_file_name] [-p param_file_name] wav_file_name_in\n"
            "[-m model_file_name]: Model file.\n"
            "[-p param_file_name]: Parameter file.\n"
            "[-d estimation_delay_frames]: Delay from trigger to estimation (in frames).\n"
            "[-s power_state_mode]: Power State mode. (0 off, 1 on).\n"
            "[-b power_state_buffer_length_num_samples]: Power State sleep_buffer length in samples.\n"
            "[-a power_state_timeout_active2low_num_frames]: Timeout length for Active to Low in frames. Must be multiple of 4.\n"
            "[-k trigger_key_file_name]: Use trigger key to remove false positives.\n"
            "[--adapt_threshold_mode mode]: Use adapt_threshold_mode = mode.\n"
            "[--enable_blanking_detection blanking_length]: Enable blanking detection.\n"
            "wav_file_name_in: Input wave file.\n", argv[0]);
    return -1;
}

// Callback function for VoiceSpot processing state change (only needed when enabling the adaptive power state (power_state_mode_in != 0))
void voicespot_processing_state_callback(int32_t voicespot_handle, rdsp_voicespot_processing_status processing_status) {
    static int64_t frame_count_full = 0;
    static int64_t frame_count_normal = 0;
    int32_t num_frames_per_minute = 60 * VOICESPOT_SAMPLE_RATE / VOICESPOT_NUM_SAMPLES_PER_FRAME;
    int32_t minutes = frame_count / num_frames_per_minute;
    float seconds = (float)(frame_count - (minutes * num_frames_per_minute)) / (VOICESPOT_SAMPLE_RATE / VOICESPOT_NUM_SAMPLES_PER_FRAME);
    
    switch (processing_status) {
    case RDSP_REQUEST_FULL_PROCESSING: // VoiceSpot request permission to go to Full processing state. Do not return until ready.
        printf("[%d:%.1f] VoiceSpot instance with handle %d is requesting permission to process at Full.\n", minutes, seconds, voicespot_handle);
        break;
    case RDSP_IS_PROCESSING_FULL: // VoiceSpot has switched to Full processing state
        frame_count_full = frame_count;     // Store current frame_count
        printf("[%d:%.1f] VoiceSpot instance with handle %d is processing at Full.\n", minutes, seconds, voicespot_handle);
        break;
    case RDSP_IS_PROCESSING_NORMAL: // VoiceSpot has switched to Normal processing state
        frame_count_normal = frame_count;     // Store current frame_count
        printf("[%d:%.1f] VoiceSpot instance with handle %d is processing at Normal. Ran Full processing for %.2f seconds\n", minutes, seconds, voicespot_handle, ((float)(frame_count - frame_count_full) / (VOICESPOT_SAMPLE_RATE / VOICESPOT_NUM_SAMPLES_PER_FRAME)));
        break;
    case RDSP_IS_PROCESSING_VERY_LOW: // VoiceSpot has switched to Very Low processing state
        printf("[%d:%.1f] VoiceSpot instance with handle %d is processing at Very Low. Ran Normal processing for %.2f seconds\n", minutes, seconds, voicespot_handle, ((float)(frame_count - frame_count_normal) / (VOICESPOT_SAMPLE_RATE / VOICESPOT_NUM_SAMPLES_PER_FRAME)));
        break;
    default:
        break;
    }
}

// Callback function for reading a single frame from the circular sleep buffer (only needed when enabling the adaptive power state (power_state_mode_in != 0))
void voicespot_read_frame_callback(int32_t voicespot_handle, uint8_t** frame_pointer) {
    // The pointer (*frame_pointer) must point to the location of the next data frame to be read from the circular sleep buffer, converted into the VoiceSpot input data type
    
    // Create pointer to data frame
    float* buffer_ptr = (float*) sleep_buffer;                                    // Create pointer to buffer
    float* buffer_read_ptr = (float*) &sleep_buffer[*read_index_ptr];

    // Check for wrap around 
    int32_t n_avail_before_wrap = VOICESPOT_SLEEP_BUFFER_LENGTH - *read_index_ptr;

    int32_t n_samples_copy = VOICESPOT_NUM_SAMPLES_PER_FRAME;
    if (n_avail_before_wrap < VOICESPOT_NUM_SAMPLES_PER_FRAME)
        n_samples_copy = n_avail_before_wrap;

    int32_t n_samples_remain = VOICESPOT_NUM_SAMPLES_PER_FRAME - n_samples_copy;

    if (n_samples_remain > 0) {
        rdsp_memcpy(read_frame_buffer + n_samples_copy, buffer_ptr, n_samples_remain * sizeof(float));  // Copy remaining samples from buffer
    }

    rdsp_memcpy(read_frame_buffer, buffer_read_ptr, n_samples_copy * sizeof(float));                    // Copy first part of frame from buffer to temp frame buffer

    // Set internal VoiceSpot read pointer to data frame pointer
    *frame_pointer = (uint8_t*) read_frame_buffer;
}

int32_t perform_blanking_detection(FILE* infile, int32_t blanking_length, int32_t* num_blankings_found, uint32_t** blanking_start_samples) {
    // Detect blanking for entire input file
    
    // Read entire file into input buffer
    wav_header header;
    fseek(infile, 0, SEEK_END); // Jump to the end of the file
    uint32_t audio_size = ftell(infile) - sizeof(wav_header); // Get the current byte offset in the file
    rewind(infile); // Jump back to the beginning of the file
    fread(&header, 1, sizeof(wav_header), infile);     // Read header to advance read location
    int32_t bytes_per_sample = header.bits_per_sample / 8; // Number of bytes per sample
    uint32_t num_samples = audio_size / bytes_per_sample;
    void* audio_buffer = rdsp_malloc(audio_size);
    uint32_t bytes_read = fread(audio_buffer, 1, audio_size, infile);
    rewind(infile); // Jump back to the beginning of the file
    fread(&header, 1, sizeof(wav_header), infile);     // Read header to advance read location
    if (bytes_read != audio_size) {
        printf("Unable to read expected amount of data from audio file\n");
        return -1;
    }
        
    // Detect blankings and put them into blanking_start_samples
    int32_t blanking_value = 0;                            // Value of blanking to search for
    int32_t blanking_start = -1;
    int32_t blanking_stop = -1;
    *blanking_start_samples = rdsp_malloc((audio_size / blanking_length) * sizeof(uint32_t));   // Allocate space for the maximum possible number of blankings
    for (int32_t i = 0; i < num_samples; i++) {
        int32_t blanking_match = 0;
        if (bytes_per_sample == sizeof(int16_t)) {
            blanking_match = ((int16_t*)audio_buffer)[i] == (int16_t) blanking_value;
        } else if (bytes_per_sample == sizeof(int32_t)) {
            blanking_match = ((int32_t*)audio_buffer)[i] == (int32_t) blanking_value;
        }
                
        if (blanking_match) {
            if (blanking_start == -1)
                blanking_start = i;
        } else {
            if (blanking_start >= 0 && blanking_stop == -1) {
                blanking_stop = i;
                if (blanking_stop - blanking_start >= blanking_length) {
                    (*blanking_start_samples)[*num_blankings_found] = blanking_start;   // Store blanking_start in array
                    (*num_blankings_found)++;                                           // Increment the number of blankings found
                    //printf("[%d] Blanking found: blanking_start_samples = %d\n", *num_blankings_found, blanking_start);
                }
                // Reset search
                blanking_start = -1;
                blanking_stop = -1;
            }
        }
    }
    
    rdsp_free(audio_buffer);
    return 0;
}

int basic_main(int argc, char **argv) {
    char *model_file_name = NULL, *param_file_name = NULL, *wav_file_name_in = NULL, * trigger_key_file_name = NULL;

    int32_t power_state_mode_in = 0;
    int32_t adapt_threshold_mode_in = -1;
    int32_t power_state_buffer_length_num_samples_in = -1;
    int32_t power_state_timeout_active2verylow_num_frames_in = -1;
    int32_t use_trigger_key = 0;
    int32_t* trigger_key = NULL;
    int32_t trigger_keys_num_entries;
    int32_t blanking_length = 0;      // Enable detecting a consecutive number of the blanking value (blanking_length samples having the value zero) as an indication that the microphone stream has been switched off
    int32_t num_blankings_found = 0;            // Number of blankings found
    int32_t num_blankings_removed = 0;          // Number of blankings removed
    uint32_t* blanking_start_samples = NULL;    // Array of blanking start locations in samples
    int32_t estimation_delay_frames = 0;        // Number of frames to delay estimation from trigger occurred. If an accurate stop estimate is needed, a delay may be needed as trigger may occur before keyword has been completely spoken.

    // Parse input arguments
    if (argc < 2) {
        return usage(argc, argv);
    }

    int i = 1;
    for (; i < argc - 1; i++) {
        if (strcmp("-m", argv[i]) == 0) {
            model_file_name = argv[++i];
        } else if (strcmp("-p", argv[i]) == 0) {
            param_file_name = argv[++i];
        } else if (strcmp("-d", argv[i]) == 0) {
            estimation_delay_frames = atoi(argv[++i]);
        } else if (strcmp("-s", argv[i]) == 0) {
            power_state_mode_in = atoi(argv[++i]);
        } else if (strcmp("-b", argv[i]) == 0) {
            power_state_buffer_length_num_samples_in = atoi(argv[++i]);
        } else if (strcmp("-a", argv[i]) == 0) {
            power_state_timeout_active2verylow_num_frames_in = atoi(argv[++i]);
        } else if (strcmp("-k", argv[i]) == 0) {
            trigger_key_file_name = argv[++i];
            use_trigger_key = 1;
        } else if (strcmp("--adapt_threshold_mode", argv[i]) == 0) {
            adapt_threshold_mode_in = atoi(argv[++i]);
        } else if (strcmp("--enable_blanking_detection", argv[i]) == 0) {
            blanking_length = atoi(argv[++i]);
        } else {
            printf("Error parsing argument %d: %s\n", i, argv[i]);
            return usage(argc, argv);
        }
    }
    wav_file_name_in = argv[i];

#if (defined(HIFI3) || defined(HIFI4) || defined(FUSIONF1) || defined(HEMILITE) || defined(HMD1A) || defined(DMX1A) || defined(HIFI3) || defined(HIFI4))
    const uint32_t plugin_heap_memory_size = 100000 & 0xFFFFFFF0; // Size is multiple of 16
    uint8_t __attribute__((aligned (16))) plugin_heap_memory[plugin_heap_memory_size];
    rdsp_plugin_malloc_init(plugin_heap_memory, plugin_heap_memory, plugin_heap_memory_size);
#endif

#ifdef LOG_CC
#ifdef _MSC_VER
    char log_path[] = "C:\\logs\\"; // Windows path
#else    
    char log_path[] = "~/logs/"; // Linux path
#endif

    memcpy(log_path_plus_file, log_path, sizeof(log_path));
    strcat(log_path_plus_file, "cc_main.log");
    
    fid_cc_log = fopen(log_path_plus_file, "w");
    if (fid_cc_log == NULL) {
        printf("Error: Unable to open log file\n");
        return -1;
    }
#endif

    if (use_trigger_key == 1) {
        //int32_t trigger_key[50] = { 2464, 4640, 6836, 9012, 11212, 13400, 15592, 17780, 19964, 22152, 24352, 26540, 28728, 30908, 33100, 35288, 37484, 39648, 41840, 44036, 46224, 48424, 50608, 52792, 54992, 57200, 59368, 61544, 63740, 65928, 68116, 70304, 72492, 74680, 76872, 79056, 81240, 83428, 85616, 87784, 89996, 92168, 94372, 96560, 98752, 100948, 103120, 105312, 107500, 109692 };
        FILE* trigger_key_FID;
        trigger_key_FID = fopen(trigger_key_file_name, "r");

        if (trigger_key_FID == NULL) {
            printf("Error: Unable to open trigger key file\n");
            exit(-1);
        }

        fscanf(trigger_key_FID, "%d", &trigger_keys_num_entries);
        trigger_key = (int32_t*)rdsp_malloc(trigger_keys_num_entries * sizeof(int32_t));

        for (int i = 0; i < trigger_keys_num_entries; i++) {
            fscanf(trigger_key_FID, "%d", &trigger_key[i]);
            if (feof(trigger_key_FID)) {
                break;
                printf("Error: Wrong number of trigger keys in trigger key file\n");

            }
        }
        fclose(trigger_key_FID);
    }

    // Setup VoiceSpot
#if defined(HIFI3) || defined(HIFI4)
    char str[128];
    NatureDSP_Signal_get_library_version(str);
//    printf("NatureDSP_Signal_get_library_version = %s\n", str);
#endif

    int32_t voicespot_status;
    rdsp_voicespot_control* voicespot_control; // Pointer to VoiceSpot control struct
#if defined(HIFI4)
    int32_t data_type = RDSP_DATA_TYPE__INT32; // Input is int32_t, fixed-point mode computations
    //int32_t data_type = RDSP_DATA_TYPE__FLOAT32; // Input is int32_t, floating-point mode computations
#elif defined(HIFI3) || defined(FUSIONF1)
    int32_t data_type = RDSP_DATA_TYPE__INT32; // Input is int32_t, fixed-point mode computations
#elif defined(HEMILITE) || defined(HMD) || defined(HMD1A)
    int32_t data_type = RDSP_DATA_TYPE__INT32; // Input is int32_t, fixed-point mode computations
#else
    int32_t data_type = RDSP_DATA_TYPE__FLOAT32; // Input is float, floating-point mode computations
#endif

    voicespot_status = rdspVoiceSpot_CreateControl(&voicespot_control, data_type); // Create VoiceSpot control structure
    printf("rdspVoiceSpot_CreateControl: voicespot_status = %d\n", (int) voicespot_status);
    int32_t voicespot_handle; // VoiceSpot handle
    int32_t enable_highpass_filter = 1;
    int32_t generate_output = 0;

    voicespot_status = rdspVoiceSpot_CreateInstance(voicespot_control, &voicespot_handle, enable_highpass_filter, generate_output); // Create VoiceSpot instance
    printf("rdspVoiceSpot_CreateInstance: voicespot_status = %d\n", (int) voicespot_status);

    uint8_t *model_blob = NULL;
    uint32_t model_blob_size = 0;
    if (model_file_name != NULL ) {
        FILE* fileptr = fopen(model_file_name, "rb"); // Open the file in binary mode
        if (fileptr == NULL ) {
            printf("Error: Model file could not be found\n");
            return -1;
        }
        fseek(fileptr, 0, SEEK_END); // Jump to the end of the file
        model_blob_size = ftell(fileptr); // Get the current byte offset in the file
        rewind(fileptr); // Jump back to the beginning of the file
        model_blob = rdsp_malloc_align(model_blob_size, 16);
        fread(model_blob, model_blob_size, 1, fileptr); // Read in the entire file
        fclose(fileptr); // Close the file
    } else {
#ifdef RDSP_MODEL_BLOB_H
        // Get model_blob from header file
        model_blob_size = sizeof(model_blob_header);
        model_blob = rdsp_malloc_align(model_blob_size, 16); // Allocate memory for an aligned copy of the model contained in header file. This allows dynamic modification for e.g. weight permutation depending on target platform
        rdsp_memcpy(model_blob, (uint8_t*) model_blob_header, model_blob_size); // Copy model_blob from header file
#endif
    }
    if (rdspVoiceSpot_CheckModelIntegrity(model_blob_size, model_blob) != RDSP_VOICESPOT_OK) {      // Check the integrity of the model
        printf("rdspVoiceSpot_CheckModelIntegrity: Model integrity check failed\n");
        return -1;
    }
    voicespot_status = rdspVoiceSpot_OpenInstance(voicespot_control, voicespot_handle, model_blob_size, model_blob, 0, 0); // Open the VoiceSpot instance
    printf("rdspVoiceSpot_OpenInstance: voicespot_status = %d\n", (int) voicespot_status);
            
    // Adaptive threshold modes
    // 0: fixed threshold (uses the array event_thresholds as thresholds)
    // 1: adaptive threshold
    // 2: adaptive sensitivity
    // 3: adaptive threshold + adaptive sensitivity
    int32_t adapt_threshold_mode = 3;
    if (adapt_threshold_mode_in >= 0)
        adapt_threshold_mode = adapt_threshold_mode_in;     // Let value provided via command-line override
    voicespot_status = rdspVoiceSpot_EnableAdaptiveThreshold(voicespot_control, voicespot_handle, adapt_threshold_mode); // Enable use of the Adaptive Threshold mechanism
    printf("rdspVoiceSpot_EnableAdaptiveThreshold: voicespot_status = %d\n", (int)voicespot_status);

    // Set up parameters using a parameter blob
	uint8_t *param_blob = NULL;
	uint32_t param_blob_size = 0;
	if (param_file_name != NULL ) {
		FILE* fileptr = fopen(param_file_name, "rb"); // Open the file in binary mode
		if (fileptr == NULL ) {
			printf("Parameter file not be found: %s\n", param_file_name);
			return -1;
		}
		fseek(fileptr, 0, SEEK_END); // Jump to the end of the file
		param_blob_size = ftell(fileptr); // Get the current byte offset in the file
		rewind(fileptr); // Jump back to the beginning of the file
		param_blob = rdsp_malloc(param_blob_size);
		fread(param_blob, param_blob_size, 1, fileptr); // Read in the entire file
		fclose(fileptr); // Close the file

		voicespot_status = rdspVoiceSpot_SetParametersFromBlob(voicespot_control, voicespot_handle, param_blob);
		printf("rdspVoiceSpot_SetParametersFromBlob: voicespot_status = %d\n", (int) voicespot_status);
		rdsp_free(param_blob);
	} else {
#ifdef RDSP_PARAM_BLOB_H
		param_blob_size = sizeof(param_blob_header);
		param_blob = (uint8_t*) param_blob_header;

		voicespot_status = rdspVoiceSpot_SetParametersFromBlob(voicespot_control, voicespot_handle, param_blob);
		printf("rdspVoiceSpot_SetParametersFromBlob: voicespot_status = %d\n", (int) voicespot_status);
#endif
	}
        
    rdsp_voicespot_version voicespot_version;
    char* voicespot_model_string;
    char** voicespot_class_string;
    int32_t num_samples_per_frame;
    int32_t num_outputs;
    rdspVoiceSpot_GetLibVersion(voicespot_control, &voicespot_version);
    printf("VoiceSpot library version: %d.%d.%d.%u\n", (int) voicespot_version.major, (int) voicespot_version.minor, (int) voicespot_version.patch, (unsigned int) voicespot_version.build);
    rdspVoiceSpot_GetModelInfo(voicespot_control, voicespot_handle, &voicespot_version, &voicespot_model_string, &voicespot_class_string, &num_samples_per_frame, &num_outputs);
    printf("VoiceSpot model version: %d.%d.%d\n", (int) voicespot_version.major, (int) voicespot_version.minor, (int) voicespot_version.patch);
    if (voicespot_model_string != NULL)
        printf("VoiceSpot model string: %s\n", voicespot_model_string);
    if (VOICESPOT_NUM_SAMPLES_PER_FRAME != num_samples_per_frame) {
        printf("Error: Mismatch in frame size\n");
        return -1;
    }
    int32_t num_frames_per_second = VOICESPOT_SAMPLE_RATE / num_samples_per_frame;

    // Adaptive power state modes
    // 0: Off/manual
    // 1: Auto
    int32_t adapt_power_state_mode = power_state_mode_in;
    int32_t buffer_size_num_samples = VOICESPOT_SLEEP_BUFFER_LENGTH;
    if (power_state_buffer_length_num_samples_in >= 0)
        buffer_size_num_samples = power_state_buffer_length_num_samples_in;

    rdsp_voicespot_sleep_cbuffer* voicespot_buffer_struct = NULL;		// Buffer struct
    if (adapt_power_state_mode != 0) {
        voicespot_buffer_struct = (rdsp_voicespot_sleep_cbuffer*) rdsp_malloc(sizeof(rdsp_voicespot_sleep_cbuffer));
        if (voicespot_buffer_struct == NULL) {
            return -1;
        }
        voicespot_buffer_struct->length_samples = buffer_size_num_samples;
        voicespot_buffer_struct->read_index_samples = 0;
        voicespot_buffer_struct->write_index_samples = 0;
        voicespot_buffer_struct->buffer = (uint8_t*) sleep_buffer;
        if (voicespot_buffer_struct->buffer == NULL) {
            return -1;
        }
        read_index_ptr = &voicespot_buffer_struct->read_index_samples;
        write_index_ptr = &voicespot_buffer_struct->write_index_samples;

        voicespot_status = rdspVoiceSpot_EnableAdaptivePowerState(voicespot_control, voicespot_handle, adapt_power_state_mode, voicespot_buffer_struct); // Create VoiceSpot Power State handler
        printf("adapt_power_state_mode = %i\n", adapt_power_state_mode);
        printf("buffer_size_num_samples = %i\n", voicespot_buffer_struct->length_samples);
        printf("rdspVoiceSpot_EnableAdaptivePowerState: voicespot_status = %d\n", (int)voicespot_status);

        voicespot_status = rdspVoiceSpot_RegisterPowerStateStatusCallback(voicespot_control, voicespot_handle, voicespot_processing_state_callback);
        printf("rdspVoiceSpot_RegisterCallback: voicespot_status = %d\n", (int)voicespot_status);

        voicespot_status = rdspVoiceSpot_RegisterReadFrameCallback(voicespot_control, voicespot_handle, voicespot_read_frame_callback);
        printf("rdspVoiceSpot_RegisterReadFrameCallback: voicespot_status = %d\n", (int)voicespot_status);

        if (power_state_timeout_active2verylow_num_frames_in >= 0) {
            int32_t parameter_id = 0x401;
            int32_t parameter_value[] = { power_state_timeout_active2verylow_num_frames_in };
            voicespot_status = rdspVoiceSpot_SetParameter(voicespot_control, voicespot_handle, parameter_id, (uint8_t*)parameter_value, sizeof(parameter_value));
            printf("rdspVoiceSpot_SetParameter: voicespot_status = %d\n", (int)voicespot_status);
            printf("Set power_state_timeout_active2verylow_num_frames = %d\n", (int)power_state_timeout_active2verylow_num_frames_in);
        }
    }
    
#if !defined(SIMULATED) && (defined(HEMILITE) || defined(HMD1A) || defined(DMX1A))
    uint32_t nb = rdsp_plugin_malloc_GetNumAllocatedBytes();
    printf("VoiceSpot allocated %u bytes\n", nb);
#endif

    int32_t** sfb_output = NULL;
    if (generate_output)
        sfb_output = (int32_t**) rdsp_malloc(num_samples_per_frame * sizeof(uint32_t));

    int32_t* max_scores = (int32_t*) rdsp_malloc(num_outputs * sizeof(int32_t));
    int64_t* max_frame_count = (int64_t*) rdsp_malloc(num_outputs * sizeof(int64_t));

    for (int i = 0; i < num_outputs; i++) {
        max_scores[i] = 0;
        max_frame_count[i] = -1;
    }
    // event_thresholds is the array of manually set minimum thresholds for trigger event per class. NULL means automatic, i.e. no manually set minimum thresholds.
    int32_t* event_thresholds = NULL;   // event_thresholds = NULL means automatic, i.e. no manually set minimum thresholds.
        
    int32_t event_duration_frames = 2 * num_frames_per_second;
    int64_t last_event = -event_duration_frames;
    int32_t processing_period = 4;
    int32_t score_index_trigger;                // Used for saving score_index of latest trigger
#ifdef PROMPT_CC
    float accum_cc = 0;
    float total_accum_cc = 0;
    uint32_t peak_cc = 0;
    rdsp_init_cycle_counter();
#endif

    int32_t* file_buffer = (int32_t*) rdsp_malloc(num_samples_per_frame * sizeof(int32_t)); // File buffer
    int32_t* frame_buffer = (int32_t*) rdsp_malloc(num_samples_per_frame * sizeof(int32_t)); // Frame buffer into VoiceSpot

    int32_t* scores = (int32_t*) rdsp_malloc(num_outputs * sizeof(int32_t));
    int32_t* scores_trigger = (int32_t*)rdsp_malloc(num_outputs * sizeof(int32_t)); // Used for saving scores of latest trigger
    int32_t num_trigger_events = 0; // Number of triggers events found in audio
    wav_header *header = (wav_header*) rdsp_malloc(sizeof(wav_header));
    
    int32_t return_val = 0;
    FILE* infile = NULL;
    if (!(infile = fopen(wav_file_name_in, "rb"))) { // Open wave file in read mode
        printf("Unable to open file %s\n", wav_file_name_in);
        printf("Maybe too many files are open! Xtensa simulator can only handle few open files\n");
        return_val = -1;
        goto exit;
    } else {
        printf("Opening file: %s\n", wav_file_name_in);
        int32_t header_size = fread(header, 1, sizeof(wav_header), infile);
        if (memcmp(header->subchunk1_id, "fmt ", 4)) {
            printf("Incompatible wav file header\n");    // Current implementation must have the "fmt " chunk as the first chunk
            return_val = -1;
            goto exit;
        }

        // Advance wav file to the first data element
        uint8_t read_values[24];    // Up to 40 - 16 = 24 additional bytes may be read from "fmt " chunk
        int32_t bytes_to_read = header->subchunk1_size - 16;    // 16 Bytes of the "fmt " chunk has already been read
        int32_t bytes_read = fread(read_values, 1, bytes_to_read, infile);
        while ((bytes_read = fread(read_values, 1, sizeof(uint32_t), infile)) == sizeof(uint32_t)) {
            if (!memcmp(read_values, "data", sizeof(uint32_t))) {
                bytes_read = fread(read_values, 1, sizeof(uint32_t), infile);    // Also read the data chunk size to advance the file to the first element of the data array
                break;
            }
        }
        if (bytes_read != sizeof(uint32_t)) {
            printf("Unable to find data chunk in input wav file\n");    // Unable to find "data" chunk in wav file
            return_val = -1;
            goto exit;
        }

        int32_t bytes_per_sample = header->bits_per_sample / 8; // Number of bytes per sample
        printf("Sampling rate: %d Hz\n", (int) header->sample_rate);
        printf("Number of bits per sample: %d\n", (int) header->bits_per_sample);

        if ((header_size != sizeof(wav_header)) || (header->num_channels != 1) || (header->sample_rate != 16000) || !(bytes_per_sample == sizeof(int16_t) || bytes_per_sample == sizeof(int32_t))) {
            printf("Only input files with 1 channel, 16/32 bits per sample and 16 kHz sample rate are supported\n");
            return_val = -1;
            goto exit;
        }
        
        // Perform blanking detection (not needed in a product implementation)
        if (blanking_length > 0) {
            if ((return_val = perform_blanking_detection(infile, blanking_length, &num_blankings_found, &blanking_start_samples)))
                goto exit;
        }

        // Frame processing loop
        while (num_samples_per_frame * bytes_per_sample == fread(file_buffer, 1, num_samples_per_frame * bytes_per_sample, infile)) { // Read a frame from file

            if (bytes_per_sample == sizeof(int16_t) && header->audio_format == WAVE_FORMAT_PCM) {
                // Convert from 16 bit fixed-point to 32 bits
                for (int32_t i = 0; i < num_samples_per_frame; i++) {
#if defined(HIFI3) || defined(HIFI4) || defined(FUSIONF1)
                    ((int32_t*)frame_buffer)[i] = ((int16_t*)file_buffer)[i] << 16; // Hifi implementation input is 32 bit fixed-point
#elif defined(HEMILITE) || defined(HMD) || defined(HMD1A)
                    ((fr32*) frame_buffer)[i] = Q15_to_afloat(((int16_t*) file_buffer)[i]); // Input is afloat
#else
                            ((float*)frame_buffer)[i] = ((int16_t*)file_buffer)[i] * (1.0f / 32768.0f); // Input is 32 bit floating-point, here normalized to the interval -1 to +1
#endif // HIFI
                }
            }

            if (bytes_per_sample == sizeof(int32_t) && header->audio_format == WAVE_FORMAT_PCM) {
                // Convert from 32 bit fixed-point to 32 bits
                for (int32_t i = 0; i < num_samples_per_frame; i++) {
#if defined(HIFI3) || defined(HIFI4) || defined(FUSIONF1)
                    ((int32_t*)frame_buffer)[i] = ((int32_t*)file_buffer)[i]; // Hifi implementation input is 32 bit fixed-point
#elif defined(HEMILITE) || defined(HMD) || defined(HMD1A)
                    ((fr32*) frame_buffer)[i] = Q31_to_afloat(((int32_t*) file_buffer)[i]); // Input is afloat
#else
                            ((float*)frame_buffer)[i] = ((int32_t*)file_buffer)[i] * (1.0f / 2147483648.0f); // Input is 32 bit floating-point, here normalized to the interval -1 to +1
#endif // HIFI
                }
            }

            if (bytes_per_sample == sizeof(float) && header->audio_format == WAVE_FORMAT_IEEE_FLOAT) {
                // Convert from 32 bit floating-point to 32 bits
                for (int32_t i = 0; i < num_samples_per_frame; i++) {
                    ((float*)frame_buffer)[i] = ((float*)file_buffer)[i]; // Input is 32 bit floating-point
                }
            }

            // Remove any blankings in frame (not needed in a typical product implementation)
            int32_t blanking_removed_in_frame = 0;
            if (num_blankings_found > 0) {
                while ((blanking_start_samples[num_blankings_removed] - num_blankings_removed * blanking_length) / num_samples_per_frame == frame_count) {
                    int32_t blanking_start, blanking_stop;
                    blanking_start = (blanking_start_samples[num_blankings_removed] - num_blankings_removed * blanking_length) % num_samples_per_frame;
                    blanking_stop = blanking_start + blanking_length;
                                        
                    // Remove any blanking values from frame_buffer
                    int32_t num_valid_samples_in_frame_buffer = blanking_start;
                    int32_t num_valid_samples_in_read = blanking_length;
                    if (blanking_stop < num_samples_per_frame) {
                        int32_t num_valid_samples_after_blanking = num_samples_per_frame - blanking_stop;
                        memmove(frame_buffer + blanking_start, frame_buffer + blanking_stop, num_valid_samples_after_blanking * sizeof(float));
                        num_valid_samples_in_frame_buffer += num_valid_samples_after_blanking;
                    } else
                        num_valid_samples_in_read = blanking_length - (blanking_stop - num_samples_per_frame);

                    // Read from file to fill up frame buffer after removing blanking
                    int32_t bytes_read = fread(file_buffer, 1, blanking_length * bytes_per_sample, infile);
                    if (bytes_read != blanking_length * bytes_per_sample)
                        break;      // End of file reached
                    
                    for (int32_t i = 0; i < num_valid_samples_in_read; i++) {
                        int32_t input_offset = blanking_length - num_valid_samples_in_read;
                        if (bytes_per_sample == sizeof(int16_t)) {
                            ((float*)frame_buffer)[num_valid_samples_in_frame_buffer + i] = ((int16_t*)file_buffer)[input_offset + i] * (1.0f / 32768.0f); // Input is 32 bit floating-point, here normalized to the interval -1 to +1
                        }
                        if (bytes_per_sample == sizeof(int32_t)) {
                            ((float*)frame_buffer)[num_valid_samples_in_frame_buffer + i] = ((int32_t*)file_buffer)[input_offset + i] * (1.0f / 2147483648.0f); // Input is 32 bit floating-point, here normalized to the interval -1 to +1
                        }
                    }
                    blanking_removed_in_frame = 1;
                    num_blankings_removed++;
                }
            }

            // Process frame
            int32_t num_scores = 0;

#ifdef PROMPT_CC
            if ((frame_count % processing_period) == 0) {
                total_accum_cc += accum_cc;
                accum_cc = 0;
                printf("CC: Voicespot[%i] = [", (int) frame_count);
            }
            uint32_t cc = rdsp_read_cycle_counter();
#endif

#ifdef LOG_CC
            uint64_t cc = rdsp_read_cycle_counter();
#endif
            // Copy frame into circular sleep buffer
            if (voicespot_buffer_struct != NULL && voicespot_buffer_struct->length_samples > num_samples_per_frame) {
                if (blanking_removed_in_frame) {
                    // Reset circular buffer
                    voicespot_buffer_struct->write_index_samples = 0;
                    voicespot_buffer_struct->read_index_samples = 0;
                }
                float* buffer_ptr = (float*)voicespot_buffer_struct->buffer;                                    // Create pointer to buffer
                float* buffer_write_ptr = &buffer_ptr[voicespot_buffer_struct->write_index_samples];            // Create pointer to frame
                // Check for wrap around
                int32_t n_avail_before_wrap = voicespot_buffer_struct->length_samples - voicespot_buffer_struct->write_index_samples;
                int32_t n_samples_copy = num_samples_per_frame;
                if (n_avail_before_wrap < num_samples_per_frame)
                    n_samples_copy = n_avail_before_wrap;

                // Copy first part of frame into buffer
                rdsp_memcpy(buffer_write_ptr, frame_buffer, n_samples_copy * sizeof(float));    

                int32_t n_samples_remain = num_samples_per_frame - n_samples_copy;

                // Copy remaining samples into buffer
                if (n_samples_remain > 0)
                    rdsp_memcpy(buffer_ptr, &frame_buffer[n_samples_copy], n_samples_remain * sizeof(float));   

                // Update write pointer and check for wrap-around
                voicespot_buffer_struct->write_index_samples += num_samples_per_frame;
                if (voicespot_buffer_struct->write_index_samples >= voicespot_buffer_struct->length_samples)    
                    voicespot_buffer_struct->write_index_samples -= voicespot_buffer_struct->length_samples;
            }

            //Set processing level - Will be ignored if adaptive power state is enabled
            int32_t processing_level;
            if ((frame_count % processing_period) < 4) {
                processing_level = RDSP_PROCESSING_LEVEL__FULL;
            } else {
                processing_level = RDSP_PROCESSING_LEVEL__SKIP_OUTPUT;
            }
            voicespot_status = rdspVoiceSpot_Process(voicespot_control, voicespot_handle, processing_level, (uint8_t*) frame_buffer, &num_scores, scores, (uint8_t**) sfb_output);

#ifdef PROMPT_CC
            cc = rdsp_read_cycle_counter() - cc; // Get number of cycles used
            accum_cc += cc;
            if (cc > peak_cc)
                peak_cc = cc;
            printf("%i ", (int) cc);
            if (((frame_count + 1) % processing_period) == 0) {
                float mcps = 80.0f * accum_cc / (processing_period * 1e6);
                printf("] = %i kCPS\n", (int) (1000 * mcps));
            }
#endif

#ifdef LOG_CC
            cc = ((uint64_t) rdsp_read_cycle_counter()) - cc; // Get number of cycles used
            fwrite(&cc, sizeof(uint64_t), 1, fid_cc_log);
#endif

            if (voicespot_status != RDSP_VOICESPOT_OK)
                printf("[%d] rdspVoiceSpot_Process: voicespot_status = %d\n", (int) frame_count, (int) voicespot_status);

            if (num_scores > 0) {
                // Store maximum over the scores
                for (int32_t score_index = 0; score_index < num_outputs; score_index++) {
                    if (scores[score_index] > max_scores[score_index]) {
                        max_scores[score_index] = scores[score_index];
                        max_frame_count[score_index] = frame_count;
                    }
                }
                
                // Check for if trigger has been found
#ifdef PROMPT_CC
                    cc = rdsp_read_cycle_counter();
#endif
                int32_t condition = (frame_count - last_event >= event_duration_frames);
                int32_t score_index = rdspVoiceSpot_CheckIfTriggered(voicespot_control, voicespot_handle, scores, condition, event_thresholds, processing_period);
                if (score_index >= 0 && frame_count_trigger == -1) {
                    frame_count_trigger = frame_count;  // Store trigger frame count for later estimation
                    rdsp_memcpy(scores_trigger, scores, num_outputs * sizeof(int32_t));     // Store trigger scores for later estimation
                    score_index_trigger = score_index;  // Store trigger score_index for later estimation
                    last_event = frame_count;
                }
#ifdef PROMPT_CC
                cc = rdsp_read_cycle_counter() - cc; // Get number of cycles used
                accum_cc += cc;
#endif

                if (frame_count_trigger >=0 && (frame_count - frame_count_trigger >= estimation_delay_frames)) {
                    // We found a trigger, so estimate the starting point now
                    int32_t start_offset_samples = 0;
                    int32_t stop_offset_samples = 0;
                    int32_t timing_accuracy = 4; // Accuracy of the timing estimate, in frames
#ifdef PROMPT_CC
                    cc = rdsp_read_cycle_counter();
#endif
                    voicespot_status = rdspVoiceSpot_EstimateStartAndStop(voicespot_control, voicespot_handle, score_index_trigger, -1, timing_accuracy, &start_offset_samples, &stop_offset_samples); // Comment out this line if timing estimation is not needed

#ifdef PROMPT_CC
                    cc = rdsp_read_cycle_counter() - cc; // Get number of cycles used
                    printf("CC: EstimateStartAndStop = %i\n", (int)cc);
#endif
                    if (voicespot_status != RDSP_VOICESPOT_OK)
                        printf("[%d] rdspVoiceSpot_EstimateStartAndStop: voicespot_status = %d\n", (int) frame_count, (int)voicespot_status);
                    int64_t trigger_sample = (frame_count_trigger + 1) * num_samples_per_frame;
                    int64_t estimation_sample = (frame_count + 1) * num_samples_per_frame;
                    int64_t start_sample = estimation_sample - start_offset_samples; // Find absolute start sample index by going back from current location
                    int64_t stop_sample = estimation_sample - stop_offset_samples; // Find absolute stop sample index by going back from current location
                    
                    if (use_trigger_key == 1) {
                        // Search key
                        int32_t margin = 40;
                        int32_t trigger_key_match = 0;
                        for (int32_t i = 0; i < trigger_keys_num_entries; i++) {
                            if (frame_count_trigger > ((int64_t)trigger_key[i] - margin) && frame_count_trigger < ((int64_t)trigger_key[i] + margin + buffer_size_num_samples)) {
                                printf("Trigger event found: Event = %d, Frame = %d, class_string = %s, Score = %d, trigger_sample = %d, start_offset_samples = %d, start_sample = %d, stop_offset_samples = %d, stop_sample = %d\n", (int)num_trigger_events, (int)frame_count_trigger, voicespot_class_string[score_index_trigger], (int)scores_trigger[score_index_trigger], (int)trigger_sample, (int)start_offset_samples, (int)start_sample, (int)stop_offset_samples, (int)stop_sample);
                                num_trigger_events++; // Increment the number of found trigger events   
                                trigger_key_match = 1;
                            }
                        }
                        if (trigger_key_match == 0)
                            printf("False positive found: Event = %d, Frame = %d, class_string = %s, Score = %d, trigger_sample = %d, start_offset_samples = %d, start_sample = %d\n", (int)num_trigger_events, (int)frame_count_trigger, voicespot_class_string[score_index_trigger], (int)scores_trigger[score_index_trigger], (int)trigger_sample, (int)start_offset_samples, (int)start_sample);
                    }
                    else {
                        printf("Trigger event found: Event = %d, Frame = %d, class_string = %s, Score = %d, trigger_sample = %d, estimation_sample = %d, start_offset_samples = %d, start_sample = %d, stop_offset_samples = %d, stop_sample = %d\n", (int)num_trigger_events, (int)frame_count_trigger, voicespot_class_string[score_index_trigger], (int)scores_trigger[score_index_trigger], (int)trigger_sample, (int)estimation_sample, (int)start_offset_samples, (int)start_sample, (int)stop_offset_samples, (int)stop_sample);
                        num_trigger_events++; // Increment the number of found trigger events
                    }
                    frame_count_trigger = -1;   // Indicate that current trigger has been handled

#ifdef HIFI4
                    if (data_type == RDSP_DATA_TYPE__FLOAT32)
                    printf("voicespot_trigger_data_float(%i,:) = [%i %i];\n", num_trigger_events, frame_count + 1, scores[score_index]);
                    else
                    printf("voicespot_trigger_data_hifi4(%i,:) = [%i %i];\n", num_trigger_events, frame_count + 1, scores[score_index]);
#elif defined(HIFI3)
                    printf("voicespot_trigger_data_hifi3(%i,:) = [%i %i];\n", num_trigger_events, frame_count + 1, scores[score_index]);
#elif defined(FUSIONF1)
                    printf("voicespot_trigger_data_fusion_f1(%i,:) = [%i %i];\n", num_trigger_events, frame_count + 1, scores[score_index]);
#elif defined(HEMILITE)
                    printf("voicespot_trigger_data_hemilite(%i,:) = [%i %i];\n", num_trigger_events, frame_count + 1, scores[score_index]);
#elif defined(HMD)
                    printf("voicespot_trigger_data_hemidelta(%i,:) = [%i %i];\n", num_trigger_events, frame_count + 1, scores[score_index]);
#endif
                }
            }
            frame_count++; // Increment the frame counter
        }
        printf("\n\nProcessed %d frames, num_blankings_found = %d. Found %d trigger events\n", (int) frame_count, (int) num_blankings_found, (int) num_trigger_events);

        printf("max_scores = [");
        for (int i = 0; i < num_outputs; i++)
            printf("%d, ", (int) max_scores[i]);
        printf("]\n");
        printf("max_frame_count = [");
        for (int i = 0; i < num_outputs; i++)
            printf("%d, ", (int) max_frame_count[i]);
        printf("]\n");

#ifdef PROMPT_CC
        int32_t total_kcps = (int32_t) (80.0f * total_accum_cc / (frame_count * 1e3));
        printf("Average kCPS: %i, Peak kCPS: %i\n", (int) total_kcps, (int) (peak_cc * 80) / 1000);
#endif
    }

    exit:

    voicespot_status = rdspVoiceSpot_ReleaseControl(voicespot_control); // Release VoiceSpot control, thereby automatically closing and releasing all instances
    if (voicespot_status != RDSP_VOICESPOT_OK)
        printf("rdspVoiceSpot_ReleaseControl: voicespot_status = %d\n", (int) voicespot_status);
    if (infile != NULL)
        fclose(infile);
    if (header != NULL)
        rdsp_free(header);
    if (sfb_output != NULL)
        rdsp_free(sfb_output);
    if (model_blob != NULL)
        rdsp_free_align(model_blob);
    if (file_buffer != NULL)
        rdsp_free(file_buffer);
    if (frame_buffer != NULL)
        rdsp_free(frame_buffer);
    if (voicespot_buffer_struct != NULL)
        rdsp_free(voicespot_buffer_struct);
    if (max_scores != NULL)
        rdsp_free(max_scores);
    if (max_frame_count != NULL)
        rdsp_free(max_frame_count);
    if (scores != NULL)
        rdsp_free(scores);
    if (scores_trigger != NULL)
        rdsp_free(scores_trigger);
    if (blanking_start_samples != NULL)
        rdsp_free(blanking_start_samples);

#ifdef LOG_CC
    if (fid_cc_log != NULL)
        fclose(fid_cc_log);
#endif

    return return_val;
}

