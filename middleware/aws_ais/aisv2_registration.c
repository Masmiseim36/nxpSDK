/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>

#include "aisv2.h"

#include "iot_secure_sockets.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/base64.h"
#include "mbedtls/md_internal.h"
#include "mbedtls/hkdf.h"

#define ECDH_MPI_LENGTH 32
#define SHA_256_LENGTH 32
#define AIS_REG_URL_POST "/v1/ais/registration"
#define AIS_REG_GATEWAY "api.amazonalexa.com"

#define HTTP_REQUEST_HEADER              \
    "POST " AIS_REG_URL_POST             \
    " HTTP/1.1\r\n"                      \
    "Host: " AIS_REG_GATEWAY             \
    "\r\n"                               \
    "Content-Type: application/json\r\n" \
    "Content-Length: %d\r\n\r\n"         \
    "%s"

#ifdef __REDLIB__
__attribute__((weak)) size_t safe_strlen(const char *ptr, size_t max)
{
    /* REDLIB Has no strnlen call; this serves as a placeholder for developer to implement own function. */
    return strlen(ptr);
}
#else
#define safe_strlen strnlen
#endif

/* Forward function declarations for AIS-private JSON functions. */
cJSON *JSON_BuildRegistration(ais_handle_t *handle, const char *clientKey);

/* Reverse in-place an even-numbered MPI raw byte array (typically 32 bytes).
 * MPIs are read/written to byte arrays in big endian with mbedTLS.  They are
 * useful in little endian for ECDH with AIS. */
static void _reverse_mpi(uint8_t *array, uint32_t len)
{
    for (int i = 0; i < len / 2; i++)
    {
        uint8_t left  = array[i];
        uint8_t right = array[(len - 1) - i];

        array[i]             = right;
        array[(len - 1) - i] = left;
    }
}

static status_t AIS_HashSecret(uint8_t *in, uint32_t inLen, uint8_t *out)
{
    status_t ret                    = kStatus_Success;
    mbedtls_md_type_t mdType        = MBEDTLS_MD_SHA256;
    const mbedtls_md_info_t *mdInfo = mbedtls_md_info_from_type(mdType);

    if ((NULL == in) || (NULL == out))
    {
        ret = kStatus_InvalidArgument;
    }
    else
    {
        /* The hash has no seed or info. This will generate the hash */
        mbedtls_hkdf(mdInfo, NULL, 0, in, inLen, NULL, 0, out, mdInfo->size);
    }

    return ret;
}

/* Perform HTTP/1.1 registration request to retrieve server public key.
 * httpRequest is input string to send to the server
 * requestLen is the length of the request string, in bytes
 * serverKey is output, on success
 */
