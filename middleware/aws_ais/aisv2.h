/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _AISV2_H_
#define _AISV2_H_

#include "fsl_common.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"

#include "iot_mqtt_agent.h"

#include "cJSON.h"

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AIS_SPEC_REV_325 1

/*! @brief AIS MQTT topic tempate. */
/*  Format:
 *  /<partnerRoot>/<clientId>/ais/<topicVersion>/<topic>
 *  NOTE: topic version hardcoded to 'v1' for this release.
 */
#define AIS_TOPIC_TEMPLATE "%s/ais/v1/%s/%s"
#define AIS_TOPIC_MAX_LENGTH 128

#define AIS_SECRET_LENGTH 16
#define AIS_REGISTRATION_INFO_FILE_NAME "registrationInfo.dat"
#define AIS_CRYPT_IV_LENGTH 12
#define AIS_CRYPT_MAC_LENGTH 16

#define AIS_MIC_PACKET_SIZE 4800
#define AIS_MQTT_MAX_RX_SIZE (4 * 1024)

#define AIS_SEQUENCE_BUFFER_SLOTS 10
#define AIS_SEQUENCE_BUFFER_DIR_SIZE (1 * 1024)
#define AIS_SEQUENCE_BUFFER_SPEAKER_SIZE AIS_MQTT_MAX_RX_SIZE

#define AIS_MSG_RETRY_MAX 3
#define AIS_MSG_RETRY_TIMEOUT (60000 / portTICK_PERIOD_MS)
#define AIS_MSG_SEQ_TIMEOUT_MSEC (10000)
#define AIS_MIC_OPEN_TIMEOUT_MSEC (25000)
#define AIS_MIC_PUBLISH_ERR_MAX (20)
#define AIS_MSG_ACCEPT (1 << 0)
#define AIS_MSG_REJECT (1 << 1)

#define AIS_EVENT_SECRET_ROTATED "SecretRotated"
#define AIS_EVENT_BUTTON_CMD_ISSUED "ButtonCommandIssued"
#define AIS_EVENT_SPEAKER_OPEN "SpeakerOpened"
#define AIS_EVENT_SPEAKER_CLOSED "SpeakerClosed"
#define AIS_EVENT_SPEAKER_MARKER_ENCOUNTERED "SpeakerMarkerEncountered"
#define AIS_EVENT_MICROPHONE_OPENED "MicrophoneOpened"
#define AIS_EVENT_MICROPHONE_CLOSED "MicrophoneClosed"
#define AIS_EVENT_OPEN_MICROPHONE_TIMED_OUT "OpenMicrophoneTimedOut"
#define AIS_EVENT_BUFFER_STATE_CHANGED "BufferStateChanged"
#define AIS_EVENT_VOLUME_CHANGED "VolumeChanged"
#define AIS_EVENT_SYNCHRONIZE_CLOCK "SynchronizeClock"
#define AIS_EVENT_SET_ALERT_SUCCEEDED "SetAlertSucceeded"
#define AIS_EVENT_SET_ALERT_FAILED "SetAlertFailed"
#define AIS_EVENT_DELETE_ALERT_SUCCEEDED "DeleteAlertSucceeded"
#define AIS_EVENT_DELETE_ALERT_FAILED "DeleteAlertFailed"
#define AIS_EVENT_ALERT_VOLUME_CHANGED "AlertVolumeChanged"
#define AIS_EVENT_SYNCHRONIZE_STATE "SynchronizeState"
#define AIS_EVENT_EXCEPTION_ENCOUNTERED "ExceptionEncountered"

/*! @brief Automatic speech recognition (ASR) profile */
#define AIS_EVENT_PROFILE_CLOSE_TALK "CLOSE_TALK"
#define AIS_EVENT_PROFILE_NEAR_FIELD "NEAR_FIELD"
#define AIS_EVENT_PROFILE_FAR_FIELD "FAR_FIELD"

/*! @brief  Recognize event payload initiator types */
#define AIS_EVENT_INITIATOR_TYPE_PH "PRESS_AND_HOLD"
#define AIS_EVENT_INITIATOR_TYPE_TAP "TAP"
#define AIS_EVENT_INITIATOR_WAKEWORD "WAKEWORD"

/*! @brief SetState event UX Attention System states */
#define AIS_EVENT_STATE_IDLE "IDLE"
#define AIS_EVENT_STATE_THINKING "THINKING"
#define AIS_EVENT_STATE_SPEAKING "SPEAKING"
#define AIS_EVENT_STATE_ALERTING "ALERTING"
#define AIS_EVENT_STATE_DO_NOT_DISTURB "DO_NOT_DISTURB"
#define AIS_EVENT_STATE_NOTIFICATION "NOTIFICATION_AVAILABLE"

/*! @brief Alert type value strings */
#define AIS_EVENT_ALERT_TIMER "TIMER"
#define AIS_EVENT_ALERT_ALARM "ALARM"
#define AIS_EVENT_ALERT_REMINDER "REMINDER"

