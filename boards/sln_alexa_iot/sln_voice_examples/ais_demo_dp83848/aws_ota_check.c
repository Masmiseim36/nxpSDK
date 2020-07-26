/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
Amazon FreeRTOS OTA Agent V0.9.5
Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 http://aws.amazon.com/freertos
 http://www.FreeRTOS.org
*/

/* Standard library includes. */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "app.h"
#include "board.h"

/* OTA check includes. */
#include "aws_ota_check.h"
#include "sln_flash.h"
#include "flexspi_hyper_flash_ops.h"

/* MQTT includes. */
#include "mqtt_connection.h"

#include "cJSON.h"

/* General constants. */
#define OTA_PUBLISH_WAIT_TICKS pdMS_TO_TICKS(10000UL)
#define OTA_SUBSCRIBE_WAIT_TICKS pdMS_TO_TICKS(30000UL)
#define OTA_UNSUBSCRIBE_WAIT_TICKS pdMS_TO_TICKS(10000UL)
#define OTA_MAX_TOPIC_LEN 256U /* Max length of a dynamically generated topic string (usually on the stack). */
#define OTA_MAX_MSG_LEN 512U
#define U32_MAX_PLACES 10U /* Maximum number of output digits of an unsigned long value. */
#define OTA_MAX_JOBID_LEN 100U
#define OTA_MAX_FILEPATH_LEN 10U
#define OTA_STATUS_MSG_MAX_SIZE 128U /* Max length of a job status message to the service. */

/* Topic strings used by the OTA process. */
/* These first few are topic extensions to the dynamic base topic that includes the Thing name. */
static const char pcOTA_JobsGetNextAccepted_TopicTemplate[] = "$aws/things/%s/jobs/$next/get/accepted";
static const char pcOTA_JobsNotifyNext_TopicTemplate[]      = "$aws/things/%s/jobs/notify-next";
static const char pcOTA_JobsGetNext_TopicTemplate[]         = "$aws/things/%s/jobs/$next/get";
static const char pcOTA_GetNextJob_MsgTemplate[]            = "{\"clientToken\":\"%u:%s\"}";
static const char pcOTA_JobStatus_StatusTemplate[]          = "{\"status\":\"%s\",\"statusDetails\":{";
static const char pcOTA_JobStatus_ReasonStrTemplate[]       = "\"reason\":\"%s: 0x%08x\"}}";
static const char pcOTA_JobStatus_TopicTemplate[]           = "$aws/things/%s/jobs/%s/update";
#define kOTA_Err_UserAbort 0x28000000UL

typedef enum __ota_app_location
{
    eOTA_AppA,
    eOTA_AppB,
    eOTA_Unknown
} ota_app_location;

static ota_app_location ota_new_app_location     = eOTA_Unknown;
static char pcOTA_JobId[OTA_MAX_JOBID_LEN]       = "";
static char pcOTA_Filepath[OTA_MAX_FILEPATH_LEN] = "";
static ota_handle_t *ota_handle                  = NULL;
static TaskHandle_t s_OTATaskHandle              = NULL;

static MQTTAgentReturnCode_t prvPublishMessage(
    const char *const pacTopic, uint16_t usTopicLen, char *pcMsg, uint32_t ulMsgSize, MQTTQoS_t eQOS);
static status_t OTA_Subscribe(const char *pcThingName);
static status_t OTA_Unsubscribe(const char *pcThingName);
static status_t OTA_CheckForUpdate(const char *pcThingName);

/* Remove the trailing '=' sign from the thing name */
static void truncate_at_equal_sign(char *my_string)
{
    const char equal = '='; // character to search
    char *found;
    found = strchr(my_string, equal);
    if (found)
    {
        /* replace '=' with null character '\0' */
        *found = '\0';
    }
}

/* Clear the OTA bit in the flash */
static int32_t fica_clear_ota_bit()
{
    uint8_t current_ota_bit;
    int32_t ret = -1;
    uint32_t config;
    uint8_t tempPage[FLASH_PAGE_SIZE];

    ret = SLN_Read_Flash_At_Address(FICA_START_ADDRESS, tempPage, FLASH_PAGE_SIZE);
    if (ret != 0)
        return -1;
    config          = *(uint32_t *)(tempPage + FICA_OFFSET_ICA_COMM);
    current_ota_bit = ((config & FICA_COMM_AIS_OTA_BIT) > 0); // 0 or 1
    /* check if the OTA bit is already 0 */
    if (current_ota_bit == 0)
        return 0;
    /* set OTA bit to 0 */
    config &= ~FICA_COMM_AIS_OTA_BIT;
    *(uint32_t *)(tempPage + FICA_OFFSET_ICA_COMM) = config;
    ret                                            = SLN_Write_Flash_At_Address(FICA_START_ADDRESS, tempPage);
    if (ret != 0)
        return -1;

    return 0;
}

