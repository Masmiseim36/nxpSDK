/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _host_types_h_
#define _host_types_h_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <deque>

//! @brief An array of strings.
typedef std::vector<std::string> string_vector_t;

//! @brief An array of bytes.
typedef std::vector<unsigned char> uchar_vector_t;

//! @brief An array of uint32_t's.
typedef std::vector<uint32_t> uint32_vector_t;

//! @brief A stream of bytes.
typedef std::deque<unsigned char> uchar_deque_t;

#endif // _host_types_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
