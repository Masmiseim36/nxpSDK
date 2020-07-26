/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "board.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include <string.h>
#include "mbedtls/base64.h"

#include "cJSON.h"

#include "comms_message_handler_fwupdate_common.h"

#if COMMS_MESSAGE_HANDLER_FWUPDATE_SERVER
#include "comms_message_handler_fwupdate_server.h"
#endif

#include "flash_ica_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

static sln_comms_fwupdate_job_desc *currentFwUpdateJob = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static sln_comms_message_status processFwUpdateCommonReq(void *clientInstance, cJSON *json);
static sln_comms_message_status handleClientFwUpdateStartReq(void *clientInstance, cJSON *json);
static sln_comms_message_status processFwUpdateStartReq(void *clientInstance, cJSON *json);
static sln_comms_message_status processFwUpdateStateReq(void *clientInstance, cJSON *json);

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
sln_comms_fwupdate_job_desc *getCurrentFwUpdateJob(void)
{
    return currentFwUpdateJob;
}

sln_comms_message_status processFwUpdateReq(void *clientInstance, cJSON *json)
{
    cJSON *messageType;
    cJSON *messageContents;
    sln_comms_message_status status = kComms_Success;

    messageContents = cJSON_GetObjectItemCaseSensitive(json, "fwupdate_message");

    messageType = cJSON_GetObjectItemCaseSensitive(messageContents, "messageType");

    if (!cJSON_IsNumber(messageType))
    {
        status = kComms_InvalidParameter;
    }

    if (status == kComms_Success)
    {
        /* Switch statement to pass to the correct module message handler */
        switch (messageType->valueint)
        {
            case kCommsFwUpdateCommonMsg:
                status = processFwUpdateCommonReq(clientInstance, messageContents);
                break;

#if COMMS_MESSAGE_HANDLER_FWUPDATE_SERVER
            case kCommsFwUpdateServerMsg:
                status = processFwUpdateServerReq(clientInstance, messageContents);
                break;
#endif
            default:
                status = kComms_UnknownMessage;
                break;
        }
    }

    return status;
}

/*******************************************************************************
 * Static Functions
 ******************************************************************************/

/**
 * @brief Process the incoming common FwUpdate message
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload to be processed
 *
 * @return          sln_comms_message_status
 */
static sln_comms_message_status processFwUpdateCommonReq(void *clientInstance, cJSON *json)
{
    cJSON *messageType;
    cJSON *messageContents;
    sln_comms_message_status status = kComms_Success;

    /* Get the message type */
    messageContents = cJSON_GetObjectItemCaseSensitive(json, "fwupdate_common_message");
    messageType     = cJSON_GetObjectItemCaseSensitive(messageContents, "messageType");

    if (!cJSON_IsNumber(messageType))
    {
        status = kComms_InvalidParameter;
    }

    if (status == kComms_Success)
    {
        /* Switch statement for handling specific Common FwUpdate message */
        switch (messageType->valueint)
        {
            case kCommsFwUpdateStart:
                status = processFwUpdateStartReq(clientInstance, messageContents);
                break;

            case kCommsFwUpdateState:
                status = processFwUpdateStateReq(clientInstance, messageContents);
                break;
        }
    }

    return status;
}

/**
 * @brief Process the incoming FwUpdate State JSON message
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload to be processed
 *
 * @return          sln_comms_message_status
 */
static sln_comms_message_status processFwUpdateStateReq(void *clientInstance, cJSON *json)
{
    cJSON *jsonMessage, *jsonFwUpdateReqState, *jsonErrorState;
    sln_comms_message_status status = kComms_Success;

    jsonMessage = cJSON_CreateObject();

    if (kComms_Success == status)
    {
        /* TODO: Not implemented yet, need to grab FwUpdate status from descriptor */
        if (NULL != getCurrentFwUpdateJob())
        {
            jsonFwUpdateReqState = cJSON_CreateBool(getCurrentFwUpdateJob()->status);
            cJSON_AddItemToObject(jsonMessage, "status", jsonFwUpdateReqState);
            jsonErrorState = cJSON_CreateNumber(status);

            cJSON_AddItemToObject(jsonMessage, "error", jsonErrorState);
            status = SLN_COMMS_MESSAGE_Send(clientInstance, jsonMessage);
        }
        else
        {
            status = kComms_FailedProcessing;
        }
    }

    /* Free the memory */
    cJSON_Delete(jsonMessage);

    return status;
}

/**
 * @brief Process the incoming FwUpdate Client start request
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload to be processed
 *
 * @return          sln_comms_message_status
 */
