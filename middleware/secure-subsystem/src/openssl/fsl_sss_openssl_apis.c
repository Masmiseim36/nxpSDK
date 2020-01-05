/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fsl_sss_openssl_apis.h>

#if SSS_HAVE_OPENSSL

/* ************************************************************************** */
/* Functions : sss_openssl_session                                            */
/* ************************************************************************** */

sss_status_t sss_openssl_open_session(sss_openssl_session_t *session,
                                      sss_type_t subsystem,
                                      uint32_t additionalApplicationId,
                                      uint32_t connectionMethod,
                                      const void *connectionData)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_openssl_close_session(sss_openssl_session_t *session)
{
    ;
}

/* End: openssl_session */

/* ************************************************************************** */
/* Functions : sss_openssl_keyobj                                             */
/* ************************************************************************** */

sss_status_t sss_openssl_key_object_init(sss_openssl_object_t *keyObject, sss_openssl_key_store_t *keyStore)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_allocate_handle(
    sss_openssl_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_get_handle(sss_openssl_object_t *keyObject, uint32_t keyId)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_set_user(sss_openssl_object_t *keyObject, uint32_t user, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_set_purpose(sss_openssl_object_t *keyObject, sss_mode_t purpose, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_set_access(sss_openssl_object_t *keyObject, uint32_t access, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_set_eccgfp_group(sss_openssl_object_t *keyObject, sss_eccgfp_group_t *group)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_get_user(sss_openssl_object_t *keyObject, uint32_t *user)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_get_purpose(sss_openssl_object_t *keyObject, sss_mode_t *purpose)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_object_get_access(sss_openssl_object_t *keyObject, uint32_t *access)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_openssl_key_object_free(sss_openssl_object_t *keyObject)
{
    ;
}

/* End: openssl_keyobj */

/* ************************************************************************** */
/* Functions : sss_openssl_keyderive                                          */
/* ************************************************************************** */

sss_status_t sss_openssl_derive_key_context_init(sss_openssl_derive_key_t *context,
                                                 sss_openssl_session_t *session,
                                                 sss_openssl_object_t *keyObject,
                                                 sss_algorithm_t algorithm,
                                                 sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_derive_key(sss_openssl_derive_key_t *context,
                                    const uint8_t *saltData,
                                    size_t saltLen,
                                    const uint8_t *info,
                                    size_t infoLen,
                                    sss_openssl_object_t *derivedKeyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_asymmetric_dh_derive_key(sss_openssl_derive_key_t *context,
                                                  sss_openssl_object_t *otherPartyKeyObject,
                                                  sss_openssl_object_t *derivedKeyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_openssl_derive_key_context_free(sss_openssl_derive_key_t *context)
{
    ;
}

/* End: openssl_keyderive */

/* ************************************************************************** */
/* Functions : sss_openssl_keystore                                           */
/* ************************************************************************** */

sss_status_t sss_openssl_key_store_context_init(sss_openssl_key_store_t *keyStore, sss_openssl_session_t *session)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_allocate(sss_openssl_key_store_t *keyStore, uint32_t keyStoreId)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_save(sss_openssl_key_store_t *keyStore)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_load(sss_openssl_key_store_t *keyStore)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_set_key(sss_openssl_key_store_t *keyStore,
                                           sss_openssl_object_t *keyObject,
                                           const uint8_t *key,
                                           size_t keyBitLen,
                                           void *options,
                                           size_t optionsLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_generate_key(sss_openssl_key_store_t *keyStore,
                                                sss_openssl_object_t *keyObject,
                                                size_t keyBitLen,
                                                void *options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_get_key(sss_openssl_key_store_t *keyStore,
                                           sss_openssl_object_t *keyObject,
                                           uint8_t *key,
                                           size_t *pKeyBitLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_open_key(sss_openssl_key_store_t *keyStore, sss_openssl_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_freeze_key(sss_openssl_key_store_t *keyStore, sss_openssl_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_key_store_erase_key(sss_openssl_key_store_t *keyStore, sss_openssl_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_openssl_key_store_context_free(sss_openssl_key_store_t *keyStore)
{
    ;
}

/* End: openssl_keystore */

/* ************************************************************************** */
/* Functions : sss_openssl_asym                                               */
/* ************************************************************************** */

sss_status_t sss_openssl_asymmetric_context_init(sss_openssl_asymmetric_t *context,
                                                 sss_openssl_session_t *session,
                                                 sss_openssl_object_t *keyObject,
                                                 sss_algorithm_t algorithm,
                                                 sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_asymmetric_encrypt(
    sss_openssl_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_asymmetric_decrypt(
    sss_openssl_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_asymmetric_sign_digest(
    sss_openssl_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t *signatureLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_asymmetric_verify_digest(
    sss_openssl_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t signatureLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_openssl_asymmetric_context_free(sss_openssl_asymmetric_t *context)
{
    ;
}

/* End: openssl_asym */

/* ************************************************************************** */
/* Functions : sss_openssl_symm                                               */
/* ************************************************************************** */

sss_status_t sss_openssl_symmetric_context_init(sss_openssl_symmetric_t *context,
                                                sss_openssl_session_t *session,
                                                sss_openssl_object_t *keyObject,
                                                sss_algorithm_t algorithm,
                                                sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_cipher_one_go(sss_openssl_symmetric_t *context,
                                       uint8_t *iv,
                                       size_t ivLen,
                                       const uint8_t *srcData,
                                       uint8_t *destData,
                                       size_t dataLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_cipher_init(sss_openssl_symmetric_t *context, uint8_t *iv, size_t ivLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_cipher_update(
    sss_openssl_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_cipher_finish(
    sss_openssl_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_openssl_cipher_crypt_ctr(sss_openssl_symmetric_t *context,
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

void sss_openssl_symmetric_context_free(sss_openssl_symmetric_t *context)
{
    ;
}

/* End: openssl_symm */

#endif /* SSS_HAVE_OPENSSL */
