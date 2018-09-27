/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/SerialPacketizer.h"
#include "blfwk/Logging.h"
#include "blfwk/utils.h"
#include "crc/crc16.h"
#ifdef WIN32
#include <windows.h>
#endif
#ifdef LINUX
#include <string.h>
#include <unistd.h>
#endif

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
enum
{
    kReadRetries = 10,
    kReadDelayMilliseconds = 10
};

//! @brief Ping response.
const ping_response_t k_PingResponse = {
    0x50010203,
    //    { kSerialProtocol_Version_Bugfix,
    //      kSerialProtocol_Version_Minor,
    //      kSerialProtocol_Version_Major,
    //      kSerialProtocol_Version_Name },
    0,     // options, recalculate crc16 if this value changes
    0xeaaa // crc16 of start byte, packet type, version and options.
           // i.e. [5a a7 00 00 01 50 00 00]
           // Calculated using CRC-16/XMODEM.
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See SerialPacketizer.h for documentation of this method.
SerialPacketizer::SerialPacketizer(UartPeripheral *peripheral, uint32_t packetTimeoutMs)
    : Packetizer(dynamic_cast<Peripheral *>(peripheral), packetTimeoutMs)
{
    // Clear the initial serial context
    memset(&m_serialContext, 0, sizeof(m_serialContext));
}

// See SerialPacketizer.h for documentation of this method.
SerialPacketizer::~SerialPacketizer()
{
    delete m_peripheral;
}

// See SerialPacketizer.h for documentation of this method.
void SerialPacketizer::finalize()
{
    send_deferred_ack();
}

// See SerialPacketizer.h for documentation of this method.
status_t SerialPacketizer::writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType)
{
    return serial_packet_write(packet, byteCount, packetType);
}

// See SerialPacketizer.h for documentation of this method.
status_t SerialPacketizer::readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType)
{
    return serial_packet_read(packet, packetLength, packetType);
}

// See SerialPacketizer.h for documentation of this method.
void SerialPacketizer::abortPacket()
{
    serial_packet_abort();
}

// See SerialPacketizer.h for documentation of this method.
void SerialPacketizer::sync()
{
    serial_packet_send_sync(kFramingPacketType_Ack);
}

// See SerialPacketizer.h for documentation of this method.
uint32_t SerialPacketizer::getMaxPacketSize()
{
    return serial_packet_get_max_packet_size();
}

// See SerialPacketizer.h for documentation of this method.
void SerialPacketizer::host_delay(uint32_t milliseconds)
{
// @todo implement for non-win32
#if defined(WIN32)
    Sleep(milliseconds);
#elif defined(LINUX)
    usleep(milliseconds * 1000);
#endif
}

