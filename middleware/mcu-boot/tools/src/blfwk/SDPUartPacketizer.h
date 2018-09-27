/*
* Copyright (c) 2015 Freescale Semiconductor, Inc.
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _sdp_uart_packetizer_h_
#define _sdp_uart_packetizer_h_

#include "Packetizer.h"
#include "UartPeripheral.h"
#include "serial.h"

#pragma once

//! @addtogroup sdp_uart_packetizer
//! @{

namespace blfwk
{

/*!
* @brief Provides source and sink for SDP packets that go over UART.
*/
class SDPUartPacketizer : public Packetizer
{
public:
    //! @brief Constants.
    enum _constants
    {
        kMaxReadSizeBytes = 64,         //!< Max bytes returned by single read.
        kNumBytesCommandResponse = 4,   //!< Number of bytes in a kPacketTypeCommand read.
    };

public:
    //! @brief Default Constructor.
    SDPUartPacketizer(UartPeripheral *peripheral, uint32_t readPacketTimeoutMs)
        : Packetizer(peripheral, readPacketTimeoutMs)
        , m_readBuf()
    {
    }

    //! @brief Destructor.
    virtual ~SDPUartPacketizer() {}

    //! @brief Read a packet.
    //!
    //! Provides the address of a buffer containing the packet.
    //!
    //! @param packet Pointer location to write packet pointer
    //! @param packetLength Number of bytes in returned packet
    virtual status_t readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType)
    {
        uint32_t count = ((packetType == kPacketType_Command) || (m_readCount == 0)) ? kNumBytesCommandResponse : m_readCount;
        if (count > sizeof(m_readBuf))
        {
            count = sizeof(m_readBuf);
        }
        status_t status = getPeripheral()->read(m_readBuf, count, packetLength, m_packetTimeoutMs);
        *packet = m_readBuf;
        return status;
    }

    //! @brief Write a packet.
    //!
    //! @param packet Pointer to packet to write
    //! @param byteCount Number of bytes in packet
    virtual status_t writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType)
    {
        return getPeripheral()->write(packet, byteCount);
    }

    //! @brief Finalize.
    virtual void finalize() {};

    //! @brief Peripheral accessor.
    virtual UartPeripheral *getPeripheral() { return (UartPeripheral *)m_peripheral; }

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
    uint8_t m_readBuf[kMaxReadSizeBytes];   //!< Buffer for read data.
};

} // namespace blfwk

//! @}

#endif // _sdp_uart_packetizer_h_
