/** @file mbedtls_common.h
 *
 *  @brief This file provides NCP common mbedtls interfaces.
 *
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MBEDTLS_COMMON_H__
#define __MBEDTLS_COMMON_H__
#include "stdint.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl_ciphersuites.h"
#include "psa/crypto.h"

#define NCP_MBEDTLS_RECV_BUF_LEN                (2048) /* it's enough for ECC secp192k1 */
#define NCP_TLS_ROLE_SERVER                     (1)
#define NCP_TLS_ROLE_CLIENT                     (0)

#define TLS_OK                                  0
#define TLS_ERR_PSA_INIT                        1
#define TLS_ERR_PARSE_CERT                      2
#define TLS_ERR_PARSE_KEY                       3
#define TLS_ERR_SSL_INIT                        4
#define TLS_ERR_CFG_CERT                        5
#define TLS_ERR_SSL_SETUP                       6
#define TLS_ERR_CRETAE_SEM                      7
#define TLS_ERR_ALLOC_MEM                       8
#define TLS_ERR_INIT_TLS                        9
#define TLS_ERR_HANDSHAKING_NOT_START           10
#define TLS_ERR_INVALID_PARAM                   11
#define TLS_ERR_RINGBUF_FULL                    12
#define TLS_ERR_HANDSHAKE                       13

#ifdef RW610
#if !defined(MBEDTLS_PLATFORM_C)
#define MBEDTLS_PLATFORM_C
#endif

#if !defined(CONFIG_HAVE_TIME_DISABLE)
#define MBEDTLS_HAVE_TIME
#endif

#if !defined(CONFIG_HAVE_TIME_DISABLE)
#define MBEDTLS_PLATFORM_TIME_ALT
#endif

#if (!defined(MBEDTLS_PLATFORM_GMTIME_R_ALT) && !defined(CONFIG_HAVE_TIME_DISABLE))
#define MBEDTLS_PLATFORM_GMTIME_R_ALT
#define MBEDTLS_PLATFORM_GMTIME_FUNC_USE
#endif
#endif

#pragma pack(1)
typedef struct {
    uint8_t  dat[NCP_MBEDTLS_RECV_BUF_LEN];
    uint16_t in;
    uint16_t out;
} ringbuf_t;

typedef /*NCP_TLV_PACK_START*/ struct {
    mbedtls_ssl_context      ssl;
    mbedtls_entropy_context  entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_config       conf;
    mbedtls_x509_crt         ca_cert;
    mbedtls_x509_crt         own_cert;
    mbedtls_pk_context       pkey;
    uint8_t                  is_server;
    uint8_t                  entropy_buf[MBEDTLS_ENTROPY_BLOCK_SIZE];
    ringbuf_t                ringbuf;
} /*NCP_TLV_PACK_END*/ mbedtls_ctx_t;

#pragma pack()


extern mbedtls_ctx_t *_mbedtls;
extern uint32_t _verify_num;


psa_status_t psa_crypto_init(void);
int port_mbedtls_send(void *ctx, const unsigned char *buf, size_t len);
int ncp_encrypt_process_handshake_data(uint8_t *data, uint16_t len);
int ncp_encrypt_setup(uint8_t is_server);
int ncp_encrypt_teardown(void);
int ncp_encrypt_process_handshake_data(uint8_t *data, uint16_t len);
int ncp_cmd_is_data_cmd(uint32_t cmd);


#endif /* __MBEDTLS_COMMON_H__ */
