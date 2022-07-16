/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mbedtls/build_info.h"
#include "mbedtls/error.h"

#if defined(FSL_FEATURE_SOC_HASHCRYPT_COUNT) && (FSL_FEATURE_SOC_HASHCRYPT_COUNT > 0)
#include "fsl_hashcrypt.h"
#include "mbedtls/platform_util.h"

/* Initialize HASHCRYPT */
static volatile bool hashcrypt_init_is_done = false;
static void mbedtls_hashcrypt_init(void)
{
    /* Initialize HASHCRYPT */
    HASHCRYPT_Init(HASHCRYPT);
    /* Secure-lock HASHCRYPT master.*/
    HASHCRYPT->LOCK = HASHCRYPT_LOCK_PATTERN(0xA75) | HASHCRYPT_LOCK_SECLOCK(0x1);
    
    hashcrypt_init_is_done = true;
}

/******************************************************************************/
/*************************** AES  *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_HASHCRYPT_AES) && defined(MBEDTLS_AES_ALT) && defined(MBEDTLS_AES_C) && MBEDTLS_MCUX_HASHCRYPT_AES

#include "mbedtls/aes.h"

void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    if(hashcrypt_init_is_done == false)
    {
        mbedtls_hashcrypt_init();
    }

    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
}

void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_aes_context ) );
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)
void mbedtls_aes_xts_init( mbedtls_aes_xts_context *ctx )
{
    mbedtls_aes_init( &ctx->crypt );
    mbedtls_aes_init( &ctx->tweak );
}

void mbedtls_aes_xts_free( mbedtls_aes_xts_context *ctx )
{
    mbedtls_aes_free( &ctx->crypt );
    mbedtls_aes_free( &ctx->tweak );
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] )
{
    if( mode == MBEDTLS_AES_ENCRYPT )
        return( mbedtls_internal_aes_encrypt( ctx, input, output ) );
    else
        return( mbedtls_internal_aes_decrypt( ctx, input, output ) );
}

#if defined(MBEDTLS_CIPHER_MODE_XTS)

/* Endianess with 64 bits values */
#ifndef GET_UINT64_LE
#define GET_UINT64_LE(n,b,i)                            \
{                                                       \
    (n) = ( (uint64_t) (b)[(i) + 7] << 56 )             \
        | ( (uint64_t) (b)[(i) + 6] << 48 )             \
        | ( (uint64_t) (b)[(i) + 5] << 40 )             \
        | ( (uint64_t) (b)[(i) + 4] << 32 )             \
        | ( (uint64_t) (b)[(i) + 3] << 24 )             \
        | ( (uint64_t) (b)[(i) + 2] << 16 )             \
        | ( (uint64_t) (b)[(i) + 1] <<  8 )             \
        | ( (uint64_t) (b)[(i)    ]       );            \
}
#endif