static sln_comms_message_status handleClientFwUpdateStartReq(void *clientInstance, cJSON *json)
{
    cJSON *jsonMessage, *jsonFwUpdateStartStatus;
    sln_comms_message_status status = kComms_Success;
    int32_t fica_status             = SLN_FLASH_NO_ERROR;

    if (kComms_Success == status)
    {
        /* Set the FwUpdate bit in the flash memory */
        fica_status = FICA_set_comm_flag(FICA_COMM_FWUPDATE_BIT);
        if (fica_status == SLN_FLASH_NO_ERROR)
        {
            configPRINTF(("Success: Cleared FWUPDATE bit in FICA\r\n"));
        }
        else
        {
            status = kComms_FailedProcessing;
        }
    }

    if (kComms_Success == status)
    {
        jsonMessage = cJSON_CreateObject();

        /* Prepare the response message */
        jsonFwUpdateStartStatus = cJSON_CreateNumber(status);
        cJSON_AddItemToObject(jsonMessage, "error", jsonFwUpdateStartStatus);

        status = SLN_COMMS_MESSAGE_Send(clientInstance, jsonMessage);

        /* Free the memory */
        cJSON_Delete(jsonMessage);

        /* Need to start the FwUpdate which requires a reboot into the bootloader */
        if (kComms_Success == status)
        {
            /* reset MCU */
            configPRINTF(("FwUpdate Restart now\r\n"));

            /* give time to logging task to print last logs */
            vTaskDelay(1000);

            NVIC_SystemReset();
        }
    }

    return status;
}

/**
 * @brief Process the incoming Generate FwUpdate start request
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload to be processed
 *
 * @return          sln_comms_message_status
 */