/*! @brief Button command value strings */
#define AIS_EVENT_BUTTON_PLAY "PLAY"
#define AIS_EVENT_BUTTON_NEXT "NEXT"
#define AIS_EVENT_BUTTON_PREVIOUS "PREVIOUS"
#define AIS_EVENT_BUTTON_STOP "STOP"
#define AIS_EVENT_BUTTON_PAUSE "PAUSE"

/*! @brief Buffer state value strings */
#define AIS_EVENT_BUFFER_GOOD "GOOD"
#define AIS_EVENT_BUFFER_OVERRUN "OVERRUN"
#define AIS_EVENT_BUFFER_OVERRUN_WARNING "OVERRUN_WARNING"
#define AIS_EVENT_BUFFER_UNDERRUN "UNDERRUN"
#define AIS_EVENT_BUFFER_UNDERRUN_WARNING "UNDERRUN_WARNING"

#define AIS_CAP_SPEAKER_BITRATE_CONSTANT "CONSTANT"
#define AIS_CAP_SPEAKER_BITRATE_VARIABLE "VARIABLE"

#define AIS_CAP_SPEAKER_DECODER_OPUS "OPUS"
#define AIS_CAP_SPEAKER_DECODER_MP3 "MP3"

#define AIS_ENABLE_LOCAL_VOLUME_CONTROL (1U)

/*! @brief Bitmask for internal task states.
 * Some states (microphone and speaker) can be used simultaneously. */
#define AIS_TASK_STATE_IDLE 0x00
#define AIS_TASK_STATE_MICROPHONE 0x01
#define AIS_TASK_STATE_SPEAKER 0x02
#define AIS_TASK_STATE_ERROR 0x04

#define AIS_EXCEPTION_DESCRIPTION_STREAMER_MSG "Streamer encountered a decode/memory/trampler error"

typedef enum
{
    AIS_CRYPT_ENCRYPT,
    AIS_CRYPT_DECRYPT
} aisCryptMode_t;

typedef enum
{
    AIS_TOPIC_CONNECTION_FROMCLIENT = 0,
    AIS_TOPIC_CONNECTION_FROMSERVICE,
    AIS_TOPIC_CAPABILITIES_PUBLISH,
    AIS_TOPIC_CAPABILITIES_ACKNOWLEDGE,
    AIS_TOPIC_DIRECTIVE,
    AIS_TOPIC_EVENT,
    AIS_TOPIC_MICROPHONE,
    AIS_TOPIC_SPEAKER,
    AIS_TOPIC_LAST
} aisTopic_t;

typedef enum
{
    AIS_DISCONNECT_NONE = 0,
    AIS_DISCONNECT_INVALID_SEQUENCE,
#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
    AIS_DISCONNECT_MESSAGE_TAMPERED,
#endif
    AIS_DISCONNECT_API_DEPRECATED,
    AIS_DISCONNECT_ENCRYPTION_ERROR,
    AIS_DISCONNECT_GOING_OFFLINE,
} ais_disconnect_code_t;

typedef enum
{
    AIS_SPEAKER_BITRATE_CONSTANT,
    AIS_SPEAKER_BITRATE_VARIABLE,
    AIS_SPEAKER_BITRATE_INVALID
} ais_speaker_bitrate_t;

typedef enum
{
    AIS_SPEAKER_DECODER_OPUS,
    AIS_SPEAKER_DECODER_MP3,
    AIS_SPEAKER_DECODER_INVALID
} ais_speaker_decoder_t;

/*! @brief Values for Recognize directive ASR Profile */
typedef enum
{
    AIS_ASR_CLOSE_TALK,
    AIS_ASR_NEAR_FIELD,
    AIS_ASR_FAR_FIELD,
    AIS_ASR_INVALID
} ais_asr_profile_t;

/*! @brief Values for Recognize directive Initiator */
typedef enum
{
    AIS_INITIATOR_PRESS_AND_HOLD,
    AIS_INITIATOR_TAP,
    AIS_INITIATOR_WAKEWORD,
    AIS_INITIATOR_INVALID
} ais_initiator_t;

/*! @brief Values for alert types */
typedef enum
{
    AIS_ALERT_TYPE_TIMER,
    AIS_ALERT_TYPE_ALARM,
    AIS_ALERT_TYPE_REMINDER,
    AIS_ALERT_TYPE_INVALID
} ais_alert_type_t;

/*! @brief Button commands */
typedef enum
{
    AIS_BUTTON_CMD_PLAY,
    AIS_BUTTON_CMD_NEXT,
    AIS_BUTTON_CMD_PREVIOUS,
    AIS_BUTTON_CMD_STOP,
    AIS_BUTTON_CMD_PAUSE,
    AIS_BUTTON_CMD_INVALID
} ais_button_cmd_t;

/*! @brief Values for flow control buffer states. */
typedef enum
{
    AIS_BUFFER_STATE_GOOD,
    AIS_BUFFER_STATE_OVERRUN,
    AIS_BUFFER_STATE_OVERRUN_WARNING,
    AIS_BUFFER_STATE_UNDERRUN,
    AIS_BUFFER_STATE_UNDERRUN_WARNING,
    AIS_BUFFER_STATE_INVALID
} ais_buffer_state_t;

/*! @brief Values for SetState directive.
 * These map to the AVS UX Attention State system */
