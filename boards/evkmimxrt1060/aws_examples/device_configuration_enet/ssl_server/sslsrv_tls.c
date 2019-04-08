/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*   This file contains the SSLSRV TLS implementation.*/
#include "sslsrv_tls.h"
#include "sslsrv_port.h"
#include "sslsrv_supp.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"

extern void aws_mbedtls_mutex_init(mbedtls_threading_mutex_t *mutex);
extern void aws_mbedtls_mutex_free(mbedtls_threading_mutex_t *mutex);
extern int aws_mbedtls_mutex_lock(mbedtls_threading_mutex_t *mutex);
extern int aws_mbedtls_mutex_unlock(mbedtls_threading_mutex_t *mutex);
#endif

#if defined(MBEDTLS_DEBUG_C)
/* Mbedtls debug */
static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void)level);

    printf("%s", str);
}
#endif

/* Initialize TLS */
sslsrv_tls_ctx_t sslsrv_tls_init(const SSLSRV_TLS_PARAM_STRUCT *params)
{
    sslsrv_tls_ctx_t ctx = NULL;

    if (params)
    {
        const char *pers = "ssl_server";

        ctx = sslsrv_mem_alloc_zero(sizeof(*ctx));
        if (ctx)
        {
#if defined(MBEDTLS_THREADING_C)
            mbedtls_threading_set_alt(aws_mbedtls_mutex_init, aws_mbedtls_mutex_free, aws_mbedtls_mutex_lock,
                                      aws_mbedtls_mutex_unlock);
#endif

            mbedtls_ssl_config_init(&ctx->conf);
#if defined(MBEDTLS_SSL_CACHE_C)
            mbedtls_ssl_cache_init(&ctx->cache);
#endif
            mbedtls_x509_crt_init(&ctx->srvcert);
            mbedtls_pk_init(&ctx->pkey);
            mbedtls_entropy_init(&ctx->entropy);
            mbedtls_ctr_drbg_init(&ctx->ctr_drbg);

#if defined(MBEDTLS_DEBUG_C)
            mbedtls_debug_set_threshold(0); /* Set this to >= 2 to see debug info */
#endif

            if (params->certificate_buffer && params->certificate_buffer_size && params->private_key_buffer &&
                params->private_key_buffer_size)
            {
                if (mbedtls_x509_crt_parse(&ctx->srvcert, params->certificate_buffer,
                                           params->certificate_buffer_size) != 0)
                {
                    goto ERROR;
                }

                if (mbedtls_pk_parse_key(&ctx->pkey, params->private_key_buffer, params->private_key_buffer_size, NULL,
                                         0) != 0)
                {
                    goto ERROR;
                }

                /* Seed the RNG  */
                if (mbedtls_ctr_drbg_seed(&ctx->ctr_drbg, mbedtls_entropy_func, &ctx->entropy,
                                          (const unsigned char *)pers, strlen(pers)) != 0)
                {
                    goto ERROR;
                }

                /* Setup stuff */
                if (mbedtls_ssl_config_defaults(&ctx->conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM,
                                                MBEDTLS_SSL_PRESET_DEFAULT) != 0)
                {
                    goto ERROR;
                }

                mbedtls_ssl_conf_rng(&ctx->conf, mbedtls_ctr_drbg_random, &ctx->ctr_drbg);
//#if 0 /* Debug info.*/
#if defined(MBEDTLS_DEBUG_C)
                mbedtls_ssl_conf_dbg(&ctx->conf, my_debug, NULL);
#endif
#if defined(MBEDTLS_SSL_CACHE_C)
                mbedtls_ssl_conf_session_cache(&ctx->conf, &ctx->cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif
                mbedtls_ssl_conf_ca_chain(&ctx->conf, ctx->srvcert.next, NULL);
                if (mbedtls_ssl_conf_own_cert(&ctx->conf, &ctx->srvcert, &ctx->pkey) != 0)
                {
                    goto ERROR;
                }
            }
        }
    }
    return ctx;

ERROR:
    if (ctx)
    {
        sslsrv_tls_release(ctx);
    }

    return NULL;
}

/* Release TLS */
void sslsrv_tls_release(sslsrv_tls_ctx_t ctx)
{
    if (ctx)
    {
        mbedtls_x509_crt_free(&ctx->srvcert);
        mbedtls_pk_free(&ctx->pkey);
        mbedtls_ssl_config_free(&ctx->conf);
#if defined(MBEDTLS_SSL_CACHE_C)
        mbedtls_ssl_cache_free(&ctx->cache);
#endif
        mbedtls_ctr_drbg_free(&ctx->ctr_drbg);
        mbedtls_entropy_free(&ctx->entropy);

        sslsrv_mem_free(ctx);
    }
}

/* Create TLS connection using sock, return TLS connection handle. */
sslsrv_tls_sock_t sslsrv_tls_socket(sslsrv_tls_ctx_t ctx, int sock)
{
    sslsrv_tls_sock_t tls_sock;

    tls_sock = sslsrv_mem_alloc_zero(sizeof(*tls_sock));
    if (tls_sock)
    {
        mbedtls_ssl_init(tls_sock);

        if (mbedtls_ssl_setup(tls_sock, &ctx->conf) != 0)
        {
            sslsrv_mem_free(tls_sock);
            tls_sock = 0;
        }
        else
        {
            int ret;

            /* set IO functions */
            mbedtls_ssl_set_bio(tls_sock, (void *)sock, sslsrv_mbedtls_send, sslsrv_mbedtls_recv, NULL);

            /* perform handshake */
            ret = mbedtls_ssl_handshake(tls_sock);
            if (ret < 0)
            {
#if 0
                PRINTF(" failed\r\n  ! mbedtls_ssl_handshake error %d\r\n\n", ret);
#endif
                sslsrv_tls_shutdown(tls_sock);
                tls_sock = 0;
            }
        }
    }
    return tls_sock;
}

/* Shutdown SSL connection. */
void sslsrv_tls_shutdown(sslsrv_tls_sock_t tls_sock)
{
    if (tls_sock)
    {
        /* Shuts down an active SSL/TLS connection */
        mbedtls_ssl_close_notify(tls_sock);
        /* Free TLS socket.*/
        mbedtls_ssl_free(tls_sock);

        sslsrv_mem_free(tls_sock);
    }
}
