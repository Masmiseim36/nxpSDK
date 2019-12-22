/**
 * @file ax_mbedtls.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Implementation of key association between A71CH and mbedtls.
 * @par History
 * 1.0   30-jan-2018 : Initial version
 *
 *****************************************************************************/

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECP_ALT)

/** @ingroup ax_mbed_tls */
/** @{ */

#include "mbedtls/version.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_internal.h"


#include "ax_mbedtls.h"
#include "mbedtls/pk_internal.h"
#include "mbedtls/platform.h"
#include "sm_printf.h"
#include "sm_const.h"
#include "HLSEObjects.h"
#include "HLSECrypto.h"
#include "HLSEAPI.h"
#include "sm_apdu.h"

#if defined(FLOW_VERBOSE) && (FLOW_VERBOSE == 1)
#   define LOG_API_CALLS 1
#else
#   define LOG_API_CALLS 1
#endif /* FLOW_VERBOSE */


#ifndef LOG_API_CALLS
#   define LOG_API_CALLS 1 /* Log by default */
#endif

static size_t ax_eckey_get_bitlen(
    const void *ctx);

static int ax_eckey_sign(
    void *ctx, mbedtls_md_type_t md_alg,
    const unsigned char *hash, size_t hash_len,
    unsigned char *sig, size_t *sig_len,
    int (*f_rng)(
        void *, unsigned char *, size_t), void *p_rng);
static int ax_eckey_verify(
    void *ctx, mbedtls_md_type_t md_alg,
    const unsigned char *hash, size_t hash_len,
    const unsigned char *sig, size_t sig_len);
static int ax_eckey_check_pair(
    const void *pub, const void *prv);
static int ax_eckeypair_can_do(
    mbedtls_pk_type_t type);
static int ax_ecpubkey_can_do(
    mbedtls_pk_type_t type);

static void ax_eckeypair_free_func(
    void *ctx);
static void ax_ecpubkey_free_func(
    void *ctx);

static const mbedtls_pk_info_t ax_mbedtls_eckeypair_info = {
    MBEDTLS_PK_ECKEY,
    "AxEC_Keypair",
    &ax_eckey_get_bitlen,
    &ax_eckeypair_can_do,
    NULL,
    &ax_eckey_sign,
    NULL, // decrypt_func,
    NULL, // encrypt_func,
    &ax_eckey_check_pair,
    NULL, //&ax_eckey_alloc,
    &ax_eckeypair_free_func,
    NULL, //&ax_eckey_debug,
    };

static const mbedtls_pk_info_t ax_mbedtls_ecpubkey_info = {
    MBEDTLS_PK_ECKEY,
    "AxEC_pubkey",
    &ax_eckey_get_bitlen,
    &ax_ecpubkey_can_do,
    &ax_eckey_verify,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &ax_ecpubkey_free_func,
    NULL,
    };

static mbedtls_ecp_keypair * gmbedtls_ecp_keypair[4];
static mbedtls_ecp_keypair * gmbedtls_ecp_pubkey[3];

int ax_mbedtls_associate_keypair(
    SST_Index_t key_index, mbedtls_pk_context * pkey)
{
    mbedtls_ecp_keypair * pax_ctx;
    HLSE_OBJECT_HANDLE keyPairHandles[A71CH_KEY_PAIR_MAX];
    U16 kpHandleNum = A71CH_KEY_PAIR_MAX;
    U16 kpHandleNumMax;
    U8 i = 0;
    HLSE_RET_CODE err;

    if (key_index >= (sizeof(gmbedtls_ecp_keypair)/sizeof(gmbedtls_ecp_keypair[0]))) {
        /** Return 1 (failed) if key_index is out of range. */
        return 1;
    }
    pkey->pk_info = &ax_mbedtls_eckeypair_info;
    if (gmbedtls_ecp_keypair[key_index] == NULL)
    {
        gmbedtls_ecp_keypair[key_index] = (mbedtls_ecp_keypair *) mbedtls_calloc(1, sizeof(mbedtls_ecp_keypair));
    }
    pax_ctx = gmbedtls_ecp_keypair[key_index];
    err = HLSE_EnumerateObjects(HLSE_KEY_PAIR, keyPairHandles, &kpHandleNum);
    kpHandleNumMax = kpHandleNum;
    if (err != HLSE_SW_OK)
    {
        return 1;
    }
    while(kpHandleNum)
    {
        if (HLSE_GET_OBJECT_INDEX(keyPairHandles[i]) == key_index)
        {
            pax_ctx->grp.hlse_handle = keyPairHandles[i];
            break;
        }
        i++;
        kpHandleNum--;
    }
    if (i >= kpHandleNumMax)
    {
        /*unable to find the keypair at specified index */
        return 1;
    }
    pax_ctx->grp.id = MBEDTLS_ECP_DP_SECP256R1;
#if LOG_API_CALLS
    sm_printf(DBGOUT, "Associating ECC key-pair '%d'.\r\n",HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle));
