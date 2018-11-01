/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include <vector>
#include <cstring>

/*!
* \brief SerialDevice class represent a COM port hardware on host
*/
class SerialDevice
{
public:
    //! \brief Generated standard constructor.
    SerialDevice()
        : m_port(_T("")){};

    //! \brief Generated standard constructor with a parameter.
    SerialDevice(LPCTSTR port)
        : m_port(port){};

    //! \brief Generated standard destructor.
    virtual ~SerialDevice(){};

    //! \brief Get the string containing COM port.(COMxx)
    virtual CString GetString() { return (m_port); };
    //! \brief Get the port number only.
    virtual uint32_t GetNum() { return _ttoi(m_port.Mid(3)); };
private:
    CString m_port; //!< A string variable contains COM port string.
};

/*!
* \brief SerialList class contains all serial devices on host
*/
class SerialList
{
public:
    //! \brief Generated standard constructor.
    SerialList(){};

    //! \brief Generated standard destructor.
    virtual ~SerialList(){};

    //! \brief Get SerialDevice variable at specified index.
    SerialDevice GetDevice(int index) { return m_serialDevices.at(index); };
    //! \brief Get the number of devices in the list.
    size_t GetDeviceCount() { return m_serialDevices.size(); };
    //! \brief Find whether the specified device is in the list.
    int FindSerialDevice(SerialDevice device)
    {
        std::vector<SerialDevice>::iterator it;
        for (it = m_serialDevices.begin(); it != m_serialDevices.end(); it++)
        {
            // Check next index.
            if (device.GetNum() > it->GetNum())
            {
                continue;
            }
            // The same COM device in the list, return the index
            else if (device.GetNum() == it->GetNum())
            {
                return it - m_serialDevices.begin();
            }
            // No matched, return negative.
            else
            {
                return -1;
            }
        }
        return -1;
    }

    //! \brief Scan all COM port hardware on the host.
    void ScanSerialDevices();

    //! \brief Sort all devices in the list as ascending order
    void SortSerialDevices();

private:
    std::vector<SerialDevice> m_serialDevices; //!< Serial Device list.
};

extern SerialList *g_pAllComDevices; //!< Global point to SerialList class.
