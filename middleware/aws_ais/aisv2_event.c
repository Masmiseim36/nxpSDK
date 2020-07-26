/*
 * Copyright 2018 NXP. 
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "aisv2.h"
#include "ais_streamer.h"

/* Forward function declarations for AIS-private JSON functions. */
cJSON *JSON_BuildHeader(ais_handle_t *handle, const char *name, const char **id);
const char* JSON_BuildEvent(ais_handle_t *handle, const char *name, ais_json_t *jsonObj, bool addPayload);
cJSON* JSON_BuildCapabilitySpeaker(ais_handle_t *handle);
cJSON* JSON_BuildCapabilityMicrophone(void);
cJSON* JSON_BuildCapabilityAlerts(ais_handle_t *handle);
cJSON* JSON_BuildCapabilityClock(void);
cJSON* JSON_BuildCapabilitySystem(ais_handle_t *handle);

status_t AIS_PublishJSON(ais_handle_t *handle, aisTopic_t topic, cJSON *json, bool encrypt);


/*! @brief Map a profile enumeration to corresponding string. */
static const char *_ais_map_asr_profile(ais_asr_profile_t profile)
{
    switch (profile)
    {
        case AIS_ASR_CLOSE_TALK:
            return AIS_EVENT_PROFILE_CLOSE_TALK;
        case AIS_ASR_NEAR_FIELD:
            return AIS_EVENT_PROFILE_NEAR_FIELD;
        case AIS_ASR_FAR_FIELD:
            return AIS_EVENT_PROFILE_FAR_FIELD;
        default:
            return "";
    }
}

/*! @brief Map an initiator enumeration to corresponding string. */
static const char *_ais_map_initiator(ais_initiator_t init)
{
    switch (init)
    {
        case AIS_INITIATOR_PRESS_AND_HOLD:
            return AIS_EVENT_INITIATOR_TYPE_PH;
        case AIS_INITIATOR_TAP:
            return AIS_EVENT_INITIATOR_TYPE_TAP;
        case AIS_INITIATOR_WAKEWORD:
            return AIS_EVENT_INITIATOR_WAKEWORD;
        default:
            return "";
    }
}

static const char *_ais_map_button_cmd(ais_button_cmd_t cmd)
{
    switch (cmd)
    {
        case AIS_BUTTON_CMD_PLAY:
            return AIS_EVENT_BUTTON_PLAY;
        case AIS_BUTTON_CMD_NEXT:
            return AIS_EVENT_BUTTON_NEXT;
        case AIS_BUTTON_CMD_PREVIOUS:
            return AIS_EVENT_BUTTON_PREVIOUS;
        case AIS_BUTTON_CMD_STOP:
            return AIS_EVENT_BUTTON_STOP;
        case AIS_BUTTON_CMD_PAUSE:
            return AIS_EVENT_BUTTON_PAUSE;
        default:
            return "";
    }
}

static const char *_ais_map_buffer_state(ais_buffer_state_t state)
{
    switch (state)
    {
    	case AIS_BUFFER_STATE_GOOD:
			return AIS_EVENT_BUFFER_GOOD;
        case AIS_BUFFER_STATE_OVERRUN:
            return AIS_EVENT_BUFFER_OVERRUN;
        case AIS_BUFFER_STATE_OVERRUN_WARNING:
            return AIS_EVENT_BUFFER_OVERRUN_WARNING;
        case AIS_BUFFER_STATE_UNDERRUN:
            return AIS_EVENT_BUFFER_UNDERRUN;
        case AIS_BUFFER_STATE_UNDERRUN_WARNING:
            return AIS_EVENT_BUFFER_UNDERRUN_WARNING;
        default:
            return "";
    }
}

status_t AIS_PublishConnect(ais_handle_t *handle)
{
    cJSON *json, *payload;
    const char *id;

    json = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "header",
                          JSON_BuildHeader(handle, "Connect", &id));
    payload = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "payload", payload);
    cJSON_AddItemToObject(payload, "awsAccountId",
                          cJSON_CreateString(handle->config->awsAccountId));
    cJSON_AddItemToObject(payload, "clientId",
                          cJSON_CreateString(handle->config->awsClientId));

    configPRINTF(("[AIS %s] Publishing Connect\r\n", id));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_CONNECTION_FROMCLIENT, json, false);
}

