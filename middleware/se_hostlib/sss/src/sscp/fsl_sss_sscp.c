/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_SSCP
#include <fsl_sscp.h>
#include <fsl_sscp_commands.h>
#include <fsl_sss_sscp.h>
#include <nxEnsure.h>
#include <stdlib.h>
#include <string.h>

#include "nxLog_sscp.h"
#if SSS_HAVE_SE050_EAR_CH || SSS_HAVE_A71CH
#include "a71ch_api.h"
#include "fsl_sscp_a71ch.h"
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
#include "a71cl_api.h"
#include "fsl_sscp_a71cl.h"
#endif
/* Shorthand for SSCP_OP_SET_PARAM */
#define SSCP_PARAMS(p1, p2, p3, p4, p5, p6)                     \
    SSCP_OP_SET_PARAM(/* First param always ContextReference */ \
        kSSCP_ParamType_ContextReference,                       \
        kSSCP_ParamType_##p1,                                   \
        kSSCP_ParamType_##p2,                                   \
        kSSCP_ParamType_##p3,                                   \
        kSSCP_ParamType_##p4,                                   \
        kSSCP_ParamType_##p5,                                   \
        kSSCP_ParamType_##p6)

#define SET_OP_0_KeyObject()              \
    op.params[0].context.ptr = keyObject; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Object;

#define SET_OP_0_KeyStore()              \
    op.params[0].context.ptr = keyStore; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_KeyStore;

#define SET_OP_0_DeriveKey()            \
    op.params[0].context.ptr = context; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_DeriveKey;

#define SET_OP_0_Asymm()                \
    op.params[0].context.ptr = context; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Asymmetric;

#define SET_OP_0_Symm()                 \
    op.params[0].context.ptr = context; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Symmetric;

#define SET_OP_0_Tunnel()               \
    op.params[0].context.ptr = context; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Tunnel;

#define SET_OP_0_Mac()                  \
    op.params[0].context.ptr = context; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mac;

#define SET_OP_0_Digest()               \
    op.params[0].context.ptr = context; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Digest;

#define SET_OP_0_RandGen()              \
    op.params[0].context.ptr = context; \
    op.params[0].context.type = kSSCP_ParamContextType_SSS_RandomGen;

/* ************************************************************************** */
/* Functions : sss_sscp_session                                               */
/* ************************************************************************** */

sss_status_t sss_sscp_session_create(sss_sscp_session_t *session,
    sss_type_t subsystem,
    uint32_t application_id,
    sss_connection_type_t connetion_type,
    void *connectionData)
{
    sss_status_t retval = kStatus_SSS_Success;
    /* Nothing special to be handled */
    return retval;
}

sss_status_t sss_sscp_session_open(sss_sscp_session_t *session,
    sss_type_t subsystem,
    uint32_t application_id,
    sss_connection_type_t connetion_type,
    void *connectionData)
{
    uint32_t ret = kStatus_SSS_InvalidArgument;
    session->sessionId = 0;
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
    if (kType_SSS_SE_A71CH == subsystem) {
        ret = sscp_a71ch_openSession(connectionData, session);
    }
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
    if (kType_SSS_SE_A71CL == subsystem) {
        ret = sscp_a71cl_openSession(connectionData);
    }
#endif
    if (ret == kStatus_SSS_Success) {
        session->subsystem = subsystem;
        session->sscp_context = NULL;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_session_prop_get_u32(
    sss_sscp_session_t *session, uint32_t property, uint32_t *pValue)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_sscp_session_prop_get_au8(sss_sscp_session_t *session,
    uint32_t property,
    uint8_t *pValue,
    size_t *pValueLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_sscp_session_close(sss_sscp_session_t *session)
{
    session->subsystem = kType_SSS_SubSystem_NONE;
    if (session->sscp_context != NULL) {
        session->sscp_context->invoke = NULL;
    }
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
    sscp_a71ch_closeSession(session);
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
    sscp_a71cl_closeSession();
#endif
    memset(session, 0, sizeof(*session));
}
void sss_sscp_session_delete(sss_sscp_session_t *session)
{
    ;
}

/* End: sscp_session */

/* ************************************************************************** */
/* Functions : sss_sscp_keyobj                                                */
/* ************************************************************************** */

sss_status_t sss_sscp_key_object_init(
    sss_sscp_object_t *keyObject, sss_sscp_key_store_t *keyStore)
{
    uint32_t ret = kStatus_SSS_Fail;
    ENSURE_OR_GO_CLEANUP(keyStore);
    ENSURE_OR_GO_CLEANUP(keyObject);
    ENSURE_OR_GO_CLEANUP(keyStore->session);
    memset(keyObject, 0, sizeof(*keyObject));
    keyObject->keyStore = keyStore;
    ret = kStatus_SSS_Success;
cleanup:
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_allocate_handle(sss_sscp_object_t *keyObject,
    uint32_t keyId,
    sss_key_part_t keyPart,
    sss_cipher_type_t cipherType,
    size_t keyByteLenMax,
    uint32_t options)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sscp_operation_t op = {0};
    if (options == kKeyObject_Mode_Persistent) {
        op.paramTypes =
            SSCP_PARAMS(ValueInput, ValueInput, None, None, None, None);
        SET_OP_0_KeyObject();
        op.params[1].value.a = keyId;
        op.params[1].value.b = keyPart;
        op.params[2].value.a = (uint32_t)keyByteLenMax;
        op.params[2].value.b = options;
        op.params[3].value.a = cipherType;

        sscp_context_t *sscpCtx = keyObject->keyStore->session->sscp_context;
        resSSCP = sscpCtx->invoke(
            sscpCtx, kSSCP_KEYOBJ_CMD_ALLOCATE_HANDLE, &op, &ret);
        if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
            keyObject->keyId = keyId;
            keyObject->objectType = keyPart;
            keyObject->cipherType = cipherType;
            ret = kStatus_SSS_Success;
        }
        else {
            ret = kStatus_SSS_Fail;
        }
    }
    else if (options == kKeyObject_Mode_Transient) {
        ret = kStatus_SSS_Fail;
        keyObject->transientObject = malloc(keyByteLenMax);
        ENSURE_OR_GO_CLEANUP(keyObject->transientObject);
        keyObject->transientObjectLen = (uint32_t)keyByteLenMax;
        memset(keyObject->transientObject, 0, keyByteLenMax);
        ret = kStatus_SSS_Success;
    }
cleanup:
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_get_handle(
    sss_sscp_object_t *keyObject, uint32_t keyId)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSS_Fail;
    sscp_operation_t op = {0};

    op.paramTypes =
        SSCP_PARAMS(ValueInput, ValueOutput, None, None, None, None);
    SET_OP_0_KeyObject();
    op.params[1].value.a = keyId;

    sscp_context_t *sscpCtx = keyObject->keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_KEYOBJ_CMD_GET_HANDLE, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        keyObject->objectType = op.params[2].value.a;
        keyObject->cipherType = op.params[2].value.b;
        keyObject->keyId = keyId;
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_set_user(
    sss_sscp_object_t *keyObject, uint32_t user, uint32_t options)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(ValueInput, ValueInput, None, None, None, None);
    SET_OP_0_KeyObject();
    op.params[1].value.a = keyObject->keyId;
    op.params[2].value.a = user;
    op.params[2].value.b = options;

    sscp_context_t *ctx = keyObject->keyStore->session->sscp_context;

    resSSCP = ctx->invoke(ctx, kSSCP_KEYOBJ_CMD_SET_USER, &op, &ret);

    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_set_purpose(
    sss_sscp_object_t *keyObject, sss_mode_t purpose, uint32_t options)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(ValueInput, None, None, None, None, None);
    SET_OP_0_KeyObject();
    op.params[1].value.a = purpose;
    op.params[1].value.b = options;

    sscp_context_t *ctx = keyObject->keyStore->session->sscp_context;

    resSSCP = ctx->invoke(ctx, kSSCP_KEYOBJ_CMD_SET_PURPOSE, &op, &ret);

    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_set_access(
    sss_sscp_object_t *keyObject, uint32_t access, uint32_t options)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(ValueInput, None, None, None, None, None);
    SET_OP_0_KeyObject();
    op.params[1].value.a = access;
    op.params[1].value.b = options;

    sscp_context_t *ctx = keyObject->keyStore->session->sscp_context;

    resSSCP = ctx->invoke(ctx, kSSCP_KEYOBJ_CMD_SET_ACCESS, &op, &ret);
    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_set_eccgfp_group(
    sss_sscp_object_t *keyObject, sss_eccgfp_group_t *group)
{
    uint32_t ret = kStatus_SSS_Fail;
    /* TBU */
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_get_user(
    sss_sscp_object_t *keyObject, uint32_t *user)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(ValueOutput, None, None, None, None, None);
    SET_OP_0_KeyObject();

    sscp_context_t *ctx = keyObject->keyStore->session->sscp_context;

    resSSCP = ctx->invoke(ctx, kSSCP_KEYOBJ_CMD_GET_USER, &op, &ret);

    if (resSSCP == kStatus_SSCP_Success) {
        *user = op.params[0].value.a;
    }
    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_get_purpose(
    sss_sscp_object_t *keyObject, sss_mode_t *purpose)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(ValueOutput, None, None, None, None, None);
    SET_OP_0_KeyObject();

    sscp_context_t *ctx = keyObject->keyStore->session->sscp_context;

    resSSCP = ctx->invoke(ctx, kSSCP_KEYOBJ_CMD_GET_PURPOSE, &op, &ret);

    if (resSSCP == kStatus_SSCP_Success) {
        *purpose = op.params[0].value.a;
    }
    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_get_access(
    sss_sscp_object_t *keyObject, uint32_t *access)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(ValueOutput, None, None, None, None, None);
    SET_OP_0_KeyObject();

    sscp_context_t *ctx = keyObject->keyStore->session->sscp_context;

    resSSCP = ctx->invoke(ctx, kSSCP_KEYOBJ_CMD_GET_ACCESS, &op, &ret);

    if (resSSCP == kStatus_SSCP_Success) {
        *access = op.params[0].value.a;
    }
    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

void sss_sscp_key_object_free(sss_sscp_object_t *keyObject)
{
    if (keyObject->transientObject != NULL)
        free(keyObject->transientObject);
    memset(keyObject, 0, sizeof(*keyObject));
}

/* End: sscp_keyobj */

/* ************************************************************************** */
/* Functions : sss_sscp_keyderive                                             */
/* ************************************************************************** */

sss_status_t sss_sscp_derive_key_context_init(sss_sscp_derive_key_t *context,
    sss_sscp_session_t *session,
    sss_sscp_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSS_Fail;
    sscp_operation_t op = {0};
    ENSURE_OR_GO_CLEANUP(context);
    ENSURE_OR_GO_CLEANUP(keyObject);
    ENSURE_OR_GO_CLEANUP(
        keyObject->keyStore->session->subsystem == kType_SSS_SE_A71CH);
    memset(context, 0, sizeof(*context));
    context->algorithm = algorithm;
    context->mode = mode;
    context->session = session;
    context->keyObject = keyObject;

    op.paramTypes =
        SSCP_PARAMS(ValueInput, ValueInput, ValueOutput, None, None, None);
    SET_OP_0_DeriveKey();
    op.params[1].value.a = context->algorithm;
    op.params[1].value.b = context->mode;
    op.params[2].value.a = context->keyObject->keyId;

    sscp_context_t *sscpCtx =
        context->keyObject->keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_DERIVE_CTX_INIT, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        context->sessionId = op.params[3].value.a;
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
cleanup:
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_derive_key_go(sss_sscp_derive_key_t *context,
    const uint8_t *saltData,
    size_t saltLen,
    const uint8_t *info,
    size_t infoLen,
    sss_sscp_object_t *derivedKeyObject,
    uint16_t deriveDataLen,
    uint8_t *hkdfOutput,
    size_t *hkdfOutputLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sss_status_t retval = kStatus_SSS_Fail;
    sscp_operation_t op = {0};
    uint8_t derivedkey[32] = {0};
    size_t derivedkeyBitlen = sizeof(derivedkey);
    sss_object_t *sss_derived_keyObject = (sss_object_t *)derivedKeyObject;

    op.paramTypes = SSCP_PARAMS(
        ValueInput, MemrefInput, MemrefInput, MemrefOutput, None, None);
    SET_OP_0_DeriveKey();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = (void *)saltData;
    op.params[2].memref.size = saltLen;
    op.params[3].memref.buffer = (void *)info;
    op.params[3].memref.size = infoLen;
    op.params[4].memref.buffer = derivedkey;
    op.params[4].memref.size = sizeof(derivedkey);
    op.params[5].value.a = (uint32_t)derivedkeyBitlen;

    sscp_context_t *sscpCtx =
        context->keyObject->keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_DERIVE_KEY, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        derivedkeyBitlen = op.params[4].memref.size * 8;
        retval = sss_key_store_set_key(sss_derived_keyObject->keyStore,
            sss_derived_keyObject,
            derivedkey,
            sizeof(derivedkey),
            derivedkeyBitlen,
            NULL,
            0);
    }

    return retval;
}

sss_status_t sss_sscp_derive_key_dh(sss_sscp_derive_key_t *context,
    sss_sscp_object_t *otherPartyKeyObject,
    sss_sscp_object_t *derivedKeyObject)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sss_status_t retval;
    sscp_operation_t op = {0};
    uint8_t pubkey[128] = {0};
    size_t pubKeyByteLen = sizeof(pubkey);
    size_t keyBitlen = sizeof(pubkey) * 8;
    uint8_t derivedkey[32] = {0};
    size_t derivedkeyBitlen = sizeof(derivedkey) * 8;

    sss_object_t *sss_other_keyObject = (sss_object_t *)otherPartyKeyObject;
    sss_object_t *sss_derived_keyObject = (sss_object_t *)derivedKeyObject;

    retval = sss_key_store_get_key(sss_other_keyObject->keyStore,
        sss_other_keyObject,
        pubkey,
        &pubKeyByteLen,
        &keyBitlen);

    if (retval == kStatus_SSS_Success) {
        retval = kStatus_SSS_Fail;
        op.paramTypes = SSCP_PARAMS(ValueInput,
            ValueInput,
            MemrefInput,
            MemrefOutput,
            MemrefOutput,
            None);
        SET_OP_0_DeriveKey();
        op.params[1].value.a = context->sessionId;
        op.params[2].memref.buffer = (pubkey);
        op.params[2].memref.size = (pubKeyByteLen * 8);
        op.params[3].memref.buffer = derivedkey;
        op.params[3].memref.size = sizeof(derivedkey);
        op.params[4].memref.size = derivedkeyBitlen;

        sscp_context_t *sscpCtx =
            context->keyObject->keyStore->session->sscp_context;

        resSSCP =
            sscpCtx->invoke(sscpCtx, kSSCP_ASYMM_DH_DERIVE_KEY, &op, &ret);
        if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
            derivedkeyBitlen = op.params[4].memref.size;
            retval = sss_key_store_set_key(sss_derived_keyObject->keyStore,
                sss_derived_keyObject,
                derivedkey,
                sizeof(derivedkey),
                derivedkeyBitlen,
                NULL,
                0);
        }
    }
    return (retval);
}

sss_status_t sss_sscp_key_store_prop_get_u32(
    sss_sscp_key_store_t *session, uint32_t property, uint32_t *pValue)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_sscp_key_store_prop_get_au8(sss_sscp_key_store_t *session,
    uint32_t property,
    uint8_t *pValue,
    size_t *pValueLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_sscp_derive_key_context_free(sss_sscp_derive_key_t *context)
{
    memset(context, 0, sizeof(*context));
}

/* End: sscp_keyderive */

/* ************************************************************************** */
/* Functions : sss_sscp_keystore                                              */
/* ************************************************************************** */

sss_status_t sss_sscp_key_store_context_init(
    sss_sscp_key_store_t *keyStore, sss_sscp_session_t *session)
{
    uint32_t ret = kStatus_SSS_Fail;
    ENSURE_OR_GO_CLEANUP(keyStore);
    ENSURE_OR_GO_CLEANUP(session);
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
    if (session->subsystem == kType_SSS_SE_A71CH) {
        sss_a71ch_key_store_t *a71ch_keyStore =
            (sss_a71ch_key_store_t *)keyStore;
        memset(a71ch_keyStore, 0, sizeof(*a71ch_keyStore));
        keyStore->session = session;
        ret = sscp_a71ch_init(
            (sscp_a71ch_context_t *)&session->mem_sscp_ctx, a71ch_keyStore);
        if (ret == kStatus_SSS_Success) {
            session->sscp_context = &session->mem_sscp_ctx;
        }
    }
#endif
#if SSS_HAVE_A71CL || SSS_HAVE_SE050_L
    if (session->subsystem == kType_SSS_SE_A71CL) {
        sss_a71cl_key_store_t *a71cl_keyStore =
            (sss_a71cl_key_store_t *)keyStore;
        memset(a71cl_keyStore, 0, sizeof(*a71cl_keyStore));
        keyStore->session = session;
        ret = sscp_a71cl_init(
            (sscp_a71cl_context_t *)&session->mem_sscp_ctx, a71cl_keyStore);
        if (ret == kStatus_SSS_Success) {
            session->sscp_context = &session->mem_sscp_ctx;
        }
    }
#endif
cleanup:
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_allocate(
    sss_sscp_key_store_t *keyStore, uint32_t keyStoreId)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};

    op.paramTypes = SSCP_PARAMS(ValueInput, None, None, None, None, None);
    SET_OP_0_KeyStore();
    op.params[1].value.a = keyStoreId;

    sscp_context_t *sscpCtx = keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_CMD_ALLOCATE_KEYSTORE, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_save(sss_sscp_key_store_t *keyStore)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(None, None, None, None, None, None);
    SET_OP_0_KeyStore();

    sscp_context_t *sscpCtx = keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SAVE_KEYSTORE, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_load(sss_sscp_key_store_t *keyStore)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(ValueInput, None, None, None, None, None);
    SET_OP_0_KeyStore();

    sscp_context_t *sscpCtx = keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_CMD_LOAD_KEYSTORE, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_set_key(sss_sscp_key_store_t *keyStore,
    sss_sscp_object_t *keyObject,
    const uint8_t *data,
    size_t dataLen,
    size_t keyBitLen,
    void *options,
    size_t optionsLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sscp_operation_t op = {0};
    if (keyObject->transientObject == NULL) {
        op.paramTypes =
            SSCP_PARAMS(ValueInput, MemrefInput, None, None, None, None);
        SET_OP_0_KeyStore();
        op.params[1].value.a = keyObject->objectType;
        op.params[1].value.b = keyObject->keyId;
        op.params[2].memref.buffer = (void *)data;
        op.params[2].memref.size = dataLen;
        op.params[3].value.a = (uint32_t)keyBitLen;
        op.params[3].value.b = keyObject->cipherType;
        sscp_context_t *sscpCtx = keyStore->session->sscp_context;
        resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SET_KEY, &op, &ret);
        if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
            ret = kStatus_SSS_Success;
        }
        else {
            ret = kStatus_SSS_Fail;
        }
    }
    else {
        memcpy(keyObject->transientObject, data, dataLen);
        ret = kStatus_SSS_Success;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_generate_key(sss_sscp_key_store_t *keyStore,
    sss_sscp_object_t *keyObject,
    size_t keyBitLen,
    void *options)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sscp_operation_t op = {0};

    op.paramTypes = SSCP_PARAMS(ValueInput, ValueInput, None, None, None, None);
    SET_OP_0_KeyStore();
    op.params[1].value.a = keyObject->objectType;
    op.params[1].value.b = keyObject->keyId;
    op.params[2].value.a = (uint32_t)(keyBitLen / 8);
    op.params[2].value.b = keyObject->cipherType;

    sscp_context_t *sscpCtx = keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_CMD_GENERATE_KEY, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_get_key(sss_sscp_key_store_t *keyStore,
    sss_sscp_object_t *keyObject,
    uint8_t *data,
    size_t *dataLen,
    size_t *pKeyBitLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sscp_operation_t op = {0};

    if (keyObject->transientObject == NULL) {
        op.paramTypes = SSCP_PARAMS(
            ValueInput, MemrefOutput, ValueOutput, None, None, None);
        SET_OP_0_KeyStore();
        op.params[1].value.a = keyObject->objectType;
        op.params[1].value.b = keyObject->keyId;
        op.params[2].memref.buffer = data;
        op.params[2].memref.size = *dataLen;
        op.params[3].value.a = (uint32_t)(*dataLen);
        op.params[3].value.b = keyObject->cipherType;

        sscp_context_t *sscpCtx = keyStore->session->sscp_context;
        resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_CMD_GET_KEY, &op, &ret);
        if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
            *dataLen = op.params[3].value.a;
            *pKeyBitLen = op.params[3].value.b;
            ret = kStatus_SSS_Success;
        }
        else {
            *pKeyBitLen = 0;
            ret = kStatus_SSS_Fail;
        }
    }
    else {
        if (*dataLen >= keyObject->transientObjectLen) {
            memcpy(data,
                keyObject->transientObject,
                keyObject->transientObjectLen);
            *pKeyBitLen = keyObject->transientObjectLen * 8;
            *dataLen = keyObject->transientObjectLen;
            ret = kStatus_SSS_Success;
        }
        else {
            LOG_E("Error: dataLen > keyObject->transientObjectLen");
            ret = kStatus_SSS_Fail;
        }
    }
    return (sss_status_t)ret;
}
#if 0
/* To be reviewed: Purnank */
sss_status_t sss_sscp_key_store_get_key_fromoffset(sss_sscp_key_store_t *keyStore,
    sss_sscp_object_t *keyObject,
    uint8_t *data,
    size_t *dataLen,
    size_t *pKeyBitLen,
    uint16_t offset)
{
    sscp_status_t resSSCP = kStatus_SSCP_Success;
    return (sss_status_t)ret;
}
#endif
sss_status_t sss_sscp_key_store_open_key(
    sss_sscp_key_store_t *keyStore, sss_sscp_object_t *keyObject)
{
    uint32_t ret = kStatus_SSS_Fail;
    /* TBU */
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_freeze_key(
    sss_sscp_key_store_t *keyStore, sss_sscp_object_t *keyObject)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sscp_operation_t op = {0};
    op.paramTypes = SSCP_PARAMS(ValueInput, None, None, None, None, None);
    SET_OP_0_KeyStore();
    op.params[1].value.a = keyObject->objectType;
    op.params[1].value.b = keyObject->keyId;
    op.params[2].value.a = keyObject->cipherType;

    sscp_context_t *sscpCtx = keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_CMD_FREEZE_KEY, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_erase_key(
    sss_sscp_key_store_t *keyStore, sss_sscp_object_t *keyObject)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};

    op.paramTypes = SSCP_PARAMS(ValueInput, None, None, None, None, None);
    SET_OP_0_KeyStore();
    op.params[1].value.a = keyObject->objectType;
    op.params[1].value.b = keyObject->keyId;
    op.params[2].value.a = keyObject->cipherType;

    sscp_context_t *sscpCtx = keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_CMD_ERASE_KEY, &op, &ret);
    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

void sss_sscp_key_store_context_free(sss_sscp_key_store_t *keyStore)
{
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
    sss_a71ch_key_store_t *a71xx_keyStore = (sss_a71ch_key_store_t *)keyStore;
    sscp_a71chkey_store_context_free(a71xx_keyStore);
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
    sss_a71cl_key_store_t *a71xx_keyStore = (sss_a71cl_key_store_t *)keyStore;
#else
#error "No Security Subsystem Defined"
#endif
    memset(a71xx_keyStore, 0, sizeof(*a71xx_keyStore));
}

/* End: sscp_keystore */

/* ************************************************************************** */
/* Functions : sss_sscp_asym                                                  */
/* ************************************************************************** */

sss_status_t sss_sscp_asymmetric_context_init(sss_sscp_asymmetric_t *context,
    sss_sscp_session_t *session,
    sss_sscp_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSS_Fail;
    sscp_operation_t op = {0};
    ENSURE_OR_GO_CLEANUP(context);
    ENSURE_OR_GO_CLEANUP(keyObject);
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
    ENSURE_OR_GO_CLEANUP(
        keyObject->keyStore->session->subsystem == kType_SSS_SE_A71CH);
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
    ENSURE_OR_GO_CLEANUP(
        keyObject->keyStore->session->subsystem == kType_SSS_SE_A71CL);
#endif
    memset(context, 0, sizeof(*context));
    context->algorithm = algorithm;
    context->mode = mode;
    context->session = session;
    context->keyObject = keyObject;

    op.paramTypes =
        SSCP_PARAMS(ValueInput, ValueInput, ValueInput, None, None, None);
    SET_OP_0_Asymm();
    op.params[1].value.a = context->algorithm;
    op.params[1].value.b = context->mode;
    op.params[2].value.a = context->keyObject->keyId;
    op.params[3].value.a = context->sessionId;

    sscp_context_t *sscpCtx =
        context->keyObject->keyStore->session->sscp_context;
    resSSCP = sscpCtx->invoke(sscpCtx, kSSCP_ASYMMETRIC_CTX_INIT, &op, &ret);
    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        context->sessionId = op.params[3].value.a;
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
cleanup:
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_asymmetric_encrypt(sss_sscp_asymmetric_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sscp_operation_t op = {0};
    op.paramTypes =
        SSCP_PARAMS(ValueInput, MemrefInput, MemrefOutput, None, None, None);
    SET_OP_0_Asymm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = (void *)srcData;
    op.params[2].memref.size = srcLen;
    op.params[3].memref.buffer = destData;
    op.params[3].memref.size = *destLen;
    sscp_context_t *ctx = context->keyObject->keyStore->session->sscp_context;
    resSSCP = ctx->invoke(ctx, kSSCP_ASYMMETRIC_CMD_ENCRYPT, &op, &ret);
    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        *destLen = op.params[3].memref.size;
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_asymmetric_decrypt(sss_sscp_asymmetric_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sscp_operation_t op = {0};
    op.paramTypes =
        SSCP_PARAMS(ValueInput, MemrefInput, MemrefOutput, None, None, None);
    SET_OP_0_Asymm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = (void *)srcData;
    op.params[2].memref.size = srcLen;
    op.params[3].memref.buffer = destData;
    op.params[3].memref.size = *destLen;
    sscp_context_t *ctx = context->keyObject->keyStore->session->sscp_context;
    resSSCP = ctx->invoke(ctx, kSSCP_ASYMMETRIC_CMD_DECRYPT, &op, &ret);
    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        *destLen = op.params[2].memref.size;
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_asymmetric_sign_digest(sss_sscp_asymmetric_t *context,
    uint8_t *digest,
    size_t digestLen,
    uint8_t *signature,
    size_t *signatureLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    sscp_operation_t op = {0};

    op.paramTypes =
        SSCP_PARAMS(ValueInput, MemrefInput, MemrefOutput, None, None, None);
    SET_OP_0_Asymm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = digest;
    op.params[2].memref.size = digestLen;
    op.params[3].memref.buffer = signature;
    op.params[3].memref.size = *signatureLen;

    sscp_context_t *sscpCtx =
        context->keyObject->keyStore->session->sscp_context;
    resSSCP =
        sscpCtx->invoke(sscpCtx, kSSCP_ASYMMETRIC_CMD_SIGN_DIGEST, &op, &ret);
    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        *signatureLen = op.params[3].memref.size;
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_asymmetric_verify_digest(sss_sscp_asymmetric_t *context,
    uint8_t *digest,
    size_t digestLen,
    uint8_t *signature,
    size_t signatureLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};

    op.paramTypes = SSCP_PARAMS(
        ValueInput, MemrefInput, MemrefInput, ValueOutput, None, None);
    SET_OP_0_Asymm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = digest;
    op.params[2].memref.size = digestLen;
    op.params[3].memref.buffer = signature;
    op.params[3].memref.size = signatureLen;

    sscp_context_t *sscpCtx =
        context->keyObject->keyStore->session->sscp_context;
    resSSCP =
        sscpCtx->invoke(sscpCtx, kSSCP_ASYMMETRIC_CMD_VERIFY_DIGEST, &op, &ret);

    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        ret = kStatus_SSS_Success;
    }
    else {
        ret = kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

void sss_sscp_asymmetric_context_free(sss_sscp_asymmetric_t *context)
{
    memset(context, 0, sizeof(*context));
}

/* End: sscp_asym */

/* ************************************************************************** */
/* Functions : sss_sscp_symm                                                  */
/* ************************************************************************** */

sss_status_t sss_sscp_symmetric_context_init(sss_sscp_symmetric_t *context,
    sss_sscp_session_t *session,
    sss_sscp_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSS_Fail;
    sscp_operation_t op = {0};
    ENSURE_OR_GO_CLEANUP(context);
    ENSURE_OR_GO_CLEANUP(keyObject);
#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
    ENSURE_OR_GO_CLEANUP(
        keyObject->keyStore->session->subsystem == kType_SSS_SE_A71CH);
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
    ENSURE_OR_GO_CLEANUP(
        keyObject->keyStore->session->subsystem == kType_SSS_SE_A71CL);
#elif
#error "No Security Subsystem Defined"
#endif
    memset(context, 0, sizeof(*context));
    context->algorithm = algorithm;
    context->mode = mode;
    context->session = session;
    context->keyObject = keyObject;

    op.paramTypes =
        SSCP_PARAMS(ValueInput, ValueInput, ValueOutput, None, None, None);
    SET_OP_0_Symm();
    op.params[1].value.a = context->algorithm;
    op.params[1].value.b = context->mode;
    op.params[2].value.a = context->keyObject->keyId;
    op.params[3].value.a = context->sessionId;

    sscp_context_t *ctx = context->keyObject->keyStore->session->sscp_context;
    resSSCP = ctx->invoke(ctx, kSSCP_SYMM_CIPHER_CTX_INIT, &op, &ret);
    if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
        ret = kStatus_SSS_Success;
        context->sessionId = op.params[3].value.a;
    }
cleanup:
    return (sss_status_t)ret;
}

sss_status_t sss_sscp_cipher_one_go(sss_sscp_symmetric_t *context,
    uint8_t *iv,
    size_t ivLen,
    const uint8_t *srcData,
    uint8_t *destData,
    size_t dataLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};

    op.paramTypes = SSCP_PARAMS(
        ValueInput, MemrefInput, MemrefInput, MemrefOutput, None, None);
    SET_OP_0_Symm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = iv;
    op.params[2].memref.size = ivLen;
    op.params[3].memref.buffer = (void *)srcData;
    op.params[3].memref.size = dataLen;
    op.params[4].memref.buffer = destData;
    op.params[4].memref.size = dataLen;

    sscp_context_t *ctx = context->keyObject->keyStore->session->sscp_context;
    resSSCP = ctx->invoke(ctx, kSSCP_SYMM_CIPHER_ONE_GO, &op, &ret);
    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return ret;
}

sss_status_t sss_sscp_cipher_init(
    sss_sscp_symmetric_t *context, uint8_t *iv, size_t ivLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes =
        SSCP_PARAMS(ValueInput, MemrefInput, None, None, None, None);
    SET_OP_0_Symm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = iv;
    op.params[2].memref.size = ivLen;

    sscp_context_t *ctx = context->keyObject->keyStore->session->sscp_context;
    resSSCP = ctx->invoke(ctx, kSSCP_SYMM_CIPHER_INIT, &op, &ret);

    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_cipher_update(sss_sscp_symmetric_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes =
        SSCP_PARAMS(ValueInput, MemrefInput, MemrefOutput, None, None, None);
    SET_OP_0_Symm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = (void *)srcData;
    op.params[2].memref.size = srcLen;
    op.params[3].memref.buffer = destData;
    op.params[3].memref.size = *destLen;

    sscp_context_t *ctx = context->keyObject->keyStore->session->sscp_context;
    resSSCP = ctx->invoke(ctx, kSSCP_SYMM_CIPHER_UPDATE, &op, &ret);

    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_cipher_finish(sss_sscp_symmetric_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};
    op.paramTypes =
        SSCP_PARAMS(ValueInput, MemrefInput, MemrefOutput, None, None, None);
    SET_OP_0_Symm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = (void *)srcData;
    op.params[2].memref.size = srcLen;
    op.params[3].memref.buffer = destData;
    op.params[3].memref.size = *destLen;

    sscp_context_t *ctx = context->keyObject->keyStore->session->sscp_context;
    resSSCP = ctx->invoke(ctx, kSSCP_SYMM_CIPHER_FINISH, &op, &ret);

    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_cipher_crypt_ctr(sss_sscp_symmetric_t *context,
    const uint8_t *srcData,
    uint8_t *destData,
    size_t size,
    uint8_t *initialCounter,
    uint8_t *lastEncryptedCounter,
    size_t *szLeft)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret;
    sscp_operation_t op = {0};

    op.paramTypes = SSCP_PARAMS(ValueInput,
        MemrefInput,
        MemrefOutput,
        MemrefInOut,
        MemrefInOut,
        MemrefInOut);

    SET_OP_0_Symm();
    op.params[1].value.a = context->sessionId;
    op.params[2].memref.buffer = (void *)(uintptr_t)srcData;
    op.params[2].memref.size = size;
    op.params[3].memref.buffer = destData;
    op.params[3].memref.size = size;
    op.params[4].memref.buffer = initialCounter;
    op.params[4].memref.size = SSS_AES_BLOCK_SIZE;
    op.params[5].memref.buffer = lastEncryptedCounter;
    op.params[5].memref.size = SSS_AES_BLOCK_SIZE;
    op.params[6].memref.buffer = szLeft;
    op.params[6].memref.size = sizeof(*szLeft);

    sscp_context_t *ctx = context->keyObject->keyStore->session->sscp_context;
    resSSCP = ctx->invoke(ctx, kSSCP_SYMM_CIPHER_CRYPT_CTR, &op, &ret);

    if (resSSCP != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return kStatus_SSS_Success;
}

void sss_sscp_symmetric_context_free(sss_sscp_symmetric_t *context)
{
    memset(context, 0, sizeof(*context));
}
/* End: sscp_symm */

/* ************************************************************************** */
/* Functions : sss_sscp_mac                                                   */
/* ************************************************************************** */

/* ************************************************************************** */
/* Functions : sss_sscp_aead                                                  */
/* ************************************************************************** */

sss_status_t sss_sscp_aead_context_init(sss_sscp_aead_t *context,
    sss_sscp_session_t *session,
    sss_sscp_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_sscp_aead_one_go(sss_sscp_aead_t *context,
    const uint8_t *srcData,
    uint8_t *destData,
    size_t size,
    uint8_t *nonce,
    size_t nonceLen,
    const uint8_t *aad,
    size_t aadLen,
    uint8_t *tag,
    size_t *tagLen)
{
    SSCP_BUILD_ASSURE(
        sizeof(sss_aead_t) >= sizeof(sss_sscp_aead_t), _sss_sscp_aead_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* set paramTypes for the tag[] buffer according to mode
    * for encrypt (and authenticate) mode, the tag[] is output, written by invoked function,
    * for decrypt (and verify) mode, the tag[] is input, to be verified by invoked function
    */
    if (context->mode == kMode_SSS_Encrypt) {
        op.paramTypes = SSCP_PARAMS(MemrefInput,
            MemrefOutput,
            MemrefInput,
            MemrefInput,
            MemrefOutput,
            None);
    }
    else {
        op.paramTypes = SSCP_PARAMS(MemrefInput,
            MemrefOutput,
            MemrefInput,
            MemrefInput,
            MemrefInput,
            None);
    }

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Aead;

    op.params[1].memref.buffer = (void *)(uintptr_t)srcData;
    op.params[1].memref.size = size;
    op.params[2].memref.buffer = destData;
    op.params[2].memref.size = size;
    op.params[3].memref.buffer = nonce;
    op.params[3].memref.size = nonceLen;
    op.params[4].memref.buffer = (void *)(uintptr_t)aad;
    op.params[4].memref.size = aadLen;
    op.params[5].memref.buffer = tag;
    op.params[5].memref.size = *tagLen;

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_AeadOneGo, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    if (context->mode == kMode_SSS_Encrypt) {
        /* tagLen returns number of bytes written to tag */
        *tagLen = op.params[5].memref.size;
    }

    return (sss_status_t)ret;
}
/* End: sscp_aead */

sss_status_t sss_sscp_aead_init(sss_sscp_aead_t *context,
    uint8_t *nonce,
    size_t nonceLen,
    size_t tagLen,
    size_t aadLen,
    size_t payloadLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_sscp_aead_update_aad(
    sss_sscp_aead_t *context, const uint8_t *aadData, size_t aadDataLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_sscp_aead_update(sss_sscp_aead_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t destData,
    size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

sss_status_t sss_sscp_aead_finish(sss_sscp_aead_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t destData,
    size_t *destLen,
    uint8_t *tag,
    size_t *tagLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_sscp_aead_context_free(sss_sscp_aead_t *context)
{
    ;
}

/* End: sscp_aead */

sss_status_t sss_sscp_mac_context_init(sss_sscp_mac_t *context,
    sss_sscp_session_t *session,
    sss_sscp_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* TBU */
    return retval;
}

void sss_sscp_mac_context_free(sss_sscp_mac_t *context)
{
    ;
}

/* End: sscp_mac */

/* ************************************************************************** */
/* Functions : sss_sscp_tunnel                                                  */
/* ************************************************************************** */
sss_status_t sss_sscp_tunnel(sss_sscp_tunnel_t *context,
    uint8_t *data,
    size_t dataLen,
    sss_sscp_object_t *keyObjects,
    uint32_t keyObjectCount,
    uint32_t tunnelType)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_PARAMS(
        MemrefInput, ContextReference, ValueInput, None, None, None);

    SET_OP_0_Tunnel();

    op.params[1].memref.buffer = data;
    op.params[1].memref.size = dataLen;
    op.params[2].context.ptr = keyObjects;
    op.params[2].context.type = kSSCP_ParamContextType_SSS_Object;
    op.params[3].value.a = keyObjectCount;
    op.params[3].value.b = tunnelType;

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_Tunnel, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}
/* End: sscp_tunnel */
/* ************************************************************************** */
/* Functions : sss_sscp_mac                                                  */
/* ************************************************************************** */
sss_status_t sss_sscp_mac_one_go(sss_sscp_mac_t *context,
    const uint8_t *message,
    size_t messageLen,
    uint8_t *mac,
    size_t *macLen)
{
    SSCP_BUILD_ASSURE(
        sizeof(sss_mac_t) >= sizeof(sss_sscp_mac_t), _sss_sscp_mac_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to macLen, it is assumed that mac[] buffer is big enough to hold full
    * mac */
    size_t len = (macLen != NULL) ? *macLen : context->macFullLen;

    /* if the *macLen cannot hold full mac (per algorithm spec) return error */
    if (len < context->macFullLen) {
        return kStatus_SSS_Fail;
    }

    op.paramTypes =
        SSCP_PARAMS(MemrefInput, MemrefOutput, None, None, None, None);

    SET_OP_0_Mac();

    op.params[1].memref.buffer = (void *)(uintptr_t)message;
    op.params[1].memref.size = messageLen;
    op.params[2].memref.buffer = mac;
    op.params[2].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MacOneGo, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        if (macLen) {
            *macLen = 0;
        }
        return kStatus_SSS_Fail;
    }

    /* update the size member of kSSCP_ParamType_MemrefOutput param with the actual byte length written to output buffer
    */
    if (macLen) {
        *macLen = op.params[2].memref.size;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_mac_init(sss_sscp_mac_t *context)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_PARAMS(None, None, None, None, None, None);

    SET_OP_0_Mac();

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MacInit, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_mac_update(
    sss_sscp_mac_t *context, const uint8_t *message, size_t messageLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_PARAMS(MemrefInput, None, None, None, None, None);

    SET_OP_0_Mac();

    op.params[1].memref.buffer = (void *)(uintptr_t)message;
    op.params[1].memref.size = messageLen;

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MacUpdate, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_mac_finish(
    sss_sscp_mac_t *context, uint8_t *mac, size_t *macLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to macLen, it is assumed that mac[] buffer is big enough to hold full
    * mac */
    size_t len = (macLen != NULL) ? *macLen : context->macFullLen;

    /* if the *macLen cannot hold full mac (per algorithm spec) return error */
    if (len < context->macFullLen) {
        return kStatus_SSS_Fail;
    }

    op.paramTypes = SSCP_PARAMS(MemrefOutput, None, None, None, None, None);

    SET_OP_0_Mac();

    op.params[1].memref.buffer = mac;
    op.params[1].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MacFinish, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        if (macLen) {
            *macLen = 0;
        }
        return kStatus_SSS_Fail;
    }

    /* the size member of kSSCP_ParamType_MemrefOutput param is updated with the actual byte length written to output
    * buffer
    */
    if (macLen) {
        *macLen = op.params[1].memref.size;
    }

    return (sss_status_t)ret;
}
/* End: sscp_mac */

/* ************************************************************************** */
/* Functions : sss_sscp_digest                                                  */
/* ************************************************************************** */
sss_status_t sss_sscp_digest_context_init(sss_sscp_digest_t *context,
    sss_sscp_session_t *session,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Fail;

    ENSURE_OR_GO_CLEANUP(context);
    context->session = session;
    context->algorithm = algorithm;
    context->mode = mode;
    retval = kStatus_SSS_Success;
cleanup:
    return retval;
}

sss_status_t sss_sscp_digest_one_go(sss_sscp_digest_t *context,
    const uint8_t *message,
    size_t messageLen,
    uint8_t *digest,
    size_t *digestLen)
{
    SSCP_BUILD_ASSURE(sizeof(sss_digest_t) >= sizeof(sss_sscp_digest_t),
        _sss_sscp_digest_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to digestLen, it is assumed that digest[] buffer is big enough to hold full
    * digest */
    size_t len = (digestLen != NULL) ? *digestLen : context->digestFullLen;

    /* if the *digestLen cannot hold full digest (per algorithm spec) return error */
    if (len < context->digestFullLen) {
        return kStatus_SSS_Fail;
    }

    op.paramTypes =
        SSCP_PARAMS(MemrefInput, MemrefOutput, None, None, None, None);

    SET_OP_0_Digest();

    op.params[1].memref.buffer = (void *)(uintptr_t)message;
    op.params[1].memref.size = messageLen;
    op.params[2].memref.buffer = digest;
    op.params[2].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_DigestOneGo, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        if (digestLen) {
            *digestLen = 0;
        }
        return kStatus_SSS_Fail;
    }

    /* update the size member of kSSCP_ParamType_MemrefOutput param with the actual byte length written to output buffer
    */
    if (digestLen) {
        *digestLen = op.params[2].memref.size;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_digest_init(sss_sscp_digest_t *context)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_PARAMS(None, None, None, None, None, None);

    SET_OP_0_Digest();

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_DigestInit, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_digest_update(
    sss_sscp_digest_t *context, const uint8_t *message, size_t messageLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_PARAMS(MemrefInput, None, None, None, None, None);

    SET_OP_0_Digest();

    op.params[1].memref.buffer = (void *)(uintptr_t)message;
    op.params[1].memref.size = messageLen;

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_DigestUpdate, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_digest_finish(
    sss_sscp_digest_t *context, uint8_t *digest, size_t *digestLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to digestLen, it is assumed that digest[] buffer is big enough to hold full
    * digest */
    size_t len = (digestLen != NULL) ? *digestLen : context->digestFullLen;

    /* if the *digestLen cannot hold full digest (per algorithm spec) return error */
    if (len < context->digestFullLen) {
        return kStatus_SSS_Fail;
    }

    op.paramTypes = SSCP_PARAMS(MemrefOutput, None, None, None, None, None);

    SET_OP_0_Digest();

    op.params[1].memref.buffer = digest;
    op.params[1].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp_context;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_DigestFinish, &op, &ret);
    if (status != kStatus_SSCP_Success) {
        if (digestLen) {
            *digestLen = 0;
        }
        return kStatus_SSS_Fail;
    }

    /* the size member of kSSCP_ParamType_MemrefOutput param is updated with the actual byte length written to output
    * buffer
    */
    if (digestLen) {
        *digestLen = op.params[1].memref.size;
    }

    return (sss_status_t)ret;
}

void sss_sscp_digest_context_free(sss_sscp_digest_t *context)
{
    memset(context, 0, sizeof(*context));
}

/* End: sscp_digest */

/* ************************************************************************** */
/* Functions : sss_sscp_rng                                                   */
/* ************************************************************************** */

sss_status_t sss_sscp_rng_context_init(
    sss_sscp_rng_context_t *context, sss_sscp_session_t *session)
{
    sss_status_t retval = kStatus_SSS_Fail;

    ENSURE_OR_GO_CLEANUP(context);
    context->session = session;
    retval = kStatus_SSS_Success;
cleanup:
    return retval;
}

sss_status_t sss_sscp_rng_get_random(
    sss_sscp_rng_context_t *context, uint8_t *random_data, size_t dataLen)
{
    sscp_status_t resSSCP = kStatus_SSCP_Fail;
    uint32_t ret = kStatus_SSCP_Fail;
    size_t chunk = 0;
    size_t offset = 0;
    sscp_operation_t op = {0};
    op.paramTypes =
        SSCP_PARAMS(ValueInput, MemrefInput, MemrefOutput, None, None, None);
    SET_OP_0_RandGen();

    while (dataLen > 0) {
        if (dataLen > 128) {
            chunk = 128;
        }
        else {
            chunk = dataLen;
        }

        op.params[1].value.a = context->session->sessionId;
        op.params[2].memref.buffer = (void *)(random_data + offset);
        op.params[2].memref.size = chunk;

        sscp_context_t *ctx = context->session->sscp_context;
        resSSCP = ctx->invoke(ctx, kSSCP_GEN_RANDOM_NUM, &op, &ret);
        if ((resSSCP == kStatus_SSCP_Success) && (ret == kStatus_SSS_Success)) {
            ret = kStatus_SSS_Success;
        }
        else {
            ret = kStatus_SSS_Fail;
            break;
        }

        offset += chunk;
        dataLen -= chunk;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_rng_context_free(sss_sscp_rng_context_t *context)
{
    sss_status_t retval = kStatus_SSS_Success;
    memset(context, 0, sizeof(*context));
    return retval;
}

/* End: sscp_rng */

#endif /* SSS_HAVE_SSCP */
