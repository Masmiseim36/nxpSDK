/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"

#if defined(MBEDTLS_AES_C)

#include <string.h>

#include "mbedtls/aes.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/platform.h"
#include "mbedtls/error.h"
#if defined(MBEDTLS_PADLOCK_C)
#include "mbedtls/padlock.h"
#endif
#if defined(MBEDTLS_AESNI_C)
#include "mbedtls/aesni.h"
#endif

#if defined(MBEDTLS_AES_ALT)
#include "aes_alt.h"
#include "ele_crypto.h"
#include "ele_mbedtls.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

/* Parameter validation macros based on platform_util.h */
#define AES_VALIDATE_RET(cond)    \
    MBEDTLS_INTERNAL_VALIDATE_RET(cond, MBEDTLS_ERR_AES_BAD_INPUT_DATA)
#define AES_VALIDATE(cond)        \
    MBEDTLS_INTERNAL_VALIDATE(cond)

void mbedtls_aes_init(mbedtls_aes_context *ctx)
{
    AES_VALIDATE(ctx != NULL);

    memset(ctx, 0, sizeof(mbedtls_aes_context));
}

void mbedtls_aes_free(mbedtls_aes_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    mbedtls_platform_zeroize(ctx, sizeof(mbedtls_aes_context));
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)
void mbedtls_aes_xts_init(mbedtls_aes_xts_context *ctx)
{
    AES_VALIDATE(ctx != NULL);

    mbedtls_aes_init(&ctx->crypt);
    mbedtls_aes_init(&ctx->tweak);
}

void mbedtls_aes_xts_free(mbedtls_aes_xts_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    mbedtls_aes_free(&ctx->crypt);
    mbedtls_aes_free(&ctx->tweak);
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(key != NULL);

    switch (keybits) {
        case 128:
            ctx->keySize = 16;
            break;
        case 192:
            ctx->keySize = 24;
            break;
        case 256:
            ctx->keySize = 32;
            break;
        default:
            return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    memcpy(ctx->key, key, ctx->keySize);

    return 0;
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    return mbedtls_aes_setkey_enc(ctx, key, keybits);
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)
static int mbedtls_aes_xts_decode_keys(const unsigned char *key,
                                       unsigned int keybits,
                                       const unsigned char **key1,
                                       unsigned int *key1bits,
                                       const unsigned char **key2,
                                       unsigned int *key2bits)
{
    const unsigned int half_keybits = keybits / 2;
    const unsigned int half_keybytes = half_keybits / 8;

    switch (keybits) {
        case 256: break;
        case 512: break;
        default: return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    *key1bits = half_keybits;
    *key2bits = half_keybits;
    *key1 = &key[0];
    *key2 = &key[half_keybytes];

    return 0;
}

int mbedtls_aes_xts_setkey_enc(mbedtls_aes_xts_context *ctx,
                               const unsigned char *key,
                               unsigned int keybits)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const unsigned char *key1, *key2;
    unsigned int key1bits, key2bits;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(key != NULL);

    ret = mbedtls_aes_xts_decode_keys(key, keybits, &key1, &key1bits,
                                      &key2, &key2bits);
    if (ret != 0) {
        return ret;
    }

    /* Set the tweak key. Always set tweak key for the encryption mode. */
    ret = mbedtls_aes_setkey_enc(&ctx->tweak, key2, key2bits);
    if (ret != 0) {
        return ret;
    }

    /* Set crypt key for encryption. */
    return mbedtls_aes_setkey_enc(&ctx->crypt, key1, key1bits);
}

int mbedtls_aes_xts_setkey_dec(mbedtls_aes_xts_context *ctx,
                               const unsigned char *key,
                               unsigned int keybits)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const unsigned char *key1, *key2;
    unsigned int key1bits, key2bits;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(key != NULL);

    ret = mbedtls_aes_xts_decode_keys(key, keybits, &key1, &key1bits,
                                      &key2, &key2bits);
    if (ret != 0) {
        return ret;
    }

    /* Set the tweak key. Always set tweak key for encryption. */
    ret = mbedtls_aes_setkey_enc(&ctx->tweak, key2, key2bits);
    if (ret != 0) {
        return ret;
    }

    /* Set crypt key for decryption. */
    return mbedtls_aes_setkey_dec(&ctx->crypt, key1, key1bits);
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * AES-ECB block encryption
 */
int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{

    ele_generic_cipher_t GenericCipherECB;
    GenericCipherECB.data     = (uint32_t) input;
    GenericCipherECB.output   = (uint32_t) output;
    GenericCipherECB.size     = 16u;
    GenericCipherECB.key      = (uint32_t) ctx->key;
    GenericCipherECB.key_size = ctx->keySize;
    GenericCipherECB.iv       = (uint32_t) NULL;
    GenericCipherECB.iv_size  = 0;
    GenericCipherECB.mode     = kEncrypt;
    GenericCipherECB.algo     = kAES_ECB;
    int ret = MBEDTLS_ERR_AES_HW_ACCEL_FAILED;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if (ELE_GenericCipher(S3MU, &GenericCipherECB) != kStatus_Success) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    } else {
        ret = 0;
    }

exit:
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}


