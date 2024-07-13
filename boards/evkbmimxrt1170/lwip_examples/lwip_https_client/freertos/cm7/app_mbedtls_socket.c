/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

/* LwIP */
#include "ethernetif.h"
#include "lwip/err.h"
#include "lwip/ip_addr.h"
#include "lwip/netdb.h"
#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/tcpip.h"

/* App */
#include "app_ca_bundle.h"
#include "app_mbedtls_socket.h"

#ifndef MBEDTLS_DEBUG_THRESHOLD
#define MBEDTLS_DEBUG_THRESHOLD 0
#endif /* MBEDTLS_DEBUG_THRESHOLD */

static int app_mbedtls_socket_tcp_send(void *fd, unsigned char const *buf, size_t len);
static int app_mbedtls_socket_tcp_recv(void *fd, unsigned char const *buf, size_t len);
static const char *app_mbedtls_socket_strerr(int code);

#if defined(MBEDTLS_DEBUG_C) && (MBEDTLS_DEBUG_THRESHOLD > 0)
static void app_mbedtls_socket_debug(void *ctx, int level, const char *file, int line, const char *str);
#endif /* defined(MBEDTLS_DEBUG_C) && (MBEDTLS_DEBUG_THRESHOLD > 0) */

int app_mbedtls_socket_connect(void *param, char *host, uint16_t port)
{
    int ret;

    app_mbedtls_socket_t *sock = param;

    mbedtls_ssl_init(&sock->ctx_ssl);
    mbedtls_ssl_config_init(&sock->ssl_config);
    mbedtls_ctr_drbg_init(&sock->ctx_ctr_drbg);
    mbedtls_x509_crt_init(&sock->ca_cert);

    mbedtls_entropy_init(&sock->ctx_entropy);

    ret = mbedtls_ctr_drbg_seed(&sock->ctx_ctr_drbg, mbedtls_entropy_func, &sock->ctx_entropy, NULL, 0);
    if (ret != 0)
    {
        PRINTF("CTR DRBG seed failed with error: %s\r\n", app_mbedtls_socket_strerr(ret));

        return ret;
    }

    ret = mbedtls_x509_crt_parse(&sock->ca_cert, (const unsigned char *)g_app_ca_bundle, g_app_ca_bundle_len);
    if (ret != 0)
    {
        PRINTF("Certificate bundle parse failed with error: %s\r\n", app_mbedtls_socket_strerr(ret));

        return ret;
    }

    char port_str[6]; /* Sufficient for a 16-bit value */
    snprintf(port_str, 6, "%d", port);

    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    PRINTF("Resolving host %s...\r\n", host);
    ret = getaddrinfo(host, port_str, &hints, &res);
    if ((ret != 0) || (res == NULL))
    {
        PRINTF("Failed to resolve host %s with error %d\r\n", host, ret);

        return -1;
    }

    sock->lwip_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock->lwip_fd < 0)
    {
        return -2;
    }

    PRINTF("Connecting to %s:%d...\r\n", inet_ntoa(((struct sockaddr_in *)res->ai_addr)->sin_addr), port);
    ret = connect(sock->lwip_fd, res->ai_addr, res->ai_addrlen);

    freeaddrinfo(res);

    if (ret != 0)
    {
        PRINTF("Unable to connect() to %s:%d\r\n", inet_ntoa(((struct sockaddr_in *)res->ai_addr)->sin_addr), port);
        goto close_exit;
    }

    ret = mbedtls_ssl_config_defaults(&sock->ssl_config, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                      MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0)
    {
        PRINTF("Failed to preset SSL config with error: %s\r\n", app_mbedtls_socket_strerr(ret));

        goto close_exit;
    }

#if defined(MBEDTLS_DEBUG_C) && (MBEDTLS_DEBUG_THRESHOLD > 0)
    mbedtls_ssl_conf_dbg(&sock->ssl_config, app_mbedtls_socket_debug, NULL);
    mbedtls_debug_set_threshold(MBEDTLS_DEBUG_THRESHOLD);
