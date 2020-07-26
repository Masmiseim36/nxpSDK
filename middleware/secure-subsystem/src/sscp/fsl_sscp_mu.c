/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdint.h>

#include "fsl_sscp_mu.h"
#include "fsl_sss_sscp.h"
#include "fsl_sss_mgmt.h"

#define MU_Deinit (void)
#define SENTINEL_SUCCESS ((uint8_t)0x3C)
#define SENTINEL_FAIL ((uint8_t)0xC3)

void MU_Init(void)
{
    SNT_mu_init();
}

sscp_status_t MU_ReceiveMsg(MU_Type *base, uint32_t msg[MU_RR_COUNT], size_t wordNum)
{
    sscp_status_t ret = kStatus_SSCP_Fail;
    if (SNT_mu_get_response(msg, wordNum) == MU_SUCCESS_RESULT)
    {
      ret = kStatus_SSCP_Success;
    }
    return ret;
}

sscp_status_t MU_SendMsg(MU_Type *base, uint32_t msg[MU_TR_COUNT], size_t wordNum)
{
    sscp_status_t ret = kStatus_SSCP_Fail;
    if (SNT_mu_send_message(msg, wordNum) == MU_SUCCESS_RESULT)
    {
        ret = kStatus_SSCP_Success;
    }
    return ret;
}

sscp_status_t sscp_mu_init(sscp_context_t *context, MU_Type *base)
{
    sscp_mu_context_t *muContext = (sscp_mu_context_t *)(uintptr_t)context;

    muContext->base = base;
    MU_Init();

    /* assign MU implementation of ::sscp_invoke_command() */
    muContext->invoke = sscp_mu_invoke_command;
    return kStatus_SSCP_Success;
}

void sscp_mu_deinit(sscp_context_t *context)
{
    /*sscp_mu_context_t *muContext = (sscp_mu_context_t *)(uintptr_t)context;

    MU_Deinit(muContext->base);*/
}

