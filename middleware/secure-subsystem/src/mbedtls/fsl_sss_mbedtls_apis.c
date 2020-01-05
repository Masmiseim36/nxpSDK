/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fsl_sss_mbedtls_apis.h>

#if SSS_HAVE_MBEDTLS

#include <assert.h>
#include <malloc.h>
#include <mbedtls/version.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

#include <mbedtls/aes.h>
#include <mbedtls/base64.h>
#include <mbedtls/ecdh.h>

#define MAX_KEY_OBJ_COUNT KS_N_ENTIRES
#define MAX_FILE_NAME_SIZE 255
#define MAX_SHARED_SECRET_DERIVED_DATA 255
#define BEGIN_PRIVATE "-----BEGIN PRIVATE KEY-----\n"
#define END_PRIVATE "\n-----END PRIVATE KEY-----"
#define BEGIN_PUBLIC "-----BEGIN PUBLIC KEY-----\n"
#define END_PUBLIC "\n-----END PUBLIC KEY-----"

/* ************************************************************************** */
/* Functions : Private sss mbedtls delceration                                */
/* ************************************************************************** */
static sss_status_t sss_mbedtls_drbg_seed(sss_mbedtls_session_t *pSession, const char *pers);

static sss_status_t sss_mbedtls_generate_ecp_key(mbedtls_pk_context *pkey,
                                                 sss_mbedtls_session_t *pSession,
                                                 size_t keyBitLen);

static sss_status_t sss_mbedtls_generate_rsa_key(mbedtls_pk_context *pkey,
                                                 sss_mbedtls_session_t *pSession,
                                                 size_t keyBitLen);

static sss_status_t sss_mbedtls_set_key(sss_mbedtls_object_t *keyObject,
                                        mbedtls_pk_context *pk,
                                        const uint8_t *key,
                                        size_t keyByteLen);

sss_status_t sss_mbedtls_key_object_allocate(
    sss_mbedtls_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t keyMode);

static sss_status_t sss_mbedtls_hkdf_extract(
    const mbedtls_md_info_t *md, const uint8_t *salt, int salt_len, const uint8_t *ikm, int ikm_len, uint8_t *prk);

static sss_status_t sss_mbedtls_hkdf_expand(const mbedtls_md_info_t *md,
                                            const uint8_t *prk,
                                            int prk_len,
                                            const uint8_t *info,
                                            int info_len,
                                            uint8_t *okm,
                                            int okm_len);

/* ************************************************************************** */
/* Functions : sss_mbedtls_session                                            */
/* ************************************************************************** */

sss_status_t sss_mbedtls_open_session(sss_mbedtls_session_t *session,
                                      sss_type_t subsystem,
                                      uint32_t additionalApplicationId,
                                      uint32_t connectionMethod,
                                      const void *connectionData)
{
    sss_status_t retval = kStatus_SSS_InvalidArgument;
    memset(session, 0, sizeof(*session));
    const char *pers = "mbedtls_session";

    if (0 == additionalApplicationId && 0 == connectionMethod)
    {
#ifdef MBEDTLS_FS_IO
        memset(session, 0, sizeof(*session));
        if (connectionData == NULL)
        {
            retval = kStatus_SSS_Success;
            session->subsystem = subsystem;
        }
        else
        {
            const char *szRootPath = (const char *)connectionData;
            session->szRootPath = szRootPath;
            retval = kStatus_SSS_Success;
            session->ctr_drbg = malloc(sizeof(*session->ctr_drbg));
            session->entropy = malloc(sizeof(*session->entropy));
            mbedtls_ctr_drbg_init((session->ctr_drbg));
            mbedtls_entropy_init((session->entropy));
            retval = sss_mbedtls_drbg_seed(session, pers);
            if (retval != kStatus_SSS_Success)
                goto exit;
            session->subsystem = subsystem;
        }
#else
        if (connectionData == NULL)
        {
            retval = kStatus_SSS_Success;
            session->ctr_drbg = malloc(sizeof(*session->ctr_drbg));
            session->entropy = malloc(sizeof(*session->entropy));
            mbedtls_ctr_drbg_init((session->ctr_drbg));
            mbedtls_entropy_init((session->entropy));
            retval = sss_mbedtls_drbg_seed(session, pers);
            if (retval != kStatus_SSS_Success)
                goto exit;
            session->subsystem = subsystem;
        }
        else
        {
            /* Can't support connectionData  != NULL for mbedTLS without
             * MBEDTLS_FS_IO */
            retval = kStatus_SSS_InvalidArgument;
        }
#endif
    }
exit:
    return retval;
}

void sss_mbedtls_close_session(sss_mbedtls_session_t *session)
{
    if (session->ctr_drbg != NULL)
        free(session->ctr_drbg);
    if (session->entropy != NULL)
        free(session->entropy);
    memset(session, 0, sizeof(*session));
}

/* End: mbedtls_session */

/* ************************************************************************** */
/* Functions : sss_mbedtls_keyobj                                             */
/* ************************************************************************** */

sss_status_t sss_mbedtls_key_object_init(sss_mbedtls_object_t *keyObject, sss_mbedtls_key_store_t *keyStore)
{
    sss_status_t retval = kStatus_SSS_Success;
    assert(keyObject);
    assert(keyStore);
    memset(keyObject, 0, sizeof(*keyObject));
    keyObject->keyStore = keyStore;
    return retval;
}

