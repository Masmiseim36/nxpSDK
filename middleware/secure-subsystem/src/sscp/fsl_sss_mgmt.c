/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_sss_mgmt.h"
#include "fsl_sscp_commands.h"
#include "fsl_sss_sscp.h"

sss_status_t sss_mgmt_context_init(sss_mgmt_t *context, sss_sscp_session_t *session)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    context->session = session;
    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_None, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = session;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Session;

    op.resultTypes            = SSCP_OP_SET_RESULT(kSSCP_ParamType_ContextReference);
    op.resultCount            = 1u;
    op.result[0].context.ptr  = context;
    op.result[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_ContextInit, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_get_property(sss_mgmt_t *context, uint32_t propertyId, uint8_t *destData, size_t *dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    size_t len = (dataLen != NULL) ? *dataLen : 0u;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInputSingle,
                                      kSSCP_ParamType_MemrefInOut, kSSCP_ParamType_None, kSSCP_ParamType_None,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].value.a       = propertyId;
    op.params[1].value.b       = 0u;
    op.params[2].memref.buffer = destData;
    op.params[2].memref.size   = len;

    op.resultTypes       = SSCP_OP_SET_RESULT(kSSCP_ParamType_ValueOutputSingle);
    op.resultCount       = 1u;
    op.result[0].value.a = (uint32_t)dataLen;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_PropertyGet, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_set_property(sss_mgmt_t *context, uint32_t propertyId, const uint8_t *srcData, size_t dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInputSingle,
                                      kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None, kSSCP_ParamType_None,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].value.a       = propertyId;
    op.params[2].memref.buffer = (void *)srcData;
    op.params[2].memref.size   = dataLen;

    op.resultTypes = SSCP_OP_SET_RESULT(kSSCP_ParamType_None);
    op.resultCount = 0u;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_PropertySet, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_fuse_shadow_register_read(sss_mgmt_t *context, uint32_t shadowRegisterId, uint32_t *destData)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInputSingle,
                                      kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_None, kSSCP_ParamType_None,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].value.a       = shadowRegisterId;
    op.params[2].memref.buffer = destData;
    op.params[2].memref.size   = sizeof(uint32_t);

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_FuseShadowRegisterRead, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_fuse_read(
    sss_mgmt_t *context, uint32_t fuseId, uint32_t *destData, void *options, size_t *optionsLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;
    size_t len           = (optionsLen != NULL) ? *optionsLen : 0u;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInputSingle,
                                      kSSCP_ParamType_MemrefOutputNoSize, kSSCP_ParamType_MemrefInput,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].value.a = fuseId;

    op.params[2].memref.buffer = destData;

    op.params[3].memref.buffer = options;
    op.params[3].memref.size   = len;

    op.resultTypes = SSCP_OP_SET_RESULT(kSSCP_ParamType_None);
    op.resultCount = 0u;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_FuseRead, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_get_lifecycle(sss_mgmt_t *context, uint32_t *lifecycleData)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefOutputNoSize, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.resultTypes       = SSCP_OP_SET_RESULT(kSSCP_ParamType_ValueOutputSingle);
    op.resultCount       = 1u;
    op.result[0].value.a = (uint32_t)lifecycleData;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_LifeCycleGet, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_fuse_program(
    sss_mgmt_t *context, uint32_t fuseId, uint32_t *srcData, void *options, size_t *optionsLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    size_t len = (optionsLen != NULL) ? *optionsLen : 0u;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInputSingle,
                                      kSSCP_ParamType_MemrefInputNoSize, kSSCP_ParamType_MemrefInput,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].value.a       = fuseId;
    op.params[2].memref.buffer = srcData;

    op.params[3].memref.buffer = options;
    op.params[3].memref.size   = len;

    op.resultTypes = SSCP_OP_SET_RESULT(kSSCP_ParamType_None);
    op.resultCount = 0u;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_FuseProgram, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_advance_lifecycle(sss_mgmt_t *context, uint32_t *lifecycleData, void *options, size_t *optionsLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    uint32_t data = (lifecycleData != NULL) ? *lifecycleData : 0x0u;
    size_t len    = (optionsLen != NULL) ? *optionsLen : 0u;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_ValueInputSingle,
                                      kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None, kSSCP_ParamType_None,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].value.a = data;

    op.params[2].memref.buffer = options;
    op.params[2].memref.size   = len;

    op.resultTypes = SSCP_OP_SET_RESULT(kSSCP_ParamType_None);
    op.resultCount = 0u;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_AdvanceLifecycle, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_import_secret(sss_mgmt_t *context, const uint8_t *srcData, size_t dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)srcData;
    op.params[1].memref.size   = dataLen;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_SecretImport, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_export_secret(sss_mgmt_t *context, uint8_t *destData, size_t *dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    size_t len = (dataLen != NULL) ? *dataLen : 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)destData;
    op.params[1].memref.size   = len;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_SecretExport, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        if (dataLen)
        {
            *dataLen = 0u;
        }
        return kStatus_SSS_Fail;
    }
    if (dataLen)
    {
        *dataLen = op.params[1].memref.size;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_attest(sss_mgmt_t *context, uint8_t *destData, size_t *dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    size_t len = (dataLen != NULL) ? *dataLen : 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)destData;
    op.params[1].memref.size   = len;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_Attest, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        if (dataLen)
        {
            *dataLen = 0u;
        }
        return kStatus_SSS_Fail;
    }
    if (dataLen)
    {
        *dataLen = op.params[1].memref.size;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_blob_load_secret(sss_mgmt_t *context, const uint8_t *srcData, size_t dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)srcData;
    op.params[1].memref.size   = dataLen;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_SecretBlobLoad, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_blob_export_secret(sss_mgmt_t *context, uint8_t *destData, size_t *dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    size_t len = (dataLen != NULL) ? *dataLen : 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefOutput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)destData;
    op.params[1].memref.size   = len;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_SecretBlobExport, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        if (dataLen)
        {
            *dataLen = 0u;
        }
        return kStatus_SSS_Fail;
    }
    if (dataLen)
    {
        *dataLen = op.params[1].memref.size;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_blob_import_secret(sss_mgmt_t *context, const uint8_t *srcData, size_t dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)srcData;
    op.params[1].memref.size   = dataLen;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_SecretBlobImport, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_get_software_version(sss_mgmt_t *context,
                                           uint32_t *version,
                                           size_t versionWordCount,
                                           uint32_t options)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInOut, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)version;
    op.params[1].memref.size   = versionWordCount;

    op.params[2].value.a = options;

    op.resultTypes = SSCP_OP_SET_RESULT(kSSCP_ParamType_None);
    op.resultCount = 0u;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_SoftwareVersionGet, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_set_software_version(sss_mgmt_t *context,
                                           uint32_t *version,
                                           size_t versionWordCount,
                                           uint32_t options)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes = SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput,
                                      kSSCP_ParamType_ValueInputSingle, kSSCP_ParamType_None, kSSCP_ParamType_None,
                                      kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)version;
    op.params[1].memref.size   = versionWordCount * sizeof(uint32_t);

    op.params[2].value.a = options;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_SoftwareVersionGet, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_set_return_fa(sss_mgmt_t *context,
                                    const uint8_t *request,
                                    size_t requestSize,
                                    void *options,
                                    size_t *optionsLen,
                                    uint32_t *resultState)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    size_t len = (optionsLen != NULL) ? *optionsLen : 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_MemrefInput,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)request;
    op.params[1].memref.size   = requestSize;

    op.params[2].memref.buffer = options;
    op.params[2].memref.size   = len;

    op.resultTypes       = SSCP_OP_SET_RESULT(kSSCP_ParamType_ValueOutputSingle);
    op.resultCount       = 0u;
    op.result[0].value.a = (uint32_t)resultState;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_ReturnFaSet, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_set_host_access_permission(sss_mgmt_t *context, const uint8_t *srcData, size_t dataLen)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_MemrefInput, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.params[1].memref.buffer = (void *)srcData;
    op.params[1].memref.size   = dataLen;

    op.resultTypes = SSCP_OP_SET_RESULT(kSSCP_ParamType_None);
    op.resultCount = 0u;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_HostAccessPermissionSet, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_integrity_check_enable(sss_mgmt_t *context)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_None, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_MGMT_IntegrityCheckEnable, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_ping(sss_mgmt_t *context)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.resultTypes = SSCP_OP_SET_RESULT(kSSCP_ParamType_None);
    op.resultCount = 0u;

    sscp_context_t *sscpCtx = ((sss_sscp_session_t *)context->session)->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_Ping, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}

sss_status_t sss_mgmt_context_free(sss_mgmt_t *context)
{
    sscp_operation_t op  = {0};
    sscp_status_t status = kStatus_SSCP_Fail;
    uint32_t ret         = 0u;

    op.paramTypes =
        SSCP_OP_SET_PARAM(kSSCP_ParamType_ContextReference, kSSCP_ParamType_None, kSSCP_ParamType_None,
                          kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None, kSSCP_ParamType_None);

    op.params[0].context.ptr  = context;
    op.params[0].context.type = kSSCP_ParamContextType_SSS_Mgmt;

    op.resultTypes = SSCP_OP_SET_RESULT(kSSCP_ParamType_None);
    op.resultCount = 0u;

    sscp_context_t *sscpCtx = context->session->sscp;
    status                  = sscpCtx->invoke(sscpCtx, kSSCP_CMD_SSS_ContextFree, &op, &ret);
    if (status != kStatus_SSCP_Success)
    {
        return kStatus_SSS_Fail;
    }
    return (sss_status_t)ret;
}