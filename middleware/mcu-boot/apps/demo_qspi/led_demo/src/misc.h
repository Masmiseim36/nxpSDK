/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdbool.h>

//! @brief Enter bootloader from user application.
void app_enter_bootloader(void);

//! @brief Init bootloader pin
void bootloader_pin_init(void);

//! @brief determine whether boot pin is asserted
bool is_bootloader_pin_clicked(void);
