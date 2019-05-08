/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"

//! @brief Type used for all status and error return values.
typedef int32_t status_t;

//! @brief Enumeration for supported flash margin levels.
typedef enum _flash_margin_value
{
    kFlashMargin_Normal,
    kFlashMargin_User,
    kFlashMargin_Factory,

    // Not real margin level, Used to determine the range of valid margin level.
    kFlashMargin_Invalid
} flash_margin_value_t;

//! @brief Enumeration for the three possible flash security states.
typedef enum _flash_security_state
{
    kFlashNotSecure,
    kFlashSecureBackdoorEnabled,
    kFlashSecureBackdoorDisabled
} flash_security_state_t;

//! @brief Enumeration for the three possible flash protection levels.
typedef enum _flash_protection_state
{
    kFlashProtection_Unprotected,
    kFlashProtection_Protected,
    kFlashProtection_Mixed
} flash_protection_state_t;

//! @brief Enumeration for the three possible flash execute access levels.
typedef enum _flash_execute_only_access_state
{
    kFlashAccess_UnLimited,
    kFlashAccess_ExecuteOnly,
    kFlashAccess_Mixed
} flash_execute_only_access_state_t;

//! @brief Enumeration for various flash properties.
typedef enum _flash_property
{
    kFlashProperty_SectorSize = 0,
    kFlashProperty_TotalFlashSize = 1,
    kFlashProperty_BlockCount = 3,
    kFlashProperty_FlashBlockBaseAddr,
    kFlashProperty_FlashFacSupport,
    kFlashProperty_FlashAccessSegmentSize,
    kFlashProperty_FlashAccessSegmentCount,
    kFlashProperty_Version,
} flash_property_t;

//! @brief callback type used for pflash block
typedef void (*flash_callback_t)(void);

//! @brief Flash driver state information.
//!
//! An instance of this structure is allocated by the user of the flash driver and
//! passed into each of the driver APIs.
typedef struct _flash_driver
{
    uint32_t PFlashBlockBase;          //!< Base address of the first PFlash block
    uint32_t PFlashTotalSize;          //!< Size of all combined PFlash block.
    uint32_t PFlashBlockCount;         //!< Number of PFlash blocks.
    uint32_t PFlashSectorSize;         //!< Size in bytes of a sector of PFlash.
    flash_callback_t PFlashCallback;   //!< Callback function for flash API.
    uint32_t PFlashAccessSegmentSize;  //!< Size in bytes of a access segment of PFlash.
    uint32_t PFlashAccessSegmentCount; //!< Number of PFlash access segments.
} flash_driver_t;

/*!
* @brief Enumeration for the two possible options of flash read resource command.
*/
typedef enum _flash_read_resource_option
{
    kFlashResource_ProgramIFR = 0,
    kFlashResource_VersionID = 1
} flash_read_resource_option_t;

//! @brief Interface for the flash driver.
typedef struct FlashDriverInterface
{
    uint32_t version; //!< flash driver API version number.
    status_t (*flash_init)(flash_driver_t *driver);
    status_t (*flash_erase_all)(flash_driver_t *driver, uint32_t key);
    status_t (*flash_erase_all_unsecure)(flash_driver_t *driver, uint32_t key);
    status_t (*flash_erase)(flash_driver_t *driver, uint32_t start, uint32_t lengthInBytes, uint32_t key);
    status_t (*flash_program)(flash_driver_t *driver, uint32_t start, uint32_t *src, uint32_t lengthInBytes);
    status_t (*flash_get_security_state)(flash_driver_t *driver, flash_security_state_t *state);
    status_t (*flash_security_bypass)(flash_driver_t *driver, const uint8_t *backdoorKey);
    status_t (*flash_verify_erase_all)(flash_driver_t *driver, flash_margin_value_t margin);
    status_t (*flash_verify_erase)(flash_driver_t *driver,
                                   uint32_t start,
                                   uint32_t lengthInBytes,
                                   flash_margin_value_t margin);
    status_t (*flash_verify_program)(flash_driver_t *driver,
                                     uint32_t start,
                                     uint32_t lengthInBytes,
                                     const uint8_t *expectedData,
                                     flash_margin_value_t margin,
                                     uint32_t *failedAddress,
                                     uint32_t *failedData);
    status_t (*flash_get_property)(flash_driver_t *driver, flash_property_t whichProperty, uint32_t *value);
    status_t (*flash_register_callback)(flash_driver_t *driver, flash_callback_t callback);
    status_t (*flash_program_once)(flash_driver_t *driver, uint32_t index, uint32_t *src, uint32_t lengthInBytes);
    status_t (*flash_read_once)(flash_driver_t *driver, uint32_t index, uint32_t *dst, uint32_t lengthInBytes);
    status_t (*flash_read_resource)(flash_driver_t *driver,
                                    uint32_t start,
                                    uint32_t *dst,
                                    uint32_t lengthInBytes,
                                    flash_read_resource_option_t option);
} flash_driver_interface_t;

//! @brief Interface for AES 128 functions
typedef struct AesDriverInterface
{
    void (*aes_init)(uint32_t *key);
    void (*aes_encrypt)(uint32_t *in, uint32_t *key, uint32_t *out);
    void (*aes_decrypt)(uint32_t *in, uint32_t *key, uint32_t *out);
} aes_driver_interface_t;

typedef struct BootloaderTree
{
    void (*runBootloader)(void *arg); //!< Function to start the bootloader executing.
    uint32_t version;                 //!< Bootloader version number.
    const char *copyright;            //!< Copyright string.
    const uint32_t *reserved;
    const flash_driver_interface_t *flashDriver; //!< Flash driver API.
    const aes_driver_interface_t *aesDriver;     //!< AES driver API.
} bootloader_tree_t;

#define BOOTLOADER_TREE_ROOT (0x1c00001cul)
enum
{
    kFlashEraseKey = 0x6B65666BUL,
#ifndef BL_TARGET_FLASH
    kApplicationBase = 0x0000,
#else
    kApplicationBase = 0xa000,
#endif
};

// See misc.h for more details.
void app_enter_bootloader(void)
{
    // Disable IRQ first
    __disable_irq();

    // Get Bootloader Tree pointer.
    const bootloader_tree_t *bootloaderTree = *(const bootloader_tree_t **)BOOTLOADER_TREE_ROOT;
    // Initialize Flash Driver
    flash_driver_t flashInstance;
    uint32_t status = bootloaderTree->flashDriver->flash_init(&flashInstance);
    if (status != 0)
    {
        // Handle error code here
    }
    // Save the flash config field before erase
    uint32_t flashConfigField[4];
    const uint32_t *flashConfigFieldStart = (const uint32_t *)(kApplicationBase + 0x400);
    for (uint32_t i = 0; i < sizeof(flashConfigField) / sizeof(flashConfigField[0]); i++)
    {
        flashConfigField[i] = *flashConfigFieldStart++;
    }
    // Erase the first sector.
    status = bootloaderTree->flashDriver->flash_erase(&flashInstance, kApplicationBase, 0x800, kFlashEraseKey);
    if (status != 0)
    {
        // Handle error code here
    }
    // Write the flash config field back.
    status = bootloaderTree->flashDriver->flash_program(&flashInstance, kApplicationBase + 0x400, flashConfigField,
                                                        sizeof(flashConfigField));
    if (status != 0)
    {
        // Handle error code here
    }

// Enter Bootloader
#ifndef BL_TARGET_FLASH
    bootloaderTree->runBootloader(0);
#else
    NVIC_SystemReset();
#endif
}
