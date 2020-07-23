/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "packet/serial_packet.h"
#include "bootloader/bl_context.h"
#include "bootloader/bl_version.h"

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#include "fsl_flash.h"
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
#include "fsl_device_registers.h"
#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
#include "qspi.h"
#endif // BL_FEATURE_QSPI_MODULE
#if defined(BL_FEATURE_CRC_CHECK) && BL_FEATURE_CRC_CHECK
#include "bootloader/bl_app_crc_check.h"
#endif // BL_FEATURE_CRC_CHECK

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

#if (defined(__ICCARM__)) // IAR
#pragma section = ".intvec"
#pragma section = "ApplicationFlash"
#pragma section = "ApplicationRam"
#if defined(BL_TARGET_RAM)
#define __RAM_START ((uint32_t)__section_begin(".intvec"))
#else
#define __RAM_START ((uint32_t)__section_begin("ApplicationRam"))
#endif // #if defined(BL_TARGET_RAM)
#define __RAM_END ((uint32_t)__section_end("ApplicationRam") - 1u)
#define __ROM_START ((uint32_t)__section_begin(".intvec"))
#define __ROM_END ((uint32_t)__section_end("ApplicationFlash"))
#elif (defined(__CC_ARM)) // MDK
extern uint32_t Image$$VECTOR_ROM$$Base[];
extern uint32_t Load$$RW_m_data$$Limit[];
extern char Image$$VECTOR_RAM$$Base[];
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit[];
#define __RAM_START ((uint32_t)Image$$VECTOR_RAM$$Base)
#define __RAM_END ((uint32_t)Image$$ARM_LIB_STACK$$ZI$$Limit - 1u)
#define __ROM_START ((uint32_t)Image$$VECTOR_ROM$$Base)
#define __ROM_END ((uint32_t)Load$$RW_m_data$$Limit)
#elif (defined(__GNUC__)) // GCC
extern uint32_t __VECTOR_RAM[];
extern uint32_t __VECTOR_TABLE[];
extern char __DATA_END[];
extern uint32_t __STACK_TOP[];
#define __RAM_START ((uint32_t)__VECTOR_RAM)
#define __RAM_END ((uint32_t)__STACK_TOP - 1u)
#define __ROM_START ((uint32_t)__VECTOR_TABLE)
#define __ROM_END ((uint32_t)__DATA_END)
#else
#error Unknown toolchain!
#endif // __ICCARM__

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Storage for property values.
static property_store_t g_propertyStore;

// See property.h for documentation on this data structure.
const property_interface_t g_propertyInterface = { bootloader_property_load_user_config, bootloader_property_init,
                                                   bootloader_property_get, bootloader_property_set_uint32,
                                                   &g_propertyStore };

//! @brief Storage for property values computed every time they are read.
static uint32_t s_propertyReturnValue;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
#if defined(BL_FEATURE_EXTERNAL_MEMORY_PROPERTY) && BL_FEATURE_EXTERNAL_MEMORY_PROPERTY
// !@brief Get external memory properties
status_t bootloader_get_external_memory_properties(uint32_t memoryId, external_memory_property_store_t *store);
#endif
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See property.h for documentation on this function.
status_t bootloader_property_load_user_config(void)
{
    bootloader_configuration_data_t *config = &g_bootloaderContext.propertyInterface->store->configurationData;

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    if (is_in_execute_only_region(kBootloaderConfigAreaAddress, sizeof(bootloader_configuration_data_t)))
    {
        (void)memset(config, 0xffu, sizeof(bootloader_configuration_data_t));
    }
    else
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    {
        union
        {
            uint32_t address;
            const void *pAddress;
        } ptr_1;

                union
        {
            bootloader_configuration_data_t *config;
            void *pAddress;
        } ptr_2;

        ptr_1.address = kBootloaderConfigAreaAddress;
        ptr_2.config = config;
        // Copy bootloader configuration data from the flash into the property store.
        (void)memcpy(ptr_2.pAddress, ptr_1.pAddress, sizeof(bootloader_configuration_data_t));

        // Verify tag. If it is invalid, wipe the config data to all 0xff.
        if ((uint8_t)kPropertyStoreTag != config->tag)
        {
            (void)memset(config, 0xff, sizeof(bootloader_configuration_data_t));
        }
    }

    // Update available peripherals based on specific chips
    update_available_peripherals();

    return (int32_t)kStatus_Success;
}