sss_status_t sss_mbedtls_key_object_allocate(
    sss_mbedtls_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t keyMode)
{
    uint32_t size = 0;
    sss_status_t retval = kStatus_SSS_Fail;
    assert(keyObject);
    keyObject->keyId = keyId;
    keyObject->objectType = keyType;
    keyObject->contents_max_size = keyByteLenMax;
    keyObject->contents_must_free = 1;
    keyObject->keyMode = keyMode;
    switch (keyType)
    {
        case kSSS_KeyType_Certificate:
        case kSSS_KeyType_AES:
            size = keyByteLenMax;
            break;
        case kSSS_KeyType_RSA_Public:
        case kSSS_KeyType_ECC_Public:
        case kSSS_KeyType_ECM_Public:
        case kSSS_KeyType_ECT_Public:
        case kSSS_KeyType_RSA_Pair:
        case kSSS_KeyType_ECC_Pair:
        case kSSS_KeyType_ECM_Pair:
        case kSSS_KeyType_ECT_Pair:
            size = sizeof(mbedtls_pk_context);
            break;
        default:
            break;
    }
    if (size != 0)
    {
        keyObject->contents = malloc(size);
        assert(keyObject->contents);
        memset(keyObject->contents, 0, size);
        retval = kStatus_SSS_Success;
    }
    else
    {
        retval = kStatus_SSS_Fail;
    }
    return retval;
}

sss_status_t sss_mbedtls_key_object_allocate_handle(
    sss_mbedtls_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    assert(keyObject);

    if (options != kKeyObject_Mode_Persistent && options != kKeyObject_Mode_Transient)
    {
        printf(
            "ERROR: sss_mbedtls_key_object_allocate_handle option='%u' not "
            "valid.\n",
            options);
        retval = kStatus_SSS_Fail;
        goto cleanup;
    }
    if (keyType > UINT8_MAX)
    {
        printf("ERROR %s: Only objectType 8 bits wide supported.\n", __FUNCTION__);
        retval = kStatus_SSS_Fail;
        goto cleanup;
    }
    if (options == kKeyObject_Mode_Persistent)
    {
#ifdef MBEDTLS_FS_IO
        uint32_t i;
        sss_mbedtls_object_t **ks;
        assert(keyObject->keyStore);
        if (keyObject->keyStore->max_object_count == 0)
        {
            printf("ERROR: Keystore not yet allocated");
            retval = kStatus_SSS_Fail;
            goto cleanup;
        }
        retval = ks_common_update_fat(keyObject->keyStore->keystore_shadow, keyId, keyType, 0);
        if (retval == kStatus_SSS_Fail)
            goto cleanup;
        ks = keyObject->keyStore->objects;
        for (i = 0; i < keyObject->keyStore->max_object_count; i++)
        {
            if (ks[i] == NULL)
            {
                ks[i] = keyObject;
                retval = sss_mbedtls_key_object_allocate(keyObject, keyId, keyType, keyByteLenMax, options);
                break;
            }
        }
#endif
    }
    else
    {
        retval = sss_mbedtls_key_object_allocate(keyObject, keyId, keyType, keyByteLenMax, options);
    }
cleanup:
    return retval;
}

sss_status_t sss_mbedtls_key_object_get_handle(sss_mbedtls_object_t *keyObject, uint32_t keyId)
{
    sss_status_t retval = kStatus_SSS_Fail;
#ifdef MBEDTLS_FS_IO
    uint32_t i;
    assert(keyObject);
    assert(keyObject->keyStore);
    retval = kStatus_SSS_Success;
    /* If key store already has loaded this and shared this - fail */
    for (i = 0; i < keyObject->keyStore->max_object_count; i++)
    {
        if (keyObject->keyStore->objects[i] != NULL && keyObject->keyStore->objects[i]->keyId == keyId)
        {
            /* Key Object already loaded and shared in another instance */
            printf("ERROR: %s: keyID=%08X already loaded/shared.", __FUNCTION__, keyId);
            retval = kStatus_SSS_Fail;
            break;
        }
    }
    if (retval == kStatus_SSS_Success)
    {
        for (i = 0; i < keyObject->keyStore->max_object_count; i++)
        {
            if (keyObject->keyStore->objects[i] == NULL)
            {
                retval = ks_mbedtls_load_key(keyObject, keyObject->keyStore->keystore_shadow, keyId);
                if (retval == kStatus_SSS_Success)
                {
                    keyObject->keyStore->objects[i] = keyObject;
                }
                break;
            }
        }
    }
#endif
    return retval;
}

sss_status_t sss_mbedtls_key_object_set_user(sss_mbedtls_object_t *keyObject, uint32_t user, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Success;
    return retval;
}

sss_status_t sss_mbedtls_key_object_set_purpose(sss_mbedtls_object_t *keyObject, sss_mode_t purpose, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Success;
    return retval;
}

sss_status_t sss_mbedtls_key_object_set_access(sss_mbedtls_object_t *keyObject, uint32_t access, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Success;
    return retval;
}

sss_status_t sss_mbedtls_key_object_set_eccgfp_group(sss_mbedtls_object_t *keyObject, sss_eccgfp_group_t *group)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_mbedtls_key_object_get_user(sss_mbedtls_object_t *keyObject, uint32_t *user)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_mbedtls_key_object_get_purpose(sss_mbedtls_object_t *keyObject, sss_mode_t *purpose)
{
    sss_status_t retval = kStatus_SSS_Success;
    /* TBU */
    return retval;
}

