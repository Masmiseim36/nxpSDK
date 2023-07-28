/*!
 * \file
 *
 * This file contains the SW implementation of the SHA1 algorithm.
 *
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
#define SHA1_LeftRotate(word, bits) ((((word) << (bits)) & 0xFFFFFFFFu) | ((word) >> (32 - (bits))))

/******************************************************************************
*******************************************************************************
* Private memory definitions
*******************************************************************************
******************************************************************************/

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

void sw_sha1_initialize_output(uint32_t *sha1_state)
{
    sha1_state[0] = 0x67452301u;
    sha1_state[1] = 0xefcdab89u;
    sha1_state[2] = 0x98badcfeu;
    sha1_state[3] = 0x10325476u;
    sha1_state[4] = 0xc3d2e1f0u;
}

/*****************************************************************************/
void sw_sha1_hash_n(uint8_t *msg_data, int32_t num_blks, uint32_t *sha1_state)
{
    uint32_t i, k, f, temp;
    int32_t  n;
    uint32_t a, b, c, d, e;
    uint32_t w[80];

    // Process the message in successive 512-bit chunks:
    for (n = 0; n < num_blks; n++)
    {
        // break chunk into sixteen 32-bit big-endian words w[i], 0..15
        for (i = 0; i < 16u; i++)
        {
            uint8_t *p = &msg_data[i * sizeof(uint32_t)];
            w[i]       = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
        }

        // Extend the sixteen 32-bit words into eighty 32-bit words:
        for (i = 16; i < 80u; i++)
        {
            w[i] = SHA1_LeftRotate((w[i - 3u] ^ w[i - 8u] ^ w[i - 14u] ^ w[i - 16u]), 1);
        }

        // Initialize hash value for this chunk:
        a = sha1_state[0];
        b = sha1_state[1];
        c = sha1_state[2];
        d = sha1_state[3];
        e = sha1_state[4];

        // Main loop:
        for (i = 0; i < 80u; i++)
        {
            if (i <= 19u)
            {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999u;
            }
            else
            {
                if (i <= 39u)
                {
                    f = b ^ c ^ d;
                    k = 0x6ED9EBA1u;
                }
                else
                {
                    if (i <= 59u)
                    {
                        f = (b & c) | (b & d) | (c & d);
                        k = 0x8F1BBCDCu;
                    }
                    else
                    {
                        f = b ^ c ^ d;
                        k = 0xCA62C1D6u;
                    }
                }
            }

            temp = SHA1_LeftRotate(a, 5) + f + e + k + w[i];
            e    = d;
            d    = c;
            c    = SHA1_LeftRotate(b, 30);
            b    = a;
            a    = temp;
        }

        // Store Hash Result
        sha1_state[0] += a;
        sha1_state[1] += b;
        sha1_state[2] += c;
        sha1_state[3] += d;
        sha1_state[4] += e;

        msg_data += 64;
    }
}
#ifdef _keepUnused
/*****************************************************************************/

/*****************************************************************************/
void sw_sha1_hash(uint8_t *msg_data, uint32_t *sha1_state)
{
    sw_sha1_hash_n(msg_data, 1, sha1_state);
}

/*****************************************************************************/
void sw_sha1_update(uint8_t *msg_data, int32_t num_blks, uint32_t *sha1_state)
{
    sw_sha1_initialize_output(sha1_state);
    sw_sha1_hash_n(msg_data, num_blks, sha1_state);
}
#endif
/*****************************************************************************/
