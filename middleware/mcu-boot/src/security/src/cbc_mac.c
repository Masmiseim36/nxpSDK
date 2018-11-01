/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "cbc_mac.h"
#include "aes_security.h"
#include "assert.h"
#include <string.h>

#define CBC_MAC_BLOCK_SIZE 16
#define CBC_MAC_BLOCK_SIZE_WORDS (CBC_MAC_BLOCK_SIZE / sizeof(uint32_t))

static uint32_t s_iv[CBC_MAC_BLOCK_SIZE_WORDS];
static uint32_t s_cbcKey[AES_128_KEY_SIZE_WORDS];

void cbc_mac_init(uint8_t *key, uint8_t *iv)
{
    if (key)
    {
        // Save the key
        memcpy(s_cbcKey, key, sizeof(s_cbcKey));
    }
    else
    {
        // Otherwise use a default key of all 0's
        memset(s_cbcKey, 0, sizeof(s_cbcKey));
    }

    if (iv)
    {
        // Save the IV
        memcpy(s_iv, iv, sizeof(s_iv));
    }
    else
    {
        // Otherwise use an IV of 0
        memset(s_iv, 0, sizeof(s_iv));
    }

    aes_init(s_cbcKey);
}

void cbc_mac_encrypt(uint8_t *data, uint32_t length, uint8_t *dest)
{
    assert((length % CBC_BLOCK_SIZE) == 0);
    uint32_t numBlocks = length / CBC_BLOCK_SIZE;
    uint32_t i;
    uint32_t block[CBC_MAC_BLOCK_SIZE_WORDS];

    for (i = 0; i < numBlocks; i++)
    {
        // copy into block to ensure 32 bit aligned
        memcpy(block, data, sizeof(block));

        // Before encrypting, XOR the block with the initialization vector
        block[0] ^= s_iv[0];
        block[1] ^= s_iv[1];
        block[2] ^= s_iv[2];
        block[3] ^= s_iv[3];

        // Encrypt in place
        aes_encrypt(block, s_cbcKey, block);

        // Save the generated cyphertext for the IV to the next block
        memcpy(s_iv, block, sizeof(s_iv));

        // Copy the data out to the destination
        memcpy(dest, block, sizeof(block));

        data += CBC_MAC_BLOCK_SIZE;
        dest += CBC_MAC_BLOCK_SIZE;
    }
}

void cbc_mac_decrypt(uint8_t *data, uint32_t length, uint8_t *dest)
{
    assert((length % CBC_BLOCK_SIZE) == 0);
    uint32_t numBlocks = length / CBC_BLOCK_SIZE;
    uint32_t i;
    uint32_t block[CBC_MAC_BLOCK_SIZE_WORDS];
    uint32_t tempBlock[CBC_MAC_BLOCK_SIZE_WORDS];

    for (i = 0; i < numBlocks; i++)
    {
        // Save the encrypted text before decrypting
        memcpy(block, data, sizeof(block));
        // Copy to tempBlock to ensure 32 bit alignment
        memcpy(tempBlock, data, sizeof(tempBlock));

        // Decrypt in place
        aes_decrypt(tempBlock, s_cbcKey, tempBlock);

        // XOR the block with the initialization vector
        tempBlock[0] ^= s_iv[0];
        tempBlock[1] ^= s_iv[1];
        tempBlock[2] ^= s_iv[2];
        tempBlock[3] ^= s_iv[3];

        // Set the saved encrypted text as the IV for the next block
        memcpy(s_iv, block, sizeof(block));

        // Copy data back out to the destination
        memcpy(dest, tempBlock, sizeof(tempBlock));

        data += CBC_MAC_BLOCK_SIZE;
        dest += CBC_MAC_BLOCK_SIZE;
    }
}

void cbc_mac_finalize(void)
{
}
