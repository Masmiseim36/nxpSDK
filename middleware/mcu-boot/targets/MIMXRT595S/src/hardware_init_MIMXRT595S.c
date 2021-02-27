/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "bl_context.h"
#if BL_FEATURE_RELIABLE_UPDATE
#include "bl_reliable_update.h"
#endif
#include "bootloader_common.h"
#include "flexspi_nor_flash.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#if BL_ENABLE_CRC_CHECK
#include "bl_app_crc_check.h"
#endif

#include "bl_api.h"

#ifdef DEBUG_UART
#include "fsl_usart.h"
#endif

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef CACHE64
#define CACHE64 CACHE64_CTRL0
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//!@brief Perfrom Cache disable operation and return the CCR register value
uint32_t cache_disable(void);

//!@brief Restore the Cache state by the argument ccr
void cache_restore(uint32_t ccr);

status_t flash_read(uint32_t address, uint8_t *buffer, size_t length);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(BOOT_HEADER_ENABLE) && (BOOT_HEADER_ENABLE == 1)
#if defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".flash_conf")))
#elif defined(__ICCARM__)
#pragma location = ".flash_conf"
#endif

__USED const flexspi_nor_config_t flexspi_config = {
    .memConfig =
        {
            .tag                 = FLEXSPI_CFG_BLK_TAG,
            .version             = FLEXSPI_CFG_BLK_VERSION,
            .csHoldTime          = 3,
            .csSetupTime         = 3,
            .deviceModeCfgEnable = 1,
            .deviceModeType      = kDeviceConfigCmdType_Spi2Xpi,
            .waitTimeCfgCommands = 1,
            .deviceModeSeq =
                {
                    .seqNum   = 1,
                    .seqId    = 6, /* See Lookup table for more details */
                    .reserved = 0,
                },
            .deviceModeArg   = 2, // Enable OPI DDR mode
            .dataValidTime =
                {
                        [0] = {.time_100ps = 16 },
                },
            .busyOffset = 0u,
            .busyBitPolarity = 0u,
            .controllerMiscOption =  (1UL << kFlexSpiMiscOffset_SafeConfigFreqEnable) | (1UL << kFlexSpiMiscOffset_DdrModeEnable),
            .deviceType    = kFlexSpiDeviceType_SerialNOR,
            .readSampleClkSrc = kFlexSPIReadSampleClk_ExternalInputFromDqsPad,
            .sflashPadType = kSerialFlash_8Pads,
            .serialClkFreq = kFlexSpiSerialClk_166MHz,
            .sflashA1Size  = BL_FEATURE_FLASH_SIZE,
            .lookupTable =
                {
                    /* Read */
                    [0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0xEE, CMD_DDR, FLEXSPI_8PAD, 0x11),
                    [1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, DUMMY_DDR, FLEXSPI_8PAD, 0x14),
                    [2] = FLEXSPI_LUT_SEQ(READ_DDR, FLEXSPI_8PAD, 0x04, STOP, FLEXSPI_1PAD, 0x00),

                    /* Read Status */
                    [4 * 1 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05, READ_SDR, FLEXSPI_1PAD, 0x04),

                    /* Read Status XPI */
                    [4 * 2 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x05, CMD_DDR, FLEXSPI_8PAD, 0xFA),
                    [4 * 2 + 1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, DUMMY_DDR, FLEXSPI_8PAD, 0x14),
                    [4 * 2 + 2] = FLEXSPI_LUT_SEQ(READ_DDR, FLEXSPI_8PAD, 0x04, STOP, FLEXSPI_1PAD, 0x00),

                    /* Write Enable */
                    [4 * 3 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP, FLEXSPI_1PAD, 0x00),


                    /* Configure Register */
                    [4 * 6 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x72, CMD_SDR, FLEXSPI_1PAD, 0x00),
                    [4 * 6 + 1] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, CMD_SDR, FLEXSPI_1PAD, 0x00),
                    [4 * 6 + 2] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x00, WRITE_SDR, FLEXSPI_1PAD, 0x01),

                    /* Write Enable XPI */
                    [4 * 4 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x06, CMD_DDR, FLEXSPI_8PAD, 0xF9),

                    /* Sector Erase */
                    [4 * 5 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x21, CMD_DDR, FLEXSPI_8PAD, 0xDE),
                    [4 * 5 + 1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, STOP, FLEXSPI_1PAD, 0x00),

                    /* Block Erase */
                    [4 * 8 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0xDC, CMD_DDR, FLEXSPI_8PAD, 0x23),
                    [4 * 8 + 1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, STOP, FLEXSPI_1PAD, 0x00),

                    /* Page Program */
                    [4 * 9 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x12, CMD_DDR, FLEXSPI_8PAD, 0xED),
                    [4 * 9 + 1] = FLEXSPI_LUT_SEQ(RADDR_DDR, FLEXSPI_8PAD, 0x20, WRITE_DDR, FLEXSPI_8PAD, 0x04),

                    /* Chip Erase */
                    [4 * 11 + 0] = FLEXSPI_LUT_SEQ(CMD_DDR, FLEXSPI_8PAD, 0x60, CMD_DDR, FLEXSPI_8PAD, 0x9F),
                },
        },
    .pageSize           = 0x100,
    .sectorSize         = 0x1000,
    .ipcmdSerialClkFreq = 0,
    .blockSize          = 0x10000,
    .flashStateCtx      = 0x07008200u,
    .serialNorType      = 2U,
};
#endif /* BOOT_HEADER_ENABLE */