#ifndef PUT_UINT64_LE
#define PUT_UINT64_LE(n,b,i)                            \
{                                                       \
    (b)[(i) + 7] = (unsigned char) ( (n) >> 56 );       \
    (b)[(i) + 6] = (unsigned char) ( (n) >> 48 );       \
    (b)[(i) + 5] = (unsigned char) ( (n) >> 40 );       \
    (b)[(i) + 4] = (unsigned char) ( (n) >> 32 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
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
static void mbedtls_gf128mul_x_ble( unsigned char r[16],
                                    const unsigned char x[16] )
{
    uint64_t a, b, ra, rb;

    GET_UINT64_LE( a, x, 0 );
    GET_UINT64_LE( b, x, 8 );

    ra = ( a << 1 )  ^ 0x0087 >> ( 8 - ( ( b >> 63 ) << 3 ) );
    rb = ( a >> 63 ) | ( b << 1 );

    PUT_UINT64_LE( ra, r, 0 );
    PUT_UINT64_LE( rb, r, 8 );
}

/*
 * AES-XTS buffer encryption/decryption
 */
int mbedtls_aes_crypt_xts( mbedtls_aes_xts_context *ctx,
                           int mode,
                           size_t length,
                           const unsigned char data_unit[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int ret;
    size_t blocks = length / 16;
    size_t leftover = length % 16;
    unsigned char tweak[16];
    unsigned char prev_tweak[16];
    unsigned char tmp[16];

    /* Sectors must be at least 16 bytes. */
    if( length < 16 )
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;

    /* NIST SP 80-38E disallows data units larger than 2**20 blocks. */
    if( length > ( 1 << 20 ) * 16 )
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;

    /* Compute the tweak. */
    ret = mbedtls_aes_crypt_ecb( &ctx->tweak, MBEDTLS_AES_ENCRYPT,
                                 data_unit, tweak );
    if( ret != 0 )
        return( ret );

    while( blocks-- )
    {
        size_t i;

        if( leftover && ( mode == MBEDTLS_AES_DECRYPT ) && blocks == 0 )
        {
            /* We are on the last block in a decrypt operation that has
             * leftover bytes, so we need to use the next tweak for this block,
             * and this tweak for the lefover bytes. Save the current tweak for
             * the leftovers and then update the current tweak for use on this,
             * the last full block. */
            memcpy( prev_tweak, tweak, sizeof( tweak ) );
            mbedtls_gf128mul_x_ble( tweak, tweak );
        }

        for( i = 0; i < 16; i++ )
            tmp[i] = input[i] ^ tweak[i];

        ret = mbedtls_aes_crypt_ecb( &ctx->crypt, mode, tmp, tmp );
        if( ret != 0 )
            return( ret );

        for( i = 0; i < 16; i++ )
            output[i] = tmp[i] ^ tweak[i];

        /* Update the tweak for the next block. */
        mbedtls_gf128mul_x_ble( tweak, tweak );

        output += 16;
        input += 16;
    }

    if( leftover )
    {
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
        for( i = 0; i < leftover; i++ )
        {
            output[i] = prev_output[i];
            tmp[i] = input[i] ^ t[i];
        }

        /* Copy ciphertext bytes from the previous block for input in this
         * round. */
        for( ; i < 16; i++ )
            tmp[i] = prev_output[i] ^ t[i];

        ret = mbedtls_aes_crypt_ecb( &ctx->crypt, mode, tmp, tmp );
        if( ret != 0 )
            return ret;

        /* Write the result back to the previous block, overriding the previous
         * output we copied. */
        for( i = 0; i < 16; i++ )
            prev_output[i] = tmp[i] ^ t[i];
    }

    return( 0 );
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                       int mode,
                       size_t length,
                       size_t *iv_off,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    int c;
    size_t n = *iv_off;

    if( mode == MBEDTLS_AES_DECRYPT )
    {
        while( length-- )
        {
            if( n == 0 )
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

            c = *input++;
            *output++ = (unsigned char)( c ^ iv[n] );
            iv[n] = (unsigned char) c;

            n = ( n + 1 ) & 0x0F;
        }
    }
    else
    {
        while( length-- )
        {
            if( n == 0 )
                mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

            iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );

            n = ( n + 1 ) & 0x0F;
        }
    }

    *iv_off = n;

    return( 0 );
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                       int mode,
                       size_t length,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    unsigned char c;
    unsigned char ov[17];

    while( length-- )
    {
        memcpy( ov, iv, 16 );
        mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );

        if( mode == MBEDTLS_AES_DECRYPT )
            ov[16] = *input;

        c = *output++ = (unsigned char)( iv[0] ^ *input++ );

        if( mode == MBEDTLS_AES_ENCRYPT )
            ov[16] = c;

        memcpy( iv, ov + 1, 16 );
    }

    return( 0 );
}
#endif /* MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_OFB)
/*
 * AES-OFB (Output Feedback Mode) buffer encryption/decryption
 */
int mbedtls_aes_crypt_ofb( mbedtls_aes_context *ctx,
                           size_t length,
                           size_t *iv_off,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int ret = 0;
    size_t n = *iv_off;

    while( length-- )
    {
        if( n == 0 )
        {
            ret = mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
            if( ret != 0 )
                goto exit;
        }
        *output++ =  *input++ ^ iv[n];

        n = ( n + 1 ) & 0x0F;
    }

    *iv_off = n;

exit:
    return( ret );
}
#endif /* MBEDTLS_CIPHER_MODE_OFB */

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    size_t key_size;

    switch (keybits)
    {
        case 128:
            key_size = 16;
            break;
        case 192:
            key_size = 24;
            break;
        case 256:
            key_size = 32;
            break;
        default:
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }
    /* secret bus is marked as key address == HASHCRYPT base */
    if ((uint32_t)key == (uint32_t)HASHCRYPT)
    {
        ctx->keyType = kHASHCRYPT_SecretKey;
    }
    else
    {
        ctx->keyType = kHASHCRYPT_UserKey;
    }
    if (kStatus_Success != HASHCRYPT_AES_SetKey(HASHCRYPT, ctx, key, key_size))
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    return (0);
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    size_t key_size;

    switch (keybits)
    {
        case 128:
            key_size = 16;
            break;
        case 192:
            key_size = 24;
            break;
        case 256:
            key_size = 32;
            break;
        default:
            return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }
    /* secret bus is marked as key address == HASHCRYPT base */
    if ((uint32_t)key == (uint32_t)HASHCRYPT)
    {
        ctx->keyType = kHASHCRYPT_SecretKey;
    }
    else
    {
        ctx->keyType = kHASHCRYPT_UserKey;
    }
    if (kStatus_Success != HASHCRYPT_AES_SetKey(HASHCRYPT, ctx, key, key_size))
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    return 0;
}

/*
 * AES-ECB block encryption
 */
int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    if (kStatus_Success != HASHCRYPT_AES_EncryptEcb(HASHCRYPT, ctx, input, output, 16))
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    return (0);
}

/*
 * AES-ECB block decryption
 */
