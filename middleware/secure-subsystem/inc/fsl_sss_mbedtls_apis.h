/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_SSS_MBEDTLS_APIS_H
#define FSL_SSS_MBEDTLS_APIS_H

#if !defined(SSS_FTR_FILE)
#include "fsl_sss_ftr_default.h"
#else
#include SSS_FTR_FILE
#endif

#if SSS_HAVE_MBEDTLS
#include <fsl_sss_mbedtls_types.h>

/* ************************************************************************** */
/* Functions                                                                  */
/* ************************************************************************** */
/**
 * @addtogroup sss_mbedtls_session
 * @{
 */
/** @copydoc sss_open_session
 *
 */
sss_status_t sss_mbedtls_open_session(sss_mbedtls_session_t *session,
                                      sss_type_t subsystem,
                                      uint32_t additionalApplicationId,
                                      uint32_t connectionMethod,
                                      const void *connectionData);

/** @copydoc sss_close_session
 *
 */
void sss_mbedtls_close_session(sss_mbedtls_session_t *session);

/*! @} */ /* end of : sss_mbedtls_session */

/**
 * @addtogroup sss_mbedtls_keyobj
 * @{
 */
/** @copydoc sss_key_object_init
 *
 */
sss_status_t sss_mbedtls_key_object_init(sss_mbedtls_object_t *keyObject, sss_mbedtls_key_store_t *keyStore);

/** @copydoc sss_key_object_allocate_handle
 *
 */
sss_status_t sss_mbedtls_key_object_allocate_handle(
    sss_mbedtls_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t options);

/** @copydoc sss_key_object_get_handle
 *
 */
sss_status_t sss_mbedtls_key_object_get_handle(sss_mbedtls_object_t *keyObject, uint32_t keyId);

/** @copydoc sss_key_object_set_user
 *
 */
sss_status_t sss_mbedtls_key_object_set_user(sss_mbedtls_object_t *keyObject, uint32_t user, uint32_t options);

/** @copydoc sss_key_object_set_purpose
 *
 */
sss_status_t sss_mbedtls_key_object_set_purpose(sss_mbedtls_object_t *keyObject, sss_mode_t purpose, uint32_t options);

/** @copydoc sss_key_object_set_access
 *
 */
sss_status_t sss_mbedtls_key_object_set_access(sss_mbedtls_object_t *keyObject, uint32_t access, uint32_t options);

/** @copydoc sss_key_object_set_eccgfp_group
 *
 */
sss_status_t sss_mbedtls_key_object_set_eccgfp_group(sss_mbedtls_object_t *keyObject, sss_eccgfp_group_t *group);

/** @copydoc sss_key_object_get_user
 *
 */
sss_status_t sss_mbedtls_key_object_get_user(sss_mbedtls_object_t *keyObject, uint32_t *user);

/** @copydoc sss_key_object_get_purpose
 *
 */
sss_status_t sss_mbedtls_key_object_get_purpose(sss_mbedtls_object_t *keyObject, sss_mode_t *purpose);

/** @copydoc sss_key_object_get_access
 *
 */
sss_status_t sss_mbedtls_key_object_get_access(sss_mbedtls_object_t *keyObject, uint32_t *access);

/** @copydoc sss_key_object_free
 *
 */
void sss_mbedtls_key_object_free(sss_mbedtls_object_t *keyObject);

/*! @} */ /* end of : sss_mbedtls_keyobj */

/**
 * @addtogroup sss_mbedtls_keyderive
 * @{
 */
/** @copydoc sss_derive_key_context_init
 *
 */
sss_status_t sss_mbedtls_derive_key_context_init(sss_mbedtls_derive_key_t *context,
                                                 sss_mbedtls_session_t *session,
                                                 sss_mbedtls_object_t *keyObject,
                                                 sss_algorithm_t algorithm,
                                                 sss_mode_t mode);

/** @copydoc sss_derive_key
 *
 */
sss_status_t sss_mbedtls_derive_key(sss_mbedtls_derive_key_t *context,
                                    const uint8_t *saltData,
                                    size_t saltLen,
                                    const uint8_t *info,
                                    size_t infoLen,
                                    sss_mbedtls_object_t *derivedKeyObject);