/* Check the JSON format is a valid content for OTA */
static bool is_valid_ota_json_message(const char *pcMsg)
{
    bool is_valid = false;

    //    const cJSON *json_token = NULL;
    const cJSON *json_execution   = NULL;
    const cJSON *json_jobId       = NULL;
    const cJSON *json_status      = NULL;
    const cJSON *json_jobDocument = NULL;
    const cJSON *json_afr_ota     = NULL;
    const cJSON *json_files       = NULL;
    const cJSON *json_file_item   = NULL;
    const cJSON *json_filepath    = NULL;
    cJSON *json_message           = cJSON_Parse(pcMsg);
    if (json_message == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            configPRINTF(("Error at cJSON_Parse: %s\r\n", error_ptr));
            goto exit;
        }
    }
    json_execution = cJSON_GetObjectItemCaseSensitive(json_message, "execution");
    if (cJSON_IsObject(json_execution))
    {
        configPRINTF(("Found element 'execution'\r\n"));
    }
    else
    {
        configPRINTF(("Did not find 'execution' element in amazon reply\r\n"));
        goto exit;
    }
    json_jobId = cJSON_GetObjectItemCaseSensitive(json_execution, "jobId");
    if (cJSON_IsString(json_jobId) && json_jobId->valuestring != NULL)
    {
        configPRINTF(("Found element 'jobId': %s\r\n", json_jobId->valuestring));
    }
    else
    {
        configPRINTF(("Did not find 'status' element in amazon reply\r\n"));
        goto exit;
    }
    json_status = cJSON_GetObjectItemCaseSensitive(json_execution, "status");
    if (cJSON_IsString(json_status) && json_status->valuestring != NULL)
    {
        configPRINTF(("Found element 'status': %s\r\n", json_status->valuestring));
        /* check the status value */
        const char *STATUS_QUEUED      = "QUEUED";
        const char *STATUS_IN_PROGRESS = "IN_PROGRESS";
        if (strcmp(json_status->valuestring, STATUS_QUEUED) != 0 &&
            strcmp(json_status->valuestring, STATUS_IN_PROGRESS) != 0)
        {
            configPRINTF(("Element 'status' is not %s or %s\r\n", STATUS_QUEUED, STATUS_IN_PROGRESS));
            goto exit;
        }
    }
    else
    {
        configPRINTF(("Did not find 'status' element in amazon reply\r\n"));
        goto exit;
    }
    json_jobDocument = cJSON_GetObjectItemCaseSensitive(json_execution, "jobDocument");
    if (cJSON_IsObject(json_jobDocument))
    {
        configPRINTF(("Found element 'jobDocument'\r\n"));
    }
    else
    {
        configPRINTF(("Did not find 'jobDocument' element in amazon reply\r\n"));
        goto exit;
    }
    json_afr_ota = cJSON_GetObjectItemCaseSensitive(json_jobDocument, "afr_ota");
    if (cJSON_IsObject(json_afr_ota))
    {
        configPRINTF(("Found element 'afr_ota'\r\n"));
        is_valid = true;
        strncpy(pcOTA_JobId, json_jobId->valuestring, OTA_MAX_JOBID_LEN);
        pcOTA_JobId[OTA_MAX_JOBID_LEN - 1] = 0;
    }
    else
    {
        configPRINTF(("Did not find 'afr_ota' element in amazon reply\r\n"));
        goto exit;
    }
    json_files = cJSON_GetObjectItemCaseSensitive(json_afr_ota, "files");
    if (cJSON_IsArray(json_files))
    {
        configPRINTF(("Found element 'files'\r\n"));
    }
    else
    {
        configPRINTF(("Did not find 'files' element in amazon reply\r\n"));
        goto exit;
    }
    json_file_item = cJSON_GetArrayItem(json_files, 0);
    json_filepath  = cJSON_GetObjectItemCaseSensitive(json_file_item, "filepath");
    if (cJSON_IsString(json_filepath) && json_filepath->valuestring != NULL)
    {
        configPRINTF(("Found element 'filepath': %s\r\n", json_filepath->valuestring));
        /* OTA job is considered valid */
        is_valid = true;
        strncpy(pcOTA_JobId, json_jobId->valuestring, OTA_MAX_JOBID_LEN);
        pcOTA_JobId[OTA_MAX_JOBID_LEN - 1] = 0;
        strncpy(pcOTA_Filepath, json_filepath->valuestring, OTA_MAX_FILEPATH_LEN);
        pcOTA_Filepath[OTA_MAX_FILEPATH_LEN - 1] = 0;
    }
    else
    {
        configPRINTF(("Did not find 'filepath' element in amazon reply\r\n"));
        goto exit;
    }

