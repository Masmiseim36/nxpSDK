/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/SimPeripheral.h"
#include "blfwk/Logging.h"

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See sim_peripheral.h for documentation of this method.
status_t SimPeripheral::read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t timeoutMs)
{
    if (m_inStream->empty())
        return kStatus_Fail;

    uint32_t bytesRead = 0;
    for (int i = 0; i < (int)requestedBytes; ++i)
    {
        if (m_inStream->empty())
            break;
        unsigned char val = m_inStream->front();
        m_inStream->pop_front();
        buffer[i] = val;
        ++bytesRead;
    }

    if (actualBytes)
    {
        *actualBytes = bytesRead;
    }

    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes read in hex
        Log::debug2("<");
        for (int i = 0; i < (int)bytesRead; i++)
        {
            Log::debug2("%02x", buffer[i]);
            if (i != (bytesRead - 1))
            {
                Log::debug2(" ");
            }
        }
        Log::debug2(">\n");
    }

    return kStatus_Success;
}

// See sim_peripheral.h for documentation of this method.
status_t SimPeripheral::write(const uint8_t *buffer, uint32_t byteCount)
{
    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes written in hex
        Log::debug2("[");
        for (int i = 0; i < (int)byteCount; i++)
        {
            Log::debug2("%02x", buffer[i]);
            if (i != (byteCount - 1))
            {
                Log::debug2(" ");
            }
        }
        Log::debug2("]\n");
    }

    for (int i = 0; i < (int)byteCount; ++i)
    {
        m_outStream->push_back(buffer[i]);
    }

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
