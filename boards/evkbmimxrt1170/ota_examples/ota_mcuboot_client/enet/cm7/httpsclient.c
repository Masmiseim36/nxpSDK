/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP. Not a Contribution
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

#include "ota_http.h"
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/sockets.h"
#include "httpsclient.h"
#include "lwip/netdb.h"
#include "fsl_debug_console.h"
#include "mbedtls/sha256.h"
#include "mflash_drv.h"
#include "flash_partitioning.h"
#include "network_cfg.h"
#include "flash_helper.h"
#include "ota_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEBUG_LEVEL 0

/* Size of maximum HTTP data payload transfer.
 * Since flash erase is done on the fly with downloading
 * data, it makes things easier when this is multiple of
 * flash page size and less or equal to flash erase sector.
 */

#define OTA_HTTP_BLOCK_SIZE 4096

#if OTA_HTTP_BLOCK_SIZE > MFLASH_SECTOR_SIZE
#error "HTTP block size can not be greater than flash erase sector size"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static TLSDataParams tlsDataParams;

/* Buffer size is given by defined block size and some space
 * for HTTP headers
 */
static unsigned char https_buf[OTA_HTTP_BLOCK_SIZE + 512];

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Send function used by mbedtls ssl */
static int lwipSend(void *fd, unsigned char const *buf, size_t len)
{
    return lwip_send((*(int *)fd), buf, len, 0);
}

/* Send function used by mbedtls ssl */
static int lwipRecv(void *fd, unsigned char *buf, size_t len)
{
    return lwip_recv((*(int *)fd), (void *)buf, len, 0);
}

/*
static int _iot_tls_verify_cert(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
    char buf[1024];
    ((void)data);

    PRINTF("\nCert veryfication requested for (Depth %d):\n", depth);
    mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);
    PRINTF("%s", buf);

    return 0;
}
*/

#ifdef MBEDTLS_DEBUG_C
static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void)level);

    PRINTF("\r\n%s, at line %d in file %s\n", str, line, file);
}
#endif

static void print_hash(const void *src, size_t size)
{
    const unsigned char *src8 = src;
    for (size_t i = 0; i < size; i++)
    {
        PRINTF("%02X", src8[i]);
    }
}

