/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"

#if defined(BOARD_DEBUG_UART_TYPE) && defined(BOARD_DEBUG_UART_TYPE) && \
    (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)

#include "fw_serial.h"
#include "fsl_lpuart_freertos.h"

static lpuart_rtos_handle_t g_rtos_handle = {0};
static lpuart_handle_t g_uart_handle = {0};
static uint8_t g_background_buffer[8];

int32_t fw_serial_init(void *uart_base, uint32_t baudrate, uint32_t clk_freq)
{
    lpuart_rtos_config_t uart_config = {
        .baudrate = baudrate,
        .parity = kLPUART_ParityDisabled,
        .stopbits = kLPUART_OneStopBit,
        .buffer = g_background_buffer,
        .buffer_size = sizeof(g_background_buffer),
    };
    uart_config.srcclk = clk_freq;
    uart_config.base = uart_base;
    int32_t result = LPUART_RTOS_Init(&g_rtos_handle, &g_uart_handle, &uart_config);
    return result;
}

int32_t fw_serial_receive(uint8_t *buffer, uint32_t length, size_t *received)
{
    int32_t result = LPUART_RTOS_Receive(&g_rtos_handle, buffer, length, received);
    return result;
}

int32_t fw_serial_send(const uint8_t *buffer, uint32_t length)
{
    int32_t result = LPUART_RTOS_Send(&g_rtos_handle, (uint8_t *)buffer, length);
    return result;
}

#endif