/*
 * AES-ECB block decryption
 */
int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx,
                                 const unsigned char input[16],
                                 unsigned char output[16])
{

    ele_generic_cipher_t GenericCipherECB;
    GenericCipherECB.data     = (uint32_t) input;
    GenericCipherECB.output   = (uint32_t) output;
    GenericCipherECB.size     = 16u;
    GenericCipherECB.key      = (uint32_t) ctx->key;
    GenericCipherECB.key_size = ctx->keySize;
    GenericCipherECB.iv       = (uint32_t) NULL;
    GenericCipherECB.iv_size  = 0;
    GenericCipherECB.mode     = kDecrypt;
    GenericCipherECB.algo     = kAES_ECB;
    int ret = MBEDTLS_ERR_AES_HW_ACCEL_FAILED;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if (ELE_GenericCipher(S3MU, &GenericCipherECB) != kStatus_Success) {
        ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        goto exit;
    } else {
        ret = 0;
    }

exit:
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb(mbedtls_aes_context *ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{
    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);

    if (mode == MBEDTLS_AES_ENCRYPT) {
        return mbedtls_internal_aes_encrypt(ctx, input, output);
    } else {
        return mbedtls_internal_aes_decrypt(ctx, input, output);
    }
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT || mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    if (length % 16) {
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;
    }

    ele_generic_cipher_t GenericCipherCBC;
    GenericCipherCBC.data     = (uint32_t) input;
    GenericCipherCBC.output   = (uint32_t) output;
    GenericCipherCBC.size     = (uint32_t) length;
    GenericCipherCBC.key      = (uint32_t) ctx->key;
    GenericCipherCBC.key_size = ctx->keySize;
    GenericCipherCBC.iv       = (uint32_t) iv;
    GenericCipherCBC.iv_size  = 16u;
    GenericCipherCBC.algo     = kAES_CBC;
    int ret = MBEDTLS_ERR_AES_HW_ACCEL_FAILED;

#if defined(MBEDTLS_THREADING_C)
    if ((ret = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0) {
        return ret;
    }
#endif

    if (mode == MBEDTLS_AES_ENCRYPT) {
        GenericCipherCBC.mode = kEncrypt;

        if (ELE_GenericCipher(S3MU, &GenericCipherCBC) != kStatus_Success) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        } else {
            ret = 0;
        }

        /* Save updated IV */
        memcpy(iv, output + length - 16, 16);
    } else if (mode == MBEDTLS_AES_DECRYPT) {
        GenericCipherCBC.mode = kDecrypt;

        /* Store IV in case output is same address as input */
        uint8_t tmp[16];
        memcpy(tmp, input + length - 16, 16);

        if (ELE_GenericCipher(S3MU, &GenericCipherCBC) != kStatus_Success) {
            ret = MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
            goto exit;
        } else {
            ret = 0;
        }

        /* Save updated IV */
        memcpy(iv, tmp, 16);

    }

exit:
#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0) {
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
    }
#endif
    return ret;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_XTS)

/* Endianess with 64 bits values */
#ifndef GET_UINT64_LE
#define GET_UINT64_LE(n, b, i)                            \
    {                                                       \
        (n) = ((uint64_t) (b)[(i) + 7] << 56)             \
              | ((uint64_t) (b)[(i) + 6] << 48)             \
              | ((uint64_t) (b)[(i) + 5] << 40)             \
              | ((uint64_t) (b)[(i) + 4] << 32)             \
              | ((uint64_t) (b)[(i) + 3] << 24)             \
              | ((uint64_t) (b)[(i) + 2] << 16)             \
              | ((uint64_t) (b)[(i) + 1] <<  8)             \
              | ((uint64_t) (b)[(i)]);            \
    }
