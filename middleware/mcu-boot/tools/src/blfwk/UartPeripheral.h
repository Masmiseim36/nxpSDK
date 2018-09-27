/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _uart_peripheral_h_
#define _uart_peripheral_h_

#include "Peripheral.h"
#include "packet/command_packet.h"

//! @addtogroup uart_peripheral
//! @{

namespace blfwk
{
/*!
 * @brief Peripheral that talks to the target device over COM port hardware.
 */
class UartPeripheral : public Peripheral
{
public:
    //! @breif Constants.
    enum _uart_peripheral_constants
    {
        // The read() implementation for the UartPeripheral does not use this the timeout parameter.
        kUartPeripheral_UnusedTimeout = 0,
        // Serial timeout is set to this default during init().
        kUartPeripheral_DefaultReadTimeoutMs = 1000,
        kUartPeripheral_DefaultBaudRate = 9600
    };

public:
    //! @brief Parameterized constructor that opens the serial port.
    //!
    //! Opens and configures the port. Throws exception if port configuration fails.
    //!
    //! Note: following COM port configuration is assumed: 8 bits, 1 stop bit, no parity.
    //!
    //! @param port OS file path for COM port. For example "COM1" on Windows.
    //! @param speed Port speed, e.g. 9600.
    UartPeripheral(const char *port, long speed = kUartPeripheral_DefaultBaudRate);

    //! @brief Destructor.
    virtual ~UartPeripheral();

    //! @brief Flush.
    //!
    //! should be called on an open COM port in order to flush any remaining data in the UART RX buffer
    void flushRX();

    //! @brief Read bytes.
    //!
    //! @param buffer Pointer to buffer.
    //! @param requestedBytes Number of bytes to read.
    //! @param actualBytes Number of bytes actually read.
    //! @param timeoutMs Time in milliseconds to wait for read to complete.
    virtual status_t read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t unused_timeoutMs);

    //! @brief Write bytes.
    //!
    //! @param buffer Pointer to buffer to write
    //! @param byteCount Number of bytes to write
    virtual status_t write(const uint8_t *buffer, uint32_t byteCount);

    //! @brief Return peripheral Type
    virtual _host_peripheral_types get_type(void) { return kHostPeripheralType_UART; }

    //! @brief Update.
    //!
    //! Re-configures the baudrate.
    //!
    //! Note: following COM port configuration is assumed: 8 bits, 1 stop bit, no parity.
    //!
    //! @param speed Port speed, e.g. 9600.
    bool update(long speed);

protected:
    //! @brief Initialize.
    //!
    //! Opens and configures the port.
    //!
    //! Note: following COM port configuration is assumed: 8 bits, 1 stop bit, no parity.
    //!
    //! @param port OS file path for COM port. For example "COM1" on Windows.
    //! @param speed Port speed, e.g. 9600.
    bool init(const char *port, long speed);
    char port_name[255];                     //!< Port name
    int m_fileDescriptor;                    //!< Port file descriptor.
    uint8_t m_buffer[kDefaultMaxPacketSize]; //!< Buffer for bytes used to build read packet.
};

} // namespace blfwk

//! @}

#endif // _uart_peripheral_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
