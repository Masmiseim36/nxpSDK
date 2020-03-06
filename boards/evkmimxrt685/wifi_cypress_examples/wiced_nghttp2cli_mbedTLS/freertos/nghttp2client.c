/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/sockets.h"
#include "nghttp2client.h"
#include "lwip/netdb.h"
#include "fsl_debug_console.h"
#include <stdlib.h>
#include <stdio.h>

#include "nghttp2/nghttp2.h"
#include "FreeRTOS.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SERVER_VERIFICATION 0
/* This is the value used for ssl read timeout */
#define IOT_SSL_READ_TIMEOUT 10
#define GET_REQUEST "GET"
#define DEBUG_LEVEL 0
#define DEBUG_CALLBACKS 0
#define NV(name, value, valueLen)                                                           \
    {                                                                                       \
        (uint8_t *)name, (uint8_t *)value, sizeof(name) - 1, valueLen, NGHTTP2_NV_FLAG_NONE \
    }

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
int8_t g_closeConnection = 0;

/* Comment macro definition configTOTAL_HEAP_SIZE when heap3 or less is used. */
/* Data types used in heap4 to get information about block size. */
#if defined(configTOTAL_HEAP_SIZE)
typedef struct A_BLOCK_LINK
{
    struct A_BLOCK_LINK *pxNextFreeBlock; /*<< The next free block in the list. */
    size_t xBlockSize;                    /*<< The size of the free block. */
} BlockLink_t;
static const size_t xHeapStructSize =
    (sizeof(BlockLink_t) + ((size_t)(portBYTE_ALIGNMENT - 1))) & ~((size_t)portBYTE_ALIGNMENT_MASK);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Send function used by mbedtls ssl */
static int lwip_send_wrapper(void *fd, unsigned char const *buf, size_t len)
{
    int ret = lwip_send((*(int *)fd), buf, len, 0);
    return ret;
}

/* Send function used by mbedtls ssl */
static int lwip_recv_wrapper(void *fd, unsigned char *buf, size_t len)
{
    int ret = lwip_recv((*(int *)fd), (void *)buf, len, 0);
    return ret;
}

static int _iot_tls_verify_cert(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
    char buf[1024];
    ((void)data);

    PRINTF("\nVerify requested for (Depth %d):\n", depth);
    mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);
    PRINTF("%s", buf);

    if ((*flags) == 0)
    {
        PRINTF("  This certificate has no flags\n");
    }
    else
    {
        PRINTF(buf, sizeof(buf), "  ! ", *flags);
        PRINTF("%s\n", buf);
    }

    return 0;
}

#ifdef MBEDTLS_DEBUG_C
static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void)level);
    PRINTF("%s\n", str);

    PRINTF("\r\n%s, at line %d in file %s\n", str, line, file);
}
#endif

static void print_header(const uint8_t *name, size_t namelen, const uint8_t *value, size_t valuelen)
{
    PRINTF("%s: %s\n", name, value);
}

static void print_headers(nghttp2_nv *nva, size_t nvlen)
{
    int i;
    for (i = 0; i < nvlen; ++i)
    {
        print_header(nva[i].name, nva[i].namelen, nva[i].value, nva[i].valuelen);
    }
    PRINTF("\n");
}

/* nghttp2_send_callback: This callback is used to send data.  */
static int on_send_callback(nghttp2_session *session, const uint8_t *data, size_t length, int flags, void *user_data)
{
    int ret;

    if (DEBUG_CALLBACKS)
    {
        PRINTF("entered on_send_callback, writing %d bytes\n", length);
    }
    nghttp2_user_session_t *userSession = (nghttp2_user_session_t *)user_data;
    while ((ret = mbedtls_ssl_write(&(userSession->con.tlsDataParams.ssl), data, length)) <= 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            PRINTF("mbedtls_ssl_write returned -0x%x\n", -ret);
            return ret;
        }
    }

    return ret;
}

/* nghttp2_on_header_callback: Called when nghttp2 library emits
 single header name/value pair. */
static int on_header_callback(nghttp2_session *session,
                              const nghttp2_frame *frame,
                              const uint8_t *name,
                              size_t namelen,
                              const uint8_t *value,
                              size_t valuelen,
                              uint8_t flags,
                              void *user_data)
{
    if (DEBUG_CALLBACKS)
    {
        PRINTF("on_header_callback %d\n", frame->hd.type);
    }
    switch (frame->hd.type)
    {
        case NGHTTP2_HEADERS:
            if (frame->headers.cat == NGHTTP2_HCAT_RESPONSE)
            {
                /* Print response headers for the initiated request. */
                print_header(name, namelen, value, valuelen);
            }
            break;
    }
    return 0;
}

