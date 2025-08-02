/*
 * Copyright 2018-2021, 2023-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <network_mbedtls.h>

#ifdef CONFIG_MBEDTLS_DEBUG
#include "mbedtls/debug.h"
#endif

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_log.h>
#include <nxp_iot_agent_common.h>

#ifndef NETWORK_malloc
#define NETWORK_malloc malloc
#endif

#ifndef NETWORK_free
#define NETWORK_free free
#endif

static void warn_crt_crl_period(uint32_t verify_result)
{
	if ((verify_result & MBEDTLS_X509_BADCRL_EXPIRED) || (verify_result & MBEDTLS_X509_BADCRL_FUTURE) ||
		(verify_result & MBEDTLS_X509_BADCERT_EXPIRED) || (verify_result & MBEDTLS_X509_BADCERT_FUTURE))
	{
		IOT_AGENT_WARN("The certificate and/or CRL are outside of their validity period, which may be caused by the boards time being out of sync. \
						Please make sure that the time is correctly set.");
	}
}

static void network_lowlevel_disconnect(mbedtls_network_context_t* network_ctx)
{
	if (network_ctx != NULL)
	{
		mbedtls_ssl_free(&network_ctx->ssl);
		mbedtls_net_free(&network_ctx->server_fd);
	}
}

static const mbedtls_ecp_group_id supported_curves [] = {MBEDTLS_ECP_DP_SECP192R1,
		MBEDTLS_ECP_DP_SECP224R1,
		MBEDTLS_ECP_DP_SECP256R1,
		MBEDTLS_ECP_DP_SECP384R1,
		MBEDTLS_ECP_DP_SECP521R1,
		MBEDTLS_ECP_DP_NONE};

void* network_new(void)
{
	mbedtls_network_context_t* network_ctx = (mbedtls_network_context_t*)NETWORK_malloc(sizeof(mbedtls_network_context_t));
	if (network_ctx != NULL) {
		memset(network_ctx, 0, sizeof(*network_ctx));
	}
	return network_ctx;
}

void network_free(void* ctx)
{
	if (ctx != NULL)
	{
		mbedtls_network_context_t* network_ctx = (mbedtls_network_context_t*)ctx;

		network_lowlevel_disconnect(network_ctx);
		mbedtls_ssl_config_free(&(network_ctx->conf));
		mbedtls_ctr_drbg_free(&(network_ctx->ctr_drbg));
		mbedtls_pk_free(&(network_ctx->pkey));
		mbedtls_entropy_free(&network_ctx->entropy);

		NETWORK_free(ctx);
	}
}

#ifdef CONFIG_MBEDTLS_DEBUG
static void network_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	mbedtls_printf("MBEDTLS: %s", str);
}
#endif

int network_configure(void* opaque_ctx, void* opaque_network_config) {

	int ret = 0;
	const char *pers = "aws_iot_tls_wrapper";

	mbedtls_network_context_t* network_ctx = (mbedtls_network_context_t*)opaque_ctx;
	mbedtls_network_config_t* network_config = (mbedtls_network_config_t*)opaque_network_config;

	mbedtls_ssl_init(&(network_ctx->ssl));
	mbedtls_ssl_config_init(&(network_ctx->conf));
	mbedtls_ctr_drbg_init(&(network_ctx->ctr_drbg));
	mbedtls_pk_init(&(network_ctx->pkey));
	mbedtls_net_init(&network_ctx->server_fd);

#ifdef CONFIG_MBEDTLS_DEBUG
	mbedtls_ssl_conf_dbg(&(network_ctx->conf), network_debug, NULL);
	mbedtls_debug_set_threshold(4);
#endif

	mbedtls_entropy_init(&(network_ctx->entropy));
	if ((ret = mbedtls_ctr_drbg_seed(&(network_ctx->ctr_drbg),
		mbedtls_entropy_func, &(network_ctx->entropy),
		(const unsigned char *)pers, strlen(pers))) != 0) {
		return 1;
	}

	network_ctx->network_config = *network_config;
	return ret;
}

int network_connect(void* opaque_ctx)
{
	mbedtls_network_context_t* network_context = (mbedtls_network_context_t*)opaque_ctx;
	mbedtls_network_config_t* network_config = &network_context->network_config;
	int ret;
	unsigned char max_fragment_len = MBEDTLS_SSL_MAX_FRAG_LEN_NONE;
	uint32_t verify_result = 0U;

	char port_str[32] = { 0 };
	if (snprintf(port_str, sizeof(port_str), "%d", network_config->port) < 0)
	{
		IOT_AGENT_ERROR("Failed to build port string");
		goto exit;
	}

	if ((ret = mbedtls_net_connect(&network_context->server_fd,
		network_config->hostname, port_str,	MBEDTLS_NET_PROTO_TCP)) != 0)
	{
		IOT_AGENT_ERROR("Failed to connect TCP socket");
		goto exit;
	}

	mbedtls_net_set_block(&network_context->server_fd);

	if ((ret = mbedtls_ssl_config_defaults(&(network_context->conf),
		MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
		MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		goto disconnect;
	}

	{
#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)
		static int allowed_ciphersuites_sha_256[] = {
			MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
			MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256,
			MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
			MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256,
			MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
			MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256,
			MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
			MBEDTLS_TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256,
			0
		};
#endif
		static int allowed_ciphersuites_sha_384[] = {
			MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384,
			MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384,
			MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384,
			MBEDTLS_TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384,
			MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
			MBEDTLS_TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384,
			MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
			MBEDTLS_TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384,
			0
		};
		switch (mbedtls_pk_get_bitlen(&network_context->pkey))
		{
#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)
			case 256U:
				mbedtls_ssl_conf_ciphersuites(&network_context->conf, allowed_ciphersuites_sha_256);
				break;
#endif
			case 384U:
				mbedtls_ssl_conf_ciphersuites(&network_context->conf, allowed_ciphersuites_sha_384);
				break;
			default:
				break;
		}
	}

#if defined(NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE) && (NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE == 1)
	mbedtls_ssl_conf_authmode(&(network_context->conf), MBEDTLS_SSL_VERIFY_REQUIRED);
	mbedtls_ssl_conf_ca_chain(&(network_context->conf), &network_config->ca_chain, NULL);
#else
	mbedtls_ssl_conf_authmode(&(network_context->conf), MBEDTLS_SSL_VERIFY_NONE);
#endif

#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER > 0x03000000) &&  !defined(MBEDTLS_SSL_MAX_CONTENT_LEN)
#define MBEDTLS_SSL_MAX_CONTENT_LEN (MBEDTLS_SSL_IN_CONTENT_LEN < MBEDTLS_SSL_OUT_CONTENT_LEN ? MBEDTLS_SSL_IN_CONTENT_LEN : MBEDTLS_SSL_OUT_CONTENT_LEN)
#endif
#if MBEDTLS_SSL_MAX_CONTENT_LEN < 2048
#error Not enough memory for TLS records
#elif MBEDTLS_SSL_MAX_CONTENT_LEN > 16*1024
#error Too big buffer for TLS records. You are wasting memory.
#endif

	if (MBEDTLS_SSL_MAX_CONTENT_LEN < 4096) {
		max_fragment_len = MBEDTLS_SSL_MAX_FRAG_LEN_2048;
	}
	else if (MBEDTLS_SSL_MAX_CONTENT_LEN < 16*1024) {
		max_fragment_len = MBEDTLS_SSL_MAX_FRAG_LEN_4096;
	}

	if (max_fragment_len != MBEDTLS_SSL_MAX_FRAG_LEN_NONE) {
		if ((ret = mbedtls_ssl_conf_max_frag_len(&(network_context->conf), max_fragment_len)) != 0) {
			goto disconnect;
		}
	}

	mbedtls_ssl_conf_rng(&(network_context->conf), mbedtls_ctr_drbg_random,
		&(network_context->ctr_drbg));

	if ((ret = mbedtls_ssl_conf_own_cert(&(network_context->conf),
		&network_context->network_config.clicert, &(network_context->pkey))) != 0) {
		goto disconnect;
	}

	if ((ret = mbedtls_ssl_setup(&(network_context->ssl), &(network_context->conf)))
		!= 0) {
		goto disconnect;
	}

	if ((ret = mbedtls_ssl_set_hostname(&(network_context->ssl), network_context->network_config.hostname)) != 0) {
		goto disconnect;
	}

	mbedtls_ssl_set_bio(&network_context->ssl, &network_context->server_fd,
		mbedtls_net_send, mbedtls_net_recv, NULL );

	mbedtls_ssl_conf_curves(&(network_context->conf), supported_curves);

	while ((ret = mbedtls_ssl_handshake(&(network_context->ssl))) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ
			&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {

			if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED) {
				verify_result = mbedtls_ssl_get_verify_result(&(network_context->ssl));
				warn_crt_crl_period(verify_result);
			}

			goto disconnect;
		}
	}

	return ret;

disconnect:
	IOT_AGENT_ERROR("Failed to establish SSL connection (Error: 0x%04X, Verify: 0x%X)", (uint32_t)ret, verify_result);
#ifdef MBEDTLS_ERROR_C
	if (mbedtls_high_level_strerr(ret) != NULL) {
		IOT_AGENT_ERROR("High-level error: %s", mbedtls_high_level_strerr(ret));
	}
	if (mbedtls_low_level_strerr(ret) != NULL) {
		IOT_AGENT_ERROR("Low-level error: %s", mbedtls_low_level_strerr(ret));
	}
#endif
	network_lowlevel_disconnect(network_context);
exit:
	return -1;
}

int network_disconnect(void* opaque_ctx)
{
	int ret;

	mbedtls_network_context_t* network_ctx = (mbedtls_network_context_t*)opaque_ctx;
	/* No error checking, the connection might be closed already */
	do {
		ret = mbedtls_ssl_close_notify(&network_ctx->ssl);
	} while (ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	network_lowlevel_disconnect(network_ctx);

	return ret;
}


