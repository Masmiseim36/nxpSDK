/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#define BOOTLOADER_HOST

#include "aes128_key_wrap_unwrap.h"
#if !defined(BOOTLOADER_HOST)
#include "security/aes_security.h"
#endif

#include "mbedtls/aes.h"

#if defined(BOOTLOADER_HOST)
// From bytes_aes.c.
extern void Cipher(unsigned char cin[], unsigned int w[], int nr, unsigned char cout[]);
extern void InvCipher(unsigned char cin[], unsigned int w[], int nr, unsigned char cout[]);
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief number of 64-bit data blocks
#define NBLOCKS_5 5
#define NBLOCKS_8 8

//! @brief 64-bit initialization vector
static const unsigned char iv[8] = { 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6 };
//! @brief 64-bit integrity check register
static unsigned char a[8];
//! @brief 8-bit array of 64-bit registers
static unsigned char r[8 * (NBLOCKS_8 + 1)];

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
// private internal function
void do_aes_key_wrap(const unsigned char plaintext[],
    unsigned char wrapped_ciphertext[],
    unsigned int expanded_kek[], 
    unsigned char kek[],
    int N,
    int useMbedtls)
{
    unsigned int i, j;    // loop counters
    unsigned char in[16]; // 128-bit temporary plaintext input vector

                          // step 1: initialize the byte-sized data variables
                          // set A = IV
                          // for i = 1 to n
                          //     R[i] = P[i]

    a[0] = iv[0];
    a[1] = iv[1];
    a[2] = iv[2];
    a[3] = iv[3];
    a[4] = iv[4];
    a[5] = iv[5];
    a[6] = iv[6];
    a[7] = iv[7];

    for (i = 1; i <= N; i++)
    {
        r[8 * i + 0] = plaintext[8 * (i - 1) + 0];
        r[8 * i + 1] = plaintext[8 * (i - 1) + 1];
        r[8 * i + 2] = plaintext[8 * (i - 1) + 2];
        r[8 * i + 3] = plaintext[8 * (i - 1) + 3];
        r[8 * i + 4] = plaintext[8 * (i - 1) + 4];
        r[8 * i + 5] = plaintext[8 * (i - 1) + 5];
        r[8 * i + 6] = plaintext[8 * (i - 1) + 6];
        r[8 * i + 7] = plaintext[8 * (i - 1) + 7];
    }

    // step 2: calculate intermediate values
    // for j = 0 to 5
    //     for i = 1 to n
    //         B = AES(K, A | R[i])
    //         A = MSB(64, B) ^ (n*j)+i
    //         R[i] = LSB(64, B)

    for (j = 0; j <= 5; j++)
    {
        for (i = 1; i <= N; i++)
        {
            in[0] = a[0];
            in[1] = a[1];
            in[2] = a[2];
            in[3] = a[3];
            in[4] = a[4];
            in[5] = a[5];
            in[6] = a[6];
            in[7] = a[7];

            in[8] = r[8 * i + 0];
            in[9] = r[8 * i + 1];
            in[10] = r[8 * i + 2];
            in[11] = r[8 * i + 3];
            in[12] = r[8 * i + 4];
            in[13] = r[8 * i + 5];
            in[14] = r[8 * i + 6];
            in[15] = r[8 * i + 7];

            if (useMbedtls)
            {
                mbedtls_aes_context aes_ctx;
                mbedtls_aes_setkey_enc(&aes_ctx, kek, 256);
                mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, in, in);
                mbedtls_aes_free(&aes_ctx);
            }
            else
            {

#if defined BOOTLOADER_HOST
                Cipher(in, expanded_kek, 10, in); // perform aes128 encryption
#else
                aes_encrypt((unsigned int *)in, expanded_kek, (unsigned int *)in);
#endif // BOOTLOADER_HOST
            }            
            a[0] = in[0];
            a[1] = in[1];
            a[2] = in[2];
            a[3] = in[3];
            a[4] = in[4];
            a[5] = in[5];
            a[6] = in[6];
            a[7] = in[7] ^ ((N * j) + i);

            r[8 * i + 0] = in[8];
            r[8 * i + 1] = in[9];
            r[8 * i + 2] = in[10];
            r[8 * i + 3] = in[11];
            r[8 * i + 4] = in[12];
            r[8 * i + 5] = in[13];
            r[8 * i + 6] = in[14];
            r[8 * i + 7] = in[15];
        } // end for (i)
    }     // end for (j)

          // step 3: output the results
          // set C[0] = A
          // for i = 1 to n
          //     C[i] = R[i]

    wrapped_ciphertext[0] = a[0];
    wrapped_ciphertext[1] = a[1];
    wrapped_ciphertext[2] = a[2];
    wrapped_ciphertext[3] = a[3];
    wrapped_ciphertext[4] = a[4];
    wrapped_ciphertext[5] = a[5];
    wrapped_ciphertext[6] = a[6];
    wrapped_ciphertext[7] = a[7];

    for (i = 1; i <= N; i++)
    {
        wrapped_ciphertext[8 * i + 0] = r[8 * i + 0];
        wrapped_ciphertext[8 * i + 1] = r[8 * i + 1];
        wrapped_ciphertext[8 * i + 2] = r[8 * i + 2];
        wrapped_ciphertext[8 * i + 3] = r[8 * i + 3];
        wrapped_ciphertext[8 * i + 4] = r[8 * i + 4];
        wrapped_ciphertext[8 * i + 5] = r[8 * i + 5];
        wrapped_ciphertext[8 * i + 6] = r[8 * i + 6];
        wrapped_ciphertext[8 * i + 7] = r[8 * i + 7];
    }
}
// See aes128_key_wrap_unwrap.h for documentation on this function.
void do_aes128_key_wrap(const unsigned char plaintext[],
                        unsigned char wrapped_ciphertext[],
                        unsigned int expanded_kek[])
{
    do_aes_key_wrap(plaintext, wrapped_ciphertext, expanded_kek, NULL, NBLOCKS_5, 0);
}

