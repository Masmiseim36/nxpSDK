/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_HTTPS_CLIENT_TASK_H__
#define __APP_HTTPS_CLIENT_TASK_H__

#include <stddef.h>

typedef struct https_client_task_context
{
    char *host;
    uint16_t port;
    char *path;
    size_t max_request_len;
    size_t max_response_len;
} https_client_task_context_t;

void app_https_client_task_init(https_client_task_context_t *context);

#endif /* __APP_HTTPS_CLIENT_TASK_H__ */