status_t AIS_PublishDisconnect(ais_handle_t *handle, const char *code)
{
    cJSON *json, *payload;
    const char *id;

    json = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "header",
                          JSON_BuildHeader(handle, "Disconnect", &id));
    payload = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "payload", payload);
    cJSON_AddItemToObject(payload, "code", cJSON_CreateString(code));

    configPRINTF(("[AIS %s] Publishing Disconnect\r\n", id));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_CONNECTION_FROMCLIENT, json, false);
}

status_t AIS_PublishCapabilities(ais_handle_t *handle)
{
    cJSON *json, *payload;
    const char *id;

    json = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "header",
                          JSON_BuildHeader(handle, "Publish", &id));
    payload = cJSON_CreateObject();
    cJSON_AddItemToObject(json, "payload", payload);

    cJSON *caps = cJSON_CreateArray();
    cJSON_AddItemToObject(payload, "capabilities", caps);

    cJSON_AddItemToArray(caps, JSON_BuildCapabilitySpeaker(handle));
    cJSON_AddItemToArray(caps, JSON_BuildCapabilityMicrophone());
    cJSON_AddItemToArray(caps, JSON_BuildCapabilityAlerts(handle));
    cJSON_AddItemToArray(caps, JSON_BuildCapabilityClock());
    cJSON_AddItemToArray(caps, JSON_BuildCapabilitySystem(handle));

    configPRINTF(("[AIS %s] Publishing Capabilities\r\n", id));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_CAPABILITIES_PUBLISH, json, true);
}

status_t AIS_EventSecretRotated(ais_handle_t *handle,
                           uint32_t eventSequenceNumber,
                           uint32_t micSequenceNumber)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_SECRET_ROTATED, &json, true);
    cJSON_AddItemToObject(json.payload, "eventSequenceNumber",
                          cJSON_CreateNumber(eventSequenceNumber));
    cJSON_AddItemToObject(json.payload, "microphoneSequenceNumber",
                          cJSON_CreateNumber(micSequenceNumber));

    configPRINTF(("[AIS %s] Publishing SecretRotated\r\n", id));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventButtonCommandIssued(ais_handle_t *handle, ais_button_cmd_t cmd)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_BUTTON_CMD_ISSUED, &json, true);
    cJSON_AddItemToObject(json.payload, "command",
                          cJSON_CreateString(_ais_map_button_cmd(cmd)));

    configPRINTF(("[AIS %s] Publishing ButtonCommandIssued: %s\r\n",
                  id, _ais_map_button_cmd(cmd)));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventSpeakerOpened(ais_handle_t *handle, uint64_t offset)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_SPEAKER_OPEN, &json, true);
    cJSON_AddItemToObject(json.payload, "offset", cJSON_CreateNumber(offset));

    configPRINTF(("[AIS %s] Publishing SpeakerOpened, offset: %d\r\n", id, offset));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}


status_t AIS_EventSpeakerClosed(ais_handle_t *handle, uint64_t offset)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_SPEAKER_CLOSED, &json, true);
    cJSON_AddItemToObject(json.payload, "offset", cJSON_CreateNumber(offset));

    configPRINTF(("[AIS %s] Publishing SpeakerClosed, offset: %d\r\n", id, offset));

	/* Stop sequence timer for stream buffer */
	handle->seqTimerSpeaker = 0;

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventSpeakerMarkerEncountered(ais_handle_t *handle, uint32_t marker)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_SPEAKER_MARKER_ENCOUNTERED, &json, true);
    cJSON_AddItemToObject(json.payload, "marker", cJSON_CreateNumber(marker));

    configPRINTF(("[AIS %s] Publishing SpeakerMarkerEncountered, marker: %d\r\n",
                  id, marker));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

/* Called by app after wakeword trigger or push to talk.
 * This will put the AIS service into microphone publish mode. */