/** @copydoc sss_asymmetric_dh_derive_key
 *
 */
sss_status_t sss_mbedtls_asymmetric_dh_derive_key(sss_mbedtls_derive_key_t *context,
                                                  sss_mbedtls_object_t *otherPartyKeyObject,
                                                  sss_mbedtls_object_t *derivedKeyObject);

/** @copydoc sss_derive_key_context_free
 *
 */
void sss_mbedtls_derive_key_context_free(sss_mbedtls_derive_key_t *context);

/*! @} */ /* end of : sss_mbedtls_keyderive */

/**
 * @addtogroup sss_mbedtls_keystore
 * @{
 */
/** @copydoc sss_key_store_context_init
 *
 */
sss_status_t sss_mbedtls_key_store_context_init(sss_mbedtls_key_store_t *keyStore, sss_mbedtls_session_t *session);

/** @copydoc sss_key_store_allocate
 *
 */
sss_status_t sss_mbedtls_key_store_allocate(sss_mbedtls_key_store_t *keyStore, uint32_t keyStoreId);

/** @copydoc sss_key_store_save
 *
 */
sss_status_t sss_mbedtls_key_store_save(sss_mbedtls_key_store_t *keyStore);

/** @copydoc sss_key_store_load
 *
 */
sss_status_t sss_mbedtls_key_store_load(sss_mbedtls_key_store_t *keyStore);

/** @copydoc sss_key_store_set_key
 *
 */
sss_status_t sss_mbedtls_key_store_set_key(sss_mbedtls_key_store_t *keyStore,
                                           sss_mbedtls_object_t *keyObject,
                                           const uint8_t *key,
                                           size_t keyBitLen,
                                           void *options,
                                           size_t optionsLen);

/** @copydoc sss_key_store_generate_key
 *
 */
sss_status_t sss_mbedtls_key_store_generate_key(sss_mbedtls_key_store_t *keyStore,
                                                sss_mbedtls_object_t *keyObject,
                                                size_t keyBitLen,
                                                void *options);

/** @copydoc sss_key_store_get_key
 *
 */
sss_status_t sss_mbedtls_key_store_get_key(sss_mbedtls_key_store_t *keyStore,
                                           sss_mbedtls_object_t *keyObject,
                                           uint8_t *key,
                                           size_t *pKeyBitLen);

/** @copydoc sss_key_store_open_key
 *
 */
sss_status_t sss_mbedtls_key_store_open_key(sss_mbedtls_key_store_t *keyStore, sss_mbedtls_object_t *keyObject);

/** @copydoc sss_key_store_freeze_key
 *
 */
sss_status_t sss_mbedtls_key_store_freeze_key(sss_mbedtls_key_store_t *keyStore, sss_mbedtls_object_t *keyObject);

/** @copydoc sss_key_store_erase_key
 *
 */
sss_status_t sss_mbedtls_key_store_erase_key(sss_mbedtls_key_store_t *keyStore, sss_mbedtls_object_t *keyObject);

/** @copydoc sss_key_store_context_free
 *
 */
void sss_mbedtls_key_store_context_free(sss_mbedtls_key_store_t *keyStore);

/*! @} */ /* end of : sss_mbedtls_keystore */

/**
 * @addtogroup sss_mbedtls_asym
 * @{
 */
/** @copydoc sss_asymmetric_context_init
 *
 */
sss_status_t sss_mbedtls_asymmetric_context_init(sss_mbedtls_asymmetric_t *context,
                                                 sss_mbedtls_session_t *session,
                                                 sss_mbedtls_object_t *keyObject,
                                                 sss_algorithm_t algorithm,
                                                 sss_mode_t mode);

/** @copydoc sss_asymmetric_encrypt
 *
 */
sss_status_t sss_mbedtls_asymmetric_encrypt(
    sss_mbedtls_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen);

/** @copydoc sss_asymmetric_decrypt
 *
 */
sss_status_t sss_mbedtls_asymmetric_decrypt(
    sss_mbedtls_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen);

