/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "security/aes_security.h"

// * - 00010000 - AES
#define LTC_MODE_AES 0x10

// ECB sub algorithm for AES mode
#define LTC_AES_ALG_ECB 0x20

// Algorithm state values
// * - 00 - Update
// * - 01 - Initialize
// * - 10 - Finalize
// * - 11 - Initialize/Finalize
#define LTC_ALGORITHM_STATE_UPDATE 0
#define LTC_ALGORITHM_STATE_INIT 1
#define LTC_ALGORITHM_STATE_FINALIZE 2
#define LTC_ALGORITHM_STATE_INIT_FIN 3

// LTC Mode values for the ENC field
// * - 0 - Decrypt.
// * - 1 - Encrypt.
#define LTC_DECRYPT 0
#define LTC_ENCRYPT 1

// Define LTC0 as LTC to make SW compatible with others.
#ifndef LTC0
#if defined(LTC)
#define LTC0 LTC
#endif
#endif

// Endian swaps each word in a 16 byte block
static void reverse_bytes_block(uint32_t *data)
{
    uint32_t i;

    for (i = 0; i < AES_BLOCK_SIZE_WORDS; i++)
    {
        data[i] = ((data[i] & (uint32_t)0x000000ffUL) << 24) | ((data[i] & (uint32_t)0x0000ff00UL) << 8) |
                  ((data[i] & (uint32_t)0x00ff0000UL) >> 8) | ((data[i] & (uint32_t)0xff000000UL) >> 24);
    }
}

void ltc_aes_init(uint32_t *key)
{
// Enable the LTC module
#if defined(SIM_SCGC2_LTC_MASK)
    SIM->SCGC2 |= SIM_SCGC2_LTC_MASK;
#elif defined(SIM_SCGC5_LTC_MASK)
    SIM->SCGC5 |= SIM_SCGC5_LTC_MASK;
#else
#error "Unknown LTC clock gate"
#endif
}

void ltc_aes_encrypt(uint32_t *in, uint32_t *key, uint32_t *out)
{
    // Clear everything
    LTC0->CW = ~0U;

    reverse_bytes_block(key);
    reverse_bytes_block(in);

    // Write the key
    LTC0->KEY[0] = key[0];
    LTC0->KEY[1] = key[1];
    LTC0->KEY[2] = key[2];
    LTC0->KEY[3] = key[3];

    // Write the key size
    LTC0->KS = (AES_128_KEY_SIZE & LTC_KS_KS_MASK);

    LTC0->MD = (LTC_MD_ALG(LTC_MODE_AES) |                // AES Mode
                LTC_MD_AAI(LTC_AES_ALG_ECB) |             // AES ECB Algorithm
                LTC_MD_AS(LTC_ALGORITHM_STATE_INIT_FIN) | // As per the RM set these bits although other parts of the
                                                          // manual say it doesnt use this
                LTC_MD_ENC(LTC_ENCRYPT));                 // We are encrypting

    // Write the data size
    LTC0->DS = (AES_BLOCK_SIZE & LTC_DS_DS_MASK);

    // Clear the done interrupt/bit
    LTC0->STA |= LTC_STA_DI_MASK;

    // write the data
    LTC0->IFIFO = in[0];
    LTC0->IFIFO = in[1];
    LTC0->IFIFO = in[2];
    LTC0->IFIFO = in[3];

    // wait for done bit
    while (!(LTC0->STA & LTC_STA_DI_MASK))
        ;

    // read out the data
    out[0] = LTC0->OFIFO;
    out[1] = LTC0->OFIFO;
    out[2] = LTC0->OFIFO;
    out[3] = LTC0->OFIFO;

    // Clear the done interrupt/bit
    LTC0->STA |= LTC_STA_DI_MASK;

    reverse_bytes_block(key);
    reverse_bytes_block(in);
    if (in != out)
    {
        reverse_bytes_block(out);
    }
    LTC0->DS = 0;
}

void ltc_aes_decrypt(uint32_t *in, uint32_t *key, uint32_t *out)
{
    // Clear everything
    LTC0->CW = ~0U;

    reverse_bytes_block(key);
    reverse_bytes_block(in);

    // Write the key
    LTC0->KEY[0] = key[0];
    LTC0->KEY[1] = key[1];
    LTC0->KEY[2] = key[2];
    LTC0->KEY[3] = key[3];

    // Write the key size
    LTC0->KS = (AES_128_KEY_SIZE & LTC_KS_KS_MASK);

    LTC0->MD = (LTC_MD_ALG(LTC_MODE_AES) |                // AES Mode
                LTC_MD_AAI(LTC_AES_ALG_ECB) |             // AES ECB Algorithm
                LTC_MD_AS(LTC_ALGORITHM_STATE_INIT_FIN) | // As per the RM set these bits although other parts of the
                                                          // manual say it doesnt use this
                LTC_MD_ENC(LTC_DECRYPT));                 // We are decrypting

    // Write the data size
    LTC0->DS = (AES_BLOCK_SIZE & LTC_DS_DS_MASK);

    // Clear the done interrupt/bit
    LTC0->STA |= LTC_STA_DI_MASK;

    // write the data
    LTC0->IFIFO = in[0];
    LTC0->IFIFO = in[1];
    LTC0->IFIFO = in[2];
    LTC0->IFIFO = in[3];

    // wait for done bit
    while (!(LTC0->STA & LTC_STA_DI_MASK))
        ;

    // read out the data
    // read out the data
    out[0] = LTC0->OFIFO;
    out[1] = LTC0->OFIFO;
    out[2] = LTC0->OFIFO;
    out[3] = LTC0->OFIFO;

    // Clear the done interrupt/bit
    LTC0->STA |= LTC_STA_DI_MASK;

    reverse_bytes_block(key);
    reverse_bytes_block(in);
    if (in != out)
    {
        reverse_bytes_block(out);
    }
    LTC0->DS = 0;
}