/* nghttp2_on_begin_headers_callback: Called when nghttp2 library gets
 started to receive header block. */
static int on_begin_headers_callback(nghttp2_session *session, const nghttp2_frame *frame, void *user_data)
{
    if (DEBUG_CALLBACKS)
    {
        PRINTF("on_begin_headers_callback %d\n", frame->hd.type);
    }
    switch (frame->hd.type)
    {
        case NGHTTP2_HEADERS:
            if (frame->headers.cat == NGHTTP2_HCAT_RESPONSE)
            {
                PRINTF("Response headers for stream ID=%d:\n", frame->hd.stream_id);
            }
            break;
    }
    return 0;
}

/* nghttp2_on_frame_recv_callback: Called when nghttp2 library
 received a complete frame from the remote peer. */
static int on_frame_recv_callback(nghttp2_session *session, const nghttp2_frame *frame, void *user_data)
{
    if (DEBUG_CALLBACKS)
    {
        PRINTF("on_frame_recv_callback %d\n", frame->hd.type);
    }
    switch (frame->hd.type)
    {
        case NGHTTP2_HEADERS:
            print_headers(frame->headers.nva, frame->headers.nvlen);
            if (frame->headers.cat == NGHTTP2_HCAT_RESPONSE)
            {
                PRINTF("Headers received\n");
            }
            break;

        case NGHTTP2_GOAWAY:
            PRINTF("NGHTTP2_GOAWAY TBD err:%d\n", frame->goaway.error_code);
            break;
    }
    return 0;
}

/* nghttp2_on_data_chunk_recv_callback: Called when DATA frame is
 received from the remote peer. */
static int on_data_chunk_recv_callback(
    nghttp2_session *session, uint8_t flags, int32_t stream_id, const uint8_t *data, size_t len, void *user_data)
{
    if (DEBUG_CALLBACKS)
    {
        PRINTF("on_data_chunk_recv_callback\n");
    }
    PRINTF("%.*s\n", len, data);

    return 0;
}

/* nghttp2_on_stream_close_callback: Called when a stream is about to
 closed. If the last stream is closed, we send GOAWAY and tear down the
 session */
static int on_stream_close_callback(nghttp2_session *session, int32_t stream_id, uint32_t error_code, void *user_data)
{
    int rv;
    PRINTF("closed stream %d with error_code=%u - %s\n", stream_id, error_code, nghttp2_http2_strerror(error_code));
    g_closeConnection++;
    rv = nghttp2_session_terminate_session(session, NGHTTP2_NO_ERROR);
    if (rv != 0)
    {
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }
    return 0;
}

/* nghttp2_on_error_callback2: Called when an error occured.
 Print error message. */