status_t AIS_EventMicrophoneOpened(ais_handle_t *handle, ais_mic_open_t *mic)
{
    ais_json_t json;
    cJSON *init, *payload_init, *wwi;
    const char *id;
    status_t ret;

    id = JSON_BuildEvent(handle, AIS_EVENT_MICROPHONE_OPENED, &json, true);

    cJSON_AddItemToObject(json.payload, "profile",
                          cJSON_CreateString(_ais_map_asr_profile(mic->asr_profile)));
    /* NOTE: offset should be first byte of pre-roll if using wakeword initiator */
    cJSON_AddItemToObject(json.payload, "offset",
                          cJSON_CreateNumber(handle->micStream.audio.audioData.offset));

    init = cJSON_CreateObject();
    cJSON_AddItemToObject(json.payload, "initiator", init);
    payload_init = cJSON_CreateObject();
    cJSON_AddItemToObject(init, "payload", payload_init);

    if (mic->init_token)
    {
        cJSON_AddItemToObject(init, "type", cJSON_CreateString(mic->init_type));
        cJSON_AddItemToObject(payload_init, "token", cJSON_CreateString(mic->init_token));
    }
    else
    {
        if (mic->initiator == AIS_INITIATOR_WAKEWORD)
        {
            cJSON_AddItemToObject(payload_init, "wakeWord", cJSON_CreateString("ALEXA"));
            wwi = cJSON_CreateObject();
            cJSON_AddItemToObject(payload_init, "wakeWordIndices", wwi);
            cJSON_AddItemToObject(wwi, "beginOffset", cJSON_CreateNumber(mic->wwStart));
            cJSON_AddItemToObject(wwi, "endOffset", cJSON_CreateNumber(mic->wwEnd));
        }

        cJSON_AddItemToObject(init, "type", cJSON_CreateString(_ais_map_initiator(mic->initiator)));
    }

    configPRINTF(("[AIS %s] Publishing MicrophoneOpened\r\n", id));

    ret = AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);

    AIS_SetState(handle, AIS_TASK_STATE_MICROPHONE);

    return ret;
}

/* Called by app to stop sending microphone data.
 * This will force an EndOfSpeech message to AIS, and transition the internal
 * task to IDLE. */
status_t AIS_EventMicrophoneClosed(ais_handle_t *handle, uint64_t offset)
{
    ais_json_t json;
    const char *id;
    status_t ret;

    id = JSON_BuildEvent(handle, AIS_EVENT_MICROPHONE_CLOSED, &json, true);
    cJSON_AddItemToObject(json.payload, "offset", cJSON_CreateNumber(offset));

    configPRINTF(("[AIS %s] Publishing MicrophoneClosed, offset: %d\r\n", id, offset));

    ret = AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);

    AIS_ClearState(handle, AIS_TASK_STATE_MICROPHONE);

    return ret;
}

status_t AIS_EventOpenMicrophoneTimedOut(ais_handle_t *handle)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_OPEN_MICROPHONE_TIMED_OUT, &json, false);

    configPRINTF(("[AIS %s] Publishing OpenMicrophoneTimedOut\r\n", id));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventBufferStateChanged(ais_handle_t *handle,
		                             ais_buffer_state_t old_state,
		                             ais_buffer_state_t new_state,
                                     uint32_t sequence)
{
    ais_json_t json;
    cJSON *message;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_BUFFER_STATE_CHANGED, &json, true);
    cJSON_AddItemToObject(json.payload, "state",
                          cJSON_CreateString(_ais_map_buffer_state(new_state)));

    message = cJSON_CreateObject();
    cJSON_AddItemToObject(json.payload, "message", message);
    cJSON_AddItemToObject(message, "topic", cJSON_CreateString("SPEAKER"));
    cJSON_AddItemToObject(message, "sequenceNumber", cJSON_CreateNumber(sequence));

    configPRINTF(("[AIS %s] Publishing BufferStateChanged, old state: %s, new state: %s, seq: %d, buffered: %d\r\n",
                  id,
				  _ais_map_buffer_state(old_state),
				  _ais_map_buffer_state(new_state),
				  sequence,
				  STREAMER_GetQueued(handle->audioPlayer)));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventVolumeChanged(ais_handle_t *handle, uint32_t volume, uint64_t offset)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_VOLUME_CHANGED, &json, true);
    cJSON_AddItemToObject(json.payload, "volume", cJSON_CreateNumber(volume));
    if (offset != 0)
    {
        cJSON_AddItemToObject(json.payload, "offset", cJSON_CreateNumber(offset));
    }

    configPRINTF(("[AIS %s] Publishing VolumeChanged, volume: %d, speakerOffset %llu\r\n", id, volume, offset));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventSynchronizeClock(ais_handle_t *handle)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_SYNCHRONIZE_CLOCK, &json, false);

    configPRINTF(("[AIS %s] Publishing SynchronizeClock\r\n", id));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventSetAlertSucceeded(ais_handle_t *handle, const char *token)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_SET_ALERT_SUCCEEDED, &json, true);
    cJSON_AddItemToObject(json.payload, "token", cJSON_CreateString(token));

    configPRINTF(("[AIS %s] Publishing SetAlertSucceeded: %s\r\n", id, token));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventSetAlertFailed(ais_handle_t *handle, const char *token)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_SET_ALERT_FAILED, &json, true);
    cJSON_AddItemToObject(json.payload, "token", cJSON_CreateString(token));

    configPRINTF(("[AIS %s] Publishing SetAlertFailed: %s\r\n", id, token));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventDeleteAlertSucceeded(ais_handle_t *handle, const char *token)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_DELETE_ALERT_SUCCEEDED, &json, true);
    cJSON_AddItemToObject(json.payload, "token", cJSON_CreateString(token));

    configPRINTF(("[AIS %s] Publishing DeleteAlertSucceeded: %s\r\n", id, token));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventDeleteAlertFailed(ais_handle_t *handle, const char *token)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_DELETE_ALERT_FAILED, &json, true);
    cJSON_AddItemToObject(json.payload, "token", cJSON_CreateString(token));

    configPRINTF(("[AIS %s] Publishing DeleteAlertFailed: %s\r\n", id, token));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventAlertVolumeChanged(ais_handle_t *handle, uint32_t volume)
{
    ais_json_t json;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_ALERT_VOLUME_CHANGED, &json, true);
    cJSON_AddItemToObject(json.payload, "volume", cJSON_CreateNumber(volume));

    configPRINTF(("[AIS %s] Publishing AlertVolumeChanged, volume: %d\r\n", id, volume));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventSynchronizeState(ais_handle_t *handle, int32_t volume, const char *alertTokens[], uint32_t alertCount)
{
    ais_json_t json;
#if defined(AIS_ENABLE_LOCAL_VOLUME_CONTROL) && (AIS_ENABLE_LOCAL_VOLUME_CONTROL == 1)
    cJSON *speaker;
#endif
    cJSON *alerts, *alertsArray;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_SYNCHRONIZE_STATE, &json, true);

