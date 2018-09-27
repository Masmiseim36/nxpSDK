/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _sim_peripheral_h_
#define _sim_peripheral_h_

#include "blfwk/Peripheral.h"

namespace blfwk
{
/*!
 * @brief Peripheral that operates in the simulation space.
 *
 * Simulation peripherals are connected together using the streams provided to the init() method.
 * For example, to connect a "host" peripheral to a "device" peripheral, specify the host's output
 * stream as the device's input stream.
 */
class SimPeripheral : public Peripheral
{
public:
    //! @brief Default Constructor.
    SimPeripheral(uchar_deque_t *inStream, uchar_deque_t *outStream)
        : m_inStream(inStream)
        , m_outStream(outStream)
    {
    }

    //! @brief Read bytes.
    //!
    //! @param buffer Pointer to buffer.
    //! @param requestedBytes Number of bytes to read.
    //! @param actualBytes Number of bytes actually read.
    //! @param timeoutMs Time in milliseconds to wait for read to complete.
    virtual status_t read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t timeoutMs);

    //! @brief Write bytes.
    //!
    //! @param buffer Pointer to buffer to write
    //! @param byteCount Number of bytes to write
    virtual status_t write(const uint8_t *buffer, uint32_t byteCount);

protected:
    uchar_deque_t *m_inStream;  //!< Writes go to this stream.
    uchar_deque_t *m_outStream; //!< Reads come from this stream.
};

} // namespace blfwk

#endif // _sim_peripheral_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
