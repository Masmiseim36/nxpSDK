/*
 * Copyright 2018-2021, 2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef _NXP_IOT_AGENT_PLATFORM_NETWORK_MBEDTLS_H_
#define _NXP_IOT_AGENT_PLATFORM_NETWORK_MBEDTLS_H_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif

#ifdef NXP_IOT_AGENT_HAVE_PSA_IMPL_SMW
#include <psa/crypto_types.h>
typedef psa_key_id_t mbedtls_svc_key_id_t;
#endif

#include <network.h>

#include <mbedtls/platform.h>
#include <mbedtls/ssl.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/error.h>
#include <mbedtls/pk.h>
#include <mbedtls/net_sockets.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct mbedtls_network_config_t
{
    const char* hostname;
	int port;
	mbedtls_x509_crt clicert;
	mbedtls_x509_crt ca_chain;
} mbedtls_network_config_t;


typedef struct mbedtls_network_context_t
{
    mbedtls_network_config_t network_config;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_pk_context	pkey;
	mbedtls_net_context server_fd;
	mbedtls_entropy_context entropy;
} mbedtls_network_context_t;


#ifdef __cplusplus
}
#endif

#endif // _NXP_IOT_AGENT_PLATFORM_NETWORK_MBEDTLS_H_