#if defined(AIS_ENABLE_LOCAL_VOLUME_CONTROL) && (AIS_ENABLE_LOCAL_VOLUME_CONTROL == 1)
    /* Only send the volume if it's valid, this assures on factory boot, the volume is received from AIS */
    if (volume >= 0)
    {
        configPRINTF(("[AIS %s] Setting last local volume %d\r\n", id, volume));
	    speaker = cJSON_CreateObject();
	    cJSON_AddItemToObject(json.payload, "speaker", speaker);
	    cJSON_AddItemToObject(speaker, "volume", cJSON_CreateNumber(volume));
    }
    else
    {
        configPRINTF(("[AIS %s] No valid volume, using volume defined by AIS\r\n", id, volume));
    }
#else
    configPRINTF(("[AIS %s] Using volume last defined in the AIS service.\r\n", id));
    configPRINTF(("[AIS %s] Volume input of %d is ignored.\r\n", id, volume));
#endif

    alerts = cJSON_CreateObject();
    cJSON_AddItemToObject(json.payload, "alerts", alerts);

    alertsArray = cJSON_CreateArray();
#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
    cJSON_AddItemToObject(alerts, "allAlerts", alertsArray);
#else
    cJSON_AddItemToObject(alerts, "offlineAlertsDeleted", alertsArray);
#endif

    for (int i = 0; i < alertCount; i++) {
        cJSON_AddItemToArray(alertsArray, cJSON_CreateString(alertTokens[i]));
    }

    configPRINTF(("[AIS %s] Publishing SynchronizeState\r\n", id));
    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}

status_t AIS_EventExceptionEncountered(ais_handle_t *handle, const char * description)
{
    ais_json_t json;
    cJSON *error;
    const char *id;

    id = JSON_BuildEvent(handle, AIS_EVENT_EXCEPTION_ENCOUNTERED, &json, true);

    error = cJSON_CreateObject();
    cJSON_AddItemToObject(json.payload, "error", error);
    cJSON_AddItemToObject(error, "code", cJSON_CreateString("INTERNAL_ERROR"));
    cJSON_AddItemToObject(error, "description", cJSON_CreateString(description));

    /* TODO: add more elaborate exception logging information (optional) */

    configPRINTF(("[AIS %s] Publishing ExceptionEncountered\r\n", id));

    return AIS_SendJSONToPublishing(handle, AIS_TOPIC_EVENT, json.json, true);
}
