/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "httpsclient.h"
#include <stdio.h>
#include <stdlib.h>
#include "fsl_debug_console.h"
#include "sln_flash_mgmt.h"
#include "sln_flash.h"
#include "device_utils.h"

#include "lwip/debug.h"
#include "lwip/netdb.h"
#include "lwip/opt.h"
#include "lwip/sockets.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"

#include "aws_clientcredential.h"
#include "cJSON.h"
#include "udp_message_handler.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This is the value used for ssl read timeout */
#define IOT_SSL_READ_TIMEOUT 100
#define DEBUG_LEVEL 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
TLSDataParams *tlsDataParams  = NULL;
const char *HTTPS_SERVER_NAME = "api.amazon.com";
const char *HTTPS_SERVER_PORT = "443";

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Send function used by mbedtls ssl */
static int lwipSend(void *fd, unsigned char const *buf, size_t len)
{
    return lwip_send((*(int *)fd), buf, len, 0);
}

/* Send function used by mbedtls ssl */
static int lwipRecv(void *fd, unsigned char const *buf, size_t len)
{
    return lwip_recv((*(int *)fd), (void *)buf, len, 0);
}

int write_request(char *https_buf, int request_len)
{
    int ret = 0;

    assert(https_buf);

    /* send the request */
    while ((ret = mbedtls_ssl_write(&(tlsDataParams->ssl), (unsigned char *)https_buf, request_len)) <= 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            configPRINTF(("mbedtls_ssl_write returned %d\r\n", ret));
            goto exit;
        }
    }

    configPRINTF(("%d bytes written\r\n", ret));

    return ret;

exit:
    return -1;
}

int read_request(char *https_buf, int https_buf_len)
{
    int ret = 0;

    assert(https_buf);

    do
    {
        ret = mbedtls_ssl_read(&(tlsDataParams->ssl), (unsigned char *)https_buf, https_buf_len);

        if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
            continue;

        if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
            break;

        if (ret < 0)
        {
            configPRINTF(("mbedtls_ssl_read returned %d\r\n", ret));
            goto exit;
        }

        if (ret >= 0 && ret <= https_buf_len)
        {
            configPRINTF(("%d bytes read\r\n", ret));
            break;
        }
    } while (1);

    return ret;

exit:
    return -1;
}

static int _iot_tls_verify_cert(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
    char buf[1024];
    ((void)data);

    mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);

    if ((*flags) == 0)
    {
        configPRINTF(("This certificate has no flags\r\n"));
    }
    else
    {
        configPRINTF((buf, sizeof(buf), "  ! ", *flags));
        configPRINTF(("%s\r\n", buf));
    }

    return 0;
}

#ifdef MBEDTLS_DEBUG_C
static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void)level);

    configPRINTF(("\r\r\n%s, at line %d in file %s\r\n", str, line, file));
}
#endif

