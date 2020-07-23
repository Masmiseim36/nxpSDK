/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/UsbHidPeripheral.h"
#include "blfwk/format_string.h"
#include "blfwk/smart_ptr.h"
#include "blfwk/Logging.h"

#include "bootloader_hid_report_ids.h"

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See UsbHidPeripheral.h for documentation of this method.
UsbHidPeripheral::UsbHidPeripheral()
    : m_vendor_id(kDefault_Vid)
    , m_product_id(kDefault_Pid)
    , m_path("")
{
    if (!init())
    {
        throw std::runtime_error(
            format_string("Error: UsbHidPeripheral() cannot open USB HID device (vid=0x%04x, pid=0x%04x).", m_vendor_id,
                          m_product_id));
    }
}

// See UsbHidPeripheral.h for documentation of this method.
UsbHidPeripheral::UsbHidPeripheral(unsigned short vendor_id,
                                   unsigned short product_id,
                                   const char *serial_number,
                                   const char *path)
    : m_vendor_id(vendor_id)
    , m_product_id(product_id)
    , m_path(path)
{
    // Convert to a wchar_t*
    std::string s(serial_number);
    m_serial_number.assign(s.begin(), s.end());

    if (!init())
    {
        if (m_path.empty())
        {
            throw std::runtime_error(
                format_string("Error: UsbHidPeripheral() cannot open USB HID device (vid=0x%04x, pid=0x%04x, sn=%s).",
                              vendor_id, product_id, s.c_str()));
        }
        else
        {
            throw std::runtime_error(
                format_string("Error: UsbHidPeripheral() cannot open USB HID device (path=%s).", m_path.c_str()));
        }
    }
}

// See UsbHidPeripheral.h for documentation of this method.
bool UsbHidPeripheral::init()
{
    // Open the device using the VID, PID,
    // and optionally the Serial number.
    if (m_path.empty())
    {
        m_device = hid_open(m_vendor_id, m_product_id, m_serial_number.c_str());
    }
    else
    {
        std::string path = m_path;
#if defined(WIN32)
        // if not start with string "\\\\.\\", then add it.
        if ((path.find("\\\\.\\") != 0) && (path.find("\\\\?\\") != 0))
        {
            // Add the prefix string.
            path = "\\\\.\\" + path;
        }

        // Replace all '\\' with '#', except the 4 prefix characters.
        while (path.find('\\', 4) != std::string::npos)
        {
            path.replace(path.find('\\', 4), 1, "#");
        }

        // if not end with string "#{4d1e55b2-f16f-11cf-88cb-001111000030}", then add it.
        if (path.find("#{4d1e55b2-f16f-11cf-88cb-001111000030}") !=
            (path.length() - sizeof("#{4d1e55b2-f16f-11cf-88cb-001111000030}") + 1 /*Add line terminator back*/))
        {
            path.append("#{4d1e55b2-f16f-11cf-88cb-001111000030}");
        }
#endif
#if _DEBUG
        Log::info("USB Device Path = %s.\n", path.c_str());
#endif
        m_device = hid_open_path(path.c_str());
    }
    if (!m_device)
    {
        return false;
    }

    return true;
}

// See UsbHidPeripheral.h for documentation of this method.
UsbHidPeripheral::~UsbHidPeripheral()
{
    if (m_device)
    {
        hid_close(m_device);
        /* Free static HIDAPI objects. */
        hid_exit();
    }
}

// See UsbHidPeripheral.h for documentation of this method.
status_t UsbHidPeripheral::read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t timeout)
{
    assert(buffer);

    // Read the requested number of bytes.
    int count = hid_read_timeout(m_device, buffer, requestedBytes, timeout);
    if (actualBytes)
    {
        *actualBytes = count;
    }

    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes read in hex
        Log::debug2("<");
        for (int i = 0; i < (int)count; i++)
        {
            Log::debug2("%02x", buffer[i]);
            if (i != (count - 1))
            {
                Log::debug2(" ");
            }
        }
        Log::debug2(">\n");
    }

    // Bail if we got an error (-1), or if the number of bytes read was less than
    // the report header.
    if (count < (int)sizeof(bl_hid_header_t))
    {
        if (count == -1)
        {
            return kStatus_Fail;
        }
        else
        {
            return kStatus_Timeout;
        }
    }

    return kStatus_Success;
}

// See UsbHidPeripheral.h for documentation of this method.
status_t UsbHidPeripheral::write(const uint8_t *buffer, uint32_t byteCount, uint32_t timeoutMS)
{
    assert(buffer);

    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes written in hex
        Log::debug2("[");
        for (uint32_t i = 0; i < byteCount; i++)
        {
            Log::debug2("%02x", buffer[i]);
            if (i != (byteCount - 1))
            {
                Log::debug2(" ");
            }
        }
        Log::debug2("]\n");
    }

    int count = hid_write_timeout(m_device, buffer, byteCount, timeoutMS);
    if (count < 0)
    {
        const wchar_t *errorMessage = hid_error(m_device);
        if (errorMessage)
        {
            int len = wcslen(errorMessage);
            char *msg = new char[len + 1];
            wcstombs(msg, errorMessage, len + 1);
            Log::error("%s", msg);
            delete[] msg;
        }
        return kStatus_Fail;
    }

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
