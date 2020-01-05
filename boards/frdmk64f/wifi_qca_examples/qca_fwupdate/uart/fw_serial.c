/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "serial_manager.h"

#include "fw_serial.h"
#include "fsl_uart_freertos.h"

static uart_rtos_handle_t g_rtos_handle = {0};
static uart_handle_t g_uart_handle      = {0};
static uint8_t g_background_buffer[8];

int32_t fw_serial_init(void *uart_base, uint32_t baudrate, uint32_t clk_freq)
{
    uart_rtos_config_t uart_config = {
        .baudrate    = baudrate,
        .parity      = kUART_ParityDisabled,
        .stopbits    = kUART_OneStopBit,
        .buffer      = g_background_buffer,
        .buffer_size = sizeof(g_background_buffer),
    };
    uart_config.srcclk = clk_freq;
    uart_config.base   = uart_base;
    int32_t result     = UART_RTOS_Init(&g_rtos_handle, &g_uart_handle, &uart_config);
    return result;
}

int32_t fw_serial_receive(uint8_t *buffer, uint32_t length, size_t *received)
{
    int32_t result = UART_RTOS_Receive(&g_rtos_handle, buffer, length, received);
    return result;
}

int32_t fw_serial_send(const uint8_t *buffer, uint32_t length)
{
    int32_t result = UART_RTOS_Send(&g_rtos_handle, (uint8_t *)buffer, length);
    return result;
}