typedef enum
{
    AIS_STATE_IDLE,
    AIS_STATE_THINKING,
    AIS_STATE_SPEAKING,
    AIS_STATE_ALERTING,
    AIS_STATE_DO_NOT_DISTURB,
    AIS_STATE_NOTIFICATION,
    AIS_STATE_INVALID
} ais_state_t;

/*! @brief Internal AIS status */
typedef enum _ais_status_t
{
    eAIS_Success,
    eAIS_OutOfMemory,
    eAIS_MicrophonePublishFailed,
    eAIS_EventPublishFailed,
    eAIS_SubscribeFailed,
} ais_status_t;

/*! @brief Store root and payload JSON objects for MQTT messages. */
typedef struct
{
    cJSON *json;
    cJSON *payload;
} ais_json_t;

/*! @brief Structure to map an MQTT topic enum to string value. */
typedef struct
{
    aisTopic_t topic;
    const char *string;
} aisTopicMap_t;

typedef struct
{
    bool echoRequest;
    uint32_t marker;
    uint64_t offset;
} echoMarker_t;

typedef struct
{
    ais_asr_profile_t asr_profile;
    ais_initiator_t initiator;
    char *init_type;
    char *init_token;
    uint64_t wwStart;
    uint64_t wwEnd;
} ais_mic_open_t;

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
typedef struct __attribute__((packed))
{
    uint32_t sequence;
    uint8_t iv[AIS_CRYPT_IV_LENGTH];
    uint8_t mac[AIS_CRYPT_MAC_LENGTH];
    /* AES-GCM encrypted message data follows. */
} plainTextHeader_t;

typedef struct __attribute__((packed))
{
    uint32_t sequence;
    uint8_t iv[AIS_CRYPT_IV_LENGTH];
    uint8_t mac[AIS_CRYPT_MAC_LENGTH];
    uint32_t encryptedSequence; /*! Part of AES-GCM encrypted message */
} commonHeader_t;
#else
typedef struct __attribute__((packed))
{
    uint32_t sequence;
    uint8_t iv[AIS_CRYPT_IV_LENGTH];
    uint8_t mac[AIS_CRYPT_MAC_LENGTH];
    /* AES-GCM encrypted message data follows. */
} commonHeader_t, plainTextHeader_t;
#endif

/*! @brief Header for binary stream MQTT message type */
typedef struct __attribute__((packed))
{
    uint32_t length;
    uint8_t type;
    uint8_t count; /* The number of data chunks in each stream message */
    uint8_t reserved[2];
    /* Binary stream data follows. */
} binaryHeader_t;

/*! @brief Binary audio stream packet for type == 1 (marker) */
typedef struct __attribute__((packed))
{
    binaryHeader_t binaryHeader;
    uint32_t marker;
} binaryMarker_t;

typedef struct __attribute__((packed))
{
    uint64_t offset;
    uint8_t *data;
} binaryAudioDataStream_t;

/*! @brief Binary audio stream packet for type == 0 (audio data) */
typedef struct __attribute__((packed))
{
    binaryHeader_t binaryHeader;
    binaryAudioDataStream_t audioData;
} binaryAudioStream_t;

/*! @brief Binary stream MQTT message type, with audio data. */
typedef struct __attribute__((packed))
{
    commonHeader_t commonHeader;
    binaryAudioStream_t audio;
} binaryStream_t;

/*! @brief Registration information that needs to be stored */
typedef struct
{
    uint8_t sharedSecret[AIS_SECRET_LENGTH];
    char awsPartnerRoot[128];
} ais_reg_config;

typedef struct _ais_config_t
{
    /* Speaker capabilities */
    uint8_t speakerChannels;
    uint32_t speakerBitrate;
    ais_speaker_bitrate_t speakerBitrateType;
    ais_speaker_decoder_t speakerDecoder;

    /* Alerts capabilities */
    uint32_t maxAlertCount;

    /* System capabilities */
    char firmwareVersion[16];
    char locale[12];

    /* Created a sub structure for flash management */
    ais_reg_config registrationConfig;
    ais_reg_config tempRegistrationConfig;

    /* AWS IoT configuration */
    char awsClientId[64];
    char awsAccountId[64];
    char awsEndpoint[128];

    /* AWS Auth configuration */
    char awsAuthClientId[128];
    char awsAuthToken[1024];
} ais_config_t;

typedef struct
{
    uint8_t data[AIS_SEQUENCE_BUFFER_DIR_SIZE];
    bool isFree;
} ais_seq_directive_slot_t;

typedef struct
{
    uint8_t data[AIS_SEQUENCE_BUFFER_SPEAKER_SIZE];
    bool isFree;
} ais_seq_speaker_slot_t;

typedef struct
{
    void *data;
    uint32_t size;
    uint32_t seq;
} ais_seq_buf_t;