sss_status_t sss_mbedtls_key_object_get_access(sss_mbedtls_object_t *keyObject, uint32_t *access)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_mbedtls_key_object_free(sss_mbedtls_object_t *keyObject)
{
    if (keyObject != NULL)
    {
#ifdef MBEDTLS_FS_IO
        unsigned int i = 0;
        for (i = 0; i < keyObject->keyStore->max_object_count; i++)
        {
            if (keyObject->keyStore->objects[i] == keyObject)
            {
                keyObject->keyStore->objects[i] = NULL;
                break;
            }
        }
#endif
        if (keyObject->contents != NULL && keyObject->contents_must_free)
        {
            switch (keyObject->objectType)
            {
                case kSSS_KeyType_RSA_Public:
                case kSSS_KeyType_ECC_Public:
                case kSSS_KeyType_ECM_Public:
                case kSSS_KeyType_ECT_Public:
                case kSSS_KeyType_ECC_Pair:
                case kSSS_KeyType_RSA_Pair:
                case kSSS_KeyType_ECM_Pair:
                case kSSS_KeyType_ECT_Pair:
                {
                    mbedtls_pk_context *pk;
                    pk = (mbedtls_pk_context *)keyObject->contents;
                    mbedtls_pk_free(pk);
                    free(pk);
                    break;
                }
                default:
                    free(keyObject->contents);
            }
        }
    }
    memset(keyObject, 0, sizeof(*keyObject));
}

/* End: mbedtls_keyobj */

/* ************************************************************************** */
/* Functions : sss_mbedtls_keyderive                                          */
/* ************************************************************************** */

sss_status_t sss_mbedtls_derive_key_context_init(sss_mbedtls_derive_key_t *context,
                                                 sss_mbedtls_session_t *session,
                                                 sss_mbedtls_object_t *keyObject,
                                                 sss_algorithm_t algorithm,
                                                 sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Success;
    assert(context);
    assert(session);
    assert(keyObject);
    assert(keyObject->contents);
    context->session = session;
    context->keyObject = keyObject;
    context->algorithm = algorithm;
    context->mode = mode;
    return retval;
}

sss_status_t sss_mbedtls_derive_key(sss_mbedtls_derive_key_t *context,
                                    const uint8_t *saltData,
                                    size_t saltLen,
                                    const uint8_t *info,
                                    size_t infoLen,
                                    sss_mbedtls_object_t *derivedKeyObject)
{
    sss_status_t retval = kStatus_SSS_Success;
    const mbedtls_md_info_t *md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    uint8_t *secret;
    size_t secreLen;
    secret = context->keyObject->contents;
    secreLen = context->keyObject->contents_size;
    uint8_t prk[MBEDTLS_MD_MAX_SIZE];
    uint8_t *derivedKey;
    derivedKey = derivedKeyObject->contents;
    retval = sss_mbedtls_hkdf_extract(md, saltData, saltLen, secret, secreLen, prk);
    if (retval == kStatus_SSS_Success)
        return sss_mbedtls_hkdf_expand(md, prk, mbedtls_md_get_size(md), info, infoLen, derivedKey,
                                       MAX_SHARED_SECRET_DERIVED_DATA);
    return retval;
}

