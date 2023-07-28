/*
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>

#include "bl_context.h"
#include "bootloader.h"
#include "bootloader_common.h"
#include "flexspi_nor_flash.h"
#include "flexspi_nor_memory.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#include "fsl_rtos_abstraction.h"
#include "memory.h"
#include "microseconds.h"
#include "normal_memory.h"

#if BL_FEATURE_FLEXSPI_NOR_MODULE

#ifndef FLEXSPI_NOR_INSTANCE
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
#define FLEXSPI_NOR_INSTANCE flexspi_nor_get_instance()
#else
#define FLEXSPI_NOR_INSTANCE BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
#endif

#ifndef FLEXSPI_NOR_ERASE_VERIFY
#define FLEXSPI_NOR_ERASE_VERIFY BL_FEATURE_FLEXSPI_NOR_MODULE_ERASE_VERIFY
#endif

#if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
#include "bl_nor_encrypt.h"
#endif
#if BL_FEATURE_GEN_KEYBLOB
#include "bl_keyblob.h"
#endif // BL_FEATURE_GEN_KEYBLOB

#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
#define FLASH_AMBA_BASE flexspi_nor_get_amba_addr()
#else
#define FLASH_AMBA_BASE BL_FLEXSPI_AMBA_BASE
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL

#ifdef BL_FLASH_CFG_BLOCK_OFFSET
#define FLASH_CFG_BLOCK_BASE (FLASH_AMBA_BASE + BL_FLASH_CFG_BLOCK_OFFSET)
#else
#define FLASH_CFG_BLOCK_BASE (FLASH_AMBA_BASE)
#endif
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#ifndef BL_TARGET_FLASH
#define lock_acquire()
#define lock_release()
#endif

#ifndef BL_FLEXSPI_NOR_CFG_OPT0
// Assume the FLASH device is an QSPI FLASH which supports JESD216 or later revision, works at 120/133MHz
#define BL_FLEXSPI_NOR_CFG_OPT0 (0xc0000007u)
#endif // BL_FLEXSPI_NOR_CFG_OPT0

#ifndef BL_FLEXSPI_NOR_CFG_OPT1
#define BL_FLEXSPI_NOR_CFG_OPT1 (0u)
#endif // BL_FLEXSPI_NOR_CFG_OPT0

enum
{
    kFlashDefaultPattern = 0xFF,
};

//! @brief FLEXSPI NOR memory feature information
//!
//! An instance is maintained in this file, will is used to keep key information for write and flush
//! operations.
typedef struct _flexspi_nor_mem_context
{
    bool isConfigured; //!< The state which indicates whether FlexSPI block is successfully
//! configured.
#if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
    bool isEncryptProgrammingEnabled; //!< The state which indicates whether encrypted programming is enabled or not.
#endif                                // BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
    bool isAddingToBuffer;            //!< State used of determine whether it is the first write at
                                      //!< Start address of one page
    uint32_t writeAddress;            //!< This address is used to record the address which is used
                                      //!< to write the whole page into FlexSPI memory
    uint32_t offset;                  //!< A variable which is used to indicate if the buffer is
                                      //!< full.
    uint32_t nextStartAddress;        //!< A variable is used to indicate if recent two writes
                                      //!< are continuous
    uint32_t buffer[kFlexSpiNorMemory_MaxPageSize / sizeof(uint32_t)]; //!< A buffer which is used to buffer a full
                                                                       //!< page of data
#if BL_FEATURE_GEN_KEYBLOB
    bool has_keyblob;
    uint32_t keyblob_offset;
#endif // BL_FEATURE_GEN_KEYBLOB
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
    uint32_t instance;
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
} flexspi_nor_mem_context_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
//! @brief Convert flexspi amba address to physical address
static uint32_t flexspi_get_phy_address(uint32_t mapAddr);
//! @brief Init function for flexspi nor module driver
static status_t flexspi_nor_memory_init(void);
//! @brief Page read function for flexspi nor module driver
static status_t flexspi_nor_memory_page_program(uint32_t address, const uint32_t *src);
//! @brief Erase all fcuntion for flexspi nor module driver
static status_t flexspi_nor_memory_erase_all(void);
//! @brief Erase function for flexspi nor module driver
static status_t flexspi_nor_memory_erase(uint32_t address, uint32_t length);
//! @brief Clear cache of flexspi nor module driver
static void flexspi_nor_memory_clear_cache(void);
//! @brief Read Serial NOR memory
static status_t flexspi_nor_memory_read(uint32_t *dst, uint32_t start, uint32_t bytes);

//! @brief verify if serial nor memory is erased.
static bool is_flexspi_nor_mem_erased(uint32_t start, uint32_t length);
// Check wether the data in Serial NOR matches the data to check
static bool flexspi_nor_memory_check(uint32_t start, uint8_t *data_to_check, uint32_t bytes);
#if BL_FEATURE_GEN_KEYBLOB
static status_t check_update_keyblob_info(void *config);
#endif // BL_FEATURE_GEN_KEYBLOB
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static flexspi_nor_config_t s_flexspiNorConfigBlock; //!< Configuration block for flexspi

//! @brief Context of Flexspi operation.
static flexspi_nor_mem_context_t s_flexspiNorContext = {
    .isConfigured = false,
    .isAddingToBuffer = false,
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
    .instance = BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE,
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
};

//! @brief Interface to flexspi memory operations
const memory_region_interface_t g_flexspiMemoryInterface = {
    .init = flexspi_nor_mem_init,
    .read = flexspi_nor_mem_read,
    .write = flexspi_nor_mem_write,
#if !BL_MIN_PROFILE
    .fill = flexspi_nor_mem_fill,
#endif // #if !BL_MIN_PROFILE
    .erase = flexspi_nor_mem_erase,
    .config = flexspi_nor_mem_config,
    .flush = flexspi_nor_mem_flush,
};
#if BL_FEATURE_FLEXSPI_ALIAS_AREA
//! @brief Interface to flexspi alias area operations
const memory_region_interface_t g_flexspiAliasAreaInterface = {
    .init = flexspi_nor_alias_init,
    .read = flexspi_nor_mem_read,
    .write = flexspi_nor_alias_write,
#if !BL_MIN_PROFILE
    .fill = flexspi_nor_alias_fill,
#endif // #if !BL_MIN_PROFILE
    .erase = flexspi_nor_alias_erase,
    .config = flexspi_nor_mem_config,
    .flush = flexspi_nor_mem_flush,
};
#endif // #if BL_FEATURE_FLEXSPI_ALIAS_AREA

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

//! @brief Get the instance of current flexspi nor
static uint32_t flexspi_nor_get_instance()
{
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
    return s_flexspiNorContext.instance;
#else
    return FLEXSPI_NOR_INSTANCE;
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
}

//! @brief Get the amba of current flexspi nor
uint32_t flexspi_nor_get_amba_addr()
{
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
    switch (flexspi_nor_get_instance())
    {
#ifdef FlexSPI0_AMBA_BASE
        case 0:
            return FlexSPI0_AMBA_BASE;
#endif
#ifdef FlexSPI1_AMBA_BASE
        case 1:
            return FlexSPI1_AMBA_BASE;
#endif
#ifdef FlexSPI2_AMBA_BASE
        case 2:
            return FlexSPI2_AMBA_BASE;
#endif
        default:
            return 0; // do not rely on this for validity check
    }
#else
    return FLASH_AMBA_BASE;
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
}

//! @brief Get the amba of current flexspi nor
uint32_t flexspi_nor_get_alias_addr()
{
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
    switch (flexspi_nor_get_instance())
    {
#ifdef FlexSPI0_ALIAS_BASE
        case 0:
            return FlexSPI0_ALIAS_BASE;
#endif
#ifdef FlexSPI1_ALIAS_BASE
        case 1:
            return FlexSPI1_ALIAS_BASE;
#endif
#ifdef FlexSPI2_ALIAS_BASE
        case 2:
            return FlexSPI2_ALIAS_BASE;
#endif
        default:
            return 0; // do not rely on this for validity check
    }
#else
    return FLASH_AMBA_BASE;
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
}

#if BL_FEATURE_GEN_KEYBLOB
static status_t check_update_keyblob_info(void *config)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((config == NULL) || (s_flexspiNorContext.isConfigured == false))
        {
            break;
        }

        // Try to read Key blob info based on config
        keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
        if (keyblob_info->option.B.tag != kKeyBlobInfoOption_Tag)
        {
            break;
        }

        int32_t keyblob_info_type = keyblob_info->option.B.type;
        if ((keyblob_info_type != kKeyBlobInfoType_Program) && (keyblob_info_type != kKeyBlobInfoType_Update))
        {
            break;
        }

        if (keyblob_info_type == kKeyBlobInfoType_Update)
        {
            status = keyblob_update(keyblob_info);
            if (status != kStatus_Success)
            {
                s_flexspiNorContext.has_keyblob = false;
                break;
            }
            s_flexspiNorContext.keyblob_offset = keyblob_info->keyblob_offset;
            s_flexspiNorContext.has_keyblob = true;
        }
        else if (keyblob_info_type == kKeyBlobInfoType_Program)
        {
            if (!s_flexspiNorContext.has_keyblob)
            {
                break;
            }
            uint32_t index = keyblob_info->option.B.image_index;
            if (index != 0)
            {
                break;
            }

            uint32_t image_start = 0;
            uint32_t image_max_size = 0;
            uint32_t page_size;
            status = flexspi_nor_get_property(kFlexspiNorProperty_StartAddress, &image_start);
            if (status != kStatus_Success)
            {
                break;
            }
            status = flexspi_nor_get_property(kFlexspiNorProperty_TotalFlashSize, &image_max_size);
            if (status != kStatus_Success)
            {
                break;
            }
            status = flexspi_nor_get_property(kFlexspiNorProperty_PageSize, &page_size);
            if (status != kStatus_Success)
            {
                break;
            }

            uint32_t keyblob_offset = s_flexspiNorContext.keyblob_offset;
            uint32_t keyblob_addr = image_start + keyblob_offset;
            uint8_t *keyblob_buffer;
            uint32_t keyblob_size;
            status = keyblob_get(&keyblob_buffer, &keyblob_size);
            if (status != kStatus_Success)
            {
                break;
            }

            // Check key blob address range
            if ((keyblob_size + keyblob_offset) > image_max_size)
            {
                status = kStatusMemoryRangeInvalid;
                break;
            }

            // Invalid key blob address, key blob must be page size aligned.
            if (keyblob_addr & (page_size - 1))
            {
                status = kStatusMemoryAlignmentError;
                break;
            }

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
            if (!is_flexspi_nor_mem_erased(keyblob_addr, page_size))
            {
                status = kStatusMemoryCumulativeWrite;
                break;
            }
#endif
            status = flexspi_nor_mem_write(keyblob_addr, keyblob_size, keyblob_buffer);
            if (status != kStatus_Success)
            {
                break;
            }

            status = flexspi_nor_mem_flush();
            if (status != kStatus_Success)
            {
                break;
            }
        }
    } while (0);

    return status;
}
#endif // #if BL_FEATURE_GEN_KEYBLOB

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_mem_init(void)
{
#if defined(BL_TARGET_FLASH) && (BL_TARGET_FLASH)
#ifndef BL_FLLEXSPI_NOR_CFG_BLOCK
    serial_nor_config_option_t option;
    option.option0.U = BL_FLEXSPI_NOR_CFG_OPT0;
    option.option1.U = BL_FLEXSPI_NOR_CFG_OPT1;

    if (option.option0.B.tag == kSerialNorCfgOption_Tag)
    {
        return flexspi_nor_mem_config((uint32_t *)&option);
    }
#else
    extern const flexspi_nor_config_t flexspi_config;
    if (flexspi_config.memConfig.tag == FLEXSPI_CFG_BLK_TAG)
    {
        flexspi_nor_config_t configBlock;
        memcpy(&configBlock, &flexspi_config, sizeof(flexspi_config));
        return flexspi_nor_mem_config((uint32_t *)&configBlock);
    }
#endif

    return kStatus_Fail;
#else
    /*
     * NOTE: The SPI NOR instruction set may be switched during Boot
     *       So, here bootloader doesn't try to read the configuration
     *       block based on Fuse settings.
     */
    return kStatus_Fail;