// See property.h for documentation on this function.
status_t bootloader_property_init(void)
{
    property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;

    // Fill in default values.
    propertyStore->bootloaderVersion.name = (char)kBootloader_Version_Name;
    propertyStore->bootloaderVersion.major = (uint8_t)kBootloader_Version_Major;
    propertyStore->bootloaderVersion.minor = (uint8_t)kBootloader_Version_Minor;
    propertyStore->bootloaderVersion.bugfix = (uint8_t)kBootloader_Version_Bugfix;

    propertyStore->serialProtocolVersion.name = (char)kSerialProtocol_Version_Name;
    propertyStore->serialProtocolVersion.major = (uint8_t)kSerialProtocol_Version_Major;
    propertyStore->serialProtocolVersion.minor = (uint8_t)kSerialProtocol_Version_Minor;
    propertyStore->serialProtocolVersion.bugfix = (uint8_t)kSerialProtocol_Version_Bugfix;

    propertyStore->targetVersion.name = (char)kTarget_Version_Name;
    propertyStore->targetVersion.major = (uint8_t)kTarget_Version_Major;
    propertyStore->targetVersion.minor = (uint8_t)kTarget_Version_Minor;
    propertyStore->targetVersion.bugfix = (uint8_t)kTarget_Version_Bugfix;

    propertyStore->verifyWrites = 1u;

    propertyStore->availableCommands = (uint32_t)kAvailableCommands;

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
    /// Initialize flash properties.
    {
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash0BlockBaseAddr,
                                                                     &propertyStore->flashStartAddress[kFlashIndex_Main]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash0TotalSize,
                                                                     &propertyStore->flashSizeInBytes[kFlashIndex_Main]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash0SectorSize,
                                                                     &propertyStore->flashSectorSize[kFlashIndex_Main]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash0BlockSize,
                                                                     &propertyStore->flashBlockSize[kFlashIndex_Main]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash0BlockCount,
                                                                     &propertyStore->flashBlockCount[kFlashIndex_Main]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash0FacSupport,
                                                                     &propertyStore->flashFacSupport[kFlashIndex_Main]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash0AccessSegmentSize,
                                                                     &propertyStore->flashAccessSegmentSize[kFlashIndex_Main]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash0AccessSegmentCount,
                                                                     &propertyStore->flashAccessSegmentCount[kFlashIndex_Main]);

#if defined(BL_HAS_SECONDARY_INTERNAL_FLASH) && BL_HAS_SECONDARY_INTERNAL_FLASH
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash1BlockBaseAddr,
                                                                     &propertyStore->flashStartAddress[kFlashIndex_Secondary]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash1TotalSize,
                                                                     &propertyStore->flashSizeInBytes[kFlashIndex_Secondary]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash1SectorSize,
                                                                     &propertyStore->flashSectorSize[kFlashIndex_Secondary]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash1BlockSize,
                                                                     &propertyStore->flashBlockSize[kFlashIndex_Secondary]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash1BlockCount,
                                                                     &propertyStore->flashBlockCount[kFlashIndex_Secondary]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash1FacSupport,
                                                                     &propertyStore->flashFacSupport[kFlashIndex_Secondary]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash1AccessSegmentSize,
                                                                     &propertyStore->flashAccessSegmentSize[kFlashIndex_Secondary]);
        (void)g_bootloaderContext.flashDriverInterface->flash_get_property(g_bootloaderContext.allFlashState,
                                                                     kFLASH_PropertyPflash1AccessSegmentCount,
                                                                     &propertyStore->flashAccessSegmentCount[kFlashIndex_Secondary]);
