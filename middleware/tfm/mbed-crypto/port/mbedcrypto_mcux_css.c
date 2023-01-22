/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mbedtls/build_info.h"

#ifdef CSS
#include <mcuxClCss.h>               /* Interface to the entire nxpClCss component */
#include <mcuxClMemory.h>
#include <fsl_css.h>
#include <mbedtls/platform.h>
#include <mbedtls/platform_util.h>
#include <mbedtls/error.h>
#include <mcuxClSession.h>

/*!
 * @brief CSS initialization.
 */
status_t mbecrypto_mcux_css_init(void)
{
    static bool css_init_is_done = false;
    status_t    status;

    if(css_init_is_done == false)
    {
        /* Enable CSS and related clocks */
        status = CSS_PowerDownWakeupInit(CSS);
        if (status == kStatus_Success)
        {
            css_init_is_done = true;
        }
        /* Enable GDET interrupt, input event to ITRC */
        CSS->CSS_INT_ENABLE |= CSS_INT_ENABLE_GDET_INT_EN_Msk;
    }
    else
    {
        status = kStatus_Success;
    }
    
    return status;
}

/******************************************************************************/
/*************************** AES  *********************************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_CSS_AES) && defined(MBEDTLS_AES_ALT) && defined(MBEDTLS_AES_C) && MBEDTLS_MCUX_CSS_AES

#include <mbedtls/aes.h>

/* Parameter validation macros based on platform_util.h */
#define AES_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_AES_BAD_INPUT_DATA )
#define AES_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )


void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    AES_VALIDATE( ctx != NULL );

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
    AES_VALIDATE( ctx != NULL );

    mbedtls_aes_init( &ctx->crypt );
    mbedtls_aes_init( &ctx->tweak );
}

void mbedtls_aes_xts_free( mbedtls_aes_xts_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_aes_free( &ctx->crypt );
    mbedtls_aes_free( &ctx->tweak );
}
#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * AES key schedule, alternative implementation
 */
static int mbedtls_aes_setkey_alt( mbedtls_aes_context *ctx,
                                   const unsigned char *key,
                                   unsigned int keybits )
{
    int retCode = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    if ((NULL == ctx) || (NULL == key))
    {
        retCode = MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }
    else if (   ((MCUXCLCSS_CIPHER_KEY_SIZE_AES_128 * 8u) != keybits)
             && ((MCUXCLCSS_CIPHER_KEY_SIZE_AES_192 * 8u) != keybits)
             && ((MCUXCLCSS_CIPHER_KEY_SIZE_AES_256 * 8u) != keybits) )
    {
        retCode = MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }
    else
    {
        uint32_t keyByteLen = (uint32_t) keybits / 8u;
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retMemCpy, tokenMemCpy,
            mcuxClMemory_copy((uint8_t *) ctx->pKey, key, keyByteLen, keyByteLen) );

        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy) == tokenMemCpy)
            && (0u == retMemCpy) )
        {
            ctx->keyLength = keyByteLen;
            retCode = 0;
        }
    }

    return retCode;
}


/*
 * AES key schedule (encryption), alternative implementation
 */
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx,
                            const unsigned char *key,
                            unsigned int keybits )
{
    return mbedtls_aes_setkey_alt(ctx, key, keybits);
}


/*
 * AES key schedule (decryption), alternative implementation
 */
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx,
                            const unsigned char *key,
                            unsigned int keybits )
{
    return mbedtls_aes_setkey_alt(ctx, key, keybits);
}


/*
 * AES-ECB block en/decryption with CSS
 */
