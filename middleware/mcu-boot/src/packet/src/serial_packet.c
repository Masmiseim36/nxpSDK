/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "bootloader/bootloader.h"
#include "packet/command_packet.h"
#include "packet/serial_packet.h"
#include "crc/crc16.h"
#include "utilities/fsl_assert.h"
#include "microseconds.h"

//! @addtogroup packet
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//#define TEST_NAK

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static status_t write_data(const uint8_t *buffer, uint32_t byteCount);
static status_t read_data(uint8_t *buffer, uint32_t byteCount, uint32_t timeoutMs);
static status_t read_data_packet(framing_data_packet_t *packet, uint8_t *data, packet_type_t packetType);
static status_t read_start_byte(framing_header_t *header);
static status_t read_header(framing_header_t *header);
static status_t read_length(framing_data_packet_t *packet);
static status_t read_crc16(framing_data_packet_t *packet);
static status_t wait_for_ack_packet(void);
static status_t send_deferred_ack(void);
static uint16_t calculate_framing_crc16(framing_data_packet_t *packet, const uint8_t *data);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

// See peripharal.h for documentation on this interface.
const peripheral_packet_interface_t g_framingPacketInterface = {
    serial_packet_init,      serial_packet_read,     serial_packet_write,
    serial_packet_abort,     serial_packet_finalize, serial_packet_get_max_packet_size,
    serial_packet_queue_byte
};

//! @brief Ping response.
#if defined(__cplusplus)
const ping_response_t k_PingResponse = {
    MAKE_VERSION(kSerialProtocol_Version_Bugfix,
                 kSerialProtocol_Version_Minor,
                 kSerialProtocol_Version_Major,
                 kSerialProtocol_Version_Name),
    0u,     // options, recalculate crc16 if this value changes
    0xeaaau // crc16 of start byte, packet type, version and options.
           // i.e. [5a a7 00 00 01 50 00 00]
           // Calculated using CRC-16/XMODEM.
};
#else
const ping_response_t k_PingResponse = {
    { { (uint8_t)kSerialProtocol_Version_Bugfix, (uint8_t)kSerialProtocol_Version_Minor, (uint8_t)kSerialProtocol_Version_Major,
        (char)kSerialProtocol_Version_Name } },
    0u,     // options, recalculate crc16 if this value changes
    0xeaaau // crc16 of start byte, packet type, version and options.
           // i.e. [5a a7 00 00 01 50 00 00]
           // Calculated using CRC-16/XMODEM.
};
#endif

//! @brief Global context data.
static serial_data_t g_serialContext;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See serial_packet.h for documentation on this function.
void serial_packet_queue_byte(uint8_t byte)
{
    g_serialContext.callbackBuffer[g_serialContext.writeOffset++] = byte;
    g_serialContext.writeOffset &= (uint32_t)kCallbackBufferSize - 1u;
}

// See serial_packet.h for documentation on this function.
status_t serial_packet_init(const peripheral_descriptor_t *self)
{
    return (int32_t)kStatus_Success;
}

// See serial_packet.h for documentation on this function.
status_t serial_packet_finalize(const peripheral_descriptor_t *self)
{
    return send_deferred_ack();
}

// See serial_packet.h for documentation on this function.
status_t serial_packet_read(const peripheral_descriptor_t *self,
                            uint8_t **packet,
                            uint32_t *packetLength,
                            packet_type_t packetType)
{
    status_t status = (int32_t)kStatus_Success;
    
    if ((packet == (void *)0u) || (packetLength == (void *)0u))
    {
        debug_printf("Error: invalid packet\r\n");
        status = (int32_t)kStatus_InvalidArgument;
    }
    else
    {
        *packetLength = 0u;

        g_serialContext.isBackToBackWrite = (_Bool)false;

        // Send ACK if needed.
        status = send_deferred_ack();
        if (status == (int32_t)kStatus_Success)
        {
            framing_data_packet_t framingPacket;

            bool isPacketOk;
            do
            {
                // Clear the packet data area so unsent parameters default to zero.
                (void)memset(g_serialContext.data, 0, sizeof(g_serialContext.data));

                // Receive the framing data packet.
                isPacketOk = true;
                status = read_data_packet(&framingPacket, g_serialContext.data, packetType);
                if (status != (int32_t)kStatus_Success)
                {
                    // No packet available.
                    *packetLength = 0u;
                    break;
                }

                // Verify crc.
                uint16_t calculated_crc = calculate_framing_crc16(&framingPacket, g_serialContext.data);
                if (framingPacket.crc16 != calculated_crc)
                {
                    debug_printf("Error: invalid crc 0x%x, expected 0x%x\r\n", framingPacket.crc16, calculated_crc);
                    isPacketOk = (_Bool)false;
                }

                // Send Nak if necessary.
                if (!isPacketOk)
                {
                    (void)serial_packet_send_sync((uint8_t)kFramingPacketType_Nak);
                }
            } while (!isPacketOk);

            if (status == (int32_t)kStatus_Success)
            {
                // Indicate an ACK must be sent.
                g_serialContext.isAckNeeded = true;

                // Set caller's data buffer and length
                *packet = g_serialContext.data;
                *packetLength = framingPacket.length;
            }
        }
    }

    return status;
}

