/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __BL_NOR_ENCRYPT_H__
#define __BL_NOR_ENCRYPT_H__

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    union
    {
        struct
        {
            uint32_t reserved : 28;
            uint32_t tag : 4; //!< Tag, must be 0x0e
        } B;
        uint32_t U;
    } option0;
    uint32_t reserved[10];
} nor_encrypt_option_t;

enum
{
    kNorEncyptOption_Tag = 0x0e, //!< Tag
};

//! @brief Bootloader clock option
typedef enum _bootloader_clock_option
{
    kClockOption_EnterBootloader = 0, //!< Clock option for entering bootloader
    kClockOption_ExitBootloader  = 1, //!< Clock option for exiting bootloader
} bootloader_clock_option_t;

//! @brief Boot device Option
typedef enum _bootdevice_option
{
    kBootDevice_FlexSpiNOR  = 0x01,
    kBootDevice_FlexSpiNAND = 0x02,
    kBootDevice_SPIEEPROM   = 0x04,
    kBootDevice_MMC_SD      = 0x08,
    kBootDevice_SemcNOR     = 0x10,
    kBootDevice_SemcNAND    = 0x20,
} bootdevice_option_t;

//! @brief Boot Mode Option
typedef enum _bootmode_option
{
    kBootMode_BootFromFuse,
    kBootMode_SerialDownloader,
    kBootMode_InternalBoot
} boot_mode_option_t;

//! @brief Hab status option
typedef enum _habstatus_option
{
    kHabStatus_Open,
    kHabStatus_Close,
} habstatus_option_t;

typedef enum _hab_config_idx
{
    kHabBootloader = 0,
    kHabApplication,
    kHabMaxConfigCount,
} hab_config_idx_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

/*!
 * @brief Determine whether there is a valid encrypt region info
 *
 */
bool bl_nor_encrypt_region_info_valid(void *arg);

/*!
 * @brief Setup BEE key
 *
 */
void bl_nor_encrypt_set_key(void);

/*!
 * @brief Check if encrypted XIP is enabled
 *
 */
bool bl_nor_encrypt_is_enabled(void);

/*!
 * @brief Return the Encrypted Region Info presence status
 *
 */
bool bl_nor_encrypt_has_encrypted_region(void);

/*!
 * @brief Refresh Encrypted region info
 *
 */
void bl_nor_encrypt_region_refresh(uint32_t start, uint32_t bytes);

/*!
 * @brief Check if a specified region is in encrypted region
 *
 */
bool bl_nor_in_encrypted_region(uint32_t start, uint32_t bytes);

/*!
 * @brief Create a new config for given index
 *
 */
status_t bl_nor_encrypt_make_new_config(uint32_t index);

/*!
 * @brief Get Configuration block
 *
 */
status_t bl_nor_encrypt_get_config_block(uint32_t index, uint32_t **start, uint32_t *bytes);

/*!
 * @brief Encrypted data in specified region
 *
 */
status_t bl_nor_encrypt_data(uint32_t addr, uint32_t size, uint32_t *data_start);

/*!
 * @brief Encrypted data in specified region, using the crypto context from before
 *        a newly generated one
 */
status_t bl_nor_encrypt_data_prev_ctx(uint32_t addr, uint32_t size, uint32_t *data_start);

/*!
 * @brief Decrypt data from specified region
 *
 */
status_t bl_nor_decrypt_data(uint32_t addr, uint32_t size, uint32_t *data_start);

/*!
 * @brief Decrypt data from specified region, , using the crypto context from before
 *        a newly generated one
 */
status_t bl_nor_decrypt_data_prev_ctx(uint32_t addr, uint32_t size, uint32_t *data_start);

/*!
 * @brief Decrypt data from specified region, using the crypto context kept
 *        in BACKUP region
 *
 */
status_t bl_nor_decrypt_data_backup_ctx(uint32_t addr, uint32_t size, uint32_t *data_start);

/*!
 * @brief Ensures that there are two independent crypto context definitions
 *
 */
status_t bl_nor_encrypt_ensure_context(void);

/*!
 * @brief Splits PRDB0 into two distinct PRDBs
 *
 */
status_t bl_nor_encrypt_split_prdb(void);

/*!
 * @brief Check for duplicate crypto contexts
 *
 */
status_t bl_nor_crypto_ctx_compare_backup(bool *isMatch, bool *isOriginal, uint32_t index);

/*!
 * @brief Load in RAM the crypto contexts kept in BACKUP region
 *
 */
status_t bl_nor_encrypt_region_info_load_backup(void);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __BL_NOR_ENCRYPT_H__
