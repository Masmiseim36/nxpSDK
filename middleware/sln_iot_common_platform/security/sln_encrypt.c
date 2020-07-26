/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "FreeRTOS.h"

#include "sln_encrypt.h"

#define SLN_ENCRYPT_SLOTS 4

static const void *s_SNL_EncryptCtx[SLN_ENCRYPT_SLOTS] = {0};
static uint16_t s_dcpUsers                             = 0;

static void SLN_Encrypt_Attach_Key(const void *ctx, uint8_t keySlot)
{
    s_SNL_EncryptCtx[keySlot] = ctx;
}

static void SLN_Encrypt_Detach_Key(const void *ctx)
{
    for (int32_t i = 0; i < SLN_ENCRYPT_SLOTS; i++)
    {
        if (ctx == s_SNL_EncryptCtx[i])
        {
            s_SNL_EncryptCtx[i] = NULL;
            break;
        }
    }
}

static bool SLN_Encrypt_Key_Loaded(const void *ctx)
{
    bool ret = false;

    for (int32_t i = 0; i < SLN_ENCRYPT_SLOTS; i++)
    {
        if (ctx == s_SNL_EncryptCtx[i])
        {
            ret = true;
            break;
        }
    }

    return ret;
}

static bool SLN_Encrypt_KeySlot_Busy(uint8_t keySlot)
{
    const sln_encrypt_ctx_t *ctx;

    for (int32_t i = 0; i < SLN_ENCRYPT_SLOTS; i++)
    {
        ctx = s_SNL_EncryptCtx[i];

        if (ctx != NULL && keySlot == ctx->handle.keySlot)
        {
            return true;
        }
    }

    return false;
}

int32_t SLN_Encrypt_Init_Slot(sln_encrypt_ctx_t *ctx)
{
    if (ctx == NULL)
    {
        return SLN_ENCRYPT_NULL_CTX;
    }

    if (ctx->keySize != 16 && ctx->keySize != 24 && ctx->keySize != 32)
    {
        return SLN_ENCRYPT_INVALID_KEY;
    }

    if (ctx->handle.keySlot != kDCP_OtpKey && ctx->handle.keySlot != kDCP_OtpUniqueKey &&
        ctx->handle.keySlot != kDCP_PayloadKey)
    {
        if (SLN_Encrypt_KeySlot_Busy(ctx->handle.keySlot))
        {
            return SLN_ENCRYPT_KEYSLOT_BUSY;
        }
    }

    /*
     * Note: in case DCP_Init() will not be called from CRYPTO_InitHardware(), we
     * can call it from here when s_dcpUsers is 0.
     */

    s_dcpUsers++;

    return SLN_ENCRYPT_STATUS_OK;
}

int32_t SLN_Encrypt_Deinit_Slot(sln_encrypt_ctx_t *ctx)
{
    if (s_dcpUsers == 0)
    {
        return SLN_ENCRYPT_STATUS_OK;
    }

    if (ctx == NULL)
    {
        return SLN_ENCRYPT_NULL_CTX;
    }

    if (SLN_Encrypt_Key_Loaded(ctx))
    {
        SLN_Encrypt_Detach_Key(ctx);
    }

    s_dcpUsers--;

    return SLN_ENCRYPT_STATUS_OK;
}

