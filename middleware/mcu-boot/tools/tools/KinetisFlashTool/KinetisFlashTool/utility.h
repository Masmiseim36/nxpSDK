/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

//! \brief Get a formated CString.
CString format_cstring(const wchar_t *fmt, ...);

//! \brief Convert a CString to int.
//!
//! \return false for an error occurred during the conversion.
//!         true for success.
bool cstringtoi(const CString &s, INT32 &number);

//! \brief Convert a CString to unsigned int.
//!
//! \return false for an error occurred during the conversion.
//!         true for success.
bool cstringtoui(const CString &s, UINT32 &number);

//! \brief Convert unsigned int to formated size string.
CString scale_bytes(UINT64 sizeInBytes);

//! \brief Convert an ASICII hex text to an integer.
unsigned char hex_to_int(char hex);

//! \brief Convert an integer to an ASICII hex.
char int_to_hex(unsigned char digit);

//! \brief Test whether the file exists.
bool file_exists(CString fullPath);