/*******************************************************************************
 * Codes
 ******************************************************************************/
//__ramfunc void switch_flash_to_spi_mode(void);

peripheral_descriptor_t const g_peripherals[1];
command_interface_t g_commandInterface;

void update_available_peripherals(void) {}

void sw_delay_us(uint32_t us)
{
    uint32_t coreClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    uint32_t ticksPerUs = coreClock / 1000000u / 4 + 1;

    while (us--)
    {
        register uint32_t ticks = ticksPerUs;
        while (ticks--)
        {
            __NOP();
        }
    }
}

void init_hardware_api(void) {}

uint32_t cache_disable(void)
{
    uint32_t ccr = CACHE64->CCR;

    if ((CACHE64->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) != 0U)
    {
        // Disable FLEXSPI Cache
        /* Enable the to push all modified lines. */
        CACHE64->CCR |= CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK | CACHE64_CTRL_CCR_GO_MASK;
        __ISB();
        __DSB();
        /* Wait until the cache command completes. */
        while ((CACHE64->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
        {
        }

        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        CACHE64->CCR &= ~(CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK);
        CACHE64->CCR &= ~CACHE64_CTRL_CCR_ENCACHE_MASK;
        __DSB();
        __ISB();
    }
    return ccr;
}
void cache_restore(uint32_t ccr)
{
    CACHE64->CCR = ccr;
    __ISB();
    __DSB();
    /* Wait until the cache command completes. */
    while ((CACHE64->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
    {
    }
}

void init_hardware(void)
{
    cache_disable();

#if defined(DEBUG_UART)
    // Select UART0, I2C2 Clock source
    CLKCTL1->FLEXCOMM[0].FCFCLKSEL = 0x00u;
    CLOCK_EnableClock(kCLOCK_Flexcomm0);
    /* reset FLEXCOMM0 for USART0 */
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
    IOPCTL->PIO[0][1] = IOPCTL_PIO_FSEL(1) | IOPCTL_PIO_IBENA(1);
    IOPCTL->PIO[0][2] = IOPCTL_PIO_FSEL(1) | IOPCTL_PIO_IBENA(1);

    usart_config_t userConfig;
    USART_GetDefaultConfig(&userConfig);
    userConfig.baudRate_Bps = 115200;
    USART_Init(PRINT_UART, &userConfig, CLOCK_GetFreq(kCLOCK_Flexcomm0Clk));
#endif
}

#if defined(DEBUG_UART)
void debug_uart_print(const uint8_t *buffer, uint32_t lengthInBytes)
{
    USART_WriteBlocking(PRINT_UART, buffer, lengthInBytes);
}
#endif

void deinit_hardware(void) {}

void sram_init(void) {}

// See bootloader_common.h for documentation on this function
bool is_flexspi_nor_present(void)
{
#if BL_FEATURE_FLEXSPI_NOR_MODULE
    return is_flexspi_nor_configured();
#else
    return false;
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE
}

void flexspi_iomux_config(uint32_t instance, flexspi_mem_config_t *config) {}

#if BL_FEATURE_RELIABLE_UPDATE
typedef struct
{
    status_t (*erase)(uint32_t start, uint32_t lengthInbytes);
    status_t (*program)(uint32_t start, uint8_t *src, uint32_t length);
} flash_driver_interface_t;

typedef struct
{
    uint32_t version;
    status_t (*get_update_partition_info)(partition_t *partition);
    status_t (*update_image_state)(uint32_t state);
    status_t (*get_image_state)(uint32_t *state);
    status_t (*update_image_state_user_api)(uint32_t state, flash_driver_interface_t *flashIf);
    status_t (*update_partition_table_user_api)(partition_t *partition,
                                                uint32_t entries,
                                                flash_driver_interface_t *flashIf);
    uint32_t magic_num;
    status_t (*boot_image_check)(image_header_t *hdr, int32_t partition_id);
} reliable_update_interface_t;

status_t api_update_swap_meta(swap_meta_t *swap_meta)
{
    status_t status = kStatus_Fail;
    swap_meta_t swap_metas[2];
    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t meta_addr = BL_FEATURE_SWAP_META_START + i * BL_FEATURE_FLASH_SECTOR_SIZE;
        (void)memcpy((uint8_t *)&swap_metas[i], (void *)meta_addr, sizeof(swap_meta_t));
    }

    uint32_t update_idx = 0;
    if ((kStatus_Success != swap_meta_check(&swap_metas[0])) && (kStatus_Success != swap_meta_check(&swap_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success != swap_meta_check(&swap_metas[1])))
    {
        update_idx = 1;
    }
    else if ((kStatus_Success != swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success == swap_meta_check(&swap_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success == swap_meta_check(&swap_metas[1])))
    {
        update_idx = (swap_metas[0].meta_version > swap_metas[1].meta_version) ? 1 : 0;
    }

    uint32_t meta_addr = BL_FEATURE_SWAP_META_START + update_idx * BL_FEATURE_FLASH_SECTOR_SIZE;

    swap_meta->meta_version++;

    // Copy the FLASH configuration data from FLASH to RAM
    flexspi_nor_config_t flashNorConfig;
    (void)memcpy(&flashNorConfig, (void *)(BL_FLEXSPI_AMBA_BASE + BL_FLEXSPI_NOR_CFG_BLOCK_OFFSET),
                 sizeof(flashNorConfig));
    if (flashNorConfig.memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_XPI])
    {
        (void)memcpy(&flashNorConfig.memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE],
                     &flashNorConfig.memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_XPI], 16);
    }
    flashNorConfig.memConfig.serialClkFreq = 0;
    flashNorConfig.ipcmdSerialClkFreq = 0;

    uint32_t instance = BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE;
    uint32_t priMask = DisableGlobalIRQ();
    EnableGlobalIRQ(priMask);
    uint32_t programBuffer[128];
    (void)memset(programBuffer, 0xff, sizeof(programBuffer));

    do
    {
        uint32_t erase_addr = meta_addr - BL_FLEXSPI_AMBA_BASE;
        uint32_t priMask = DisableGlobalIRQ();
        status = flexspi_nor_flash_erase(instance, &flashNorConfig, erase_addr, BL_FEATURE_FLASH_SECTOR_SIZE);
        EnableGlobalIRQ(priMask);
        if (status != kStatus_Success)
        {
            break;
        }

        (void)memcpy(programBuffer, swap_meta, sizeof(*swap_meta));
        priMask = DisableGlobalIRQ();
        status = flexspi_nor_flash_page_program(instance, &flashNorConfig, erase_addr, programBuffer);
        EnableGlobalIRQ(priMask);
        if (status != kStatus_Success)
        {
            break;
        }

    } while (0);

    return status;
}

status_t app_api_update_swap_meta(swap_meta_t *swap_meta, flash_driver_interface_t *flashIf)
{
    status_t status = kStatus_Fail;
    swap_meta_t swap_metas[2];
    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t meta_addr = BL_FEATURE_SWAP_META_START + i * BL_FEATURE_FLASH_SECTOR_SIZE;
        flash_read(meta_addr, (uint8_t *)&swap_metas[i], sizeof(swap_metas[i]));
    }

    uint32_t update_idx = 0;
    if ((kStatus_Success != swap_meta_check(&swap_metas[0])) && (kStatus_Success != swap_meta_check(&swap_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success != swap_meta_check(&swap_metas[1])))
    {
        update_idx = 1;
    }
    else if ((kStatus_Success != swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success == swap_meta_check(&swap_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == swap_meta_check(&swap_metas[0])) &&
             (kStatus_Success == swap_meta_check(&swap_metas[1])))
    {
        update_idx = (swap_metas[0].meta_version > swap_metas[1].meta_version) ? 1 : 0;
    }

    uint32_t meta_addr = BL_FEATURE_SWAP_META_START + update_idx * BL_FEATURE_FLASH_SECTOR_SIZE;

    swap_meta->meta_version++;
    do
    {
        uint32_t erase_addr = meta_addr - BL_FLEXSPI_AMBA_BASE;
        // Ensure that the program operation cannots be interrupted.

        uint32_t priMask = DisableGlobalIRQ();
        status = flashIf->erase(erase_addr, BL_FEATURE_FLASH_SECTOR_SIZE);
        EnableGlobalIRQ(priMask);
        if (status != kStatus_Success)
        {
            break;
        }

        priMask = DisableGlobalIRQ();
        status = flashIf->program(erase_addr, (uint8_t *)swap_meta, sizeof(*swap_meta));
        EnableGlobalIRQ(priMask);
        if (status != kStatus_Success)
        {
            break;
        }

    } while (0);

    return status;
}

status_t app_api_update_boot_meta(bootloader_meta_t *boot_meta, flash_driver_interface_t *flashIf)
{
    status_t status = kStatus_Fail;

    bootloader_meta_t boot_metas[2];
    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t meta_addr = BL_FEATURE_BOOT_META_START + i * BL_FEATURE_FLASH_SECTOR_SIZE;
        flash_read(meta_addr, (uint8_t *)&boot_metas[i], sizeof(boot_metas[i]));
    }

    uint32_t update_idx = 0;
    if ((kStatus_Success != boot_meta_check(&boot_metas[0])) && (kStatus_Success != boot_meta_check(&boot_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == boot_meta_check(&boot_metas[0])) &&
             (kStatus_Success != boot_meta_check(&boot_metas[1])))
    {
        update_idx = 1;
    }
    else if ((kStatus_Success != boot_meta_check(&boot_metas[0])) &&
             (kStatus_Success == boot_meta_check(&boot_metas[1])))
    {
        update_idx = 0;
    }
    else if ((kStatus_Success == boot_meta_check(&boot_metas[0])) &&
             (kStatus_Success == boot_meta_check(&boot_metas[1])))
    {
        update_idx = (boot_metas[0].meta_version > boot_metas[1].meta_version) ? 1 : 0;
    }

    uint32_t meta_addr = BL_FEATURE_BOOT_META_START + update_idx * BL_FEATURE_FLASH_SECTOR_SIZE;

    boot_metas->meta_version++;
    do
    {
        uint32_t erase_addr = meta_addr - BL_FLEXSPI_AMBA_BASE;
        // Ensure that the program operation cannots be interrupted
        uint32_t priMask = DisableGlobalIRQ();
        status = flashIf->erase(erase_addr, BL_FEATURE_FLASH_SECTOR_SIZE);
        EnableGlobalIRQ(priMask);
        if (status != kStatus_Success)
        {
            break;
        }

        priMask = DisableGlobalIRQ();
        status = flashIf->program(erase_addr, (uint8_t *)boot_meta, sizeof(*boot_meta));
        EnableGlobalIRQ(priMask);
        if (status != kStatus_Success)
        {
            break;
        }

    } while (0);

    return status;
}

status_t get_update_partition_info(partition_t *partition)
{
    bootloader_meta_t boot_meta;
    status_t status = load_boot_meta(&boot_meta);
    if (status != kStatus_Success)
    {
        return status;
    }
    (void)memcpy(partition, &boot_meta.partition[kPartition_Secondary], sizeof(*partition));

    return kStatus_Success;
}

status_t update_partition_table_user_api(partition_t *partition, uint32_t entries, flash_driver_interface_t *flashIf)
{
    bootloader_meta_t boot_meta;
    bool has_boot_meta = false;

    if ((flashIf == NULL) || (entries > ARRAY_SIZE(boot_meta.partition)))
    {
        return kStatus_InvalidArgument;
    }

    status_t status = load_boot_meta(&boot_meta);
    if (status == kStatus_Success)
    {
        has_boot_meta = true;
    }

    if (!has_boot_meta)
    {
        (void)memset(&boot_meta, 0, sizeof(boot_meta));
        boot_meta.tag = BOOTLOADER_META_TAG;
        boot_meta.features.enabledPeripherals = 0xffffffffu;
        boot_meta.features.periphDetectTimeout = 0xffffffffu;
        boot_meta.features.wdTimeout = 0xffffffffu;
    }

    boot_meta.patition_entries = entries;
    (void)memcpy(&boot_meta.partition, partition, entries * sizeof(*partition));
    status = app_api_update_boot_meta(&boot_meta, flashIf);

    return status;
}

status_t update_image_state(uint32_t state)
{
    swap_meta_t swap_meta;

    status_t status;

    do
    {
        status = load_swap_meta(&swap_meta);
        if (status != kStatus_Success)
        {
            break;
        }

        if (state == kSwapType_ReadyForTest)
        {
            image_header_t boot_header;

            get_image_header(kPartition_Secondary, &boot_header);
            if (boot_image_check(&boot_header, kPartition_Secondary) == kStatus_Success)
            {
                swap_meta.swap_type = kSwapType_ReadyForTest;
                swap_meta.copy_status = 0;
                swap_meta.swap_progress.swap_offset = 0;
                swap_meta.swap_progress.swap_status = kSwapStage_NotStarted;
                swap_meta.image_info[1].size = boot_header.image_size + boot_header.header_size;
                swap_meta.confirm_info = 0;
                api_update_swap_meta(&swap_meta);
                status = kStatus_Success;
            }
            else
            {
                status = kStatus_Fail;
            }
        }
        else if (state == kSwapType_Permanent)
        {
            swap_meta.copy_status = 0;
            swap_meta.swap_progress.swap_offset = 0;
            swap_meta.swap_progress.swap_status = kSwapStage_NotStarted;
            swap_meta.confirm_info = kImageConfirm_Okay;
            swap_meta.swap_type = kSwapType_Permanent;
            api_update_swap_meta(&swap_meta);
            status = kStatus_Success;
        }
        else if (state == kSwapType_Rollback)
        {
            swap_meta.copy_status = kCopyStatus_Done;
            swap_meta.swap_type = kSwapType_Test;
            swap_meta.confirm_info = 0;
            swap_meta.swap_progress.swap_offset = 0;
            swap_meta.swap_progress.swap_status = kSwapStage_Done;
            status = api_update_swap_meta(&swap_meta);
            status = status;
        }
        else
        {
            status = kStatus_InvalidArgument;
        }
    } while (0);

    return status;
}

status_t update_image_state_user_api(uint32_t state, flash_driver_interface_t *flashIf)
{
    swap_meta_t swap_meta;

    status_t status;
    do
    {
        status = load_swap_meta(&swap_meta);
        if (status != kStatus_Success)
        {
            break;
        }

        if (state == kSwapType_ReadyForTest)
        {
            image_header_t boot_header;
            get_image_header(kPartition_Secondary, &boot_header);
            if (boot_image_check(&boot_header, kPartition_Secondary) == kStatus_Success)
            {
                swap_meta.swap_type = kSwapType_ReadyForTest;
                swap_meta.copy_status = 0;
                swap_meta.swap_progress.swap_offset = 0;
                swap_meta.swap_progress.swap_status = kSwapStage_NotStarted;
                swap_meta.image_info[1].size = boot_header.image_size + boot_header.header_size;
                swap_meta.confirm_info = 0;
                status = app_api_update_swap_meta(&swap_meta, flashIf);
            }
            else
            {
                status = kStatus_Fail;
            }
        }
        else if (state == kSwapType_Permanent)
        {
            swap_meta.copy_status = 0;
            swap_meta.swap_progress.swap_offset = 0;
            swap_meta.swap_progress.swap_status = kSwapStage_NotStarted;
            swap_meta.confirm_info = kImageConfirm_Okay;
            swap_meta.swap_type = kSwapType_Permanent;
            status = app_api_update_swap_meta(&swap_meta, flashIf);
        }
        else if (state == kSwapType_Rollback)
        {
            swap_meta.copy_status = kCopyStatus_Done;
            swap_meta.swap_type = kSwapType_Test;
            swap_meta.confirm_info = 0;
            swap_meta.swap_progress.swap_offset = 0;
            swap_meta.swap_progress.swap_status = kSwapStage_Done;
            status = app_api_update_swap_meta(&swap_meta, flashIf);
        }
        else
        {
            status = kStatus_InvalidArgument;
        }
    } while (0);

    return status;
}

status_t get_image_state(uint32_t *state)
{
    swap_meta_t swap_meta;
    status_t status = load_swap_meta(&swap_meta);
    if (status == kStatus_Success)
    {
        *state = swap_meta.swap_type;
    }

    return status;
}

__USED const reliable_update_interface_t g_reliableUpdateAPI = {
    .version = MAKE_VERSION(1, 1, 0),
    .magic_num = 0x900dbeef,
    .get_update_partition_info = get_update_partition_info,
    .update_image_state = update_image_state,
    .get_image_state = get_image_state,
    .update_image_state_user_api = update_image_state_user_api,
    .update_partition_table_user_api = update_partition_table_user_api,
    .boot_image_check = boot_image_check,

};
#else
__USED const uint32_t g_reliableUpdateAPI = 0;
#endif //#if BL_FEATURE_RELIABLE_UPDATE

//! Note: Here the API reads the data by the IP command in case the cache is enabled in user application, the AHB read
//! may return incorrect data
status_t flash_read(uint32_t address, uint8_t *buffer, size_t length)
{
    flexspi_nor_config_t flashNorConfig;
    uint32_t flashCfgAddr = BL_FLEXSPI_NOR_CFG_BLOCK_OFFSET + BL_FLEXSPI_AMBA_BASE;
    (void)memcpy(&flashNorConfig, (void *)flashCfgAddr, sizeof(flashNorConfig));
    uint32_t flashAddr = address - BL_FLEXSPI_AMBA_BASE;
    uint32_t instance = BL_FEATURE_FLEXSPI_NOR_MODULE_PERIPHERAL_INSTANCE;
    uint32_t currentAddr = flashAddr;
    uint32_t maxCopySizeOnePass = 32U * 1024UL;
    status_t status = kStatus_Fail;
    while (length > 0)
    {
        uint32_t copySize = (length > maxCopySizeOnePass) ? maxCopySizeOnePass : length;

        status = flexspi_nor_flash_read(instance, &flashNorConfig, (uint32_t *)buffer, currentAddr, copySize);
        if (status != kStatus_Success)
        {
            break;
        }
        length -= copySize;
        buffer += copySize;
        currentAddr += copySize;
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