static int mbedtls_internal_aes_css( mbedtls_aes_context *ctx,
                                     const unsigned char *pInput,
                                     unsigned char *pOutput,
                                     mcuxClCss_CipherOption_t cssCipherOption,
                                     unsigned char *pIv,
                                     size_t length )
{
    /* Call Css to process one block. */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retCssCipherAsync, tokenCssCipherAsync,
        mcuxClCss_Cipher_Async(cssCipherOption,
                              0u, /* keyIdx is ignored. */
                              (const uint8_t *) ctx->pKey,
                              ctx->keyLength,
                              pInput,
                              length,
                              pIv,
                              pOutput) );
    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Cipher_Async) != tokenCssCipherAsync)
        || (MCUXCLCSS_STATUS_OK_WAIT != retCssCipherAsync) )
    {
        /* _Cipher_Async shall not return _SW_CANNOT_INTERRUPT after successfully returning from _WaitForOperation. */
        /* _Cipher_Async shall not return _SW_INVALID_PARAM if parameters are set properly. */
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }

    /* Wait for mcuxClCss_Cipher_Async. */
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retCssWaitCipher, tokenCssWaitCipher,
        mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR) );
    if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) != tokenCssWaitCipher)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if (MCUXCLCSS_STATUS_OK != retCssWaitCipher)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return 0;
}


/*
 * AES-ECB block encryption, alternative implementation
 */
int mbedtls_internal_aes_encrypt( mbedtls_aes_context *ctx,
                                  const unsigned char input[16],
                                  unsigned char output[16] )
{
    mcuxClCss_CipherOption_t cipherOption = (mcuxClCss_CipherOption_t) {
        .bits.dcrpt  = MCUXCLCSS_CIPHER_ENCRYPT,
        .bits.cphmde = MCUXCLCSS_CIPHERPARAM_ALGORITHM_AES_ECB,
        .bits.cphsoe = MCUXCLCSS_CIPHER_STATE_OUT_DISABLE,
        .bits.cphsie = MCUXCLCSS_CIPHER_STATE_IN_DISABLE,
        .bits.extkey = MCUXCLCSS_CIPHER_EXTERNAL_KEY };

    /* Initialize CSS */
    status_t ret_hw_init = mbecrypto_mcux_css_init();
    if( kStatus_Success != ret_hw_init)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return mbedtls_internal_aes_css(ctx, input, output, cipherOption, NULL, 16u);
}


/*
 * AES-ECB block decryption, alternative implementation
 */
int mbedtls_internal_aes_decrypt( mbedtls_aes_context *ctx,
                                  const unsigned char input[16],
                                  unsigned char output[16] )
{
    mcuxClCss_CipherOption_t cipherOption = (mcuxClCss_CipherOption_t) {
        .bits.dcrpt  = MCUXCLCSS_CIPHER_DECRYPT,
        .bits.cphmde = MCUXCLCSS_CIPHERPARAM_ALGORITHM_AES_ECB,
        .bits.cphsoe = MCUXCLCSS_CIPHER_STATE_OUT_DISABLE,
        .bits.cphsie = MCUXCLCSS_CIPHER_STATE_IN_DISABLE,
        .bits.extkey = MCUXCLCSS_CIPHER_EXTERNAL_KEY };

    /* Initialize CSS */
    status_t ret_hw_init = mbecrypto_mcux_css_init();
    if(kStatus_Success != ret_hw_init)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return mbedtls_internal_aes_css(ctx, input, output, cipherOption, NULL, 16u);
}


