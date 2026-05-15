/*
 * Support code for serial recovery feature of MCUboot
 * Adapted for MCUXpresso SDK
 * 
 * Copyright (C) 2026 NXP Semiconductors
 * Copyright (C) 2018, Nordic Semiconductor ASA
 * Copyright (C) 2017 Intel Corporation
 * Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Modified by NXP 2026 - Combined Intel and Nordic/ARM sources
 */

#ifndef __SERIAL_RECOVERY_SUPPORT_H__
#define __SERIAL_RECOVERY_SUPPORT_H__

#include <stdint.h>
#include <stddef.h>
#include <errno.h>

#define MCUBOOT_PERUSER_MGMT_GROUP_ENABLED  0

#ifndef ENOMEM
#define ENOMEM          12  /* Out of memory */
#endif

#ifndef EINVAL
#define EINVAL          22  /* Invalid argument */
#endif

/**
 * @brief Compute the checksum of a buffer with polynomial 0x1021
 *
 * @param seed Value to seed the CRC with
 * @param src Input bytes for the computation
 * @param len Length of the input in bytes
 *
 * @return The computed CRC16 value (without any XOR applied to it)
 */
uint16_t crc16_itu_t(uint16_t seed, const uint8_t *src, size_t len);

/**
 * @brief          Encode a buffer into base64 format
 *
 * @param dst      destination buffer
 * @param dlen     size of the destination buffer
 * @param olen     number of bytes written
 * @param src      source buffer
 * @param slen     amount of data to be encoded
 *
 * @return         0 if successful, or -ENOMEM if the buffer is too small.
 *                 *olen is always updated to reflect the amount
 *                 of data that has (or would have) been written.
 *                 If that length cannot be represented, then no data is
 *                 written to the buffer and *olen is set to the maximum
 *                 length representable as a size_t.
 *
 * @note           Call this function with dlen = 0 to obtain the
 *                 required buffer size in *olen
 */
int base64_encode(uint8_t *dst, size_t dlen, size_t *olen, const uint8_t *src,
		  size_t slen);

/**
 * @brief          Decode a base64-formatted buffer
 *
 * @param dst      destination buffer (can be NULL for checking size)
 * @param dlen     size of the destination buffer
 * @param olen     number of bytes written
 * @param src      source buffer
 * @param slen     amount of data to be decoded
 *
 * @return         0 if successful, -ENOMEM, or -EINVAL if the input data is
 *                 not correct. *olen is always updated to reflect the amount
 *                 of data that has (or would have) been written.
 *
 * @note           Call this function with *dst = NULL or dlen = 0 to obtain
 *                 the required buffer size in *olen
 */
int base64_decode(uint8_t *dst, size_t dlen, size_t *olen, const uint8_t *src,
		  size_t slen);

int serial_recovery_read(char *str, int cnt, int *newline);

void serial_recovery_write(const char *ptr, int cnt);

void nxp_delay_usecs(uint32_t usecs);

void nxp_system_reset(void);

#endif