int32_t SLN_Encrypt_AES_CBC_PKCS7(
    sln_encrypt_ctx_t *ctx, const uint8_t *in, size_t inSize, uint8_t *out, size_t outSize)
{
    uint32_t keySize;
    uint8_t *iv, *encKey;
    dcp_handle_t *handle;
    uint32_t alignedSize, lastSize;
    int32_t ret = SLN_ENCRYPT_STATUS_OK;

    iv      = (uint8_t *)ctx->iv;
    encKey  = (uint8_t *)ctx->key;
    handle  = &ctx->handle;
    keySize = ctx->keySize;

    if (outSize % AES_BLOCK_SIZE)
    {
        ret = SLN_ENCRYPT_WRONG_OUT_BUFSIZE;
        goto exit;
    }

    if (!SLN_Encrypt_Key_Loaded(ctx))
    {
        DCP_AES_SetKey(DCP, handle, encKey, keySize);
        SLN_Encrypt_Attach_Key(ctx, handle->keySlot);
    }

    alignedSize = (inSize / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    lastSize    = inSize % AES_BLOCK_SIZE;

    /* Encrypt the 16-byte aligned part first */
    if (alignedSize)
    {
        ret = DCP_AES_EncryptCbc(DCP, handle, in, out, alignedSize, iv);
        if (SLN_ENCRYPT_STATUS_OK != ret)
        {
            ret = SLN_ENCRYPT_DCP_ENCRYPT_ERROR_1;
            goto exit;
        }
    }

    /* And the rest copy in a 16-byte buffer */
    if (lastSize)
    {
        uint8_t pad = AES_BLOCK_SIZE - lastSize;
        uint8_t alignedInBuf[AES_BLOCK_SIZE];

        memset(alignedInBuf, 0, AES_BLOCK_SIZE);
        memcpy(alignedInBuf, in + alignedSize, lastSize);

        /* Implement PKCS#7 padding so we can recover plain text length later */
        if (pad)
        {
            memset(alignedInBuf + lastSize, pad, pad);
        }

        ret = DCP_AES_EncryptCbc(DCP, handle, alignedInBuf, out + alignedSize, AES_BLOCK_SIZE, iv);
        if (SLN_ENCRYPT_STATUS_OK != ret)
        {
            ret = SLN_ENCRYPT_DCP_ENCRYPT_ERROR_2;
            goto exit;
        }
    }

exit:
    return ret;
}

int32_t SLN_Decrypt_AES_CBC_PKCS7(
    sln_encrypt_ctx_t *ctx, const uint8_t *in, size_t inSize, uint8_t *out, size_t *outSize)
{
    uint32_t keySize = 0;
    ;
    uint8_t *iv     = NULL;
    uint8_t *decKey = NULL;
    dcp_handle_t *handle;
    uint32_t alignedSize = 0;
    uint32_t lastSize    = 0;
    int32_t ret          = SLN_ENCRYPT_STATUS_OK;
    uint8_t pad          = 0;

    iv      = (uint8_t *)ctx->iv;
    decKey  = (uint8_t *)ctx->key;
    handle  = &ctx->handle;
    keySize = ctx->keySize;

    if (inSize % AES_BLOCK_SIZE)
    {
        ret = SLN_ENCRYPT_WRONG_IN_BUFSIZE;
        goto exit;
    }

    if (!SLN_Encrypt_Key_Loaded(ctx))
    {
        DCP_AES_SetKey(DCP, handle, decKey, keySize);
        SLN_Encrypt_Attach_Key(ctx, handle->keySlot);
    }

    alignedSize = (inSize > AES_BLOCK_SIZE) ? inSize - AES_BLOCK_SIZE : 0;
    lastSize    = AES_BLOCK_SIZE;

    /* Decrypt the 16-byte aligned part first */
    if (alignedSize)
    {
        ret = DCP_AES_DecryptCbc(DCP, handle, in, out, alignedSize, iv);
        if (SLN_ENCRYPT_STATUS_OK != ret)
        {
            ret = SLN_ENCRYPT_DCP_DECRYPT_ERROR_1;
            goto exit;
        }
    }

    /* And the rest copy in a 16-byte buffer */
    uint8_t alignedOutBuf[AES_BLOCK_SIZE];

    ret = DCP_AES_DecryptCbc(DCP, handle, in + alignedSize, alignedOutBuf, AES_BLOCK_SIZE, iv);
    if (SLN_ENCRYPT_STATUS_OK != ret)
    {
        ret = SLN_ENCRYPT_DCP_DECRYPT_ERROR_2;
        goto exit;
    }

    /* Work out plain length from PKCS#7 padding */
    pad = alignedOutBuf[AES_BLOCK_SIZE - 1];

    if (pad < AES_BLOCK_SIZE)
    {
        /* Verify padding makes sense */
        uint32_t startIdx = AES_BLOCK_SIZE - 1;
        uint32_t endIdx   = startIdx - pad;
        for (uint32_t idx = startIdx; idx > endIdx; idx--)
        {
            if (alignedOutBuf[idx] != pad)
            {
                // Not valid padding, assume input lengt is output length
                pad = 0;
                break;
            }
        }

        lastSize -= pad;
    }

    /* Copy final data into output buffer */
    memcpy(out + alignedSize, alignedOutBuf, lastSize);

    /* Update output size */
    *outSize = alignedSize + lastSize;

exit:
    return SLN_ENCRYPT_STATUS_OK;
}

int32_t SLN_Crc(sln_encrypt_ctx_t *ctx, const uint8_t *in, size_t inSize, uint32_t *out, size_t *outSize)
{
    dcp_handle_t *handle;
    int32_t ret     = SLN_ENCRYPT_STATUS_OK;
    status_t status = kStatus_Success;

    if ((NULL == ctx))
    {
        ret = SLN_ENCRYPT_NULL_CTX;
        goto exit;
    }

    handle = &ctx->handle;

    if (NULL == handle)
    {
        ret = SLN_ENCRYPT_NULL_HANDLE;
        goto exit;
    }

    if ((NULL == in) || (NULL == out) || (NULL == outSize))
    {
        ret = SLN_ENCRYPT_NULL_PARAM;
        goto exit;
    }

    status = DCP_HASH(DCP, handle, kDCP_Crc32, in, inSize, (uint8_t *)out, outSize);

    if (kStatus_Success != status)
    {
        ret = SLN_ENCRYPT_DCP_CRC_ERROR;
    }

exit:
    return ret;
}
