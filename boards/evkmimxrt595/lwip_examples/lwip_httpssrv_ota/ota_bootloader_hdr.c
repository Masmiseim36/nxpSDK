/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ota_bootloader_supp.h"

/* Dummy header/placeholder. Also allows for direct execution of XIP image flashed by the debugger */
const boot_image_header_t ota_header = {
    .tag           = IMG_HDR_TAG,
    .load_addr     = ((uint32_t)&ota_header) + BL_IMG_HEADER_SIZE,
    .image_type    = IMG_TYPE_XIP,
    .image_size    = 0,
    .algorithm     = IMG_CHK_ALG_CRC32,
    .header_size   = BL_IMG_HEADER_SIZE,
    .image_version = 0,
    .checksum      = {0xFFFFFFFF},
};