// See aes256_key_wrap_unwrap.h for documentation on this function.
void do_aes_key_wrap_N8(const unsigned char plaintext[],
    unsigned char wrapped_ciphertext[],
    unsigned char kek[])
{
    do_aes_key_wrap(plaintext, wrapped_ciphertext, NULL, kek, NBLOCKS_8, 1);
}

//******************************************************************************
//******************************************************************************

// Private internal function

unsigned int do_aes_key_unwrap(const unsigned char wrapped_ciphertext[],
    unsigned char unwrapped_plaintext[],
    unsigned int expanded_kek[], 
    unsigned char kek[],
    int N,
    int useMbedtls)
{
    signed int i, j;      // loop counters
    unsigned char in[16]; // 128-bit temporary ciphertext input vector

                          // step 1: initialize variables
                          // set A = C[0]
                          // for i = 1 to n
                          //     R[i] = C[i]

    a[0] = wrapped_ciphertext[0];
    a[1] = wrapped_ciphertext[1];
    a[2] = wrapped_ciphertext[2];
    a[3] = wrapped_ciphertext[3];
    a[4] = wrapped_ciphertext[4];
    a[5] = wrapped_ciphertext[5];
    a[6] = wrapped_ciphertext[6];
    a[7] = wrapped_ciphertext[7];

    for (i = 1; i <= N; i++)
    {
        r[8 * i + 0] = wrapped_ciphertext[8 * i + 0];
        r[8 * i + 1] = wrapped_ciphertext[8 * i + 1];
        r[8 * i + 2] = wrapped_ciphertext[8 * i + 2];
        r[8 * i + 3] = wrapped_ciphertext[8 * i + 3];
        r[8 * i + 4] = wrapped_ciphertext[8 * i + 4];
        r[8 * i + 5] = wrapped_ciphertext[8 * i + 5];
        r[8 * i + 6] = wrapped_ciphertext[8 * i + 6];
        r[8 * i + 7] = wrapped_ciphertext[8 * i + 7];
    }

    // step 2: calculate intermediate values
    // for j = 5 to 0
    //     for i = n to 1
    //         B = AES-1(K, (A ^ (n*j+i) | R[i])
    //         A = MSB(64, B)
    //         R[i] = LSB(64, B)

    for (j = 5; j >= 0; j--)
    {
        for (i = N; i >= 1; i--)
        {
            in[0] = a[0];
            in[1] = a[1];
            in[2] = a[2];
            in[3] = a[3];
            in[4] = a[4];
            in[5] = a[5];
            in[6] = a[6];
            in[7] = a[7] ^ ((N * j) + i);

            in[8] = r[8 * i + 0];
            in[9] = r[8 * i + 1];
            in[10] = r[8 * i + 2];
            in[11] = r[8 * i + 3];
            in[12] = r[8 * i + 4];
            in[13] = r[8 * i + 5];
            in[14] = r[8 * i + 6];
            in[15] = r[8 * i + 7];

            if (useMbedtls)
            {
                mbedtls_aes_context aes_ctx;
                mbedtls_aes_setkey_dec(&aes_ctx, kek, 256);
                mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, in, in);
                mbedtls_aes_free(&aes_ctx);
            }
            else
            {
#if defined BOOTLOADER_HOST
                InvCipher(in, expanded_kek, 10, in); // perform aes128 decryption
#else
            // TODO aes_decrypt reverses bytes(?)
                aes_decrypt((unsigned int *)in, expanded_kek, (unsigned int *)in);
#endif // BOOTLOADER_HOST
            }
            a[0] = in[0];
            a[1] = in[1];
            a[2] = in[2];
            a[3] = in[3];
            a[4] = in[4];
            a[5] = in[5];
            a[6] = in[6];
            a[7] = in[7];

            r[8 * i + 0] = in[8];
            r[8 * i + 1] = in[9];
            r[8 * i + 2] = in[10];
            r[8 * i + 3] = in[11];
            r[8 * i + 4] = in[12];
            r[8 * i + 5] = in[13];
            r[8 * i + 6] = in[14];
            r[8 * i + 7] = in[15];
        } // end for (i)
    }     // end for (j)

          // step 3: output the results
          // if A == IV
          //    then
          //         for i = 1 to n
          //             P[i] = R[i]
          //    else
          //         return an error

    unwrapped_plaintext[0] = a[0];
    unwrapped_plaintext[1] = a[1];
    unwrapped_plaintext[2] = a[2];
    unwrapped_plaintext[3] = a[3];
    unwrapped_plaintext[4] = a[4];
    unwrapped_plaintext[5] = a[5];
    unwrapped_plaintext[6] = a[6];
    unwrapped_plaintext[7] = a[7];

    for (i = 1; i <= N; i++)
    {
        unwrapped_plaintext[8 * i + 0] = r[8 * i + 0];
        unwrapped_plaintext[8 * i + 1] = r[8 * i + 1];
        unwrapped_plaintext[8 * i + 2] = r[8 * i + 2];
        unwrapped_plaintext[8 * i + 3] = r[8 * i + 3];
        unwrapped_plaintext[8 * i + 4] = r[8 * i + 4];
        unwrapped_plaintext[8 * i + 5] = r[8 * i + 5];
        unwrapped_plaintext[8 * i + 6] = r[8 * i + 6];
        unwrapped_plaintext[8 * i + 7] = r[8 * i + 7];
    }

    if ((unwrapped_plaintext[0] == iv[0]) && (unwrapped_plaintext[1] == iv[1]) && (unwrapped_plaintext[2] == iv[2]) &&
        (unwrapped_plaintext[3] == iv[3]) && (unwrapped_plaintext[4] == iv[4]) && (unwrapped_plaintext[5] == iv[5]) &&
        (unwrapped_plaintext[6] == iv[6]) && (unwrapped_plaintext[7] == iv[7]))
        return 0; // error-free exit
    else
        return -1; // error exit
}

// See aes128_key_wrap_unwrap.h for documentation on this function.
unsigned int do_aes128_key_unwrap(const unsigned char wrapped_ciphertext[],
                                  unsigned char unwrapped_plaintext[],
                                  unsigned int expanded_kek[])
{
    return do_aes_key_unwrap(wrapped_ciphertext,
        unwrapped_plaintext,
        expanded_kek,
        NULL,
        NBLOCKS_5,
        0);
}


// See aes256_key_wrap_unwrap.h for documentation on this function.
unsigned int do_aes_key_unwrap_N8(const unsigned char wrapped_ciphertext[],
    unsigned char unwrapped_plaintext[],
    unsigned char kek[])
{
    return do_aes_key_unwrap(wrapped_ciphertext,
        unwrapped_plaintext,
        NULL,
        kek,
        NBLOCKS_8,
        1);
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
