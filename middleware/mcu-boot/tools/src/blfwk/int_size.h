/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_int_size_h_)
#define _int_size_h_

namespace blfwk
{
//! Supported sizes of integers.
typedef enum
{
    kWordSize,     //!< 32-bit word.
    kHalfWordSize, //!< 16-bit half word.
    kByteSize      //!< 8-bit byte.
} int_size_t;

enum
{
    MinEraseAlignment = 1024,
};

}; // namespace blfwk

#endif // _int_size_h_