/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );
    AES_VALIDATE_RET( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );

    if( mode == MBEDTLS_AES_ENCRYPT )
        return( mbedtls_internal_aes_encrypt( ctx, input, output ) );
    else
        return( mbedtls_internal_aes_decrypt( ctx, input, output ) );
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption, alternative implementation
 */
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    uint32_t temp[4];

    if ((NULL == ctx) || (NULL == iv) || (NULL == input) || (NULL == output)
        || ((MBEDTLS_AES_ENCRYPT != mode) && (MBEDTLS_AES_DECRYPT != mode)) )
    {
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }
    else if (kStatus_Success != (length % 16u))
    {
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;
    }
    else
    {
        const uint8_t *pNewIv;
        mcuxClCss_CipherOption_t cipherOption;
        if (MBEDTLS_AES_ENCRYPT == mode)
        {
            pNewIv = output;
            cipherOption = (mcuxClCss_CipherOption_t) {
                .bits.dcrpt = MCUXCLCSS_CIPHER_ENCRYPT,
                .bits.cphmde = MCUXCLCSS_CIPHERPARAM_ALGORITHM_AES_CBC,
                .bits.extkey = MCUXCLCSS_CIPHER_EXTERNAL_KEY };
        }
        else
        {
            pNewIv = (uint8_t *) temp;
            cipherOption = (mcuxClCss_CipherOption_t) {
                .bits.dcrpt  = MCUXCLCSS_CIPHER_DECRYPT,
                .bits.cphmde = MCUXCLCSS_CIPHERPARAM_ALGORITHM_AES_CBC,
                .bits.extkey = MCUXCLCSS_CIPHER_EXTERNAL_KEY };

            /* Backup input[] as the next IV (ps, input[] will be overwritten if result in-place). */
            MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retMemCpy0, tokenMemCpy,
                mcuxClMemory_copy((uint8_t *) temp, input, 16u, 16u) );
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy) != tokenMemCpy)
                || (0u != retMemCpy0) )
            {
                return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
            }
        }

        /* Initialize CSS */
        status_t ret_hw_init = mbecrypto_mcux_css_init();
        if( kStatus_Success != ret_hw_init)
        {
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }

        int retCode = mbedtls_internal_aes_css(ctx, input, output, cipherOption, iv, length);
        if (0 != retCode)
        {
            return retCode;
        }

        /* Copy new IV to iv[]. */
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retMemCpy1, tokenMemCpy,
            mcuxClMemory_copy(iv, pNewIv, 16u, 16u) );
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy) != tokenMemCpy)
            || (0u != retMemCpy1) )
        {
            return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
        }
    }

    return 0;
}
#endif  /* MBEDTLS_CIPHER_MODE_CBC */

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
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t blocks = length / 16;
    size_t leftover = length % 16;
    unsigned char tweak[16];
    unsigned char prev_tweak[16];
    unsigned char tmp[16];

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );
    AES_VALIDATE_RET( data_unit != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );

    /* Data units must be at least 16 bytes long. */
    if( length < 16 )
        return MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH;

    /* NIST SP 800-38E disallows data units larger than 2**20 blocks. */
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

#if defined(MBEDTLS_CIPHER_MODE_CFB) /* Not HW accelerated, just a copy */
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
    size_t n;

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );
    AES_VALIDATE_RET( iv_off != NULL );
    AES_VALIDATE_RET( iv != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );

    n = *iv_off;

    if( n > 15 )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

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

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( mode == MBEDTLS_AES_ENCRYPT ||
                      mode == MBEDTLS_AES_DECRYPT );
    AES_VALIDATE_RET( iv != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );
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

