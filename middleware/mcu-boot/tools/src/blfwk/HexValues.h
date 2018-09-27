/*
 * File:    HexValues.h
 *
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_HexValues_h_)
#define _HexValues_h_

#include <stdint.h>

//! \brief Determines whether \a c is a hex digit character.
bool isHexDigit(char c);

//! \brief Converts a hexidecimal character to the integer equivalent.
uint8_t hexCharToInt(char c);

//! \brief Converts a hex-encoded byte to the integer equivalent.
uint8_t hexByteToInt(const char *encodedByte);

#endif // _HexValues_h_
