/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_COREHTTP_H__
#define __APP_COREHTTP_H__

#include <stdint.h>

/* coreHTTP */
#include "core_http_client.h"

NetworkContext_t *app_corehttp_create_context(char *host, uint16_t port);
void app_corehttp_destroy_context(NetworkContext_t *ctx);
int32_t app_corehttp_send(NetworkContext_t *ctx, const void *buf, size_t size);
int32_t app_corehttp_recv(NetworkContext_t *ctx, void *buf, size_t size);

#endif /* __APP_COREHTTP_H__ */