int on_error_callback2(nghttp2_session *session, int lib_error_code, const char *msg, size_t len, void *user_data)
{
    PRINTF("nghttp2_on_error_callback2 %d: %s %s \n", lib_error_code, msg, session->iframe.sbuf.pos);
    return 0;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

/* fixes assertion error in:
 * assert(nghttp2_buf_avail(buf) >= datamax);
 */
ssize_t on_data_source_read_length_callback(nghttp2_session *session,
                                            uint8_t frame_type,
                                            int32_t stream_id,
                                            int32_t session_remote_window_size,
                                            int32_t stream_remote_window_size,
                                            uint32_t remote_max_frame_size,
                                            void *user_data)
{
    PRINTF("on_data_source_read_length_callback\n");
    ssize_t len = 2048;
    len         = min(len, session_remote_window_size);
    len         = min(len, stream_remote_window_size);
    len         = min(len, remote_max_frame_size);

    return len;
}

/* Function which is replacing default malloc function with FreeRTOS pvPortMalloc. */
static void *freertos_alloc(size_t size, void *mem_user_data)
{
    (void)mem_user_data;

    return pvPortMalloc(size);
}

/* Function which is replacing default free function with FreeRTOS vPortFree. */
static void freertos_free(void *ptr, void *mem_user_data)
{
    (void)mem_user_data;

    vPortFree(ptr);
}

/* Comment macro definition configTOTAL_HEAP_SIZE when heap3 or less is used. */
#if defined(configTOTAL_HEAP_SIZE)

/* Function which is replacing default calloc function. */
static void *freertos_calloc(size_t nmemb, size_t size, void *mem_user_data)
{
    (void)mem_user_data;
    void *ptr;
    int allocSize = nmemb * size;

    ptr = pvPortMalloc(allocSize);
    if (ptr != NULL)
    {
        memset(ptr, 0, allocSize);
    }

    return ptr;
}

/* Function which is replacing default calloc function for mbedtls. */
void *freertos_calloc_mbedtls(size_t nmemb, size_t size)
{
    return freertos_calloc(nmemb, size, NULL);
}

/* Function which is replacing default realloc function. */
static void *freertos_realloc(void *ptr, size_t size, void *mem_user_data)
{
    (void)mem_user_data;

    if (ptr == NULL)
    {
        ptr = pvPortMalloc(size);
    }
    else
    {
        vTaskSuspendAll();
        {
            /* The memory will have an BlockLink_t structure immediately
            before it. */
            uint8_t *puc = (uint8_t *)ptr;
            BlockLink_t *pxLink;

            puc -= xHeapStructSize;

            /* This casting is to keep the compiler from issuing warnings. */
            pxLink = (void *)puc;

            int dif = pxLink->xBlockSize - sizeof(xHeapStructSize);
            if (dif < size)
            {
                void *ptrResult = pvPortMalloc(size);
                if (ptrResult != NULL)
                {
                    vPortFree(ptr);
                }
                ptr = ptrResult;
            }
        }
        (void)xTaskResumeAll();
    }

    return ptr;
}

#else /* configTOTAL_HEAP_SIZE */

/* Function which is replacing default calloc function with using mbedtls calloc implementation. */
extern void *pvPortCalloc(size_t num, size_t size);
static void *freertos_calloc(size_t nmemb, size_t size, void *mem_user_data)
{
    return pvPortCalloc(nmemb, size);
}

/* Function which is replacing default realloc function. */
static void *freertos_realloc(void *ptr, size_t size, void *mem_user_data)
{
    (void)mem_user_data;

    vTaskSuspendAll();
    {
        ptr = realloc(ptr, size);
        traceMALLOC(ptr, size);
    }
    (void)xTaskResumeAll();

#if (configUSE_MALLOC_FAILED_HOOK == 1)
    {
        if (ptr == NULL)
        {
            extern void vApplicationMallocFailedHook(void);
            vApplicationMallocFailedHook();
        }
    }
#endif /* configUSE_MALLOC_FAILED_HOOK */

    return ptr;
}

#endif /* configTOTAL_HEAP_SIZE */

int tls_init(nghttp2_con_info_t *con)
{
    int ret = 0;
#if SERVER_VERIFICATION
    char vrfyBuf[512];
#endif
    const mbedtls_md_info_t *mdInfo;
    TLSDataParams *tlsDataParams = &con->tlsDataParams;

#ifdef MBEDTLS_DEBUG_C
    unsigned char *buf = freertos_alloc(MBEDTLS_SSL_MAX_CONTENT_LEN + 1, NULL);
    if (buf == NULL)
    {
        PRINTF("Memory alloc failed - buf\n");
        return FAILURE;
    }
#endif

    mbedtls_ssl_init(&(tlsDataParams->ssl));
    mbedtls_ssl_config_init(&(tlsDataParams->conf));
    mbedtls_hmac_drbg_init(&(tlsDataParams->hmac_drbg));

#if defined(MBEDTLS_DEBUG_C)
    /* Enable debug output of mbedtls */
    mbedtls_ssl_conf_dbg(&(tlsDataParams->conf), my_debug, NULL);
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

    PRINTF("\n  . Seeding the random number generator...\n");
    mbedtls_entropy_init(&(tlsDataParams->entropy));
    mdInfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if ((ret = mbedtls_hmac_drbg_seed(&(tlsDataParams->hmac_drbg), mdInfo, mbedtls_entropy_func,
                                      &(tlsDataParams->entropy), NULL, 0)) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_hmac_drbg_seed returned -0x%x\n", -ret);
        return NETWORK_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED;
    }

    /* configure ALPN */
    const char *alpnList[3] = {0};
    memset((void *)alpnList, 0, sizeof(alpnList));
    alpnList[0] = "h2";
    alpnList[1] = "http/1.1";
    alpnList[2] = NULL;

    mbedtls_ssl_conf_alpn_protocols(&(tlsDataParams->conf), alpnList);

    /* restrict cypher suites */
    int cypher[3] = {0};
    memset((void *)cypher, 0, sizeof(cypher));
    cypher[0] = MBEDTLS_TLS_DHE_RSA_WITH_AES_128_GCM_SHA256;
    cypher[1] = MBEDTLS_TLS_DHE_RSA_WITH_AES_256_GCM_SHA384;
    cypher[2] = (int)NULL;
    mbedtls_ssl_conf_ciphersuites(&(tlsDataParams->conf), cypher);

    PRINTF("Connecting to %s/%d\n", con->serverName, con->serverPort);

    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;
#ifdef AI_ADDRCONFIG
    hints.ai_flags |= AI_ADDRCONFIG;
#endif /* AI_ADDRCONFIG */
    char portS[10] = {0};
    sprintf(portS, "%d", con->serverPort);

    ret = getaddrinfo(con->serverName, portS, &hints, &res);
    if ((ret != 0) || (res == NULL))
    {
        return NETWORK_ERR_NET_UNKNOWN_HOST;
    }

    tlsDataParams->fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (tlsDataParams->fd < 0)
    {
        return NETWORK_ERR_NET_SOCKET_FAILED;
    }

    ret = connect(tlsDataParams->fd, res->ai_addr, res->ai_addrlen);

    freeaddrinfo(res);

    if (ret != 0)
    {
        close(tlsDataParams->fd);
        return NETWORK_ERR_NET_CONNECT_FAILED;
    }

    if ((ret = mbedtls_ssl_set_hostname(&(tlsDataParams->ssl), con->serverName)) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", ret);
        return SSL_CONNECTION_ERROR;
    }

    PRINTF("  . Setting up the SSL/TLS structure...\n");
    if ((ret = mbedtls_ssl_config_defaults(&(tlsDataParams->conf), MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_ssl_config_defaults returned -0x%x\n\n", -ret);
        return SSL_CONNECTION_ERROR;
    }

#if SERVER_VERIFICATION
    mbedtls_ssl_conf_authmode(&(tlsDataParams->conf), MBEDTLS_SSL_VERIFY_REQUIRED);
#else
    /* mbedtls_ssl_conf_authmode(&(tlsDataParams->conf), MBEDTLS_SSL_VERIFY_OPTIONAL); */
    mbedtls_ssl_conf_authmode(&(tlsDataParams->conf), MBEDTLS_SSL_VERIFY_NONE);
#endif

    mbedtls_ssl_conf_verify(&(tlsDataParams->conf), _iot_tls_verify_cert, NULL);

    mbedtls_ssl_conf_rng(&(tlsDataParams->conf), mbedtls_hmac_drbg_random, &(tlsDataParams->hmac_drbg));

    if ((ret = mbedtls_ssl_setup(&(tlsDataParams->ssl), &(tlsDataParams->conf))) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_ssl_setup returned -0x%x\n\n", -ret);
        return SSL_CONNECTION_ERROR;
    }
    PRINTF("\n\nSSL state connect : %d ", tlsDataParams->ssl.state);

    mbedtls_ssl_set_bio(&(tlsDataParams->ssl), &(tlsDataParams->fd), lwip_send_wrapper, lwip_recv_wrapper, NULL);

    PRINTF(" ok\n");
    PRINTF("\n\nSSL state connect : %d \n", tlsDataParams->ssl.state);
    PRINTF("  . Performing the SSL/TLS handshake...\n");
    while ((ret = mbedtls_ssl_handshake(&(tlsDataParams->ssl))) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            PRINTF(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n", -ret);
            if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED)
            {
                PRINTF(
                    "    Unable to verify the server's certificate. "
                    "    Alternatively, you may want to use "
                    "auth_mode=optional for testing purposes.\n");
            }
            return SSL_CONNECTION_ERROR;
        }
    }

    PRINTF(" ok\n    [ Protocol is %s ]\n    [ Ciphersuite is %s ]\n", mbedtls_ssl_get_version(&(tlsDataParams->ssl)),
           mbedtls_ssl_get_ciphersuite(&(tlsDataParams->ssl)));

    /* ALPN negotiated protocol */
    const char *alp = mbedtls_ssl_get_alpn_protocol(&(tlsDataParams->ssl));
    PRINTF("    [ Application Layer Protocol is %s ]\n", alp ? alp : "(none)");

    if ((ret = mbedtls_ssl_get_record_expansion(&(tlsDataParams->ssl))) >= 0)
    {
        PRINTF("    [ Record expansion is %d ]\n", ret);
    }
    else
    {
        PRINTF("    [ Record expansion is unknown (compression) ]\n");
    }

    PRINTF("  . Verifying peer X.509 certificate...\n");

#if SERVER_VERIFICATION
    if ((tlsDataParams->flags = mbedtls_ssl_get_verify_result(&(tlsDataParams->ssl))) != 0)
    {
        PRINTF(" failed\n");
        mbedtls_x509_crt_verify_info(vrfyBuf, sizeof(vrfyBuf), "  ! ", tlsDataParams->flags);
        PRINTF("%s\n", vrfyBuf);
        ret = SSL_CONNECTION_ERROR;
    }
    else
    {
        PRINTF(" ok\n");
        ret = SUCCESS;
    }
#else
    PRINTF(" Server Verification skipped\n");
    ret = SUCCESS;
#endif

#ifdef MBEDTLS_DEBUG_C
    if (mbedtls_ssl_get_peer_cert(&(tlsDataParams->ssl)) != NULL)
    {
        PRINTF("  . Peer certificate information    ...\n");
        mbedtls_x509_crt_info((char *)buf, sizeof(buf) - 1, "      ", mbedtls_ssl_get_peer_cert(&(tlsDataParams->ssl)));
        PRINTF("%s\n", buf);
    }
    freertos_free(buf, NULL);
#endif

    mbedtls_ssl_conf_read_timeout(&(tlsDataParams->conf), IOT_SSL_READ_TIMEOUT);

    return ret;
}

int nghttp2_init(nghttp2_user_session_t *userSession)
{
    int ret                                    = 0;
    nghttp2_session **nghttp2Session           = &userSession->nghttp2Session;
    nghttp2_session_callbacks sessionCallbacks = {0};

    nghttp2_session_callbacks_set_send_callback(&sessionCallbacks, on_send_callback);
    nghttp2_session_callbacks_set_on_frame_recv_callback(&sessionCallbacks, on_frame_recv_callback);
    nghttp2_session_callbacks_set_on_data_chunk_recv_callback(&sessionCallbacks, on_data_chunk_recv_callback);
    nghttp2_session_callbacks_set_on_stream_close_callback(&sessionCallbacks, on_stream_close_callback);
    nghttp2_session_callbacks_set_on_header_callback(&sessionCallbacks, on_header_callback);
    nghttp2_session_callbacks_set_on_begin_headers_callback(&sessionCallbacks, on_begin_headers_callback);
    nghttp2_session_callbacks_set_data_source_read_length_callback(&sessionCallbacks,
                                                                   on_data_source_read_length_callback);
    nghttp2_session_callbacks_set_error_callback2(&sessionCallbacks, on_error_callback2);

    nghttp2_mem mem = {NULL, freertos_alloc, freertos_free, freertos_calloc, freertos_realloc};
    ret             = nghttp2_session_client_new3(nghttp2Session, &sessionCallbacks, userSession, NULL, &mem);
    if (ret)
    {
        PRINTF("Register callback fails: %d\n", ret);
    }

    PRINTF("Writing HTTP request...\n");

    /* client settings */
    nghttp2_settings_entry iv[1] = {{NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, 1}};

    /* client 24 bytes magic string will be sent by nghttp2 library */
    /* submit request is actually not sending, just preparing. All data are sent in while loop below. */
    int rv = nghttp2_submit_settings(*nghttp2Session, NGHTTP2_FLAG_NONE, iv, ARRAY_SIZE(iv));
    if (rv != 0)
    {
        PRINTF("Could not submit SETTINGS: %s", nghttp2_strerror(rv));
        return rv;
    }

    int32_t streamId;
    void *streamUserData           = NULL;
    nghttp2_data_provider *dataPrd = NULL;
    char *method                   = GET_REQUEST;

    /* create headers */
    nghttp2_nv hdrs[] = {NV(":method", method, strlen(method)),
                         NV(":scheme", userSession->con.serverScheme, strlen(userSession->con.serverScheme)),
                         NV(":authority", userSession->con.serverAuthority, strlen(userSession->con.serverAuthority)),
                         NV(":path", userSession->con.serverPagePath, strlen(userSession->con.serverPagePath))};

    int sizeHDRS = ARRAY_SIZE(hdrs);

    PRINTF("Headers requested:\n");
    print_headers(hdrs, sizeHDRS);

    /* submit request is actually not sending, just preparing. All data are sent in while loop below. */
    streamId = nghttp2_submit_request(*nghttp2Session, NULL, hdrs, sizeHDRS, dataPrd, streamUserData);

    if (streamId < 0)
    {
        PRINTF("Could not submit HTTP request: %s\n", nghttp2_strerror(streamId));
        return streamId;
    }
    PRINTF("created new stream: %d\n", streamId);

    return ret;
}

int nghttp2_start(nghttp2_user_session_t *userSession)
{
    if (userSession == NULL)
    {
        return FAILURE;
    }

    int ret                         = 0;
    nghttp2_session *nghttp2Session = userSession->nghttp2Session;
    TLSDataParams *tlsDataParams    = &userSession->con.tlsDataParams;
    unsigned char *buf              = freertos_alloc(MBEDTLS_SSL_MAX_CONTENT_LEN + 1, NULL);
    if (buf == NULL)
    {
        PRINTF("Memory alloc failed - buf\n");
        return FAILURE;
    }

    /* send receive messages */
    while (!g_closeConnection || nghttp2_session_want_write(nghttp2Session) ||
           nghttp2_session_want_read(nghttp2Session))
    {
        if (nghttp2_session_want_write(nghttp2Session))
        {
            ret = nghttp2_session_send(nghttp2Session);
            if (ret != 0)
            {
                PRINTF("Fatal error: %s\n", nghttp2_strerror(ret));
                break;
            }
        }

        /* avoid blocking read */
        if (!nghttp2_session_want_read(nghttp2Session))
        {
            continue;
        }

        ret = mbedtls_ssl_read(&(tlsDataParams->ssl), buf, MBEDTLS_SSL_MAX_CONTENT_LEN);

        if (ret < 0)
        {
            PRINTF("failed\n  ! mbedtls_ssl_read returned %d\n\n", ret);
            break;
        }

        buf[ret] = '\0';

        ret = nghttp2_session_mem_recv(nghttp2Session, buf, ret);

        if (!ret)
        {
            PRINTF("nghttp2_session_mem_recv error\n");
            break;
        }
    }
    freertos_free(buf, NULL);
    return ret;
}

/* Release TLS */
void nghttp2_release(nghttp2_user_session_t *userSession)
{
    if (userSession->nghttp2Session)
    {
        nghttp2_session_del(userSession->nghttp2Session);
    }
}

/* Release TLS */
void tls_release(nghttp2_con_info_t *con)
{
    TLSDataParams *tlsDataParams = &con->tlsDataParams;
    close(tlsDataParams->fd);
    mbedtls_ssl_free(&(tlsDataParams->ssl));
    mbedtls_ssl_config_free(&(tlsDataParams->conf));
    mbedtls_hmac_drbg_free(&(tlsDataParams->hmac_drbg));
    mbedtls_entropy_free(&(tlsDataParams->entropy));
}

void nghttp2_client(void)
{
    nghttp2_user_session_t *userSession = freertos_alloc(sizeof(nghttp2_user_session_t), NULL);
    int ret;

    if (userSession == NULL)
    {
        PRINTF("Memory alloc failed - userSession\n");
        return;
    }

    /* Official test server. For testing purposes can be used also local server
     executed on Linux maschine. Source files are located in nghttp2 repository. */
    const char *HTTPS_SERVER_SCHEME = "https";
    const char *HTTPS_SERVER_NAME   = "nghttp2.org";
    const char *HTTPS_SERVER_PATH   = "/";
    uint16_t HTTPS_SERVER_PORT      = 443;

    userSession->con.serverPort      = HTTPS_SERVER_PORT;
    userSession->con.serverScheme    = HTTPS_SERVER_SCHEME;
    userSession->con.serverName      = HTTPS_SERVER_NAME;
    userSession->con.serverPagePath  = HTTPS_SERVER_PATH;
    userSession->con.serverAuthority = HTTPS_SERVER_NAME;

    PRINTF("TLS init\n");
    ret = tls_init(&userSession->con);

    if (!ret)
    {
        PRINTF("NGHTTP2 init\n");
        ret = nghttp2_init(userSession);
    }
    if (!ret)
    {
        PRINTF("NGHTTP2 start\n");
        ret = nghttp2_start(userSession);
    }
    if (ret)
    {
        PRINTF("Error %d\n", ret);
    }

    tls_release(&userSession->con);
    nghttp2_release(userSession);
    freertos_free(userSession, NULL);

    PRINTF("NGHTTP2 finished.\n");
}
