/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_AESCounter_h_)
#define _AESCounter_h_

#include <string.h>
#include <iostream>
#include "Random.h"

//! An AES-128 counter is 128 bits, or 16 bytes.
typedef uint8_t aes128_counter_t[16];

/*!
 * \brief Base class for AESCounter<S>.
 *
 * This class implements some bigger, non-template methods used in the
 * AESCounter<S> templated subclass.
 */
class AESCounterBase
{
public:
    //! \brief Reads hex encoded data from \a stream.
    void _readFromStream(std::istream &stream, unsigned bytes, uint8_t *buffer);

    //! \brief Writes hex encoded data to \a stream.
    void _writeToStream(std::ostream &stream, unsigned bytes, const uint8_t *buffer) const;
};

/*!
 * \brief Generic AES Counter class.
 *
 * The template parameter \a S is the number of bits in the counter.
 *
 * The underlying counter type can be accessed like this: AESCounter<128>::counter_t
 *
 * When a counter instance is destroyed, it erases the counter data by setting it
 * to all zeroes.
 *
 * \todo Add a way to allow only counter sizes of 128, 192, and 256 bits.
 * \todo Find a cross platform way to prevent the counter data from being written
 *       to the VM swapfile.
 *
 * AESCounter<128> counter = AESCounter<128>::readFromStream(s);
 */
template <int S>
class AESCounter : public AESCounterBase
{
public:
    //! Type for this size of AES counter.
    typedef uint8_t counter_t[S / 8];

public:
    //! \brief Default constructor.
    //!
    //! Initializes the counter to 0.
    AESCounter() { memset(m_counter, 0, sizeof(m_counter)); }
    //! \brief Constructor taking a counter value reference.
    AESCounter(const counter_t &counter) { memcpy(m_counter, &counter, sizeof(m_counter)); }
    // \brief Constructor taking a counter value pointer.
    AESCounter(const counter_t *counter) { memcpy(m_counter, counter, sizeof(m_counter)); }
    //! \brief Constructor, reads counter from stream in hex format.
    AESCounter(std::istream &stream) { readFromStream(stream); }
    //! \brief Copy constructor.
    AESCounter(const AESCounter<S> &other) { memcpy(m_counter, other.m_counter, sizeof(m_counter)); }
    //! \brief Destructor.
    //!
    //! Sets the counter value to zero.
    ~AESCounter() { memset(m_counter, 0, sizeof(m_counter)); }
    //! \brief Set to the counter to a randomly generated value.
    void randomize()
    {
        RandomNumberGenerator rng;
        rng.generateBlock(m_counter, sizeof(m_counter));
    }

    //! \brief Reads the counter from a hex encoded data stream.
    void readFromStream(std::istream &stream)
    {
        _readFromStream(stream, S / 8, reinterpret_cast<uint8_t *>(&m_counter));
    }

    //! \brief Writes the counter to a data stream in hex encoded format.
    void writeToStream(std::ostream &stream) const
    {
        _writeToStream(stream, S / 8, reinterpret_cast<const uint8_t *>(&m_counter));
    }

    //! \brief Increments the counter by val
    void incrementCounter(unsigned val)
    {
        for (unsigned j = 0; j < val; j++)
        {
            for (unsigned i = sizeof(AESCounter<S>::counter_t) - 1, carry = 1; (i >= 0) && carry; i--)
            {
                carry = !++m_counter[i];
            }
        }
    }

    //! \name Counter accessors
    //@{
    inline const counter_t &getCounter() const { return m_counter; }
    inline void getCounter(counter_t *counter) const { memcpy(counter, m_counter, sizeof(m_counter)); }
    inline void setCounter(const counter_t &counter) { memcpy(m_counter, &counter, sizeof(m_counter)); }
    inline void setCounter(const counter_t *counter) { memcpy(m_counter, counter, sizeof(m_counter)); }
    inline void setCounter(const AESCounter<S> &counter) { memcpy(m_counter, counter.m_counter, sizeof(m_counter)); }
    //@}

    //! \name Operators
    //@{
    const AESCounter<S> &operator=(const AESCounter<S> &counter)
    {
        setCounter(counter);
        return *this;
    }
    const AESCounter<S> &operator=(const counter_t &counter)
    {
        setCounter(counter);
        return *this;
    }
    const AESCounter<S> &operator=(const counter_t *counter)
    {
        setCounter(counter);
        return *this;
    }

    operator const counter_t &() const { return m_counter; }
    operator const counter_t *() const { return m_counter; }
    friend std::ostream &operator<<(std::ostream &os, const AESCounter<S> &counter)
    {
        counter.writeToStream(os);
        return os;
    }
    //@}

protected:
    counter_t m_counter; //!< The counter value.
};

//! Standard type definition for an AES-128 counter.
typedef AESCounter<128> AES128Counter;

#endif // _AESCounter_h_
