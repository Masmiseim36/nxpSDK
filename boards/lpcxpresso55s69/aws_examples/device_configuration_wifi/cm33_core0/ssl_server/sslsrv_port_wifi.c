/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sslsrv_port.h"

#include <stdint.h>
#include "sslsrv_config.h"
#include "sslsrv.h"
#include "qcom_api.h"

int sslsrv_mbedtls_send(void *ctx, unsigned char const *buf, size_t len)
{
    char *loc_buf = custom_alloc(len);
    if (loc_buf == NULL)
    {
        return -1;
    }
    memcpy(loc_buf, buf, len);

    int result;
    result = qcom_send((int)ctx, loc_buf, len, 0);
    custom_free(loc_buf);

    return result;
}

int sslsrv_mbedtls_recv(void *ctx, unsigned char *buf, size_t len)
{
    int result;

    A_STATUS xStatus;
    xStatus = (A_STATUS)t_select(Custom_Api_GetDriverCxt(0), (uint32_t)ctx, 30000);
    if (xStatus == A_OK)
    {
        char *loc_buf;
        result = qcom_recv((int)ctx, &loc_buf, len, 0);
        if ((result > 0) && (loc_buf != NULL))
        {
            memcpy(buf, loc_buf, result);
            zero_copy_free(loc_buf);
        }
    }
    else if (xStatus == A_SOCK_INVALID)
    {
        return -1;
    }
    else
    {
        /* No data to read */
        return 0;
    }

    return result;
}

int sslsrv_init_socket(volatile int *sock, uint32_t max_ses)
{
    int error;

    SOCKADDR_T address;
    memset(&address, 0, sizeof(address));
    address.sin_family = ATH_AF_INET;
    address.sin_port   = SSLSRV_CFG_DEFAULT_PORT;

    if ((*sock = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0)) < 0)
    {
        return (SSLSRV_CREATE_FAIL);
    }

    /* Bind socket */
    error = qcom_bind(*sock, (struct sockaddr *)&address, sizeof(address));
    if (error == -1)
    {
        return (SSLSRV_BIND_FAIL);
    }

    /* Listen */
    error = qcom_listen(*sock, max_ses);
    if (error == -1)
    {
        return (SSLSRV_LISTEN_FAIL);
    }
    return (SSLSRV_OK);
}

int sslsrv_wait_for_conn(int sock)
{
    int32_t retval = -1;

    retval = t_select(Custom_Api_GetDriverCxt(0), sock, 0xFFFFFFFFU);
    if (retval == A_OK)
    {
        retval = sock;
    }

    return (retval);
}

int sslsrv_accept(int sock)
{
    struct sockaddr remote_addr;
    socklen_t length;

    memset(&remote_addr, 0, sizeof(remote_addr));
    length = sizeof(remote_addr);

    return qcom_accept(sock, &remote_addr, &length);
}

int sslsrv_close(int sock)
{
    qcom_socket_close(sock);

    return 0;
}
