/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _Packetizer_h_
#define _Packetizer_h_

#include "bootloader_common.h"
#include "bootloader/bl_peripheral.h"

#include <time.h>

//! @addtogroup host_packetizers
//! @{

namespace blfwk
{
// Forward declarations.
class Peripheral;

//! @brief Packetizer status codes.
enum _packetizer_status
{
    kStatus_NoPingResponse = MAKE_STATUS(kStatusGroup_Packetizer, 0),
    kStatus_InvalidPacketType = MAKE_STATUS(kStatusGroup_Packetizer, 1),
    kStatus_InvalidCRC = MAKE_STATUS(kStatusGroup_Packetizer, 2),
    kStatus_NoCommandResponse = MAKE_STATUS(kStatusGroup_Packetizer, 3)
};

/*!
 * @brief Interface class for packetization of commands and data.
 */
class Packetizer
{
public:
    Packetizer(Peripheral *peripheral, uint32_t packetTimeoutMs)
        : m_peripheral(peripheral)
        , m_packetTimeoutMs(packetTimeoutMs)
        , m_version()
        , m_options(0)
        , m_isAbortEnabled(false)
        , m_readCount(0)
    {
    }

    virtual ~Packetizer(){};

    //! @brief Read a packet.
    virtual status_t readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType) = 0;

    //! @brief Write a packet.
    virtual status_t writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType) = 0;

    //! @brief Abort data phase.
    virtual void abortPacket() = 0;

    //! @brief Send framing packet ack.
    virtual void sync() = 0;

    //! @brief Finalize.
    virtual void finalize() = 0;

    //! @brief Enable simulator command processor pump.
    virtual void enableSimulatorPump() = 0;

    //! @brief Pump simulator command processor.
    virtual void pumpSimulator() = 0;

    //! @brief Set aborted flag.
    //!
    //! Used for out-of-band flow control for simulator.
    virtual void setAborted(bool aborted) = 0;

    //! @brief Return the max packet size.
    virtual uint32_t getMaxPacketSize() = 0;

    //! @brieif Optional control of number of bytes requested from peripheral by readPacket().
    virtual void setReadCount(uint32_t byteCount) { m_readCount = byteCount; }
    //! @brief Peripheral accessor.
    virtual Peripheral *getPeripheral() { return m_peripheral; }
    //! @brief Get Framing Protocol Version
    standard_version_t getVersion() { return m_version; }
    //! @brief Get Framing Protocol Options
    uint16_t getOptions() { return m_options; }
    //! @brief Set abort packet check enable.
    void setAbortEnabled(bool isEnabled) { m_isAbortEnabled = isEnabled; }
    //! @biref Check if abort data phase is enabled.
    bool isAbortEnabled() { return m_isAbortEnabled; }
protected:
    Peripheral *m_peripheral;     //!< Peripheral to send/receive bytes on.
    standard_version_t m_version; //!< Framing protocol version.
    uint16_t m_options;           //!< Framing protocol options bitfield.
    uint32_t m_packetTimeoutMs;
    bool m_isAbortEnabled; //!< True if allowing abort packet. Not used by all packetizers.
    uint32_t m_readCount;  //!< Optional control of number of bytes requested by readPacket().
};

} // namespace blfwk

//! @}

#endif // _Packetizer_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
