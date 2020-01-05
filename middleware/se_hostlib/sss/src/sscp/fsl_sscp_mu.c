/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sscp_mu.h"
// #include "fsl_sscp_mu.h"
#define MU_Init (void)
#define MU_Deinit (void)

uint32_t MU_ReceiveMsg(MU_Type *base, uint32_t msg[16])
{
    return 0;
}

void MU_SendMsg(MU_Type *base, uint32_t msg[16], uint32_t wordNum)
{
    return;
}

sscp_status_t sscp_mu_init(sscp_context_t *context, MU_Type *base)
{
    sscp_mu_context_t *muContext = (sscp_mu_context_t *)(uintptr_t)context;

    muContext->base = base;
    MU_Init(base);

    /* assign MU implementation of ::sscp_invoke_command() */
    muContext->invoke = sscp_mu_invoke_command;
    return kStatus_SSCP_Success;
}

void sscp_mu_deinit(sscp_context_t *context)
{
    sscp_mu_context_t *muContext = (sscp_mu_context_t *)(uintptr_t)context;

    MU_Deinit(muContext->base);
}

sscp_status_t sscp_mu_invoke_command(sscp_context_t *context,
    uint32_t commandID,
    sscp_operation_t *op,
    uint32_t *ret)
{
    sscp_mu_context_t *muContext = (sscp_mu_context_t *)(uintptr_t)context;

    /* parse the operation to create message */
    uint32_t msg[16] = {0};
    int i;
    msg[0] = commandID;
    msg[1] = op->paramTypes;
    int wrIdx = 2;

    bool done = false;
    for (i = 0; !done && (i < SSCP_OPERATION_PARAM_COUNT); i++) {
        switch (SSCP_OP_GET_PARAM(i, op->paramTypes)) {
        case kSSCP_ParamType_ContextReference:
            msg[wrIdx++] = (uint32_t)(op->params[i].context.ptr);
            break;

        case kSSCP_ParamType_Aggregate:
            break;

        case kSSCP_ParamType_MemrefInput:
        case kSSCP_ParamType_MemrefInOut:
            msg[wrIdx++] = (uint32_t)(op->params[i].memref.buffer);
            msg[wrIdx++] = op->params[i].memref.size;
            break;

        case kSSCP_ParamType_MemrefOutput:
            msg[wrIdx++] = (uint32_t)(op->params[i].memref.buffer);
            msg[wrIdx++] = (uint32_t) & (op->params[i].memref.size);
            break;

        case kSSCP_ParamType_ValueInput:
            msg[wrIdx++] = op->params[i].value.a;
            msg[wrIdx++] = op->params[i].value.b;
            break;

        case kSSCP_ParamType_None:
            done = true; /* break the for loop */
            break;

        default:
            break;
        }

        if (wrIdx >= 16) {
            break;
        }
    }

    MU_SendMsg(muContext->base, msg, wrIdx);

    /* poll for response */
    MU_ReceiveMsg(muContext->base, msg);
    *ret = msg[1];

    return kStatus_SSCP_Success;
}
