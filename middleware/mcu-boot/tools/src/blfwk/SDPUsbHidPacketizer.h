/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _sdp_usb_hid_packetizer_h_
#define _sdp_usb_hid_packetizer_h_

#include "Packetizer.h"
#include "UsbHidPeripheral.h"
#include "hidapi.h"

#pragma once

 //! @addtogroup sdp_usb_hid_packetizer
 //! @{

namespace blfwk
{

/*!
* @brief Provides source and sink for SDP packets that go over USB HID class.
*/
class SDPUsbHidPacketizer : public Packetizer
{
public:
    //! @brief Constants
    enum _usbhid_constants
    {
        kMaxReportSizeBytes = 1024 + 1, //!< Number of bytes in largest report, including report type byte.
        kIdReport1 = 1,                 //!< Code for report ID 1.
        kReport1SizeBytes = 16 + 1,     //!< Number of bytes in report 1, including report type byte.
        kIdReport2 = 2,                 //!< Code for report ID 2.
        kReport2SizeBytes = 1024 + 1,   //!< Number of bytes in report 2, including report type byte.
        kIdReport3 = 3,                 //!< Code for report ID 3.
		#if defined(WIN32)
		kReport3SizeBytes = 64 + 1,     //!< Number of bytes in report 3, including report type byte.
		#elif defined(LINUX) || defined(MACOSX)
		kReport3SizeBytes = 4 + 1,     //!< Number of bytes in report 3, including report type byte.
		#endif
        kIdReport4 = 4,                 //!< Code for report ID 4.
        kReport4SizeBytes = 64 + 1,     //!< Number of bytes in report 4, including report type byte.
        kPollPacketMaxRetryCnt = 50,    //!< Number of read retries.
    };

public:
    //! @brief Default Constructor.
    SDPUsbHidPacketizer(UsbHidPeripheral *peripheral, uint32_t readPacketTimeoutMs)
        : Packetizer(peripheral, readPacketTimeoutMs)
    {
    }

    //! @brief Destructor.
    virtual ~SDPUsbHidPacketizer() {}

    //! @brief Read a packet.
    //!
    //! Provides the address of a buffer containing the packet.
    //!
    //! @param packet Pointer location to write packet pointer
    //! @param packetLength Number of bytes in returned packet
    virtual status_t readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType);

    //! @brief Write a packet.
    //!
    //! @param packet Pointer to packet to write
    //! @param byteCount Number of bytes in packet
    virtual status_t writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType);

    //! @brief Finalize.
    virtual void finalize() {};

    //! @brief Peripheral accessor.
    virtual UsbHidPeripheral *getPeripheral() { return (UsbHidPeripheral *)m_peripheral; }

    //! @brief Abort data phase.
    virtual void abortPacket() {};

    //! @brief Send framing packet ack.
    virtual void sync() {};

    //! @brief Enable simulator command processor pump.
    virtual void enableSimulatorPump() {};

    //! @brief Pump simulator command processor.
    virtual void pumpSimulator() {};

    //! @brief Set aborted flag.
    //!
    //! Used for out-of-band flow control for simulator.
    virtual void setAborted(bool aborted) {};

    //! @brief Return the max packet size.
    virtual uint32_t getMaxPacketSize() { return 0; }

protected:
    uint8_t m_report[kMaxReportSizeBytes];  //!< Buffer for report bytes.
};

} // namespace blfwk

//! @}

#endif // _sdp_usb_hid_packetizer_h_
