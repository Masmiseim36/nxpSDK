/*
 * Copyright 2024 NXP
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2015 Runtime Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * \file
 *
 * This source code is imported from Zephyr project.
 * Note a few include modifications were required to make it compile in a non
 * Zephyr environment.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef ZEPHYR_SETTINGS_ORIGINAL_SOURCE
#include <errno.h>

#include <zephyr/settings/settings.h>
#include "settings/settings_file.h"
#include <zephyr/kernel.h>
#else
#include "fwk_hal_macros.h"
#include "port/nvs_errno.h"
#include "port/nvs_port.h"
#include "settings.h"
#endif

extern struct k_mutex settings_lock;

bool settings_subsys_initialized;

void settings_init(void);

int settings_backend_init(void);

int settings_subsys_init(void)
{

    int err = 0;

    k_mutex_init(&settings_lock);

	k_mutex_lock(&settings_lock, K_FOREVER);

	if (!settings_subsys_initialized) {
		settings_init();

		err = settings_backend_init();

		if (!err) {
			settings_subsys_initialized = true;
		}
	}

	k_mutex_unlock(&settings_lock);

	return err;
}
