/*
 * The Clear BSD License
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "erpc_framed_transport.h"
#include "erpc_message_buffer.h"
#include <cassert>
#include <cstdio>

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

FramedTransport::FramedTransport(void)
: Transport()
, m_crcImpl(NULL)
#if ERPC_THREADS
, m_sendLock()
, m_receiveLock()
#endif
{
}

FramedTransport::~FramedTransport(void) {}

void FramedTransport::setCrc16(Crc16 *crcImpl)
{
    assert(crcImpl);
    m_crcImpl = crcImpl;
}

erpc_status_t FramedTransport::receive(MessageBuffer *message)
{
    assert(m_crcImpl && "Uninitialized Crc16 object.");
    Header h;

    {
#if ERPC_THREADS
        Mutex::Guard lock(m_receiveLock);
#endif

        // Receive header first.
        erpc_status_t ret = underlyingReceive((uint8_t *)&h, sizeof(h));
        if (ret != kErpcStatus_Success)
        {
            return ret;
        }

        // received size can't be larger then buffer length.
        if (h.m_messageSize > message->getLength())
        {
            return kErpcStatus_ReceiveFailed;
        }

        // Receive rest of the message now we know its size.
        ret = underlyingReceive(message->get(), h.m_messageSize);
        if (ret != kErpcStatus_Success)
        {
            return ret;
        }
    }

    // Verify CRC.
    uint16_t computedCrc = m_crcImpl->computeCRC16(message->get(), h.m_messageSize);
    if (computedCrc != h.m_crc)
    {
        return kErpcStatus_CrcCheckFailed;
    }

    message->setUsed(h.m_messageSize);
    return kErpcStatus_Success;
}

erpc_status_t FramedTransport::send(MessageBuffer *message)
{
    assert(m_crcImpl && "Uninitialized Crc16 object.");
#if ERPC_THREADS
    Mutex::Guard lock(m_sendLock);
#endif

    uint16_t messageLength = message->getUsed();

    // Send header first.
    Header h;
    h.m_messageSize = messageLength;
    h.m_crc = m_crcImpl->computeCRC16(message->get(), messageLength);
    erpc_status_t ret = underlyingSend((uint8_t *)&h, sizeof(h));
    if (ret != kErpcStatus_Success)
    {
        return ret;
    }

    // Send the rest of the message.
    return underlyingSend(message->get(), messageLength);
}