/*! @brief AIS interface structure */
typedef struct _ais_handle_t
{
    TaskHandle_t mqttTask;
    uint8_t state;

    /* Global mutex for general AIS state consistency */
    SemaphoreHandle_t aisStateLock;

    binaryStream_t micStream;

    void *audioPlayer;

    /* Track message sequence numbers for each individual topic. */
    uint32_t topicSequence[AIS_TOPIC_LAST];

    /* Sequence to rotate on based on rotateSecret message from Alexa. */
    uint32_t rotateTopicSequence[AIS_TOPIC_LAST];

    /* Pointer to the secret the topic should use. */
    uint8_t *topicSecret[AIS_TOPIC_LAST];

    /* An array of topics indicating whether there is an impending secret rotate. */
    bool rotateSecret[AIS_TOPIC_LAST];

    /* Re-sequencing buffers for directive topic. */
    ais_seq_buf_t seqListDirective[AIS_SEQUENCE_BUFFER_SLOTS];
    SemaphoreHandle_t seqMutexDirective;

    /* Re-sequencing buffers for speaker topic. */
    ais_seq_buf_t seqListSpeaker[AIS_SEQUENCE_BUFFER_SLOTS];
    SemaphoreHandle_t seqMutexSpeaker;

    TickType_t seqTimerDirective;
    TickType_t seqTimerSpeaker;

    bool aisConnected;
    bool firstConnect;
    bool pendDisconnect;
    bool pendDisconnectSent;
    ais_disconnect_code_t pendDisconnectCode;

    /* Data buffers for sending mic and JSON data.
     * Will be allocated dynamically in AIS_Init(). */
    char *msgMicBuffer;
    char *msgJsonBuffer;
    char *msgCryptBuffer;

    ais_config_t *config;
    EventGroupHandle_t aisEvent;

    /* Sequence numbers to target for rotation on receive topics. */
    bool rotateSecretReq;

    /* Random number entropy context.
     * Used for UUID creation. */
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
} ais_handle_t;

typedef bool (*ais_process_func_t)(ais_handle_t *handle, const char *data, uint32_t length);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * AIS Client Library Functions
 ******************************************************************************/

/*!
 * @brief Initialize the AIS service interface
 *
 * This function initializes the AIS interface.  It creates an internal AIS task
 * for executing the state machine, sends the device capabilities to AIS, and
 * subscribes to topics.
 *
 * This function requires a valid and active connection to AIS MQTT service.
 * The handle to this is passed in in the 'mqttHandle' parameter.
 *
 * @param handle Pointer to AIS interface handle that will be initialized
 * @param audioPlayer Pointer to opaque type for audio playback.  This handle
 *        will be passed to speaker application callback functions and can
 *        optionally be used to faciliate playback.
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_Init(ais_handle_t *handle, void *audioPlayer);

/*!
 * @brief Deinit the AIS service interface
 *
 * This function De-initializes the AIS service and destorys the background
 * task.  It also unsubscribes from the AIS service MQTT topics.
 * This function should only be called while the client library is disconnected
 * from the service, to ensure a smooth shutdown.
 *
 * NOTE that this function does not disconnect or destroy the MQTT handle.
 *
 * @param handle Pointer to AIS interface handle
 */
void AIS_Deinit(ais_handle_t *handle);

/*!
 * @brief Connect to the AIS service
 *
 * This function subscribes to topics, connects to the service, and sends the
 * capabilities (only on the first connection).
 *
 * @param handle Pointer to AIS interface handle
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_Connect(ais_handle_t *handle);

/*!
 * @brief Disconnect from the AIS service
 *
 * This function unsubscribes to all topics and disconnects from the AIS
 * service.
 *
 * @param handle Pointer to AIS interface handle
 * @param code Disconnect code string to send to service
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_Disconnect(ais_handle_t *handle, ais_disconnect_code_t code);

/*!
 * @brief State update post disconnect from the AIS service
 *
 * This function resets AIS specific internal variables.
 *
 * @param handle Pointer to AIS interface handle
 */
void AIS_Disconnected(ais_handle_t *handle);

/*!
 * @brief Register with AIS service
 *
 * This function initiates an HTTPS request to the AIS registration endpoint to
 * register the current device and client ID.  The registration process performs
 * an ECDH key exchange to generate a shared secret for end-to-end encryption
 * that should be stored by the application in non-volatile memory.
 *
 * @param handle Pointer to AIS interface handle
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_Register(ais_handle_t *handle);

/*! @brief Internal state execution for receiving speaker data
 *
 * @param handle Pointer to AIS interface handle
 * */
void AIS_StateSpeaker(ais_handle_t *handle);

/*! @brief Terminate streamer playback and receiving speaker data
 *
 * @param handle Pointer to AIS interface handle
 * */
void AIS_TerminateSpeaker(ais_handle_t *handle);

/*!
 * @brief Set internal AIS state machine flag
 *
 * This function sets an internal state flag to indicate to the AIS background
 * task state machine that new processing may be required.
 *
 * This is mostly used internal to AIS, but it can be used by an application
 * directly if needed.
 *
 * @param handle Pointer to AIS interface handle
 * @param mask State bit mask value to set
 */
void AIS_SetState(ais_handle_t *handle, uint8_t mask);

/*!
 * @brief Clear internal AIS state machine flag
 *
 * This function sets an internal state flag to indicate to the AIS background
 * task state machine that new processing may be required.
 *
 * This is mostly used internal to AIS, but it can be used by an application
 * directly if needed.
 *
 * @param handle Pointer to AIS interface handle
 * @param mask State bit mask value to set
 */
