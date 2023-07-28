/**
 *  \file appl_aics.h
 *
 *  \brief This file defines the Audio Input Control Service:
 *  Application Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_AICS_
#define _H_APPL_AICS_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Macros */
#define APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN       64U

/**
 * AICS: Input State Characteristic Fields.
 */
typedef struct _APPL_AICS_CHAR_INPUT_STATE_PARAMS
{
    /**
     * Gain Setting
     */
    CHAR gain_setting;

    /**
     * Mute
     */
    UCHAR mute;

    /**
     * Gain Mode
     */
    UCHAR gain_mode;

    /**
     * Change Counter
     * Range: 0 - 255
     */
    UCHAR change_counter;
}APPL_AICS_CHAR_INPUT_STATE_PARAMS;

/**
 * AICS: Gain Setting Characteristic Fields.
 */
typedef struct _APPL_AICS_CHAR_GAIN_SETTING_PARAMS
{
    /**
     * Gain Setting Units
     */
    UCHAR gain_setting_units;

    /**
     * Gain Setting Minimum
     */
    CHAR gain_settting_min;

    /**
     * Gain Setting Maximum
     */
    CHAR gain_setting_max;
}APPL_AICS_CHAR_GAIN_SETTING_PARAMS;

/**
 * Audio Input Control Service Characteristics Info.
 */
typedef struct _APPL_AICS_CHAR_INFO
{
    /**
     * Input State
     */
    APPL_AICS_CHAR_INPUT_STATE_PARAMS * input_state;

    /**
     * Gain Setting Properties
     */
    APPL_AICS_CHAR_GAIN_SETTING_PARAMS * gain_setting;

    /**
     * Input Type
     * TODO: this Assign number relook the type.
     */
    UCHAR input_type;

    /**
     * Input Status
     */
    UCHAR input_status;

    /**
     * Audio Input Description
     * TODO: Make as Pointer?
     */
    UCHAR audio_in_desc[APPL_AICS_MAX_CHAR_AUDIO_INPUT_DESC_LEN];

    /**
     * Audio Input Description Leng
     */
    UCHAR audio_in_desc_len;
}APPL_AICS_CHAR_INFO;

/* --------------------------------------------- APIs */

/* Parser for printing AICS Gain Mode - common for MICP and VCP */
void appl_aics_parse_and_display_gain_mode
     (
         UINT8 gain_mode
     );

/* Parser for printing AICS Audio Input Type - common for MICP and VCP */
void appl_aics_parse_and_display_audio_input_type
     (
         UINT8 audio_input_type
     );

/* Parser for printing AICS Audio Input Status - common for MICP and VCP */
void appl_aics_parse_and_display_audio_input_status
     (
         UINT8 audio_input_status
     );

/* Parser for printing AICS Characteristics - common for MICP and VCP */
void appl_aics_parse_and_display_char_value
     (
         UINT16   char_uuid,
         UCHAR  * aics_data,
         UINT16   datalen
     );

/* Parser for printing AICS application error codes */
void appl_aics_display_appl_err_codes(GA_RESULT status);

#endif /* _H_APPL_AICS_ */