// See serial_packet.h for documentation on this function.
status_t serial_packet_write(const peripheral_descriptor_t *self,
                             const uint8_t *packet,
                             uint32_t byteCount,
                             packet_type_t packetType)
{
    status_t status =  (int32_t)kStatus_Success;

    if ((packet == (void *)0u) || (byteCount > (uint32_t)kOutgoingPacketBufferSize))
    {
        debug_printf("Error: invalid packet or packet size %d\r\n", byteCount);
        status = (int32_t)kStatus_InvalidArgument;
    }
    else
    {
        // Send ACK if needed.
        status = send_deferred_ack();
        if (status == (int32_t)kStatus_Success)
        {
            // Back-to-back writes require delay for receiver to enter peripheral read routine.
            if (g_serialContext.isBackToBackWrite)
            {
                g_serialContext.isBackToBackWrite = (_Bool)false;
            }

            // Initialize the framing data packet.
            serial_framing_packet_t *framingPacket = &g_serialContext.framingPacket;
            framingPacket->dataPacket.header.startByte = (uint8_t)kFramingPacketStartByte;
            framingPacket->dataPacket.header.packetType = (uint8_t)kFramingPacketType_Command;
            if (packetType != kPacketType_Command)
            {
                framingPacket->dataPacket.header.packetType = (uint8_t)kFramingPacketType_Data;
            }
            framingPacket->dataPacket.length = (uint16_t)byteCount;

            // Copy the caller's data buffer into the framing packet.
            if (byteCount != 0u)
            {
                (void)memcpy(framingPacket->data, packet, byteCount);
            }

            // Calculate and set the framing packet crc.
            framingPacket->dataPacket.crc16 =
                calculate_framing_crc16(&framingPacket->dataPacket, (uint8_t *)framingPacket->data);
#if defined(TEST_NAK)
            ++framingPacket->dataPacket.crc16;
#endif // TEST_NAK

            // Send the framing data packet.
            status = write_data((uint8_t *)framingPacket, sizeof(framing_data_packet_t) + byteCount);
            if (status == (int32_t)kStatus_Success)
            {
                status = wait_for_ack_packet();
            }
        }
    }

    return status;
}

// See serial_packet.h for documentation on this function.
void serial_packet_abort(const peripheral_descriptor_t *self)
{
    assert(g_serialContext.isAckNeeded);
    g_serialContext.isAckAbortNeeded = true;
    g_serialContext.isAckNeeded = (_Bool)false;
}

// See serial_packet.h for documentation on this function.
uint32_t serial_packet_get_max_packet_size(const peripheral_descriptor_t *self)
{
    return (uint32_t)kMinFramingPacketBufferSize;
}

// See serial_packet.h for documentation on this function.
status_t serial_packet_send_sync(uint8_t framingPacketType)
{
    framing_sync_packet_t sync;
    sync.header.startByte = (uint8_t)kFramingPacketStartByte;
    sync.header.packetType = framingPacketType;

    // Indicate last transaction was a write.
    g_serialContext.isBackToBackWrite = true;

    status_t status = write_data((uint8_t *)&sync, sizeof(sync));
    if (status != (int32_t)kStatus_Success)
    {
        debug_printf("Error: cannot send sync packet 0x%x, status = 0x%x\r\n", framingPacketType, status);
    }

    return status;
}