exit:
    /* cleanup */
    cJSON_Delete(json_message);
    return is_valid;
}

static void sendRejectOtaJobStatus()
{
    uint32_t ulTopicLen, ulMsgSize;
    MQTTAgentReturnCode_t eResult = eMQTTAgentSuccess;
    MQTTQoS_t eQOS;
    char pcMsg[OTA_STATUS_MSG_MAX_SIZE];
    char pcTopicBuffer[OTA_MAX_TOPIC_LEN];

    /* All job state transitions except streaming progress use QOS 1 since it is required to have status in the job
     * document. */
    eQOS = eMQTTQoS1;

    /* A message size of zero means don't publish anything. */
    ulMsgSize = 0UL;

    ulMsgSize = (uint32_t)snprintf(pcMsg, sizeof(pcMsg), pcOTA_JobStatus_StatusTemplate, "REJECTED");

    ulMsgSize += (uint32_t)snprintf(&pcMsg[ulMsgSize], sizeof(pcMsg) - ulMsgSize, pcOTA_JobStatus_ReasonStrTemplate,
                                    "rejected", kOTA_Err_UserAbort);

    if (ulMsgSize > 0UL)
    {
        /* Try to build the dynamic job status topic . */
        ulTopicLen = (uint32_t)snprintf(pcTopicBuffer, sizeof(pcTopicBuffer), pcOTA_JobStatus_TopicTemplate,
                                        ota_handle->thingName, pcOTA_JobId);

        /* If the topic name was built, try to publish the status message to it. */
        if ((ulTopicLen > 0UL) && (ulTopicLen < sizeof(pcTopicBuffer)))
        {
            configPRINTF(("Msg: %s\r\n", pcMsg));
            eResult = prvPublishMessage(pcTopicBuffer, (uint16_t)ulTopicLen, &pcMsg[0], ulMsgSize, eQOS);

            if (eResult != eMQTTAgentSuccess)
            {
                configPRINTF(("Failed: %s\r\n", pcTopicBuffer));
            }
            else
            {
                configPRINTF(("'%s' to %s\r\n", "REJECTED", pcTopicBuffer));
            }
        }
        else
        {
            configPRINTF(("Failed to build job status topic!\r\n"));
        }
    }
    else
    {
        /* Just ignore the zero length message. */
    }
}

/* This function is called whenever we receive a MQTT publish message on one of our OTA topics. */
static MQTTBool_t prvOTAPublishCallback(void *pvCallbackContext, const MQTTPublishData_t *const pxPublishData)
{
    DEFINE_OTA_METHOD_NAME("OTA_Cb");
    int32_t ret;

    configPRINTF(("[%s] MQTT reply = %.*s\r\n", OTA_METHOD_NAME, pxPublishData->usTopicLength,
                  (const char *)pxPublishData->pucTopic));

    if (is_valid_ota_json_message((const char *)pxPublishData->pvData))
    {
        configPRINTF(
            ("[%s] Valid job format: jobId = %s, filepath = %s\r\n", OTA_METHOD_NAME, pcOTA_JobId, pcOTA_Filepath));
        if (strcmp(pcOTA_Filepath, JSON_FILEPATH_APPA) == 0)
        {
            ota_new_app_location = eOTA_AppA;
        }
        else if (strcmp(pcOTA_Filepath, JSON_FILEPATH_APPB) == 0)
        {
            ota_new_app_location = eOTA_AppB;
        }
        else
        {
            ota_new_app_location = eOTA_Unknown;
            configPRINTF(("[%s] Invalid 'filepath'\r\n", OTA_METHOD_NAME));
        }

        if (ota_new_app_location != eOTA_Unknown)
        {
            /* Find the current running bank by checking the ResetISR Address in the vector table (which is loaded into
             * DTC) */
            uint32_t runningFromBankA =
                (((*(uint32_t *)(APPLICATION_RESET_ISR_ADDRESS)) & APP_VECTOR_TABLE_APP_A) == APP_VECTOR_TABLE_APP_A);
            uint32_t runningFromBankb =
                (((*(uint32_t *)(APPLICATION_RESET_ISR_ADDRESS)) & APP_VECTOR_TABLE_APP_B) == APP_VECTOR_TABLE_APP_B);

            /* check address of current application from vector table */
            if ((runningFromBankA && ota_new_app_location == eOTA_AppB) ||
                (runningFromBankb && ota_new_app_location == eOTA_AppA))
            {
                /* clear the OTA update indicator in flash memory */
                ret = fica_clear_ota_bit();
                if (ret != 0)
                {
                    configPRINTF(("[%s] Failed: clear OTA bit in FICA\r\n", OTA_METHOD_NAME));
                }
                else
                {
                    /* reset MCU */
                    configPRINTF(("[%s] Restart now\r\n", OTA_METHOD_NAME));

                    /* give time to logging task to print last logs */
                    vTaskDelay(1000);

                    NVIC_SystemReset();
                }
            }
            else
            {
                configPRINTF(("[%s] Invalid %s target location\r\n", OTA_METHOD_NAME, pcOTA_Filepath));
                vTaskResume(s_OTATaskHandle);
            }
        }
    }
    return eMQTTFalse;
}

