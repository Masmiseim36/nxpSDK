/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sss_sscp.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "fsl_sss_api.h"
#include "fsl_sscp.h"
#include "fsl_sscp_commands.h"

sss_status_t sss_sscp_cipher_one_go(
    sss_sscp_symmetric_t *context, uint8_t *iv, size_t ivLen, const uint8_t *srcData, uint8_t *destData, size_t dataLen)
{
    SSCP_BUILD_ASSURE(sizeof(sss_symmetric_t) >= sizeof(sss_sscp_symmetric_t), _sss_sscp_symmetric_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput,
                                      kSSCP_ParamType_MemrefInput, kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_None,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Symmetric;

    op.params[1].memref.buffer = iv;
    op.params[1].memref.size = ivLen;
    op.params[2].memref.buffer = (void *)(uintptr_t)srcData;
    op.params[2].memref.size = dataLen;
    op.params[3].memref.buffer = destData;
    op.params[3].memref.size = dataLen;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_SymmetricCipherOneGo, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
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
    SSCP_BUILD_ASSURE(sizeof(sss_symmetric_t) >= sizeof(sss_sscp_symmetric_t), _sss_sscp_symmetric_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* this func specific for AES CTR */
    /* don't consider DES at the moment for the CTR mode */
    if (context->algorithm != kAlgorithm_SSS_AES_CTR)
    {
        return kStatus_SSS_InvalidArgument;
    }

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput,
                                      kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_MemrefInOut,
                                      kSSCP_ParamType_MemrefInOut, kSSCP_ParamType_MemrefInOut, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Symmetric;

    op.params[1].memref.buffer = (void *)(uintptr_t)srcData;
    op.params[1].memref.size = size;
    op.params[2].memref.buffer = destData;
    op.params[2].memref.size = size;
    op.params[3].memref.buffer = initialCounter;
    op.params[3].memref.size = SSS_AES_BLOCK_SIZE;
    op.params[4].memref.buffer = lastEncryptedCounter;
    op.params[4].memref.size = SSS_AES_BLOCK_SIZE;
    op.params[5].memref.buffer = szLeft;
    op.params[5].memref.size = sizeof(*szLeft);

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_SymmetricCryptCtr, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
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
    SSCP_BUILD_ASSURE(sizeof(sss_aead_t) >= sizeof(sss_sscp_aead_t), _sss_sscp_aead_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* set paramTypes for the tag[] buffer according to mode
     * for encrypt (and authenticate) mode, the tag[] is output, written by invoked function,
     * for decrypt (and verify) mode, the tag[] is input, to be verified by invoked function
     */
    if (context->mode == kMode_SSS_Encrypt)
    {
        op.paramTypes =
            SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput,
                              kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_MemrefInput,
                              kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_None);
    }
    else
    {
        op.paramTypes =
            SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput,
                              kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_MemrefInput,
                              kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None);
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

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_AeadOneGo, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    if (context->mode == kMode_SSS_Encrypt)
    {
        /* tagLen returns number of bytes written to tag */
        *tagLen = op.params[5].memref.size;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_digest_one_go(
    sss_sscp_digest_t *context, const uint8_t *message, size_t messageLen, uint8_t *digest, size_t *digestLen)
{
    SSCP_BUILD_ASSURE(sizeof(sss_digest_t) >= sizeof(sss_sscp_digest_t), _sss_sscp_digest_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to digestLen, it is assumed that digest[] buffer is big enough to hold full
     * digest */
    size_t len = (digestLen != NULL) ? *digestLen : context->digestFullLen;

    /* if the *digestLen cannot hold full digest (per algorithm spec) return error */
    if (len < context->digestFullLen)
    {
        return kStatus_SSS_Fail;
    }

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_MemrefOutput,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Digest;

    op.params[1].memref.buffer = (void *)(uintptr_t)message;
    op.params[1].memref.size = messageLen;
    op.params[2].memref.buffer = digest;
    op.params[2].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_DigestOneGo, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        if (digestLen)
        {
            *digestLen = 0;
        }
        return kStatus_SSS_Fail;
    }

    /* update the size member of kSSCP_ParamType_MemrefOutput param with the actual byte length written to output buffer
     */
    if (digestLen)
    {
        *digestLen = op.params[2].memref.size;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_digest_init(sss_sscp_digest_t *context)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_None, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Digest;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_DigestInit, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_digest_update(sss_sscp_digest_t *context, const uint8_t *message, size_t messageLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Digest;

    op.params[1].memref.buffer = (void *)(uintptr_t)message;
    op.params[1].memref.size = messageLen;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_DigestUpdate, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_digest_finish(sss_sscp_digest_t *context, uint8_t *digest, size_t *digestLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to digestLen, it is assumed that digest[] buffer is big enough to hold full
     * digest */
    size_t len = (digestLen != NULL) ? *digestLen : context->digestFullLen;

    /* if the *digestLen cannot hold full digest (per algorithm spec) return error */
    if (len < context->digestFullLen)
    {
        return kStatus_SSS_Fail;
    }

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Digest;

    op.params[1].memref.buffer = digest;
    op.params[1].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_DigestFinish, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        if (digestLen)
        {
            *digestLen = 0;
        }
        return kStatus_SSS_Fail;
    }

    /* the size member of kSSCP_ParamType_MemrefOutput param is updated with the actual byte length written to output
     * buffer
     */
    if (digestLen)
    {
        *digestLen = op.params[1].memref.size;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_mac_one_go(
    sss_sscp_mac_t *context, const uint8_t *message, size_t messageLen, uint8_t *mac, size_t *macLen)
{
    SSCP_BUILD_ASSURE(sizeof(sss_mac_t) >= sizeof(sss_sscp_mac_t), _sss_sscp_mac_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to macLen, it is assumed that mac[] buffer is big enough to hold full
     * mac */
    size_t len = (macLen != NULL) ? *macLen : context->macFullLen;

    /* if the *macLen cannot hold full mac (per algorithm spec) return error */
    if (len < context->macFullLen)
    {
        return kStatus_SSS_Fail;
    }

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_MemrefOutput,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mac;

    op.params[1].memref.buffer = (void *)(uintptr_t)message;
    op.params[1].memref.size = messageLen;
    op.params[2].memref.buffer = mac;
    op.params[2].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MacOneGo, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        if (macLen)
        {
            *macLen = 0;
        }
        return kStatus_SSS_Fail;
    }

    /* update the size member of kSSCP_ParamType_MemrefOutput param with the actual byte length written to output buffer
     */
    if (macLen)
    {
        *macLen = op.params[2].memref.size;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_mac_init(sss_sscp_mac_t *context)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_None, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mac;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MacInit, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_mac_update(sss_sscp_mac_t *context, const uint8_t *message, size_t messageLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mac;

    op.params[1].memref.buffer = (void *)(uintptr_t)message;
    op.params[1].memref.size = messageLen;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MacUpdate, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_mac_finish(sss_sscp_mac_t *context, uint8_t *mac, size_t *macLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to macLen, it is assumed that mac[] buffer is big enough to hold full
     * mac */
    size_t len = (macLen != NULL) ? *macLen : context->macFullLen;

    /* if the *macLen cannot hold full mac (per algorithm spec) return error */
    if (len < context->macFullLen)
    {
        return kStatus_SSS_Fail;
    }

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mac;

    op.params[1].memref.buffer = mac;
    op.params[1].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MacFinish, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        if (macLen)
        {
            *macLen = 0;
        }
        return kStatus_SSS_Fail;
    }

    /* the size member of kSSCP_ParamType_MemrefOutput param is updated with the actual byte length written to output
     * buffer
     */
    if (macLen)
    {
        *macLen = op.params[1].memref.size;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_asymmetric_sign_digest(
    sss_sscp_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t *signatureLen)
{
    SSCP_BUILD_ASSURE(sizeof(sss_asymmetric_t) >= sizeof(sss_sscp_asymmetric_t), _sss_sscp_asymmetric_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    /* if the caller gives NULL pointer to signatureLen, it is assumed that signature[] buffer is big enough to hold
     * full
     * signature */
    size_t len = (signatureLen != NULL) ? *signatureLen : context->signatureFullLen;

    /* if the *signatureLen cannot hold full signature (per algorithm spec) return error */
    if (len < context->signatureFullLen)
    {
        return kStatus_SSS_Fail;
    }

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_MemrefOutput,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Asymmetric;

    op.params[1].memref.buffer = digest;
    op.params[1].memref.size = digestLen;
    op.params[2].memref.buffer = signature;
    op.params[2].memref.size = len;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_AsymmetricSignDigest, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    /* the size member of kSSCP_ParamType_MemrefOutput param is updated with the actual byte length written to output
     * buffer
     */
    if (signatureLen)
    {
        *signatureLen = op.params[2].memref.size;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_asymmetric_verify_digest(
    sss_sscp_asymmetric_t *context, uint8_t *digest, size_t digestLen, uint8_t *signature, size_t signatureLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_MemrefInput,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Asymmetric;

    /* common function parameters */
    op.params[1].memref.buffer = digest;
    op.params[1].memref.size = digestLen;
    op.params[2].memref.buffer = signature;
    op.params[2].memref.size = signatureLen;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_AsymmetricVerifyDigest, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_tunnel(sss_sscp_tunnel_t *context,
                             uint8_t *data,
                             size_t dataLen,
                             sss_sscp_object_t *keyObjects,
                             uint32_t keyObjectCount,
                             uint32_t tunnelType)
{
    SSCP_BUILD_ASSURE(sizeof(sss_tunnel_t) >= sizeof(sss_sscp_tunnel_t), _sss_sscp_tunnel_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput,
                                      kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInput,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Tunnel;

    op.params[1].memref.buffer = data;
    op.params[1].memref.size = dataLen;
    op.params[2].context.ptr = keyObjects;
    op.params[2].context.type = kSSCP_ParamContextType_SSS_Object;
    op.params[3].value.a = keyObjectCount;
    op.params[3].value.b = tunnelType;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_Tunnel, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_asymmetric_dh_derive_key(sss_sscp_derive_key_t *context,
                                               sss_sscp_object_t *otherPartyKeyObject,
                                               sss_sscp_object_t *derivedKeyObject)
{
    SSCP_BUILD_ASSURE(sizeof(sss_derive_key_t) >= sizeof(sss_sscp_derive_key_t), _sss_sscp_derive_key_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ContextReference,
                                      kSSCP_ParamType_ContextReference, kSSCP_ParamType_None, kSSCP_ParamType_None,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_DeriveKey;

    op.params[1].context.ptr = otherPartyKeyObject;
    op.params[1].context.type = kSSCP_ParamContextType_SSS_Object;

    op.params[2].context.ptr = derivedKeyObject;
    op.params[2].context.type = kSSCP_ParamContextType_SSS_Object;

    sscp_context_t *sscpCtx = context->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_AsymmetricDeriveKey, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_allocate(sss_sscp_key_store_t *keyStore, uint32_t keyStoreId)
{
    SSCP_BUILD_ASSURE(sizeof(sss_key_store_t) >= sizeof(sss_sscp_key_store_t), _sss_sscp_key_store_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = keyStore;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_KeyStore;

    op.params[1].value.a = keyStoreId;
    op.params[1].value.b = 0;

    sscp_context_t *sscpCtx = keyStore->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_KeyStoreAllocate, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_object_allocate_handle(
    sss_sscp_object_t *keyObject, uint32_t keyId, sss_key_type_t keyType, uint32_t keyByteLenMax, uint32_t options)
{
    SSCP_BUILD_ASSURE(sizeof(sss_object_t) >= sizeof(sss_sscp_object_t), _sss_sscp_object_size);
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInput, kSSCP_ParamType_ValueInput,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = keyObject;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Object;

    op.params[1].value.a = keyId;
    op.params[1].value.b = (uint32_t)keyType;
    op.params[2].value.a = keyByteLenMax;
    op.params[2].value.b = (uint32_t)options;

    sscp_context_t *sscpCtx = keyObject->keyStore->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_KeyObjectAllocateHandle, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}

sss_status_t sss_sscp_key_store_set_key(sss_sscp_key_store_t *keyStore,
                                        sss_sscp_object_t *keyObject,
                                        const uint8_t *key,
                                        uint32_t keyBitLen,
                                        void *options,
                                        size_t optionsLen)
{
    sscp_operation_t op = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret = 0;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ContextReference,
                                      kSSCP_ParamType_MemrefInput, kSSCP_ParamType_ValueInput,
                                      kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr = keyStore;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_KeyStore;

    op.params[1].context.ptr = keyObject;
    op.params[1].context.type = kSSCP_ParamContextType_SSS_Object;

    op.params[2].memref.buffer = (void *)(uintptr_t)key;
    op.params[2].memref.size = (keyBitLen + 7u) / 8u;
    op.params[3].value.a = keyBitLen;
    op.params[3].value.b = 0;
    op.params[4].memref.buffer = options;
    op.params[4].memref.size = optionsLen;

    sscp_context_t *sscpCtx = keyStore->session->sscp;
    status = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_KeyStoreSetKey, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }

    return (sss_status_t)ret;
}
