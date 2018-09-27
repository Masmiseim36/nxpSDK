/*
* Copyright (c) 2015 Freescale Semiconductor, Inc.
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "blfwk/SDPUsbHidPacketizer.h"
#include "blfwk/Logging.h"

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

status_t SDPUsbHidPacketizer::writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType)
{
    assert(m_peripheral);
    assert(packetType == kPacketType_Command || packetType == kPacketType_Data);
    assert(byteCount);
    assert(packet);

    uint32_t numBytes = 0;

    // Construct report contents.
    memset(&m_report, 0, sizeof(m_report));
    if (packetType == kPacketType_Command)
    {
        m_report[0] = kIdReport1;       // Report 1 is used to send a command to the device.
        numBytes = kReport1SizeBytes;
    }
    else
    {
        m_report[0] = kIdReport2;       // Report 2 is used to send data to the device.
        numBytes = kReport2SizeBytes;
    }

    memcpy(&m_report[1], packet, byteCount);

    return getPeripheral()->write((uint8_t *)&m_report, numBytes, m_packetTimeoutMs);
}

status_t SDPUsbHidPacketizer::readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType)
{
    assert(m_peripheral);
    assert(packet);
    assert(packetLength);
    *packet = NULL;
    *packetLength = 0;
    // packetType is not used.

    // Read report.
    uint32_t actualBytes = 0;
    uint32_t retryCnt = 0;
    do
    {
        status_t retVal =
            m_peripheral->read((uint8_t *)&m_report, sizeof(m_report), &actualBytes, m_packetTimeoutMs);
        if (retVal != kStatus_Success)
        {
            return retVal;
        }

        if (actualBytes)
        {
            // Check the report ID.
            uint8_t reportId = m_report[0];
            if (reportId == kIdReport3)
            {
                if (actualBytes != kReport3SizeBytes)
                {
                    Log::error("usbhid: received unexpected number of bytes=%x\n", actualBytes);
                    return kStatus_Fail;
                }
            }
            else if (reportId == kIdReport4)
            {
                if (actualBytes != kReport4SizeBytes)
                {
                    Log::error("usbhid: received unexpected number of bytes=%x\n", actualBytes);
                    return kStatus_Fail;
                }
            }
            else
            {
                Log::error("usbhid: received unexpected report=%x\n", reportId);
                return kStatus_Fail;
            }
        }

    } while (!actualBytes && (++retryCnt < kPollPacketMaxRetryCnt));

    // Return results.
    *packet = &m_report[1];
    *packetLength = actualBytes - 1;

    return kStatus_Success;
}
