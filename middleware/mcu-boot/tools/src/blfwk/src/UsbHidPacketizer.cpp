/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/UsbHidPacketizer.h"
#include "blfwk/Logging.h"
#include "blfwk/smart_ptr.h"
#include "blfwk/utils.h"
#ifdef LINUX
#include <string.h>
#endif

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See usb_hid_packetizer.h for documentation of this method.
UsbHidPacketizer::UsbHidPacketizer(UsbHidPeripheral *peripheral, uint32_t packetTimeoutMs)
    : Packetizer(peripheral, packetTimeoutMs)
{
    m_continuousReadCount = 0;
    memset(&m_report, 0, sizeof(m_report));
    memset(&m_abortReport, 0, sizeof(m_abortReport));
}

// See usb_hid_packetizer.h for documentation of this method.
UsbHidPacketizer::~UsbHidPacketizer()
{
    delete m_peripheral;
}

// See usb_hid_packetizer.h for documentation of this method.
status_t UsbHidPacketizer::writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType)
{
    assert(m_peripheral);
    if (byteCount)
    {
        assert(packet);
    }

    // Determine report ID based on packet type.
    uint8_t reportID;
    switch (packetType)
    {
        case kPacketType_Command:
            reportID = kBootloaderReportID_CommandOut;
            break;
        case kPacketType_Data:
            reportID = kBootloaderReportID_DataOut;
            break;
        default:
            Log::error("usbhid: unsupported packet type %d\n", (int)packetType);
            return kStatus_Fail;
    };

    if (m_isAbortEnabled && (reportID == kBootloaderReportID_DataOut))
    {
        // Check if the target has sent an abort report.
        if (pollForAbortPacket())
        {
            Log::info("usb hid detected receiver data abort\n");
            return kStatus_AbortDataPhase;
        }
    }

    // Construct report contents.
    memset(&m_report, 0, sizeof(m_report));
    m_report.header.reportID = reportID;
    m_report.header.packetLengthLsb = byteCount & 0xff;
    m_report.header.packetLengthMsb = (byteCount >> 8) & 0xff;

    // If not a zero-length report, copy in packet data.
    if (byteCount)
    {
        memcpy(m_report.packet, packet, byteCount);
    }

    m_continuousReadCount = 0;

    return getPeripheral()->write((uint8_t *)&m_report, sizeof(bl_hid_header_t) + byteCount, m_packetTimeoutMs);
}

// See usb_hid_packetizer.h for documentation of this method.
status_t UsbHidPacketizer::readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType)
{
    assert(m_peripheral);
    assert(packet);
    assert(packetLength);
    *packet = NULL;
    *packetLength = 0;

    // Determine report ID based on packet type.
    uint8_t reportID;
    switch (packetType)
    {
        case kPacketType_Command:
            reportID = kBootloaderReportID_CommandIn;
            break;
        case kPacketType_Data:
            reportID = kBootloaderReportID_DataIn;
            break;
        default:
            Log::error("usbhid: unsupported packet type %d\n", (int)packetType);
            return kStatus_Fail;
    };

    // Read report.
    uint32_t actualBytes = 0;
    uint16_t lengthInPacket = 0;
    uint32_t retryCnt = 0;
    do
    {
        status_t retVal =
            m_peripheral->read((unsigned char *)&m_report, sizeof(m_report), &actualBytes, m_packetTimeoutMs);
        if (retVal != kStatus_Success)
        {
            return retVal;
        }

        if (!retryCnt)
        {
            m_continuousReadCount++;
        }

        // Check the report ID.
        if (m_report.header.reportID != reportID)
        {
            Log::error("usbhid: received unexpected report=%x\n", m_report.header.reportID);
            return kStatus_Fail;
        }

        // Extract the packet length encoded as bytes 1 and 2 of the report. The packet length
        // is transferred in little endian byte order.
        lengthInPacket = m_report.header.packetLengthLsb | (m_report.header.packetLengthMsb << 8);

        // See if we received the data abort packet,
        // if the data abort packet was received, try to read pending generic response packet.
        if (lengthInPacket == 0)
        {
            Log::info("usbhid: received data phase abort\n");

            // If continuous read occurs, that means this is a read-memory command,
            // host tool shouldn't wait for pending packet because there are no pending packets.
            if (m_continuousReadCount >= kContinuousReadMargin)
            {
                break;
            }
            retryCnt++;
        }
        else if (lengthInPacket > getMaxPacketSize())
        {
            Log::error("Data packet size(%d) is bigger than max supported size(%d).", lengthInPacket,
                       getMaxPacketSize());
            return kStatus_Fail;
        }
    } while (actualBytes && !lengthInPacket && retryCnt < kPollPacketMaxRetryCnt);

    // Make sure we got all of the packet. Target will send the maximum
    // report size, so there may be extra trailing bytes.
    if ((actualBytes - sizeof(m_report.header)) < lengthInPacket)
    {
        Log::error("usbhid: received only %d bytes of packet with length %d\n", actualBytes - sizeof(m_report.header),
                   lengthInPacket);
        return kStatus_Fail;
    }

    // Return results.
    *packet = m_report.packet;
    *packetLength = lengthInPacket;

    return kStatus_Success;
}

// See usb_hid_packetizer.h for documentation of this method.
void UsbHidPacketizer::flushInput()
{
    uint32_t actualBytes = 0;
    do
    {
        m_peripheral->read((unsigned char *)&m_report, sizeof(m_report), &actualBytes, m_packetTimeoutMs);
    } while (actualBytes > 0);
}

// See usb_hid_packetizer.h for documentation of this method.
void UsbHidPacketizer::abortPacket()
{
    // Abort data phase by writing a zero-length command packet.
    writePacket(NULL, 0, kPacketType_Command);
    flushInput();
}

bool UsbHidPacketizer::pollForAbortPacket()
{
    // Just check to see if there is data to be read from hid device.
    // No reason to wait (milliseconds = 0), because we aren't really expecting anything.
    uint32_t actualBytes = 0;
    m_peripheral->read((unsigned char *)&m_abortReport, sizeof(m_abortReport), &actualBytes, kPollAbortTimeoutMs);
    if (actualBytes == 0)
    {
        // No abort packet
        return false;
    }
    else
    {
        // Got an abort packet
        return true;
    }
}

// See usb_hid_packetizer.h for documentation of this method.
uint32_t UsbHidPacketizer::getMaxPacketSize()
{
    return kMaxHostPacketSize;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