static status_t AIS_SendRegistration(char *httpRequest, uint32_t requestLen, uint8_t *serverKey, char *topicRoot)
{
    status_t ret                     = 0;
    Socket_t socket                  = {0};
    SocketsSockaddr_t pxAddress      = {0};
    const TickType_t xReceiveTimeOut = pdMS_TO_TICKS(10000);

    pxAddress.usPort         = SOCKETS_htons(443);
    pxAddress.ulAddress      = SOCKETS_GetHostByName(AIS_REG_GATEWAY);
    pxAddress.ucSocketDomain = SOCKETS_AF_INET;
    pxAddress.ucLength       = sizeof(SocketsSockaddr_t);

    socket = SOCKETS_Socket(SOCKETS_AF_INET, SOCKETS_SOCK_STREAM, SOCKETS_IPPROTO_TCP);

    if (socket == SOCKETS_INVALID_SOCKET)
    {
        configPRINTF(("[AIS] Cannot create socket\r\n"));
        return 1;
    }

    ret |= SOCKETS_SetSockOpt(socket, 0, SOCKETS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof(xReceiveTimeOut));
    ret |= SOCKETS_SetSockOpt(socket, 0, SOCKETS_SO_REQUIRE_TLS, NULL, 0);

    char *ppcAlpns[] = {"http/1.1"};
    ret |= SOCKETS_SetSockOpt(socket, 0, /* Level - Unused. */
                              SOCKETS_SO_ALPN_PROTOCOLS, ppcAlpns, sizeof(ppcAlpns) / sizeof(ppcAlpns[0]));

    ret |= SOCKETS_SetSockOpt(socket, 0, /* Level - Unused. */
                              SOCKETS_SO_SERVER_NAME_INDICATION, AIS_REG_GATEWAY, sizeof(AIS_REG_GATEWAY));

    if (ret == SOCKETS_ERROR_NONE)
    {
        configPRINTF(("[AIS] Registration connecting...\r\n"));

        ret = SOCKETS_Connect(socket, &pxAddress, sizeof(pxAddress));
        if (ret == SOCKETS_ERROR_NONE)
        {
            configPRINTF(("[AIS] Registration request...\r\n"));

            ret = SOCKETS_Send(socket, httpRequest, requestLen, 0);
            if (ret >= 0)
            {
                /* Reuse the request buffer for the response. */
                memset(httpRequest, 0, ret);
                ret = SOCKETS_Recv(socket, httpRequest, 2000, 0);

                if (ret >= 0 && strstr(httpRequest, "HTTP/1.1 2") != NULL)
                {
                    ret         = 0;
                    cJSON *json = cJSON_Parse(strstr(httpRequest, "{"));
                    if (json)
                    {
                        cJSON *enc  = cJSON_GetObjectItem(json, "encryption");
                        cJSON *pKey = cJSON_GetObjectItem(enc, "publicKey");

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
                        cJSON *iot        = cJSON_GetObjectItem(json, "iot");
                        cJSON *ptopicRoot = cJSON_GetObjectItem(iot, "topicRoot");
#endif

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
                        strcpy((char *)topicRoot, ptopicRoot->valuestring);
#endif
                        strcpy((char *)serverKey, pKey->valuestring);

                        cJSON_Delete(json);
                        goto finish;
                    }
                    else
                    {
                        configPRINTF(("[AIS] HTTP response - cannot parse JSON\r\n"));
                        configPRINTF(("[AIS] HTTP '%s'\r\n", httpRequest));
                    }
                }
                else
                {
                    configPRINTF(("[AIS] HTTP response - cannot find the header, ret %d\r\n", ret));
                    configPRINTF(("[AIS] HTTP '%s'\r\n", httpRequest));
                }
            }
            else
            {
                configPRINTF(("[AIS] HTTP request - error %d\r\n", ret));
            }
        }
        else
        {
            configPRINTF(("[AIS] Registration - cannot connect, error %d\r\n", ret));
        }
    }

    if (!ret)
    {
        ret = 1;
    }

finish:
    SOCKETS_Close(socket);

    return ret;
}