#endif
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH
        if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
        {
            (void)g_bootloaderContext.dflashDriverInterface->flash_get_property(g_bootloaderContext.dFlashState,
                                                                          kFLEXNVM_PropertyDflashBlockBaseAddr,
                                                                         &propertyStore->flashStartAddress[kFalshIndex_DFlash]);     
            (void)g_bootloaderContext.dflashDriverInterface->flash_get_property(g_bootloaderContext.dFlashState,
                                                                          kFLEXNVM_PropertyDflashTotalSize,
                                                                         &propertyStore->flashSizeInBytes[kFalshIndex_DFlash]);          
            (void)g_bootloaderContext.dflashDriverInterface->flash_get_property(g_bootloaderContext.dFlashState,
                                                                          kFLEXNVM_PropertyDflashSectorSize,
                                                                         &propertyStore->flashSectorSize[kFalshIndex_DFlash]);      
            (void)g_bootloaderContext.dflashDriverInterface->flash_get_property(g_bootloaderContext.dFlashState,
                                                                          kFLEXNVM_PropertyDflashBlockSize,
                                                                         &propertyStore->flashBlockSize[kFalshIndex_DFlash]);                                                                       
            (void)g_bootloaderContext.dflashDriverInterface->flash_get_property(g_bootloaderContext.dFlashState,
                                                                          kFLEXNVM_PropertyDflashBlockCount,
                                                                         &propertyStore->flashBlockCount[kFalshIndex_DFlash]);  

        }
#endif //  BL_FEATURE_SUPPORT_DFLASH     
    }
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH

    // Fill in reserved regions.
    //! @todo Support other tool chain

    uint32_t flashStart = 0u;
    uint32_t flashEnd = 0u;
    uint32_t ramStart = 0u;
    uint32_t ramEnd = 0u;

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_TARGET_FLASH) && BL_TARGET_FLASH)
    flashStart = (&g_bootloaderContext.memoryMap[kIndexFlashArray])->startAddress;
    flashEnd = (&g_bootloaderContext.memoryMap[kIndexFlashArray])->startAddress;
#else
    flashStart = __ROM_START;
    flashEnd = __ROM_END;
    assert(flashEnd);

    // Force flash erase size alignment.
    // Note: Assume that flash-resident bootloader is always in Main flash.
    flashStart = ALIGN_DOWN(flashStart, propertyStore->flashSectorSize[kFlashIndex_Main]);
    flashEnd = ALIGN_UP(flashEnd, propertyStore->flashSectorSize[kFlashIndex_Main]) - 1u;
#endif
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
    ramStart = __RAM_START;
    ramEnd = __RAM_END;
    assert(ramEnd);

    propertyStore->reservedRegions[(uint32_t)kProperty_FlashReservedRegionIndex].startAddress = flashStart;
    propertyStore->reservedRegions[(uint32_t)kProperty_FlashReservedRegionIndex].endAddress = flashEnd;
    propertyStore->reservedRegions[(uint32_t)kProperty_RamReservedRegionIndex].startAddress = ramStart;
    propertyStore->reservedRegions[(uint32_t)kProperty_RamReservedRegionIndex].endAddress = ramEnd;

    // Fill in available peripherals array.
    const peripheral_descriptor_t *peripherals = g_bootloaderContext.allPeripherals;
    propertyStore->availablePeripherals = 0u;
    for (uint32_t i = 0u; peripherals[i].typeMask != 0u; ++i)
    {
        // Check that the peripheral is enabled in the user configuration data.
        if ((propertyStore->configurationData.enabledPeripherals & peripherals[i].typeMask) != 0u)
        {
            propertyStore->availablePeripherals |= peripherals[i].typeMask;
        }
    }

// Fill in unique device id value.
// Different series have different length of UID (K series=128 bits, KL series=80 bits)
#if (!(defined(FSL_FEATURE_SIM_HAS_NO_UID) && FSL_FEATURE_SIM_HAS_NO_UID)) && defined(SIM)
#if defined(SIM_UIDH) | defined(SIM_UIDH_UID)
#if defined(SIM_UIDM_UID)
    propertyStore->UniqueDeviceId.uid[2] = SIM->UIDH;
#else
    propertyStore->UniqueDeviceId.uid[3] = SIM->UIDH;
