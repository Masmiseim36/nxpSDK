/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include <string.h>

#include "cJSON.h"

#include "comms_message_handler_cfg.h"
#include "comms_message_handler.h"

#if COMMS_MESSAGE_HANDLER_AIS_SUPPORTED
#include "comms_message_handler_ais.h"
#endif

#if COMMS_MESSAGE_HANDLER_FWUPDATE
#include "comms_message_handler_fwupdate_common.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

sln_common_connection_desc interfaceDesc;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static sln_comms_message_status parseCommsMessage(sln_common_connection_recv_context *readContext, cJSON *json);
static sln_comms_message_status sendErrorStatus(sln_common_connection_recv_context *readContext,
                                                sln_comms_message_status statusCode);

/*******************************************************************************
 * Functions
 ******************************************************************************/
void SLN_COMMS_MESSAGE_Init(sln_common_interface_type interfaceType)
{
    cJSON_Hooks hooks;
    sln_common_connection_message_status commonConnStatus = kCommon_Success;

    interfaceDesc.recv_cb  = &SLN_COMMS_MESSAGE_Process;
    interfaceDesc.connType = interfaceType;

    /* Initialize cJSON library to use FreeRTOS heap memory management. */
    hooks.malloc_fn = pvPortMalloc;
    hooks.free_fn   = vPortFree;
    cJSON_InitHooks(&hooks);

    commonConnStatus = SLN_CommonConnectionHandlerInit(&interfaceDesc);
    configPRINTF(("Comms init return %d\r\n", commonConnStatus));
}

sln_common_connection_message_status SLN_COMMS_MESSAGE_Process(sln_common_connection_recv_context *readContext)
{
    sln_comms_message_status status = kComms_Success;
    cJSON *json;

    json = cJSON_Parse((const char *)readContext->data);

    if (!json)
    {
        status = kComms_FailedParsing;
    }

    if (status == kComms_Success)
    {
        status = parseCommsMessage(readContext, json);
    }

    if (status != kComms_Success)
    {
        /* Send error back */
        status = sendErrorStatus(readContext, status);
    }

    /* Free heap memory allocated with cJSON_Parse() */
    cJSON_Delete(json);

    return status;
}

sln_comms_message_status SLN_COMMS_MESSAGE_Send(sln_common_connection_recv_context *readContext, cJSON *jsonMessage)
{
    char *jsonStr;
    sln_comms_message_status status                  = kComms_Success;
    sln_common_connection_message_status commsStatus = kCommon_Success;
    sln_common_connection_write_context writeContext;
    uint32_t size = 0;

    jsonStr = cJSON_PrintUnformatted(jsonMessage);

    if (jsonStr == NULL)
    {
        status = kComms_FailedProcessing;
    }

    if (kComms_Success == status)
    {
        size = (uint32_t)strlen(jsonStr);

        writeContext.connContext = readContext->connContext;
        writeContext.data        = (uint8_t *)jsonStr;
        writeContext.len         = size;
        commsStatus              = interfaceDesc.write(&writeContext);

        if (commsStatus != kCommon_Success)
        {
            status = kComms_FailedToTransmit;
        }
    }

    vPortFree(jsonStr);

    return status;
}
/*******************************************************************************
 * Private Functions
 ******************************************************************************/
/**
 * @brief Assemble and send the error status
 *
 * @param readContext: Incoming context containing the received data and the connection context
 * @param statusCode: The error code to be send
 *
 * @return sln_comms_message_status
 */
static sln_comms_message_status sendErrorStatus(sln_common_connection_recv_context *readContext,
                                                sln_comms_message_status statusCode)
{
    cJSON *jsonMessage, *jsonErrorState;
    sln_comms_message_status status = kComms_Success;

    jsonMessage    = cJSON_CreateObject();
    jsonErrorState = cJSON_CreateNumber(statusCode);

    cJSON_AddItemToObject(jsonMessage, "error", jsonErrorState);

    if (kComms_Success == status)
    {
        status = SLN_COMMS_MESSAGE_Send(readContext, jsonMessage);
    }

    cJSON_Delete(jsonMessage);

    return status;
}

/**
 * @brief Parse the incoming Comms message
 *
 * @param readContext: Incoming context containing the received data and the connection context
 * @param json: The payload received
 *
 * @return sln_comms_message_status
 */
static sln_comms_message_status parseCommsMessage(sln_common_connection_recv_context *readContext, cJSON *json)
{
    cJSON *messageType;
    sln_comms_message_status status = kComms_Success;

    messageType = cJSON_GetObjectItemCaseSensitive(json, "messageType");

    if (!cJSON_IsNumber(messageType))
    {
        status = kComms_InvalidParameter;
    }

    if (status == kComms_Success)
    {
        switch (messageType->valueint)
        {
#if COMMS_MESSAGE_HANDLER_AIS_SUPPORTED
            case kCommsAis:
                status = processAisReq(readContext, json);
                break;
#endif
#if COMMS_MESSAGE_HANDLER_FWUPDATE
            case kCommsFwUpdate:
                status = processFwUpdateReq(readContext, json);
                break;
#endif
            default:
                status = kComms_UnknownMessage;
                break;
        }
    }

    return status;
}