//! @brief Wait for an ACK, handling NAKs as needed.
static status_t wait_for_ack_packet(void)
{
    status_t status = (int32_t)kStatus_Success;
    framing_sync_packet_t sync;
    do
    {
        // Receive the sync packet.
        status = read_header(&sync.header);
        if (status != (int32_t)kStatus_Success)
        {
            break;
        }

        if ((sync.header.packetType != (uint8_t)kFramingPacketType_Ack) && (sync.header.packetType != (uint8_t)kFramingPacketType_Nak) &&
            (sync.header.packetType != (uint8_t)kFramingPacketType_AckAbort))
        {
            debug_printf("Error: Unexpected sync byte 0x%x received, expected Ack, AckAbort or Nak\r\n",
                         sync.header.packetType);
            status = (int32_t)kStatus_InvalidArgument;
            break;
        }

        if (sync.header.packetType == (uint8_t)kFramingPacketType_AckAbort)
        {
            status = (int32_t)kStatus_AbortDataPhase;
            break;
        }

        if (sync.header.packetType == (uint8_t)kFramingPacketType_Nak)
        {
// Re-transmit the last packet.
#if defined(TEST_NAK)
            --g_serialContext.framingPacket.dataPacket.crc16;
#endif // TEST_NAK
            status = write_data((uint8_t *)&g_serialContext.framingPacket,
                                sizeof(framing_data_packet_t) + (uint32_t)g_serialContext.framingPacket.dataPacket.length);
            if (status != (int32_t)kStatus_Success)
            {
                break;
            }
        }
    } while (sync.header.packetType == (uint8_t)kFramingPacketType_Nak);

    return status;
}

// See serial_packet.h for documentation on this function.
status_t serial_send_ping_response(const peripheral_descriptor_t *peripheral)
{
    assert(peripheral);

    // Only reply if we're in an idle state
    if (!g_serialContext.isAckNeeded || !g_serialContext.isBackToBackWrite || !g_serialContext.isAckAbortNeeded)
    {
        const uint8_t header[] = { (uint8_t)kFramingPacketStartByte, (uint8_t)kFramingPacketType_PingResponse };
        (void)peripheral->byteInterface->write(peripheral, (const uint8_t *)&header, sizeof(header));
        (void)peripheral->byteInterface->write(peripheral, (const uint8_t *)&k_PingResponse, sizeof(k_PingResponse));
    }

    return (int32_t)kStatus_Ping;
}

//! @brief Send ACK if needed.
static status_t send_deferred_ack(void)
{
    status_t status = (int32_t)kStatus_Success;

    if (g_serialContext.isAckNeeded)
    {
        // Send Ack for last received packet.
        g_serialContext.isAckNeeded = (_Bool)false;
        status = serial_packet_send_sync((uint8_t)kFramingPacketType_Ack);
    }
    else if (g_serialContext.isAckAbortNeeded)
    {
        // Send AckAbort for last received packet.
        g_serialContext.isAckAbortNeeded = (_Bool)false;
        status = serial_packet_send_sync((uint8_t)kFramingPacketType_AckAbort);
    }
    else
    {
        status = (int32_t)kStatus_Success;
    }

    return status;
}

//! @brief Write buffer to peripheral until all bytes sent.
static status_t write_data(const uint8_t *buffer, uint32_t byteCount)
{
    status_t retVal;

    retVal = g_bootloaderContext.activePeripheral->byteInterface->write(g_bootloaderContext.activePeripheral, buffer,
                                                                        byteCount);

    return retVal;
}

//! @brief Read from peripheral until specified number of bytes received.
static status_t read_data(uint8_t *buffer, uint32_t byteCount, uint32_t timeoutMs)
{
    // On the target we read from our interrupt buffer
    uint32_t currentBytesRead = 0u;
    volatile uint64_t startTicks = microseconds_get_ticks();
    __ISB();
    uint64_t timeOutTicks = microseconds_convert_to_ticks(timeoutMs * 4000u);
    uint64_t endTicks = startTicks;
    uint64_t deltaTicks = 0u;
    status_t status = (int32_t)kStatus_Success;

    while (currentBytesRead != byteCount)
    {
        endTicks = microseconds_get_ticks();
        deltaTicks = endTicks - startTicks;

        // Check timer roll over
        if (endTicks < startTicks)
        {
            deltaTicks = endTicks + (~startTicks) + 1u;
        }

        if ((timeOutTicks != 0u) && (deltaTicks >= timeOutTicks))
        {
            status = (int32_t)kStatus_Timeout;
            break;
        }

        if (g_serialContext.readOffset != g_serialContext.writeOffset)
        {
            buffer[currentBytesRead++] = g_serialContext.callbackBuffer[g_serialContext.readOffset++];

            g_serialContext.readOffset &= (uint32_t)kCallbackBufferSize - 1u;
        }
    }

    return status;
}

