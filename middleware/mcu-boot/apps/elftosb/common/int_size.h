/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_int_size_h_)
#define _int_size_h_

namespace elftosb
{
//! Supported sizes of integers.
typedef enum
{
    kWordSize,     //!< 32-bit word.
    kHalfWordSize, //!< 16-bit half word.
    kByteSize      //!< 8-bit byte.
} int_size_t;

}; // namespace elftosb

#endif // _int_size_h_