#endif
#endif
#if defined(SIM_UIDM_UID)
    propertyStore->UniqueDeviceId.uid[1] = SIM->UIDM;
#else
    propertyStore->UniqueDeviceId.uid[2] = SIM->UIDMH;
    propertyStore->UniqueDeviceId.uid[1] = SIM->UIDML;
#endif // defined(SIM_UIDM)
    propertyStore->UniqueDeviceId.uid[0] = SIM->UIDL;
#endif // #if !FSL_FEATURE_SIM_HAS_NO_UID

    // Set address range of RAM in property interface
    const memory_map_entry_t *map = (const memory_map_entry_t *)&g_bootloaderContext.memoryMap[(uint32_t)kIndexSRAM];
    propertyStore->ramStartAddress[kPropertyIndex_SRAM] = map->startAddress;
    propertyStore->ramSizeInBytes[kPropertyIndex_SRAM] = map->endAddress - map->startAddress + 1u;
#if defined(K28F15_SERIES)
    map = (memory_map_entry_t *)&g_bootloaderContext.memoryMap[kIndexOCRAM];
    propertyStore->ramStartAddress[kPropertyIndex_OCRAM] = map->startAddress;
    propertyStore->ramSizeInBytes[kPropertyIndex_OCRAM] = map->endAddress - map->startAddress + 1u;
#elif defined(KV58F24_SERIES)
    map = (memory_map_entry_t *)&g_bootloaderContext.memoryMap[kIndexDTCM];
    propertyStore->ramStartAddress[kPropertyIndex_DTCM] = map->startAddress;
    propertyStore->ramSizeInBytes[kPropertyIndex_DTCM] = map->endAddress - map->startAddress + 1u;
    map = (memory_map_entry_t *)&g_bootloaderContext.memoryMap[kIndexOCRAM];
    propertyStore->ramStartAddress[kPropertyIndex_OCRAM] = map->startAddress;
    if (map->startAddress == map->endAddress)
    {
        propertyStore->ramSizeInBytes[kPropertyIndex_OCRAM] = 0u;
    }
    else
    {  
        propertyStore->ramSizeInBytes[kPropertyIndex_OCRAM] = map->endAddress - map->startAddress + 1u;
    }
#endif // defined(K28F15_SERIES)

#if defined(BL_FEATURE_CRC_CHECK) && BL_FEATURE_CRC_CHECK
    // Initialize crc check status property based on BCA related fields.
    init_crc_check_status(propertyStore);
#endif

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
    // Fill in default margin level.
    propertyStore->flashReadMargin = (uint32_t)kFTFx_MarginValueUser;
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH

#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
    propertyStore->qspiInitStatus = get_qspi_otfad_init_status();
#endif // BL_FEATURE_QSPI_MODULE

    // Update available peripherals based on specific chips
    update_flash_properties();

    return (int32_t)kStatus_Success;
}

