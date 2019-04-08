/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*   This file contains the SSLSRV implementation.*/

#include "sslsrv.h"
#include "sslsrv_supp.h"
#include "sslsrv_prv.h"
#include "sslsrv_port.h"
#include <string.h>

#define SSLSRV_SERVER_TASK_NAME "SSL server"

/*
** Function for starting the SSL server
**
** IN:
**      SSLSRV_PARAM_STRUCT*   params - server parameters (port, ip, index page etc.)
**
** OUT:
**      none
**
** Return Value:
**      uint32_t      server handle if successful, NULL otherwise
*/
uint32_t SSLSRV_init(SSLSRV_PARAM_STRUCT *params)
{
    SSLSRV_STRUCT *server;
    uint32_t handle = 0;

    server = sslsrv_create_server(params);
    if (server)
    {
        server->valid = SSLSRV_VALID;

        BaseType_t xReturned;
        xReturned = xTaskCreate(sslsrv_server_task, SSLSRV_SERVER_TASK_NAME, SSLSRV_CFG_SERVER_STACK_SIZE, server,
                                server->params.task_prio, (TaskHandle_t *)&server->server_tid);
        if (xReturned != pdPASS)
        {
            sslsrv_destroy_server(server);
            sslsrv_mem_free(server);
        }
        else
        {
            handle = (uint32_t)server;
        }
    }

    return (handle);
}

/*
** Function for releasing/stopping SSL server
**
** IN:
**      uint32_t       server_h - server handle
**
** OUT:
**      none
**
** Return Value:
**      uint32_t      error code. SSLSRV_OK if everything went right, positive number otherwise
*/
void SSLSRV_release(uint32_t server_h)
{
    SSLSRV_STRUCT *server = (void *)server_h;

    if (server)
    {
        /* Shutdown server task and wait for its termination. */
        if (server->sock != -1)
        {
            server->valid = SSLSRV_INVALID;
            sslsrv_close(server->sock);
        }

        while (server->server_tid) /* Wait for task completition.*/
        {
            taskYIELD();
        }
        sslsrv_mem_free(server);
    }
}