sss_status_t sss_mbedtls_asymmetric_dh_derive_key(sss_mbedtls_derive_key_t *context,
                                                  sss_mbedtls_object_t *otherPartyKeyObject,
                                                  sss_mbedtls_object_t *derivedKeyObject)
{
    sss_status_t retval = kStatus_SSS_Success;
    int ret;
    mbedtls_pk_context *pKeyPrv;
    mbedtls_ecp_keypair *pEcpPrv;

    mbedtls_pk_context *pKeyExt;
    mbedtls_ecp_keypair *pEcpExt;
    int sharedSecretLen;
    int sharedSecretLen_Derived;
    mbedtls_mpi rawSharedData;

    pKeyPrv = (mbedtls_pk_context *)context->keyObject->contents;
    pEcpPrv = mbedtls_pk_ec(*pKeyPrv);

    pKeyExt = (mbedtls_pk_context *)otherPartyKeyObject->contents;
    pEcpExt = mbedtls_pk_ec(*pKeyExt);

    mbedtls_mpi_init(&rawSharedData);
    sharedSecretLen = mbedtls_mpi_size(&(pEcpPrv->d));

    ret = mbedtls_ecdh_compute_shared(&pEcpPrv->grp, &rawSharedData, &(pEcpExt->Q), &(pEcpPrv->d),
                                      mbedtls_ctr_drbg_random, context->session->ctr_drbg);
    if (ret != 0)
    {
        printf(" failed\n  ! mbedtls_pk_sign returned -0x%04x\n", -ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }
    sharedSecretLen_Derived = mbedtls_mpi_size(&rawSharedData);
    if (sharedSecretLen_Derived != sharedSecretLen)
    {
        mbedtls_mpi_free(&rawSharedData);
        printf("Failed: Incorrect shared kay length\n");
        retval = kStatus_SSS_Fail;
        goto exit;
    }
    ret = mbedtls_mpi_write_binary(&rawSharedData, derivedKeyObject->contents, derivedKeyObject->contents_max_size);
    if (ret != 0)
    {
        printf("Failed: unable to write shared kay\n");
        retval = kStatus_SSS_Fail;
        goto exit;
    }
    mbedtls_mpi_free(&rawSharedData);
    derivedKeyObject->contents_size = sharedSecretLen_Derived;
exit:
    return retval;
}

void sss_mbedtls_derive_key_context_free(sss_mbedtls_derive_key_t *context)
{
    if (context->keyObject)
        sss_mbedtls_key_object_free(context->keyObject);
    memset(context, 0, sizeof(*context));
}

/* End: mbedtls_keyderive */

/* ************************************************************************** */
/* Functions : sss_mbedtls_keystore                                           */
/* ************************************************************************** */

sss_status_t sss_mbedtls_key_store_context_init(sss_mbedtls_key_store_t *keyStore, sss_mbedtls_session_t *session)
{
    sss_status_t retval = kStatus_SSS_Success;
    assert(keyStore);
    assert(session);
    memset(keyStore, 0, sizeof(*keyStore));
    keyStore->session = session;
    return retval;
}

sss_status_t sss_mbedtls_key_store_allocate(sss_mbedtls_key_store_t *keyStore, uint32_t keyStoreId)
{
    sss_status_t retval = kStatus_SSS_Success;
    assert(keyStore);

#ifdef MBEDTLS_FS_IO
    if (keyStore->objects == NULL)
    {
        keyStore->max_object_count = MAX_KEY_OBJ_COUNT;
        keyStore->objects = (sss_mbedtls_object_t **)malloc(MAX_KEY_OBJ_COUNT * sizeof(sss_mbedtls_object_t *));
        memset(keyStore->objects, 0, (MAX_KEY_OBJ_COUNT * sizeof(sss_mbedtls_object_t *)));
        if (NULL == keyStore->objects)
        {
            printf("ERROR! Could not allocate key store.");
            retval = kStatus_SSS_Fail;
        }
        else
        {
            ks_mbedtls_fat_allocate(&keyStore->keystore_shadow);
            ks_mbedtls_fat_load(keyStore->session->szRootPath, keyStore->keystore_shadow);
            retval = kStatus_SSS_Success;
        }
    }
    else
    {
        printf("ERROR! KeyStore already allocated.");
        retval = kStatus_SSS_Fail;
    }
#endif
    return retval;
}

sss_status_t sss_mbedtls_key_store_save(sss_mbedtls_key_store_t *keyStore)
{
    sss_status_t retval = kStatus_SSS_Success;
    assert(keyStore);
    assert(keyStore->session);
#ifdef MBEDTLS_FS_IO
    if (keyStore->session->szRootPath)
    {
        if (NULL != keyStore->objects)
        {
            uint32_t i;
            for (i = 0; i < keyStore->max_object_count; i++)
            {
                if (NULL != keyStore->objects[i])
                {
                    ks_mbedtls_store_key(keyStore->objects[i]);
                }
            }
        }
        retval = ks_mbedtls_fat_update(keyStore);
    }
    else
    {
        retval = kStatus_SSS_Fail;
    }
#endif
    return retval;
}

sss_status_t sss_mbedtls_key_store_load(sss_mbedtls_key_store_t *keyStore)
{
    sss_status_t retval = kStatus_SSS_Success;
    assert(keyStore);
    assert(keyStore->session);
#ifdef MBEDTLS_FS_IO
    if (keyStore->session->szRootPath)
    {
        if (NULL == keyStore->keystore_shadow)
        {
            ks_mbedtls_fat_allocate(&keyStore->keystore_shadow);
        }
        retval = ks_mbedtls_fat_load(keyStore->session->szRootPath, keyStore->keystore_shadow);
        keyStore->max_object_count = keyStore->keystore_shadow->maxEntries;
    }
    else
    {
        retval = kStatus_SSS_Fail;
    }
#endif
    return retval;
}

sss_status_t sss_mbedtls_key_store_set_key(sss_mbedtls_key_store_t *keyStore,
                                           sss_mbedtls_object_t *keyObject,
                                           const uint8_t *key,
                                           size_t keyBitLen,
                                           void *options,
                                           size_t optionsLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    mbedtls_pk_context *pk = NULL;
    size_t keyByteLen = keyBitLen / 8;
    assert(keyObject);
    assert(keyObject->contents);
    pk = (mbedtls_pk_context *)keyObject->contents;
    retval = sss_mbedtls_set_key(keyObject, pk, key, keyByteLen);
    return retval;
}

sss_status_t sss_mbedtls_key_store_generate_key(sss_mbedtls_key_store_t *keyStore,
                                                sss_mbedtls_object_t *keyObject,
                                                size_t keyBitLen,
                                                void *options)
{
    sss_status_t retval = kStatus_SSS_Success;
    sss_mbedtls_session_t *pS = keyStore->session;
    mbedtls_pk_context *pkey;
    assert(keyObject->contents); /* Must be allocated in allocate handle */
    pkey = (mbedtls_pk_context *)keyObject->contents;

    sss_key_type_t key_type = keyObject->objectType;
    assert(keyStore);
    assert(keyObject);
    assert(keyObject->contents);
    mbedtls_pk_init(pkey);
    switch (key_type)
    {
        case kSSS_KeyType_ECC_Pair:
            retval = sss_mbedtls_generate_ecp_key(pkey, pS, keyBitLen);
            if (retval != kStatus_SSS_Success)
                goto exit;
            break;
        case kSSS_KeyType_RSA_Pair:
            retval = sss_mbedtls_generate_rsa_key(pkey, pS, keyBitLen);
            if (retval != kStatus_SSS_Success)
                goto exit;
            break;
        default:
            break;
    }
exit:
    return retval;
}

sss_status_t sss_mbedtls_key_store_get_key(sss_mbedtls_key_store_t *keyStore,
                                           sss_mbedtls_object_t *keyObject,
                                           uint8_t *key,
                                           size_t *pKeyBitLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    mbedtls_pk_context *pk = NULL;
    int ret = -1;
    assert(keyObject);
    assert(keyObject->contents);
    uint8_t output[256];
    unsigned char *c = output;
    memset(output, 0, 256);

    switch (keyObject->objectType)
    {
        case kSSS_KeyType_Certificate:
        case kSSS_KeyType_AES:
            memcpy(key, keyObject->contents, keyObject->contents_max_size);
            retval = kStatus_SSS_Success;
            break;
        case kSSS_KeyType_RSA_Public:
        case kSSS_KeyType_ECC_Public:
        case kSSS_KeyType_ECM_Public:
        case kSSS_KeyType_ECT_Public:
        case kSSS_KeyType_RSA_Pair:
        case kSSS_KeyType_ECC_Pair:
        case kSSS_KeyType_ECM_Pair:
        case kSSS_KeyType_ECT_Pair:
            pk = (mbedtls_pk_context *)keyObject->contents;
            if (keyObject->keyMode == kKeyObject_Mode_Persistent)
                ret = mbedtls_pk_write_pubkey_der(pk, output, sizeof(output));
            else
                ret = mbedtls_pk_write_key_der(pk, output, sizeof(output));
            if (ret > 0)
            {
                if (*pKeyBitLen < ((uint32_t)ret * 8))
                {
                    retval = kStatus_SSS_Fail;
                }
                else
                {
                    *pKeyBitLen = ret * 8;
                    /* Data is put at end, so copy it to front of output buffer */
                    c = output + sizeof(output) - ret;
                    memcpy(key, c, ret);
                    retval = kStatus_SSS_Success;
                }
            }
            break;
        default:
            break;
    }
    return retval;
}

sss_status_t sss_mbedtls_key_store_open_key(sss_mbedtls_key_store_t *keyStore, sss_mbedtls_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_mbedtls_key_store_freeze_key(sss_mbedtls_key_store_t *keyStore, sss_mbedtls_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_mbedtls_key_store_erase_key(sss_mbedtls_key_store_t *keyStore, sss_mbedtls_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Success;
    assert(keyStore);
    assert(keyObject);

    if (keyObject->keyMode == kKeyObject_Mode_Persistent)
    {
#ifdef MBEDTLS_FS_IO
        /* first check if key exists delete key from shadow KS*/
        retval = ks_common_remove_fat(keyObject->keyStore->keystore_shadow, keyObject->keyId);
        if (retval != kStatus_SSS_Success)
            goto Cleanup;
        /* Update shadow keystore in file system*/
        retval = ks_mbedtls_fat_update(keyObject->keyStore);
        if (retval != kStatus_SSS_Success)
            goto Cleanup;

        /*Clear key object from file*/
        retval = ks_mbedtls_remove_key(keyObject);
#endif
    }
    sss_mbedtls_key_store_save(keyObject->keyStore);
Cleanup:
    return retval;
}

void sss_mbedtls_key_store_context_free(sss_mbedtls_key_store_t *keyStore)
{
#ifdef MBEDTLS_FS_IO
    if (NULL != keyStore->objects)
    {
        uint32_t i;
        for (i = 0; i < keyStore->max_object_count; i++)
        {
            if (keyStore->objects[i] != NULL)
            {
                sss_mbedtls_key_object_free(keyStore->objects[i]);
                keyStore->objects[i] = NULL;
            }
        }
        free(keyStore->objects);
    }
#endif
    memset(keyStore, 0, sizeof(*keyStore));
}

/* End: mbedtls_keystore */

/* ************************************************************************** */
/* Functions : sss_mbedtls_asym                                               */
/* ************************************************************************** */

sss_status_t sss_mbedtls_asymmetric_context_init(sss_mbedtls_asymmetric_t *context,
                                                 sss_mbedtls_session_t *session,
                                                 sss_mbedtls_object_t *keyObject,
                                                 sss_algorithm_t algorithm,
                                                 sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Success;

    assert(context);
    assert(keyObject);
    assert(keyObject->keyStore->session->subsystem == kType_SSS_mbedTLS);

    context->algorithm = algorithm;
    context->mode = mode;
    context->session = session;
    context->keyObject = keyObject;
    return retval;
}

sss_status_t sss_mbedtls_asymmetric_encrypt(
    sss_mbedtls_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    int ret;
    sss_mbedtls_object_t *keyObj = context->keyObject;
    sss_mbedtls_session_t *pS = context->session;
    mbedtls_pk_context *pKey;
    pKey = (mbedtls_pk_context *)keyObj->contents;
    mbedtls_rsa_context *keyRsa;
    keyRsa = mbedtls_pk_rsa(*pKey);
    ret = mbedtls_rsa_pkcs1_encrypt(keyRsa, mbedtls_ctr_drbg_random, pS->ctr_drbg, MBEDTLS_RSA_PUBLIC, srcLen, srcData,
                                    destData);
    if (ret != 0)
    {
        retval = kStatus_SSS_Fail;
        printf(" failed\n  ! mbedtls_rsa_pkcs1_encrypt returned %d\n\n", ret);
        goto exit;
    }
    *destLen = keyRsa->len;
exit:
    return retval;
}

sss_status_t sss_mbedtls_asymmetric_decrypt(
    sss_mbedtls_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    int ret;
    size_t olen;
    uint8_t opPlainTxt[128];
    sss_mbedtls_object_t *keyObj = context->keyObject;
    sss_mbedtls_session_t *pS = context->session;
    mbedtls_pk_context *pKey;
    pKey = (mbedtls_pk_context *)keyObj->contents;

    ret = mbedtls_rsa_pkcs1_decrypt(mbedtls_pk_rsa(*pKey), mbedtls_ctr_drbg_random, pS->ctr_drbg, MBEDTLS_RSA_PRIVATE,
                                    &olen, destData, opPlainTxt, 1024);
    if (ret != 0)
    {
        retval = kStatus_SSS_Fail;
        printf(" failed\n  !mbedtls_rsa_pkcs1_decrypt %d\n\n", ret);
        goto exit;
    }
    if (memcmp(opPlainTxt, srcData, srcLen))
    {
        printf("failed\n.. ! decrypted data does not match with the source data");
        retval = kStatus_SSS_Fail;
        goto exit;
    }
exit:
    return retval;
}

sss_status_t sss_mbedtls_asymmetric_sign_digest(
    sss_mbedtls_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t *signatureLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    int ret;
    unsigned char hash[32];
    sss_mbedtls_session_t *pS = context->session;
    mbedtls_pk_context *pKey = (mbedtls_pk_context *)context->keyObject->contents;

    ret = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), digest, digestLen, hash);

    if (ret != 0)
    {
        printf(" failed\n  ! Could not open or read -%04X\n\n", ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }
    ret = mbedtls_pk_sign(pKey, MBEDTLS_MD_SHA256, hash, 0, signature, signatureLen, mbedtls_ctr_drbg_random,
                          pS->ctr_drbg);

    if (ret != 0)
    {
        printf(" failed\n  ! mbedtls_pk_sign returned -0x%04x\n", -ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }
exit:
    return retval;
}

sss_status_t sss_mbedtls_asymmetric_verify_digest(
    sss_mbedtls_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t signatureLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    sss_mbedtls_session_t *pS = context->session;
    int ret;
    unsigned char hash[32];
    mbedtls_pk_context *pKey = (mbedtls_pk_context *)context->keyObject->contents;

    ret = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), digest, digestLen, hash);
    if (ret != 0)
    {
        printf(" failed\n  ! Could not open or read -%04X\r\n", ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }

    ret = mbedtls_pk_verify(pKey, MBEDTLS_MD_SHA256, hash, 0, signature, signatureLen);
    if (ret != 0)
    {
        printf(" failed\n  ! mbedtls_pk_verify returned -0x%04x\r\n", -ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }
exit:
    return retval;
}

void sss_mbedtls_asymmetric_context_free(sss_mbedtls_asymmetric_t *context)
{
    if (context->keyObject)
        sss_mbedtls_key_object_free(context->keyObject);
    memset(context, 0, sizeof(*context));
}

/* End: mbedtls_asym */

/* ************************************************************************** */
/* Functions : sss_mbedtls_symm                                               */
/* ************************************************************************** */

sss_status_t sss_mbedtls_symmetric_context_init(sss_mbedtls_symmetric_t *context,
                                                sss_mbedtls_session_t *session,
                                                sss_mbedtls_object_t *keyObject,
                                                sss_algorithm_t algorithm,
                                                sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Success;

    context->session = session;
    context->keyObject = keyObject;
    context->algorithm = algorithm;
    context->mode = mode;

    return retval;
}

sss_status_t sss_mbedtls_cipher_one_go(sss_mbedtls_symmetric_t *context,
                                       uint8_t *iv,
                                       size_t ivLen,
                                       const uint8_t *srcData,
                                       uint8_t *destData,
                                       size_t dataLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    mbedtls_aes_context ctx;
    int mbedtls_ret;

    mbedtls_aes_init(&ctx);

    switch (context->mode)
    {
        case kMode_SSS_Encrypt:
            if (context->algorithm == kAlgorithm_SSS_AES_ECB)
            {
                mbedtls_ret =
                    mbedtls_aes_setkey_enc(&ctx, context->keyObject->contents, context->keyObject->contents_size * 8);
                if (mbedtls_ret != 0)
                {
                    return retval;
                }
                mbedtls_ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, srcData, destData);
                if (mbedtls_ret != 0)
                {
                    return retval;
                }
            }
            else if (context->algorithm == kAlgorithm_SSS_AES_CBC)
            {
                mbedtls_ret =
                    mbedtls_aes_setkey_enc(&ctx, context->keyObject->contents, context->keyObject->contents_size * 8);
                if (mbedtls_ret != 0)
                {
                    return retval;
                }
                mbedtls_ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, dataLen, iv, srcData, destData);
                if (mbedtls_ret != 0)
                {
                    return retval;
                }
            }
            break;
        case kMode_SSS_Decrypt:
            mbedtls_ret =
                mbedtls_aes_setkey_dec(&ctx, context->keyObject->contents, context->keyObject->contents_size * 8);
            if (mbedtls_ret != 0)
            {
                return retval;
            }
            if (context->algorithm == kAlgorithm_SSS_AES_ECB)
            {
                mbedtls_ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, srcData, destData);
                if (mbedtls_ret != 0)
                {
                    return retval;
                }
            }
            else if (context->algorithm == kAlgorithm_SSS_AES_CBC)
            {
                mbedtls_ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, dataLen, iv, srcData, destData);
                if (mbedtls_ret != 0)
                {
                    return retval;
                }
            }
            break;
        default:
            retval = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
            break;
    }

    mbedtls_aes_free(&ctx);

    /* TBU */
    return retval;
}