#endif

#ifndef PUT_UINT64_LE
#define PUT_UINT64_LE(n, b, i)                            \
    {                                                       \
        (b)[(i) + 7] = (unsigned char) ((n) >> 56);       \
        (b)[(i) + 6] = (unsigned char) ((n) >> 48);       \
        (b)[(i) + 5] = (unsigned char) ((n) >> 40);       \
        (b)[(i) + 4] = (unsigned char) ((n) >> 32);       \
        (b)[(i) + 3] = (unsigned char) ((n) >> 24);       \
        (b)[(i) + 2] = (unsigned char) ((n) >> 16);       \
        (b)[(i) + 1] = (unsigned char) ((n) >>  8);       \
        (b)[(i)] = (unsigned char) ((n));       \
    }
#endif

typedef unsigned char mbedtls_be128[16];

/*
 * GF(2^128) multiplication function
 *
 * This function multiplies a field element by x in the polynomial field
 * representation. It uses 64-bit word operations to gain speed but compensates
 * for machine endianess and hence works correctly on both big and little
 * endian machines.
 */
static void mbedtls_gf128mul_x_ble(unsigned char r[16],
                                   const unsigned char x[16])
{
    uint64_t a, b, ra, rb;

    GET_UINT64_LE(a, x, 0);
    GET_UINT64_LE(b, x, 8);

    ra = (a << 1)  ^ 0x0087 >> (8 - ((b >> 63) << 3));
    rb = (a >> 63) | (b << 1);

    PUT_UINT64_LE(ra, r, 0);
    PUT_UINT64_LE(rb, r, 8);
}

/*
 * AES-XTS buffer encryption/decryption
 */
