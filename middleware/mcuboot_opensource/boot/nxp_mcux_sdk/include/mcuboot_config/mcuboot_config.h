/*
 * Copyright (c) 2018 Open Source Foundries Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MCUBOOT_CONFIG_H__
#define __MCUBOOT_CONFIG_H__

#include "sblconfig.h"

/*
 * Template configuration file for MCUboot.
 *
 * When porting MCUboot to a new target, copy it somewhere that your
 * include path can find it as mcuboot_config/mcuboot_config.h, and
 * make adjustments to suit your platform.
 *
 * For examples, see:
 *
 * boot/zephyr/include/mcuboot_config/mcuboot_config.h
 * boot/mynewt/mcuboot_config/include/mcuboot_config/mcuboot_config.h
 */

/*
 * UPGRADE MODE
 *
 * NXP supports several upgrade modes defined below.
 * Refer to ota_examples\_doc documentation for more information.
 */

#if (defined(CONFIG_BOOT_MODE_OVERWRITE_ONLY) + \
     defined(CONFIG_BOOT_MODE_FLASH_REMAP) + \
     defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP) + \
     defined(CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT) + \
     defined(CONFIG_BOOT_MODE_SWAP)) != 1
#error "An upgrade mode must be defined"
#endif

#if defined(CONFIG_BOOT_MODE_OVERWRITE_ONLY)

/* Upgrade mode: OVERWRITE_ONLY */
#define MCUBOOT_OVERWRITE_ONLY
#define UPGRADE_MODE  "OVERWRITE ONLY"
/* Uncomment to only erase and overwrite those primary slot sectors needed
 * to install the new image, rather than the entire image slot. */
//#define MCUBOOT_OVERWRITE_ONLY_FAST

#elif defined(CONFIG_BOOT_MODE_FLASH_REMAP)

/* Upgrade mode: DIRECT-XIP + FLASH REMAP */
#define MCUBOOT_DIRECT_XIP
#define MCUBOOT_DIRECT_XIP_REVERT
#define UPGRADE_MODE  "FLASH REMAP"

#elif defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP)

/* Upgrade mode: OVERWRITE_ONLY + ENCRYPTED XIP extension */
#define MCUBOOT_OVERWRITE_ONLY
#define MCUBOOT_OVERWRITE_ONLY_FAST
#define MCUBOOT_IMAGE_ACCESS_HOOKS
#define UPGRADE_MODE  "ENCRYPTED XIP"

#elif defined(CONFIG_BOOT_MODE_SINGLE_APPLICATION_SLOT)

/* Upgrade mode: SINGLE_APPLICATION_SLOT */
#define MCUBOOT_SINGLE_APPLICATION_SLOT
#define UPGRADE_MODE  "SINGLE APPLICATION SLOT"

#elif defined(CONFIG_BOOT_MODE_SWAP)

/* Upgrade mode: SWAP MODE */ 
#define MCUBOOT_SWAP_USING_MOVE 1
#define UPGRADE_MODE  "SWAP"

#else

#error "An upgrade mode is not defined"

#endif /* Upgrade mode */

/*
 * CRYPTOGRAPHIC SETTINGS
 */

/* 
 * Cryptographic backend
 *
 * You must choose between PSA, mbedTLS or Tinycrypt as source of
 * cryptographic primitives.
 */
/* Use mbedTLS legacy API (no hw acceleration) */
#if defined(CONFIG_BOOT_USE_MBEDTLS)
#define MCUBOOT_USE_MBED_TLS
#endif

/* Use PSA Crypto API (hw acceleration support) - experimental */
#if defined(CONFIG_BOOT_USE_PSA_CRYPTO)   
#define MCUBOOT_USE_PSA_CRYPTO
#endif

/* Uncomment to use lightweight TinyCrypt */
#ifdef CONFIG_BOOT_USE_TINYCRYPT
#define MCUBOOT_USE_TINYCRYPT
#endif

#if (defined(CONFIG_BOOT_USE_MBEDTLS) + defined(CONFIG_BOOT_USE_PSA_CRYPTO) + \
    defined(CONFIG_BOOT_USE_TINYCRYPT)) > 1
#error "Only one crypto backend can be enabled"
#endif

/*
 * Signature types
 *
 * You must choose exactly one signature type.
 */