/** @copydoc sss_asymmetric_sign_digest
 *
 */
sss_status_t sss_mbedtls_asymmetric_sign_digest(
    sss_mbedtls_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t *signatureLen);

/** @copydoc sss_asymmetric_verify_digest
 *
 */
sss_status_t sss_mbedtls_asymmetric_verify_digest(
    sss_mbedtls_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t signatureLen);

/** @copydoc sss_asymmetric_context_free
 *
 */
void sss_mbedtls_asymmetric_context_free(sss_mbedtls_asymmetric_t *context);

/*! @} */ /* end of : sss_mbedtls_asym */

/**
 * @addtogroup sss_mbedtls_symm
 * @{
 */
/** @copydoc sss_symmetric_context_init
 *
 */
sss_status_t sss_mbedtls_symmetric_context_init(sss_mbedtls_symmetric_t *context,
                                                sss_mbedtls_session_t *session,
                                                sss_mbedtls_object_t *keyObject,
                                                sss_algorithm_t algorithm,
                                                sss_mode_t mode);

/** @copydoc sss_cipher_one_go
 *
 */
sss_status_t sss_mbedtls_cipher_one_go(sss_mbedtls_symmetric_t *context,
                                       uint8_t *iv,
                                       size_t ivLen,
                                       const uint8_t *srcData,
                                       uint8_t *destData,
                                       size_t dataLen);

/** @copydoc sss_cipher_init
 *
 */
sss_status_t sss_mbedtls_cipher_init(sss_mbedtls_symmetric_t *context, uint8_t *iv, size_t ivLen);

/** @copydoc sss_cipher_update
 *
 */
sss_status_t sss_mbedtls_cipher_update(
    sss_mbedtls_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen);

/** @copydoc sss_cipher_finish
 *
 */
sss_status_t sss_mbedtls_cipher_finish(
    sss_mbedtls_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen);

/** @copydoc sss_cipher_crypt_ctr
 *
 */
sss_status_t sss_mbedtls_cipher_crypt_ctr(sss_mbedtls_symmetric_t *context,
                                          const uint8_t *srcData,
                                          uint8_t *destData,
                                          size_t size,
                                          uint8_t *initialCounter,
                                          uint8_t *lastEncryptedCounter,
                                          size_t *szLeft);

/** @copydoc sss_symmetric_context_free
 *
 */
void sss_mbedtls_symmetric_context_free(sss_mbedtls_symmetric_t *context);

/*! @} */ /* end of : sss_mbedtls_symm */

/* clang-format off */
#   if (SSS_HAVE_SSS == 1)
        /* Direct Call : session */
#       define sss_open_session(session,subsystem,additionalApplicationId,connectionMethod,connectionData) \
            sss_mbedtls_open_session(((sss_mbedtls_session_t * ) session),(subsystem),(additionalApplicationId),(connectionMethod),(connectionData))
#       define sss_close_session(session) \
            sss_mbedtls_close_session(((sss_mbedtls_session_t * ) session))
        /* Direct Call : keyobj */
#       define sss_key_object_init(keyObject,keyStore) \
            sss_mbedtls_key_object_init(((sss_mbedtls_object_t * ) keyObject),((sss_mbedtls_key_store_t * ) keyStore))
#       define sss_key_object_allocate_handle(keyObject,keyId,keyType,keyByteLenMax,options) \
            sss_mbedtls_key_object_allocate_handle(((sss_mbedtls_object_t * ) keyObject),(keyId),(keyType),(keyByteLenMax),(options))
#       define sss_key_object_get_handle(keyObject,keyId) \
            sss_mbedtls_key_object_get_handle(((sss_mbedtls_object_t * ) keyObject),(keyId))
#       define sss_key_object_set_user(keyObject,user,options) \
            sss_mbedtls_key_object_set_user(((sss_mbedtls_object_t * ) keyObject),(user),(options))
#       define sss_key_object_set_purpose(keyObject,purpose,options) \
            sss_mbedtls_key_object_set_purpose(((sss_mbedtls_object_t * ) keyObject),(purpose),(options))