int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    if (kStatus_Success != HASHCRYPT_AES_DecryptEcb(HASHCRYPT, ctx, input, output, 16))
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    return (0);
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
    if (length % 16)
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);

    if (mode == MBEDTLS_AES_DECRYPT)
    {
        uint8_t tmp[16];
        memcpy(tmp, input + length - 16, 16);
        if (kStatus_Success != HASHCRYPT_AES_DecryptCbc(HASHCRYPT, ctx, input, output, length, iv))
        {
            return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
        }
        memcpy(iv, tmp, 16);
    }
    else
    {
        if (kStatus_Success != HASHCRYPT_AES_EncryptCbc(HASHCRYPT, ctx, input, output, length, iv))
        {
            return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
        }
        memcpy(iv, output + length - 16, 16);
    }

    return (0);
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx,
                          size_t length,
                          size_t *nc_off,
                          unsigned char nonce_counter[16],
                          unsigned char stream_block[16],
                          const unsigned char *input,
                          unsigned char *output)
{
    if (kStatus_Success !=
        HASHCRYPT_AES_CryptCtr(HASHCRYPT, ctx, input, output, length, nonce_counter, stream_block, nc_off))
    {
        return (MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED);
    }

    return (0);
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_MCUX_HASHCRYPT_AES && MBEDTLS_AES_ALT && MBEDTLS_AES_C  */


/******************************************************************************/
/*************************** SHA1 *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_HASHCRYPT_SHA1) && defined(MBEDTLS_SHA1_ALT) && defined(MBEDTLS_SHA1_C)

#include "mbedtls/sha1.h"

void mbedtls_sha1_init(mbedtls_sha1_context *ctx)
{
    memset(ctx, 0, sizeof(mbedtls_sha1_context));

    if(hashcrypt_init_is_done == false)
    {
        mbedtls_hashcrypt_init();
    }
}

void mbedtls_sha1_free(mbedtls_sha1_context *ctx)
{
    if (ctx == NULL)
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_sha1_context ) );
}

void mbedtls_sha1_clone(mbedtls_sha1_context *dst, const mbedtls_sha1_context *src)
{
    memcpy(dst, src, sizeof(mbedtls_sha1_context));
}

/*
 * SHA-1 context setup
 */
int mbedtls_sha1_starts(mbedtls_sha1_context *ctx)
{
    status_t ret = kStatus_Fail;
    ret = HASHCRYPT_SHA_Init(HASHCRYPT, ctx, kHASHCRYPT_Sha1);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    return 0;
}

int mbedtls_internal_sha1_process(mbedtls_sha1_context *ctx, const unsigned char data[64])
{
    status_t ret = kStatus_Fail;
    ret = HASHCRYPT_SHA_Update(HASHCRYPT, ctx, data, 64);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    return 0;
}

/*
 * SHA-1 process buffer
 */
int mbedtls_sha1_update(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    status_t ret = kStatus_Fail;
    ret = HASHCRYPT_SHA_Update(HASHCRYPT, ctx, input, ilen);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    return 0;
}

/*
 * SHA-1 final digest
 */
int mbedtls_sha1_finish(mbedtls_sha1_context *ctx, unsigned char output[20])
{
    status_t ret = kStatus_Fail;
    size_t outputSize = 20;
    ret = HASHCRYPT_SHA_Finish(HASHCRYPT, ctx, output, &outputSize);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    return 0;
}

#endif /* MBEDTLS_MCUX_HASHCRYPT_SHA1 && MBEDTLS_SHA1_ALT && MBEDTLS_SHA1_C */


/******************************************************************************/
/*************************** SHA256********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_HASHCRYPT_SHA256) && defined(MBEDTLS_SHA256_ALT) && defined(MBEDTLS_SHA256_C)
#include "mbedtls/sha256.h"

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    memset(ctx, 0, sizeof(mbedtls_sha256_context));

    if(hashcrypt_init_is_done == false)
    {
        mbedtls_hashcrypt_init();
    }
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if( ctx == NULL )
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst, const mbedtls_sha256_context *src)
{
    memcpy(dst, src, sizeof(*dst));
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int is224)
{
    status_t ret = kStatus_Fail;
    if (!is224) /* SHA-224 not supported */
    {
        ret = HASHCRYPT_SHA_Init(HASHCRYPT, ctx, kHASHCRYPT_Sha256);
    }
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    return 0;
}

int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    status_t ret = kStatus_Fail;
    ret = HASHCRYPT_SHA_Update(HASHCRYPT, ctx, data, 64);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    return 0;
}

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    status_t ret = kStatus_Fail;
    ret = HASHCRYPT_SHA_Update(HASHCRYPT, ctx, input, ilen);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    return 0;
}

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char output[32])
{
    status_t ret = kStatus_Fail;
    size_t outputSize = 32;

    unsigned char output_tmp[32]; //NXP cause of HashCrypt module //DM Remove when it is fixed by the driver
    memcpy(output_tmp, output, 32); //DM
    
    ret = HASHCRYPT_SHA_Finish(HASHCRYPT, ctx, output_tmp, &outputSize);
    if (ret != kStatus_Success)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    
    memcpy(output, output_tmp, 32); //DM
    return 0;
}

#endif /* MBEDTLS_MCUX_HASHCRYPT_SHA256 && MBEDTLS_SHA256_ALT && MBEDTLS_SHA256_C*/


#endif /* FSL_FEATURE_SOC_HASHCRYPT_COUNT */
