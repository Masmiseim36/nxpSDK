/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_SSS_SSCP_H_
#define _FSL_SSS_SSCP_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "fsl_sscp.h"
#include "fsl_sss_api.h"

#if !defined(SSS_SSCP_CONFIG_FILE)
#include "fsl_sss_sscp_config.h"
#else
#include SSS_SSCP_CONFIG_FILE
#endif

typedef struct _sss_sscp_session
{
    /*! Indicates which security subsystem is selected to be used. */
    sss_type_t subsystem;

    /*! Implementation specific part */
    sscp_context_t *sscp;
} sss_sscp_session_t;

typedef struct _sss_sscp_key_store
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_sscp_session_t *session;
    /*! Implementation specific part */
    uint32_t keyStoreId;
} sss_sscp_key_store_t;

typedef struct _sss_sscp_object
{
    /*! key store holding the data and other properties */
    sss_sscp_key_store_t *keyStore;

    uint32_t objectType; /*! TODO define object types */
    /*! Application specific key identifier. The keyId is kept in the key store along with the key data and other
     * properties. */
    uint32_t keyId;
    /*! Implementation specific part */
} sss_sscp_object_t;

/*! @brief ::_sss_symmetric with SSCP specific information */
typedef struct _sss_sscp_symmetric
{
    /*! Virtual connection between application (user context) and
                specific security subsystem  and function thereof. */
    sss_sscp_session_t *session;
    sss_sscp_object_t *keyObject; /*! Reference to key and it's properties. */
    sss_algorithm_t algorithm;    /*!  */
    sss_mode_t mode;              /*!  */

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_SSCP_SYMMETRIC_CONTEXT_SIZE];
    } context;
} sss_sscp_symmetric_t;

typedef struct _sss_sscp_aead
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_sscp_session_t *session;
    sss_sscp_object_t *keyObject; /*! Reference to key and it's properties. */
    sss_algorithm_t algorithm;    /*!  */
    sss_mode_t mode;              /*!  */

    /*! Implementation specific part */
} sss_sscp_aead_t;

typedef struct _sss_sscp_digest
{
    /*! Virtual connection between application (user context) and specific security subsystem and function thereof. */
    sss_sscp_session_t *session;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */
    /*! Full digest length per algorithm definition. This field is initialized along with algorithm. */
    size_t digestFullLen;

    /*! Implementation specific part */
    struct
    {
        uint8_t data[SSS_SSCP_DIGEST_CONTEXT_SIZE];
    } context;
} sss_sscp_digest_t;

typedef struct _sss_sscp_mac
{
    /*! Virtual connection between application (user context) and specific
     * security subsystem and function thereof. */
    sss_sscp_session_t *session;
    sss_sscp_object_t *keyObject; /*! Reference to key and it's properties. */
    sss_algorithm_t algorithm;    /*!  */
    sss_mode_t mode;              /*!  */

    /*! Implementation specific part */
    uint32_t macFullLen;
    struct
    {
        uint8_t data[SSS_SSCP_MAC_CONTEXT_SIZE];
    } context;
} sss_sscp_mac_t;

typedef struct _sss_sscp_asymmetric
{
    sss_sscp_session_t *session;
    sss_sscp_object_t *keyObject;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */
    size_t signatureFullLen;

    /*! Implementation specific part */
} sss_sscp_asymmetric_t;

typedef struct _sss_sscp_tunnel
{
    sss_sscp_session_t *session;
    uint32_t tunnelType;

    /*! Implementation specific part */
} sss_sscp_tunnel_t;

typedef struct _sss_sscp_derive_key
{
    sss_sscp_session_t *session;
    sss_sscp_object_t *keyObject;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */

    /*! Implementation specific part */
} sss_sscp_derive_key_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

sss_status_t sss_sscp_cipher_one_go(sss_sscp_symmetric_t *context,
                                    uint8_t *iv,
                                    size_t ivLen,
                                    const uint8_t *srcData,
                                    uint8_t *destData,
                                    size_t dataLen);

sss_status_t sss_sscp_cipher_crypt_ctr(sss_sscp_symmetric_t *context,
                                       const uint8_t *srcData,
                                       uint8_t *destData,
                                       size_t size,
                                       uint8_t *initialCounter,
                                       uint8_t *lastEncryptedCounter,
                                       size_t *szLeft);

sss_status_t sss_sscp_aead_one_go(sss_sscp_aead_t *context,
                                  const uint8_t *srcData,
                                  uint8_t *destData,
                                  size_t size,
                                  uint8_t *nonce,
                                  size_t nonceLen,
                                  const uint8_t *aad,
                                  size_t aadLen,
                                  uint8_t *tag,
                                  size_t *tagLen);

sss_status_t sss_sscp_digest_one_go(
    sss_sscp_digest_t *context, const uint8_t *message, size_t messageLen, uint8_t *digest, size_t *digestLen);

sss_status_t sss_sscp_digest_init(sss_sscp_digest_t *context);

sss_status_t sss_sscp_digest_update(sss_sscp_digest_t *context, const uint8_t *message, size_t messageLen);

sss_status_t sss_sscp_digest_finish(sss_sscp_digest_t *context, uint8_t *digest, size_t *digestLen);

sss_status_t sss_sscp_mac_one_go(
    sss_sscp_mac_t *context, const uint8_t *message, size_t messageLen, uint8_t *mac, size_t *macLen);

sss_status_t sss_sscp_mac_init(sss_sscp_mac_t *context);

sss_status_t sss_sscp_mac_update(sss_sscp_mac_t *context, const uint8_t *message, size_t messageLen);

sss_status_t sss_sscp_mac_finish(sss_sscp_mac_t *context, uint8_t *mac, size_t *macLen);

sss_status_t sss_sscp_asymmetric_sign_digest(
    sss_sscp_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t *signatureLen);

sss_status_t sss_sscp_asymmetric_verify_digest(
    sss_sscp_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t signatureLen);

sss_status_t sss_sscp_tunnel(sss_sscp_tunnel_t *context,
                             uint8_t *data,
                             size_t dataLen,
                             sss_sscp_object_t *keyObjects,
                             uint32_t keyObjectCount,
                             uint32_t tunnelType);

sss_status_t sss_sscp_asymmetric_dh_derive_key(sss_sscp_derive_key_t *context,
                                               sss_sscp_object_t *otherPartyKeyObject,
                                               sss_sscp_object_t *derivedKeyObject);

sss_status_t sss_sscp_key_store_allocate(sss_sscp_key_store_t *keyStore, uint32_t keyStoreId);

sss_status_t sss_sscp_key_object_allocate_handle(
    sss_sscp_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, uint32_t keyByteLenMax, uint32_t options);

sss_status_t sss_sscp_key_store_set_key(sss_sscp_key_store_t *keyStore,
                                        sss_sscp_object_t *keyObject,
                                        const uint8_t *key,
                                        uint32_t keyBitLen,
                                        void *options,
                                        size_t optionsLen);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_SSS_SSCP_H_ */