#       define sss_key_object_set_access(keyObject,access,options) \
            sss_mbedtls_key_object_set_access(((sss_mbedtls_object_t * ) keyObject),(access),(options))
#       define sss_key_object_set_eccgfp_group(keyObject,group) \
            sss_mbedtls_key_object_set_eccgfp_group(((sss_mbedtls_object_t * ) keyObject),(group))
#       define sss_key_object_get_user(keyObject,user) \
            sss_mbedtls_key_object_get_user(((sss_mbedtls_object_t * ) keyObject),(user))
#       define sss_key_object_get_purpose(keyObject,purpose) \
            sss_mbedtls_key_object_get_purpose(((sss_mbedtls_object_t * ) keyObject),(purpose))
#       define sss_key_object_get_access(keyObject,access) \
            sss_mbedtls_key_object_get_access(((sss_mbedtls_object_t * ) keyObject),(access))
#       define sss_key_object_free(keyObject) \
            sss_mbedtls_key_object_free(((sss_mbedtls_object_t * ) keyObject))
        /* Direct Call : keyderive */
#       define sss_derive_key_context_init(context,session,keyObject,algorithm,mode) \
            sss_mbedtls_derive_key_context_init(((sss_mbedtls_derive_key_t * ) context),((sss_mbedtls_session_t * ) session),((sss_mbedtls_object_t * ) keyObject),(algorithm),(mode))
#       define sss_derive_key(context,saltData,saltLen,info,infoLen,derivedKeyObject) \
            sss_mbedtls_derive_key(((sss_mbedtls_derive_key_t * ) context),(saltData),(saltLen),(info),(infoLen),((sss_mbedtls_object_t * ) derivedKeyObject))
#       define sss_asymmetric_dh_derive_key(context,otherPartyKeyObject,derivedKeyObject) \
            sss_mbedtls_asymmetric_dh_derive_key(((sss_mbedtls_derive_key_t * ) context),((sss_mbedtls_object_t * ) otherPartyKeyObject),((sss_mbedtls_object_t * ) derivedKeyObject))
#       define sss_derive_key_context_free(context) \
            sss_mbedtls_derive_key_context_free(((sss_mbedtls_derive_key_t * ) context))
        /* Direct Call : keystore */
#       define sss_key_store_context_init(keyStore,session) \
            sss_mbedtls_key_store_context_init(((sss_mbedtls_key_store_t * ) keyStore),((sss_mbedtls_session_t * ) session))
#       define sss_key_store_allocate(keyStore,keyStoreId) \
            sss_mbedtls_key_store_allocate(((sss_mbedtls_key_store_t * ) keyStore),(keyStoreId))
#       define sss_key_store_save(keyStore) \
            sss_mbedtls_key_store_save(((sss_mbedtls_key_store_t * ) keyStore))
#       define sss_key_store_load(keyStore) \
            sss_mbedtls_key_store_load(((sss_mbedtls_key_store_t * ) keyStore))
#       define sss_key_store_set_key(keyStore,keyObject,key,keyBitLen,options,optionsLen) \
            sss_mbedtls_key_store_set_key(((sss_mbedtls_key_store_t * ) keyStore),((sss_mbedtls_object_t * ) keyObject),(key),(keyBitLen),(options),(optionsLen))
#       define sss_key_store_generate_key(keyStore,keyObject,keyBitLen,options) \
            sss_mbedtls_key_store_generate_key(((sss_mbedtls_key_store_t * ) keyStore),((sss_mbedtls_object_t * ) keyObject),(keyBitLen),(options))
#       define sss_key_store_get_key(keyStore,keyObject,key,pKeyBitLen) \
            sss_mbedtls_key_store_get_key(((sss_mbedtls_key_store_t * ) keyStore),((sss_mbedtls_object_t * ) keyObject),(key),(pKeyBitLen))
#       define sss_key_store_open_key(keyStore,keyObject) \
            sss_mbedtls_key_store_open_key(((sss_mbedtls_key_store_t * ) keyStore),((sss_mbedtls_object_t * ) keyObject))
