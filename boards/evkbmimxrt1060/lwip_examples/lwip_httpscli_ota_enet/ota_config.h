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

#define OTA_SERVER_NAME_DEFAULT "192.168.0.10"

/*
 * OTA server service name or port number string.
 */

#define OTA_SERVER_PORT_DEFAULT "4433"

/*
 * OTA image path on the server
 */

#define OTA_IMAGE_PATH_DEFAULT "/ota.bin"

#endif /* __OTA_CONFIG_H__ */