sss_status_t sss_mbedtls_cipher_init(sss_mbedtls_symmetric_t *context, uint8_t *iv, size_t ivLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    const mbedtls_cipher_info_t *cipher_info;
    context->cipher_ctx = (mbedtls_cipher_context_t *)malloc(sizeof(mbedtls_cipher_context_t));

    if (iv != NULL && ivLen > 0)
    {
        if (context->algorithm == kAlgorithm_SSS_AES_ECB)
        {
            cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
        }

        mbedtls_cipher_init(context->cipher_ctx);

        if (0 == mbedtls_cipher_setup(context->cipher_ctx, cipher_info))
        {
            if (context->mode == kMode_SSS_Encrypt)
            {
                if (mbedtls_cipher_setkey(context->cipher_ctx, context->keyObject->contents,
                                          (context->keyObject->contents_size * 8), MBEDTLS_ENCRYPT) != 0)
                {
                    retval = kStatus_SSS_InvalidArgument;
                }
            }
            else if (context->mode == kMode_SSS_Decrypt)
            {
                if (mbedtls_cipher_setkey(context->cipher_ctx, context->keyObject->contents,
                                          (context->keyObject->contents_size * 8), MBEDTLS_DECRYPT) != 0)
                {
                    retval = kStatus_SSS_InvalidArgument;
                }
            }
            else
            {
                retval = kStatus_SSS_InvalidArgument;
            }
            if (retval == kStatus_SSS_Success)
            {
                mbedtls_cipher_set_iv(context->cipher_ctx, iv, ivLen);
                mbedtls_cipher_reset(context->cipher_ctx);
            }
        }
    }
    return retval;
}

