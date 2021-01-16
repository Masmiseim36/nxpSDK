/*
 * Copyright 2018-2020 NXP
 *
 * License: NXP LA_OPT_NXP_Software_License
 *
 * NXP Confidential. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.  This code may only be used in a microprocessor,
 * microcontroller, sensor or digital signal processor ("NXP Product")
 * supplied directly or indirectly from NXP.  See the full NXP Software
 * License Agreement in license/LA_OPT_NXP_Software_License.pdf
 *
 * FreeMASTER Communication Driver - protocol implementation
 */

/*
 * This code is based on original implementation by Brad Conte. Extended to
 * support big-endian platforms for FreeMASTER driver needs.
 * Origin: https://github.com/B-Con/crypto-algorithms
 *
 * Author:     Brad Conte (brad AT bradconte.com)
 * Copyright:
 * Disclaimer: This code is presented "as is" without any guarantees.
 * Details:    Implementation of the SHA1 hashing algorithm.
               Algorithm specification can be found here:
                * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
               This implementation uses little endian byte order.
 */

#include "freemaster.h"
#include "freemaster_private.h"

#if FMSTR_DISABLE == 0

/*****************************************************************************
 * Local Functions
 ******************************************************************************/

#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))

static void FMSTR_Sha1Transform(FMSTR_SHA1_CTX *ctx, const FMSTR_U8 *data)
{
    // save 352 bytes on stack by making these static, we never run more instances than one anyway
    static FMSTR_U32 a, b, c, d, e, i, j, t, m[80];

    for (i = 0; i < 16U; i++)
    {
        j = i * 4U;

        a = ((FMSTR_U32)data[j]) << 24;
        a += ((FMSTR_U32)data[j + 1U]) << 16;
        a += ((FMSTR_U32)data[j + 2U]) << 8;
        a += data[j + 3U];
        m[i] = a;
    }

    for (; i < 80U; i++)
    {
        m[i] = (m[i - 3U] ^ m[i - 8U] ^ m[i - 14U] ^ m[i - 16U]);
        m[i] = (m[i] << 1) | (m[i] >> 31);
    }

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];

    for (i = 0; i < 20U; i++)
    {
        t = ROTLEFT(a, 5) + ((b & c) ^ (~b & d)) + e + ctx->k[0] + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
    }

    for (; i < 40U; i++)
    {
        t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + ctx->k[1] + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
    }

    for (; i < 60U; i++)
    {
        t = ROTLEFT(a, 5) + ((b & c) ^ (b & d) ^ (c & d)) + e + ctx->k[2] + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
    }

    for (; i < 80U; i++)
    {
        t = ROTLEFT(a, 5) + (b ^ c ^ d) + e + ctx->k[3] + m[i];
        e = d;
        d = c;
        c = ROTLEFT(b, 30);
        b = a;
        a = t;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
}

/*****************************************************************************
 * API Functions
 ******************************************************************************/

void FMSTR_Sha1Init(FMSTR_SHA1_CTX *ctx)
{
    ctx->datalen = 0U;
    ctx->bitlen  = 0U;

    ctx->state[0] = 0x67452301UL;
    ctx->state[1] = 0xEFCDAB89UL;
    ctx->state[2] = 0x98BADCFEUL;
    ctx->state[3] = 0x10325476UL;
    ctx->state[4] = 0xc3d2e1f0UL;

    ctx->k[0] = 0x5a827999UL;
    ctx->k[1] = 0x6ed9eba1UL;
    ctx->k[2] = 0x8f1bbcdcUL;
    ctx->k[3] = 0xca62c1d6UL;
}

void FMSTR_Sha1Update(FMSTR_SHA1_CTX *ctx, const FMSTR_U8 *data, FMSTR_SIZE len)
{
    FMSTR_SIZE i;

    for (i = 0U; i < len; i++)
    {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64U)
        {
            FMSTR_Sha1Transform(ctx, ctx->data);
            ctx->bitlen += 512U;
            ctx->datalen = 0U;
        }
    }
}

void FMSTR_Sha1Final(FMSTR_SHA1_CTX *ctx, FMSTR_U8 *hash)
{
    FMSTR_U32 i;

    i = ctx->datalen;

    // Pad whatever data is left in the buffer.
    if (ctx->datalen < 56U)
    {
        ctx->data[i++] = 0x80U;
        while (i < 56U)
        {
            ctx->data[i++] = 0x00U;
        }
    }
    else
    {
        ctx->data[i++] = 0x80U;
        while (i < 64U)
        {
            ctx->data[i++] = 0x00U;
        }
        FMSTR_Sha1Transform(ctx, ctx->data);
        FMSTR_MemSet(ctx->data, 0U, 56U);
    }

    // Append to the padding the total message's length in bits and transform.
    i = ctx->datalen * 8U;
    ctx->bitlen += (FMSTR_U64)i;
    ctx->data[63] = (FMSTR_U8)(ctx->bitlen);
    ctx->data[62] = (FMSTR_U8)(ctx->bitlen >> 8);
    ctx->data[61] = (FMSTR_U8)(ctx->bitlen >> 16);
    ctx->data[60] = (FMSTR_U8)(ctx->bitlen >> 24);
    ctx->data[59] = (FMSTR_U8)(ctx->bitlen >> 32);
    ctx->data[58] = (FMSTR_U8)(ctx->bitlen >> 40);
    ctx->data[57] = (FMSTR_U8)(ctx->bitlen >> 48);
    ctx->data[56] = (FMSTR_U8)(ctx->bitlen >> 56);
    FMSTR_Sha1Transform(ctx, ctx->data);

#if FMSTR_PLATFORM_BIG_ENDIAN > 0
    /* copy state to output buffer */
    for (i = 0U; i < 5U; i++)
    {
        *(FMSTR_U32 *)(hash + i * 4U) = ctx->state[i];
    }
#else
    /* copy while reversing byte order */
    for (i = 0U; i < 4U; i++)
    {
        FMSTR_SIZE shift = 24U - i * 8U;
        hash[i]          = (FMSTR_U8)((ctx->state[0] >> shift) & 0xffU);
        hash[i + 4U]     = (FMSTR_U8)((ctx->state[1] >> shift) & 0xffU);
        hash[i + 8U]     = (FMSTR_U8)((ctx->state[2] >> shift) & 0xffU);
        hash[i + 12U]    = (FMSTR_U8)((ctx->state[3] >> shift) & 0xffU);
        hash[i + 16U]    = (FMSTR_U8)((ctx->state[4] >> shift) & 0xffU);
    }
#endif
}

#endif /* !FMSTR_DISABLE */
