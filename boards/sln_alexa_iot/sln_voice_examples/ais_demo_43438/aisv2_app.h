/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _AISV2_APP_H_
#define _AISV2_APP_H_

#include "aisv2.h"

#define AIS_APP_TIMER_INTERVAL_MSEC              (30000U) /* Update internal timer every 30 seconds */
#define AIS_APP_TIME_SYNC_INTERVAL_SEC           (30U)    /* Re-sync time every 30 sec */
#define AIS_APP_TIMER_SYNC_CLOCK_TIMEOUT_MSEC    (10000U) /* Timeout for receiving sync clock directive is 10 seconds */
#define AIS_APP_MQTT_DISCONNECT_TIMEOUT_MSEC     (30000U) /* Timeout for MQTT Disconnect is 30 seconds */
#define AIS_APP_MQTT_RECONNECT_TIMEOUT_MSEC      (90000U) /* Timeout for MQTT Reconnect is 90 seconds */
#define AIS_APP_MQTT_AIS_CONNECT_TIMEOUT_MSEC    (60000U) /* Timeout for AIS Connection events is 60 seconds */
#define AIS_APP_MQTT_OTA_DISCONNECT_TIMEOUT_MSEC (30000U) /* Timeout for OTA MQTT Disconnection is 30 seconds */
#define AIS_APP_MQTT_OTA_CONNECT_TIMEOUT_MSEC    (60000U) /* Timeout for OTA MQTT Connection is 60 seconds */
#define AIS_APP_TIMER_INTERVAL_SEC               (30U)
#define AIS_APP_THINK_STATE_TIMEOUT_MSEC         (10000U)
#define AIS_APP_MIC_PUBLISHING_HEAP_THRESHOLD    (81920U) /* Stop Mic Publishing when heap below this threshold */

/** @brief These are the app notifications types
 *
 *  They are used to signal to the appTask when various events happen, so it can handle them accordingly
 */
typedef enum _app_notifications
{
    kExceptionEncountered = (1U << 28U), /**< This notification is sent in case of a streamer error*/
    kVolChanged           = (1U << 29U), /**< This notification is sent in case of a volume change */
    kAlertVolChanged      = (1U << 30U), /**< This notification is sent in case of an alert volume change*/
    kSyncClock            = (1U << 31U), /**< This notification is sent as a response to the synchronize clock*/
} ais_app_notifications_t;

/** @brief This types are used by vProcessVolume
 *
 *  They are used to determine if it increses or decreases the volume
 */
typedef enum _app_volume_direction
{
    kVolumeUp   = (1),
    kVolumeDown = (-1)
} ais_app_volume_direction_t;

/** @brief This is the main structure of the app
 *
 */
typedef struct
{
    ais_state_t state;     /**< The curent state of the app */
    ais_state_t prevState; /**< The previous state of the app */

    uint64_t speakerOffsetStart;       /**< The start offset of the speaker */
    uint64_t speakerOffsetEnd;         /**< The end offset of the speaker */
    uint64_t speakerOffsetWritten;     /**< The offset of the last thing written to the streamer */
    uint32_t speakerOpusFramesFlushed; /**< Remember how much was flushed at stop to use at the next interaction */
    echoMarker_t
        speakerMarker;    /**< The speaker marker sent by the service used to check the progress of the playback */
    bool speakerOpen;     /**< Tells if the speaker is open */
    bool speakerOpenSent; /**< Tells if we notify the service that the speaker is open so we can avoid sending multiple
                             SpeakerOpened messages during a playback */
    bool bargein;         /**< Tells if we are in a barge in situation */

    ais_buffer_state_t
        prevSpeakerBufferState; /**< Used for better transition handling, saves the previos speaker buffer state */
    ais_buffer_state_t speakerBufferState; /**< Holds the current speaker buffer state */
    uint32_t overrunSequence;              /**< Holds record of last overrun sequence until received again */

    uint64_t prevTime;    /**< Saves the previous time sent by a sync clock */
    uint64_t currTime;    /**< Saves the current time sent by a sync clock */
    uint64_t lastRefresh; /**< Saves the time of the last refresh */

    bool syncClockReceived; /**< Tells if we receive a sync clock */

    TickType_t previousTickTime; /**< Saves the previous time that microphone data was sent to the service */
    int32_t volume;              /**< Saves the current volume of the device */
    uint32_t alertVolume;        /**< Saves the current alert volume */
} ais_app_data_t;

/** @brief Grabs the ais_app_data structure
 *
 *  @return ais_app_data_t structure or NULL
 */
ais_app_data_t *AIS_APP_GetAppData(void);

#endif