int https_client_tls_init()
{
    int ret          = 0;
    const char *pers = "dummy";
    char vrfy_buf[512];
    bool ServerVerificationFlag = false;
    const mbedtls_md_info_t *md_info;

    uint8_t *pucCertificateData      = NULL;
    uint32_t ulCertificateDataLength = 0;
    uint8_t *pucKeyData              = NULL;
    uint32_t ulKeyDataLength         = 0;

#ifdef MBEDTLS_DEBUG_C
    unsigned char buf[MBEDTLS_SSL_MAX_CONTENT_LEN + 1];
#endif

    tlsDataParams = pvPortMalloc(sizeof(TLSDataParams));
    assert(tlsDataParams);

    mbedtls_ssl_init(&(tlsDataParams->ssl));
    mbedtls_ssl_config_init(&(tlsDataParams->conf));
    mbedtls_hmac_drbg_init(&(tlsDataParams->hmac_drbg));
    mbedtls_x509_crt_init(&(tlsDataParams->cacert));
    mbedtls_x509_crt_init(&(tlsDataParams->clicert));
    mbedtls_pk_init(&(tlsDataParams->pkey));

#if defined(MBEDTLS_DEBUG_C)
    /* Enable debug output of mbedtls */
    mbedtls_ssl_conf_dbg(&(tlsDataParams->conf), my_debug, NULL);
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

    configPRINTF(("Seeding the random number generator... \r\n"));
    mbedtls_entropy_init(&(tlsDataParams->entropy));
    md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if ((ret = mbedtls_hmac_drbg_seed(&(tlsDataParams->hmac_drbg), md_info, mbedtls_entropy_func,
                                      &(tlsDataParams->entropy), (const unsigned char *)pers, strlen(pers))) != 0)
    {
        configPRINTF(("mbedtls_hmac_drbg_seed returned -0x%x\r\n", -ret));
        return NETWORK_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED;
    }

    /* Read the certificate from storage. */
    pucCertificateData = (uint8_t *)pvPortMalloc(clientcredentialCLIENT_CERTIFICATE_LENGTH);

    if (NULL == pucCertificateData)
    {
        ret = -1;
        goto exit;
    }

    /* The certificate is stored in plain text, so read it without decryption */
    uint32_t certLen = 0;
    SLN_FLASH_MGMT_Read(clientcredentialCLIENT_CERTIFICATE_PEM, pucCertificateData, &certLen);
    ulCertificateDataLength = safe_strlen((char *)pucCertificateData, clientcredentialCLIENT_CERTIFICATE_LENGTH) + 1;

    /* Read the private key from storage. */
    pucKeyData = (uint8_t *)clientcredentialCLIENT_PRIVATE_KEY_PEM;
    ulKeyDataLength =
        safe_strlen(clientcredentialCLIENT_PRIVATE_KEY_PEM, clientcredentialCLIENT_PRIVATE_KEY_LENGTH) + 1;

    pucKeyData = (uint8_t *)pvPortMalloc(clientcredentialCLIENT_PRIVATE_KEY_LENGTH);

    if (NULL == pucKeyData)
    {
        ret = -1;
        goto exit;
    }

    char *keyPhrase = NULL;
    PKCS11_KeyGen(&keyPhrase);

    /* The certificate is stored in plain text, so read it without decryption */
    uint32_t keyLen = 0;
    SLN_FLASH_MGMT_Read(clientcredentialCLIENT_PRIVATE_KEY_PEM, pucKeyData, &keyLen);
    ulKeyDataLength = safe_strlen((char *)pucKeyData, clientcredentialCLIENT_PRIVATE_KEY_LENGTH) + 1;

    configPRINTF(("Loading the client cert. and key... \r\n"));
    ret = mbedtls_x509_crt_parse(&(tlsDataParams->clicert), (const unsigned char *)pucCertificateData,
                                 ulCertificateDataLength);
    if (ret != 0)
    {
        configPRINTF(("mbedtls_x509_crt_parse returned -0x%x while parsing device cert\r\n", -ret));
        return NETWORK_X509_DEVICE_CRT_PARSE_ERROR;
    }

    ret = mbedtls_pk_parse_key(&(tlsDataParams->pkey), (const unsigned char *)pucKeyData, ulKeyDataLength,
                               (uint8_t *)keyPhrase, strlen(keyPhrase));
    if (ret != 0)
    {
        configPRINTF(("mbedtls_pk_parse_key returned -0x%x while parsing private key\r\n", -ret));
        return NETWORK_PK_PRIVATE_KEY_PARSE_ERROR;
    }

    vPortFree(keyPhrase);
    keyPhrase = NULL;

    configPRINTF(("Connecting to %s/%s\r\n", HTTPS_SERVER_NAME, HTTPS_SERVER_PORT));

    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    ret = getaddrinfo(HTTPS_SERVER_NAME, HTTPS_SERVER_PORT, &hints, &res);
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

    configPRINTF(("Setting up the SSL/TLS structure... \r\n"));
    if ((ret = mbedtls_ssl_config_defaults(&(tlsDataParams->conf), MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        configPRINTF(("mbedtls_ssl_config_defaults returned -0x%x\r\n", -ret));
        return SSL_CONNECTION_ERROR;
    }

    mbedtls_ssl_conf_verify(&(tlsDataParams->conf), _iot_tls_verify_cert, NULL);
    if (ServerVerificationFlag == true)
    {
        mbedtls_ssl_conf_authmode(&(tlsDataParams->conf), MBEDTLS_SSL_VERIFY_REQUIRED);
    }
    else
    {
        mbedtls_ssl_conf_authmode(&(tlsDataParams->conf), MBEDTLS_SSL_VERIFY_OPTIONAL);
    }
    mbedtls_ssl_conf_rng(&(tlsDataParams->conf), mbedtls_hmac_drbg_random, &(tlsDataParams->hmac_drbg));

    mbedtls_ssl_conf_ca_chain(&(tlsDataParams->conf), &(tlsDataParams->cacert), NULL);
    if ((ret = mbedtls_ssl_conf_own_cert(&(tlsDataParams->conf), &(tlsDataParams->clicert), &(tlsDataParams->pkey))) !=
        0)
    {
        configPRINTF(("mbedtls_ssl_conf_own_cert returned %d\r\n", ret));
        return SSL_CONNECTION_ERROR;
    }

    if ((ret = mbedtls_ssl_setup(&(tlsDataParams->ssl), &(tlsDataParams->conf))) != 0)
    {
        configPRINTF(("mbedtls_ssl_setup returned -0x%x\r\n", -ret));
        return SSL_CONNECTION_ERROR;
    }
    if ((ret = mbedtls_ssl_set_hostname(&(tlsDataParams->ssl), HTTPS_SERVER_NAME)) != 0)
    {
        configPRINTF(("mbedtls_ssl_set_hostname returned %d\r\n", ret));
        return SSL_CONNECTION_ERROR;
    }
    configPRINTF(("SSL state connect : %d\r\n", tlsDataParams->ssl.state));

    mbedtls_ssl_set_bio(&(tlsDataParams->ssl), &(tlsDataParams->fd), lwipSend, (mbedtls_ssl_recv_t *)lwipRecv, NULL);

    configPRINTF(("Performing the SSL/TLS handshake...\r\n"));
    while ((ret = mbedtls_ssl_handshake(&(tlsDataParams->ssl))) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            configPRINTF(("mbedtls_ssl_handshake returned -0x%x\r\n", -ret));
            if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED)
            {
                configPRINTF(
                    ("    Unable to verify the server's certificate. "
                     "    Alternatively, you may want to use "
                     "auth_mode=optional for testing purposes.\r\n"));
            }
            return SSL_CONNECTION_ERROR;
        }
    }

    configPRINTF(("ok\r\n    [ Protocol is %s ]\r\n    [ Ciphersuite is %s ]\r\n",
                  mbedtls_ssl_get_version(&(tlsDataParams->ssl)), mbedtls_ssl_get_ciphersuite(&(tlsDataParams->ssl))));
    if ((ret = mbedtls_ssl_get_record_expansion(&(tlsDataParams->ssl))) >= 0)
    {
        configPRINTF(("Record expansion is %d\r\n", ret));
    }
    else
    {
        configPRINTF(("[ Record expansion is unknown (compression) ]\r\n"));
    }

    configPRINTF(("Verifying peer X.509 certificate... \r\n"));

    if (ServerVerificationFlag == true)
    {
        if ((tlsDataParams->flags = mbedtls_ssl_get_verify_result(&(tlsDataParams->ssl))) != 0)
        {
            configPRINTF((" failed\r\n"));
            mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", tlsDataParams->flags);
            configPRINTF(("%s\r\n", vrfy_buf));
            ret = SSL_CONNECTION_ERROR;
        }
        else
        {
            configPRINTF(("ok\r\n"));
            ret = SUCCESS;
        }
    }
    else
    {
        configPRINTF(("Server Verification skipped\r\n"));
        ret = SUCCESS;
    }

#ifdef MBEDTLS_DEBUG_C
    if (mbedtls_ssl_get_peer_cert(&(tlsDataParams->ssl)) != NULL)
    {
        configPRINTF(("  . Peer certificate information    ...\r\n"));
        mbedtls_x509_crt_info((char *)buf, sizeof(buf) - 1, "      ", mbedtls_ssl_get_peer_cert(&(tlsDataParams->ssl)));
        configPRINTF(("%s\r\n", buf));
    }
#endif

    mbedtls_ssl_conf_read_timeout(&(tlsDataParams->conf), IOT_SSL_READ_TIMEOUT);

exit:
    memset(pucKeyData, 0x00, clientcredentialCLIENT_PRIVATE_KEY_LENGTH);
    memset(pucCertificateData, 0x00, clientcredentialCLIENT_CERTIFICATE_LENGTH);
    vPortFree(pucKeyData);
    vPortFree(pucCertificateData);
    pucKeyData         = NULL;
    pucCertificateData = NULL;

    return (Error_t)ret;
}

/* Release TLS */
void https_client_tls_release()
{
    close(tlsDataParams->fd);
    mbedtls_x509_crt_free(&(tlsDataParams->clicert));
    mbedtls_x509_crt_free(&(tlsDataParams->cacert));
    mbedtls_pk_free(&(tlsDataParams->pkey));
    mbedtls_ssl_free(&(tlsDataParams->ssl));
    mbedtls_ssl_config_free(&(tlsDataParams->conf));
    mbedtls_hmac_drbg_free(&(tlsDataParams->hmac_drbg));
    mbedtls_entropy_free(&(tlsDataParams->entropy));

    vPortFree(tlsDataParams);
}
