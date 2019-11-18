/*
 * Copyright (c) 2018, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <string.h>

#include "fsl_common.h"
#include "srtm_auto_service.h"

#include "srtm_heap.h"
#include "srtm_list.h"
#include "srtm_dispatcher.h"
#include "srtm_service.h"
#include "srtm_service_struct.h"
#include "srtm_channel.h"
#include "srtm_channel_struct.h"
#include "srtm_peercore.h"
#include "srtm_peercore_struct.h"
#include "srtm_message.h"
#include "srtm_message_struct.h"
#include "srtm_mutex.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Protocol definition */
#define SRTM_AUTO_CATEGORY (0x8U)

#define SRTM_AUTO_VERSION (0x0100U)

#define SRTM_AUTO_RETURN_CODE_SUCEESS (0x0U)
#define SRTM_AUTO_RETURN_CODE_FAIL (0x1U)
#define SRTM_AUTO_RETURN_CODE_UNSUPPORTED (0x2U)

/* Auto Service Notification Command definition */

/* Auto event register */
typedef struct _srtm_auto_client
{
    srtm_list_t node;
    srtm_channel_t channel;
    uint32_t clientId;
} * srtm_auto_client_t;

/* Service handle */
typedef struct _srtm_auto_service
{
    struct _srtm_service service;
    srtm_auto_adapter_t vehicle;
    srtm_list_t clients;
} * srtm_auto_service_t;

typedef enum _srtm_auto_async_req_state
{
    SRTM_AUTO_ASYNC_REQ_STATE_INIT = 0U, /* State before request sent. */
    SRTM_AUTO_ASYNC_REQ_STATE_WAITING,   /* Waiting for the result */
    SRTM_AUTO_ASYNC_REQ_STATE_CANCELLED, /* Request cancelled */
    SRTM_AUTO_ASYNC_REQ_STATE_FINISHED   /* Got result. */
} srtm_auto_async_req_state_t;

SRTM_ANON_DEC_BEGIN
typedef struct _srtm_auto_async_req
{
    bool syncResult; /* Whether return result in sync/async way. */
    srtm_mutex_t mutex;
    srtm_sem_t sem;
    srtm_service_t service;
    uint8_t retCode;
    uint32_t cmdParamLen;
    union
    {
        uint8_t *result;
        srtm_auto_cmd_cb_t resultCb;
    };
    union
    {
        uint32_t resultLen;
        void *resultCbParam;
    };
    srtm_auto_async_req_state_t reqState;
} * srtm_auto_async_req_t;
SRTM_ANON_DEC_END

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void SRTM_AutoService_DestroyAsyncReq(srtm_auto_async_req_t req)
{
    if (req->mutex)
    {
        SRTM_Mutex_Destroy(req->mutex);
    }
    if (req->sem)
    {
        SRTM_Sem_Destroy(req->sem);
    }
    SRTM_Heap_Free(req);
}

static srtm_auto_async_req_t SRTM_AutoService_CreateAsyncReq(srtm_service_t service)
{
    srtm_auto_async_req_t req;

    req = (srtm_auto_async_req_t)SRTM_Heap_Malloc(sizeof(struct _srtm_auto_async_req));
    if (req)
    {
        memset(req, 0, sizeof(struct _srtm_auto_async_req));
        req->mutex = SRTM_Mutex_Create();
        req->sem   = SRTM_Sem_Create(1, 0);
        if (!req->mutex || !req->sem)
        {
            SRTM_AutoService_DestroyAsyncReq(req);
            req = NULL;
        }
        else
        {
            req->service = service;
        }
    }

    return req;
}

static srtm_status_t SRTM_AutoService_Register(srtm_service_t service,
                                               srtm_channel_t channel,
                                               uint32_t clientId,
                                               uint8_t *reqData,
                                               uint32_t reqLen,
                                               uint8_t *respData,
                                               uint32_t respLen)
{
    srtm_auto_service_t handle  = (srtm_auto_service_t)service;
    srtm_auto_adapter_t vehicle = handle->vehicle;
    srtm_auto_client_t client;
    srtm_list_t *list;
    srtm_status_t status = SRTM_Status_Success;

    /* First find out if client already registered */
    for (list = handle->clients.next; list != &handle->clients; list = list->next)
    {
        client = SRTM_LIST_OBJ(srtm_auto_client_t, node, list);
        if (client->channel == channel && client->clientId == clientId)
        {
            break;
        }
    }

    if (list == &handle->clients)
    {
        /* Not found */
        client = (srtm_auto_client_t)SRTM_Heap_Malloc(sizeof(struct _srtm_auto_client));
        if (!client)
        {
            status = SRTM_Status_OutOfMemory;
        }
        else
        {
            /* Notify application that there's a new client registered */
            assert(vehicle->registerEvent);
            status = vehicle->registerEvent(vehicle, clientId, reqData, reqLen, respData, respLen);

            if (status != SRTM_Status_Success)
            {
                SRTM_Heap_Free(client);
            }
            else
            {
                client->channel  = channel;
                client->clientId = clientId;
                SRTM_List_AddTail(&handle->clients, &client->node);
            }
        }
    }
    else
    {
        SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_WARN, "%s: already registered\r\n", __func__);
        status = SRTM_Status_Error;
    }

    return status;
}

