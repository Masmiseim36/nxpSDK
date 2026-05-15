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
#include "app_corehttp.h"
#include "app_mbedtls_socket.h"

/* Debug Console */
#include "fsl_debug_console.h"

typedef struct
{
    app_mbedtls_socket_t tls;
} app_corehttp_ctx_t;

NetworkContext_t *app_corehttp_create_context(char *host, uint16_t port)
{
    app_corehttp_ctx_t *ctx = pvPortMalloc(sizeof(app_corehttp_ctx_t));
    if (ctx == NULL)
    {
        PRINTF("NetworkCtx: Failed to allocate context.\r\n");
        return NULL;
    }

    int ret = app_mbedtls_socket_connect(&ctx->tls, host, port);
    if (ret != 0)
    {
        PRINTF("NetworkCtx: Failed to establish TLS connection to %s:%d, ret=%d.\r\n", host, port, ret);
        goto free_exit;
    }

    PRINTF("NetworkCtx: Connection established.\r\n");

    return (NetworkContext_t *)ctx;

free_exit:
    vPortFree(ctx);
    return NULL;
}

void app_corehttp_destroy_context(NetworkContext_t *ctx)
{
    app_corehttp_ctx_t *c = (app_corehttp_ctx_t *)ctx;

    app_mbedtls_socket_close(&c->tls);
    PRINTF("NetworkCtx: Connection closed.\r\n");

    vPortFree(c);
}

int32_t app_corehttp_send(NetworkContext_t *ctx, const void *buf, size_t size)
{
    app_corehttp_ctx_t *c = (app_corehttp_ctx_t *)ctx;

    return app_mbedtls_socket_send(&c->tls, (uint8_t *)buf, size);
}

int32_t app_corehttp_recv(NetworkContext_t *ctx, void *buf, size_t size)
{
    app_corehttp_ctx_t *c = (app_corehttp_ctx_t *)ctx;

    return app_mbedtls_socket_recv(&c->tls, (uint8_t *)buf, size);
}
