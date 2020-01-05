/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <fsl_sss_api.h>
#if !defined(SSS_FTR_FILE)
#include "fsl_sss_ftr_default.h"
#else
#include SSS_FTR_FILE
#endif

#if SSS_HAVE_SSCP
#include <fsl_sss_sscp_apis.h>
#endif /* SSS_HAVE_SSCP */

#if SSS_HAVE_MBEDTLS
#include <fsl_sss_mbedtls_apis.h>
#endif /* SSS_HAVE_MBEDTLS */

#if SSS_HAVE_OPENSSL
#include <fsl_sss_openssl_apis.h>
#endif /* SSS_HAVE_OPENSSL */

#if (SSS_HAVE_SSS > 1)

sss_status_t sss_open_session(sss_session_t *session,
                              sss_type_t subsystem,
                              uint32_t additionalApplicationId,
                              uint32_t connectionMethod,
                              const void *connectionData)
{
#if SSS_HAVE_SSCP
    if (SSS_SUBSYSTEM_TYPE_IS_SSCP(subsystem))
    {
        sss_sscp_session_t *sscp_session = (sss_sscp_session_t *)session;
        return sss_sscp_open_session(sscp_session, subsystem, additionalApplicationId, connectionMethod,
                                     connectionData);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SUBSYSTEM_TYPE_IS_MBEDTLS(subsystem))
    {
        sss_mbedtls_session_t *mbedtls_session = (sss_mbedtls_session_t *)session;
        return sss_mbedtls_open_session(mbedtls_session, subsystem, additionalApplicationId, connectionMethod,
                                        connectionData);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SUBSYSTEM_TYPE_IS_OPENSSL(subsystem))
    {
        sss_openssl_session_t *openssl_session = (sss_openssl_session_t *)session;
        return sss_openssl_open_session(openssl_session, subsystem, additionalApplicationId, connectionMethod,
                                        connectionData);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

void sss_close_session(sss_session_t *session)
{
#if SSS_HAVE_SSCP
    if (SSS_SESSION_TYPE_IS_SSCP(session))
    {
        sss_sscp_session_t *sscp_session = (sss_sscp_session_t *)session;
        sss_sscp_close_session(sscp_session);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SESSION_TYPE_IS_MBEDTLS(session))
    {
        sss_mbedtls_session_t *mbedtls_session = (sss_mbedtls_session_t *)session;
        sss_mbedtls_close_session(mbedtls_session);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SESSION_TYPE_IS_OPENSSL(session))
    {
        sss_openssl_session_t *openssl_session = (sss_openssl_session_t *)session;
        sss_openssl_close_session(openssl_session);
    }
#endif /* SSS_HAVE_OPENSSL */
}

sss_status_t sss_key_object_init(sss_object_t *keyObject, sss_key_store_t *keyStore)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        return sss_sscp_key_object_init(sscp_keyObject, sscp_keyStore);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        return sss_mbedtls_key_object_init(mbedtls_keyObject, mbedtls_keyStore);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        return sss_openssl_key_object_init(openssl_keyObject, openssl_keyStore);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_allocate_handle(
    sss_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t options)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_allocate_handle(sscp_keyObject, keyId, keyType, keyByteLenMax, options);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_allocate_handle(mbedtls_keyObject, keyId, keyType, keyByteLenMax, options);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_allocate_handle(openssl_keyObject, keyId, keyType, keyByteLenMax, options);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_get_handle(sss_object_t *keyObject, uint32_t keyId)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_get_handle(sscp_keyObject, keyId);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_get_handle(mbedtls_keyObject, keyId);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_get_handle(openssl_keyObject, keyId);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_set_user(sss_object_t *keyObject, uint32_t user, uint32_t options)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_set_user(sscp_keyObject, user, options);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_set_user(mbedtls_keyObject, user, options);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_set_user(openssl_keyObject, user, options);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_set_purpose(sss_object_t *keyObject, sss_mode_t purpose, uint32_t options)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_set_purpose(sscp_keyObject, purpose, options);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_set_purpose(mbedtls_keyObject, purpose, options);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_set_purpose(openssl_keyObject, purpose, options);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_set_access(sss_object_t *keyObject, uint32_t access, uint32_t options)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_set_access(sscp_keyObject, access, options);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_set_access(mbedtls_keyObject, access, options);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_set_access(openssl_keyObject, access, options);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_set_eccgfp_group(sss_object_t *keyObject, sss_eccgfp_group_t *group)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_set_eccgfp_group(sscp_keyObject, group);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_set_eccgfp_group(mbedtls_keyObject, group);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_set_eccgfp_group(openssl_keyObject, group);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_get_user(sss_object_t *keyObject, uint32_t *user)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_get_user(sscp_keyObject, user);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_get_user(mbedtls_keyObject, user);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_get_user(openssl_keyObject, user);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_get_purpose(sss_object_t *keyObject, sss_mode_t *purpose)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_get_purpose(sscp_keyObject, purpose);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_get_purpose(mbedtls_keyObject, purpose);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_get_purpose(openssl_keyObject, purpose);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_object_get_access(sss_object_t *keyObject, uint32_t *access)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_object_get_access(sscp_keyObject, access);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_object_get_access(mbedtls_keyObject, access);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_object_get_access(openssl_keyObject, access);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

void sss_key_object_free(sss_object_t *keyObject)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        sss_sscp_key_object_free(sscp_keyObject);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        sss_mbedtls_key_object_free(mbedtls_keyObject);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        sss_openssl_key_object_free(openssl_keyObject);
    }
#endif /* SSS_HAVE_OPENSSL */
}

