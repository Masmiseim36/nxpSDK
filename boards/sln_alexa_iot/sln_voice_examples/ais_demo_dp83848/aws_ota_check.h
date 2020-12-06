/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 */

#ifndef _AWS_OTA_CHECK_H_
#define _AWS_OTA_CHECK_H_

/* Includes required by the FreeRTOS timers structure. */
#include "FreeRTOS.h"
#include "timers.h"

/* MQTT includes. */
#include "iot_mqtt_agent.h"

/* Evaluates to the length of a constant string defined like 'static const char str[]= "xyz"; */
#define CONST_STRLEN(s) (((uint32_t)sizeof(s)) - 1UL)

#define APPLICATION_RESET_ISR_ADDRESS (SCB->VTOR + 0x4)

#define APP_VECTOR_TABLE_APP_A (0x60300000U)
#define APP_VECTOR_TABLE_APP_B (0x60D00000U)
#define JSON_FILEPATH_APPA     "AppA"
#define JSON_FILEPATH_APPB     "AppB"
#define JSON_FILEPATH_APPUNK   "UnknownApp"

/* When subscribing to MQTT topics with a callback handler, we use the callback
 * context variable as a subscription type to expedite dispatch of the published
 * messages instead of comparing against the topic string.
 */
typedef enum
{
    eOTA_PubMsgType_Job = 0, /* Messages on the topic are job messages. */
    eOTA_PubMsgType_Stream   /* Messages on the topic are stream messages. */
} OTA_PubMsgType_t;

/**
 * @brief OTA Error type.
 */
typedef uint32_t OTA_Err_t;

#define DEFINE_OTA_METHOD_NAME(name) static const char OTA_METHOD_NAME[] = name;

/**
 * @defgroup OTA Agent error codes.
 * @brief Error codes returned by OTA agent API.
 *
 * @note OTA agent error codes are in the upper 8 bits of the 32 bit OTA error word, OTA_Err_t.
 */
#define kOTA_Err_Uninitialized 0xff000000UL /*!< The error code has not yet been set by a logic path. */
#define kOTA_Err_None          0x00000000UL
#define kOTA_Err_AbortFailed   0x07000000UL /*!< Error trying to abort the OTA. */
#define kOTA_Err_PublishFailed 0x08000000UL /*!< Attempt to publish a MQTT message failed. */
#define kOTA_Err_TopicTooLarge 0x2a000000UL /*!< Attempt to build a topic string larger than the supplied buffer. */

#define OTA_MAX_THINGNAME_LEN 64

typedef struct _ota_handle_t
{
    bool otaInitDone;
    char thingName[OTA_MAX_THINGNAME_LEN];
} ota_handle_t;

/**
 * @brief This function populates the OTA handler with the Mqtt handler and the thing name
 * @param pOtaHandle    Pointer to OTA handler
 * @param pcThingName   A constant char with the name of the thing
 *
 * @return              A status_t indicating the if the operation was successful
 */
status_t OTA_Init(ota_handle_t *pOtaHandle, const char *pcThingName);
/**
 * @brief This function subscribes to the OTA Mqtt topic and checks if a job is available
 * @param pOtaHandle    Pointer to OTA handler
 *
 * @return              A status_t indicating the if the operation was successful
 */
status_t OTA_Connect(ota_handle_t *pOtaHandle);
/**
 * @brief This function unsubscribes from the OTA Mqtt topic
 * @param pOtaHandle    Pointer to OTA handler
 *
 * @return              A status_t indicating the if the operation was successful
 */
status_t OTA_Disconnect(ota_handle_t *pOtaHandle);

#define FICA_START_ADDRESS    0x01FC0000
#define FICA_OFFSET_ICA_COMM  8
#define FICA_COMM_AIS_OTA_BIT 0x00000010

void ota_task(void *arg);

#endif /* _AWS_OTA_CHECK_H_ */
