/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/SimPacketizer.h"
#include "blfwk/Logging.h"
#include "bootloader/bl_command.h"
#include "bootloader/bootloader.h"

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See SimPacketizer.h for documentation of this method.
SimPacketizer::SimPacketizer(SimPeripheral *peripheral)
    : Packetizer(dynamic_cast<Peripheral *>(peripheral), kSimReadTimeoutMs)
    , m_isPumpEnabled(false)
    , m_isAborted(false)
{
}

// See SimPacketizer.h for documentation of this method.
SimPacketizer::~SimPacketizer()
{
    delete m_peripheral;
}

// See SimPacketizer.h for documentation of this method.
void SimPacketizer::finalize()
{
}

// See SimPacketizer.h for documentation of this method.
status_t SimPacketizer::writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType)
{
    assert(packet);

    // Check for receiver data phase abort.
    if (m_isAborted)
    {
        m_isAborted = false;
        return kStatus_AbortDataPhase;
    }

    // Write framing packet header.
    uint8_t buffer = (uint8_t)byteCount;
    status_t status = m_peripheral->write(&buffer, 1);

    // Write the packet.
    if (byteCount && (status == kStatus_Success))
    {
        status = m_peripheral->write(packet, byteCount);
    }

    pumpSimulator();
    return status;
}

// See SimPacketizer.h for documentation of this method.
status_t SimPacketizer::readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType)
{
    assert(packet);
    assert(packetLength);
    *packet = NULL;
    *packetLength = 0;

    // Read framing packet header.
    uint8_t buffer;
    status_t status = m_peripheral->read(&buffer, 1, NULL, 0);
    if (status != kStatus_Success)
    {
        // fatal error in sumulator
        Log::error("Error: no data for readPacket\n");
        return status;
    }

    // Read the packet.
    int length = (int)(buffer);
    if (length == 0)
    {
        // zero length packet
        return kStatus_Success;
    }

    status = m_peripheral->read(m_buffer, length, NULL, 0);
    if (status != kStatus_Success)
    {
        Log::error("Error: malformed packet\n");
        return kStatus_Fail;
    }

    *packet = m_buffer;
    *packetLength = length;
    return kStatus_Success;
}

void SimPacketizer::pumpSimulator()
{
    if (m_isPumpEnabled)
    {
        bootloader_command_pump();
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