sss_status_t sss_derive_key_context_init(sss_derive_key_t *context,
                                         sss_session_t *session,
                                         sss_object_t *keyObject,
                                         sss_algorithm_t algorithm,
                                         sss_mode_t mode)
{
#if SSS_HAVE_SSCP
    if (SSS_SESSION_TYPE_IS_SSCP(session))
    {
        sss_sscp_derive_key_t *sscp_context = (sss_sscp_derive_key_t *)context;
        sss_sscp_session_t *sscp_session = (sss_sscp_session_t *)session;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_derive_key_context_init(sscp_context, sscp_session, sscp_keyObject, algorithm, mode);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SESSION_TYPE_IS_MBEDTLS(session))
    {
        sss_mbedtls_derive_key_t *mbedtls_context = (sss_mbedtls_derive_key_t *)context;
        sss_mbedtls_session_t *mbedtls_session = (sss_mbedtls_session_t *)session;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_derive_key_context_init(mbedtls_context, mbedtls_session, mbedtls_keyObject, algorithm,
                                                   mode);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SESSION_TYPE_IS_OPENSSL(session))
    {
        sss_openssl_derive_key_t *openssl_context = (sss_openssl_derive_key_t *)context;
        sss_openssl_session_t *openssl_session = (sss_openssl_session_t *)session;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_derive_key_context_init(openssl_context, openssl_session, openssl_keyObject, algorithm,
                                                   mode);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_derive_key(sss_derive_key_t *context,
                            const uint8_t *saltData,
                            size_t saltLen,
                            const uint8_t *info,
                            size_t infoLen,
                            sss_object_t *derivedKeyObject)
{
#if SSS_HAVE_SSCP
    if (SSS_DERIVE_KEY_TYPE_IS_SSCP(context))
    {
        sss_sscp_derive_key_t *sscp_context = (sss_sscp_derive_key_t *)context;
        sss_sscp_object_t *sscp_derivedKeyObject = (sss_sscp_object_t *)derivedKeyObject;
        return sss_sscp_derive_key(sscp_context, saltData, saltLen, info, infoLen, sscp_derivedKeyObject);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_DERIVE_KEY_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_derive_key_t *mbedtls_context = (sss_mbedtls_derive_key_t *)context;
        sss_mbedtls_object_t *mbedtls_derivedKeyObject = (sss_mbedtls_object_t *)derivedKeyObject;
        return sss_mbedtls_derive_key(mbedtls_context, saltData, saltLen, info, infoLen, mbedtls_derivedKeyObject);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_DERIVE_KEY_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_derive_key_t *openssl_context = (sss_openssl_derive_key_t *)context;
        sss_openssl_object_t *openssl_derivedKeyObject = (sss_openssl_object_t *)derivedKeyObject;
        return sss_openssl_derive_key(openssl_context, saltData, saltLen, info, infoLen, openssl_derivedKeyObject);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_asymmetric_dh_derive_key(sss_derive_key_t *context,
                                          sss_object_t *otherPartyKeyObject,
                                          sss_object_t *derivedKeyObject)
{
#if SSS_HAVE_SSCP
    if (SSS_DERIVE_KEY_TYPE_IS_SSCP(context))
    {
        sss_sscp_derive_key_t *sscp_context = (sss_sscp_derive_key_t *)context;
        sss_sscp_object_t *sscp_otherPartyKeyObject = (sss_sscp_object_t *)otherPartyKeyObject;
        sss_sscp_object_t *sscp_derivedKeyObject = (sss_sscp_object_t *)derivedKeyObject;
        return sss_sscp_asymmetric_dh_derive_key(sscp_context, sscp_otherPartyKeyObject, sscp_derivedKeyObject);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_DERIVE_KEY_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_derive_key_t *mbedtls_context = (sss_mbedtls_derive_key_t *)context;
        sss_mbedtls_object_t *mbedtls_otherPartyKeyObject = (sss_mbedtls_object_t *)otherPartyKeyObject;
        sss_mbedtls_object_t *mbedtls_derivedKeyObject = (sss_mbedtls_object_t *)derivedKeyObject;
        return sss_mbedtls_asymmetric_dh_derive_key(mbedtls_context, mbedtls_otherPartyKeyObject,
                                                    mbedtls_derivedKeyObject);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_DERIVE_KEY_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_derive_key_t *openssl_context = (sss_openssl_derive_key_t *)context;
        sss_openssl_object_t *openssl_otherPartyKeyObject = (sss_openssl_object_t *)otherPartyKeyObject;
        sss_openssl_object_t *openssl_derivedKeyObject = (sss_openssl_object_t *)derivedKeyObject;
        return sss_openssl_asymmetric_dh_derive_key(openssl_context, openssl_otherPartyKeyObject,
                                                    openssl_derivedKeyObject);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

void sss_derive_key_context_free(sss_derive_key_t *context)
{
#if SSS_HAVE_SSCP
    if (SSS_DERIVE_KEY_TYPE_IS_SSCP(context))
    {
        sss_sscp_derive_key_t *sscp_context = (sss_sscp_derive_key_t *)context;
        sss_sscp_derive_key_context_free(sscp_context);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_DERIVE_KEY_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_derive_key_t *mbedtls_context = (sss_mbedtls_derive_key_t *)context;
        sss_mbedtls_derive_key_context_free(mbedtls_context);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_DERIVE_KEY_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_derive_key_t *openssl_context = (sss_openssl_derive_key_t *)context;
        sss_openssl_derive_key_context_free(openssl_context);
    }
#endif /* SSS_HAVE_OPENSSL */
}

sss_status_t sss_key_store_context_init(sss_key_store_t *keyStore, sss_session_t *session)
{
#if SSS_HAVE_SSCP
    if (SSS_SESSION_TYPE_IS_SSCP(session))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        sss_sscp_session_t *sscp_session = (sss_sscp_session_t *)session;
        return sss_sscp_key_store_context_init(sscp_keyStore, sscp_session);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SESSION_TYPE_IS_MBEDTLS(session))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        sss_mbedtls_session_t *mbedtls_session = (sss_mbedtls_session_t *)session;
        return sss_mbedtls_key_store_context_init(mbedtls_keyStore, mbedtls_session);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SESSION_TYPE_IS_OPENSSL(session))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        sss_openssl_session_t *openssl_session = (sss_openssl_session_t *)session;
        return sss_openssl_key_store_context_init(openssl_keyStore, openssl_session);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_allocate(sss_key_store_t *keyStore, uint32_t keyStoreId)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        return sss_sscp_key_store_allocate(sscp_keyStore, keyStoreId);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        return sss_mbedtls_key_store_allocate(mbedtls_keyStore, keyStoreId);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        return sss_openssl_key_store_allocate(openssl_keyStore, keyStoreId);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_save(sss_key_store_t *keyStore)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        return sss_sscp_key_store_save(sscp_keyStore);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        return sss_mbedtls_key_store_save(mbedtls_keyStore);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        return sss_openssl_key_store_save(openssl_keyStore);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_load(sss_key_store_t *keyStore)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        return sss_sscp_key_store_load(sscp_keyStore);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        return sss_mbedtls_key_store_load(mbedtls_keyStore);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        return sss_openssl_key_store_load(openssl_keyStore);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_set_key(sss_key_store_t *keyStore,
                                   sss_object_t *keyObject,
                                   const uint8_t *key,
                                   size_t keyBitLen,
                                   void *options,
                                   size_t optionsLen)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_store_set_key(sscp_keyStore, sscp_keyObject, key, keyBitLen, options, optionsLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_store_set_key(mbedtls_keyStore, mbedtls_keyObject, key, keyBitLen, options, optionsLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_store_set_key(openssl_keyStore, openssl_keyObject, key, keyBitLen, options, optionsLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_generate_key(sss_key_store_t *keyStore,
                                        sss_object_t *keyObject,
                                        size_t keyBitLen,
                                        void *options)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_store_generate_key(sscp_keyStore, sscp_keyObject, keyBitLen, options);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_store_generate_key(mbedtls_keyStore, mbedtls_keyObject, keyBitLen, options);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_store_generate_key(openssl_keyStore, openssl_keyObject, keyBitLen, options);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_get_key(sss_key_store_t *keyStore, sss_object_t *keyObject, uint8_t *key, size_t *pKeyBitLen)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_store_get_key(sscp_keyStore, sscp_keyObject, key, pKeyBitLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_store_get_key(mbedtls_keyStore, mbedtls_keyObject, key, pKeyBitLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_store_get_key(openssl_keyStore, openssl_keyObject, key, pKeyBitLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_open_key(sss_key_store_t *keyStore, sss_object_t *keyObject)
{
#if SSS_HAVE_SSCP
    if (SSS_OBJECT_TYPE_IS_SSCP(keyObject))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_store_open_key(sscp_keyStore, sscp_keyObject);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_OBJECT_TYPE_IS_MBEDTLS(keyObject))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_store_open_key(mbedtls_keyStore, mbedtls_keyObject);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_OBJECT_TYPE_IS_OPENSSL(keyObject))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_store_open_key(openssl_keyStore, openssl_keyObject);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_freeze_key(sss_key_store_t *keyStore, sss_object_t *keyObject)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_store_freeze_key(sscp_keyStore, sscp_keyObject);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_store_freeze_key(mbedtls_keyStore, mbedtls_keyObject);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_store_freeze_key(openssl_keyStore, openssl_keyObject);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_key_store_erase_key(sss_key_store_t *keyStore, sss_object_t *keyObject)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_key_store_erase_key(sscp_keyStore, sscp_keyObject);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_key_store_erase_key(mbedtls_keyStore, mbedtls_keyObject);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_key_store_erase_key(openssl_keyStore, openssl_keyObject);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

void sss_key_store_context_free(sss_key_store_t *keyStore)
{
#if SSS_HAVE_SSCP
    if (SSS_KEY_STORE_TYPE_IS_SSCP(keyStore))
    {
        sss_sscp_key_store_t *sscp_keyStore = (sss_sscp_key_store_t *)keyStore;
        sss_sscp_key_store_context_free(sscp_keyStore);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_KEY_STORE_TYPE_IS_MBEDTLS(keyStore))
    {
        sss_mbedtls_key_store_t *mbedtls_keyStore = (sss_mbedtls_key_store_t *)keyStore;
        sss_mbedtls_key_store_context_free(mbedtls_keyStore);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore))
    {
        sss_openssl_key_store_t *openssl_keyStore = (sss_openssl_key_store_t *)keyStore;
        sss_openssl_key_store_context_free(openssl_keyStore);
    }
#endif /* SSS_HAVE_OPENSSL */
}

sss_status_t sss_asymmetric_context_init(sss_asymmetric_t *context,
                                         sss_session_t *session,
                                         sss_object_t *keyObject,
                                         sss_algorithm_t algorithm,
                                         sss_mode_t mode)
{
#if SSS_HAVE_SSCP
    if (SSS_SESSION_TYPE_IS_SSCP(session))
    {
        sss_sscp_asymmetric_t *sscp_context = (sss_sscp_asymmetric_t *)context;
        sss_sscp_session_t *sscp_session = (sss_sscp_session_t *)session;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_asymmetric_context_init(sscp_context, sscp_session, sscp_keyObject, algorithm, mode);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SESSION_TYPE_IS_MBEDTLS(session))
    {
        sss_mbedtls_asymmetric_t *mbedtls_context = (sss_mbedtls_asymmetric_t *)context;
        sss_mbedtls_session_t *mbedtls_session = (sss_mbedtls_session_t *)session;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_asymmetric_context_init(mbedtls_context, mbedtls_session, mbedtls_keyObject, algorithm,
                                                   mode);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SESSION_TYPE_IS_OPENSSL(session))
    {
        sss_openssl_asymmetric_t *openssl_context = (sss_openssl_asymmetric_t *)context;
        sss_openssl_session_t *openssl_session = (sss_openssl_session_t *)session;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_asymmetric_context_init(openssl_context, openssl_session, openssl_keyObject, algorithm,
                                                   mode);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_asymmetric_encrypt(
    sss_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
#if SSS_HAVE_SSCP
    if (SSS_ASYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_asymmetric_t *sscp_context = (sss_sscp_asymmetric_t *)context;
        return sss_sscp_asymmetric_encrypt(sscp_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_ASYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_asymmetric_t *mbedtls_context = (sss_mbedtls_asymmetric_t *)context;
        return sss_mbedtls_asymmetric_encrypt(mbedtls_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_ASYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_asymmetric_t *openssl_context = (sss_openssl_asymmetric_t *)context;
        return sss_openssl_asymmetric_encrypt(openssl_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_asymmetric_decrypt(
    sss_asymmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
#if SSS_HAVE_SSCP
    if (SSS_ASYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_asymmetric_t *sscp_context = (sss_sscp_asymmetric_t *)context;
        return sss_sscp_asymmetric_decrypt(sscp_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_ASYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_asymmetric_t *mbedtls_context = (sss_mbedtls_asymmetric_t *)context;
        return sss_mbedtls_asymmetric_decrypt(mbedtls_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_ASYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_asymmetric_t *openssl_context = (sss_openssl_asymmetric_t *)context;
        return sss_openssl_asymmetric_decrypt(openssl_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_asymmetric_sign_digest(
    sss_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t *signatureLen)
{
#if SSS_HAVE_SSCP
    if (SSS_ASYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_asymmetric_t *sscp_context = (sss_sscp_asymmetric_t *)context;
        return sss_sscp_asymmetric_sign_digest(sscp_context, digest, digestLen, signature, signatureLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_ASYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_asymmetric_t *mbedtls_context = (sss_mbedtls_asymmetric_t *)context;
        return sss_mbedtls_asymmetric_sign_digest(mbedtls_context, digest, digestLen, signature, signatureLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_ASYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_asymmetric_t *openssl_context = (sss_openssl_asymmetric_t *)context;
        return sss_openssl_asymmetric_sign_digest(openssl_context, digest, digestLen, signature, signatureLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_asymmetric_verify_digest(
    sss_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t signatureLen)
{
#if SSS_HAVE_SSCP
    if (SSS_ASYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_asymmetric_t *sscp_context = (sss_sscp_asymmetric_t *)context;
        return sss_sscp_asymmetric_verify_digest(sscp_context, digest, digestLen, signature, signatureLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_ASYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_asymmetric_t *mbedtls_context = (sss_mbedtls_asymmetric_t *)context;
        return sss_mbedtls_asymmetric_verify_digest(mbedtls_context, digest, digestLen, signature, signatureLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_ASYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_asymmetric_t *openssl_context = (sss_openssl_asymmetric_t *)context;
        return sss_openssl_asymmetric_verify_digest(openssl_context, digest, digestLen, signature, signatureLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

void sss_asymmetric_context_free(sss_asymmetric_t *context)
{
#if SSS_HAVE_SSCP
    if (SSS_ASYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_asymmetric_t *sscp_context = (sss_sscp_asymmetric_t *)context;
        sss_sscp_asymmetric_context_free(sscp_context);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_ASYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_asymmetric_t *mbedtls_context = (sss_mbedtls_asymmetric_t *)context;
        sss_mbedtls_asymmetric_context_free(mbedtls_context);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_ASYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_asymmetric_t *openssl_context = (sss_openssl_asymmetric_t *)context;
        sss_openssl_asymmetric_context_free(openssl_context);
    }
#endif /* SSS_HAVE_OPENSSL */
}

sss_status_t sss_symmetric_context_init(sss_symmetric_t *context,
                                        sss_session_t *session,
                                        sss_object_t *keyObject,
                                        sss_algorithm_t algorithm,
                                        sss_mode_t mode)
{
#if SSS_HAVE_SSCP
    if (SSS_SESSION_TYPE_IS_SSCP(session))
    {
        sss_sscp_symmetric_t *sscp_context = (sss_sscp_symmetric_t *)context;
        sss_sscp_session_t *sscp_session = (sss_sscp_session_t *)session;
        sss_sscp_object_t *sscp_keyObject = (sss_sscp_object_t *)keyObject;
        return sss_sscp_symmetric_context_init(sscp_context, sscp_session, sscp_keyObject, algorithm, mode);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SESSION_TYPE_IS_MBEDTLS(session))
    {
        sss_mbedtls_symmetric_t *mbedtls_context = (sss_mbedtls_symmetric_t *)context;
        sss_mbedtls_session_t *mbedtls_session = (sss_mbedtls_session_t *)session;
        sss_mbedtls_object_t *mbedtls_keyObject = (sss_mbedtls_object_t *)keyObject;
        return sss_mbedtls_symmetric_context_init(mbedtls_context, mbedtls_session, mbedtls_keyObject, algorithm, mode);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SESSION_TYPE_IS_OPENSSL(session))
    {
        sss_openssl_symmetric_t *openssl_context = (sss_openssl_symmetric_t *)context;
        sss_openssl_session_t *openssl_session = (sss_openssl_session_t *)session;
        sss_openssl_object_t *openssl_keyObject = (sss_openssl_object_t *)keyObject;
        return sss_openssl_symmetric_context_init(openssl_context, openssl_session, openssl_keyObject, algorithm, mode);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_cipher_one_go(
    sss_symmetric_t *context, uint8_t *iv, size_t ivLen, const uint8_t *srcData, uint8_t *destData, size_t dataLen)
{
#if SSS_HAVE_SSCP
    if (SSS_SYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_symmetric_t *sscp_context = (sss_sscp_symmetric_t *)context;
        return sss_sscp_cipher_one_go(sscp_context, iv, ivLen, srcData, destData, dataLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_symmetric_t *mbedtls_context = (sss_mbedtls_symmetric_t *)context;
        return sss_mbedtls_cipher_one_go(mbedtls_context, iv, ivLen, srcData, destData, dataLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_symmetric_t *openssl_context = (sss_openssl_symmetric_t *)context;
        return sss_openssl_cipher_one_go(openssl_context, iv, ivLen, srcData, destData, dataLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_cipher_init(sss_symmetric_t *context, uint8_t *iv, size_t ivLen)
{
#if SSS_HAVE_SSCP
    if (SSS_SYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_symmetric_t *sscp_context = (sss_sscp_symmetric_t *)context;
        return sss_sscp_cipher_init(sscp_context, iv, ivLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_symmetric_t *mbedtls_context = (sss_mbedtls_symmetric_t *)context;
        return sss_mbedtls_cipher_init(mbedtls_context, iv, ivLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_symmetric_t *openssl_context = (sss_openssl_symmetric_t *)context;
        return sss_openssl_cipher_init(openssl_context, iv, ivLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_cipher_update(
    sss_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
#if SSS_HAVE_SSCP
    if (SSS_SYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_symmetric_t *sscp_context = (sss_sscp_symmetric_t *)context;
        return sss_sscp_cipher_update(sscp_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_symmetric_t *mbedtls_context = (sss_mbedtls_symmetric_t *)context;
        return sss_mbedtls_cipher_update(mbedtls_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_symmetric_t *openssl_context = (sss_openssl_symmetric_t *)context;
        return sss_openssl_cipher_update(openssl_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_cipher_finish(
    sss_symmetric_t *context, const uint8_t *srcData, size_t srcLen, uint8_t *destData, size_t *destLen)
{
#if SSS_HAVE_SSCP
    if (SSS_SYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_symmetric_t *sscp_context = (sss_sscp_symmetric_t *)context;
        return sss_sscp_cipher_finish(sscp_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_symmetric_t *mbedtls_context = (sss_mbedtls_symmetric_t *)context;
        return sss_mbedtls_cipher_finish(mbedtls_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_symmetric_t *openssl_context = (sss_openssl_symmetric_t *)context;
        return sss_openssl_cipher_finish(openssl_context, srcData, srcLen, destData, destLen);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

sss_status_t sss_cipher_crypt_ctr(sss_symmetric_t *context,
                                  const uint8_t *srcData,
                                  uint8_t *destData,
                                  size_t size,
                                  uint8_t *initialCounter,
                                  uint8_t *lastEncryptedCounter,
                                  size_t *szLeft)
{
#if SSS_HAVE_SSCP
    if (SSS_SYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_symmetric_t *sscp_context = (sss_sscp_symmetric_t *)context;
        return sss_sscp_cipher_crypt_ctr(sscp_context, srcData, destData, size, initialCounter, lastEncryptedCounter,
                                         szLeft);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_symmetric_t *mbedtls_context = (sss_mbedtls_symmetric_t *)context;
        return sss_mbedtls_cipher_crypt_ctr(mbedtls_context, srcData, destData, size, initialCounter,
                                            lastEncryptedCounter, szLeft);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_symmetric_t *openssl_context = (sss_openssl_symmetric_t *)context;
        return sss_openssl_cipher_crypt_ctr(openssl_context, srcData, destData, size, initialCounter,
                                            lastEncryptedCounter, szLeft);
    }
#endif /* SSS_HAVE_OPENSSL */
    return kStatus_SSS_InvalidArgument;
}

void sss_symmetric_context_free(sss_symmetric_t *context)
{
#if SSS_HAVE_SSCP
    if (SSS_SYMMETRIC_TYPE_IS_SSCP(context))
    {
        sss_sscp_symmetric_t *sscp_context = (sss_sscp_symmetric_t *)context;
        sss_sscp_symmetric_context_free(sscp_context);
    }
#endif /* SSS_HAVE_SSCP */
#if SSS_HAVE_MBEDTLS
    if (SSS_SYMMETRIC_TYPE_IS_MBEDTLS(context))
    {
        sss_mbedtls_symmetric_t *mbedtls_context = (sss_mbedtls_symmetric_t *)context;
        sss_mbedtls_symmetric_context_free(mbedtls_context);
    }
#endif /* SSS_HAVE_MBEDTLS */
#if SSS_HAVE_OPENSSL
    if (SSS_SYMMETRIC_TYPE_IS_OPENSSL(context))
    {
        sss_openssl_symmetric_t *openssl_context = (sss_openssl_symmetric_t *)context;
        sss_openssl_symmetric_context_free(openssl_context);
    }
#endif /* SSS_HAVE_OPENSSL */
}

#endif /* SSS_HAVE_SSS > 1 */
