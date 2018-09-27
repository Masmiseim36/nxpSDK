/*
 * File:    RijndaelCTR.cpp
 *
 * Copyright (c) Freescale Semiconductor, Inc. All rights reserved.
 * See included license file for license details.
 */

#include "blfwk/RijndaelCTR.h"
#include "blfwk/rijndael.h"
#include <assert.h>
#include "blfwk/Logging.h"

void logHexArray(Logger::log_level_t level, const uint8_t *bytes, unsigned count);

//! \param key The key to use
//! \param counter The counter to use
RijndaelCTR::RijndaelCTR(const AESKey<128> &key, const AESCounter<128> &counter)
    : m_key(key)
    , m_counter(counter)
{
}

void RijndaelCTR::decrypt(const uint8_t *data, unsigned length, uint8_t *dest)
{
    baseProcess(data, length, dest);
}

void RijndaelCTR::encrypt(const uint8_t *data, unsigned length, uint8_t *dest)
{
    baseProcess(data, length, dest);
}

void RijndaelCTR::baseProcess(const uint8_t *data, unsigned length, uint8_t *dest)
{
    Rijndael cipher;
    unsigned blocks = length / BLOCK_SIZE;
    block_t remainder;
    AESCounter<128>::counter_t currentCounter;

    cipher.init(Rijndael::ECB, Rijndael::Encrypt, m_key, Rijndael::Key16Bytes);

    while (blocks--)
    {
        m_counter.getCounter(&currentCounter);

        cipher.blockEncrypt(currentCounter, BLOCK_SIZE * 8, currentCounter);

        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            dest[i] = data[i] ^ currentCounter[i];
        }

        data += BLOCK_SIZE;
        dest += BLOCK_SIZE;

        m_counter.incrementCounter(BLOCK_SIZE);
    }

    if (length % sizeof(block_t))
    {
        m_counter.getCounter(&currentCounter);

        memset(remainder, 0, sizeof(remainder));
        memcpy(remainder, data, length % sizeof(block_t));

        cipher.blockEncrypt(currentCounter, BLOCK_SIZE * 8, currentCounter);

        for (unsigned int i = 0; i < length % sizeof(block_t); i++)
        {
            remainder[i] = data[i] ^ currentCounter[i];
        }

        memcpy(dest, remainder, length % sizeof(block_t));

        m_counter.incrementCounter(BLOCK_SIZE);
    }
}