// See SerialPacketizer.h for documentation of this method.
status_t SerialPacketizer::ping(
    int retries, unsigned int delay, ping_response_t *pingResponse, int comSpeed, int *actualComSpeed)
{
    status_t status = kStatus_NoPingResponse;
    uint8_t startByte = 0;
    uint32_t bytesRead = 0;
    const int initialRetries = retries;
    static bool retryDoubledBaudrate = true;

    if (actualComSpeed != NULL)
    {
        *actualComSpeed = comSpeed;
    }
    else
    {
        // Always use the specified baudrate if the actualComSpeed is invalid
        if (m_peripheral->get_type() == Peripheral::kHostPeripheralType_UART)
        {
            ((UartPeripheral *)m_peripheral)->update(comSpeed);
        }
    }

    framing_header_t pingPacket;
    pingPacket.startByte = kFramingPacketStartByte;
    pingPacket.packetType = kFramingPacketType_Ping;

    // Send ping until we receive a start byte.
    do
    {
        // Send the ping
        if (m_peripheral->write((uint8_t *)&pingPacket, sizeof(pingPacket)) == kStatus_Success)
        {
            double timeout = 0.500;
            double duration = 0.0;
#if defined(WIN32) || defined(LINUX)
            clock_t start = clock();
#endif

            // Try for half a second to get a response from the ping.
            while (duration < timeout)
            {
                if (m_peripheral->read(&startByte, sizeof(startByte), &bytesRead,
                                       UartPeripheral::kUartPeripheral_UnusedTimeout) == kStatus_Success)
                {
                    if (startByte == kFramingPacketStartByte)
                    {
                        break;
                    }
                }

                host_delay(kReadDelayMilliseconds);
#if defined(WIN32)
                duration = (double)(clock() - start) / CLOCKS_PER_SEC; // Windows: CLOCKS_PER_SEC = 1,000.
#elif defined(LINUX)
                duration = (double)(clock() - start) / (CLOCKS_PER_SEC / 1000); // Linux: CLOCKS_PER_SEC = 1,000,000.
#endif
            }

            // If we got our start byte, move on to read the response packet
            if (startByte == kFramingPacketStartByte)
            {
                break;
            }
#if 0
            else if (m_peripheral->get_type() == Peripheral::kHostPeripheralType_UART)
            {
                // Try to double the baudrate if the startByte is incorrect
                if (retryDoubledBaudrate)
                {
                    retries = 1;
                    retryDoubledBaudrate = false;
                    ((UartPeripheral*)m_peripheral)->update(comSpeed * 2);

                    if (actualComSpeed != NULL)
                    {
                        *actualComSpeed = comSpeed;
                    }
                }
            }
#endif
        }

        host_delay(delay);

    } while (retries--);

    if (startByte == kFramingPacketStartByte)
    {
        Log::info("Ping responded in %d attempt(s)\n", (initialRetries - retries) + 1);

        // Wait for the rest of the ping bytes
        // In the case of testing low baud rates the target needs time to respond
        // 100 baud rate reply is looking for 9 more bytes = 90 bits with start/stop overhead
        // 90 bits / 100 baud = .9 seconds = 900 milliseconds. The additional 20 milliseconds is to ensure
        // that even high baud rates have some sort of delay and to give a little wiggle room for lower baud rates
        if (comSpeed)
        {
            host_delay(((1000 * 90) / comSpeed) + 20);
        }

        // Read response packet type.
        uint8_t packetType;
        status = m_peripheral->read(&packetType, sizeof(packetType), &bytesRead,
                                    UartPeripheral::kUartPeripheral_UnusedTimeout);
        if (status == kStatus_Success)
        {
            if (packetType == kFramingPacketType_PingResponse)
            {
                // Read response.
                ping_response_t response;
                status = m_peripheral->read((uint8_t *)&response, sizeof(response), &bytesRead,
                                            UartPeripheral::kUartPeripheral_UnusedTimeout);
                if (status == kStatus_Success)
                {
                    // Validate reponse CRC.

                    // Initialize the CRC16 information.
                    uint16_t crc16;
                    crc16_data_t crcInfo;
                    crc16_init(&crcInfo);

                    // Include the start byte and packetType in the CRC.
                    crc16_update(&crcInfo, &startByte, sizeof(startByte));
                    crc16_update(&crcInfo, &packetType, sizeof(packetType));

                    // Run CRC on all other bytes except the CRC field.
                    crc16_update(&crcInfo, (uint8_t *)&response, sizeof(response) - sizeof(uint16_t));

                    // Finalize the CRC calculations
                    crc16_finalize(&crcInfo, &crc16);

                    if (response.crc16 == crc16)
                    {
                        Log::debug("Framing protocol version = 0x%x, options = 0x%x\n", response.version,
                                   response.options);
                        m_version = response.version;
                        m_options = response.options;

                        if (pingResponse)
                        {
                            *pingResponse = response;
                        }

                        status = kStatus_Success;
                    }
                    else
                    {
                        Log::info("Error: ping crc16 failed, received 0x%x, expected 0x%x\n", response.crc16, crc16);
                        status = kStatus_InvalidCRC;
                    }
                }
            }
            else
            {
                status = kStatus_InvalidPacketType;
            }
        }
    }

#if 0
    // Reset the acutalComSpeed to comSpeed if the autobaud detection failed.
    if (status != kStatus_Success)
    {
        retryDoubledBaudrate = true;
        if (actualComSpeed != NULL)
        {
            *actualComSpeed = comSpeed;
        }
    }
#endif

    return status;
}