void AIS_ClearState(ais_handle_t *handle, uint8_t mask);

/*!
 * @brief Check internal AIS state machine flag
 *
 * This function verifies an internal state flag in the AIS background
 * task state machine.
 *
 * This is mostly used internal to AIS, but it can be used by an application
 * directly if needed.
 *
 * @param handle Pointer to AIS interface handle
 * @param state State value to check if set
 */
bool AIS_CheckState(ais_handle_t *handle, uint8_t state);

/*!
 * @brief Lock AIS state variables and appData parameters
 *
 * This function locks on a mutex to prevent concurrent access to appData
 * and other AIS state variables. These are relevant for various streamer
 * related operations like open speaker and barge-in.
 *
 * @param handle Pointer to AIS interface handle
 */
void AIS_State_Lock(ais_handle_t *handle);

/*!
 * @brief Unlock access to the AIS state
 *
 * This function unlocks the AIS state mutex
 *
 * @param handle Pointer to AIS interface handle
 */
void AIS_State_Unlock(ais_handle_t *handle);

/*!
 * @brief Apply AIS configuration to client library
 *
 * This function sets the client library internal configuration to the values in
 * parameter 'config'.
 *
 * NOTE The configuration structure is not copied and only referenced internally
 * by pointer.  It must remain in-scope for the duration of the client library
 * usage.
 *
 * @param handle Pointer to AIS interface handle
 * @param config Pointer to AIS configuration structure to use
 */
void AIS_SetConfig(ais_handle_t *handle, ais_config_t *config);

/*!
 * @brief Apply AIS shared secret to the client library
 *
 * This function sets the 16-byte AES-128 key into the client library for
 * end-to-end encryption.  This should be called before AIS_Connect(), using the
 * shared secret from the registration process that is stored in non-volatile
 * memory.
 *
 * @param handle Pointer to AIS interface handle
 * @param secret Pointer to secret (16-bytes in size)
 */
void AIS_SetSecret(ais_handle_t *handle, uint8_t *secret);

/*!
 * @brief Deal with resequencing for out of order MQTT messages
 *
 * Check the resequencing queues for the speaker/directive topics
 * and try to match if we find the expected sequence number.
 * Also, check for the 10 seconds timer expiration where necessary
 * and update timer for each topic accordingly.
 *
 * @param handle Pointer to AIS interface handle
 * @param topic AIS topic id for the sequence received
 * @param processFunc Pointer for the topic processing function
 */
void AIS_MessageResequence(ais_handle_t *handle, aisTopic_t topic, ais_process_func_t processFunc);

/*******************************************************************************
 * AIS Event Functions
 ******************************************************************************/

