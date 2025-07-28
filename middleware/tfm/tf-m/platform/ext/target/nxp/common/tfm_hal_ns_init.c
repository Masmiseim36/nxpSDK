/*
 * Copyright (c) 2022-2024, Arm Limited. All rights reserved.
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "app.h"
#include "uart_stdout.h"
#include "Driver_USART.h"

int32_t tfm_ns_platform_init (void)
{
    BOARD_InitHardware();
	
    stdio_init();

    return ARM_DRIVER_OK;
}
