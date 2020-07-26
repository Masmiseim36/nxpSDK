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

#include "mbedtls/base64.h"

#define AIS_CAPABILITY_TYPE "AisInterface"
#define AIS_CAPABILITY_VERSION "1.0"

static const char *_ais_map_speaker_bitrate(ais_speaker_bitrate_t type)
{
    switch (type) {
        case AIS_SPEAKER_BITRATE_CONSTANT:
            return AIS_CAP_SPEAKER_BITRATE_CONSTANT;
        case AIS_SPEAKER_BITRATE_VARIABLE:
            return AIS_CAP_SPEAKER_BITRATE_VARIABLE;
        default:
            return "";
    }
}

static const char *_ais_map_speaker_decoder(ais_speaker_decoder_t decoder)
{
    switch (decoder) {
        case AIS_SPEAKER_DECODER_OPUS:
            return AIS_CAP_SPEAKER_DECODER_OPUS;
        case AIS_SPEAKER_DECODER_MP3:
            return AIS_CAP_SPEAKER_DECODER_MP3;
        default:
            return "";
    }
}

/* NOTE: 'out' input buffer must be at least 17 bytes. */
static void _build_uuid(ais_handle_t *handle, uint8_t *out)
{
    size_t outBytes;
    uint8_t randomBuf[12];

    mbedtls_ctr_drbg_random(&handle->ctr_drbg, randomBuf, 12);
    mbedtls_base64_encode(out, 17, &outBytes, randomBuf, 12);

    out[16] = '\0';
}

cJSON *JSON_BuildHeader(ais_handle_t *handle, const char *name, const char **id)
{
    cJSON *header, *jsonId;
    uint8_t messageId[17];

    _build_uuid(handle, messageId);
    jsonId = cJSON_CreateString((char*) messageId);

    header = cJSON_CreateObject();
    cJSON_AddItemToObject(header, "name", cJSON_CreateString(name));
    cJSON_AddItemToObject(header, "messageId", jsonId);

    *id = jsonId->valuestring;

    return header;
}

const char* JSON_BuildEvent(ais_handle_t *handle, const char *name,
                            ais_json_t *jsonObj, bool addPayload)
{
    cJSON *json, *events, *event;
    const char *id;

    json = cJSON_CreateObject();

    events = cJSON_CreateArray();
    cJSON_AddItemToObject(json, "events", events);

    event = cJSON_CreateObject();
    cJSON_AddItemToArray(events, event);

    cJSON_AddItemToObject(event, "header", JSON_BuildHeader(handle, name, &id));

    jsonObj->json = json;

    if (addPayload) {
        cJSON *payload = cJSON_CreateObject();
        cJSON_AddItemToObject(event, "payload", payload);

        jsonObj->payload = payload;
    } else {
        jsonObj->payload = NULL;
    }

    return id;
}

cJSON* JSON_BuildCapability(const char *name)
{
    cJSON *cap = cJSON_CreateObject();

    /* Speaker */
    cJSON_AddItemToObject(cap, "type", cJSON_CreateString(AIS_CAPABILITY_TYPE));
    cJSON_AddItemToObject(cap, "interface", cJSON_CreateString(name));
    cJSON_AddItemToObject(cap, "version", cJSON_CreateString(AIS_CAPABILITY_VERSION));

    return cap;
}

