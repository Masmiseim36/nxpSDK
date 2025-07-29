/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

/* RT700 MMU0 register to detect flash remap */
#define FLASH_REMAP_OFFSET_REG          0x40030000

/* MAX_IMG_SECTORS >= (AppImageSize / SectorSize), needed by SWAP */
#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 800

/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP

/*
 * MCUBoot upgrade mode
 *
 * The default MCUBoot configuration is to use SWAP mechanism. In case the flash
 * remapping functionality is supported by processor the alternative mechanism
 * using DIRECT-XIP mode can be used and evaluated by user.
 * Comment this to enable SWAP mode or when encrypted XIP extension is enabled.
 */
#define CONFIG_MCUBOOT_FLASH_REMAP_ENABLE

/* To be able to bootstrap signed image from debug session */
#define CONFIG_BOOT_BOOTSTRAP

/* Crypto */
#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256
#define MCUBOOT_USE_TINYCRYPT

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
