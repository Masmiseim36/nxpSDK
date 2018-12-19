/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*   Header for SSLSRV TLS.*/

#ifndef _SSLSRV_TLS_H_
#define _SSLSRV_TLS_H_

#include "sslsrv_config.h"
#include "sslsrv.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/debug.h"

typedef mbedtls_ssl_context *sslsrv_tls_sock_t;

typedef struct
{
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt srvcert;
    mbedtls_pk_context pkey;
#if defined(MBEDTLS_SSL_CACHE_C)
    mbedtls_ssl_cache_context cache;
#endif
} * sslsrv_tls_ctx_t;

#ifdef __cplusplus
extern "C" {
#endif

sslsrv_tls_ctx_t sslsrv_tls_init(const SSLSRV_TLS_PARAM_STRUCT *params);
void sslsrv_tls_release(sslsrv_tls_ctx_t ctx);

sslsrv_tls_sock_t sslsrv_tls_socket(sslsrv_tls_ctx_t ctx, int sock);
void sslsrv_tls_shutdown(sslsrv_tls_sock_t tls_sock);

// int sslsrv_tls_recv(sslsrv_tls_sock_t tls_sock, void *buf, size_t len, int flags);
// int sslsrv_tls_send(sslsrv_tls_sock_t tls_sock, const void *buf, size_t len, int flags);

#ifdef __cplusplus
}
#endif

#endif /* _SSLSRV_TLS_H_ */
