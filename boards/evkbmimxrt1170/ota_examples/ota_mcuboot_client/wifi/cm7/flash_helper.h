/*
 * Copyright (c) 2022, Freescale Semiconductor, Inc.
 * Copyright 2022 NXP. Not a Contribution
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FLASH_HELPER_H__
#define __FLASH_HELPER_H__

#include <stdint.h>
#include "fsl_common.h"

void hexdump(const void *src, size_t size);

int flash_verify_erase(uint32_t offset, size_t size);
int flash_erase(uint32_t offset, size_t size);
int flash_program(uint32_t offset, void *data, size_t size);
int flash_sha256(uint32_t offset, size_t size, uint8_t md[32]);

#endif /* __FLASH_HELPER_H__ */