#if defined(MBEDTLS_CIPHER_MODE_OFB) /* Not HW accelerated, just a copy */
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
    size_t n;

    AES_VALIDATE_RET( ctx != NULL );
    AES_VALIDATE_RET( iv_off != NULL );
    AES_VALIDATE_RET( iv != NULL );
    AES_VALIDATE_RET( input != NULL );
    AES_VALIDATE_RET( output != NULL );

    n = *iv_off;

    if( n > 15 )
        return( MBEDTLS_ERR_AES_BAD_INPUT_DATA );

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

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption, alternative implementation
 */
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    if ((NULL == ctx) || (NULL == nc_off)
        || (NULL == nonce_counter) || (NULL == stream_block)
        || (NULL == input) || (NULL == output) )
    {
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }
    else if (15u < *nc_off)
    {
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }
    else
    {
        uint32_t offset = *nc_off;
        uint32_t remainLength = length;
        const uint8_t *pInput = input;
        uint8_t *pOutput = output;

        /* En/decrypt by XORing with remaining byte(s) in stream_block[]. */
        while ((0u < remainLength) && (0u != (offset & 15u)))
        {
            *pOutput = *pInput ^ stream_block[offset];
            pInput++;
            pOutput++;
            remainLength--;
            offset++;
        }

        /* Here, offset = 0,    if starting with a new block (*nc_off = 0);   */
        /*       offset = 1~15, if there is unused byte(s) in stream_block[]; */
        /*       offset = 16,   if all bytes in stream_block[] are used.      */

        mcuxClCss_CipherOption_t cipherOption = (mcuxClCss_CipherOption_t) {
            .bits.dcrpt  = MCUXCLCSS_CIPHER_ENCRYPT,
            .bits.cphmde = MCUXCLCSS_CIPHERPARAM_ALGORITHM_AES_CTR,
            .bits.cphsoe = MCUXCLCSS_CIPHER_STATE_OUT_ENABLE,
            .bits.cphsie = MCUXCLCSS_CIPHER_STATE_IN_ENABLE,
            .bits.extkey = MCUXCLCSS_CIPHER_EXTERNAL_KEY };

        /* Initialize CSS */
        status_t ret_hw_init = mbecrypto_mcux_css_init();
        if(kStatus_Success != ret_hw_init)
        {
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }

        /* En/decrypt full block(s) with CSS. */
        uint32_t remainLengthFullBlock = remainLength & (~ (uint32_t) 15u);
        if (0u != remainLengthFullBlock)
        {
            int retCode = mbedtls_internal_aes_css(ctx, pInput, pOutput, cipherOption, nonce_counter, remainLengthFullBlock);
            if (0 != retCode)
            {
                /* unexpected error. */
                return retCode;
            }
        }

        pInput += remainLengthFullBlock;
        pOutput += remainLengthFullBlock;
        remainLength &= 15u;

        /* En/decrypt the last incomplete block (if exists). */
        if (0u != remainLength)
        {
            /* Prepare the last incomplete block in temp buffer (stream_block[]). */
            uint32_t i = 0u;
            do
            {
                stream_block[i] = pInput[i];
                i++;
            } while(i < remainLength);
            do
            {
                stream_block[i] = 0u;
                i++;
            } while (i < 16u);

            /* En/decrypt the last block. */
            int retCode = mbedtls_internal_aes_css(ctx, stream_block, stream_block, cipherOption, nonce_counter, 16u);
            if (0 != retCode)
            {
                /* unexpected error. */
                return retCode;
            }

            /* Move result to output buffer. */
            i = 0u;
            do
            {
                pOutput[i] = stream_block[i];
                stream_block[i] = 0u;
                i++;
            } while (i < remainLength);

            offset = remainLength;
        }
        else
        {
            /* If there is no incomplete last block, clean up used byte(s) in stream_block[]. */
            for (uint32_t j = *nc_off; j < offset; j++)
            {
                stream_block[j] = 0u;
            }
        }

        *nc_off = offset & 15u;
    }

    return 0;
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_MCUX_CSS_AES && MBEDTLS_AES_ALT && MBEDTLS_AES_C */

/******************************************************************************/
/*************************** SHA256 & SHA224 **********************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_CSS_SHA256) && MBEDTLS_MCUX_CSS_SHA256 && defined(MBEDTLS_SHA256_ALT) && defined(MBEDTLS_SHA256_C)
#include <mbedtls/sha256.h>

#define SHA256_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

void mbedtls_sha256_init( mbedtls_sha256_context *ctx )
{
    SHA256_VALIDATE( ctx != NULL );

    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_clone( mbedtls_sha256_context *dst,
                           const mbedtls_sha256_context *src )
{
    SHA256_VALIDATE( dst != NULL );
    SHA256_VALIDATE( src != NULL );

    *dst = *src;
}

int mbedtls_sha256_starts (mbedtls_sha256_context *ctx, int is224)
{
    if(ctx == NULL)
    {
        return MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }

    /* Initialize CSS */
    status_t ret_hw_init = mbecrypto_mcux_css_init();
    if(kStatus_Success != ret_hw_init)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    mcuxClSession_Descriptor_t session_descriptor;
    const mcuxClHash_Algo_t* pHash_algo;

    mcuxClHash_Context_t* pContext = &ctx->context;

    mcuxClSession_Handle_t session = &session_descriptor;

    if(0u == is224)
    {
        pHash_algo = &mcuxClHash_AlgoSHA256;
    }
    else
    {
        pHash_algo = &mcuxClHash_AlgoSHA224;
    }

    uint32_t workarea[MCUXCLHASH_WA_SIZE_MAX/sizeof(uint32_t)];

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(restSessionInit, tokenSessionInit, mcuxClSession_init(
            session,
            workarea,
            sizeof(workarea),
            NULL,
            0U));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != tokenSessionInit)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLSESSION_STATUS_OK != restSessionInit)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retInit, tokenInit, mcuxClHash_init(session, pContext, pHash_algo));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_init) != tokenInit)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLHASH_STATUS_OK != retInit)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return 0;
}

