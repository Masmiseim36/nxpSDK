/** @file
 * @brief Bluetooth shell module
 *
 * Provides some common functions that can be used by other shell files.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/byteorder.h>
#include <porting.h>

#include "shell_bt.h"

/* Microseconds from 01-Jan-0000 till epoch(in ISO 8601: 2021 - 04 - 30T00 : 00 : 00Z). */
#define SHELL_uS_TIMESTAMP_PSEUDO      0x00E29EE5C36CE000ULL
/**
 *  \fn SHELL_get_us_timestamp
 *
 *  \brief To get the microsecond timestamp
 *
 *  \par Description:
 *  This function implements the OS Wrapper to get system time in microseconds
 *
 *  \return System time in microseconds
 */
uint64_t SHELL_get_us_timestamp(void)
{
    uint64_t timestamp;
    timestamp = OSA_TimeGetMsec() * (uint64_t)1000UL;

    timestamp = timestamp + SHELL_uS_TIMESTAMP_PSEUDO;

    return timestamp;
}

void shell_hexdump(const struct shell *sh, const uint8_t *data, size_t len)
{
    shell_dump(sh, " Data: ");

    while (len--) {
        shell_dump(sh, "%02X ", *data++);
    }
	shell_dump(sh, "\r\n");
}

unsigned long shell_strtoul(const char *str, int base, int *err)
{
	unsigned long val;
	char *endptr = NULL;

	if (*str == '-') {
		*err = -EINVAL;
		return 0;
	}

	val = strtoul(str, &endptr, base);

	return val;
}

unsigned long long shell_strtoull(const char *str, int base, int *err)
{
	unsigned long long val;
	char *endptr = NULL;

	if (*str == '-') {
		*err = -EINVAL;
		return 0;
	}

	val = strtoull(str, &endptr, base);

	return val;
}

long shell_strtol(const char *str, int base, int *err)
{
	long val;
	char *endptr = NULL;

	val = strtol(str, &endptr, base);
	if (endptr == str || *endptr) {
		*err = -EINVAL;
		return 0;
	}

	return val;
}

bool shell_strtobool(const char *str, int base, int *err)
{
	if (!strcmp(str, "on") || !strcmp(str, "enable") || !strcmp(str, "true")) {
		return true;
	}

	if (!strcmp(str, "off") || !strcmp(str, "disable") || !strcmp(str, "false")) {
		return false;
	}

	return shell_strtoul(str, base, err);
}