#endif
}

#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
static bool flexspi_nor_instance_available(uint32_t inst)
{
    switch (inst)
    {
#if defined(FlexSPI0_AMBA_BASE)
        case 0:
            return true;
#endif
#if defined(FlexSPI1_AMBA_BASE)
        case 1:
            return true;
#endif
#if defined(FlexSPI2_AMBA_BASE)
        case 2:
            return true;
#endif
        default:
            return false;
    }
}
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL

// See memory.h for documentation on this function.
status_t flexspi_nor_mem_config(uint32_t *config)
{
    status_t status = kStatus_InvalidArgument;

    bool isNorConfigOption = false;
    bool needProgramConfigBlock = false;

    do
    {
        uint32_t startAddr = (uint32_t)config;
        uint32_t endAddr = startAddr + sizeof(flexspi_nor_config_t) - 1;

        // Should check the config is in valid internal space.
        if ((!is_valid_application_location(startAddr)) || (!is_valid_application_location(endAddr)))
        {
            break;
        }

        // Try to check whether the 'config' variable is an option supported by flexspi_nor_get_config function
        serial_nor_config_option_t *option = (serial_nor_config_option_t *)startAddr;
        // Try to check whether the number stored at the specified addres is a magic number
        uint32_t magic_number = *(uint32_t *)startAddr;
        // Try to check whether the config variable stored at the specified addres is a whole config block
        flexspi_nor_config_t *norConfig = (flexspi_nor_config_t *)config;
#if BL_FEATURE_GEN_KEYBLOB
        keyblob_info_t *keyblob_info = (keyblob_info_t *)config;
#endif // BL_FEATURE_GEN_KEYBLOB
#if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
        nor_encrypt_option_t *norEncryptOption = (nor_encrypt_option_t *)config;
#endif

#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
        if (MAGIC_NUMBER_FLEXSPI_NOR_PRECFG == option->option0.P.magic)
        {
            if (!flexspi_nor_instance_available(option->option0.P.cf9_field))
            {
                status = kStatus_InvalidArgument;
                break;
            }

            if (option->option0.P.cf9_field != s_flexspiNorContext.instance)
            {
                s_flexspiNorContext.instance = option->option0.P.cf9_field;
                s_flexspiNorContext.isConfigured = false;
            }
            status = kStatus_Success;
            break;
        }
        else if (option->option0.B.tag == kSerialNorCfgOption_Tag)
#else
        if (option->option0.B.tag == kSerialNorCfgOption_Tag)
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
        {
            status = flexspi_nor_get_config(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock, option);
            if (status != kStatus_Success)
            {
                break;
            }
            isNorConfigOption = true;
        }
        else if (magic_number == MAGIC_NUMBER_FLEXSPI_NOR_PROGRAM_CFG)
        {
            if (s_flexspiNorContext.isConfigured)
            {
                needProgramConfigBlock = true;
            }
        }
        else if (norConfig->memConfig.tag == FLEXSPI_CFG_BLK_TAG) // Fallback mode
        {
            // Over-write config block.
            memcpy(&s_flexspiNorConfigBlock, norConfig, sizeof(flexspi_nor_config_t));
            isNorConfigOption = true;
        }
#if BL_FEATURE_GEN_KEYBLOB
        else if (keyblob_info->option.B.tag == kKeyBlobInfoOption_Tag)
        {
            status = check_update_keyblob_info(config);
            if (status != kStatus_Success)
            {
                break;
            }
            else
            {
                status = kStatus_Success;
            }
        }
#endif // BL_FEATURE_GEN_KEYBLOB
#if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
        else if (norEncryptOption->option0.B.tag == kNorEncyptOption_Tag)
        {
            if (s_flexspiNorContext.isConfigured && bl_nor_encrypt_region_info_valid(config))
            {
                status = bl_nor_encrypt_init(config);
                if (status == kStatus_Success)
                {
                    s_flexspiNorContext.isEncryptProgrammingEnabled = true;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            s_flexspiNorContext.isEncryptProgrammingEnabled = false;
            break;
        }
#else
        else
        {
            break;
        }
#endif // BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING

        if (isNorConfigOption)
        {
            // All tag check is passed. Then start to configure SPI NOR device.
            // First, mark SPI NOR as not configured.
            s_flexspiNorContext.isConfigured = false;

            status = flexspi_nor_flash_init(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock);
            if (status != kStatus_Success)
            {
                break;
            }
            flexspi_nor_memory_clear_cache();

            status = flexspi_nor_update_map();
            if (status != kStatus_Success)
            {
                break;
            }

            s_flexspiNorContext.isConfigured = true;
        } //  if (isNorConfigOption)

        if (needProgramConfigBlock)
        {
            bool isParallelMode = flexspi_is_parallel_mode(&s_flexspiNorConfigBlock.memConfig);
            volatile uint8_t *config_buffer = (uint8_t *)&s_flexspiNorConfigBlock;
            bool hasError = false;
            uint32_t temp_buf[sizeof(s_flexspiNorConfigBlock) / sizeof(uint32_t)];
            do
            {
                // Clear parallel mode setting temporarily
                if (isParallelMode)
                {
                    memcpy((void *)&temp_buf, &s_flexspiNorConfigBlock, sizeof(temp_buf));
                    config_buffer = (uint8_t *)temp_buf;

                    s_flexspiNorConfigBlock.memConfig.controllerMiscOption &=
                        ~FLEXSPI_BITMASK(kFlexSpiMiscOffset_ParallelEnable);
                    s_flexspiNorConfigBlock.pageSize /= 2;
                    s_flexspiNorConfigBlock.sectorSize /= 2;
                    s_flexspiNorConfigBlock.blockSize /= 2;
                    status = flexspi_nor_flash_init(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock);
                    if (status != kStatus_Success)
                    {
                        hasError = true;
                        break;
                    }
                }

                // Update flexspi nor config block if it is not present
                if (is_flexspi_nor_mem_erased(FLASH_CFG_BLOCK_BASE, sizeof(flexspi_nor_config_t)))
                {
                    bool needSwapConfigBlock = s_flexspiNorConfigBlock.isDataOrderSwapped;
                    // Swap before write
                    if (needSwapConfigBlock)
                    {
                        uint8_t *configBlock = (uint8_t *)&s_flexspiNorConfigBlock;
                        uint8_t *temp_buf_8 = (uint8_t *)&temp_buf;
                        for (uint32_t i = 0; i < sizeof(s_flexspiNorConfigBlock); i += 2)
                        {
                            temp_buf_8[i] = configBlock[i + 1];
                            temp_buf_8[i + 1] = configBlock[i];
                        }
                        config_buffer = (uint8_t *)temp_buf;
                    }
                    status = flexspi_nor_mem_write(FLASH_CFG_BLOCK_BASE, sizeof(flexspi_nor_config_t),
                                                   (const uint8_t *)config_buffer);
                    if (status != kStatus_Success)
                    {
                        // Erase the error config block, ususally, erase instruciton is right while the read instruction
                        // is
                        // wrong
                        // because erase always executes at 30MHz while the read can work at the specified frequency
                        flexspi_nor_memory_erase(FLASH_CFG_BLOCK_BASE, sizeof(s_flexspiNorConfigBlock));
                        hasError = true;
                        break;
                    }
                    status = flexspi_nor_mem_flush();
                    if (status != kStatus_Success)
                    {
                        // Erase the error config block
                        flexspi_nor_mem_erase(FLASH_CFG_BLOCK_BASE, s_flexspiNorConfigBlock.sectorSize);
                        hasError = true;
                        break;
                    }
                } // if (is_flexspi_nor_mem_erased(FLASH_CFG_BLOCK_BASE, sizeof(flexspi_nor_config_t)))
                else
                {
                    status = kStatusMemoryCumulativeWrite;
                    hasError = true;
                    break;
                }
            } while (0);

            // Restore Flash configuration
            if (isParallelMode)
            {
                if (config_buffer != (uint8_t *)&s_flexspiNorConfigBlock)
                {
                    memcpy(&s_flexspiNorConfigBlock, (void *)config_buffer, sizeof(s_flexspiNorConfigBlock));
                }
                status = flexspi_nor_flash_init(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock);
                if (status != kStatus_Success)
                {
                    break;
                }
            }
            if (hasError)
            {
                break;
            }
        } // if (needProgramConfigBlock)

#if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
        if (!s_flexspiNorContext.isEncryptProgrammingEnabled)
        {
            if (bl_nor_encrypt_has_encrypted_region())
            {
                s_flexspiNorContext.isEncryptProgrammingEnabled = true;
            }
        }
        else
        {
            for (uint32_t i = 0; i < 2; i++)
            {
                uint32_t start, bytes;
                status_t status = bl_nor_encrypt_get_config_block(i, &start, &bytes);
                if (status == kStatus_Success)
                {
                    // Program Encrypted Configuration Info to destination
                    if (is_flexspi_nor_mem_erased(FLASH_AMBA_BASE + BL_PROT_REGION_BLOCK_OFFSET(i), 512))
                    {
                        status = flexspi_nor_mem_write(FLASH_AMBA_BASE + BL_PROT_REGION_BLOCK_OFFSET(i), bytes,
                                                       (uint8_t *)start);
                        if (status != kStatus_Success)
                        {
                            break;
                        }
                        status = flexspi_nor_mem_flush();
                        if (status != kStatus_Success)
                        {
                            break;
                        }
                    }
                }
            }
        }
#endif // #if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING

    } while (0);

    return status;
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_mem_read(uint32_t address, uint32_t length, uint8_t *buffer)
{
    if (!is_flexspi_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }
    uint32_t phy_address = flexspi_get_phy_address(address);
    return flexspi_nor_memory_read((uint32_t *)buffer, phy_address, length);
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    // Note: the check for "length != 0" and "range not in reserved region" is done in mem_write().
    assert(length);
    assert(buffer);

    // Check if flexspi is configured
    if (!is_flexspi_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    status_t status;
    uint32_t writeLength;

    while (length)
    {
        // If buffer is invalid, it means it is a new write operation.
        if (!s_flexspiNorContext.isAddingToBuffer)
        {
            // If address is page aligned, it means it is a valid start address, else return an error status
            if (address & (s_flexspiNorConfigBlock.pageSize - 1))
            {
                return kStatus_FlexSPINOR_WriteAlignmentError;
            }

            // Start buffering data
            s_flexspiNorContext.isAddingToBuffer = true;
            s_flexspiNorContext.offset = 0;
            s_flexspiNorContext.writeAddress = address;
        }
        else
        {
            // In this case, it means recent two writes are not continuous, should flush last cached data into memory,
            // then switch to processsing of this write operation.
            if ((s_flexspiNorContext.offset + s_flexspiNorContext.writeAddress) != address)
            {
                // flush cached data into target memory,
                status = flexspi_nor_mem_flush();
                if (status != kStatus_Success)
                {
                    return status;
                }
                // Start processing this write
                continue;
            }
            // Otherwise, it means recent two writes are continuous, continue to buffer data until whole page gets
            // buffered.
        }

        if (s_flexspiNorContext.offset + length < s_flexspiNorConfigBlock.pageSize)
        {
            writeLength = length;
        }
        else
        {
            writeLength = s_flexspiNorConfigBlock.pageSize - s_flexspiNorContext.offset;
        }

        // Copy data to internal buffer
        uint8_t *p_buffer_8 = (uint8_t *)s_flexspiNorContext.buffer;
        memcpy(&p_buffer_8[s_flexspiNorContext.offset], buffer, writeLength);
        s_flexspiNorContext.offset += writeLength;
        address += writeLength;
        buffer += writeLength;
        length -= writeLength;

        assert(s_flexspiNorContext.offset <= s_flexspiNorConfigBlock.pageSize);
        // If the buffer is full, it is time to flush cached data to target memory.
        if (s_flexspiNorContext.offset == s_flexspiNorConfigBlock.pageSize)
        {
            status = flexspi_nor_mem_flush();
            if (status != kStatus_Success)
            {
                return status;
            }
        }
    }

    return kStatus_Success;
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_mem_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    // Check if flexspi is configured
    if (!is_flexspi_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    uint32_t alignedLength = ALIGN_DOWN(length, 4U);
    uint32_t leftLengthInBytes = length & 3;

    status_t status;

    while (alignedLength)
    {
        status = flexspi_nor_mem_write(address, 4, (const uint8_t *)&pattern);
        if (status != kStatus_Success)
        {
            return status;
        }

        address += 4;
        alignedLength -= 4;
    }

    if (leftLengthInBytes)
    {
        status = flexspi_nor_mem_write(address, leftLengthInBytes, (const uint8_t *)&pattern);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    return flexspi_nor_mem_flush();
}

static bool is_flexspi_nor_mem_erased(uint32_t start, uint32_t length)
{
    bool is_erased = true;
    uint32_t read_length;
    while (length)
    {
        uint32_t readBuffer[kFlexSpiNorMemory_MaxPageSize / sizeof(uint32_t)];
        read_length = length < sizeof(readBuffer) ? length : sizeof(readBuffer);
        uint32_t phy_address = flexspi_get_phy_address(start);
        flexspi_nor_memory_read(readBuffer, phy_address, read_length);
        uint32_t *buf_32 = &readBuffer[0];
        for (uint32_t i = 0; i < read_length / 4; i++)
        {
            if (*buf_32++ != 0xFFFFFFFFUL)
            {
                return false;
            }
        }
        length -= read_length;
        start += read_length;
    }
    return is_erased;
}

static bool flexspi_nor_memory_check(uint32_t start, uint8_t *data_to_check, uint32_t length)
{
    uint32_t buffer[kFlexSpiNorMemory_MaxPageSize / sizeof(uint32_t)];

    uint32_t read_length;
    while (length)
    {
        read_length = length < sizeof(s_flexspiNorContext.buffer) ? length : sizeof(s_flexspiNorContext.buffer);
        uint32_t phy_address = flexspi_get_phy_address(start);
        flexspi_nor_memory_read((uint32_t *)&buffer, phy_address, read_length);

        if (memcmp(buffer, data_to_check, read_length) != 0)
        {
            return false;
        }

        length -= read_length;
        start += read_length;
        data_to_check += read_length;
    }

    return true;
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_mem_flush(void)
{
    status_t status = kStatus_Success;

    if (s_flexspiNorContext.isAddingToBuffer)
    {
        s_flexspiNorContext.isAddingToBuffer = false;
        uint8_t *p_buffer_8 = (uint8_t *)s_flexspiNorContext.buffer;
        // Fill unused region with 0xFFs.
        if (s_flexspiNorContext.offset != s_flexspiNorConfigBlock.pageSize)
        {
            memset(&p_buffer_8[s_flexspiNorContext.offset], 0xFF,
                   s_flexspiNorConfigBlock.pageSize - s_flexspiNorContext.offset);
        }

#if BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE
        if (!is_flexspi_nor_mem_erased(s_flexspiNorContext.writeAddress, s_flexspiNorConfigBlock.pageSize))
        {
            s_flexspiNorContext.isAddingToBuffer = false;
            return kStatusMemoryCumulativeWrite;
        }
#endif // BL_FEATURE_FLASH_CHECK_CUMULATIVE_WRITE

#if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
        if (bl_nor_in_encrypted_region(s_flexspiNorContext.writeAddress, s_flexspiNorConfigBlock.pageSize))
        {
            status = bl_nor_encrypt_data(s_flexspiNorContext.writeAddress, s_flexspiNorConfigBlock.pageSize,
                                         (uint32_t *)s_flexspiNorContext.buffer);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
#endif
        // Write whole page to spi flash
        status =
            flexspi_nor_memory_page_program(s_flexspiNorContext.writeAddress, (uint32_t *)s_flexspiNorContext.buffer);
        // Clear related states.
        s_flexspiNorContext.isAddingToBuffer = false;
        s_flexspiNorContext.offset = 0;
        if (status != kStatus_Success)
        {
            return status;
        }
#if (__CORTEX_M == 7)
        if (SCB->CCR & SCB_CCR_DC_Msk)
        {
            SCB_InvalidateDCache_by_Addr((uint32_t *)s_flexspiNorContext.writeAddress,
                                         s_flexspiNorConfigBlock.pageSize);
        }
#endif
        property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;
        if (propertyStore->verifyWrites == true)
        {
            // Verify whether the data has been programmed to Serial NOR flash successfully.
            if (!flexspi_nor_memory_check(s_flexspiNorContext.writeAddress, (uint8_t *)s_flexspiNorContext.buffer,
                                          s_flexspiNorConfigBlock.pageSize))
            {
                return kStatus_FlexSPINOR_CommandFailure;
            }
        }
    }
    return status;
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_mem_erase(uint32_t address, uint32_t length)
{
    assert(length);

    // Check if QuadSPI is configured
    if (!is_flexspi_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    uint32_t sectorSize = s_flexspiNorConfigBlock.sectorSize;
    uint32_t alignedAddress = ALIGN_DOWN(address, sectorSize);
    uint32_t alignedLength = ALIGN_UP(address + length, sectorSize) - alignedAddress;

    status_t status = flexspi_nor_memory_erase(alignedAddress, alignedLength);
    if (status != kStatus_Success)
    {
        return status;
    }

#if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
    bl_nor_encrypt_region_refresh(alignedAddress, sectorSize);
#endif

#if (__CORTEX_M == 7)
    if (SCB->CCR & SCB_CCR_DC_Msk)
    {
        SCB_InvalidateDCache_by_Addr((uint32_t *)alignedAddress, sectorSize);
    }
#endif

#if FLEXSPI_NOR_ERASE_VERIFY
    property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;
    if (propertyStore->verifyWrites == true)
    {
        if (!is_flexspi_nor_mem_erased(alignedAddress, alignedLength))
        {
            return kStatus_FlexSPINOR_CommandFailure;
        }
    }
#endif // #if FLEXSPI_NOR_ERASE_VERIFY

    return kStatus_Success;
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_mem_erase_all(void)
{
    status_t status = kStatus_Fail;
    // Check if QuadSPI is configured
    if (!is_flexspi_nor_configured())
    {
        return kStatusMemoryNotConfigured;
    }

    uint32_t startAddress, lengthInBytes, sectorSize;
    status = flexspi_nor_get_property(kFlexspiNorProperty_StartAddress, &startAddress);
    if (status != kStatus_Success)
    {
        return status;
    }
    status = flexspi_nor_get_property(kFlexspiNorProperty_TotalFlashSize, &lengthInBytes);
    if (status != kStatus_Success)
    {
        return status;
    }
    status = flexspi_nor_get_property(kFlexspiNorProperty_SectorSize, &sectorSize);
    if (status != kStatus_Success)
    {
        return status;
    }

// Decompose the the flash erase all into two region erases.
#if (defined(BL_TARGET_FLASH) && (BL_TARGET_FLASH > 0))
    reserved_region_t *reservedRegion =
        &g_bootloaderContext.propertyInterface->store->reservedRegions[kProperty_FlashReservedRegionIndex];

    uint32_t alignedStart = ALIGN_UP(reservedRegion->endAddress, sectorSize);
    uint32_t eraseSize = (uint32_t)((uint64_t)startAddress + lengthInBytes - alignedStart);
    startAddress = alignedStart;
    lengthInBytes = eraseSize;
    status = flexspi_nor_mem_erase(startAddress, lengthInBytes);
#else
    status = flexspi_nor_memory_erase_all();
#if FLEXSPI_NOR_ERASE_VERIFY
    property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;
    if (propertyStore->verifyWrites == true)
    {
        if (!is_flexspi_nor_mem_erased(startAddress, lengthInBytes))
        {
            return kStatus_FlexSPINOR_CommandFailure;
        }
    }
#endif // #if FLEXSPI_NOR_ERASE_VERIFY
#endif // #if BL_TARGET_FLASH
    if (status != kStatus_Success)
    {
        return status;
    }

#if BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING
    bl_nor_encrypt_region_refresh(startAddress, lengthInBytes);
#endif // BL_FEATURE_FLEXSPI_ENCRYPT_PROGRAMMING

    return kStatus_Success;
}

#if BL_FEATURE_FLEXSPI_ALIAS_AREA
// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_alias_init(void)
{
    // Do nothing, mem init will init alias area at the same time.
    return kStatus_Success;
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_alias_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    return flexspi_nor_mem_write(flexspi_get_map_address(address), length, buffer);
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_alias_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    return flexspi_nor_mem_fill(flexspi_get_map_address(address), length, pattern);
}

// See flexspi_nor_memory.h for documentation on this function.
status_t flexspi_nor_alias_erase(uint32_t address, uint32_t length)
{
    return flexspi_nor_mem_erase(flexspi_get_map_address(address), length);
}
#endif // #if BL_FEATURE_FLEXSPI_ALIAS_AREA

//! @brief Get Property from flexspi driver
status_t flexspi_nor_get_property(uint32_t whichProperty, uint32_t *value)
{
    if (value == NULL)
    {
        return kStatus_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kFlexspiNorProperty_InitStatus:
            *value = is_flexspi_nor_configured() ? kStatus_Success : kStatusMemoryNotConfigured;
            break;

        case kFlexspiNorProperty_StartAddress:
            *value = FLASH_AMBA_BASE;
            break;

        case kFlexspiNorProperty_TotalFlashSizeInKBytes:
        {
            uint32_t totalFlashSizeInKbytes =
                s_flexspiNorConfigBlock.memConfig.sflashA1Size + s_flexspiNorConfigBlock.memConfig.sflashA2Size +
                s_flexspiNorConfigBlock.memConfig.sflashB1Size + s_flexspiNorConfigBlock.memConfig.sflashB2Size;

            *value = totalFlashSizeInKbytes / 1024;
        }
        break;

        case kFlexspiNorProperty_PageSize:
            *value = s_flexspiNorConfigBlock.pageSize;
            break;

        case kFlexspiNorProperty_SectorSize:
            *value = s_flexspiNorConfigBlock.sectorSize;
            break;

        case kFlexspiNorProperty_BlockSize:
            if (s_flexspiNorConfigBlock.blockSize)
            {
                *value = s_flexspiNorConfigBlock.blockSize;
            }
            else
            {
                return kStatus_InvalidArgument;
            }
            break;

        case kFlexspiNorProperty_TotalFlashSize:
        {
            uint32_t totalFlashSize =
                s_flexspiNorConfigBlock.memConfig.sflashA1Size + s_flexspiNorConfigBlock.memConfig.sflashA2Size +
                s_flexspiNorConfigBlock.memConfig.sflashB1Size + s_flexspiNorConfigBlock.memConfig.sflashB2Size;

            *value = totalFlashSize;
        }
        break;

        default: // catch inputs that are not recognized
            return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}

//! @brief Get the status of flexspi configuration
bool is_flexspi_nor_configured()
{
    return s_flexspiNorContext.isConfigured;
}

//! @brief Convert flexspi amba address to physical address
static uint32_t flexspi_get_phy_address(uint32_t mapAddr)
{
    return mapAddr - FLASH_AMBA_BASE;
}

static status_t flexspi_nor_memory_init(void)
{
    return flexspi_nor_flash_init(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock);
}

static status_t flexspi_nor_memory_page_program(uint32_t address, const uint32_t *src)
{
    lock_acquire();
    status_t status = flexspi_nor_flash_page_program(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock,
                                                     flexspi_get_phy_address(address), src);
    lock_release();

    return status;
}

static status_t flexspi_nor_memory_erase_all(void)
{
    lock_acquire();
    status_t status = flexspi_nor_flash_erase_all(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock);
    lock_release();

    return status;
}

static status_t flexspi_nor_memory_erase(uint32_t address, uint32_t length)
{
    uint32_t phyAddr = flexspi_get_phy_address(address);
    lock_acquire();
    status_t status = flexspi_nor_flash_erase(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock, phyAddr, length);
    lock_release();
    return status;
}

static void flexspi_nor_memory_clear_cache(void)
{
    flexspi_clear_cache(FLEXSPI_NOR_INSTANCE);
}

static status_t flexspi_nor_memory_read(uint32_t *dst, uint32_t start, uint32_t bytes)
{
    return flexspi_nor_flash_read(FLEXSPI_NOR_INSTANCE, &s_flexspiNorConfigBlock, dst, start, bytes);
}

#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
