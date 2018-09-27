/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _serial_packetizer_h_
#define _serial_packetizer_h_

#include "Packetizer.h"
#include "UartPeripheral.h"
#include "bootloader_common.h"
#include "packet/serial_packet.h"

//! @addtogroup serial_packetizer
//! @{

namespace blfwk
{
// Forward declarations.
class Peripheral;

/*!
 * @brief Provides source and sink for packets that go over the serial peripherals.
 */
class SerialPacketizer : public Packetizer
{
public:
    //! @brief Constructor.
    SerialPacketizer(UartPeripheral *peripheral, uint32_t packetTimeoutMs);

    //! @brief Destructor.
    virtual ~SerialPacketizer();

    //! @brief Peripheral accessor.
    virtual UartPeripheral *getPeripheral() { return dynamic_cast<UartPeripheral *>(m_peripheral); }
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
    virtual void abortPacket();

    //! @brief Send framing packet ack.
    virtual void sync();

    //! @brief Finalize.
    virtual void finalize();

    //! @brief Enable simulator command processor pump.
    virtual void enableSimulatorPump() {}
    //! @brief Pump simulator command processor.
    virtual void pumpSimulator() {}
    //! @brief Set aborted flag.
    virtual void setAborted(bool aborted) {}
    //! @brief Return the max packet size.
    virtual uint32_t getMaxPacketSize();

    //! @brief Delay milliseconds.
    void host_delay(uint32_t milliseconds);

    //! @brief Send a ping packet and receive an ack.
    //!
    //! This is a method for host only side pinging of the target. The reponse from the
    //! target to a ping packet is a ping response packet. Since the target may or may
    //! not be online there is optionally a series of retries to make the best attempt
    //! at communication possible
    //!
    //! @param retries The number of attempts that should be made.
    //! @param delay The time in milliseconds between each attempt.
    //! @param comSpeed The peripheral baud rate. Used in order to calculate the
    //!     receive delay in the case of low com speeds such as 100 and 300 which need
    //!     nearly a second to complete
    virtual status_t ping(int retries, unsigned int delay, ping_response_t *response, int comSpeed, int *actualComSpeed);

protected:
    //! @brief Send ACK if needed.
    status_t send_deferred_ack();

    //! @brief Read packet using serial framing.
    //!
    //! On return, caller must call flow control method to send AckContinue or AckWait followed by Continue.
    status_t serial_packet_read(uint8_t **packet, uint32_t *packetLength, packet_type_t packetType);

    //! @brief Write packet using serial framing.
    status_t serial_packet_write(const uint8_t *packet, uint32_t byteCount, packet_type_t packetType);

    //! @brief Abort data phase.
    //!
    //! Respond to next host data packet with AckAbort instead of Ack
    //! (i.e. receiver data phase abort).
    void serial_packet_abort();

    //! @brief Get max packet size.
    uint32_t serial_packet_get_max_packet_size();

    //! @brief Send a sync packet of the specified type.
    status_t serial_packet_send_sync(uint8_t framingPacketType);

    //! @brief Send a ping message back in response to a ping.
    status_t serial_send_ping_response();

    //! @brief Wait for an ACK, handling NAKs as needed.
    status_t wait_for_ack_packet();

    //! @brief Read from peripheral until entire data framing packet read.
    status_t read_data_packet(framing_data_packet_t *packet, uint8_t *data, packet_type_t packetType);

    //! @brief Read from peripheral until start byte found.
    status_t read_start_byte(framing_header_t *header);

    //! @brief Read from peripheral until packet header found.
    status_t read_header(framing_header_t *header);

    //! @brief Read from peripheral until packet length found.
    status_t read_length(framing_data_packet_t *packet);

    //! @brief Read from peripheral until crc16 is found.
    status_t read_crc16(framing_data_packet_t *packet);

    //! @brief Calculate crc over framing data packet.
    uint16_t calculate_framing_crc16(framing_data_packet_t *packet, const uint8_t *data);

    serial_data_t m_serialContext;
};

} // namespace blfwk

//! @}

#endif // _serial_packetizer_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