static sln_comms_message_status processFwUpdateStartReq(void *clientInstance, cJSON *json)
{
    cJSON *jsonMessage, *jsonFwUpdateStartReq, *jsonFwUpdateStartStatus;
    sln_comms_message_status status = kComms_Success;
    int32_t fica_status             = SLN_FLASH_NO_ERROR;

    jsonMessage = cJSON_CreateObject();

    if (kComms_Success == status)
    {
        /* If an FwUpdate start already came in, ignore */
        if (currentFwUpdateJob != NULL)
        {
            status = kComms_AlreadyInProgress;
        }
    }

    if (kComms_Success == status)
    {
        /* Create a new FwUpdate descriptor */
        currentFwUpdateJob = pvPortMalloc(sizeof(sln_comms_fwupdate_job_desc));
        if (NULL == currentFwUpdateJob)
        {
            status = kComms_FailedProcessing;
        }
    }

    /* Parse the Job ID */
    if (kComms_Success == status)
    {
        jsonFwUpdateStartReq = cJSON_GetObjectItemCaseSensitive(json, "job_id");
        if (cJSON_IsString(jsonFwUpdateStartReq) && jsonFwUpdateStartReq->valuestring != NULL)
        {
            configPRINTF(("Found element 'job_id': %s\r\n", jsonFwUpdateStartReq->valuestring));
            currentFwUpdateJob->jobId = (char *)pvPortMalloc(strlen(jsonFwUpdateStartReq->valuestring));

            if (NULL == currentFwUpdateJob->jobId)
            {
                status = kComms_FailedProcessing;
            }
            if (kComms_Success == status)
            {
                memcpy(currentFwUpdateJob->jobId, jsonFwUpdateStartReq->valuestring,
                       strlen(jsonFwUpdateStartReq->valuestring));
            }
        }
        else
        {
            status = kComms_InvalidParameter;
        }
    }

    /* Parse the bank in which the image will be programmed */
    if (kComms_Success == status)
    {
        jsonFwUpdateStartReq = cJSON_GetObjectItemCaseSensitive(json, "app_bank_type");
        if (cJSON_IsNumber(jsonFwUpdateStartReq))
        {
            int32_t imageType = FICA_IMG_TYPE_NONE;

            configPRINTF(("Found element 'app_bank_type': %d\r\n", jsonFwUpdateStartReq->valueint));

            status = FICA_GetCurAppStartType(&imageType);

            /* Only accept if it's not the current bank */
            if ((SLN_FLASH_NO_ERROR == status) && (jsonFwUpdateStartReq->valueint > FICA_IMG_TYPE_NONE) &&
                (jsonFwUpdateStartReq->valueint < FICA_NUM_IMG_TYPES) && (jsonFwUpdateStartReq->valueint != imageType))
            {
                currentFwUpdateJob->appBankType = jsonFwUpdateStartReq->valueint;
            }
            else
            {
                status = kComms_InvalidParameter;
            }
        }
        else
        {
            status = kComms_InvalidParameter;
        }
    }

    /* Parse the signature */
    if (kComms_Success == status)
    {
        jsonFwUpdateStartReq = cJSON_GetObjectItemCaseSensitive(json, "signature");
        if (cJSON_IsString(jsonFwUpdateStartReq) && jsonFwUpdateStartReq->valuestring != NULL)
        {
            configPRINTF(("Found element 'signature': %s\r\n", jsonFwUpdateStartReq->valuestring));

            if (NULL == currentFwUpdateJob->signature)
            {
                status = kComms_FailedProcessing;
            }
            if (kComms_Success == status)
            {
                uint32_t decodeSize = 0;
                uint32_t mbedStatus = 0;

                /* Decode the signature as it comes encoded based on Ivaldi signing process */
                mbedStatus =
                    mbedtls_base64_decode(currentFwUpdateJob->signature, COMMS_FWUPDATE_MAX_SIGNATURE_SIZE, &decodeSize,
                                          (const unsigned char *)jsonFwUpdateStartReq->valuestring,
                                          strlen(jsonFwUpdateStartReq->valuestring));

                if (mbedStatus != 0)
                {
                    status = kComms_InvalidParameter;
                }
            }
        }
        else
        {
            status = kComms_InvalidParameter;
        }
    }

    /* Parse the image size */
    if (kComms_Success == status)
    {
        jsonFwUpdateStartReq = cJSON_GetObjectItemCaseSensitive(json, "image_size");
        if (cJSON_IsNumber(jsonFwUpdateStartReq))
        {
            uint32_t maxSize     = 0;
            uint32_t fica_status = SLN_FLASH_NO_ERROR;
            configPRINTF(("Found element 'image_size': %i\r\n", jsonFwUpdateStartReq->valueint));

            fica_status = FICA_get_app_img_max_size(currentFwUpdateJob->appBankType, &maxSize);

            /* Fail if it failed to get the max size */
            if (SLN_FLASH_NO_ERROR != fica_status)
            {
                status = kComms_FailedProcessing;
            }

            /* Fail if it exceeds the maximum size */
            if (kComms_Success == status)
            {
                if ((jsonFwUpdateStartReq->valueint <= maxSize) && (jsonFwUpdateStartReq->valueint >= 0))
                {
                    currentFwUpdateJob->imageSize = jsonFwUpdateStartReq->valueint;
                }
                else
                {
                    status = kComms_InvalidParameter;
                }
            }
        }
        else
        {
            status = kComms_InvalidParameter;
        }
    }

    /* If we are running from the main app, run the client FwUpdate handle
     * If we are running from the bootloader and FwUpdate server is supported, got to FwUpdate start handle
     */
    if (kComms_Success == status)
    {
        int32_t imageType          = FICA_IMG_TYPE_NONE;
        uint32_t currentImgAddress = SCB->VTOR - FlexSPI_AMBA_BASE;
        fica_status                = FICA_GetImgTypeFromAddr(currentImgAddress, &imageType);

        if (SLN_FLASH_NO_ERROR != fica_status)
        {
            status = kComms_FailedProcessing;
        }

        if (kComms_Success == status)
        {
            if (currentFwUpdateJob->appBankType == imageType)
            {
                status = kComms_InvalidParameter;
            }
            else
            {
                if ((FICA_IMG_TYPE_APP_A == imageType) || (FICA_IMG_TYPE_APP_B == imageType))
                {
                    status = handleClientFwUpdateStartReq(clientInstance, json);
                }
                else
                {
#if COMMS_MESSAGE_HANDLER_FWUPDATE_SERVER
                    status = handleServerFwUpdateStartReq(currentFwUpdateJob->appBankType);
#else
                    status = kComms_InvalidParameter;
#endif
                }
            }
        }
    }

    if ((kComms_Success != status) && (currentFwUpdateJob != NULL))
    {
        if (kComms_AlreadyInProgress != status)
        {
            vPortFree(currentFwUpdateJob);
            currentFwUpdateJob = NULL;
        }
    }

    if (kComms_Success == status)
    {
        /* Send response */
        jsonFwUpdateStartStatus = cJSON_CreateNumber(status);

        cJSON_AddItemToObject(jsonMessage, "error", jsonFwUpdateStartStatus);

        status = SLN_COMMS_MESSAGE_Send(clientInstance, jsonMessage);

        /* Free the memory */
        cJSON_Delete(jsonMessage);

        if (kComms_Success != status)
        {
            configPRINTF(("Failed to send comms message\r\n"));
            vPortFree(currentFwUpdateJob);
        }
    }

    return status;
}
