/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief event descriptor declaration.
 */

#ifndef _HAL_EVENT_DESCRIPTOR_COMMON_H_
#define _HAL_EVENT_DESCRIPTOR_COMMON_H_

typedef enum _event_type
{
    kEventType_Common  = 0x00,
    kEventType_FaceRec = 0x100,
    kEventType_Voice   = 0x200,
} event_type_t;

typedef enum _event_id
{
    kEventID_GetDisplayOutput = kEventType_Common,
    kEventID_SetDisplayOutput,
    kEventID_GetDisplayOutputSource,
    kEventID_SetDisplayOutputSource,
    kEventID_GetIRLedBrightness,
    kEventID_SetIRLedBrightness,
    kEventID_GetWhiteLedBrightness,
    kEventID_SetWhiteLedBrightness,
    kEventID_GetSpeakerVolume,
    kEventID_SetSpeakerVolume,
    kEventID_SetBLEConnection,
    kEventID_GetBLEConnection,

    kEventID_SetLogLevel,
    kEventID_GetLogLevel,

    kEventID_SetLPMTrigger,
    kEventID_GetLPMTrigger,

    kEventID_ControlIRLedBrightness,
    kEventID_ControlWhiteLedBrightness,
    kEventID_ControlIRCamExposure,
    kEventID_ControlRGBCamExposure,

    kEventID_LastCommon
} event_id_t;

typedef enum _event_status
{
    kEventStatus_Ok,
    kEventStatus_Error,
    kEventStatus_WrongParam,
} event_status_t;

typedef struct _event_base
{
    uint32_t eventId;
    int (*respond)(uint32_t event_id, void *response, event_status_t status, unsigned char isFinished);
} event_base_t;

typedef struct _ble_address
{
    char ssid[9];
} ble_address_t;

typedef struct _ir_led_event
{
    uint8_t brightness; /* Brightness % (0-100) */
} ir_led_event_t;

typedef struct _white_led_event
{
    uint8_t brightness; /* Brightness % (0-100) */
} white_led_event_t;

typedef struct _brightness_control_event
{
    bool enable;       /* enable (false-true) */
    uint8_t direction; /* direction (0-1) */
} brightness_control_event_t;

typedef struct _display_output_event
{
    uint8_t displayOutput;
    uint8_t displayOutputSource;
} display_output_event_t;

typedef struct _log_level_event
{
    uint8_t logLevel;
} log_level_event_t;

typedef struct _speaker_volume_event_t
{
    uint32_t volume; /* Volume % (0-100) */
} speaker_volume_event_t;

typedef struct _lpm_event
{
    union
    {
        uint8_t status;
        uint8_t mode;
    };
} lpm_event_t;

typedef struct _event_common
{
    event_base_t eventBase;
    union
    {
        void *data;
        log_level_event_t logLevel;
        display_output_event_t displayOutput;
        speaker_volume_event_t speakerVolume;
        ir_led_event_t irLed;
        white_led_event_t whiteLed;
        lpm_event_t lpm;
        brightness_control_event_t brightnessControl;
    };
} event_common_t;

#endif /* _HAL_EVENT_DESCRIPTOR_COMMON_H_ */
