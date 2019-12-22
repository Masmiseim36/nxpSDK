/*
 * Amazon FreeRTOS Shadow Demo V1.2.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2017-2019 NXP
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Logging includes. */
#include "iot_logging_task.h"

/* MQTT include. */
#include "iot_mqtt_agent.h"

/* Required to get the broker address and port. */
#include "aws_clientcredential.h"
#include "queue.h"

/* Required for shadow API's */
#include "aws_shadow.h"
#include "jsmn.h"

#include "iot_init.h"

#include "board.h"

/* Maximal count of tokens in parsed JSON */
#define MAX_CNT_TOKENS 32

#define shadowDemoTIMEOUT pdMS_TO_TICKS(30000UL)

/* Name of the thing */
#define shadowDemoTHING_NAME clientcredentialIOT_THING_NAME

#define shadowBUFFER_LENGTH 210

/* Board specific accelerometer driver include */
#if defined(BOARD_ACCEL_FXOS)
#include "fsl_fxos.h"
#elif defined(BOARD_ACCEL_MMA)
#include "fsl_mma.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** stack size for task that handles shadow delta and updates
 */
#define DEMO_REMOTE_CONTROL_TASK_STACK_SIZE ((uint16_t)configMINIMAL_STACK_SIZE * (uint16_t)20)

typedef struct
{
    char *pcDeltaDocument;
    uint32_t ulDocumentLength;
    void *xBuffer;
} jsonDelta_t;

#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
/* Type definition of structure for data from the accelerometer */
typedef struct
{
    int16_t A_x;
    int16_t A_y;
    int16_t A_z;
} vector_t;
#endif

/* Accelerometer driver specific defines */
#if defined(BOARD_ACCEL_FXOS)
#define ACCELL_READ_SENSOR_DATA(handle, data) FXOS_ReadSensorData(handle, data)
#elif defined(BOARD_ACCEL_MMA)
#define ACCELL_READ_SENSOR_DATA(handle, data) MMA_ReadSensorData(handle, data)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void prvShadowMainTask(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static char pcUpdateBuffer[shadowBUFFER_LENGTH];
static ShadowClientHandle_t xClientHandle;
QueueHandle_t jsonDeltaQueue = NULL;

/* Actual state of LED */
uint16_t ledState       = 0;
uint16_t parsedLedState = 0;

#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
/* Actual state of accelerometer */
uint16_t accState       = 0;
uint16_t parsedAccState = 0;
#endif

/* Has the board RGB LED */
#if defined HAS_RGB_LED
bool g_hasRgbLed = true;
#else
bool g_hasRgbLed = false;
#endif

/* Accelerometer and magnetometer */
#if defined(BOARD_ACCEL_FXOS)
extern fxos_handle_t accelHandle;
#elif defined(BOARD_ACCEL_MMA)
extern mma_handle_t accelHandle;
#endif

#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
extern uint8_t g_accelDataScale;
extern uint8_t g_accelResolution;
#endif

extern void turnOnLed(uint8_t id);
extern void turnOffLed(uint8_t id);

extern const char *ledName[];
extern uint8_t ledCount;
extern char ledColors[];

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
/*!
 * @brief Read accelerometer sensor value
 */
void read_mag_accel(vector_t *results, bool *status, uint8_t accelResolution)
{
#if defined(BOARD_ACCEL_FXOS)
    fxos_data_t sensorData = {0};
#elif defined(BOARD_ACCEL_MMA)
    mma_data_t sensorData = {0};
#endif
    if (kStatus_Success != ACCELL_READ_SENSOR_DATA(&accelHandle, &sensorData))
    {
        /* Failed to read magnetometer and accelerometer data! */
        *status = false;
        return;
    }

    uint8_t divider = (1 << (16 - accelResolution));

    /* Get the accelerometer data from the sensor */
    results->A_x =
        (int16_t)((uint16_t)((uint16_t)sensorData.accelXMSB << 8) | (uint16_t)sensorData.accelXLSB) / divider;
    results->A_y =
        (int16_t)((uint16_t)((uint16_t)sensorData.accelYMSB << 8) | (uint16_t)sensorData.accelYLSB) / divider;
    results->A_z =
        (int16_t)((uint16_t)((uint16_t)sensorData.accelZMSB << 8) | (uint16_t)sensorData.accelZLSB) / divider;

    *status = true;
}