#endif /* defined(MBEDTLS_DEBUG_C) && (MBEDTLS_DEBUG_THRESHOLD > 0) */

    mbedtls_ssl_conf_authmode(&sock->ssl_config, MBEDTLS_SSL_VERIFY_REQUIRED);
    mbedtls_ssl_conf_rng(&sock->ssl_config, mbedtls_ctr_drbg_random, &sock->ctx_ctr_drbg);
    mbedtls_ssl_conf_ca_chain(&sock->ssl_config, &sock->ca_cert, NULL);

    ret = mbedtls_ssl_setup(&sock->ctx_ssl, &sock->ssl_config);
    if (ret != 0)
    {
        PRINTF("Failed to setup SSL context with error: %s\r\n", app_mbedtls_socket_strerr(ret));

        goto close_exit;
    }

    ret = mbedtls_ssl_set_hostname(&sock->ctx_ssl, host);
    if (ret != 0)
    {
        PRINTF("Failed to set SSL hostname with error: %s\r\n", app_mbedtls_socket_strerr(ret));

        goto close_exit;
    }

    mbedtls_ssl_set_bio(&sock->ctx_ssl, &sock->lwip_fd, app_mbedtls_socket_tcp_send,
                        (mbedtls_ssl_recv_t *)app_mbedtls_socket_tcp_recv, NULL);

    PRINTF("Performing SSL/TLS handshake...\r\n");

    do
    {
        ret = mbedtls_ssl_handshake(&sock->ctx_ssl);

        if (ret != 0 && ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED)
            {
                PRINTF("SSL handshake: Cerficate verification failed.\r\n");
            }
            else
            {
                PRINTF("SSL handshake: Other error: %s\r\n", app_mbedtls_socket_strerr(ret));
            }
            goto close_exit;
        }
    } while (ret != 0);

    PRINTF("SSL: Protocol: %s, cipher suite: %s\r\n", mbedtls_ssl_get_version(&sock->ctx_ssl),
           mbedtls_ssl_get_ciphersuite(&sock->ctx_ssl));

    ret = mbedtls_ssl_get_record_expansion(&sock->ctx_ssl);
    if (ret == MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE)
    {
        PRINTF("SSL: Compression is used, record expansion is unavailable.\r\n");
    }
    else if (ret >= 0)
    {
        PRINTF("SSL: Record expansion: %d\r\n", ret);
    }

    if (mbedtls_ssl_get_peer_cert(&sock->ctx_ssl) != NULL)
    {
        char *buf = pvPortMalloc(MBEDTLS_SSL_MAX_CONTENT_LEN + 1);
        if (buf == NULL)
        {
            PRINTF("SSL: Failed to allocate certificate info buffer.\r\n");

            goto close_exit;
        }

        PRINTF("SSL: Peer certificate information:\n");
        mbedtls_x509_crt_info((char *)buf, MBEDTLS_SSL_MAX_CONTENT_LEN, "\r>>> ",
                              mbedtls_ssl_get_peer_cert(&sock->ctx_ssl));
        PRINTF("%s\r\n", buf);

        vPortFree(buf);
    }

    return 0;

close_exit:
    close(sock->lwip_fd);

    return ret;
}

void app_mbedtls_socket_close(void *param)
{
    app_mbedtls_socket_t *sock = param;

    close(sock->lwip_fd);

    mbedtls_x509_crt_free(&sock->ca_cert);
    mbedtls_ssl_free(&sock->ctx_ssl);
    mbedtls_ssl_config_free(&sock->ssl_config);
    mbedtls_ctr_drbg_free(&sock->ctx_ctr_drbg);
    mbedtls_entropy_free(&sock->ctx_entropy);
}

int app_mbedtls_socket_send(void *param, uint8_t *buf, int size)
{
    int ret;

    app_mbedtls_socket_t *sock = param;

    ret = mbedtls_ssl_write(&sock->ctx_ssl, buf, size);
    if (ret < 0)
    {
        PRINTF("SSL: send() failed with error: %s\r\n", app_mbedtls_socket_strerr(ret));
    }

    return ret;
}

int app_mbedtls_socket_recv(void *param, uint8_t *buf, int size)
{
    int ret;

    app_mbedtls_socket_t *sock = param;

    ret = mbedtls_ssl_read(&sock->ctx_ssl, buf, size);
    if (ret < 0)
    {
        PRINTF("SSL: recv() failed with error: %s\r\n", app_mbedtls_socket_strerr(ret));
    }

    return ret;
}

static int app_mbedtls_socket_tcp_send(void *fd, unsigned char const *buf, size_t len)
{
    return lwip_send((*(int *)fd), buf, len, 0);
}

static int app_mbedtls_socket_tcp_recv(void *fd, unsigned char const *buf, size_t len)
{
    return lwip_recv((*(int *)fd), (void *)buf, len, 0);
}

static const char *app_mbedtls_socket_strerr(int code)
{
    static char buffer[8];
    const char *msg;

    msg = mbedtls_high_level_strerr(code);
    if (msg != NULL)
    {
        return msg;
    }

    msg = mbedtls_low_level_strerr(code);
    if (msg != NULL)
    {
        return msg;
    }
    else if (code < 0)
    {
        snprintf(buffer, sizeof(buffer), "-0x%04x", -code);
        return buffer;
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "0x%04x", code);
        return buffer;
    }
}

#if defined(MBEDTLS_DEBUG_C) && (MBEDTLS_DEBUG_THRESHOLD > 0)
static void app_mbedtls_socket_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    const char *file_name = file;

    (void)ctx;
    (void)level;

    /* Extract file name from path */
    for (; *file != '\0'; file++)
    {
        if (*file == '/' || *file == '\\')
        {
            file_name = file + 1;
        }
    }

    mbedtls_printf("%s:%04d: %s", file_name, line, str);
}
#endif /* defined(MBEDTLS_DEBUG_C) && (MBEDTLS_DEBUG_THRESHOLD > 0) */
