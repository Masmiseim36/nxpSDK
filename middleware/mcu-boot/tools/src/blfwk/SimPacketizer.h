/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _sim_packetizer_h_
#define _sim_packetizer_h_

#include "Packetizer.h"
#include "SimPeripheral.h"
#include "packet/command_packet.h"

namespace blfwk
{
// Forward declarations.
class Peripheral;

/*!
 * @brief Provides source and sink for packets in the simulator space.
 */
class SimPacketizer : public Packetizer
{
public:
    //! @brief Constants
    enum _simPacketizer_contants
    {
        kSimReadTimeoutMs = 5000
    };

public:
    //! @brief Default Constructor.
    SimPacketizer(SimPeripheral *peripheral);

    //! @brief Destructor.
    virtual ~SimPacketizer();

    //! @brief Peripheral accessor.
    virtual SimPeripheral *getPeripheral() { return dynamic_cast<SimPeripheral *>(m_peripheral); }
    //! @brief Read a packet.
    //!
    //! Provides the address of a buffer containing the packet.
    //!
    //! @param packet Pointer location to write packet pointer
    //! @param packetLength Number of bytes in returned packet.
    virtual status_t readPacket(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType);

    //! @brief Write a packet.
    //!
    //! @param packet Pointer to packet to write.
    //! @param byteCount Number of bytes in packet.
    virtual status_t writePacket(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType);

    //! @brief Abort data phase.
    virtual void abortPacket() { setAborted(true); }
    //! @brief Send framing packet ack.
    virtual void sync(){};

    //! @brief Finalize.
    virtual void finalize();

    //! @brief Enable simulator command processor pump.
    virtual void enableSimulatorPump() { m_isPumpEnabled = true; }
    //! @brief Pump simulator command processor.
    virtual void pumpSimulator();

    //! @brief Set aborted flag.
    //!
    //! Used for out-of-band flow control for simulator.
    virtual void setAborted(bool aborted) { m_isAborted = aborted; }
    //! @brief Return the max packet size.
    virtual uint32_t getMaxPacketSize() { return kDefaultMaxPacketSize; }
protected:
    bool m_isPumpEnabled;                    //!< True if simulator pump enabled.
    bool m_isAborted;                        //!< Data phase abort requested by receiver.
    uint8_t m_buffer[kDefaultMaxPacketSize]; //!< Buffer for bytes used to build read packet.
};

} // namespace blfwk

#endif // _sim_packetizer_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