/* Uncomment to enable RSA signature */
#ifdef CONFIG_BOOT_SIGNATURE_TYPE_RSA
#define MCUBOOT_SIGN_RSA
#if (CONFIG_BOOT_SIGNATURE_TYPE_RSA_LEN != 2048 && CONFIG_BOOT_SIGNATURE_TYPE_RSA_LEN != 3072)
#error "Invalid RSA key size (must be 2048 or 3072)"
#else
#define MCUBOOT_SIGN_RSA_LEN CONFIG_BOOT_SIGNATURE_TYPE_RSA_LEN
#endif
#elif defined(CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256)
#define MCUBOOT_SIGN_EC256
#elif defined(CONFIG_BOOT_SIGNATURE_TYPE_ED25519)
#define MCUBOOT_SIGN_ED25519
#endif

/*
 * Encrypted image
 *
 * You must choose exactly one encryption type.
 */

#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP) && defined(CONFIG_BOOT_USE_PSA_CRYPTO)
#error "Currently Encrypted XIP is not supported with PSA Crypto API"
#endif

#ifdef CONFIG_BOOT_ENCRYPT_RSA
#define MCUBOOT_ENC_IMAGES
#define MCUBOOT_ENCRYPT_RSA
#endif

#ifdef CONFIG_BOOT_ENCRYPT_EC256
#define MCUBOOT_ENC_IMAGES
#define MCUBOOT_ENCRYPT_EC256
#endif

/*
 * Serial recovery
 */
#define MCUBOOT_ERASE_PROGRESSIVELY
#define MCUBOOT_BOOT_MGMT_ECHO

#if defined(CONFIG_BOOT_SERIAL_RECOVERY) && defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP)
#warning "Serial recovery currently doesn't support encrypted XIP mode"
#endif

/* Uncomment to enable Hardware Key */
#ifdef CONFIG_BOOT_HW_KEY
#define MCUBOOT_HW_KEY
#endif

/*
 * Always check the signature of the image in the primary slot before booting,
 * even if no upgrade was performed. This is recommended if the boot
 * time penalty is acceptable.
 */
#ifdef CONFIG_BOOT_SIGNATURE
#define MCUBOOT_VALIDATE_PRIMARY_SLOT
#endif

#ifdef CONFIG_UPDATEABLE_IMAGE_NUMBER
#define MCUBOOT_IMAGE_NUMBER CONFIG_UPDATEABLE_IMAGE_NUMBER
#else
#define MCUBOOT_IMAGE_NUMBER 1
#endif

/*
 * FLASH ABSTRACTION
 */

/* Uncomment if your flash map API supports flash_area_get_sectors().
 * See the flash APIs for more details. */
#define MCUBOOT_USE_FLASH_AREA_GET_SECTORS

/* Default maximum number of flash sectors per image slot; change
 * as desirable. */
#ifdef CONFIG_MCUBOOT_MAX_IMG_SECTORS
#define MCUBOOT_MAX_IMG_SECTORS CONFIG_MCUBOOT_MAX_IMG_SECTORS
#else
#error "CONFIG_MCUBOOT_MAX_IMG_SECTORS is not defined"
#endif

/*
 * LOGGING
 */

/*
 * If logging is enabled the following functions must be defined by the
 * platform:
 *
 *    MCUBOOT_LOG_MODULE_REGISTER(domain)
 *      Register a new log module and add the current C file to it.
 *
 *    MCUBOOT_LOG_MODULE_DECLARE(domain)
 *      Add the current C file to an existing log module.
 *
 *    MCUBOOT_LOG_ERR(...)
 *    MCUBOOT_LOG_WRN(...)
 *    MCUBOOT_LOG_INF(...)
 *    MCUBOOT_LOG_DBG(...)
 *
 * The function priority is:
 *
 *    MCUBOOT_LOG_ERR > MCUBOOT_LOG_WRN > MCUBOOT_LOG_INF > MCUBOOT_LOG_DBG
 */
#ifndef CONFIG_MCUBOOT_DISABLE_LOGGING
#define MCUBOOT_HAVE_LOGGING
#endif

/*
 * Assertions
 */

/* Uncomment if your platform has its own mcuboot_config/mcuboot_assert.h.
 * If so, it must provide an ASSERT macro for use by bootutil. Otherwise,
 * "assert" is used. */
/* #define MCUBOOT_HAVE_ASSERT_H */

/* BOOTSTRAP support */
#ifdef CONFIG_BOOT_BOOTSTRAP
#define MCUBOOT_BOOTSTRAP 1
#endif

/* Not enabled, no feed activity */
#define MCUBOOT_WATCHDOG_FEED() \
    do                          \
    {                           \
    } while (0)

#define MCUBOOT_CPU_IDLE() \
    do {                   \
    } while (0)
#endif /* __MCUBOOT_CONFIG_H__ */