int mbedtls_aes_crypt_xts(mbedtls_aes_xts_context *ctx,
                          int mode,
                          size_t length,
                          const unsigned char data_unit[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t blocks = length / 16;
    size_t leftover = length % 16;
    unsigned char tweak[16];
    unsigned char prev_tweak[16];
    unsigned char tmp[16];

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(data_unit != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    /* Data units must be at least 16 bytes long. */
    if (length < 16) {
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;
    }

    /* NIST SP 800-38E disallows data units larger than 2**20 blocks. */
    if (length > (1 << 20) * 16) {
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;
    }

    /* Compute the tweak. */
    ret = mbedtls_aes_crypt_ecb(&ctx->tweak, MBEDTLS_AES_ENCRYPT,
                                data_unit, tweak);
    if (ret != 0) {
        return ret;
    }

    while (blocks--) {
        size_t i;

        if (leftover && (mode == MBEDTLS_AES_DECRYPT) && blocks == 0) {
            /* We are on the last block in a decrypt operation that has
             * leftover bytes, so we need to use the next tweak for this block,
             * and this tweak for the lefover bytes. Save the current tweak for
             * the leftovers and then update the current tweak for use on this,
             * the last full block. */
            memcpy(prev_tweak, tweak, sizeof(tweak));
            mbedtls_gf128mul_x_ble(tweak, tweak);
        }

        for (i = 0; i < 16; i++) {
            tmp[i] = input[i] ^ tweak[i];
        }

        ret = mbedtls_aes_crypt_ecb(&ctx->crypt, mode, tmp, tmp);
        if (ret != 0) {
            return ret;
        }

        for (i = 0; i < 16; i++) {
            output[i] = tmp[i] ^ tweak[i];
        }

        /* Update the tweak for the next block. */
        mbedtls_gf128mul_x_ble(tweak, tweak);

        output += 16;
        input += 16;
    }

    if (leftover) {
        /* If we are on the leftover bytes in a decrypt operation, we need to
         * use the previous tweak for these bytes (as saved in prev_tweak). */
        unsigned char *t = mode == MBEDTLS_AES_DECRYPT ? prev_tweak : tweak;

        /* We are now on the final part of the data unit, which doesn't divide
         * evenly by 16. It's time for ciphertext stealing. */
        size_t i;
        unsigned char *prev_output = output - 16;

        /* Copy ciphertext bytes from the previous block to our output for each
         * byte of cyphertext we won't steal. At the same time, copy the
         * remainder of the input for this final round (since the loop bounds
         * are the same). */
        for (i = 0; i < leftover; i++) {
            output[i] = prev_output[i];
            tmp[i] = input[i] ^ t[i];
        }

        /* Copy ciphertext bytes from the previous block for input in this
         * round. */
        for (; i < 16; i++) {
            tmp[i] = prev_output[i] ^ t[i];
        }

        ret = mbedtls_aes_crypt_ecb(&ctx->crypt, mode, tmp, tmp);
        if (ret != 0) {
            return ret;
        }

        /* Write the result back to the previous block, overriding the previous
         * output we copied. */
        for (i = 0; i < 16; i++) {
            prev_output[i] = tmp[i] ^ t[i];
        }
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
#if !defined(MBEDTLS_AES_CRYPT_CFB_ALT)
int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx,
                             int mode,
                             size_t length,
                             size_t *iv_off,
                             unsigned char iv[16],
                             const unsigned char *input,
                             unsigned char *output)
{
    int c;
    size_t n;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(iv_off != NULL);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    n = *iv_off;

    if (n > 15) {
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }

    if (mode == MBEDTLS_AES_DECRYPT) {
        while (length--) {
            if (n == 0) {
                mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv);
            }

            c = *input++;
            *output++ = (unsigned char) (c ^ iv[n]);
            iv[n] = (unsigned char) c;

            n = (n + 1) & 0x0F;
        }
    } else {
        while (length--) {
            if (n == 0) {
                mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv);
            }

            iv[n] = *output++ = (unsigned char) (iv[n] ^ *input++);

            n = (n + 1) & 0x0F;
        }
    }

    *iv_off = n;

    return 0;
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8(mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output)
{
    unsigned char c;
    unsigned char ov[17];

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(mode == MBEDTLS_AES_ENCRYPT ||
                     mode == MBEDTLS_AES_DECRYPT);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);
    while (length--) {
        memcpy(ov, iv, 16);
        mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv);

        if (mode == MBEDTLS_AES_DECRYPT) {
            ov[16] = *input;
        }

        c = *output++ = (unsigned char) (iv[0] ^ *input++);

        if (mode == MBEDTLS_AES_ENCRYPT) {
            ov[16] = c;
        }

        memcpy(iv, ov + 1, 16);
    }

    return 0;
}
#endif /* !MBEDTLS_AES_CRYPT_CFB_ALT */
#endif /* MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_OFB)
/*
 * AES-OFB (Output Feedback Mode) buffer encryption/decryption
 */
int mbedtls_aes_crypt_ofb(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *iv_off,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int ret = 0;
    size_t n;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(iv_off != NULL);
    AES_VALIDATE_RET(iv != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    n = *iv_off;

    if (n > 15) {
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }

    while (length--) {
        if (n == 0) {
            ret = mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv);
            if (ret != 0) {
                goto exit;
            }
        }
        *output++ =  *input++ ^ iv[n];

        n = (n + 1) & 0x0F;
    }

    *iv_off = n;

exit:
    return ret;
}
#endif /* MBEDTLS_CIPHER_MODE_OFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
#if !defined(MBEDTLS_AES_CRYPT_CTR_ALT)
int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    int c, i;
    size_t n;

    AES_VALIDATE_RET(ctx != NULL);
    AES_VALIDATE_RET(nc_off != NULL);
    AES_VALIDATE_RET(nonce_counter != NULL);
    AES_VALIDATE_RET(stream_block != NULL);
    AES_VALIDATE_RET(input != NULL);
    AES_VALIDATE_RET(output != NULL);

    n = *nc_off;

    if (n > 0x0F) {
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }

    while (length--) {
        if (n == 0) {
            mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, nonce_counter, stream_block);

            for (i = 16; i > 0; i--) {
                if (++nonce_counter[i - 1] != 0) {
                    break;
                }
            }
        }
        c = *input++;
        *output++ = (unsigned char) (c ^ stream_block[n]);

        n = (n + 1) & 0x0F;
    }

    *nc_off = n;

    return 0;
}
#endif /* !MBEDTLS_AES_CRYPT_CTR_ALT */
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_AES_ALT */
#endif /* MBEDTLS_AES_C */