sss_status_t sss_mbedtls_cipher_update(
    sss_mbedtls_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    int retMbedtlsVal;
    size_t ilen;
    size_t iolen = 0;
    uint32_t offset;
    uint16_t blockSize;

    if (srcData != NULL && destData != NULL && srcLen != 0)
    {
        blockSize = mbedtls_cipher_get_block_size(context->cipher_ctx);
        for (offset = 0x00; offset < srcLen; offset += blockSize)
        {
            ilen = ((srcLen - offset) > blockSize) ? blockSize : (srcLen - offset);

            retMbedtlsVal = mbedtls_cipher_update(context->cipher_ctx, srcData, ilen, &destData[offset], &iolen);

            if (retMbedtlsVal != 0)
            {
                return retval;
            }
            *destLen += iolen;
        }

        retval = kStatus_SSS_Success;
    }
    return retval;
}

sss_status_t sss_mbedtls_cipher_finish(
    sss_mbedtls_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    uint8_t *inputbuf = (uint8_t *)srcData;

    if ((mbedtls_cipher_finish(context->cipher_ctx, inputbuf, &srcLen)) != 0)
    {
        retval = kStatus_SSS_Fail;
    }
    mbedtls_cipher_free(context->cipher_ctx);
    memset(context->cipher_ctx, 0, sizeof(*(context->cipher_ctx)));
    free(context->cipher_ctx);

    return retval;
}