status_t AIS_Register(ais_handle_t *handle)
{
    status_t ret;
    mbedtls_ecdh_context ctx_cli;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    size_t lenClientKey, lenServerKey;
    cJSON *json;

    char *jsonStr;
    char *httpRequest;
    uint32_t httpRequestLen;
    char clientKey[ECDH_MPI_LENGTH * 2];
    uint8_t serverKey[ECDH_MPI_LENGTH * 2];
    uint8_t serverKeyBinary[ECDH_MPI_LENGTH];
    uint8_t mpiBuffer[ECDH_MPI_LENGTH];
    uint8_t ecdhSecret[ECDH_MPI_LENGTH];
    uint8_t hashBuffer[SHA_256_LENGTH];

    const char RegistrationRetryCount = 3;

    configPRINTF(("[AIS] Registering client %s\r\n", handle->config->awsAuthClientId));

    mbedtls_ecdh_init(&ctx_cli);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0)) != 0)
    {
        configPRINTF(("[AIS ERR] mbedtls_ctr_drbg_seed returned %d\n", ret));
    }
    if (!ret)
    {
        ret = mbedtls_ecp_group_load(&ctx_cli.grp, MBEDTLS_ECP_DP_CURVE25519);
    }
    if (!ret)
    {
        ret = mbedtls_ecdh_gen_public(&ctx_cli.grp, &ctx_cli.d, &ctx_cli.Q, mbedtls_ctr_drbg_random, &ctr_drbg);
    }
    if (!ret)
    {
        /* Write X-coord of ECP as public key to byte array. */
        ret = mbedtls_mpi_write_binary(&ctx_cli.Q.X, mpiBuffer, ECDH_MPI_LENGTH);
        /* Convert public key buffer from big to little endian. */
        _reverse_mpi(mpiBuffer, ECDH_MPI_LENGTH);
    }
    if (!ret)
    {
        ret = mbedtls_base64_encode((unsigned char *)clientKey, ECDH_MPI_LENGTH * 2, &lenClientKey, mpiBuffer,
                                    ECDH_MPI_LENGTH);
        clientKey[lenClientKey] = '\0';
    }
    if (!ret)
    {
        for (int i = 0; i < RegistrationRetryCount; i++)
        {
            configPRINTF(("[AIS] Registration Attempt %d\r\n", i + 1));

            /* Allocate storage for the HTTP request string. */
            httpRequest = (char *)pvPortMalloc(2 * 1024);

            json    = JSON_BuildRegistration(handle, clientKey);
            jsonStr = cJSON_PrintUnformatted(json);

            httpRequestLen = sprintf(httpRequest, HTTP_REQUEST_HEADER, strlen(jsonStr), jsonStr);

            cJSON_Delete(json);
            vPortFree(jsonStr);

            ret = AIS_SendRegistration(httpRequest, httpRequestLen, serverKey,
                                       handle->config->registrationConfig.awsPartnerRoot);

            vPortFree(httpRequest);

            if (ret)
            {
                configPRINTF(("[AIS ERR] Send Registration failed: %d\r\n", ret));
            }
            else
            {
                break;
            }
        }
    }
    if (ret)
    {
        configPRINTF(("[AIS ERR] Registration failed: %d\r\n", ret));
    }
    else
    {
        configPRINTF(("[AIS] Registration received public key: %s, Topic Root: %s\r\n", serverKey,
                      handle->config->registrationConfig.awsPartnerRoot));

        /* Check to see if we received a string */
        ret = safe_strlen(handle->config->registrationConfig.awsPartnerRoot,
                          sizeof(handle->config->registrationConfig.awsPartnerRoot));

        /* Client: read peer's key and generate shared secret */
        if (ret > 0)
        {
            /* Set '1' as ECP Z-coord value.  Curve25519 uses this + X-coord for key exchange. */
            ret = mbedtls_mpi_lset(&ctx_cli.Qp.Z, 1);
        }
        if (!ret)
        {
            ret = mbedtls_base64_decode(serverKeyBinary, ECDH_MPI_LENGTH, &lenServerKey, serverKey,
                                        strlen((char *)serverKey));
        }
        if (!ret)
        {
            /* Convert server public key buffer from little to big endian. */
            _reverse_mpi(serverKeyBinary, ECDH_MPI_LENGTH);
            /* Copy sever public key byte array to X-coord. */
            ret = mbedtls_mpi_read_binary(&ctx_cli.Qp.X, serverKeyBinary, lenServerKey);
        }

        if (!ret)
        {
            ret = mbedtls_ecdh_compute_shared(&ctx_cli.grp, &ctx_cli.z, &ctx_cli.Qp, &ctx_cli.d,
                                              mbedtls_ctr_drbg_random, &ctr_drbg);
        }

        if (!ret)
        {
            /* Write computed MPI as the shared secret. */
            ret = mbedtls_mpi_write_binary(&ctx_cli.z, ecdhSecret, ECDH_MPI_LENGTH);
            /* Convert computed shared key from big to little endian. */
            _reverse_mpi(ecdhSecret, ECDH_MPI_LENGTH);

#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 1))
            /* Hash the generated secret */
            AIS_HashSecret(ecdhSecret, ECDH_MPI_LENGTH, hashBuffer);

            memcpy(handle->config->registrationConfig.sharedSecret, hashBuffer, AIS_SECRET_LENGTH);
#else
            /* Save off only the lower 16-bytes of the 32-byte ECDH shared key
             * as the AIS secret. */
            memcpy(handle->config->registrationConfig.sharedSecret, ecdhSecret, AIS_SECRET_LENGTH);
#endif
        }

        if (!ret)
        {
            /* Store the generated shared secret in non-volatile memory. */
            AIS_AppRegistrationInfo(&handle->config->registrationConfig);

            configPRINTF(("[AIS] Registration successfully complete\r\n"));
        }

        mbedtls_ecdh_free(&ctx_cli);
        mbedtls_ctr_drbg_free(&ctr_drbg);
        mbedtls_entropy_free(&entropy);
    }

    return ret;
}
