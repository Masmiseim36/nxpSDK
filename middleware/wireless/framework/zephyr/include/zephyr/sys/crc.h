/*
 * Copyright (c) 2018 Workaround GmbH.
 * Copyright (c) 2017 Intel Corporation.
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * Copyright (c) 2015 Runtime Inc
 * Copyright (c) 2018 Google LLC.
 * Copyright (c) 2022 Meta
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/** @file
 * @brief CRC computation function
 */

#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initial value expected to be used at the beginning of the crc8_ccitt
 * computation.
 */
#define CRC8_CCITT_INITIAL_VALUE 0xFFu

/**
 * @brief Compute CCITT variant of CRC 8
 *
 * Normal CCITT variant of CRC 8 is using 0x07.
 *
 * @param initial_value Initial value for the CRC computation
 * @param buf Input bytes for the computation
 * @param len Length of the input in bytes
 *
 * @return The computed CRC8 value
 */
uint8_t crc8_ccitt(uint8_t initial_value, const void *buf, size_t len);

/**
 * @brief Generic function for computing CRC 8
 *
 * Compute CRC 8 by passing in the address of the input, the input length
 * and polynomial used in addition to the initial value.
 *
 * @param src Input bytes for the computation
 * @param len Length of the input in bytes
 * @param polynomial The polynomial to use omitting the leading x^8
 *        coefficient
 * @param initial_value Initial value for the CRC computation
 * @param reversed Should we use reflected/reversed values or not
 *
 * @return The computed CRC8 value
 */
uint8_t crc8(const uint8_t *src, size_t len, uint8_t polynomial, uint8_t initial_value,
	  bool reversed);

#ifdef __cplusplus
}
#endif

#endif