sss_status_t sss_mbedtls_cipher_crypt_ctr(sss_mbedtls_symmetric_t *context,
                                          const uint8_t *srcData,
                                          uint8_t *destData,
                                          size_t size,
                                          uint8_t *initialCounter,
                                          uint8_t *lastEncryptedCounter,
                                          size_t *szLeft)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_mbedtls_symmetric_context_free(sss_mbedtls_symmetric_t *context)
{
    ;
}

/* End: mbedtls_symm */

/* ************************************************************************** */
/* Functions : Private sss mbedtls functions                                  */
/* ************************************************************************** */

static sss_status_t sss_mbedtls_set_key(sss_mbedtls_object_t *keyObject,
                                        mbedtls_pk_context *pk,
                                        const uint8_t *key,
                                        size_t keyByteLen)
{
    sss_status_t retval = kStatus_SSS_Success;
    int ret;
    size_t base64_olen;
    char pem_format[1024];
    char base64_format[1024];
    ret = mbedtls_base64_encode(base64_format, sizeof(base64_format), &base64_olen, key, keyByteLen);

    switch (keyObject->objectType)
    {
        case kSSS_KeyType_Certificate:
        case kSSS_KeyType_AES:
            if (keyByteLen > keyObject->contents_max_size)
            {
                printf(
                    "ERROR: Not enough memory for key_size='%d' bytes. Have only "
                    "'%d'\n",
                    keyByteLen, keyObject->contents_max_size);
                retval = kStatus_SSS_Fail;
            }
            else
            {
                memcpy(keyObject->contents, key, keyByteLen);
                keyObject->contents_size = keyByteLen;
            }
            break;
        case kSSS_KeyType_ECC_Private:
        case kSSS_KeyType_ECC_Pair:
            snprintf(pem_format, sizeof(pem_format), BEGIN_PRIVATE "%s" END_PRIVATE, base64_format);
            ret = mbedtls_pk_parse_key(pk, pem_format, strlen(pem_format) + 1, NULL, 0);
            (ret == 0) ? (retval = kStatus_SSS_Success) : (retval = kStatus_SSS_Fail);
            break;
        case kSSS_KeyType_ECC_Public:
            snprintf(pem_format, sizeof(pem_format), BEGIN_PUBLIC "%s" END_PUBLIC, base64_format);
            ret = mbedtls_pk_parse_public_key(pk, pem_format, strlen(pem_format) + 1);
            (ret == 0) ? (retval = kStatus_SSS_Success) : (retval = kStatus_SSS_Fail);
            break;
        default:
            break;
    }
    return retval;
}

