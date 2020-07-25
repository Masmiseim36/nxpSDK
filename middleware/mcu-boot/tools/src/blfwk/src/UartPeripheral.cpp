/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/UartPeripheral.h"
#include "blfwk/Logging.h"
#include "blfwk/format_string.h"
#include "blfwk/serial.h"

#if defined(Linux)
#include <termios.h>
#endif

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See uart_peripheral.h for documentation of this method.
UartPeripheral::UartPeripheral(const char *port, long speed)
    : m_fileDescriptor(-1)
{
    if (!init(port, speed))
    {
        throw std::runtime_error(
            format_string("Error: UartPeripheral() cannot open PC UART port(%s), speed(%d Hz).", port, speed));
    }
}

// See uart_peripheral.h for documentation of this method.
bool UartPeripheral::init(const char *port, long speed)
{
    assert(port);

    strcpy(UartPeripheral::port_name, const_cast<char*>(port));

    if (m_fileDescriptor != -1)
    {
        serial_close(m_fileDescriptor);
    }
    // Open the port.
    m_fileDescriptor = serial_open(UartPeripheral::port_name);
    if (m_fileDescriptor == -1)
    {
        return false;
    }
#if defined(LINUX)
    // Non standard baud rates depend on the specific system.
    // Only supports standard baud rates.
    switch (speed)
    {
        case 9600:
            speed = B9600;
            break;
        case 19200:
            speed = B19200;
            break;
        case 38400:
            speed = B38400;
            break;
        case 57600:
            speed = B57600;
            break;
        case 115200:
            speed = B115200;
            break;
            default:
            Log::warning("Warning: unsupported standard baud rate(%d), set to default(57600)\n", speed);
            speed = B57600;
            break;
    }
#endif
    serial_setup(m_fileDescriptor, speed);

    // Flush garbage from receive buffer before setting read timeout.
    flushRX();

    // Set host serial timeout to 10 milliseconds. Higherlevel timeouts are implemented in
    // SerialPacketizer.cpp
    serial_set_read_timeout(m_fileDescriptor, kUartPeripheral_DefaultReadTimeoutMs);

    return true;
}

bool UartPeripheral::update(long speed)
{
    char port_name[255];
    strcpy(port_name, UartPeripheral::port_name);
    return UartPeripheral::init(port_name, speed);
}

// See host_peripheral.h for documentation of this method.
UartPeripheral::~UartPeripheral()
{
    if (m_fileDescriptor != -1)
    {
        serial_close(m_fileDescriptor);
    }
}

// See host_peripheral.h for documentation of this method.
status_t UartPeripheral::read(uint8_t *buffer,
                              uint32_t requestedBytes,
                              uint32_t *actualBytes,
                              uint32_t unused_timeoutMs)
{
    assert(buffer);

    // Read the requested number of bytes.
    int count = serial_read(m_fileDescriptor, reinterpret_cast<char *>(buffer), requestedBytes);
    if (actualBytes)
    {
        *actualBytes = count;
    }

    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes read in hex
        Log::debug2("<");
        for (int i = 0; i < (int)count; i++)
        {
            Log::debug2("%02x", buffer[i]);
            if (i != (count - 1))
            {
                Log::debug2(" ");
            }
        }
        Log::debug2(">\n");
    }

    if (count < (int)requestedBytes)
    {
        // Anything less than requestedBytes is a timeout error.
        return kStatus_Timeout;
    }

    return kStatus_Success;
}

// See uart_peripheral.h for documentation of this method.
void UartPeripheral::flushRX()
{
// An attempt was made on win32 to use a serial_flush function
// which would call PurgeComm(hCom, PURGE_RXABORT | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR)
// even though the function returned success there were still errant data in the RX buffer. Using reads
// to empty the RX buffer has worked

// This read loop never exits on the Mac, and doesn't appear to be necessary.
#if (defined(WIN32) || defined(LINUX))
    // Read up to the requested number of bytes.
    char *readBuf = reinterpret_cast<char *>(m_buffer);
    while (serial_read(m_fileDescriptor, readBuf, 16))
    {
        readBuf = reinterpret_cast<char *>(m_buffer);
    }
#endif // defined(WIN32) || defined(LINUX)
}

// See host_peripheral.h for documentation of this method.
status_t UartPeripheral::write(const uint8_t *buffer, uint32_t byteCount)
{
    assert(buffer);

    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes written in hex
        Log::debug2("[");
        for (int i = 0; i < (int)byteCount; i++)
        {
            Log::debug2("%02x", buffer[i]);
            if (i != (byteCount - 1))
            {
                Log::debug2(" ");
            }
        }
        Log::debug2("]\n");
    }

    if (serial_write(m_fileDescriptor, reinterpret_cast<char *>(const_cast<uint8_t *>(buffer)), byteCount) == byteCount)
        return kStatus_Success;
    else
        return kStatus_Fail;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