sscp_status_t sscp_mu_invoke_command(sscp_context_t *context,
                                     sscp_command_t commandID,
                                     sscp_operation_t *op,
                                     uint32_t *ret)
{
    sscp_mu_context_t *muContext = (sscp_mu_context_t *)(uintptr_t)context;
    /* parse the operation to create message */
    uint32_t msg[MU_TR_COUNT] = {0};
    uint32_t wrIdx            = 1u;
    bool done                 = false;
    for (uint32_t i = 0u; (!done) && (i < SSCP_OPERATION_PARAM_COUNT); i++)
    {
        switch (SSCP_OP_GET_PARAM(i, op->paramTypes))
        {
            case kSSCP_ParamType_ContextReference:
                switch (op->params[i].context.type)
                {
                    case kSSCP_ParamContextType_SSS_Session:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_session_t *)(op->params[i].context.ptr))->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Symmetric:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_symmetric_t *)(op->params[i].context.ptr))->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Aead:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_aead_t *)(op->params[i].context.ptr))->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Digest:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_digest_t *)op->params[i].context.ptr)->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Asymmetric:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_asymmetric_t *)op->params[i].context.ptr)->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Object:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_object_t *)op->params[i].context.ptr)->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_KeyStoreCtx:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_key_store_t *)op->params[i].context.ptr)->keyStoreCtx;
                        break;
                    case kSSCP_ParamContextType_SSS_KeyStore:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_key_store_t *)op->params[i].context.ptr)->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_DeriveKey:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_derive_key_t *)op->params[i].context.ptr)->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Mgmt:
                        msg[wrIdx++] = (uint32_t)((sss_mgmt_t *)op->params[i].context.ptr)->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Rng:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_rng_t *)op->params[i].context.ptr)->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Mac:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_mac_t *)op->params[i].context.ptr)->ctx;
                        break;
                    case kSSCP_ParamContextType_SSS_Tunnel:
                        msg[wrIdx++] = (uint32_t)((sss_sscp_tunnel_t *)op->params[i].context.ptr)->ctx;
                        break;
                    default:
                        break;
                }
                break;

            case kSSCP_ParamType_Aggregate:
                break;

            case kSSCP_ParamType_MemrefInput:
            case kSSCP_ParamType_MemrefInOut:
                msg[wrIdx++] = (uint32_t)(op->params[i].memref.buffer);
                msg[wrIdx++] = op->params[i].memref.size;
                break;

            case kSSCP_ParamType_MemrefOutputNoSize:
            case kSSCP_ParamType_MemrefInputNoSize:
                msg[wrIdx++] = (uint32_t)(op->params[i].memref.buffer);
                break;

            case kSSCP_ParamType_MemrefOutput:
                msg[wrIdx++] = (uint32_t)(op->params[i].memref.buffer);
                msg[wrIdx++] = (uint32_t)(op->params[i].memref.size);
                break;

            case kSSCP_ParamType_ValueInputTuple:
                msg[wrIdx++] = op->params[i].value.a;
                msg[wrIdx++] = op->params[i].value.b;
                break;

            case kSSCP_ParamType_ValueInputSingle:
                msg[wrIdx++] = op->params[i].value.a;
                break;

            case kSSCP_ParamType_None:
                done = true; /* break the for loop */
                break;

            default:
                break;
        }
        if (wrIdx >= MU_TR_COUNT - MU_MSG_HEADER_SIZE)
        {
            break;
        }
    }

    mu_hdr_t muMsgHeader;
    muMsgHeader.check_bits = STATIC_CHECK_BITS;
    muMsgHeader.tag_sts    = MESSAGING_TAG_COMMAND;
    muMsgHeader.command    = commandID;
    muMsgHeader.size       = (uint8_t)(wrIdx - MU_MSG_HEADER_SIZE);
    msg[0]                 = (uint32_t)(*((uint32_t *)(&muMsgHeader)));

    if (MU_SendMsg(muContext->base, msg, wrIdx) != kStatus_SSCP_Success)
    {
        return kStatus_SSCP_Fail;
    }

    /* poll for response */
    if (MU_ReceiveMsg(muContext->base, msg, MU_RR_COUNT) != kStatus_SSCP_Success)
    {
        return kStatus_SSCP_Fail;
    }
    mu_hdr_t *muReplyHeader = (mu_hdr_t *)&msg[0];
    if (muReplyHeader->size != op->resultCount)
    {
        return kStatus_SSCP_Fail;
    }
    if (muReplyHeader->command != commandID)
    {
        return kStatus_SSCP_Fail;
    }
    for (uint32_t i = 1u; i <= op->resultCount; i++)
    {
      uint32_t k = i - 1u;
        switch (SSCP_OP_GET_PARAM(k, op->resultTypes))
        {
            case kSSCP_ParamType_ContextReference:
                switch (op->result[k].context.type)
                {
                    case kSSCP_ParamContextType_SSS_Session:
                        ((sss_sscp_session_t *)(op->result[k].context.ptr))->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_Symmetric:
                        ((sss_sscp_symmetric_t *)(op->result[k].context.ptr))->ctx = msg[i];
                        break;                        
                    case kSSCP_ParamContextType_SSS_Aead:
                        ((sss_sscp_aead_t *)(op->result[k].context.ptr))->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_Digest:
                        ((sss_sscp_digest_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_Asymmetric:
                        ((sss_sscp_asymmetric_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_Object:
                        ((sss_sscp_object_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_KeyStoreCtx:
                        ((sss_sscp_key_store_t *)op->result[k].context.ptr)->keyStoreCtx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_KeyStore:
                        ((sss_sscp_key_store_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_DeriveKey:
                        ((sss_sscp_derive_key_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;    
                    case kSSCP_ParamContextType_SSS_Mgmt:
                        ((sss_mgmt_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_Rng:
                        ((sss_sscp_rng_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_Mac:
                        ((sss_sscp_mac_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;
                    case kSSCP_ParamContextType_SSS_Tunnel:
                        ((sss_sscp_tunnel_t *)op->result[k].context.ptr)->ctx = msg[i];
                        break;
                    default:
                        break;
                }
                break;
            case kSSCP_ParamType_ValueOutputSingle:
                *((uint32_t *)(op->result[k].value.a)) = msg[i];
                break;
            default:
                break;
        }
    }

    if (muReplyHeader->tag_sts == SENTINEL_SUCCESS)
    {
        *ret = kStatus_SSS_Success;
    }
    else
    {
        return kStatus_SSCP_Fail;
    }

    return kStatus_SSCP_Success;
}