static srtm_status_t SRTM_AutoService_Unregister(srtm_service_t service,
                                                 srtm_channel_t channel,
                                                 uint32_t clientId,
                                                 uint8_t *reqData,
                                                 uint32_t reqLen,
                                                 uint8_t *respData,
                                                 uint32_t respLen)
{
    srtm_auto_service_t handle  = (srtm_auto_service_t)service;
    srtm_auto_adapter_t vehicle = handle->vehicle;
    srtm_auto_client_t client;
    srtm_list_t *list;
    srtm_status_t status = SRTM_Status_Success;

    /* First find out if client already registered */
    for (list = handle->clients.next; list != &handle->clients; list = list->next)
    {
        client = SRTM_LIST_OBJ(srtm_auto_client_t, node, list);
        if (client->channel == channel && client->clientId == clientId)
        {
            break;
        }
    }

    if (list == &handle->clients)
    {
        /* Not found */
        status = SRTM_Status_Error;
    }
    else
    {
        /* Notify application that there's a client unregistered */
        assert(vehicle->unregisterEvent);
        status = vehicle->unregisterEvent(vehicle, clientId, reqData, reqLen, respData, respLen);

        if (status == SRTM_Status_Success)
        {
            SRTM_List_Remove(&client->node);
            SRTM_Heap_Free(client);
        }
    }

    return status;
}

static srtm_status_t SRTM_AutoService_Event(srtm_service_t service,
                                            srtm_channel_t channel,
                                            uint8_t command,
                                            uint32_t clientId,
                                            uint8_t *reqData,
                                            uint32_t reqLen,
                                            uint8_t *respData,
                                            uint32_t respLen)
{
    srtm_auto_service_t handle  = (srtm_auto_service_t)service;
    srtm_auto_adapter_t vehicle = handle->vehicle;
    srtm_auto_client_t client;
    srtm_list_t *list;
    srtm_status_t status = SRTM_Status_Success;

    /* First find out if client already registered */
    for (list = handle->clients.next; list != &handle->clients; list = list->next)
    {
        client = SRTM_LIST_OBJ(srtm_auto_client_t, node, list);
        if (client->channel == channel && client->clientId == clientId)
        {
            break;
        }
    }

    if (list == &handle->clients)
    {
        /* Not found */
        SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_WARN, "%s(%d): client %d is not registered\r\n", __func__, command,
                           clientId);
        status = SRTM_Status_Error;
    }
    else
    {
        switch (command)
        {
            case SRTM_AUTO_CMD_CONTROL:
                assert(vehicle->control);
                status = vehicle->control(vehicle, clientId, reqData, reqLen, respData, respLen);
                break;
            case SRTM_AUTO_CMD_POWER_REPORT:
                assert(vehicle->powerReport);
                status = vehicle->powerReport(vehicle, clientId, reqData, reqLen, respData, respLen);
                break;
            case SRTM_AUTO_CMD_GET_INFO:
                assert(vehicle->getInfo);
                status = vehicle->getInfo(vehicle, clientId, reqData, reqLen, respData, respLen);
                break;
            default:
                SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_WARN, "%s: command %d unsupported!\r\n", __func__, command);
                status = SRTM_Status_Error;
                break;
        }
    }

    return status;
}

