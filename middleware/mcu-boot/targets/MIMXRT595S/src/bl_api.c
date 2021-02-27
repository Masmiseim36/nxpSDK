/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "bl_api.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
//! @brief Boot parameters of the user application
//!  WORD    OFFSET      FIELD                            DESCRIPTION
//!          [31:24]     TAG                             Must be '0xEB'
//!          [23:20]     Boot mode                       0:Master boot mode; 1: ISP boot
//!          [19:16]     Boot interface                  0:USART 1:I2C 2:SPI 3:USB HID 4:QSPI 5:USB DFU 6:SD 7:MMC
//!          [15:12]     Boot instance                   0 or 1; This instance is only used when boot interface is SD or
//!          MMC,
//!                                                      for other interfaces it is ignored
//!          [11:08]     Redundant boot image index      Redundant boot image index for FlexSPI NOR flash
//!          [07:00]     Reserved
//!
//!  TAG[31:24]	BOOT MODE[23:20]       INTERFACE[19:16]	  INSTANCE[15:12]	 RBII            Reserved[07:00]	COMBINATION
//!  BOOT ACTION
//!   0xEB	0	                0	                X	          X                X	                0xEB00XXXX	MASTR BOOT:
//!   USART
//!	        0	                1	                X	          X                X	                0xEB01XXXX	MASTR BOOT:
//! I2C
//!	        0	                2	                X	          X                X	                0xEB02XXXX	MASTR BOOT:
//! SPI
//!	        0	                3	                X	          X                X	                0xEB03XXXX	MASTR BOOT:
//! USB HID
//!	        0	                4	                X	          0                X	                0xEB04X0XX	MASTR BOOT:
//! FlexSPI:boot image index 0
//!	        0	                4	                X	          1                X	                0xEB04X1XX	MASTR BOOT:
//! FlexSPI:boot image index 1
//!	        0	                5	                X	          X                X	                0xEB05XXXX	MASTR BOOT:
//! USB DFU
//!	        0	                6	                0	          X                X	                0xEB060XXX	MASTR BOOT:
//! SDINSTANCE 0)
//!	        0	                6	                1	          X                X	                0xEB061XXX	MASTR BOOT:
//! SDINSTANCE 1)
//!	        0	                7	                0	          X                X	                0xEB070XXX	MASTR BOOT:
//! MMC(INSTANCE 0)
//!	        0	                7	                1	          X                X	                0xEB071XXX	MASTR BOOT:
//! MMC(INSTANCE 1)
//!	        1	                0	                X	          X                X	                0xEB10XXXX	ISP BOOT:
//! USART
//!	        1	                1	                X	          X                X	                0xEB11XXXX	ISP BOOT: I2C
//!	        1	                2	                X	          X                X	                0xEB12XXXX	ISP BOOT: SPI
//!

typedef struct _user_app_boot_invoke_option
{
    union
    {
        struct
        {
            uint32_t reserved : 8;
            uint32_t boot_image_index : 4;
            uint32_t instance : 4;
            uint32_t boot_interface : 4;
            uint32_t mode : 4;
            uint32_t tag : 8;
        } B;
        uint32_t U;
    } option;
} user_app_boot_invoke_option_t;

//! @brief Boot interface can be selected by user application
//! @note  For USB-HID QSPI USB-DFU SD MMC, these interfaces are invalid for ISP boot
enum
{
    kUserAppBootPeripheral_UART = 0u,
    kUserAppBootPeripheral_I2C = 1u,
    kUserAppBootPeripheral_SPI = 2u,
    kUserAppBootPeripheral_USB_HID = 3u,
    kUserAppBootPeripheral_QSPI = 4u,
    kUserAppBootPeripheral_DFU = 5u,
    kUserAppBootPeripheral_SD = 6u,
    kUserAppBootPeripheral_MMC = 7u,
};

//! @brief Boot mode can be selected by user application
//! @note  For master boot, valid boot insterfaces for user application are USART I2C SPI USB-HID USB-DFU SD MMC
//!        For ISP boot, valid boot interfaces for user application are USART I2C SPI
enum
{
    kUserAppBootMode_MasterBoot = 0,
    kUserAppBootMode_IspBoot = 1,
};