cJSON* JSON_BuildCapabilitySpeaker(ais_handle_t *handle)
{
    cJSON *cap = JSON_BuildCapability("Speaker");

    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(cap, "configurations", config);
    cJSON *audbuf = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "audioBuffer", audbuf);
    cJSON_AddItemToObject(audbuf, "sizeInBytes", cJSON_CreateNumber(AWS_AUDIO_BUFFER_SIZE));
    cJSON *reporting = cJSON_CreateObject();
    cJSON_AddItemToObject(audbuf, "reporting", reporting);
    cJSON_AddItemToObject(reporting, "overrunWarningThreshold",
                          cJSON_CreateNumber(AWS_AUDIO_BUFFER_OVERRUN_THRESHOLD));
    cJSON_AddItemToObject(reporting, "underrunWarningThreshold",
                          cJSON_CreateNumber(AWS_AUDIO_BUFFER_UNDERRUN_THRESHOLD));
    cJSON *audcodec = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "audioDecoder", audcodec);
    cJSON_AddItemToObject(audcodec, "format", cJSON_CreateString(
        _ais_map_speaker_decoder(handle->config->speakerDecoder)));
    cJSON_AddItemToObject(audcodec, "numberOfChannels",
                          cJSON_CreateNumber(handle->config->speakerChannels));
    cJSON *bitrate = cJSON_CreateObject();
    cJSON_AddItemToObject(audcodec, "bitrate", bitrate);
    cJSON_AddItemToObject(bitrate, "type", cJSON_CreateString(
        _ais_map_speaker_bitrate(handle->config->speakerBitrateType)));
    cJSON_AddItemToObject(bitrate, "bitsPerSecond",
                          cJSON_CreateNumber(handle->config->speakerBitrate));

    return cap;
}

cJSON* JSON_BuildCapabilityMicrophone(void)
{
    cJSON *cap = JSON_BuildCapability("Microphone");

    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(cap, "configurations", config);
    cJSON *audcodec = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "audioEncoder", audcodec);
    cJSON_AddItemToObject(audcodec, "format",
                          cJSON_CreateString("AUDIO_L16_RATE_16000_CHANNELS_1"));

    return cap;
}

cJSON* JSON_BuildCapabilityAlerts(ais_handle_t *handle)
{
    cJSON *cap = JSON_BuildCapability("Alerts");

    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(cap, "configurations", config);
    cJSON_AddItemToObject(config, "maxAlertCount",
                          cJSON_CreateNumber(handle->config->maxAlertCount));

    return cap;
}

cJSON* JSON_BuildCapabilityClock(void)
{
    return JSON_BuildCapability("Clock");
}

cJSON* JSON_BuildCapabilitySystem(ais_handle_t *handle)
{
    cJSON *cap = JSON_BuildCapability("System");

    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(cap, "configurations", config);
    cJSON_AddItemToObject(config, "firmwareVersion",
                          cJSON_CreateString(handle->config->firmwareVersion));
    cJSON_AddItemToObject(config, "locale",
                          cJSON_CreateString(handle->config->locale));
    cJSON *mqtt = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "mqtt", mqtt);
    cJSON *message = cJSON_CreateObject();
    cJSON_AddItemToObject(mqtt, "message", message);
    cJSON_AddItemToObject(message, "maxSizeInBytes",
                          cJSON_CreateNumber(AIS_MQTT_MAX_RX_SIZE));

    return cap;
}

cJSON* JSON_BuildRegistration(ais_handle_t *handle, const char *clientKey)
{
    cJSON *json, *authentication, *encryption, *iot;

    json = cJSON_CreateObject();
    authentication = cJSON_CreateObject();
    cJSON_AddItemToObject(authentication, "token", cJSON_CreateString(handle->config->awsAuthToken));
    cJSON_AddItemToObject(authentication, "clientId", cJSON_CreateString(handle->config->awsAuthClientId));
    cJSON_AddItemToObject(json, "authentication", authentication);

    encryption = cJSON_CreateObject();
    cJSON_AddItemToObject(encryption, "algorithm", cJSON_CreateString("ECDH_CURVE_25519_16_BYTE_SHA256"));
    cJSON_AddItemToObject(encryption, "publicKey", cJSON_CreateString(clientKey));
    cJSON_AddItemToObject(json, "encryption", encryption);

    iot = cJSON_CreateObject();
    cJSON_AddItemToObject(iot, "awsAccountId", cJSON_CreateString(handle->config->awsAccountId));
    cJSON_AddItemToObject(iot, "clientId", cJSON_CreateString(handle->config->awsClientId));
    cJSON_AddItemToObject(iot, "endpoint", cJSON_CreateString(handle->config->awsEndpoint));

#if (!defined(AIS_SPEC_REV_325))
    cJSON_AddItemToObject(iot, "topicRoot", cJSON_CreateString(handle->config->registrationConfig.awsPartnerRoot));
#endif
    cJSON_AddItemToObject(json, "iot", iot);

    return json;
}
