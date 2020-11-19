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
#include "lwip/sockets.h"
#include "mbedtls/ssl.h"

int sslsrv_mbedtls_send(void *ctx, unsigned char const *buf, size_t len)
{
    int result;

    result = lwip_send((int)ctx, buf, len, 0);

    return result;
}

int sslsrv_mbedtls_recv(void *ctx, unsigned char *buf, size_t len)
{
    int result;

    fd_set read_set;

    FD_ZERO(&read_set);
    FD_SET((int)ctx, &read_set);

    struct timeval timeout;
    timeout.tv_sec  = 30;
    timeout.tv_usec = 0;

    result = lwip_select((int)ctx + 1, &read_set, NULL, NULL, &timeout);
    if (result > 0)
    {
        result = lwip_recv((int)ctx, buf, len, 0);
        if (result == 0)
        {
            /* Connection was closed */
            return -1;
        }
    }

    return result;
}

int sslsrv_init_socket(volatile int *sock, uint32_t max_ses)
{
    int error;

    struct sockaddr address;
    address.sa_family                                   = AF_INET;
    ((struct sockaddr_in *)(&address))->sin_port        = PP_HTONS(SSLSRV_CFG_DEFAULT_PORT);
    ((struct sockaddr_in *)(&address))->sin_addr.s_addr = INADDR_ANY;

    if ((*sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return (SSLSRV_CREATE_FAIL);
    }

    /* Bind socket */
    error = lwip_bind(*sock, &address, sizeof(address));
    if (error == -1)
    {
        return (SSLSRV_BIND_FAIL);
    }

    /* Listen */
    error = lwip_listen(*sock, max_ses);
    if (error == -1)
    {
        return (SSLSRV_LISTEN_FAIL);
    }
    return (SSLSRV_OK);
}

int sslsrv_wait_for_conn(int sock)
{
    fd_set readset;
    int32_t retval = -1;

    FD_ZERO(&readset);
    FD_SET(sock, &readset);

    if (lwip_select(1, (fd_set *)&readset, 0, 0, 0) == 1)
    {
        if (FD_ISSET(sock, &readset))
        {
            retval = sock;
        }
    }
    return (retval);
}

int sslsrv_accept(int sock)
{
    struct sockaddr remote_addr;
    socklen_t length;

    memset(&remote_addr, 0, sizeof(remote_addr));
    length = sizeof(remote_addr);

    return lwip_accept(sock, &remote_addr, &length);
}

int sslsrv_close(int sock)
{
    lwip_shutdown(sock, SHUT_WR);
    lwip_close(sock);

    return 0;
}