/* Both request and notify are called from SRTM dispatcher context */
static srtm_status_t SRTM_AutoService_Request(srtm_service_t service, srtm_request_t request)
{
    srtm_status_t status;
    srtm_channel_t channel;
    uint8_t command;
    uint32_t payloadLen;
    srtm_response_t response;
    struct _srtm_auto_payload *autoReq;
    struct _srtm_auto_payload *autoResp;

    assert(service->dispatcher);

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s\r\n", __func__);

    channel    = SRTM_CommMessage_GetChannel(request);
    command    = SRTM_CommMessage_GetCommand(request);
    autoReq    = (struct _srtm_auto_payload *)SRTM_CommMessage_GetPayload(request);
    payloadLen = SRTM_CommMessage_GetPayloadLen(request);

    response = SRTM_Response_Create(channel, SRTM_AUTO_CATEGORY, SRTM_AUTO_VERSION, command,
                                    sizeof(struct _srtm_auto_payload));
    if (!response)
    {
        return SRTM_Status_OutOfMemory;
    }

    autoResp = (struct _srtm_auto_payload *)SRTM_CommMessage_GetPayload(response);

    status = SRTM_Service_CheckVersion(service, request, SRTM_AUTO_VERSION);
    if (status != SRTM_Status_Success || !autoReq || payloadLen != sizeof(struct _srtm_auto_payload))
    {
        SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_WARN, "%s: format error!\r\n", __func__);
        autoResp->clientId = autoReq ? autoReq->clientId : 0;
        autoResp->retCode  = SRTM_AUTO_RETURN_CODE_UNSUPPORTED;
    }
    else
    {
        autoResp->clientId = autoReq->clientId;
        SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO,
                           "SRTM receive request:client=%d, cmd=%x, data0=%x,data1=%x,data2=%x!\r\n", autoReq->clientId,
                           command, autoReq->data[0], autoReq->data[1], autoReq->data[2]);
        switch (command)
        {
            case SRTM_AUTO_CMD_REGISTER:
                status = SRTM_AutoService_Register(service, channel, autoReq->clientId, autoReq->data,
                                                   SRTM_USER_DATA_LENGTH, autoResp->data, SRTM_USER_DATA_LENGTH);
                autoResp->retCode =
                    status == SRTM_Status_Success ? SRTM_AUTO_RETURN_CODE_SUCEESS : SRTM_AUTO_RETURN_CODE_FAIL;
                break;
            case SRTM_AUTO_CMD_UNREGISTER:
                status = SRTM_AutoService_Unregister(service, channel, autoReq->clientId, autoReq->data,
                                                     SRTM_USER_DATA_LENGTH, autoResp->data, SRTM_USER_DATA_LENGTH);
                autoResp->retCode =
                    status == SRTM_Status_Success ? SRTM_AUTO_RETURN_CODE_SUCEESS : SRTM_AUTO_RETURN_CODE_FAIL;
                break;
            default:
                status = SRTM_AutoService_Event(service, channel, command, autoReq->clientId, autoReq->data,
                                                SRTM_USER_DATA_LENGTH, autoResp->data, SRTM_USER_DATA_LENGTH);
                autoResp->retCode =
                    status == SRTM_Status_Success ? SRTM_AUTO_RETURN_CODE_SUCEESS : SRTM_AUTO_RETURN_CODE_FAIL;
                break;
        }
    }

    return SRTM_Dispatcher_DeliverResponse(service->dispatcher, response);
}

static srtm_status_t SRTM_AutoService_Notify(srtm_service_t service, srtm_notification_t notif)
{
    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_WARN, "%s: command %d unsupported!\r\n", __func__,
                       SRTM_CommMessage_GetCommand(notif));

    return SRTM_Status_ServiceNotFound;
}

static void SRTM_AutoService_Cleanup(srtm_auto_service_t handle, srtm_peercore_t core)
{
    srtm_auto_client_t client;
    srtm_list_t *list, *next;

    for (list = handle->clients.next; list != &handle->clients; list = next)
    {
        next   = list->next;
        client = SRTM_LIST_OBJ(srtm_auto_client_t, node, list);
        assert(client->channel);
        if (client->channel->core == core || !core)
        {
            SRTM_List_Remove(list);
            SRTM_Heap_Free(client);
        }
    }
}

srtm_service_t SRTM_AutoService_Create(srtm_auto_adapter_t vehicle)
{
    srtm_auto_service_t handle;

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s\r\n", __func__);

    handle = (srtm_auto_service_t)SRTM_Heap_Malloc(sizeof(struct _srtm_auto_service));
    assert(handle);

    vehicle->service = &handle->service;
    handle->vehicle  = vehicle;

    SRTM_List_Init(&handle->clients);
    SRTM_List_Init(&handle->service.node);
    handle->service.dispatcher = NULL;
    handle->service.category   = SRTM_AUTO_CATEGORY;
    handle->service.destroy    = SRTM_AutoService_Destroy;
    handle->service.request    = SRTM_AutoService_Request;
    handle->service.notify     = SRTM_AutoService_Notify;

    return &handle->service;
}

