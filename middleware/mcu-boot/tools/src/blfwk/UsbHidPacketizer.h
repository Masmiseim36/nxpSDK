/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _usb_hid_packetizer_h_
#define _usb_hid_packetizer_h_

#include "Packetizer.h"
#include "UsbHidPeripheral.h"
#include "hidapi.h"

#include "bootloader_common.h"

#include "bootloader_hid_report_ids.h"

//! @addtogroup usb_hid_packetizer
//! @{

namespace blfwk
{
/*!
 * @brief Provides source and sink for packets that go over USB HID class.
 */
class UsbHidPacketizer : public Packetizer
{
public:
    //! @brief Constants
    enum _usbhid_contants
    {
        kReadFlushTimeoutMs = 100000,
#if defined(BL_WITHOUT_RECEIVE_SB_FILE_CMD)
        kPollAbortTimeoutMs = 0,
#else
        kPollAbortTimeoutMs = 10,
#endif
        kPollPacketMaxRetryCnt = 50,
        kContinuousReadMargin = 2,
    };
public:
    //! @brief Default Constructor.
    UsbHidPacketizer(UsbHidPeripheral *peripheral, uint32_t readPacketTimeoutMs);

    //! @brief Destructor.
    virtual ~UsbHidPacketizer();

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

    //! @brief Abort data phase.
    virtual void abortPacket();

    //! @brief Send framing packet ack/nak.
    virtual void sync(){};

    //! @brief Finalize.
    virtual void finalize(){};

    //! @brief Enable simulator command processor pump.
    virtual void enableSimulatorPump() {}
    //! @brief Pump simulator command processor.
    virtual void pumpSimulator() {}
    //! @brief Set aborted flag.
    virtual void setAborted(bool aborted) {}
    //! @brief Returns the max packet size supported
    virtual uint32_t getMaxPacketSize();

    //! @brief Peripheral accessor.
    virtual UsbHidPeripheral *getPeripheral() { return (UsbHidPeripheral *)m_peripheral; }
protected:
    //! @brief Flush input from device.
    virtual void flushInput();

    //! @brief Poll overlapped read for receiver data phase abort.
    bool pollForAbortPacket();

protected:
    bl_hid_report_t m_report;      //!< Used for building and receiving the report.
    bl_hid_report_t m_abortReport; //!< Used for received abort report.

private:
    uint32_t
        m_continuousReadCount; //!< Used for distinguish abort report for write-memory/receive-sb-file or read-memory
};

} // namespace blfwk

//! @}

#endif // _usb_hid_packetizer_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
