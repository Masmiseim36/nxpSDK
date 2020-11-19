/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 *   SSLSRV support functions.
 */

#include "sslsrv.h"
#include "sslsrv_prv.h"
#include "sslsrv_supp.h"
#include "sslsrv_port.h"
#include <string.h>
#include <stdlib.h>
#include "sslsrv_tls.h"

static int32_t sslsrv_set_params(SSLSRV_STRUCT *server, SSLSRV_PARAM_STRUCT *params);

/*
 * Allocate server structure, init sockets, etc.
 */
SSLSRV_STRUCT *sslsrv_create_server(SSLSRV_PARAM_STRUCT *params)
{
    SSLSRV_STRUCT *server = NULL;
    int32_t error;

    if ((server = sslsrv_mem_alloc_zero(sizeof(SSLSRV_STRUCT))) == NULL)
    {
        return (NULL);
    }

    error = sslsrv_set_params(server, params);
    if (error != SSLSRV_OK)
    {
        goto EXIT;
    }

    server->ses_cnt = xSemaphoreCreateCounting(server->params.max_ses, server->params.max_ses);
    if (server->ses_cnt == NULL)
    {
        goto EXIT;
    }

    /* Allocate space for session pointers */
    server->session = sslsrv_mem_alloc_zero(sizeof(SSLSRV_SESSION_STRUCT *) * server->params.max_ses);
    if (server->session == NULL)
    {
        goto EXIT;
    }

    /* Init sockets. */
    error = sslsrv_init_socket(&server->sock, server->params.max_ses);

    if (error != SSLSRV_OK)
    {
        goto EXIT;
    }
    return (server);

EXIT:
    sslsrv_destroy_server(server);
    sslsrv_mem_free((void *)server);
    return (NULL);
}

/*
 * Close sockets, free memory etc.
 */
void sslsrv_destroy_server(SSLSRV_STRUCT *server)
{
    uint32_t n = 0;
    bool wait  = false;

    if (server)
    {
        if (server->valid)
        {
            server->valid = SSLSRV_INVALID;
            if (server->sock != -1)
            {
                sslsrv_close(server->sock);
                server->sock = -1;
            }
        }

        if (server->session)
        {
            /* Invalidate sessions (this is signal for session tasks to end them) */
            for (n = 0; n < server->params.max_ses; n++)
            {
                if (server->session[n])
                {
                    server->session[n]->valid = SSLSRV_INVALID;
                }
            }
            /* Wait until all session tasks end */
            do
            {
                wait = false;
                for (n = 0; n < server->params.max_ses; n++)
                {
                    if (server->session[n])
                    {
                        wait = true;
                        break;
                    }
                }
                taskYIELD();
            } while (wait);

            sslsrv_mem_free((void *)server->session);
            server->session = NULL;
        }

        if (server->ses_cnt)
        {
            vSemaphoreDelete(server->ses_cnt);
        }

        if (server->tls_ctx)
        {
            sslsrv_tls_release(server->tls_ctx);
        }
    }
}

/*
** Internal function for server parameters initialization
**
** IN:
**      SSLSRV_STRUCT* server - server structure pointer
**
**      SSLSRV_PARAM_STRUCT* params - pointer to user parameters if there are any
** OUT:
**      none
**
** Return Value:
**      uint32_t      error code. SSLSRV_OK if everything went right, positive number otherwise
*/
static int32_t sslsrv_set_params(SSLSRV_STRUCT *server, SSLSRV_PARAM_STRUCT *params)
{
    server->params.max_ses   = SSLSRV_CFG_DEFAULT_SES_CNT;
    server->params.task_prio = SSLSRV_CFG_DEFAULT_PRIORITY;
    server->tls_ctx          = NULL;

    /* If there is parameters structure copy nonzero values to server */
    if (params != NULL)
    {
        if (params->max_ses)
            server->params.max_ses = params->max_ses;
        if (params->task_prio)
            server->params.task_prio = params->task_prio;

        if (params->tls_param)
        {
            server->tls_ctx = sslsrv_tls_init(params->tls_param);
            if (server->tls_ctx == NULL)
            {
                return (SSLSRV_ERR);
            }
        }
    }

    return (SSLSRV_OK);
}

/*
 * Abort connection on socket.
 */
void sslsrv_abort(int sock)
{
    //    struct linger l_options;

    /* Set linger options for RST flag sending. */
    //    l_options.l_onoff = 1;
    //    l_options.l_linger_ms = 0;
    //   setsockopt(sock, SOL_SOCKET, SO_LINGER, &l_options, sizeof(l_options));
    sslsrv_close(sock);
}

void *sslsrv_mem_alloc_zero(size_t xSize)
{
    void *result = sslsrv_mem_alloc(xSize);
    if (result)
    {
        memset(result, 0, xSize);
    }
    return result;
}