int mbedtls_sha256_update (mbedtls_sha256_context *ctx,
                               const unsigned char *input,
                               size_t ilen)
{
    if(ctx == NULL || input == NULL)
    {
        return MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }

    mcuxClSession_Descriptor_t session_descriptor;
    mcuxClSession_Handle_t session = &session_descriptor;

    uint32_t workarea[MCUXCLHASH_WA_SIZE_MAX/sizeof(uint32_t)];

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(restSessionInit, tokenSessionInit, mcuxClSession_init(
            session,
            workarea,
            sizeof(workarea),
            NULL,
            0U));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != tokenSessionInit)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLSESSION_STATUS_OK != restSessionInit)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    mcuxClHash_Context_t* pContext = &ctx->context;

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retUpdate, tokenUpdate, mcuxClHash_update(session, pContext, input, ilen));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_update) != tokenUpdate)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLHASH_STATUS_OK != retUpdate)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return 0;
}

int mbedtls_sha256_finish (mbedtls_sha256_context *ctx,
                               unsigned char output[32])
{
    if(ctx == NULL || output == NULL)
    {
        return MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }

    mcuxClSession_Descriptor_t session_descriptor;
    mcuxClSession_Handle_t session = &session_descriptor;

    uint32_t workarea[MCUXCLHASH_WA_SIZE_MAX/sizeof(uint32_t)];

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(restSessionInit, tokenSessionInit, mcuxClSession_init(
            session,
            workarea,
            sizeof(workarea),
            NULL,
            0U));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != tokenSessionInit)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLSESSION_STATUS_OK != restSessionInit)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    mcuxClHash_Context_t* pContext = &ctx->context;

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retFinish, tokenFinish, mcuxClHash_finish(session, pContext, output, NULL));

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retCleanup, tokenCleanup, mcuxClSession_cleanup(session));
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retDestroy, toeknDestroy, mcuxClSession_destroy(session));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_finish) != tokenFinish ||
       MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) != tokenCleanup ||
       MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) != toeknDestroy)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLHASH_STATUS_OK != retFinish || MCUXCLSESSION_STATUS_OK != retCleanup ||  MCUXCLSESSION_STATUS_OK != retDestroy)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return 0;
}

int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx,
                                    const unsigned char data[64])
{
    return 0;
}

#endif /* MBEDTLS_MCUX_CSS_SHA256 && MBEDTLS_SHA256_ALT && MBEDTLS_SHA256_C */

/******************************************************************************/
/*************************** SHA512 & SHA384 **********************************/
/******************************************************************************/
#if defined(MBEDTLS_MCUX_CSS_SHA512) && MBEDTLS_MCUX_CSS_SHA512 && defined(MBEDTLS_SHA512_ALT) && defined(MBEDTLS_SHA512_C)
#include <mbedtls/sha512.h>

#define SHA512_VALIDATE_RET(cond)                           \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_SHA512_BAD_INPUT_DATA )
#define SHA512_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

void mbedtls_sha512_init( mbedtls_sha512_context *ctx )
{
    SHA512_VALIDATE( ctx != NULL );

    memset( ctx, 0, sizeof( mbedtls_sha512_context ) );
}

void mbedtls_sha512_free( mbedtls_sha512_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_sha512_context ) );
}