// See property.h for documentation on this function.
status_t bootloader_property_get(uint8_t tag, uint32_t id, const void **value, uint32_t *valueSize)
{
    property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;
    status_t status = (int32_t)kStatus_Success;

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
    uint32_t flashIndex = (uint32_t)kFlashIndex_Main;
#if defined(BL_HAS_SECONDARY_INTERNAL_FLASH) && BL_HAS_SECONDARY_INTERNAL_FLASH
    if (id < kFLASHCount)
    {
        flashIndex = id;
    }
#endif // BL_HAS_SECONDARY_INTERNAL_FLASH
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH
    if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
    {
        flashIndex = id;
    }
#endif // BL_FEATURE_SUPPORT_DFLASH    
#endif // !BL_FEATURE_HAS_NO_INTERNAL_FLASH

    // Set default value size, may be modified below.
    uint32_t returnSize = sizeof(uint32_t);
    const void *returnValue;
    switch (tag)
    {
        case (uint8_t)kPropertyTag_BootloaderVersion:
            returnValue = &propertyStore->bootloaderVersion.version;
            break;

        case (uint8_t)kPropertyTag_AvailablePeripherals:
            returnValue = &propertyStore->availablePeripherals;
            break;

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
        case (uint8_t)kPropertyTag_FlashStartAddress:
            returnValue = &propertyStore->flashStartAddress[flashIndex];
            break;

        case (uint8_t)kPropertyTag_FlashSizeInBytes:
            returnValue = &propertyStore->flashSizeInBytes[flashIndex];
            break;

        case (uint8_t)kPropertyTag_FlashSectorSize:
            returnValue = &propertyStore->flashSectorSize[flashIndex];
            break;
        case (uint8_t)kPropertyTag_FlashBlockCount:
            returnValue = &propertyStore->flashBlockCount[flashIndex];
            break;
#endif // !BL_FEATURE_HAS_NO_INTERNAL_FLASH

        case (uint8_t)kPropertyTag_RAMStartAddress:
#if defined(KV58F24_SERIES) || defined(K28F15_SERIES) || defined(BL_FEATURE_MULTI_SRAM_SECTIONS)
            if (id >= kRAMCount)
            {
                returnValue = &propertyStore->ramStartAddress[0];
            }
            else
            {
                returnValue = &propertyStore->ramStartAddress[id];
            }
#else
            returnValue = &propertyStore->ramStartAddress[0];
#endif // #if defined(KV58F24_SERIES) || defined(K28F15_SERIES) || defined(BL_FEATURE_MULTI_SRAM_SECTIONS)
            break;

        case (uint8_t)kPropertyTag_RAMSizeInBytes:
#if defined(KV58F24_SERIES) || defined(K28F15_SERIES) || defined(BL_FEATURE_MULTI_SRAM_SECTIONS)
            if (id >= kRAMCount)
            {
                returnValue = &propertyStore->ramSizeInBytes[0];
            }
            else
            {
                returnValue = &propertyStore->ramSizeInBytes[id];
            }
#else
            returnValue = &propertyStore->ramSizeInBytes[0];
#endif // #if defined(KV58F24_SERIES) || defined(K28F15_SERIES) || defined(BL_FEATURE_MULTI_SRAM_SECTIONS)
            break;

        case (uint8_t)kPropertyTag_AvailableCommands:
            returnValue = &propertyStore->availableCommands;
            break;

#if defined(BL_FEATURE_CRC_CHECK) && BL_FEATURE_CRC_CHECK
        case (uint8_t)kPropertyTag_CrcCheckStatus:
            returnValue = &propertyStore->crcCheckStatus;
            break;
#endif // else falls through to unknown
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
        case (uint8_t)kPropertyTag_VerifyWrites:
            returnValue = &propertyStore->verifyWrites;
            break;
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
        case (uint8_t)kPropertyTag_MaxPacketSize:
            // Read the max packet size from the active peripheral.
            s_propertyReturnValue = g_bootloaderContext.activePeripheral->packetInterface->getMaxPacketSize(
                g_bootloaderContext.activePeripheral);
            returnValue = &s_propertyReturnValue;
            break;

        case (uint8_t)kPropertyTag_ReservedRegions:
            returnSize = sizeof(propertyStore->reservedRegions);
            returnValue = propertyStore->reservedRegions;
            break;
#if !(defined(FSL_FEATURE_SIM_HAS_NO_SDID) && FSL_FEATURE_SIM_HAS_NO_SDID) && defined(SIM)
        case (uint8_t)kPropertyTag_SystemDeviceId:
            s_propertyReturnValue = SIM->SDID;
            returnValue = &s_propertyReturnValue;
            break;
#endif // #if !FSL_FEATURE_SIM_HAS_NO_SDID

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
        case (uint8_t)kPropertyTag_FlashSecurityState:
        {
            // Note: Both Main and Secondary flash share the same security state
            //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
            ftfx_security_state_t securityState;
            (void)g_bootloaderContext.flashDriverInterface->flash_get_security_state(
                g_bootloaderContext.allFlashState, &securityState);
            s_propertyReturnValue = (securityState != kFTFx_SecurityStateNotSecure)?1u:0u;
            returnValue = &s_propertyReturnValue;
            break;
        }
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH

#if !(defined(FSL_FEATURE_SIM_HAS_NO_UID) && FSL_FEATURE_SIM_HAS_NO_UID)
        case (uint8_t)kPropertyTag_UniqueDeviceId:
            returnSize = sizeof(propertyStore->UniqueDeviceId);
            returnValue = &propertyStore->UniqueDeviceId;
            break;
#endif // #if !FSL_FEATURE_SIM_HAS_NO_UID

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
        case (uint8_t)kPropertyTag_FacSupport:
            returnSize = sizeof(propertyStore->flashFacSupport[flashIndex]);
            returnValue = &propertyStore->flashFacSupport[flashIndex];
            break;

        case (uint8_t)kPropertyTag_FlashAccessSegmentSize:
            returnSize = sizeof(propertyStore->flashAccessSegmentSize[flashIndex]);
            returnValue = &propertyStore->flashAccessSegmentSize[flashIndex];
            break;

        case (uint8_t)kPropertyTag_FlashAccessSegmentCount:
            returnSize = sizeof(propertyStore->flashAccessSegmentCount[flashIndex]);
            returnValue = &propertyStore->flashAccessSegmentCount[flashIndex];
            break;

        case (uint8_t)kPropertyTag_FlashReadMargin:
            // Note: Currently both Main and Secondary flash share the same flash read margin
            returnSize = sizeof(propertyStore->flashReadMargin);
            returnValue = &propertyStore->flashReadMargin;
            break;
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH

#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
        case (uint8_t)kPropertyTag_QspiInitStatus:
            returnValue = &propertyStore->qspiInitStatus;
            break;
#endif // else falls through to unknown

        case (uint8_t)kPropertyTag_TargetVersion:
            returnValue = &propertyStore->targetVersion.version;
            break;
#if defined(BL_FEATURE_EXTERNAL_MEMORY_PROPERTY) && BL_FEATURE_EXTERNAL_MEMORY_PROPERTY
        case (uint8_t)kPropertyTag_ExternalMemoryAttributes:
        {
            status =
                bootloader_get_external_memory_properties(id, &propertyStore->externalMemoryPropertyStore);
            if (status == (int32_t)kStatus_Success)
            {
                returnSize = sizeof(propertyStore->externalMemoryPropertyStore);
                returnValue = &propertyStore->externalMemoryPropertyStore;
            }
        }
            break;
#endif // BL_FEATURE_EXTERNAL_MEMORY_PROPERTY

#if defined(BL_FEATURE_RELIABLE_UPDATE) && BL_FEATURE_RELIABLE_UPDATE
        case (uint8_t)kPropertyTag_ReliableUpdateStatus:
            returnValue = &propertyStore->reliableUpdateStatus;
            break;
#endif // BL_FEATURE_RELIABLE_UPDATE

        default:
            status = (int32_t)kStatus_UnknownProperty;
            break;
    }

    if (status == (int32_t)kStatus_Success)
    {
        // Set the return size.
        if (valueSize != (void *)0u)
        {
            *valueSize = returnSize;
        }

        // Set the return value
        if (value != (void *)0u)
        {
            *value = returnValue;
        }
    }

    return status;
}

