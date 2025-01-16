/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

/* CONFIG_MCUBOOT_MAX_IMG_SECTORS >= (AppImageSize / SectorSize) */
#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 800

//#define MCUBOOT_OVERWRITE_ONLY

/* To be able to bootstrap signed image from debug session */
#define CONFIG_BOOT_BOOTSTRAP

/* Crypto */

#define COMPONENT_MCUBOOT_SECURE
#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256
#define MCUBOOT_USE_TINYCRYPT

#endif