void SRTM_AutoService_Destroy(srtm_service_t service)
{
    srtm_auto_service_t handle = (srtm_auto_service_t)service;

    assert(service);

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s\r\n", __func__);

    /* Service must be unregistered from dispatcher before destroy */
    assert(SRTM_List_IsEmpty(&service->node));

    SRTM_AutoService_Cleanup(handle, NULL);

    SRTM_Heap_Free(handle);
}

void SRTM_AutoService_Reset(srtm_service_t service, srtm_peercore_t core)
{
    srtm_auto_service_t handle = (srtm_auto_service_t)service;

    assert(service);

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s\r\n", __func__);

    SRTM_AutoService_Cleanup(handle, core);
}

static void SRTM_AutoService_ResponseCallback(srtm_dispatcher_t disp,
                                              srtm_message_t req,
                                              srtm_message_t resp,
                                              void *param)
{
    srtm_auto_async_req_t asyncReq = (srtm_auto_async_req_t)param;
    struct _srtm_auto_payload *autoReq;
    struct _srtm_auto_payload *autoResp;
    uint32_t payloadLen = 0;
    bool freeAsyncReq   = false;

    autoReq  = (struct _srtm_auto_payload *)SRTM_CommMessage_GetPayload(req);
    autoResp = resp ? (struct _srtm_auto_payload *)SRTM_CommMessage_GetPayload(resp) : NULL;
    if (autoResp)
    {
        payloadLen = SRTM_CommMessage_GetPayloadLen(resp);
        if (payloadLen < sizeof(struct _srtm_auto_payload))
        {
            assert(false);
        }
    }

    SRTM_Mutex_Lock(asyncReq->mutex);
    if (asyncReq->reqState == SRTM_AUTO_ASYNC_REQ_STATE_WAITING)
    {
        asyncReq->retCode = autoResp ? autoResp->retCode : SRTM_Status_TransferFailed;
        if (asyncReq->syncResult)
        {
            if (asyncReq->result && autoResp)
            {
                assert(asyncReq->resultLen <= SRTM_USER_DATA_LENGTH);
                memcpy(asyncReq->result, autoResp->data, asyncReq->resultLen);
            }
            SRTM_Sem_Post(asyncReq->sem);
        }
        else
        {
            asyncReq->resultCb(
                asyncReq->service, autoReq->clientId, (srtm_auto_cmd_t)SRTM_CommMessage_GetCommand(req), autoReq->data,
                asyncReq->cmdParamLen, autoResp ? autoResp->retCode : SRTM_Status_TransferFailed,
                autoResp ? autoResp->data : NULL, autoResp ? SRTM_USER_DATA_LENGTH : 0, asyncReq->resultCbParam);
            freeAsyncReq = true;
        }
        asyncReq->reqState = SRTM_AUTO_ASYNC_REQ_STATE_FINISHED;
    }
    else
    {
        freeAsyncReq = true;
    }
    SRTM_Mutex_Unlock(asyncReq->mutex);
    if (freeAsyncReq)
    {
        SRTM_AutoService_DestroyAsyncReq(asyncReq);
    }
}

/* Called in SRTM dispatcher context */
static void SRTM_AutoService_DeliverRequest(srtm_dispatcher_t dispatcher, void *param1, void *param2)
{
    srtm_auto_async_req_t asyncReq = (srtm_auto_async_req_t)param1;
    srtm_auto_service_t handle     = (srtm_auto_service_t)asyncReq->service;
    srtm_request_t req             = (srtm_request_t)param2;
    struct _srtm_auto_payload *autoReq;
    srtm_status_t status;
    srtm_auto_client_t client;
    srtm_list_t *list;

    autoReq = (struct _srtm_auto_payload *)SRTM_CommMessage_GetPayload(req);

    for (list = handle->clients.next; list != &handle->clients; list = list->next)
    {
        client = SRTM_LIST_OBJ(srtm_auto_client_t, node, list);
        if (client->clientId == autoReq->clientId)
        {
            req->channel = client->channel;
            status       = SRTM_Dispatcher_DeliverRequest(dispatcher, req, SRTM_AutoService_ResponseCallback, param1);
            if (status == SRTM_Status_Success)
            {
                asyncReq->reqState = SRTM_AUTO_ASYNC_REQ_STATE_WAITING;
            }
            break;
        }
    }
}

