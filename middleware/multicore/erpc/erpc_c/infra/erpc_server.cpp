/*
 * The Clear BSD License
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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

#include "erpc_server.h"
#include "assert.h"

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

#if ERPC_NESTED_CALLS_DETECTION
bool nestingDetection = false;
#endif

void Server::setTransport(Transport *transport)
{
    m_transport = transport;
}

void Server::addService(Service *service)
{
    if (!m_firstService)
    {
        m_firstService = service;
        return;
    }

    Service *link = m_firstService;
    while (link->getNext() != NULL)
    {
        link = link->getNext();
    }

    link->setNext(service);
}

erpc_status_t Server::readHeadOfMessage(Codec *codec, message_type_t &msgType, uint32_t &serviceId, uint32_t &methodId,
                                        uint32_t &sequence)
{
    codec->startReadMessage(&msgType, &serviceId, &methodId, &sequence);
    return codec->getStatus();
}

erpc_status_t Server::processMessage(Codec *codec, message_type_t msgType, uint32_t serviceId, uint32_t methodId,
                                     uint32_t sequence)
{
    if (msgType != kInvocationMessage && msgType != kOnewayMessage)
    {
        return kErpcStatus_InvalidArgument;
    }

    Service *service = findServiceWithId(serviceId);
    if (!service)
    {
        return kErpcStatus_InvalidArgument;
    }

    return service->handleInvocation(methodId, sequence, codec, m_messageFactory);
}

Service *Server::findServiceWithId(uint32_t serviceId)
{
    Service *service = m_firstService;
    while (service != NULL)
    {
        if (service->getServiceId() == serviceId)
        {
            return service;
        }

        service = service->getNext();
    }
    return NULL;
}
