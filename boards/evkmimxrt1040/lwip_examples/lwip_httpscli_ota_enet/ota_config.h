/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __OTA_CONFIG_H__
#define __OTA_CONFIG_H__

#include "flash_partitioning.h"

/*
 * OTA server DNS name or IP string.
 */

#define OTA_SERVER_NAME "192.168.0.116"

/*
 * OTA server service name or port number string.
 */

#define OTA_SERVER_PORT "4433"

/*
 * OTA image path on the server
 */

#define OTA_IMAGE_PATH "/ota.bin"

/*
 * Defines size and address of internal flash area dedicated for storing downloaded image update
 */

#define OTA_STORAGE_SIZE (BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP)
#define OTA_STORAGE_ADDR BOOT_FLASH_CAND_APP

#endif /* __OTA_CONFIG_H__ */
