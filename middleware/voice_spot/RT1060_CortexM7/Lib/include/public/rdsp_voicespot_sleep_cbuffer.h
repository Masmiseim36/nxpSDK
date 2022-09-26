/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_VOICESPOT_SLEEP_CBUFFER_H
#define RDSP_VOICESPOT_SLEEP_CBUFFER_H

// External API header for RDSP VoiceSpot Power State buffer and callback status enum

#if defined(__cplusplus) && !defined(CPP_LINKAGE)
extern "C" {
#endif


typedef struct rdsp_voicespot_buffer_struct {
	uint8_t* buffer;
	int32_t read_index_samples;
	volatile int32_t write_index_samples;
	int32_t length_samples;
} rdsp_voicespot_sleep_cbuffer;

/// VoiceSpot callback enum
typedef enum {
	RDSP_IS_PROCESSING_FULL = 1, RDSP_IS_PROCESSING_VERY_LOW = 2, RDSP_REQUEST_FULL_PROCESSING = 3, RDSP_IS_PROCESSING_NORMAL = 4
} rdsp_voicespot_processing_status;


#if defined(__cplusplus) && !defined(CPP_LINKAGE)
}
#endif

#endif // RDSP_VOICESPOT_SLEEP_CBUFFER_H
