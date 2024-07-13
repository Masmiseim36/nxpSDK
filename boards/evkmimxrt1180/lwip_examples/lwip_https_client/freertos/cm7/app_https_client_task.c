/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

/* App */
#include "app_https_client_task.h"
#include "app_corehttp.h"
#include "app_mbedtls_socket.h"

static void client_task(void *args);

void app_https_client_task_init(https_client_task_context_t *context)
{
    https_client_task_context_t *args;
    BaseType_t result;

    args = (https_client_task_context_t *)pvPortMalloc(sizeof(https_client_task_context_t));
    if (args == NULL)
    {
        PRINTF("Failed to allocate client_task context.\r\n");
        return;
    }

    memcpy(args, context, sizeof(https_client_task_context_t));

    result = xTaskCreate(client_task, "client_task", 4096, args, 4, NULL);
    if (result != pdTRUE)
    {
        vPortFree(args);
        PRINTF("client_task creation failed");
    }
}

static void client_task(void *args)
{
    https_client_task_context_t *context = (https_client_task_context_t *)args;

    assert(context != NULL);
    assert(context->host != NULL);
    assert(context->path != NULL);
    assert(context->port != 0U);
    assert(context->max_request_len > 0U);
    assert(context->max_response_len > 0U);

    /* Create context for underlying transport */

    NetworkContext_t *nwk_ctx = app_corehttp_create_context(context->host, context->port);
    if (nwk_ctx == NULL)
    {
        goto task_end;
    }

    HTTPStatus_t status;

    /* Build request headers */

    HTTPRequestHeaders_t req_headers;
    HTTPRequestInfo_t req_info;

    memset(&req_headers, 0x00, sizeof(req_headers));
    memset(&req_info, 0x00, sizeof(req_info));

    req_headers.pBuffer   = pvPortMalloc(context->max_request_len);
    req_headers.bufferLen = context->max_request_len;

    if (req_headers.pBuffer == NULL)
    {
        PRINTF("Failed to allocate header buffer.\r\n");

        goto destroy_ctx_exit;
    }

    req_info.pMethod   = HTTP_METHOD_GET;
    req_info.methodLen = sizeof(HTTP_METHOD_GET) - 1;
    req_info.pPath     = context->path;
    req_info.pathLen   = strlen(context->path);
    req_info.pHost     = context->host;
    req_info.hostLen   = strlen(context->host);
    req_info.reqFlags  = HTTP_REQUEST_KEEP_ALIVE_FLAG;

    status = HTTPClient_InitializeRequestHeaders(&req_headers, &req_info);
    if (status != HTTPSuccess)
    {
        PRINTF("Failed to build request headers.\r\n");

        goto free_hdr_exit;
    }

    /* Create transport interface */

    TransportInterface_t xport_interface = {
        .send            = app_corehttp_send,
        .recv            = app_corehttp_recv,
        .writev          = NULL,
        .pNetworkContext = nwk_ctx,
    };

    HTTPResponse_t resp;

    memset(&resp, 0x00, sizeof(resp));

    resp.pBuffer   = pvPortMalloc(context->max_response_len);
    resp.bufferLen = context->max_response_len;

    if (resp.pBuffer == NULL)
    {
        PRINTF("Failed to allocate response buffer.\r\n");

        goto free_hdr_exit;
    }

    PRINTF("GET %s\r\n", context->path);
    status = HTTPClient_Send(&xport_interface, &req_headers, NULL, 0, &resp, 0);
    if (status != HTTPSuccess)
    {
        PRINTF("HTTP request encountered error: %d\r\n", status);

        goto free_resp_exit;
    }

    PRINTF("Response code: %d\r\n", resp.statusCode);
    PRINTF("Response body:\r\n");

    for (uint32_t i = 0; i < resp.bodyLen; i++)
    {
        if (resp.pBody[i] == '\n')
        {
            PRINTF("\r");
        }
        PRINTF("%c", resp.pBody[i]);
    }

    PRINTF("\r\n");

free_resp_exit:
    vPortFree(resp.pBuffer);

free_hdr_exit:
    vPortFree(req_headers.pBuffer);

destroy_ctx_exit:
    app_corehttp_destroy_context(nwk_ctx);

task_end:
    vPortFree(context);
    vTaskDelete(NULL);
}