static srtm_status_t SRTM_AutoService_DoRequest(
    srtm_auto_async_req_t asyncReq, uint32_t clientId, srtm_auto_cmd_t cmd, uint8_t *cmdParam, uint32_t paramLen)
{
    srtm_request_t req;
    struct _srtm_auto_payload *autoReq;
    srtm_status_t status;
    srtm_procedure_t proc;

    assert(paramLen <= SRTM_USER_DATA_LENGTH);

    req = SRTM_Request_Create(NULL, SRTM_AUTO_CATEGORY, SRTM_AUTO_VERSION, cmd, sizeof(struct _srtm_auto_payload));
    if (!req)
    {
        return SRTM_Status_OutOfMemory;
    }

    autoReq           = (struct _srtm_auto_payload *)SRTM_CommMessage_GetPayload(req);
    autoReq->clientId = clientId;
    if (cmdParam)
    {
        memcpy(autoReq->data, cmdParam, paramLen);
    }

    proc = SRTM_Procedure_Create(SRTM_AutoService_DeliverRequest, asyncReq, req);
    if (!proc)
    {
        SRTM_Request_Destroy(req);
        return SRTM_Status_OutOfMemory;
    }

    status = SRTM_Dispatcher_CallProc(asyncReq->service->dispatcher, proc, SRTM_WAIT_FOR_EVER);
    if (status != SRTM_Status_Success || asyncReq->reqState == SRTM_AUTO_ASYNC_REQ_STATE_INIT)
    {
        SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_ERROR, "%s: clientId %d, cmd %d request error\r\n", __func__, clientId,
                           cmd);
        SRTM_Request_Destroy(req);
        status = SRTM_Status_Error;
    }

    SRTM_Procedure_Destroy(proc);

    return status;
}

srtm_status_t SRTM_AutoService_SendCommand(srtm_service_t service,
                                           uint32_t clientId,
                                           srtm_auto_cmd_t cmd,
                                           uint8_t *cmdParam,
                                           uint32_t paramLen,
                                           uint8_t *result,
                                           uint32_t resultLen,
                                           uint32_t timeout)
{
    srtm_status_t status;
    srtm_auto_async_req_t asyncReq;
    bool freeAsyncReq = true;

    assert(service);
    assert(timeout > 0);

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s: clientId %d, cmd %d\r\n", __func__, clientId, cmd);

    asyncReq = SRTM_AutoService_CreateAsyncReq(service);
    if (!asyncReq)
    {
        return SRTM_Status_OutOfMemory;
    }

    asyncReq->syncResult  = true;
    asyncReq->result      = result;
    asyncReq->resultLen   = resultLen;
    asyncReq->cmdParamLen = paramLen;
    status                = SRTM_AutoService_DoRequest(asyncReq, clientId, cmd, cmdParam, paramLen);
    if (status == SRTM_Status_Success)
    {
        /* Wait result */
        SRTM_Sem_Wait(asyncReq->sem, timeout);
        SRTM_Mutex_Lock(asyncReq->mutex);
        if (asyncReq->reqState != SRTM_AUTO_ASYNC_REQ_STATE_FINISHED)
        {
            /* Timeout, we cancel the request. asyncReq will be destroyed by response callback. */
            asyncReq->reqState = SRTM_AUTO_ASYNC_REQ_STATE_CANCELLED;
            status             = SRTM_Status_TransferTimeout;
            freeAsyncReq       = false; /* asyncReq will be freed later in response callback. */
        }
        else
        {
            status = asyncReq->retCode == SRTM_AUTO_RETURN_CODE_SUCEESS ? SRTM_Status_Success : SRTM_Status_Error;
        }
        SRTM_Mutex_Unlock(asyncReq->mutex);
    }

    if (freeAsyncReq)
    {
        SRTM_AutoService_DestroyAsyncReq(asyncReq);
    }

    return status;
}

srtm_status_t SRTM_AutoService_DeliverCommand(srtm_service_t service,
                                              uint32_t clientId,
                                              srtm_auto_cmd_t cmd,
                                              uint8_t *cmdParam,
                                              uint32_t paramLen,
                                              srtm_auto_cmd_cb_t callback,
                                              void *userParam)
{
    srtm_status_t status;
    srtm_auto_async_req_t asyncReq;

    assert(service);

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s: clientId %d, cmd %d\r\n", __func__, clientId, cmd);

    asyncReq = SRTM_AutoService_CreateAsyncReq(service);
    if (!asyncReq)
    {
        return SRTM_Status_OutOfMemory;
    }

    asyncReq->syncResult    = false;
    asyncReq->resultCb      = callback;
    asyncReq->resultCbParam = userParam;
    asyncReq->cmdParamLen   = paramLen;
    status                  = SRTM_AutoService_DoRequest(asyncReq, clientId, cmd, cmdParam, paramLen);

    return status;
}
