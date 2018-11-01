/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdafx.h"
#include "SerialList.h"

#define KEY_SERIALCOMM_PATH _T("HARDWARE\\DEVICEMAP\\SERIALCOMM")

// See SerialList.h for documentation of this method.
SerialList *g_pAllComDevices;

// See SerialList.h for documentation of this method.
void SerialList::ScanSerialDevices()
{
    if (m_serialDevices.size() != 0)
    {
        m_serialDevices.clear();
    }

    HKEY hKey;
    int i = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_SERIALCOMM_PATH, NULL, KEY_READ, &hKey);
    if (i == ERROR_SUCCESS)
    {
        int i = 0, status;
        TCHAR Name[128], Data[32];
        DWORD dNameLength, dDataLength;
        do
        {
            dNameLength = sizeof(Name);
            dDataLength = sizeof(Data);

            status = ::RegEnumValue(hKey, i, Name, &dNameLength, NULL, NULL, (PUCHAR)Data, &dDataLength);
            if ((status == ERROR_SUCCESS) && (dDataLength != 0))
            {
                m_serialDevices.push_back(SerialDevice(Data));
            }
            i++;
        } while (status != ERROR_NO_MORE_ITEMS);
    }
    ::RegCloseKey(hKey);

    SortSerialDevices();
}

// See SerialList.h for documentation of this method.
void SerialList::SortSerialDevices()
{
    size_t length = GetDeviceCount();
    for (size_t i = 0; i < length; i++)
    {
        for (size_t j = i + 1; j < length; j++)
        {
            if (GetDevice(i).GetNum() > GetDevice(j).GetNum())
            {
                std::swap(m_serialDevices[i], m_serialDevices[j]);
            }
        }
    }
}