void mbedtls_sha512_clone( mbedtls_sha512_context *dst,
                           const mbedtls_sha512_context *src )
{
    SHA512_VALIDATE( dst != NULL );
    SHA512_VALIDATE( src != NULL );

    *dst = *src;
}

int mbedtls_sha512_starts (mbedtls_sha512_context *ctx, int is384)
{
    if(ctx == NULL)
    {
        return MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }

    /* Initialize CSS */
    status_t ret_hw_init = mbecrypto_mcux_css_init();
    if(kStatus_Success != ret_hw_init)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    mcuxClSession_Descriptor_t session_descriptor;
    const mcuxClHash_Algo_t* pHash_algo;

    mcuxClHash_Context_t* pContext = &ctx->context;

    mcuxClSession_Handle_t session = &session_descriptor;

    if(0u == is384)
    {
        pHash_algo = &mcuxClHash_AlgoSHA512;
    }
    else
    {
        pHash_algo = &mcuxClHash_AlgoSHA384;
    }

    uint32_t workarea[MCUXCLHASH_WA_SIZE_MAX/sizeof(uint32_t)];

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(restSessionInit, tokenSessionInit, mcuxClSession_init(
            session,
            workarea,
            sizeof(workarea),
            NULL,
            0U));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != tokenSessionInit)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLSESSION_STATUS_OK != restSessionInit)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retInit, tokenInit, mcuxClHash_init(session, pContext, pHash_algo));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_init) != tokenInit)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLHASH_STATUS_OK != retInit)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return 0;
}

int mbedtls_sha512_update(mbedtls_sha512_context *ctx,
                               const unsigned char *input,
                               size_t ilen)
{
    if(ctx == NULL || input == NULL)
    {
        return MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }

    mcuxClSession_Descriptor_t session_descriptor;
    mcuxClSession_Handle_t session = &session_descriptor;

    uint32_t workarea[MCUXCLHASH_WA_SIZE_MAX/sizeof(uint32_t)];

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(restSessionInit, tokenSessionInit, mcuxClSession_init(
            session,
            workarea,
            sizeof(workarea),
            NULL,
            0U));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != tokenSessionInit)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLSESSION_STATUS_OK != restSessionInit)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    mcuxClHash_Context_t* pContext = &ctx->context;

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retUpdate, tokenUpdate, mcuxClHash_update(session, pContext, input, ilen));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_update) != tokenUpdate)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLHASH_STATUS_OK != retUpdate)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return 0;
}

int mbedtls_sha512_finish(mbedtls_sha512_context *ctx,
                               unsigned char output[64])
{
    if(ctx == NULL || output == NULL)
    {
        return MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }

    mcuxClSession_Descriptor_t session_descriptor;
    mcuxClSession_Handle_t session = &session_descriptor;

    uint32_t workarea[MCUXCLHASH_WA_SIZE_MAX/sizeof(uint32_t)];

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(restSessionInit, tokenSessionInit, mcuxClSession_init(
            session,
            workarea,
            sizeof(workarea),
            NULL,
            0U));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != tokenSessionInit)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLSESSION_STATUS_OK != restSessionInit)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }
    mcuxClHash_Context_t* pContext = &ctx->context;

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retFinish, tokenFinish, mcuxClHash_finish(session, pContext, output, NULL));

    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retCleanup, tokenCleanup, mcuxClSession_cleanup(session));
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(retDestroy, toeknDestroy, mcuxClSession_destroy(session));

    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_finish) != tokenFinish ||
       MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) != tokenCleanup ||
       MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) != toeknDestroy)
    {
        return MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    }
    if(MCUXCLHASH_STATUS_OK != retFinish || MCUXCLSESSION_STATUS_OK != retCleanup ||  MCUXCLSESSION_STATUS_OK != retDestroy)
    {
        return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
    }

    return 0;
}

int mbedtls_internal_sha512_process(mbedtls_sha512_context *ctx,
                                    const unsigned char data[64])
{
    return 0;
}

#endif /* MBEDTLS_MCUX_CSS_SHA512 && MBEDTLS_SHA512_ALT && MBEDTLS_SHA512_C */


#endif /* CSS */