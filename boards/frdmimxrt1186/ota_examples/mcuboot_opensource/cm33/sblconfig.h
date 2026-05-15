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

/* MCU SBL core */

#define CONFIG_MCUBOOT_MAX_IMG_SECTORS 800

/*
 * MCUBoot upgrade mode (default)
 * If supported by the device, an alternative upgrade mode can be selected.
 * For more information please see documentation (ota_examples/_doc).
 */
//#define CONFIG_BOOT_MODE_OVERWRITE_ONLY
//#define CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT
#define CONFIG_BOOT_MODE_SWAP

#define CONFIG_BOOT_BOOTSTRAP

/* Crypto Config */

#define CONFIG_BOOT_SIGNATURE
/* NXP MCUX-82181: there are hardcoded dependencies to fix - disable PSA API */
//#define CONFIG_BOOT_USE_PSA_CRYPTO
#define CONFIG_BOOT_USE_MBEDTLS
#define CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256

#endif /* CONFIG_BOOT_CUSTOM_DEVICE_SETUP */

#endif
