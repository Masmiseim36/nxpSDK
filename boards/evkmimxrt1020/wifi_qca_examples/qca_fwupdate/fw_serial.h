/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FW_SERIAL_H__
#define __FW_SERIAL_H__

#include <stdint.h>
#include <stddef.h>

int32_t fw_serial_init(void *uart_base, uint32_t baudrate, uint32_t clk_freq);
int32_t fw_serial_receive(uint8_t *buffer, uint32_t length, size_t *received);
int32_t fw_serial_send(const uint8_t *buffer, uint32_t length);

#endif