/* Build JSON document with reported state of the "accel" */
int buildJsonAccel()
{
    /* Read data from accelerometer */
    vector_t vec = {0};
    bool read_ok = false;
    read_mag_accel(&vec, &read_ok, g_accelResolution);
    if (read_ok == false)
    {
        return -1;
    }

    /* Convert raw data from accelerometer to acceleration range multiplied by 1000 (for range -2/+2 the values will be
     * in range -2000/+2000) */
    vec.A_x = (int16_t)((int32_t)vec.A_x * g_accelDataScale * 1000 / (1 << (g_accelResolution - 1)));
    vec.A_y = (int16_t)((int32_t)vec.A_y * g_accelDataScale * 1000 / (1 << (g_accelResolution - 1)));
    vec.A_z = (int16_t)((int32_t)vec.A_z * g_accelDataScale * 1000 / (1 << (g_accelResolution - 1)));

    char tmpBufAccel[128] = {0};
    sprintf(tmpBufAccel, "{\"accel\":{\"x\":%d,\"y\":%d,\"z\":%d}}", vec.A_x, vec.A_y, vec.A_z);

    int ret = 0;
    ret     = snprintf(pcUpdateBuffer, shadowBUFFER_LENGTH,
                   "{\"state\":{"
                   "\"desired\":{"
                   "\"accelUpdate\":null"
                   "},"
                   "\"reported\":%s},"
                   "\"clientToken\": \"token-%d\""
                   "}",
                   tmpBufAccel, (int)xTaskGetTickCount());

    if (ret >= shadowBUFFER_LENGTH || ret < 0)
    {
        return -1;
    }
    else
    {
        return ret;
    }
}
#endif

/* Called when there's a difference between "reported" and "desired" in Shadow document. */
static BaseType_t prvDeltaCallback(void *pvUserData,
                                   const char *const pcThingName,
                                   const char *const pcDeltaDocument,
                                   uint32_t ulDocumentLength,
                                   MQTTBufferHandle_t xBuffer)
{
    (void)pvUserData;
    (void)pcThingName;

    /* add the to queue for processing */
    jsonDelta_t jsonDelta;
    jsonDelta.pcDeltaDocument  = (char *)pcDeltaDocument;
    jsonDelta.ulDocumentLength = ulDocumentLength;
    jsonDelta.xBuffer          = xBuffer;

    if (jsonDeltaQueue == NULL)
    {
        return pdFALSE;
    }

    if (xQueueSend(jsonDeltaQueue, &jsonDelta, (TickType_t)10) != pdPASS)
    {
        configPRINTF(("Fail to send message to jsonDeltaQueue.\r\n"));
        /* return pdFALSE - don't take ownership */
        return pdFALSE;
    }

    /* return pdTRUE - take ownership of the mqtt buffer - must be returned by SHADOW_ReturnMQTTBuffer */
    return pdTRUE;
}

/* Generate initial shadow document */
static uint32_t prvGenerateShadowJSON()
{
    /* Init shadow document with settings desired and reported state of device. */
    return snprintf(pcUpdateBuffer, shadowBUFFER_LENGTH,
                    "{"
                    "\"state\":{"
                    "\"desired\":{"
                    "\"LEDstate\":%d"
                    "},"
                    "\"reported\":{"
                    "\"LEDstate\":%d,"
#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
                    "\"accel\":{\"x\":0,\"y\":0,\"z\":0},"
#endif
                    "\"LEDinfo\":{"
                    "\"isRgbLed\":%s,"
                    "\"colors\":%s"
                    "}"
                    "}"
                    "},"
                    "\"clientToken\": \"token-%d\""
                    "}",
                    ledState, ledState, g_hasRgbLed ? "true" : "false", ledColors, (int)xTaskGetTickCount());
}