#endif /* LOG_API_CALLS */
    pkey->pk_ctx = pax_ctx;
    return 0;
}

int ax_mbedtls_associate_pubkey(
    SST_Index_t key_index, mbedtls_pk_context * pkey)
{
    mbedtls_ecp_keypair * pax_ctx;
    HLSE_RET_CODE err;
    HLSE_OBJECT_HANDLE handles[A71CH_PUBLIC_KEY_MAX];
    U16 handleNum = A71CH_PUBLIC_KEY_MAX;
    U16 handleNumMax;
    U8 i = 0;

    if (key_index >= (sizeof(gmbedtls_ecp_pubkey)/sizeof(gmbedtls_ecp_pubkey[0]))) {
        return 1;
    }
    pkey->pk_info = &ax_mbedtls_ecpubkey_info;
    if (gmbedtls_ecp_pubkey[key_index] == NULL)
    {
        gmbedtls_ecp_pubkey[key_index] = (mbedtls_ecp_keypair *) mbedtls_calloc(1, sizeof(mbedtls_ecp_keypair));
    }
    pax_ctx = gmbedtls_ecp_pubkey[key_index];
    err = HLSE_EnumerateObjects(HLSE_PUBLIC_KEY, handles, &handleNum);
    handleNumMax = handleNum;
    if (err != HLSE_SW_OK)
    {
        return 1;
    }
    while(handleNum)
    {
        if (HLSE_GET_OBJECT_INDEX(handles[i]) == key_index)
        {
            pax_ctx->grp.hlse_handle = handles[i];
            break;
        }
        i++;
        handleNum--;
    }
    if (i >= handleNumMax)
    {
        /*Unable to find the key at specified index */
        return 1;

    }
    pax_ctx->grp.id = MBEDTLS_ECP_DP_SECP256R1;

#if LOG_API_CALLS
    sm_printf(DBGOUT, "Associating ECC public key '%d'.\r\n",HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle));
#endif /* LOG_API_CALLS */
    pkey->pk_ctx = pax_ctx;
    return 0;
}

int ax_mbedtls_associate_ecdhctx(SST_Index_t key_index, mbedtls_ssl_handshake_params * handshake) {
    if (key_index >= (sizeof(gmbedtls_ecp_keypair)/sizeof(gmbedtls_ecp_keypair[0]))) {
        return 1;
    }
    handshake->ecdh_ctx.grp.id = MBEDTLS_ECP_DP_SECP256R1;
    handshake->ecdh_ctx.grp.hlse_handle = HLSE_KEY_PAIR | key_index;
#if LOG_API_CALLS > 1
    sm_printf(DBGOUT, "Associating ECC key-pair '%d' for handshake.\r\n",key_index);
#endif
    return 0;
}


static size_t ax_eckey_get_bitlen(
    const void *ctx)
{
    return ((64 << 1) + 1);
}

static int ax_eckey_verify(
    void *ctx, mbedtls_md_type_t md_alg,
    const unsigned char *hash, size_t hash_len,
    const unsigned char *sig, size_t sig_len)
{
    int ret = 1;
    HLSE_RET_CODE err;
    SM_Error_t status =  SW_OK;
    mbedtls_ecp_keypair * pax_ctx = (mbedtls_ecp_keypair *) ctx;
    HLSE_MECHANISM_INFO mechInfo;

#if LOG_API_CALLS
    sm_printf(DBGOUT, "Using ECC key '%d' for verification.\r\n",HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle));
