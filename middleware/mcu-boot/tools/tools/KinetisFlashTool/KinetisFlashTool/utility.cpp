/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdafx.h"
#include "utility.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdexcept>
#include <string.h>
#include <stdlib.h>

//! Size of the temporary buffer to hold the formatted output string.
#define WIN32_FMT_BUF_LEN (512)

/*!
* \brief Simple template class to free a pointer.
*/
template <typename T>
class free_ptr
{
public:
    //! \brief Constructor.
    free_ptr(T ptr)
        : m_p(ptr)
    {
    }

    //! \brief Destructor.
    ~free_ptr()
    {
        if (m_p)
        {
            free(m_p);
        }
    }

protected:
    T m_p; //!< The value to free.
};

// See utility.h for documentation of this method.
CString format_cstring(const wchar_t *format, ...)
{
    LPWSTR buffer;
    va_list args;
    int len;
    int result = -1;

    va_start(args, format);
    len = _vsctprintf(format, args) + 1;
    buffer = (LPWSTR)malloc(len * sizeof(WCHAR));
    if (buffer)
    {
        result = std::vswprintf(buffer, len, format, args);
    }
    va_end(args);
    if (result != -1 && buffer)
    {
        free_ptr<LPWSTR> freebuf(buffer);
        return CString(buffer);
    }
    return _T("");
}

// See utility.h for documentation of this method.
bool cstringtoi(const CString &s, INT32 &number)
{
    if (s.IsEmpty())
        return false;

    TCHAR *p;
    INT64 temp;
    temp = _tcstoll(s, &p, 0);
    if ((temp > INT32_MAX) || (temp < INT32_MIN))
    {
        return false;
    }
    number = static_cast<INT32>(temp);
    return (p != NULL) && (*p == 0);
}

// See utility.h for documentation of this method.
bool cstringtoui(const CString &s, UINT32 &number)
{
    if (s.IsEmpty())
        return false;

    if (s[0] == '-')
        return false;

    TCHAR *p;
    UINT64 temp;
    temp = _tcstoull(s, &p, 0);
    if (temp > UINT32_MAX)
    {
        return false;
    }
    number = static_cast<UINT32>(temp);
    return (p != NULL) && (*p == 0);
}

// See utility.h for documentation of this method.
CString scale_bytes(UINT64 sizeInBytes)
{
    double originalSize = (double)sizeInBytes;
    double scaledSize = 0;

    scaledSize = originalSize / (1024 * 1024 * 1024);
    if (scaledSize >= 1.0) // GB
    {
        if (sizeInBytes % (1024 * 1024 * 1024))
            return format_cstring(_T("%.3f GB"), scaledSize);
        else
            return format_cstring(_T("%.f GB"), scaledSize);
    }

    scaledSize = originalSize / (1024 * 1024);
    if (scaledSize >= 1.0) // MB
    {
        if (sizeInBytes % (1024 * 1024))
            return format_cstring(_T("%.3f MB"), scaledSize);
        else
            return format_cstring(_T("%.f MB"), scaledSize);
    }

    scaledSize = originalSize / 1024;
    if (scaledSize >= 1.0) // KB
    {
        if (sizeInBytes % 1024)
            return format_cstring(_T("%.3f KB"), scaledSize);
        else
            return format_cstring(_T("%.f KB"), scaledSize);
    }
    else // bytes
    {
        return format_cstring(_T("%d bytes"), sizeInBytes);
    }
} // scale_bytes()

// See utility.h for documentation of this method.
unsigned char hex_to_int(char hex)
{
    if (isdigit(hex))
        return hex - '0';
    else if (hex >= 'a' && hex <= 'f')
        return 10 + hex - 'a';
    else if (hex >= 'A' && hex <= 'F')
        return 10 + hex - 'A';
    // unknow char
    return 0;
}

// See utility.h for documentation of this method.
char int_to_hex(unsigned char digit)
{
    digit &= 0xF;
    if (digit < 10)
        return digit + '0';
    else
        return digit - 10 + 'A';
}

// See utility.h for documentation of this method.
bool file_exists(CString fullPath)
{
    // Do not use PathFileExists here. folder names should not be acceptable.
    CFile testFileExist;
    if (!testFileExist.Open(fullPath, CFile::modeRead, NULL))
    {
        return false;
    }
    testFileExist.Close();
    return true;
}