/* Reports current state to shadow */
static uint32_t prvReportShadowJSON()
{
    return snprintf(pcUpdateBuffer, shadowBUFFER_LENGTH,
                    "{"
                    "\"state\":{"
                    "\"reported\":{"
                    "\"LEDstate\":%d"
                    "}"
                    "},"
                    "\"clientToken\": \"token-%d\""
                    "}",
                    ledState, (int)xTaskGetTickCount());
}

int parseStringValue(char *val, char *json, jsmntok_t *token)
{
    if (token->type == JSMN_STRING)
    {
        int len = token->end - token->start;
        memcpy(val, json + token->start, len);
        val[len] = '\0';
        return 0;
    }
    return -1;
}

int parseUInt16Value(uint16_t *val, char *json, jsmntok_t *token)
{
    if (token->type == JSMN_PRIMITIVE)
    {
        if (sscanf(json + token->start, "%hu", val) == 1)
        {
            return 0;
        }
    }
    return -1;
}

int compareString(char *json, jsmntok_t *token, char *string)
{
    if (token->type == JSMN_STRING)
    {
        int len    = strlen(string);
        int tokLen = token->end - token->start;
        if (len > tokLen)
        {
            len = tokLen;
        }
        return strncmp(string, json + token->start, len);
    }
    return -1;
}

