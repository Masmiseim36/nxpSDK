/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * Copyright 2021 ACRIOS Systems s.r.o.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_message_buffer.h"

#include <cassert>
#include <cstring>

using namespace erpc;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

erpc_status_t MessageBuffer::read(uint16_t offset, void *data, uint32_t length)
{
    erpc_status_t err;

    if ((offset + length) > m_len)
    {
        err = kErpcStatus_BufferOverrun;
    }
    else
    {
        if (length > 0U)
        {
            memcpy(data, &m_buf[offset], length);
        }

        err = kErpcStatus_Success;
    }

    return err;
}

erpc_status_t MessageBuffer::write(uint16_t offset, const void *data, uint32_t length)
{
    erpc_status_t err;

    if ((offset + length) > m_len)
    {
        err = kErpcStatus_BufferOverrun;
    }
    else
    {
        if (length > 0U)
        {
            memcpy(m_buf, data, length);
        }

        err = kErpcStatus_Success;
    }

    return err;
}

erpc_status_t MessageBuffer::copy(const MessageBuffer *other)
{
    assert(m_len >= other->m_len);

    m_used = other->m_used;
    memcpy(m_buf, other->m_buf, m_used);

    return kErpcStatus_Success;
}

void MessageBuffer::swap(MessageBuffer *other)
{
    assert(other);

    MessageBuffer temp(*other);

    other->m_len = m_len;
    other->m_used = m_used;
    other->m_buf = m_buf;
    m_len = temp.m_len;
    m_used = temp.m_used;
    m_buf = temp.m_buf;
}

void MessageBuffer::Cursor::set(MessageBuffer *buffer)
{
    m_buffer = buffer;
    // RPMSG when nested calls are enabled can set NULL buffer.
    // assert(buffer->get() && "Data buffer wasn't set to MessageBuffer.");
    // receive function should return err if it couldn't set data buffer.
    m_pos = buffer->get();
    m_remaining = buffer->getLength();
}

erpc_status_t MessageBuffer::Cursor::read(void *data, uint32_t length)
{
    assert(m_pos && "Data buffer wasn't set to MessageBuffer.");

    erpc_status_t err;

    if (m_remaining < length)
    {
        err = kErpcStatus_BufferOverrun;
    }
    else
    {
        memcpy(data, m_pos, length);
        m_pos += length;
        m_remaining -= length;

        err = kErpcStatus_Success;
    }

    return err;
}

erpc_status_t MessageBuffer::Cursor::write(const void *data, uint32_t length)
{
    assert(m_pos && "Data buffer wasn't set to MessageBuffer.");

    erpc_status_t err;

    if (length > m_remaining)
    {
        err = kErpcStatus_BufferOverrun;
    }
    else
    {
        memcpy(m_pos, data, length);
        m_pos += length;
        m_remaining -= length;
        m_buffer->setUsed(m_buffer->getUsed() + length);

        err = kErpcStatus_Success;
    }

    return err;
}

erpc_status_t MessageBufferFactory::prepareServerBufferForSend(MessageBuffer *message)
{
    message->setUsed(0);
    return kErpcStatus_Success;
}