status_t OTA_Init(ota_handle_t *pOtaHandle, const char *pcThingName)
{
    DEFINE_OTA_METHOD_NAME("OTA_Init");

    configPRINTF(("[%s] Start init\r\n", OTA_METHOD_NAME));

    pOtaHandle->otaInitDone = false;

    /* store the thing name */
    strncpy(pOtaHandle->thingName, (const char *)pcThingName, OTA_MAX_THINGNAME_LEN);
    pOtaHandle->thingName[OTA_MAX_THINGNAME_LEN - 1] = 0;

    truncate_at_equal_sign(pOtaHandle->thingName);

    pOtaHandle->otaInitDone = true;

    ota_handle = pOtaHandle;

    return kStatus_Success;
}

status_t OTA_Connect(ota_handle_t *pOtaHandle)
{
    DEFINE_OTA_METHOD_NAME("OTA_Connect");

    configPRINTF(("[%s] Start connect\r\n", OTA_METHOD_NAME));

    if (!pOtaHandle->otaInitDone)
    {
        configPRINTF(("[%s] Init not done\r\n", OTA_METHOD_NAME));
        return kStatus_Fail;
    }

    /* subscribe to job change related topics */
    if (OTA_Subscribe(pOtaHandle->thingName) != kStatus_Success)
    {
        configPRINTF(("[%s] Failed: OTA subscribe to MQTT topic has failed\r\n", OTA_METHOD_NAME));
        return kStatus_Fail;
    }

    /* check once if a current OTA job is available (pending at start) */
    if (OTA_CheckForUpdate(pOtaHandle->thingName) != kStatus_Success)
    {
        configPRINTF(("[%s] Failed: OTA check for update has failed\r\n", OTA_METHOD_NAME));
        return kStatus_Fail;
    }
    return kStatus_Success;
}

status_t OTA_Disconnect(ota_handle_t *pOtaHandle)
{
    DEFINE_OTA_METHOD_NAME("OTA_Disconnect");

    configPRINTF(("[%s] Start disconnect\r\n", OTA_METHOD_NAME));

    if (!pOtaHandle->otaInitDone)
    {
        configPRINTF(("[%s] Init not done\r\n", OTA_METHOD_NAME));
        return kStatus_Fail;
    }

    /* unsubscribe to job change related topics */
    if (OTA_Unsubscribe(pOtaHandle->thingName) != kStatus_Success)
    {
        configPRINTF(("[%s] Failed: OTA subscribe to MQTT topic has failed\r\n", OTA_METHOD_NAME));
        return kStatus_Fail;
    }

    return kStatus_Success;
}