//!@brief FLEXSPI Flash driver API Interface
typedef struct
{
    uint32_t version;
    status_t (*init)(uint32_t instance, flexspi_nor_config_t *config);
    status_t (*page_program)(uint32_t instance, flexspi_nor_config_t *config, uint32_t dstAddr, const uint32_t *src);
    status_t (*erase_all)(uint32_t instance, flexspi_nor_config_t *config);
    //    status_t (*wait_busy)(uint32_t instance, flexspi_nor_config_t *config, bool isParallelMode, uint32_t address);
    status_t (*erase)(uint32_t instance, flexspi_nor_config_t *config, uint32_t start, uint32_t length);
    status_t (*erase_sector)(uint32_t instance, flexspi_nor_config_t *config, uint32_t address);
    status_t (*erase_block)(uint32_t instance, flexspi_nor_config_t *config, uint32_t address);
    status_t (*get_config)(uint32_t instance, flexspi_nor_config_t *config, serial_nor_config_option_t *option);
    status_t (*read)(uint32_t instance, flexspi_nor_config_t *config, uint32_t *dst, uint32_t start, uint32_t bytes);
    //    status_t (*hw_reset)(uint32_t instance, flexspi_nor_config_t *config);
    status_t (*xfer)(uint32_t instance, flexspi_xfer_t *xfer);
    status_t (*update_lut)(uint32_t instance, uint32_t seqIndex, const uint32_t *lutBase, uint32_t numberOfSeq);
    status_t (*set_clock_source)(uint32_t clockSrc);
    void (*config_clock)(uint32_t instance, uint32_t freqOption, uint32_t sampleClkMode);
} flexspi_nor_flash_driver_t;

//!@brief OTP driver API Interface
typedef struct
{
    status_t (*init)(uint32_t src_clk_freq);
    status_t (*deinit)(void);
    status_t (*fuse_read)(uint32_t addr, uint32_t *data);
    status_t (*fuse_program)(uint32_t addr, uint32_t data, bool lock);
    status_t (*crc_calc)(uint32_t *src, uint32_t numberOfWords, uint32_t *crcChecksum);
    status_t (*reload)(void);
    status_t (*crc_check)(uint32_t start_addr, uint32_t end_addr, uint32_t crc_addr);
} ocotp_driver_t;

//! @brief Root of the bootloader API tree.
//!
//! An instance of this struct resides in read-only memory in the bootloader. It
//! provides a user application access to APIs exported by the bootloader.
//!
//! @note The order of existing fields must not be changed.
//!
//! @ingroup context
typedef struct BootloaderTree
{
    void (*runBootloader)(void *arg); //!< Function to start the bootloader executing.
    uint32_t version;                 //!< Bootloader version number.
    const char *copyright;            //!< Copyright string.
    const uint32_t reserved0;
    const uint32_t reserved1;
    const uint32_t reserved2;
    const uint32_t reserved3;
    const flexspi_nor_flash_driver_t *flexspiNorDriver; //!< FlexSPI NOR FLASH Driver API.
    const ocotp_driver_t *otpDriver;                    //!< OTP driver API.
    const uint32_t reserved4;
} bootloader_tree_t;

#define BOOTLOADER_TREE_LOCATION (0x1302f000)
#define g_bootloaderTree ((bootloader_tree_t *)BOOTLOADER_TREE_LOCATION)

/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * FlexSPI NOR driver
 ******************************************************************************/
uint32_t flexspi_nor_driver_get_version(void)
{
    return g_bootloaderTree->flexspiNorDriver->version;
}

status_t flexspi_nor_flash_init(uint32_t instance, flexspi_nor_config_t *config)
{
    status_t status = g_bootloaderTree->flexspiNorDriver->init(instance, config);

    if (config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_XPI])
    {
        memcpy(&config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE],
               &config->memConfig.lookupTable[4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_XPI], 16);
    }

    return status;
}

status_t flexspi_nor_flash_page_program(uint32_t instance,
                                        flexspi_nor_config_t *config,
                                        uint32_t dstAddr,
                                        const uint32_t *src)
{
    return g_bootloaderTree->flexspiNorDriver->page_program(instance, config, dstAddr, src);
}

status_t flexspi_nor_flash_erase_all(uint32_t instance, flexspi_nor_config_t *config)
{
    return g_bootloaderTree->flexspiNorDriver->erase_all(instance, config);
}

status_t flexspi_nor_get_config(uint32_t instance, flexspi_nor_config_t *config, serial_nor_config_option_t *option)
{
    if (config->memConfig.readSampleClkSrc == kFlexSPIReadSampleClk_LoopbackInternally)
    {
        if (option->option0.B.misc_mode != kSerialNorEnhanceMode_InternalLoopback)
        {
            config->memConfig.readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackFromDqsPad;
        }
    }
    return g_bootloaderTree->flexspiNorDriver->get_config(instance, config, option);
}

status_t flexspi_nor_flash_erase(uint32_t instance, flexspi_nor_config_t *config, uint32_t start, uint32_t length)
{
    return g_bootloaderTree->flexspiNorDriver->erase(instance, config, start, length);
}