#       define sss_key_store_freeze_key(keyStore,keyObject) \
            sss_mbedtls_key_store_freeze_key(((sss_mbedtls_key_store_t * ) keyStore),((sss_mbedtls_object_t * ) keyObject))
#       define sss_key_store_erase_key(keyStore,keyObject) \
            sss_mbedtls_key_store_erase_key(((sss_mbedtls_key_store_t * ) keyStore),((sss_mbedtls_object_t * ) keyObject))
#       define sss_key_store_context_free(keyStore) \
            sss_mbedtls_key_store_context_free(((sss_mbedtls_key_store_t * ) keyStore))
        /* Direct Call : asym */
#       define sss_asymmetric_context_init(context,session,keyObject,algorithm,mode) \
            sss_mbedtls_asymmetric_context_init(((sss_mbedtls_asymmetric_t * ) context),((sss_mbedtls_session_t * ) session),((sss_mbedtls_object_t * ) keyObject),(algorithm),(mode))
#       define sss_asymmetric_encrypt(context,srcData,srcLen,destData,destLen) \
            sss_mbedtls_asymmetric_encrypt(((sss_mbedtls_asymmetric_t * ) context),(srcData),(srcLen),(destData),(destLen))
#       define sss_asymmetric_decrypt(context,srcData,srcLen,destData,destLen) \
            sss_mbedtls_asymmetric_decrypt(((sss_mbedtls_asymmetric_t * ) context),(srcData),(srcLen),(destData),(destLen))
#       define sss_asymmetric_sign_digest(context,digest,digestLen,signature,signatureLen) \
            sss_mbedtls_asymmetric_sign_digest(((sss_mbedtls_asymmetric_t * ) context),(digest),(digestLen),(signature),(signatureLen))
#       define sss_asymmetric_verify_digest(context,digest,digestLen,signature,signatureLen) \
            sss_mbedtls_asymmetric_verify_digest(((sss_mbedtls_asymmetric_t * ) context),(digest),(digestLen),(signature),(signatureLen))
#       define sss_asymmetric_context_free(context) \
            sss_mbedtls_asymmetric_context_free(((sss_mbedtls_asymmetric_t * ) context))
        /* Direct Call : symm */
#       define sss_symmetric_context_init(context,session,keyObject,algorithm,mode) \
            sss_mbedtls_symmetric_context_init(((sss_mbedtls_symmetric_t * ) context),((sss_mbedtls_session_t * ) session),((sss_mbedtls_object_t * ) keyObject),(algorithm),(mode))
#       define sss_cipher_one_go(context,iv,ivLen,srcData,destData,dataLen) \
            sss_mbedtls_cipher_one_go(((sss_mbedtls_symmetric_t * ) context),(iv),(ivLen),(srcData),(destData),(dataLen))
#       define sss_cipher_init(context,iv,ivLen) \
            sss_mbedtls_cipher_init(((sss_mbedtls_symmetric_t * ) context),(iv),(ivLen))
#       define sss_cipher_update(context,srcData,srcLen,destData,destLen) \
            sss_mbedtls_cipher_update(((sss_mbedtls_symmetric_t * ) context),(srcData),(srcLen),(destData),(destLen))
#       define sss_cipher_finish(context,srcData,srcLen,destData,destLen) \
            sss_mbedtls_cipher_finish(((sss_mbedtls_symmetric_t * ) context),(srcData),(srcLen),(destData),(destLen))
#       define sss_cipher_crypt_ctr(context,srcData,destData,size,initialCounter,lastEncryptedCounter,szLeft) \
            sss_mbedtls_cipher_crypt_ctr(((sss_mbedtls_symmetric_t * ) context),(srcData),(destData),(size),(initialCounter),(lastEncryptedCounter),(szLeft))
#       define sss_symmetric_context_free(context) \
            sss_mbedtls_symmetric_context_free(((sss_mbedtls_symmetric_t * ) context))
#   endif /* (SSS_HAVE_SSS == 1) */
/* clang-format on */
#endif /* SSS_HAVE_MBEDTLS */
#endif /* FSL_SSS_MBEDTLS_APIS_H */