/*! @brief Subscribe to MQTT topics */
static status_t OTA_Subscribe(const char *pcThingName)
{
    DEFINE_OTA_METHOD_NAME("OTA_Subscribe");

    char pcJobTopic[OTA_MAX_TOPIC_LEN];
    MQTTAgentSubscribeParams_t stJobsSubscription;

    /* Clear subscription struct and set common parameters for job topics used by OTA. */
    memset(&stJobsSubscription, 0, sizeof(stJobsSubscription));
    stJobsSubscription.xQoS     = eMQTTQoS1;
    stJobsSubscription.pucTopic = (const uint8_t *)pcJobTopic; /* Point to local string storage. Built below. */
    stJobsSubscription.pvPublishCallbackContext = (void *)
        eOTA_PubMsgType_Job; /*lint !e923 The publish callback context is implementing data hiding with a void* type.*/
    stJobsSubscription.pxPublishCallback = prvOTAPublishCallback;
    stJobsSubscription.usTopicLength =
        (uint16_t)snprintf(pcJobTopic, /*lint -e586 Intentionally using snprintf. */
                           sizeof(pcJobTopic), pcOTA_JobsGetNextAccepted_TopicTemplate, (const uint8_t *)pcThingName);

    if ((stJobsSubscription.usTopicLength > 0U) && (stJobsSubscription.usTopicLength < sizeof(pcJobTopic)))
    {
        /* Subscribe to the first of two jobs topics. */
        if (MQTT_AGENT_Subscribe(APP_MQTT_GetHandle(), &stJobsSubscription, (TickType_t)OTA_SUBSCRIBE_WAIT_TICKS) !=
            eMQTTAgentSuccess)
        {
            configPRINTF(("[%s] Failed: %s\n\r", OTA_METHOD_NAME, stJobsSubscription.pucTopic));
        }
        else
        {
            configPRINTF(("[%s] OK: %s\n\r", OTA_METHOD_NAME, stJobsSubscription.pucTopic));
            stJobsSubscription.usTopicLength = (uint16_t)snprintf(
                pcJobTopic, /*lint -e586 Intentionally using snprintf. */
                sizeof(pcJobTopic), pcOTA_JobsNotifyNext_TopicTemplate, (const uint8_t *)pcThingName);
            if ((stJobsSubscription.usTopicLength > 0U) && (stJobsSubscription.usTopicLength < sizeof(pcJobTopic)))
            {
                /* Subscribe to the second of two jobs topics. */
                if (MQTT_AGENT_Subscribe(APP_MQTT_GetHandle(), &stJobsSubscription,
                                         (TickType_t)OTA_SUBSCRIBE_WAIT_TICKS) != eMQTTAgentSuccess)
                {
                    configPRINTF(("[%s] Failed: %s\n\r", OTA_METHOD_NAME, stJobsSubscription.pucTopic));
                }
                else
                {
                    configPRINTF(("[%s] OK: %s\n\r", OTA_METHOD_NAME, stJobsSubscription.pucTopic));
                    return kStatus_Success;
                }
            }
        }
    }

    return kStatus_Fail;
}

/*! @brief Unsubscribe from MQTT topics */
static status_t OTA_Unsubscribe(const char *pcThingName)
{
    DEFINE_OTA_METHOD_NAME("OTA_Unsubscribe");

    char pcJobTopic[OTA_MAX_TOPIC_LEN];
    MQTTAgentUnsubscribeParams_t stJobsUnsubscription;

    /* Clear subscription struct and set common parameters for job topics used by OTA. */
    memset(&stJobsUnsubscription, 0, sizeof(stJobsUnsubscription));
    stJobsUnsubscription.pucTopic = (const uint8_t *)pcJobTopic; /* Point to local string storage. Built below. */
    stJobsUnsubscription.usTopicLength =
        (uint16_t)snprintf(pcJobTopic, /*lint -e586 Intentionally using snprintf. */
                           sizeof(pcJobTopic), pcOTA_JobsGetNextAccepted_TopicTemplate, (const uint8_t *)pcThingName);

    if ((stJobsUnsubscription.usTopicLength > 0U) && (stJobsUnsubscription.usTopicLength < sizeof(pcJobTopic)))
    {
        /* Unsubscribe from the first of two jobs topics. */
        if (MQTT_AGENT_Unsubscribe(APP_MQTT_GetHandle(), &stJobsUnsubscription,
                                   (TickType_t)OTA_UNSUBSCRIBE_WAIT_TICKS) != eMQTTAgentSuccess)
        {
            configPRINTF(("[%s] Failed: %s\n\r", OTA_METHOD_NAME, stJobsUnsubscription.pucTopic));
        }
        else
        {
            configPRINTF(("[%s] OK: %s\n\r", OTA_METHOD_NAME, stJobsUnsubscription.pucTopic));
            stJobsUnsubscription.usTopicLength = (uint16_t)snprintf(
                pcJobTopic, /*lint -e586 Intentionally using snprintf. */
                sizeof(pcJobTopic), pcOTA_JobsNotifyNext_TopicTemplate, (const uint8_t *)pcThingName);
            if ((stJobsUnsubscription.usTopicLength > 0U) && (stJobsUnsubscription.usTopicLength < sizeof(pcJobTopic)))
            {
                /* Unsubscribe from the second of two jobs topics. */
                if (MQTT_AGENT_Unsubscribe(APP_MQTT_GetHandle(), &stJobsUnsubscription,
                                           (TickType_t)OTA_UNSUBSCRIBE_WAIT_TICKS) != eMQTTAgentSuccess)
                {
                    configPRINTF(("[%s] Failed: %s\n\r", OTA_METHOD_NAME, stJobsUnsubscription.pucTopic));
                }
                else
                {
                    configPRINTF(("[%s] OK: %s\n\r", OTA_METHOD_NAME, stJobsUnsubscription.pucTopic));
                    return kStatus_Success;
                }
            }
        }
    }

    return kStatus_Fail;
}