// See property.h for documentation on this function.
status_t bootloader_property_set_uint32(uint8_t tag, uint32_t value)
{
    status_t status = (int32_t)kStatus_Success;

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
    property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH

    switch (tag)
    {
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
        case (uint8_t)kPropertyTag_VerifyWrites:
            if (value != 0u && value != 1u)
            {
                status = (int32_t)kStatus_InvalidPropertyValue;
            }
            propertyStore->verifyWrites = value;
            break;

        case (uint8_t)kPropertyTag_FlashReadMargin:
            if (value >= (uint32_t)kFTFx_MarginValueInvalid)
            {
                status = (int32_t)kStatus_InvalidPropertyValue;
            }
            propertyStore->flashReadMargin = value;
            break;
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH

        case (uint8_t)kPropertyTag_BootloaderVersion:
        case (uint8_t)kPropertyTag_AvailablePeripherals:
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
        case (uint8_t)kPropertyTag_FlashStartAddress:
        case (uint8_t)kPropertyTag_FlashSizeInBytes:
        case (uint8_t)kPropertyTag_FlashSectorSize:
        case (uint8_t)kPropertyTag_FlashBlockCount:
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
        case (uint8_t)kPropertyTag_RAMStartAddress:
        case (uint8_t)kPropertyTag_RAMSizeInBytes:
        case (uint8_t)kPropertyTag_AvailableCommands:
#if defined(BL_FEATURE_CRC_CHECK) && BL_FEATURE_CRC_CHECK
        case (uint8_t)kPropertyTag_CrcCheckStatus:
#endif
#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
        case (uint8_t)kPropertyTag_QspiInitStatus:
#endif
#if defined(BL_FEATURE_EXTERNAL_MEMORY_PROPERTY) && BL_FEATURE_EXTERNAL_MEMORY_PROPERTY
        case (uint8_t)kPropertyTag_ExternalMemoryAttributes:
#endif // BL_FEATURE_EXTERNAL_MEMORY_PROPERTY
#if defined(BL_FEATURE_RELIABLE_UPDATE) && BL_FEATURE_RELIABLE_UPDATE
        case (uint8_t)kPropertyTag_ReliableUpdateStatus:
#endif // BL_FEATURE_RELIABLE_UPDATE
        case (uint8_t)kPropertyTag_MaxPacketSize:
        case (uint8_t)kPropertyTag_ReservedRegions:
        case (uint8_t)kPropertyTag_SystemDeviceId:
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
        case (uint8_t)kPropertyTag_FlashSecurityState:
#endif
        case (uint8_t)kPropertyTag_UniqueDeviceId:
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
        case (uint8_t)kPropertyTag_FacSupport:
        case (uint8_t)kPropertyTag_FlashAccessSegmentSize:
        case (uint8_t)kPropertyTag_FlashAccessSegmentCount:
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
        case (uint8_t)kPropertyTag_TargetVersion:
            status = (int32_t) kStatus_ReadOnlyProperty;
            break;
        default:
            status = (int32_t)kStatus_UnknownProperty;
            break;
    }
    return status;
}