static sss_status_t sss_mbedtls_drbg_seed(sss_mbedtls_session_t *pSession, const char *pers)
{
    int ret;
    sss_status_t retval = kStatus_SSS_Success;
    ret = mbedtls_ctr_drbg_seed(pSession->ctr_drbg, mbedtls_entropy_func, pSession->entropy,
                                (const unsigned char *)pers, strlen(pers));
    if (ret != 0)
    {
        printf(" failed\n  ! mbedtls_ctr_drbg_seed returned -0x%04x\n", -ret);
        retval = kStatus_SSS_Fail;
    }
    return (retval);
}

static sss_status_t sss_mbedtls_generate_ecp_key(mbedtls_pk_context *pkey,
                                                 sss_mbedtls_session_t *pSession,
                                                 size_t keyBitLen)
{
    int ret;
    sss_status_t retval = kStatus_SSS_Success;

    ret = mbedtls_pk_setup(pkey, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));
    if (ret != 0)
    {
        printf(" failed\n  !  mbedtls_pk_setup returned -0x%04x", -ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }

    if (keyBitLen == 256)
    {
        ret = mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(*pkey), mbedtls_ctr_drbg_random,
                                  pSession->ctr_drbg);
    }
    else
    {
        printf("ERROR: %s: Don't support keyBitLen='%u' right now", __FUNCTION__, keyBitLen);
        ret = 1;
    }

    if (ret != 0)
    {
        printf(" failed\n  !  mbedtls_ecp_gen_key returned -0x%04x", -ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }
exit:
    return retval;
}

static sss_status_t sss_mbedtls_generate_rsa_key(mbedtls_pk_context *pkey,
                                                 sss_mbedtls_session_t *pSession,
                                                 size_t keyBitLen)
{
    int ret;
    sss_status_t retval = kStatus_SSS_Success;

    ret = mbedtls_pk_setup(pkey, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA));
    if (ret != 0)
    {
        printf(" failed\n  !  mbedtls_pk_setup returned -0x%04x", -ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }

    if (keyBitLen == 1024)
    {
        ret =
            mbedtls_rsa_gen_key(mbedtls_pk_rsa(*pkey), mbedtls_ctr_drbg_random, (pSession->ctr_drbg), keyBitLen, 65537);
    }
    else
    {
        printf("ERROR: %s: Don't support keyBitLen='%u' right now", __FUNCTION__, keyBitLen);
        ret = 1;
    }

    if (ret != 0)
    {
        printf(" failed\n  !  mbedtls_rsa_gen_key returned -0x%04x", -ret);
        retval = kStatus_SSS_Fail;
        goto exit;
    }
exit:
    return retval;
}
static sss_status_t sss_mbedtls_hkdf_expand(const mbedtls_md_info_t *md,
                                            const uint8_t *prk,
                                            int prk_len,
                                            const uint8_t *info,
                                            int info_len,
                                            uint8_t *okm,
                                            int okm_len)
{
    int hash_len;
    int N;
    int T_len = 0, where = 0, i, ret;
    mbedtls_md_context_t ctx;
    unsigned char T[MBEDTLS_MD_MAX_SIZE];
    sss_status_t retval = kStatus_SSS_Success;

    if (info_len < 0 || okm_len < 0 || okm == NULL)
    {
        retval = kStatus_SSS_InvalidArgument;
        goto exit;
    }

    hash_len = mbedtls_md_get_size(md);

    if (info == NULL)
    {
        info = (const unsigned char *)"";
    }

    N = okm_len / hash_len;

    if ((okm_len % hash_len) != 0)
    {
        N++;
    }

    if (N > 255)
    {
        retval = kStatus_SSS_InvalidArgument;
        goto exit;
    }

    mbedtls_md_init(&ctx);

    if ((ret = mbedtls_md_setup(&ctx, md, 1)) != 0)
    {
        mbedtls_md_free(&ctx);
        retval = kStatus_SSS_Fail;
        goto exit;
    }

    /* Section 2.3. */
    for (i = 1; i <= N; i++)
    {
        unsigned char c = i;

        ret = mbedtls_md_hmac_starts(&ctx, prk, prk_len) || mbedtls_md_hmac_update(&ctx, T, T_len) ||
              mbedtls_md_hmac_update(&ctx, info, info_len) ||
              /* The constant concatenated to the end of each T(n) is a single
              octet. */
              mbedtls_md_hmac_update(&ctx, &c, 1) || mbedtls_md_hmac_finish(&ctx, T);

        if (ret != 0)
        {
            mbedtls_md_free(&ctx);
            retval = kStatus_SSS_Fail;
            goto exit;
        }

        memcpy(okm + where, T, (i != N) ? hash_len : (okm_len - where));
        where += hash_len;
        T_len = hash_len;
    }

    mbedtls_md_free(&ctx);
exit:
    return retval;
}

static sss_status_t sss_mbedtls_hkdf_extract(
    const mbedtls_md_info_t *md, const uint8_t *salt, int salt_len, const uint8_t *ikm, int ikm_len, uint8_t *prk)
{
    int hash_len;
    int ret;
    unsigned char null_salt[MBEDTLS_MD_MAX_SIZE] = {'\0'};
    sss_status_t retval = kStatus_SSS_Success;

    if (salt_len < 0)
    {
        return kStatus_SSS_Fail;
    }

    hash_len = mbedtls_md_get_size(md);

    if (salt == NULL)
    {
        salt = null_salt;
        salt_len = hash_len;
    }

    ret = mbedtls_md_hmac(md, salt, salt_len, ikm, ikm_len, prk);
    if (ret != 0)
    {
        retval = kStatus_SSS_Fail;
    }
    return retval;
}
#endif /* SSS_HAVE_MBEDTLS */
