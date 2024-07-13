/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_MBEDTLS_SOCKET_H__
#define __APP_MBEDTLS_SOCKET_H__

#include <stdint.h>

/* MbedTLS Config */
#ifdef MBEDTLS_MCUX_ELE_S400_API
#include "ele_mbedtls.h"
#else
#include "ksdk_mbedtls.h"
#endif /* MBEDTLS_MCUX_ELE_S400_API */

/* MbedTLS */
#include "mbedtls/certs.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/x509.h"

typedef struct
{
    mbedtls_entropy_context ctx_entropy;
    mbedtls_ctr_drbg_context ctx_ctr_drbg;
    mbedtls_ssl_context ctx_ssl;
    mbedtls_ssl_config ssl_config;
    mbedtls_x509_crt ca_cert;
    int lwip_fd;
} app_mbedtls_socket_t;

int app_mbedtls_socket_connect(void *param, char *host, uint16_t port);
void app_mbedtls_socket_close(void *param);
int app_mbedtls_socket_send(void *param, uint8_t *buf, int size);
int app_mbedtls_socket_recv(void *param, uint8_t *buf, int size);

#endif /* __APP_MBEDTLS_SOCKET_H__ */