int findTokenIndex(char *json, jsmntok_t *tokens, uint32_t length, char *name)
{
    int i;
    for (i = 0; i < length; i++)
    {
        if (compareString(json, &tokens[i], name) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* Process shadow delta JSON */
void processShadowDeltaJSON(char *json, uint32_t jsonLength)
{
    jsmn_parser parser;
    jsmn_init(&parser);

    jsmntok_t tokens[MAX_CNT_TOKENS];

    int tokenCnt = 0;
    tokenCnt     = jsmn_parse(&parser, json, jsonLength, tokens, MAX_CNT_TOKENS);
    /* the token with state of device is at 6th positin in this delta JSON:
     * {"version":229,"timestamp":1510062270,"state":{"LEDstate":1},"metadata":{"LEDstate":{"timestamp":1510062270}}} */
    if (tokenCnt < 6)
    {
        return;
    }

    /* find index of token "state" */
    int stateTokenIdx;
    stateTokenIdx = findTokenIndex(json, tokens, tokenCnt, "state");
    if (stateTokenIdx < 0)
    {
        return;
    }

    int i = stateTokenIdx + 1;

    /* end position of "state" object in JSON */
    int stateTokenEnd = tokens[i].end;

    char key[20]         = {0};
    int err              = 0;
    uint16_t parsedValue = 0;

    while (i < tokenCnt)
    {
        if (tokens[i].end > stateTokenEnd)
        {
            break;
        }

        err = parseStringValue(key, json, &tokens[i++]);
        if (err == 0)
        {
            if (strstr(key, "LEDstate"))
            {
                /* found "LEDstate" keyword, parse value of next token */
                err = parseUInt16Value(&parsedValue, json, &tokens[i]);
                if (err == 0)
                {
                    parsedLedState = parsedValue;
                }
            }
#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
            else if (strstr(key, "accelUpdate"))
            {
                /* found "updateAccel" keyword, parse value of next token */
                err = parseUInt16Value(&parsedValue, json, &tokens[i]);
                if (err == 0)
                {
                    parsedAccState = parsedValue;
                }
            }
#endif
            i++;
        }
    }
}

static ShadowReturnCode_t prvShadowClientCreateConnect(void)
{
    MQTTAgentConnectParams_t xConnectParams;
    ShadowCreateParams_t xCreateParams;
    ShadowReturnCode_t xReturn;

    xCreateParams.xMQTTClientType = eDedicatedMQTTClient;
    xReturn                       = SHADOW_ClientCreate(&xClientHandle, &xCreateParams);

    if (xReturn == eShadowSuccess)
    {
        memset(&xConnectParams, 0x00, sizeof(xConnectParams));
        xConnectParams.pcURL  = clientcredentialMQTT_BROKER_ENDPOINT;
        xConnectParams.usPort = clientcredentialMQTT_BROKER_PORT;

        xConnectParams.xFlags            = mqttagentREQUIRE_TLS;
        xConnectParams.pcCertificate     = NULL;
        xConnectParams.ulCertificateSize = 0;
        xConnectParams.pxCallback        = NULL;
        xConnectParams.pvUserData        = &xClientHandle;

        xConnectParams.pucClientId      = (const uint8_t *)(clientcredentialIOT_THING_NAME);
        xConnectParams.usClientIdLength = (uint16_t)strlen(clientcredentialIOT_THING_NAME);
        xReturn                         = SHADOW_ClientConnect(xClientHandle, &xConnectParams, shadowDemoTIMEOUT);

        if (xReturn != eShadowSuccess)
        {
            configPRINTF(("Shadow_ClientConnect unsuccessful, returned %d.\r\n", xReturn));
        }
    }
    else
    {
        configPRINTF(("Shadow_ClientCreate unsuccessful, returned %d.\r\n", xReturn));
    }

    return xReturn;
}

void prvShadowMainTask(void *pvParameters)
{
    (void)pvParameters;

    /* Initialize common libraries required by demo. */
    if (IotSdk_Init() != true)
    {
        configPRINTF(("Failed to initialize the common library."));
        vTaskDelete(NULL);
    }

    jsonDeltaQueue = xQueueCreate(8, sizeof(jsonDelta_t));
    if (jsonDeltaQueue == NULL)
    {
        configPRINTF(("Failed to create jsonDeltaQueue queue.\r\n"));
        vTaskDelete(NULL);
    }

    if (prvShadowClientCreateConnect() != eShadowSuccess)
    {
        configPRINTF(("Failed to initialize, stopping demo.\r\n"));
        vTaskDelete(NULL);
    }

    ShadowOperationParams_t xOperationParams;
    xOperationParams.pcThingName = shadowDemoTHING_NAME;
    xOperationParams.xQoS        = eMQTTQoS0;
    xOperationParams.pcData      = NULL;
    /* Don't keep subscriptions, since SHADOW_Delete is only called here once. */
    xOperationParams.ucKeepSubscriptions = pdFALSE;

    ShadowReturnCode_t xReturn;

    /* Delete the device shadow before initial update */
    xReturn = SHADOW_Delete(xClientHandle, &xOperationParams, shadowDemoTIMEOUT);

    /* Atttempting to delete a non-existant shadow returns eShadowRejectedNotFound.
     * Either eShadowSuccess or eShadowRejectedNotFound signify that there's no
     * existing Thing Shadow, so both values are ok. */
    if ((xReturn != eShadowSuccess) && (xReturn != eShadowRejectedNotFound))
    {
        configPRINTF(("Failed to delete device shadow, stopping demo.\r\n"));
        vTaskDelete(NULL);
    }

    /* Register callbacks. This demo doesn't use updated or deleted callbacks, so
     * those members are set to NULL. The callbacks are registered after deleting
     * the Shadow so that any previous Shadow doesn't unintentionally trigger the
     * delta callback.*/
    ShadowCallbackParams_t xCallbackParams;
    xCallbackParams.pcThingName            = shadowDemoTHING_NAME;
    xCallbackParams.xShadowUpdatedCallback = NULL;
    xCallbackParams.xShadowDeletedCallback = NULL;
    xCallbackParams.xShadowDeltaCallback   = prvDeltaCallback;

    xReturn = SHADOW_RegisterCallbacks(xClientHandle, &xCallbackParams, shadowDemoTIMEOUT);
    if (xReturn != eShadowSuccess)
    {
        configPRINTF(("Failed to register the delta callback, stopping demo.\r\n"));
        vTaskDelete(NULL);
    }

    xOperationParams.pcData       = pcUpdateBuffer;
    xOperationParams.ulDataLength = prvGenerateShadowJSON();
    /* Keep subscriptions across multiple calls to SHADOW_Update. */
    xOperationParams.ucKeepSubscriptions = pdTRUE;

    /* create initial shadow document for the thing */
    if (SHADOW_Update(xClientHandle, &xOperationParams, shadowDemoTIMEOUT) != eShadowSuccess)
    {
        configPRINTF(("Failed to update device shadow, stopping demo.\r\n"));
        vTaskDelete(NULL);
    }

    configPRINTF(("AWS Remote Control Demo initialized.\r\n"));
    configPRINTF(("Use mobile application to control the remote device.\r\n"));

    jsonDelta_t jsonDelta;

    for (;;)
    {
        /* process delta shadow JSON received in prvDeltaCallback() */
        if (xQueueReceive(jsonDeltaQueue, &jsonDelta, portMAX_DELAY) == pdTRUE)
        {
            /* process item from queue */
            processShadowDeltaJSON(jsonDelta.pcDeltaDocument, jsonDelta.ulDocumentLength);
            if (parsedLedState != ledState)
            {
                for (int i = 0; i < ledCount; i++)
                {
                    /* change from 0 to 1 */
                    if (((ledState & (1 << i)) == 0) && ((parsedLedState & (1 << i)) != 0))
                    {
                        /* turn on led */
                        configPRINTF(("Turn on %s\r\n", ledName[i]));
                        turnOnLed(i);
                    }

                    /* change from 1 to 0 */
                    if (((ledState & (1 << i)) != 0) && ((parsedLedState & (1 << i)) == 0))
                    {
                        /* turn off led */
                        configPRINTF(("Turn off %s\r\n", ledName[i]));
                        turnOffLed(i);
                    }
                }
                ledState = parsedLedState;

                /* update device shadow */
                xOperationParams.ulDataLength = prvReportShadowJSON();
                xReturn                       = SHADOW_Update(xClientHandle, &xOperationParams, shadowDemoTIMEOUT);
                if (xReturn == eShadowSuccess)
                {
                    configPRINTF(("Successfully performed update.\r\n"));
                }
                else
                {
                    configPRINTF(("Update failed, returned %d.\r\n", xReturn));
                }
            }
#if defined(BOARD_ACCEL_FXOS) || defined(BOARD_ACCEL_MMA)
            if (parsedAccState == 1)
            {
                configPRINTF(("Update accelerometer.\r\n"));
                xOperationParams.ulDataLength = buildJsonAccel();
                xReturn                       = SHADOW_Update(xClientHandle, &xOperationParams, shadowDemoTIMEOUT);
                if (xReturn == eShadowSuccess)
                {
                    configPRINTF(("Successfully performed update.\r\n"));
                }
                else
                {
                    configPRINTF(("Update failed, returned %d.\r\n", xReturn));
                }
                parsedAccState = 0;
            }
#endif
            /* return mqtt buffer */
            xReturn = SHADOW_ReturnMQTTBuffer(xClientHandle, jsonDelta.xBuffer);
            if (xReturn != eShadowSuccess)
            {
                configPRINTF(("Return MQTT buffer failed, returned %d.\r\n", xReturn));
            }
        }
    }
}

void vStartLedDemoTask(void)
{
    (void)xTaskCreate(prvShadowMainTask, "AWS-RemoteCtrl", DEMO_REMOTE_CONTROL_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY,
                      NULL);
}