int network_read(void* context, uint8_t* buffer, size_t len)
{
	mbedtls_ssl_context *ssl = &((mbedtls_network_context_t*)context)->ssl;
	return mbedtls_ssl_read(ssl, buffer, len);
}

int network_write(void* context, const uint8_t* buffer, size_t len)
{
	mbedtls_ssl_context *ssl = &((mbedtls_network_context_t*)context)->ssl;
	return mbedtls_ssl_write(ssl, buffer, len);
}

int network_verify_server_certificate(void* context, uint8_t* trusted_bytes, size_t trusted_size,
	uint8_t* crl_bytes, size_t crl_size, uint32_t* error)
{
    int network_status = 0;
	mbedtls_network_context_t* network_context = (mbedtls_network_context_t*) context;
	mbedtls_ssl_context *ssl = &((mbedtls_network_context_t*)context)->ssl;

	mbedtls_x509_crl crl;
	mbedtls_x509_crl_init(&crl);

	*error = 0U;

	network_status = mbedtls_x509_crl_parse_der(&crl, (const unsigned char *)crl_bytes, crl_size);
	if (network_status != 0) {
		IOT_AGENT_ERROR("Failed to parse CRL");
	    goto exit;
	}

#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER < 0x03010000)
	network_status = mbedtls_x509_crt_verify(ssl->session->peer_cert, network_context->conf.ca_chain, &crl, NULL, error, NULL, NULL);
#else
	network_status = mbedtls_x509_crt_verify((mbedtls_x509_crt*)mbedtls_ssl_get_peer_cert(ssl), &network_context->network_config.ca_chain, &crl, NULL, error, NULL, NULL);
#endif //defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER < 0x03010000)

    if (*error != 0U) {
		warn_crt_crl_period(*error);
		IOT_AGENT_ERROR("Failed to verify server certificate with CRL (Verify: 0x%X)", *error);
        network_status = MBEDTLS_ERR_X509_CERT_VERIFY_FAILED;
        goto exit;
    }

exit:
    mbedtls_x509_crl_free(&crl);
    return network_status;
}