status_t flexspi_nor_flash_read(
    uint32_t instance, flexspi_nor_config_t *config, uint32_t *dst, uint32_t start, uint32_t bytes)
{
    return g_bootloaderTree->flexspiNorDriver->read(instance, config, dst, start, bytes);
}

status_t flexspi_update_lut(uint32_t instance, uint32_t seqIndex, const uint32_t *lutBase, uint32_t numberOfSeq)
{
    return g_bootloaderTree->flexspiNorDriver->update_lut(instance, seqIndex, lutBase, numberOfSeq);
}

status_t flexspi_command_xfer(uint32_t instance, flexspi_xfer_t *xfer)
{
    return g_bootloaderTree->flexspiNorDriver->xfer(instance, xfer);
}

status_t flexspi_nor_set_clock_source(uint32_t clockSrc)
{
    return g_bootloaderTree->flexspiNorDriver->set_clock_source(clockSrc);
}

void flexspi_nor_flash_config_clock(uint32_t instance, uint32_t freqOption, uint32_t sampleClkMode)
{
    g_bootloaderTree->flexspiNorDriver->config_clock(instance, freqOption, sampleClkMode);
}

status_t flexspi_nor_auto_config(uint32_t instance, flexspi_nor_config_t *config, serial_nor_config_option_t *option)
{
    /*
    // Wait until the FLEXSPI is idle
    register uint32_t delaycnt = 10000u;
    while(delaycnt--)
    {
    }
    status_t status = g_bootloaderTree->flexspiNorDriver->get_config(instance, config, option);
    if (status != kStatus_Success)
    {
        return status;
    }

    return g_bootloaderTree->flexspiNorDriver->init(instance, config);
    */

    // Note: Below are the Position-independent codes for above the flow.
    static uint8_t s_ramfuncArray[] = { 0xF8, 0xB5, 0x0D, 0x46, 0x04, 0x46, 0x42, 0xF2, 0x10, 0x71, 0x08,
                                        0x46, 0x41, 0x1E, 0x00, 0x28, 0xFB, 0xD1, 0x4F, 0xF6, 0x1C, 0x76,
                                        0xC1, 0xF2, 0x01, 0x36, 0x33, 0x68, 0x29, 0x46, 0x20, 0x46, 0xDF,
                                        0x69, 0xB8, 0x47, 0x30, 0xB9, 0x32, 0x68, 0x29, 0x46, 0x20, 0x46,
                                        0x53, 0x68, 0xBD, 0xE8, 0xF4, 0x40, 0x18, 0x47, 0xF2, 0xBD };

    uint32_t alignedBuffer[(sizeof(s_ramfuncArray) + 3) / sizeof(uint32_t)];
    memcpy(alignedBuffer, s_ramfuncArray, sizeof(s_ramfuncArray));
    static status_t (*autoConfigCallback)(uint32_t, flexspi_nor_config_t *, serial_nor_config_option_t *);
    autoConfigCallback =
        (status_t(*)(uint32_t, flexspi_nor_config_t *, serial_nor_config_option_t *))((uint32_t)&alignedBuffer + 1);

    return autoConfigCallback(instance, config, option);
}

/*******************************************************************************
 * OTP API
 ******************************************************************************/
status_t otp_init(uint32_t src_clk_freq)
{
    return g_bootloaderTree->otpDriver->init(src_clk_freq);
}
status_t otp_deinit(void)
{
    return g_bootloaderTree->otpDriver->deinit();
}

status_t otp_fuse_read(uint32_t addr, uint32_t *data)
{
    return g_bootloaderTree->otpDriver->fuse_read(addr, data);
}

status_t otp_fuse_program(uint32_t addr, uint32_t data, bool lock)
{
    return g_bootloaderTree->otpDriver->fuse_program(addr, data, lock);
}

status_t otp_shadow_register_reload(void)
{
    return g_bootloaderTree->otpDriver->reload();
}

status_t otp_crc_check(uint32_t start_addr, uint32_t end_addr, uint32_t crc_addr)
{
    return g_bootloaderTree->otpDriver->crc_check(start_addr, end_addr, crc_addr);
}

status_t otp_crc_calc(uint32_t *src, uint32_t numberOfWords, uint32_t *crcChecksum)
{
    return g_bootloaderTree->otpDriver->crc_calc(src, numberOfWords, crcChecksum);
}

void flexspi_clear_cache(uint32_t instance) {}

/*******************************************************************************
 * ReInvoke ROM API
 ******************************************************************************/
void bootloader_user_entry(void *arg)
{
    g_bootloaderTree->runBootloader(arg);
}