/*!
 * @brief Inform AIS the shared secret has been rotated
 *
 * @param handle Pointer to AIS interface handle
 * @param eventSequenceNumber Sequence number of the first event that will be
 *                            encrypted with the new secret.
 * @param micSequenceNumber Sequence number of the first mic message that will
 *                          be encrypted with the new secret.
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventSecretRotated(ais_handle_t *handle, uint32_t eventSequenceNumber, uint32_t micSequenceNumber);

/*!
 * @brief Inform AIS of user-initiated button action
 *
 * @param handle Pointer to AIS interface handle
 * @param cmd Button command pressed
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventButtonCommandIssued(ais_handle_t *handle, ais_button_cmd_t cmd);

/*!
 * @brief Inform AIS that device has stopped streaming speaker data
 *
 * @param handle Pointer to AIS interface handle
 * @param offset Final speaker offset that has been played back
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventSpeakerClosed(ais_handle_t *handle, uint64_t offset);

/*!
 * @brief Inform AIS that device has started streaming data
 *
 * @param handle Pointer to AIS interface handle
 * @param offset Starting streaming offset
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventSpeakerOpened(ais_handle_t *handle, uint64_t offset);

/*!
 * @brief Inform AIS that device has played speaker data to marker level
 *
 * This function is sent to AIS when the audio player has reached the offset
 * immediately preceding the marker value.
 *
 * @param handle Pointer to AIS interface handle
 * @param marker Marker value from speaker topic
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventSpeakerMarkerEncountered(ais_handle_t *handle, uint32_t marker);

/*!
 * @brief Initiate a question to AIS
 *
 * This function starts the microphone transfer process to ask a question to
 * AIS.  It should be called by an app when a wakeword triggers or a push to
 * talk is started.  It sends the Recognize directive to AIS, and begins calling
 * the application microphone callback to fill mic data
 * (AIS_AppCallback_Microphone).
 *
 * @param handle Pointer to AIS interface handle
 * @param mic Pointer to AIS microphone opened configuration structure
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventMicrophoneOpened(ais_handle_t *handle, ais_mic_open_t *mic);

/*!
 * @brief Stop sending question data to AIS
 *
 * This function ends the microphone sending to AIS and sends a MicrophoneClosed
 * directive to the AIS service.  This is normally used only in a push-to-talk
 * application, when the user releases a button and the end of speech is known.
 *
 * @param handle Pointer to AIS interface handle
 * @param offset Byte offset into audio stream that microphone was closed
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventMicrophoneClosed(ais_handle_t *handle, uint64_t offset);

/*!
 * @brief Inform AIS that device could not open microphone in time
 *
 * When receiving an OpenMicrophone directive, the device has a limited time to
 * open the microphone and begin processing data.  If this doesn't occur before
 * the timeout, this even must be sent.
 *
 * @param handle Pointer to AIS interface handle
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventOpenMicrophoneTimedOut(ais_handle_t *handle);

/*!
 * @brief Update AIS service on audio speaker flow control status
 *
 * @param handle Pointer to AIS interface handle
 * @param old_state Previous control state
 * @param new_state Flow control state that has been triggered
 * @param sequence Sequence number of audio speaker data that triggered new
 *                 state
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventBufferStateChanged(ais_handle_t *handle,
                                     ais_buffer_state_t old_state,
                                     ais_buffer_state_t new_state,
                                     uint32_t sequence);

/*!
 * @brief Inform AIS of volume change due to button/GUI action
 *
 * @param handle Pointer to AIS interface handle
 * @param volume Volume request by application, 1-100 range
 * @param offset offset the volume change occured. 0 value doesn't send offset
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventVolumeChanged(ais_handle_t *handle, uint32_t volume, uint64_t offset);

/*!
 * @brief Request current time from AIS
 *
 * Response will come with SetClock directive
 *
 * @param handle Pointer to AIS interface handle
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventSynchronizeClock(ais_handle_t *handle);

/*!
 * @brief Inform AIS alert was successfully saved to local memory
 *
 * Response from SetAlert directive
 *
 * @param handle Pointer to AIS interface handle
 * @param token Token string identifying alert
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventSetAlertSucceeded(ais_handle_t *handle, const char *token);

/*!
 * @brief Inform AIS alert was not successfully saved to local memory
 *
 * Response from SetAlert directive
 *
 * @param handle Pointer to AIS interface handle
 * @param token Token string identifying alert
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventSetAlertFailed(ais_handle_t *handle, const char *token);

/*!
 * @brief Inform AIS alert was successfully deleted from local memory
 *
 * Response from DeleteAlert directive
 *
 * @param handle Pointer to AIS interface handle
 * @param token Token string identifying alert
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventDeleteAlertSucceeded(ais_handle_t *handle, const char *token);

/*!
 * @brief Inform AIS alert was not successfully deleted from local memory
 *
 * Response from DeleteAlert directive
 *
 * @param handle Pointer to AIS interface handle
 * @param token Token string identifying alert
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventDeleteAlertFailed(ais_handle_t *handle, const char *token);

/*!
 * @brief Inform AIS service that volume for alert was successfuly saved
 *
 * Response from SetAlertVolume directive
 *
 * @param handle Pointer to AIS interface handle
 * @param volume Volume to use for alerts
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventAlertVolumeChanged(ais_handle_t *handle, uint32_t volume);

/*!
 * @brief Communicate alert status to service on new connection
 *
 * This event should be sent by the application immediately after connecting to
 * the service.
 *
 * @param handle Pointer to AIS interface handle
 * @param volume Volume being used for alerts
 * @param alertTokens List of string alert tokens for alerts currently saved in
 *                    local memory
 * @param alertCount Number of alert token strings contained in 'alertTokens'
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventSynchronizeState(ais_handle_t *handle,
                                   int32_t volume,
                                   const char *alertTokens[],
                                   uint32_t alertCount);

/*!
 * @brief Indicate to service that exception has occurred in the client
 *
 * @param handle Pointer to AIS interface handle
 * @param description String to send in the description
 * @return kStatus_Success on success, other on failure
 */
status_t AIS_EventExceptionEncountered(ais_handle_t *handle, const char *description);

/*******************************************************************************
 * Application Callback Functions
 ******************************************************************************/

/*!
 * @brief App Callback - Initialization
 *
 * This function is called during AIS_Connect() to initialize any
 * application-specific variables or data.
 */
void AIS_AppInit(void);

/*!
 * @brief App Callback - StopCapture
 *
 * This function is called when AIS receives a StopCapture directive for
 * executing application-specific behavior.
 */
void AIS_AppCallback_StopCapture(void);

/*!
 * @brief App Callback - /microphone
 *
 * This function is called when the state machine is sending microphone data to
 * the AIS service on the microphone topic.
 *
 * The application should set the *data parameter to point to an
 * application-controlled data buffer with microphone data.  This function can
 * block while the app collects this data.  The size of the microphone data
 * buffer should be limited to the maximum size supported by the network stack.
 *
 * @param data Pointer to a pointer to a data buffer to be set by the app
 * @param size Pointer to number of bytes in the data buffer
 */
uint32_t AIS_AppCallback_Microphone(uint8_t **data, uint32_t *size);

/*!
 * @brief App Callback - /speaker update state
 *
 * This function is called when the state machine is receiving data from AIS on
 * the /speaker topic.  It can optionally check for end of playback and update
 * the state machine to clear the SPEAKER state, or any other app-specific
 * behavior when playback completes.
 *
 * @param handle Pointer to AIS interface handle
 */
