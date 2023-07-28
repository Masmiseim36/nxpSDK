/*!
 * \file
 *
 * This file contains the SW implementation of the SHA256 algorithm.
 *
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2022 NXP
 *
 * NXP Confidential Proprietary
 *
 * No part of this document must be reproduced in any form - including copied,
 * transcribed, printed or by any electronic means - without specific written
 * permission from NXP.
 *
 */

#include "CryptoLibSW.h"

/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
******************************************************************************/
#define SHA256_RightRotate(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))

/******************************************************************************
*******************************************************************************
* Private memory definitions
*******************************************************************************
******************************************************************************/
/* Hash round constants */
static const uint32_t k[] = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u, 0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u};

static uint32_t rotateOrigin;

/******************************************************************************
*******************************************************************************
* Public function prototypes
*******************************************************************************
******************************************************************************/
/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
******************************************************************************/

void sw_sha256_initialize_output(uint32_t *sha256_state)
{
    sha256_state[0] = 0x6a09e667u;
    sha256_state[1] = 0xbb67ae85u;
    sha256_state[2] = 0x3c6ef372u;
    sha256_state[3] = 0xa54ff53au;
    sha256_state[4] = 0x510e527fu;
    sha256_state[5] = 0x9b05688cu;
    sha256_state[6] = 0x1f83d9abu;
    sha256_state[7] = 0x5be0cd19u;
}

/*****************************************************************************/
void sw_sha256_hash_n(const uint8_t *msg_data, int32_t num_blks, uint32_t *sha256_state)
{
    uint32_t w[16];
    uint32_t a, b, c, d, e, f, g, h, s0, s1, t1, ch;
    int32_t  n;
    uint8_t  i;

    // Process the message in successive 512-bit chunks:
    for (n = 0; n < num_blks; n++)
    {
        // Initialize hash value for this chunk:
        a = sha256_state[0];
        b = sha256_state[1];
        c = sha256_state[2];
        d = sha256_state[3];
        e = sha256_state[4];
        f = sha256_state[5];
        g = sha256_state[6];
        h = sha256_state[7];

        /* Initialize the w array */
        for (i = 0; i < 16u; i++)
        {
            uint8_t *p = (uint8_t *)&msg_data[i * sizeof(uint32_t)];
            w[i]       = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
        }

        /* The 64 hashing rounds */
        for (i = 0; i < 64u; i++)
        {
            /* Update the w array as a circular buffer */
            if (i >= 16u)
            {
                rotateOrigin = w[(i - 15u) & 0x0Fu];
                s0 = SHA256_RightRotate(rotateOrigin, 7) ^ SHA256_RightRotate(rotateOrigin, 18) ^ (rotateOrigin >> 3);
                rotateOrigin = w[(i - 2u) & 0x0Fu];
                s1 = SHA256_RightRotate(rotateOrigin, 17) ^ SHA256_RightRotate(rotateOrigin, 19) ^ (rotateOrigin >> 10);
                w[i & 0x0Fu] += s0 + w[(i - 7u) & 0x0Fu] + s1;
            }

            rotateOrigin = a;
            s0           = SHA256_RightRotate(a, 2) ^ SHA256_RightRotate(a, 13) ^ SHA256_RightRotate(a, 22);
            s0 += (a & b) ^ (a & c) ^ (b & c);

            rotateOrigin = e;
            s1           = SHA256_RightRotate(e, 6) ^ SHA256_RightRotate(e, 11) ^ SHA256_RightRotate(e, 25);
            ch           = (e & f) ^ (~e & g);
            t1           = h + s1 + ch + k[i] + w[i & 0x0Fu];

            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + s0;
        }

        /* update the intermediate hash */
        sha256_state[0] += a;
        sha256_state[1] += b;
        sha256_state[2] += c;
        sha256_state[3] += d;
        sha256_state[4] += e;
        sha256_state[5] += f;
        sha256_state[6] += g;
        sha256_state[7] += h;

        msg_data += 64;
    } /* for(...) */
}
#ifdef _keepUnused
/*****************************************************************************/
void sw_sha256_hash(uint8_t *msg_data, uint32_t *sha256_state)
{
    sw_sha256_hash_n(msg_data, 1, sha256_state);
}

/*****************************************************************************/
void sw_sha256_update(uint8_t *msg_data, int32_t num_blks, uint32_t *sha256_state)
{
    sw_sha256_initialize_output(sha256_state);
    sw_sha256_hash_n(msg_data, num_blks, sha256_state);
}
#endif
/*****************************************************************************/
