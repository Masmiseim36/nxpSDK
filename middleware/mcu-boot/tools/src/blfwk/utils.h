/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _utils_h_
#define _utils_h_

#include <string>
#include <vector>

namespace utils
{
//! @brief Split a string into tokens by delimiter.
std::vector<std::string> string_split(const std::string &s, char delim);

//! @brief Remove all except hex digits from a string.
std::string string_hex(const std::string &s);

//! @brief Check if a string is signed a number. If so, return number.
bool stringtoi(const std::string &s, int32_t &number);

//! @brief Check if a string is an unsigned number. If so, return number.
bool stringtoui(const std::string &s, uint32_t &number);

//! @brief Format bytes into GB, MB, KB, or bytes.
std::string scale_bytes(uint64_t sizeInBytes);

} // namespace utils

#endif // _utils_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
