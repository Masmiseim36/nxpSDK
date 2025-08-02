/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SBL_CONFIG_H__
#define SBL_CONFIG_H__

/*******************************************************************/
/* Use default configuration if setup from Kconfig is not provided */
/*******************************************************************/
#ifndef CONFIG_BOOT_CUSTOM_DEVICE_SETUP

/* MCUBoot Flash Config */

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 800

/* Crypto Config */

#define CONFIG_BOOT_SIGNATURE
#define CONFIG_BOOT_SIGNATURE_TYPE_RSA
#define CONFIG_BOOT_SIGNATURE_TYPE_RSA_LEN 2048
#define COMPONENT_MBEDTLS
#define CONFIG_BOOT_BOOTSTRAP

/* Encrypted XIP support config */

/*
 * Enable extension utilizing on-the-fly decryption of encrypted image.
 * For more information please see readme file.
 */
//#define CONFIG_ENCRYPT_XIP_EXT_ENABLE

/*
 * Encrypted XIP extension uses simpler OVERWRITE_ONLY mode instead of three
 * slot configuration.
 */
//#define CONFIG_ENCRYPT_XIP_EXT_OVERWRITE_ONLY

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