// Private Implementation

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::serial_packet_read(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType)
{
    if (!packet || !packetLength)
    {
        Log::error("Error: invalid packet\r\n");
        return kStatus_InvalidArgument;
    }
    *packetLength = 0;
    status_t status;

    m_serialContext.isBackToBackWrite = false;

    // Send ACK if needed.
    status = send_deferred_ack();
    if (status != kStatus_Success)
    {
        return status;
    }

    framing_data_packet_t framingPacket;

    bool isPacketOk;
    do
    {
        // Clear the packet data area so unsent parameters default to zero.
        memset(m_serialContext.data, 0, sizeof(m_serialContext.data));

        // Receive the framing data packet.
        isPacketOk = true;
        status_t status = read_data_packet(&framingPacket, m_serialContext.data, packetType);
        if (status != kStatus_Success)
        {
            // No packet available.
            *packetLength = 0;
            return status;
        }

        // Verify crc.
        uint16_t calculated_crc = calculate_framing_crc16(&framingPacket, m_serialContext.data);
        if (framingPacket.crc16 != calculated_crc)
        {
            Log::error("Error: invalid crc 0x%x, expected 0x%x\r\n", framingPacket.crc16, calculated_crc);
            isPacketOk = false;
        }

        // Send Nak if necessary.
        if (!isPacketOk)
        {
            serial_packet_send_sync(kFramingPacketType_Nak);
        }
    } while (!isPacketOk);

    // Indicate an ACK must be sent.
    m_serialContext.isAckNeeded = true;

    // Set caller's data buffer and length
    *packet = m_serialContext.data;
    *packetLength = framingPacket.length;

    return kStatus_Success;
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::serial_packet_write(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType)
{
    if (!packet || (byteCount > kOutgoingPacketBufferSize))
    {
        Log::error("Error: invalid packet or packet size %d\r\n", byteCount);
        return kStatus_InvalidArgument;
    }

    // Send ACK if needed.
    status_t status = send_deferred_ack();
    if (status != kStatus_Success)
    {
        return status;
    }

    // Back-to-back writes require delay for receiver to enter peripheral read routine.
    if (m_serialContext.isBackToBackWrite)
    {
        m_serialContext.isBackToBackWrite = false;

        host_delay(100);
    }

    // Initialize the framing data packet.
    serial_framing_packet_t *framingPacket = &m_serialContext.framingPacket;
    framingPacket->dataPacket.header.startByte = kFramingPacketStartByte;
    framingPacket->dataPacket.header.packetType = kFramingPacketType_Command;
    if (packetType != kPacketType_Command)
    {
        framingPacket->dataPacket.header.packetType = kFramingPacketType_Data;
    }
    framingPacket->dataPacket.length = (uint16_t)byteCount;

    // Copy the caller's data buffer into the framing packet.
    if (byteCount)
    {
        memcpy(framingPacket->data, packet, byteCount);
    }

    // Calculate and set the framing packet crc.
    framingPacket->dataPacket.crc16 =
        calculate_framing_crc16(&framingPacket->dataPacket, (uint8_t *)framingPacket->data);
#if defined(TEST_NAK)
    ++framingPacket->dataPacket.crc16;
#endif // TEST_NAK

    // Send the framing data packet.
    status = m_peripheral->write((uint8_t *)framingPacket, sizeof(framing_data_packet_t) + byteCount);
    if (status != kStatus_Success)
    {
        return status;
    }

    return wait_for_ack_packet();
}

// See SerialPacketizer.h for documentation on this function.
void SerialPacketizer::serial_packet_abort()
{
    assert(m_serialContext.isAckNeeded);
    m_serialContext.isAckAbortNeeded = true;
    m_serialContext.isAckNeeded = false;
}

// See SerialPacketizer.h for documentation on this function.
uint32_t SerialPacketizer::serial_packet_get_max_packet_size()
{
    return kMaxHostPacketSize;
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::serial_packet_send_sync(uint8_t framingPacketType)
{
    framing_sync_packet_t sync;
    sync.header.startByte = kFramingPacketStartByte;
    sync.header.packetType = framingPacketType;

    // Indicate last transaction was a write.
    m_serialContext.isBackToBackWrite = true;

    status_t status = m_peripheral->write((uint8_t *)&sync, sizeof(sync));
    if (status != kStatus_Success)
    {
        Log::error("Error: cannot send sync packet 0x%x, status = 0x%x\r\n", framingPacketType, status);
    }

    return status;
}

// See SerialPacketizer.h for documentation of this method.
status_t SerialPacketizer::wait_for_ack_packet()
{
    framing_sync_packet_t sync;
    status_t status = kStatus_NoCommandResponse;

    do
    {
        // Receive the sync packet.
        status = read_header(&sync.header);
        if (status != kStatus_Success)
        {
            break;
        }

        if ((sync.header.packetType != kFramingPacketType_Ack) && (sync.header.packetType != kFramingPacketType_Nak) &&
            (sync.header.packetType != kFramingPacketType_AckAbort))
        {
            Log::error("Error: Unexpected sync byte 0x%x received, expected Ack, AckAbort or Nak\r\n",
                       sync.header.packetType);
            status = kStatus_InvalidPacketType;
            break;
        }

        if (sync.header.packetType == kFramingPacketType_AckAbort)
        {
            status = kStatus_AbortDataPhase;
            break;
        }

        if (sync.header.packetType == kFramingPacketType_Nak)
        {
// Re-transmit the last packet.
#if defined(TEST_NAK)
            --g_serialContext.framingPacket.dataPacket.crc16;
#endif // TEST_NAK
            status =
                m_peripheral->write((uint8_t *)&m_serialContext.framingPacket,
                                    sizeof(framing_data_packet_t) + m_serialContext.framingPacket.dataPacket.length);
            if (status != kStatus_Success)
            {
                break;
            }
        }

    } while (sync.header.packetType == kFramingPacketType_Nak);

    return status;
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::serial_send_ping_response()
{
    assert(m_peripheral);

    // Only reply if we're in an idle state
    if (!m_serialContext.isAckNeeded || !m_serialContext.isBackToBackWrite || !m_serialContext.isAckAbortNeeded)
    {
        const uint8_t header[] = { kFramingPacketStartByte, kFramingPacketType_PingResponse };
        m_peripheral->write((const uint8_t *)&header, sizeof(header));
        m_peripheral->write((uint8_t *)&k_PingResponse, sizeof(k_PingResponse));
    }

    return kStatus_Ping;
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::send_deferred_ack()
{
    if (m_serialContext.isAckNeeded)
    {
        // Send Ack for last received packet.
        m_serialContext.isAckNeeded = false;
        return serial_packet_send_sync(kFramingPacketType_Ack);
    }
    else if (m_serialContext.isAckAbortNeeded)
    {
        // Send AckAbort for last received packet.
        m_serialContext.isAckAbortNeeded = false;
        return serial_packet_send_sync(kFramingPacketType_AckAbort);
    }
    else
    {
        return kStatus_Success;
    }
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::read_data_packet(framing_data_packet_t *packet, uint8_t *data, packet_type_t packetType)
{
    // Read the packet header.
    status_t status = read_header(&packet->header);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (packet->header.packetType == kFramingPacketType_Ping)
    {
        return serial_send_ping_response();
    }

    uint8_t expectedPacketType = kFramingPacketType_Command;

    if (packetType != kPacketType_Command)
    {
        expectedPacketType = kFramingPacketType_Data;
    }
    if (packet->header.packetType != expectedPacketType)
    {
        Log::error("Error: read_data_packet found unexpected packet type 0x%x\r\n", packet->header.packetType);
        return kStatus_Fail;
    }

    // Read the packet length.
    status = read_length(packet);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Make sure the packet doesn't exceed the allocated buffer size.
    if (packet->length > getMaxPacketSize())
    {
        Log::error("Error: Data packet size(%d) is bigger than max supported size(%d)\r\n", packet->length,
                   getMaxPacketSize());
        return kStatus_Fail;
    }

    // Read the crc
    status = read_crc16(packet);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Read the data.
    if (packet->length > 0)
    {
        // Clear the data area so unsent parameters default to zero.
        memset(data, 0, packet->length);

        status = m_peripheral->read(data, packet->length, NULL, UartPeripheral::kUartPeripheral_UnusedTimeout);
    }

    return status;
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::read_start_byte(framing_header_t *header)
{
    double timeout = (double)m_packetTimeoutMs / 1000;
    double duration = 0.0;
#if defined(WIN32) || defined(LINUX)
    clock_t start = clock();
#endif

    // Read until start byte found.
    while (duration < timeout)
    {
        status_t status =
            m_peripheral->read(&header->startByte, 1, NULL, UartPeripheral::kUartPeripheral_UnusedTimeout);
        if (status != kStatus_Success && status != kStatus_Timeout)
        {
            return status;
        }

        if (header->startByte == kFramingPacketStartByte)
        {
            return kStatus_Success;
        }

        // This will keep us from doing non necessary delays in case the byte received
        // is actually the start byte, this delay and retry scenario is for cases when waiting
        // for a response from a device that was issued a long running command like a flash-erase-region
        // that may take several seconds to complete.
        host_delay(kDefaultByteReadTimeoutMs);

#if defined(WIN32)
        duration = (double)(clock() - start) / CLOCKS_PER_SEC; // Windows: CLOCKS_PER_SEC = 1,000.
#elif defined(LINUX)
        duration = (double)(clock() - start) / (CLOCKS_PER_SEC / 1000);         // Linux: CLOCKS_PER_SEC = 1,000,000.
#endif
    }

    Log::error("Error: read_start_byte() timeout after %2.3f seconds\n", duration);
    return kStatus_Timeout;
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::read_header(framing_header_t *header)
{
    // Wait for start byte.
    status_t status = read_start_byte(header);
    if (status != kStatus_Success)
    {
        return status;
    }

    return m_peripheral->read(&header->packetType, sizeof(header->packetType), NULL,
                              UartPeripheral::kUartPeripheral_UnusedTimeout);
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::read_length(framing_data_packet_t *packet)
{
    union
    {
        uint8_t bytes[sizeof(uint16_t)];
        uint16_t halfword;
    } buffer;

    status_t status = m_peripheral->read((uint8_t *)&buffer.bytes, sizeof(buffer), NULL,
                                         UartPeripheral::kUartPeripheral_UnusedTimeout);

    packet->length = buffer.halfword;
    return status;
}

// See SerialPacketizer.h for documentation on this function.
status_t SerialPacketizer::read_crc16(framing_data_packet_t *packet)
{
    union
    {
        uint8_t bytes[sizeof(uint16_t)];
        uint16_t halfword;
    } buffer;

    status_t status = m_peripheral->read((uint8_t *)&buffer.bytes, sizeof(buffer), NULL,
                                         UartPeripheral::kUartPeripheral_UnusedTimeout);

    packet->crc16 = buffer.halfword;
    return status;
}

// See SerialPacketizer.h for documentation on this function.
uint16_t SerialPacketizer::calculate_framing_crc16(framing_data_packet_t *packet, const uint8_t *data)
{
    uint16_t crc16;

    // Initialize the CRC16 information
    crc16_data_t crcInfo;
    crc16_init(&crcInfo);

    // Run CRC on all header bytes besides the CRC field
    crc16_update(&crcInfo, (uint8_t *)&packet->header.startByte, sizeof(framing_data_packet_t) - sizeof(uint16_t));

    // Continue running CRC on any payload bytes
    crc16_update(&crcInfo, data, packet->length);

    // Finalize the CRC calculations
    crc16_finalize(&crcInfo, &crc16);

    return crc16;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