/* Request for the next available OTA job from the job service by publishing
 * a "get next job" message to the job service. */
static status_t OTA_CheckForUpdate(const char *pcThingName)
{
    DEFINE_OTA_METHOD_NAME("OTA_CheckForUpdate");
    char pcJobTopic[OTA_MAX_TOPIC_LEN];
    static uint32_t ulReqCounter  = 0;
    MQTTAgentReturnCode_t eResult = eMQTTAgentSuccess;
    uint32_t ulMsgLen;
    uint16_t usTopicLen;
    OTA_Err_t xError = kOTA_Err_Uninitialized;

    /* The following buffer is big enough to hold a dynamically constructed $next/get job message.
     * It contains a client token that is used to track how many requests have been made. */
    char pcMsg[CONST_STRLEN(pcOTA_GetNextJob_MsgTemplate) + U32_MAX_PLACES + OTA_MAX_THINGNAME_LEN];

    configPRINTF(("[%s] Request #%u\r\n", OTA_METHOD_NAME, ulReqCounter));
    /*lint -e586 Intentionally using snprintf. */
    ulMsgLen = (uint32_t)snprintf(pcMsg, sizeof(pcMsg), pcOTA_GetNextJob_MsgTemplate, ulReqCounter, pcThingName);
    ulReqCounter++;
    usTopicLen = (uint16_t)snprintf(pcJobTopic, sizeof(pcJobTopic), pcOTA_JobsGetNext_TopicTemplate, pcThingName);

    if ((usTopicLen > 0U) && (usTopicLen < sizeof(pcJobTopic)))
    {
        eResult = prvPublishMessage(pcJobTopic, usTopicLen, pcMsg, ulMsgLen, eMQTTQoS1);

        if (eResult != eMQTTAgentSuccess)
        {
            configPRINTF(("[%s] Failed to publish MQTT message.\r\n", OTA_METHOD_NAME));
            xError = kOTA_Err_PublishFailed;
        }
        else
        {
            /* Nothing special to do. We succeeded. */
            xError = kOTA_Err_None;
        }
    }
    else
    {
        configPRINTF(("[%s] Topic too large for supplied buffer.\r\n", OTA_METHOD_NAME));
        xError = kOTA_Err_TopicTooLarge;
    }
    if (xError == kOTA_Err_None)
        return kStatus_Success;
    else
        return kStatus_Fail;
}

/* Publish a message to the specified client/topic at the given QOS. */
static MQTTAgentReturnCode_t prvPublishMessage(
    const char *const pacTopic, uint16_t usTopicLen, char *pcMsg, uint32_t ulMsgSize, MQTTQoS_t eQOS)
{
    MQTTAgentReturnCode_t eResult;
    MQTTAgentPublishParams_t xPublishParams;

    xPublishParams.pucTopic      = (const uint8_t *)pacTopic;
    xPublishParams.usTopicLength = usTopicLen;
    xPublishParams.xQoS          = eQOS;
    xPublishParams.pvData        = pcMsg;
    xPublishParams.ulDataLength  = ulMsgSize;

    if (APP_MQTT_Getv2Handle())
    {
        eResult = MQTT_AGENT_Publish(APP_MQTT_GetHandle(), &xPublishParams, (TickType_t)OTA_PUBLISH_WAIT_TICKS);
    }
    else
    {
        eResult = eMQTTAgentFailure;
    }

    return eResult;
}

void ota_task(void *arg)
{
    s_OTATaskHandle = xTaskGetCurrentTaskHandle();

    while (1)
    {
        vTaskSuspend(NULL);

        sendRejectOtaJobStatus();
    }

    vTaskDelete(NULL);
}