#if defined(BL_FEATURE_EXTERNAL_MEMORY_PROPERTY) && BL_FEATURE_EXTERNAL_MEMORY_PROPERTY
status_t bootloader_get_external_memory_properties(uint32_t memoryId, external_memory_property_store_t *store)
{
    status_t status = (int32_t)kStatus_Success;
    extern const external_memory_property_interface_t g_externalMemPropertyInterfaceMap[];

    // Find external memory property interface map.
    const external_memory_property_interface_t *map = &g_externalMemPropertyInterfaceMap[0];
    while (map != (void *)0u && map->get != (void *)0u)
    {
        if (map->memoryId == memoryId)
        {
            break;
        }
        map++;
    }

    if (map->get == (void *)0u)
    {
        status = (int32_t)kStatus_InvalidArgument;
    }
    else
    {
        external_memory_property_store_t propertyStore;
        uint32_t memoryInitStatus;
        map->get(kExternalMemoryPropertyTag_InitStatus, &memoryInitStatus);
        if (memoryInitStatus != (int32_t)kStatus_Success)
        {
            status = (int32_t)memoryInitStatus;
        }
        else
        {
            uint32_t *property = (uint32_t *)&propertyStore.startAddress;
            propertyStore.availableAttributesFlag = 0u;
            for (uint32_t tag = kExternalMemoryPropertyTag_Start; tag <= kExternalMemoryPropertyTag_End; tag++)
            {
                uint32_t tmp = 0u;
                status_t status1 = map->get(tag, &tmp);
                if (status1 == kStatus_Success)
                {
                    *property = tmp;
                    propertyStore.availableAttributesFlag |= 1u << (tag - 1u);
                }
                else
                {
                    *property = 0u;
                }

                property++;
            }

            memcpy(store, &propertyStore, sizeof(propertyStore));
        }
    }
    return status;
}
#endif // BL_FEATURE_EXTERNAL_MEMORY_PROPERTY

////////////////////////////////////////////////////////////////////////////////
// EOF