#endif /* LOG_API_CALLS */
    if (HLSE_GET_OBJECT_TYPE(pax_ctx->grp.hlse_handle) == HLSE_PUBLIC_KEY)
    {
        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_VERIFY;
        err = HLSE_VerifySignature(&mechInfo, pax_ctx->grp.hlse_handle , (U8 *)hash, hash_len,
                (U8 *)sig, sig_len);

    }
    else
    {
        sm_printf(DBGOUT, "hlse_handle:0x'%X' is not a public key.\r\n", pax_ctx->grp.hlse_handle);
        err = HLSE_ERR_GENERAL_ERROR;
        status = SW_CONDITIONS_NOT_SATISFIED;
    }

    if ((err == HLSE_SW_OK) && (status == SW_OK))
    {
        ret = 0;
    }
    return (ret);
}

static int ax_eckey_sign(
    void *ctx, mbedtls_md_type_t md_alg,
    const unsigned char *hash, size_t hash_len,
    unsigned char *sig, size_t *sig_len,
    int (*f_rng)(
        void *, unsigned char *, size_t), void *p_rng)
{
    int ret = 0;
    HLSE_RET_CODE err;
    mbedtls_ecp_keypair * pax_ctx = (mbedtls_ecp_keypair *) ctx;
    U16 u16_sig_len = 72; //*sig_len;
    HLSE_MECHANISM_INFO mechInfo;

    if (hash_len > AX_SHA256_LEN)
        hash_len = AX_SHA256_LEN;

#if LOG_API_CALLS
    sm_printf(DBGOUT, "Using ECC key '%d' for signing.\r\n",HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle));
#endif /* LOG_API_CALLS */
    if (HLSE_GET_OBJECT_TYPE(pax_ctx->grp.hlse_handle) == HLSE_KEY_PAIR)
    {

        memset(&mechInfo, 0, sizeof(mechInfo));
        mechInfo.mechanism = HLSE_ECDSA_SIGN;
        err = HLSE_Sign(&mechInfo, pax_ctx->grp.hlse_handle, (U8 *)hash, hash_len, sig, &u16_sig_len);

        *sig_len = u16_sig_len;
        if (err == HLSE_SW_OK)
            ret = 0;
        else
            ret = 1;
    }
    else
    {
        ret = 1;
    }

    return (ret);
}

static int ax_eckey_check_pair(
    const void *pub, const void *prv)
{
    return 0;
}

static int ax_eckeypair_can_do(
    mbedtls_pk_type_t type)
{
    return (type == MBEDTLS_PK_ECKEY ||
        type == MBEDTLS_PK_ECKEY_DH ||
        type == MBEDTLS_PK_ECDSA);
}

static int ax_ecpubkey_can_do(
    mbedtls_pk_type_t type)
{
    return (type == MBEDTLS_PK_ECKEY ||
        type == MBEDTLS_PK_ECKEY_DH ||
        type == MBEDTLS_PK_ECDSA);
}


static void ax_eckeypair_free_func(
    void *ctx)
{
    mbedtls_ecp_keypair * pax_ctx = (mbedtls_ecp_keypair *) ctx;
    if (pax_ctx != NULL
        && pax_ctx->grp.hlse_handle != 0
        && gmbedtls_ecp_keypair[HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle)] != NULL)
    {
        mbedtls_ecp_keypair_free(gmbedtls_ecp_keypair[HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle)]);
        gmbedtls_ecp_keypair[HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle)] = NULL;
        pax_ctx->grp.hlse_handle = 0;
        mbedtls_free(ctx);
    }
    return;
}

static void ax_ecpubkey_free_func(
    void *ctx)
{
    mbedtls_ecp_keypair * pax_ctx = (mbedtls_ecp_keypair *) ctx;
    if (pax_ctx != NULL
        && pax_ctx->grp.hlse_handle != 0
        && gmbedtls_ecp_pubkey[HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle)] != NULL)
    {
        mbedtls_ecp_keypair_free(gmbedtls_ecp_pubkey[HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle)]);
        gmbedtls_ecp_pubkey[HLSE_GET_OBJECT_INDEX(pax_ctx->grp.hlse_handle)] = NULL;
        pax_ctx->grp.hlse_handle = 0;
        mbedtls_free(ctx);
    }
    return;
}

/** @} */

#endif /* MBEDTLS_ECP_ALT */