int https_client_tls_init(const char *host, const char *service)
{
    int ret          = 0;
    const char *pers = "https_ota_demo";
    char vrfy_buf[512];
    bool ServerVerificationFlag = false;

#ifdef PRINT_CERT_INFO
    /* requires high stack usage! */
    unsigned char buf[MBEDTLS_SSL_MAX_CONTENT_LEN + 1];
#endif

    if (!host || !service)
    {
        return NULL_VALUE_ERROR;
    }

    mbedtls_ssl_init(&(tlsDataParams.ssl));
    mbedtls_ssl_config_init(&(tlsDataParams.conf));
    mbedtls_ctr_drbg_init(&(tlsDataParams.ctr_drbg));
    mbedtls_x509_crt_init(&(tlsDataParams.cacert));
    mbedtls_x509_crt_init(&(tlsDataParams.clicert));
    mbedtls_pk_init(&(tlsDataParams.pkey));

#if defined(MBEDTLS_DEBUG_C)
    /* Enable debug output of mbedtls */
    mbedtls_ssl_conf_dbg(&(tlsDataParams.conf), my_debug, NULL);
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

    PRINTF("\n  . Seeding the random number generator...");
    mbedtls_entropy_init(&(tlsDataParams.entropy));
    if ((ret = mbedtls_ctr_drbg_seed(&(tlsDataParams.ctr_drbg), mbedtls_entropy_func,
                                      &(tlsDataParams.entropy), (const unsigned char *)pers, strlen(pers))) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_ctr_drbg_seed returned -0x%x\n", -ret);
        return NETWORK_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED;
    }

    PRINTF("\n  . Loading the CA root certificate...");
    ret = mbedtls_x509_crt_parse(&(tlsDataParams.cacert), (const unsigned char *)mbedtls_test_ca_crt,
                                 mbedtls_test_ca_crt_len);
    if (ret < 0)
    {
        PRINTF(" failed\n  !  mbedtls_x509_crt_parse returned -0x%x while parsing root cert\n\n", -ret);
        return NETWORK_X509_ROOT_CRT_PARSE_ERROR;
    }
    PRINTF(" ok (%d skipped)\n", ret);

    PRINTF("  . Loading the client cert. and key...");
    ret = mbedtls_x509_crt_parse(&(tlsDataParams.clicert), (const unsigned char *)mbedtls_test_cli_crt,
                                 mbedtls_test_cli_crt_len);
    if (ret != 0)
    {
        PRINTF(" failed\n  !  mbedtls_x509_crt_parse returned -0x%x while parsing device cert\n\n", -ret);
        return NETWORK_X509_DEVICE_CRT_PARSE_ERROR;
    }
    PRINTF(" ok\n");

    ret = mbedtls_pk_parse_key(&(tlsDataParams.pkey), (const unsigned char *)mbedtls_test_cli_key,
                               mbedtls_test_cli_key_len, NULL, 0);
    if (ret != 0)
    {
        PRINTF(" failed\n  !  mbedtls_pk_parse_key returned -0x%x while parsing private key\n\n", -ret);
        return NETWORK_PK_PRIVATE_KEY_PARSE_ERROR;
    }

    PRINTF("  . Connecting to %s:%s\n", host, service);

    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    ret = getaddrinfo(host, service, &hints, &res);
    if ((ret != 0) || (res == NULL))
    {
        return NETWORK_ERR_NET_UNKNOWN_HOST;
    }

    tlsDataParams.fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (tlsDataParams.fd < 0)
    {
        return NETWORK_ERR_NET_SOCKET_FAILED;
    }

    ret = connect(tlsDataParams.fd, res->ai_addr, res->ai_addrlen);

    freeaddrinfo(res);

    if (ret != 0)
    {
        close(tlsDataParams.fd);
        return NETWORK_ERR_NET_CONNECT_FAILED;
    }

    PRINTF("  . Setting up the SSL/TLS structure...");
    if ((ret = mbedtls_ssl_config_defaults(&(tlsDataParams.conf), MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_ssl_config_defaults returned -0x%x\n\n", -ret);
        return SSL_CONNECTION_ERROR;
    }
    PRINTF(" ok\n");

    /* mbedtls_ssl_conf_verify(&(tlsDataParams.conf), _iot_tls_verify_cert, NULL); */

    if (ServerVerificationFlag == true)
    {
        mbedtls_ssl_conf_authmode(&(tlsDataParams.conf), MBEDTLS_SSL_VERIFY_REQUIRED);
    }
    else
    {
        mbedtls_ssl_conf_authmode(&(tlsDataParams.conf), MBEDTLS_SSL_VERIFY_OPTIONAL);
    }
    mbedtls_ssl_conf_rng(&(tlsDataParams.conf), mbedtls_ctr_drbg_random, &(tlsDataParams.ctr_drbg));

    mbedtls_ssl_conf_ca_chain(&(tlsDataParams.conf), &(tlsDataParams.cacert), NULL);
    if ((ret = mbedtls_ssl_conf_own_cert(&(tlsDataParams.conf), &(tlsDataParams.clicert), &(tlsDataParams.pkey))) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
        return SSL_CONNECTION_ERROR;
    }

    if ((ret = mbedtls_ssl_setup(&(tlsDataParams.ssl), &(tlsDataParams.conf))) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_ssl_setup returned -0x%x\n\n", -ret);
        return SSL_CONNECTION_ERROR;
    }
    if ((ret = mbedtls_ssl_set_hostname(&(tlsDataParams.ssl), host)) != 0)
    {
        PRINTF(" failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", ret);
        return SSL_CONNECTION_ERROR;
    }

    mbedtls_ssl_set_bio(&(tlsDataParams.ssl), &(tlsDataParams.fd), lwipSend, (mbedtls_ssl_recv_t *)lwipRecv, NULL);

    PRINTF("  . SSL state connect : %d \n", tlsDataParams.ssl.state);
    PRINTF("  . Performing the SSL/TLS handshake...\n");
    while ((ret = mbedtls_ssl_handshake(&(tlsDataParams.ssl))) != 0)
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

    PRINTF("    [ Protocol is %s ]\n    [ Ciphersuite is %s ]\n", mbedtls_ssl_get_version(&(tlsDataParams.ssl)),
           mbedtls_ssl_get_ciphersuite(&(tlsDataParams.ssl)));
    if ((ret = mbedtls_ssl_get_record_expansion(&(tlsDataParams.ssl))) >= 0)
    {
        PRINTF("    [ Record expansion is %d ]\n", ret);
    }
    else
    {
        PRINTF("    [ Record expansion is unknown (compression) ]\n");
    }

    PRINTF("  . Verifying peer X.509 certificate...");

    if (ServerVerificationFlag == true)
    {
        if ((tlsDataParams.flags = mbedtls_ssl_get_verify_result(&(tlsDataParams.ssl))) != 0)
        {
            PRINTF(" FAILED\n");
            mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", tlsDataParams.flags);
            PRINTF("%s\n", vrfy_buf);
            ret = SSL_CONNECTION_ERROR;
        }
        else
        {
            PRINTF(" OK\n");
            ret = SUCCESS;
        }
    }
    else
    {
        PRINTF(" SKIPPED\n");
        ret = SUCCESS;
    }

#ifdef PRINT_CERT_INFO
    if (mbedtls_ssl_get_peer_cert(&(tlsDataParams.ssl)) != NULL)
    {
        PRINTF("  . Peer certificate information    ...\n");
        mbedtls_x509_crt_info((char *)buf, sizeof(buf) - 1, "      ", mbedtls_ssl_get_peer_cert(&(tlsDataParams.ssl)));
        PRINTF("%s\n", buf);
    }
#endif

    return (Error_t)ret;
}

/* Release TLS */
void https_client_tls_release(void)
{
    lwip_close(tlsDataParams.fd);
    mbedtls_x509_crt_free(&(tlsDataParams.clicert));
    mbedtls_x509_crt_free(&(tlsDataParams.cacert));
    mbedtls_pk_free(&(tlsDataParams.pkey));
    mbedtls_ssl_free(&(tlsDataParams.ssl));
    mbedtls_ssl_config_free(&(tlsDataParams.conf));
    mbedtls_ctr_drbg_free(&(tlsDataParams.ctr_drbg));
    mbedtls_entropy_free(&(tlsDataParams.entropy));
}

/*
 * host - used in HTTP header in Host field
 * fPath - path to requested file
 * dstAddrPhy - physical flash address where file will be downloaded
 * dstSize - available size for file download
 * fSize - actual file size downloaded
 */
int https_client_ota_download(const char *host, const char *fPath, uint32_t dstAddrPhy, size_t dstSize, size_t *fSize)
{
    NetworkContext_t coreHttp_NetCtx = {&tlsDataParams.ssl};

    TransportInterface_t ti = {.recv = coreHttp_recv, .send = coreHttp_send, .pNetworkContext = &coreHttp_NetCtx};

    struct OtaHttpConf httpConf = {.ti = &ti, .dataBuf = https_buf, .dataBufSize = sizeof(https_buf), .hostName = host};

    int ret;
    unsigned char md_net[32], md_flash[32];
    mbedtls_sha256_context md_ctx;

    mbedtls_sha256_init(&md_ctx);
    mbedtls_sha256_starts_ret(&md_ctx, 0);

    uint32_t addr_phy   = dstAddrPhy;
    uint32_t file_size  = 0;
    size_t bytes_recvd  = 0;
    size_t storage_size = dstSize;

    /* Determine File Size */

    PRINTF("\nGetting size of requested file '%s'\n", fPath);

    ret = OtaHttp_GetFileSize(fPath, &file_size, &httpConf);
    if (ret != HTTPSuccess)
    {
        PRINTF("Failed to get file size, ret=%d\n", ret);
        return EXIT_FAILURE;
    }

    PRINTF("Determined file size is %u bytes\n", file_size);

    if (file_size > storage_size)
    {
        PRINTF("Requested file TOO BIG! file_size=%d storage_size=%d\n", file_size, storage_size);
        return EXIT_FAILURE;
    }

    /* Download file in chunks of size OTA_HTTP_BLOCK_SIZE */

    PRINTF("Starting download of %u bytes with block size of %u bytes\n", file_size, OTA_HTTP_BLOCK_SIZE);

    while (bytes_recvd < file_size)
    {
        int cnt;
        status_t status;
        size_t flash_offset = addr_phy + bytes_recvd;
        size_t remains      = file_size - bytes_recvd;
        size_t chunk        = (remains < OTA_HTTP_BLOCK_SIZE) ? remains : OTA_HTTP_BLOCK_SIZE;

        cnt = OtaHttp_GetFileContent(fPath, bytes_recvd, chunk, https_buf, &httpConf);

        // PRINTF("Fetch of %d bytes, got %d\n", chunk, cnt);
        PUTCHAR('.');

        if (cnt < 0)
        {
            PRINTF("HTTP File Request at offset %u failed with %d\n", bytes_recvd, cnt);
            break;
        }
        else if (cnt != chunk)
        {
            /* On partial content request the server can return less data than
             * requested if the file is smaller, however in this case the file
             * size was determined beforehand so (cnt == chunk) should always hold.
             */
            PRINTF("UNEXPECTED size read at offset %u: %u read, %u expected.\n", bytes_recvd, cnt, chunk);
            break;
        }

        mbedtls_sha256_update(&md_ctx, https_buf, cnt);

        /* Flash erase is done on the fly with download since erasing large portion
         * of flash while executing from it would cause other system tasks to starve
         * (e.g. TCP connection)
         */
        if (0 == (flash_offset % MFLASH_SECTOR_SIZE))
        {
            // PUTCHAR('E');
            status = mflash_drv_sector_erase(flash_offset);
            if (status != kStatus_Success)
            {
                PRINTF("FAILED to erase sector at offset %u\n", addr_phy);
                break;
            }
        }

        // PUTCHAR('P');
        flash_program(flash_offset, https_buf, cnt);

        bytes_recvd += cnt;
    }

    PRINTF("\nDownload loop completed with size %u, expected %u\n\n", bytes_recvd, file_size);

    if (bytes_recvd != file_size)
    {
        PRINTF("FAILED to download requested file.\n");
        return EXIT_FAILURE;
    }

    /* Message Digest check */

    PRINTF("SHA256 of downloaded data: ");
    mbedtls_sha256_finish(&md_ctx, md_net);
    print_hash(md_net, 10);
    PRINTF("...\n");

    PRINTF("SHA256 of flashed data:    ");
    flash_sha256(addr_phy, file_size, md_flash);
    print_hash(md_flash, 10);
    PRINTF("...\n");

    if (memcmp(md_net, md_flash, sizeof(md_flash)) != 0)
    {
        PRINTF("FAILED to compare MD's.\n");
        return EXIT_FAILURE;
    }

    if (fSize)
    {
        *fSize = file_size;
    }

    return SUCCESS;
}
