/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "core_http_client.h"
#include "lwip/sockets.h"

/* Implementation of recv/send functions to connect CoreHTTP
 * with the TLS layer.
 */

int32_t coreHttp_recv(NetworkContext_t *pNetworkContext, void *pBuffer, size_t bytesToRecv)
{
    int cnt = 0;

    cnt = mbedtls_ssl_read(pNetworkContext->sslCtx, pBuffer, bytesToRecv);

    if (cnt == MBEDTLS_ERR_SSL_WANT_READ || cnt == MBEDTLS_ERR_SSL_WANT_WRITE)
    {
        /* The TLS layer is busy with TLS-related overhead, the caller
         * should repeat the request - CoreHTTP should
         * do so after getting a zero byte read.
         */
        cnt = 0;
    }
    else if (cnt < 0)
    {
        LogError(("coreHttp_recv: ssl_recv FAILED with %d\n", cnt));
        return cnt;
    }

    return cnt;
}

int32_t coreHttp_send(NetworkContext_t *pNetworkContext, const void *pBuffer, size_t bytesToSend)
{
    int cnt;

    cnt = mbedtls_ssl_write(pNetworkContext->sslCtx, (const unsigned char *)pBuffer, bytesToSend);

    if (cnt == MBEDTLS_ERR_SSL_WANT_READ || cnt == MBEDTLS_ERR_SSL_WANT_WRITE)
    {
        /* The TLS layer is busy with TLS-related overhead, the caller
         * should repeat the request.
         */
        cnt = 0;
    }
    else if (cnt < 0)
    {
        LogError(("coreHttp_send: ssl_send FAILED with %d\n", cnt));
        return cnt;
    }

    return cnt;
}
