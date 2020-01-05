/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 *   SSLSRV tasks and session processing.
 */

#include "sslsrv.h"
#include "sslsrv_prv.h"
#include "sslsrv_supp.h"
#include "sslsrv_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "device_configuration.h"

//#define SSL_SRV_DEBUG

#if !defined(SSL_SRV_DEBUG)
#define SSL_SRV_PRINTF(...)
#else
#define SSL_SRV_PRINTF vLoggingPrintf
#endif

#define SSLSRV_SESSION_TASK_NAME "SSL server session"

static SSLSRV_SESSION_STRUCT *sslsrv_ses_alloc(SSLSRV_STRUCT *server, int sock);
static void sslsrv_ses_free(SSLSRV_SESSION_STRUCT *session);
static void sslsrv_ses_close(SSLSRV_SESSION_STRUCT *session);
static int sslsrv_ses_init(SSLSRV_STRUCT *server, SSLSRV_SESSION_STRUCT *session, const int sock);
static void sslsrv_session_task(void *arg);

/*
** SSLSRV main task which creates new task for each new client request
*/
void sslsrv_server_task(void *arg)
{
    SSLSRV_STRUCT *server = (SSLSRV_STRUCT *)arg;

    while (1)
    {
        int i;
        int new_sock;

        /* limit number of opened sessions */
        if (xSemaphoreTake(server->ses_cnt, portMAX_DELAY) != pdTRUE)
        {
            break;
        }
        SSL_SRV_PRINTF("waiting for client connection ...\n");
        /* Get socket with incoming connection (IPv4 or IPv6) */
        int connsock = sslsrv_wait_for_conn(server->sock);
        if (server->valid == SSLSRV_INVALID)
        {
            break;
        }
        if (connsock == -1)
        {
            SSL_SRV_PRINTF("client connection failed\n");
            xSemaphoreGive(server->ses_cnt);
            // vTaskDelay(1000);
            // taskYIELD();
        }
        else
        {
            new_sock = sslsrv_accept(server->sock);
            SSL_SRV_PRINTF("sslsrv_accept(): %d\n", new_sock);
            if (new_sock < 0)
            {
                xSemaphoreGive(server->ses_cnt);
                /* We probably run out of sockets. Wait some time then try again to prevent session tasks resource
                 * starvation */
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            else
            {
                /* Find empty session */
                for (i = 0; i < server->params.max_ses; i++)
                {
                    if (server->session[i] == NULL)
                    {
                        break;
                    }
                }

                if (i != server->params.max_ses)
                {
                    SSLSRV_SESSION_STRUCT *session;

                    /* Allocate session */
                    session = sslsrv_ses_alloc(server, new_sock);
                    if (session)
                    {
                        SSL_SRV_PRINTF("client connected, creating session ...\n");
                        SSLSRV_SES_TASK_PARAM *ses_param;

                        /* Allocate session task parameter */
                        ses_param = sslsrv_mem_alloc_zero(sizeof(SSLSRV_SES_TASK_PARAM));

                        if (ses_param != NULL)
                        {
                            if (SSLSRV_OK == sslsrv_ses_init(server, session, new_sock))
                            {
                                server->session[i] = session;

                                ses_param->server    = server;
                                ses_param->session_p = &server->session[i];

                                /* Try to create task for session */
                                BaseType_t xReturned;
                                xReturned = xTaskCreate(sslsrv_session_task, SSLSRV_SESSION_TASK_NAME,
                                                        SSLSRV_CFG_SESSION_STACK_SIZE, ses_param,
                                                        server->params.task_prio, NULL);
                                if (xReturned != pdPASS)
                                {
                                    sslsrv_ses_close(session);
                                    sslsrv_ses_free(session);
                                    server->session[i] = NULL;
                                    xSemaphoreGive(server->ses_cnt);
                                    sslsrv_mem_free(ses_param);
                                }
                            }
                            else
                            {
                                SSL_SRV_PRINTF("sslsrv_ses_init() failed\n");
                                sslsrv_ses_close(session);
                                sslsrv_ses_free(session);
                                xSemaphoreGive(server->ses_cnt);
                                sslsrv_mem_free(ses_param);
                            }
                        }
                        else
                        {
                            sslsrv_ses_close(session);
                            sslsrv_ses_free(session);
                            xSemaphoreGive(server->ses_cnt);
                        }
                    }
                    else
                    {
                        sslsrv_abort(new_sock);
                        xSemaphoreGive(server->ses_cnt);
                        SSL_SRV_PRINTF("sslsrv_ses_alloc(): %d\n", session);
                    }
                }
                else
                {
                    sslsrv_abort(new_sock);
                    xSemaphoreGive(server->ses_cnt);
                }
            }
        }
    }
    /* Server release.*/
    sslsrv_destroy_server(server);
    server->server_tid = 0;

    vTaskDelete(NULL);
}

/*
** Session task.
** This task is responsible for session creation, processing and cleanup.
*/
static void sslsrv_session_task(void *arg)
{
    SSLSRV_SES_TASK_PARAM *ses_param = (SSLSRV_SES_TASK_PARAM *)arg;
    SSLSRV_STRUCT *server            = ses_param->server;
    SSLSRV_SESSION_STRUCT *session   = *ses_param->session_p;

    while (session->valid)
    {
        /* Run state machine for session */
        session->process_func(server, session);
        taskYIELD();
    }
    SSL_SRV_PRINTF("closing session...\n");
    sslsrv_ses_close(session);
    sslsrv_ses_free(session);
    *ses_param->session_p = NULL;

    /* Cleanup and end task */
    sslsrv_mem_free(ses_param);
    xSemaphoreGive(server->ses_cnt);

    vTaskDelete(NULL);
}

/*
** Function for session allocation
**
** IN:
**      SSLSRV_STRUCT *server - pointer to server structure (needed for session parameters).
**
** OUT:
**      none
**
** Return Value:
**      SSLSRV_SESSION_STRUCT* - pointer to allocated session. Non-zero if allocation was OK, NULL otherwise
*/
static SSLSRV_SESSION_STRUCT *sslsrv_ses_alloc(SSLSRV_STRUCT *server, int sock)
{
    SSLSRV_SESSION_STRUCT *session = NULL;

    if (server)
    {
        session = sslsrv_mem_alloc_zero(sizeof(SSLSRV_SESSION_STRUCT));
        if (session)
        {
            session->sock = -1;

            /* Alloc session buffer */
            session->buffer.data = sslsrv_mem_alloc_zero(sizeof(char) * SSLSRV_SES_BUF_SIZE_PRV);
            if (NULL == session->buffer.data)
            {
                goto ERROR;
            }

            if (server->tls_ctx != 0)
            {
                session->tls_sock = sslsrv_tls_socket(server->tls_ctx, sock);
                if (session->tls_sock == 0)
                {
                    goto ERROR;
                }
            }
        }
    }

    return session;

ERROR:
    if (session->buffer.data)
    {
        sslsrv_mem_free(session->buffer.data);
        session->buffer.data = NULL;
    }
    sslsrv_mem_free(session);
    return (NULL);
}

/*
** Function used to free session structure
**
** IN:
**      SSLSRV_SESSION_STRUCT* session - session structure pointer
**
** OUT:
**      none
**
** Return Value:
**      none
*/
static void sslsrv_ses_free(SSLSRV_SESSION_STRUCT *session)
{
    if (session)
    {
        if (session->buffer.data)
        {
            sslsrv_mem_free(session->buffer.data);
        }

        sslsrv_mem_free(session);
    }
}

/*
** Function used to init session structure
**
** IN:
**      SSLSRV_SESSION_STRUCT* session - session structure pointer
**      SSLSRV_STRUCT *server - pointer to server structure (needed for session parameters)
**      const int sock - socket handle used for communication with client
**
** OUT:
**      none
**
** Return Value:
**      error code - SSLSRV_ERR or SSLSRV_OK
*/
static int sslsrv_ses_init(SSLSRV_STRUCT *server, SSLSRV_SESSION_STRUCT *session, int sock)
{
    if (server && session)
    {
        session->sock          = sock;
        session->valid         = SSLSRV_VALID;
        session->timeout       = pdMS_TO_TICKS(SSLSRV_CFG_SES_TIMEOUT);
        session->time          = xTaskGetTickCount();
        session->process_func  = sslsrv_process;
        session->authenticated = false;
        return SSLSRV_OK;
    }
    return SSLSRV_ERR;
}

/*
** Function used to close session
**
** IN:
**      SSLSRV_SESSION_STRUCT* session - session structure pointer
**
** OUT:
**      none
**
** Return Value:
**      none
*/
static void sslsrv_ses_close(SSLSRV_SESSION_STRUCT *session)
{
    if (session != NULL)
    {
        if (session->tls_sock != 0)
        {
            sslsrv_tls_shutdown(session->tls_sock);
            session->tls_sock = 0;
        }

        if (session->sock != -1)
        {
            sslsrv_close(session->sock);
            session->sock = -1;
        }
    }
}

int srv_ssl_read(SSLSRV_SESSION_STRUCT *session, char *buffer, size_t length)
{
    uint32_t to_receive = length;
    uint32_t received   = 0;
    int result;
    do
    {
        result = mbedtls_ssl_read(session->tls_sock, (unsigned char *)buffer + received, to_receive);
        if (result > 0)
        {
            received += result;
        }
        if (result < 0)
        {
            SSL_SRV_PRINTF("mbedtls_ssl_read(): %d\n", result);
            return result;
        }

        /* check session timeout */
        if ((xTaskGetTickCount() - session->time) > session->timeout)
        {
            SSL_SRV_PRINTF("session timeout\n");
            return -1;
        }
    } while (received < to_receive);

    return received;
}

/*
** SSL session state machine
**
** IN:
**      SSLSRV_SESSION_STRUCT* session - session structure pointer.
**      SSLSRV_STRUCT *server - pointer to server structure (needed for session parameters).
**
** OUT:
**      none
**
** Return Value:
**      none
*/
void sslsrv_process(void *server_ptr, void *session_ptr)
{
    uint32_t time_interval;
    int result;
    // SSLSRV_STRUCT *server = (SSLSRV_STRUCT *)server_ptr;
    SSLSRV_SESSION_STRUCT *session = (SSLSRV_SESSION_STRUCT *)session_ptr;
    if (!session->valid)
    {
        return;
    }

    /* Check session timeout */
    time_interval = xTaskGetTickCount() - session->time;
    if (time_interval > session->timeout)
    {
        session->valid = SSLSRV_INVALID;
        return;
    }

    char *buffer = session->buffer.data;

    /* Receive header - cmd and data length */
    result = srv_ssl_read(session, buffer, 4);
    if (result < 4)
    {
        session->valid = SSLSRV_INVALID;
        return;
    }

    uint16_t cmd;
    cmd = ((uint16_t)(buffer[0]) << 8) | buffer[1];

    uint16_t data_len;
    data_len = ((uint16_t)(buffer[2]) << 8) | buffer[3];

    if (data_len >= SSLSRV_SES_BUF_SIZE_PRV)
    {
        return;
    }

    if (data_len > 0)
    {
        /* Receive data */
        result = srv_ssl_read(session, buffer, data_len);
        if (result < 0)
        {
            return;
        }

        /* The upper layer stores the data as string, so it should be null terminated */
        buffer[data_len] = '\0';
        data_len++;
    }

    response_t resp;
    memset(&resp, 0, sizeof(response_t));
    resp.status = kCMD_Fail;
    if (session->authenticated == true)
    {
        resp = dev_cfg_process_cmd((request_cmd_t)cmd, buffer, data_len);
    }
    else
    {
        if (cmd == kCMD_Login)
        {
            if (dev_cfg_check_login_password(buffer, data_len) == 0)
            {
                session->authenticated = true;
                resp.status            = kCMD_Ok;
            }
        }
    }

    if ((resp.data != NULL) && (resp.data_length > 0))
    {
        /* Allocate buffer for 1B-status | 2B-payload length | nB-payload */
        uint8_t *tx_buff = (uint8_t *)sslsrv_mem_alloc(3 + resp.data_length);
        if (tx_buff == NULL)
        {
            return;
        }

        tx_buff[0] = resp.status;
        tx_buff[1] = (uint8_t)(resp.data_length >> 8);
        tx_buff[2] = (uint8_t)resp.data_length;

        memcpy(&tx_buff[3], resp.data, resp.data_length);

        /* Send buffer */
        result = mbedtls_ssl_write(session->tls_sock, (unsigned char *)tx_buff, resp.data_length + 3);
        SSL_SRV_PRINTF("Send data, mbedtls_ssl_write(): %d\n", result);

        sslsrv_mem_free(tx_buff);
    }
    else
    {
        /* Send response to client */
        result = mbedtls_ssl_write(session->tls_sock, (unsigned char *)&resp.status, sizeof(response_cmd_t));
        SSL_SRV_PRINTF("mbedtls_ssl_write(): %d, data: %d\n", result, resp.status);
    }
    session->time = xTaskGetTickCount();

    return;
}
