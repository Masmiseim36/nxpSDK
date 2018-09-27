/*
 * File:    RijndaelCTR.h
 *
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_RijndaelCTR_h_)
#define _RijndaelCTR_h_

#include "AESKey.h"
#include "AESCounter.h"
#include <string.h>

/*!
 * \brief Class to compute AES 128 CTR encryption/decryption.
 *
 * Currently only supports 128-bit keys and block sizes.
 */
class RijndaelCTR
{
public:
    enum
    {
        BLOCK_SIZE = 16 //!< Number of bytes in one cipher block.
    };

    //! The cipher block data type.
    typedef uint8_t block_t[BLOCK_SIZE];

public:
    //! \brief Constructor.
    RijndaelCTR(const AESKey<128> &key, const AESCounter<128> &counter);

    //! \brief Encrypt data.
    void encrypt(const uint8_t *data, unsigned length, uint8_t *dest);

    //! \brief Encrypt data.
    void decrypt(const uint8_t *data, unsigned length, uint8_t *dest);

    //! \brief Assignment operator.
    RijndaelCTR &operator=(const RijndaelCTR &other)
    {
        m_key = other.m_key;
        m_counter = other.m_counter;
        return *this;
    }

protected:
    AESKey<128> m_key;         //!< 128-bit key to use for encrypt/decrypt
    AESCounter<128> m_counter; //!< Counter value for encrypt/decrypt

    void incrementCounter(AESCounter<128>::counter_t *counter);
    void baseProcess(const uint8_t *data, unsigned length, uint8_t *dest);

private:
    //! \brief inaccessible default constructor.
    RijndaelCTR() {}
};

#endif // _RijndaelCTR_h_