void AIS_AppCallback_SpeakerState(ais_handle_t *handle);

/*!
 * @brief App Callback - /speaker force terminate (e.g. on disconnect)
 *
 * This function is called when speaker playing needs to END abruptly and further
 * received data from AIS on the /speaker topic needs to be ignored.
 * It STOPS the STREAMER and clears the SPEAKING state in AIS state machine.
 *
 * @param handle Pointer to AIS interface handle
 */
void AIS_AppCallback_TerminateSpeaker(ais_handle_t *handle);

/*!
 * @brief App Callback - available data in audio buffer
 *
 * This function is called by the AIS client library when processing new
 * /speaker messages.  It should return how many bytes of storage are avialable
 * in the internal audio buffer.  If this value is smaller than the amount
 * needed for speaker data processing, the message processing is delayed.
 *
 * @param handle Pointer to AIS interface handle
 * @return Number of bytes available for writing to the audio buffer
 */
uint32_t AIS_AppCallback_SpeakerAvailableBuffer(ais_handle_t *handle);

/*!
 * @brief App Callback - audio buffer overflow detected
 *
 * This function is called by the AIS client library when processing new
 * /speaker messages if an overflow condition is detected where not enough audio
 * buffer space is available
 *
 * @param handle Pointer to AIS interface handle
 * @param sequence Sequence number of speaker message that caused overflow
 */
void AIS_AppCallback_SpeakerOverflow(ais_handle_t *handle, uint32_t sequence);

/*!
 * @brief App Callback - /speaker receive data
 *
 * This function is called when the state machine is receiving data from AIS on
 * the /speaker topic.  The raw audio data is passed to this function and can be
 * used for playback.
 *
 * @param handle Pointer to AIS interface handle
 * @param data Pointer to audio data (fixed MP3 for AISv1)
 * @param size Size in bytes of audio data
 * @param offset Byte offset in the ongoing speaker stream
 */
void AIS_AppCallback_Speaker(ais_handle_t *handle, uint8_t *data, uint32_t size, uint64_t offset, uint8_t count);

/*!
 * @brief App Callback - speaker marker receivd
 *
 * This function is called by the AIS client library when a speaker marker point
 * has been received.  This must be echoed back to the service after the audio
 * data up to this marker has been played back.
 *
 * @param handle Pointer to AIS interface handle
 * @param marker Marker identifier
 */
void AIS_AppCallback_SpeakerMarker(ais_handle_t *handle, uint32_t marker);

/*!
 * @brief App Callback - Play
 *
 * This function is called when AIS receives a Play directive for executing
 * application-specific behavior.
 *
 * @param handle Pointer to AIS interface handle
 * @param offset Byte offset into audio stream at which to start playback.
 */
void AIS_AppCallback_OpenSpeaker(ais_handle_t *handle, uint64_t offset);

/*!
 * @brief App Callback - Stop
 *
 * This function is called when AIS receives a Stop directive for executing
 * application-specific behavior.
 *
 * @param handle Pointer to AIS interface handle
 * @param offset Byte offset indicating the end of audio data.  If this is
 *               invalid, indicates audio playback should be stopped immediately.
 *               Invalid offset marked with -1.
 * @param immediate Boolean indicating whether to close speaker immediately or
 *                  wait until offset argument.
 */
void AIS_AppCallback_CloseSpeaker(ais_handle_t *handle, uint64_t offset, bool immediate);

/*!
 * @brief App Callback - SetAttentionState
 *
 * This function is called when AIS receives a SetAttentionState directive for
 * executing application-specific behavior.
 *
 * The states here indicate what new state the AIS cloud service has
 * transitioned to.  These map to states in the AVS UX Attention System.
 *
 * @param state AIS service state
 * @param offset Byte offset indicating when in the audio stream to transition
 *               to the new state.
 * @param immediate Boolean indicating whether to change state immediately or
 *                  wait until offset argument
 */
void AIS_AppCallback_SetAttentionState(ais_state_t state, uint64_t offset, bool immediate);

/*!
 * @brief App Callback - SetVolume
 *
 * This function is called when AIS receives a SetVolume directive for executing
 * application-specific behavior.
 *
 * Volume value from AIS ranges from 1-100.  Application can use this to adjust
 * the output speaker volume of the device.
 *
 * @param handle Pointer to AIS interface handle
 * @param volume Requested device volume (1-100)
 * @param offset Byte offset indicating when in the audio stream to change to
 *               new volume.
 * @param immediate Boolean indicating if transition should happen immediately
 *                  or at the offset argument
 */
void AIS_AppCallback_SetVolume(ais_handle_t *handle, uint32_t volume, uint64_t offset, bool immediate);

/*!
 * @brief App Callback - SetAlertVolume
 *
 * This function is called when AIS receives a SetAlertVolume directive for
 * executing application-specific behavior.  The volume should be applied to all
 * future alert playback.
 *
 * Volume value from AIS ranges from 1-100.  Application can use this to adjust
 * the output speaker volume of the device.
 *
 * @param handle Pointer to AIS interface handle
 * @param volume Requested device volume (1-100)
 */
void AIS_AppCallback_SetAlertVolume(uint32_t volume);