//! @brief Read from peripheral until entire data framing packet read.
static status_t read_data_packet(framing_data_packet_t *packet, uint8_t *data, packet_type_t packetType)
{
    // Read the packet header.
    status_t status = read_header(&packet->header);
    if (status == (int32_t)kStatus_Success)
    {
        if (packet->header.packetType == (uint8_t)kFramingPacketType_Ping)
        {
            status = serial_send_ping_response(g_bootloaderContext.activePeripheral);
        }
        else
        {
            uint8_t expectedPacketType = (uint8_t)kFramingPacketType_Command;

            if (packetType != kPacketType_Command)
            {
                expectedPacketType = (uint8_t)kFramingPacketType_Data;
            }
            if (packet->header.packetType != expectedPacketType)
            {
                debug_printf("Error: read_data_packet found unexpected packet type 0x%x\r\n", packet->header.packetType);
                status = (int32_t)kStatus_Fail;
            }
            else
            {
                // Read the packet length.
                status = read_length(packet);
                if (status == (int32_t)kStatus_Success)
                {
                    // Make sure the packet doesn't exceed the allocated buffer size.
                    packet->length = MIN((uint16_t)kIncomingPacketBufferSize, packet->length);

                    // Read the crc
                    status = read_crc16(packet);
                    if (status == (int32_t)kStatus_Success)
                    {
                        // Read the data.
                        if (packet->length > 0u)
                        {
                            // Clear the data area so unsent parameters default to zero.
                            (void)memset(data, 0, packet->length);

                            status = read_data(data, packet->length, (uint32_t)kDefaultByteReadTimeoutMs * packet->length);
                        }
                    }
                }
            }
        }
    }
    return status;
}

//! @brief Read from peripheral until start byte found.
static status_t read_start_byte(framing_header_t *header)
{
    status_t status = (int32_t)kStatus_Success;
    // Read until start byte found.
    do
    {
        status = read_data(&header->startByte, 1u, 0u); // no timeout for first byte of packet
        if (status != (int32_t)kStatus_Success)
        {
            break;
        }
    } while (header->startByte != (uint8_t)kFramingPacketStartByte);

    return status;
}

//! @brief Read from peripheral until packet header found.
static status_t read_header(framing_header_t *header)
{
    // Wait for start byte.
    status_t status = read_start_byte(header);
    if (status == (int32_t)kStatus_Success)
    {
        status = read_data(&header->packetType, sizeof(header->packetType),
                     (uint8_t)kDefaultByteReadTimeoutMs * sizeof(header->packetType));
    }

    return status;
}

//! @brief Read from peripheral until packet length found.
static status_t read_length(framing_data_packet_t *packet)
{
    union
    {
        uint8_t bytes[sizeof(uint16_t)];
        uint16_t halfword;
    } buffer;

    status_t status = read_data((uint8_t *)&buffer.bytes, sizeof(buffer), (uint8_t)kDefaultByteReadTimeoutMs * sizeof(buffer));

    packet->length = buffer.halfword;
    return status;
}

//! @brief Read from peripheral until crc16 is found.
static status_t read_crc16(framing_data_packet_t *packet)
{
    union
    {
        uint8_t bytes[sizeof(uint16_t)];
        uint16_t halfword;
    } buffer;

    status_t status = read_data((uint8_t *)&buffer.bytes, sizeof(buffer), (uint8_t)kDefaultByteReadTimeoutMs * sizeof(buffer));

    packet->crc16 = buffer.halfword;
    return status;
}

//! @brief Calculate crc over framing data packet.
static uint16_t calculate_framing_crc16(framing_data_packet_t *packet, const uint8_t *data)
{
    uint16_t crc16;

    // Initialize the CRC16 information
    crc16_data_t crcInfo;
    crc16_init(&crcInfo);

    union
    {
        const uint8_t *src;
        void *startByte;
    } crc_src;

    crc_src.startByte = (void *)&packet->header.startByte;

    // Run CRC on all header bytes besides the CRC field
    crc16_update(&crcInfo, crc_src.src, sizeof(framing_data_packet_t) - sizeof(uint16_t));

    // Continue running CRC on any payload bytes
    crc16_update(&crcInfo, data, packet->length);

    // Finalize the CRC calculations
    crc16_finalize(&crcInfo, &crc16);

    return crc16;
}

//! @}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