/*!
 * @brief App Callback - SetClock
 *
 * This function is called when AIS receives a SetClock directive
 *
 * @param handle Pointer to AIS interface handle
 * @param time 64-bit time in seconds since the 1900 epoch
 */
void AIS_AppCallback_SetClock(uint64_t time);

/*!
 * @brief App Callback - SetAlert
 *
 * This function is called when AIS receives a SetAlert directive
 *
 * @param handle Pointer to AIS interface handle
 * @param token Unique token string for alert
 * @param scheduledTime 64-bit time in seconds since the 1900 epoch at which
 *                      time the alert should be played back.
 * @param durationInMilliseconds The duration the alert should be played back
 *                               when the scheduled time arrives.
 * @param type The alert type
 */
void AIS_AppCallback_SetAlert(const char *token,
                              uint64_t scheduledTime,
                              uint32_t durationInMilliseconds,
                              ais_alert_type_t type);

/*!
 * @brief App Callback - DeleteAlert
 *
 * This function is called when AIS receives a DeleteAlert directive
 *
 * @param handle Pointer to AIS interface handle
 * @param token Unique token string for alert
 */
void AIS_AppCallback_DeleteAlert(const char *token);

/*!
 * @brief App Callback - OpenMicrophone
 *
 * This function is called when AIS receives an OpenMicrophone directive for
 * executing application-specific behavior.
 *
 * @param timeout Timeout in milliseconds that AIS will wait to receive an event
 *        to initiate speech
 * @param type Opaque string from AIS service to pass back with MicrophoneOpened
 * @param token Opaque string from AIS service to pass back with MicrophoneOpened
 */
void AIS_AppCallback_OpenMicrophone(uint32_t timeout, const char *type, const char *token);

/*!
 * @brief App Callback - CloseMicrophone
 *
 * This function is called when AIS receives an CloseMicrophone.  The
 * application should close the microphone immediately and stop processing voice
 * data.
 */
void AIS_AppCallback_CloseMicrophone(ais_handle_t *handle);

/*!
 * @brief App Callback - Exception
 *
 * This function is called when AIS receives an Exception directive for executing
 * application-specific behavior.
 *
 * Exception data is a JSON string that can be parsed by the application.  It
 * contains information on an AIS service failure.
 *
 * @param code Error code string
 * @param description Error description string
 */
void AIS_AppCallback_Exception(const char *code, const char *description);

/*!
 * @brief App Callback - Internal error
 *
 * This function is called when the internal AIS state machine enters an
 * error state.
 *
 * An error indicating a failure to publish microphone data will only affect one
 * message sequence, and the system will continue to try subsequent messages.
 *
 * An error indicating a failure with anything else is unrecoverable and will
 * terminate the service task.
 *
 * @param error Internal error status
 */
void AIS_AppError(ais_status_t error);

/*!
 * @brief App Callback - Save shared secret
 *
 * This function is called when the internal AIS state machine or registration
 * process needs to save a new shared secret in non-volatile memory.
 *
 * @param secret Shared secret (16 bytes)
 */
void AIS_AppRegistrationInfo(ais_reg_config *registrationInfo);

/*!
 * @brief Deregistration function to factory reset
 *
 * This function is called when an encryption error occurs. This will happen
 * when the de-register button is pressed in the Alexa App.
 *
 */
void AIS_Deregister(void);

/*!
 * @brief App Callback - AIS client library has disconnected
 *
 * This function is called when the internal AIS state machine has disconnected
 * from the service due to an error that was not requested by the application.
 *
 * @param *handle Reference to current ais_handle_t in use
 * @param code Error enum
 * @param description Error code description (or NULL)
 */
void AIS_AppDisconnected(ais_handle_t *handle, ais_disconnect_code_t code, const char *description);

/*! @brief Task responsible for publishing MQTT json messages */
void AIS_PublishTask(void *arg);

/*! @brief Queues the publishing of MQTT JSON events to AIS service, then notifies AIS_PublishTask */
status_t AIS_SendJSONToPublishing(ais_handle_t *handle, aisTopic_t topic, cJSON *json, bool encrypt);

/*!
 * @brief Secret Rotate function check per topic
 *
 * This function is called when a sequence number is incremented to see if the topic
 * is ready to rotate secret from the service. This checks to see if the sequence number
 * matches the expected secret rotate sequence to ensure seemless rotatation
 *
 * @param *handle Reference to current ais_handle_t in use
 * @param topic MQTT Topic for the service
 */
void AIS_CheckForSecretRotate(ais_handle_t *handle, aisTopic_t topic);

/*!
 * @brief This Function moves the topic secret pointers
 *
 * This function moves all the topic secret pointers to the long term memory location
 * of the secret. This is only executed when all topics are pointing to a newly rotated
 * secret. This allows the old secret to be flushed.
 *
 * @param *handle Reference to current ais_handle_t in use
 * @param topic MQTT Topic for the service
 * @param secret New memory location of secret to be pointed too
 */
status_t AIS_UpdateTopicSecretPointer(ais_handle_t *handle, aisTopic_t topic, uint8_t *secret);

#if defined(__cplusplus)
}
#endif

#endif
