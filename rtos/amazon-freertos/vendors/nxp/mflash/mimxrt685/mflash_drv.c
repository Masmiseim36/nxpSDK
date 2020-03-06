/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "mflash_drv.h"
#include <stdbool.h>

#define FLASH_PORT kFLEXSPI_PortB1

#define NOR_CMD_LUT_SEQ_IDX_READ 0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS 1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE 2
#define NOR_CMD_LUT_SEQ_IDX_READID_OPI 3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI 4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR 5
#define NOR_CMD_LUT_SEQ_IDX_CHIPERASE 6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM 7
#define NOR_CMD_LUT_SEQ_IDX_ENTEROPI 8
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI 9

#define CUSTOM_LUT_LENGTH 60
#define FLASH_BUSY_STATUS_POL 1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_ERROR_STATUS_MASK 0x0e
#define FLASH_ENABLE_OCTAL_CMD 0x02
#define CACHE_MAINTAIN 1

//static uint32_t g_clk_div = 0;
static uint8_t g_tmp_data[MFLASH_PAGE_SIZE];

static status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base, bool enableOctal)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    if (enableOctal)
    {
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI;
    }
    else
    {
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
    }

    flashXfer.data     = &readValue;
    flashXfer.dataSize = 1;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
        if (FLASH_BUSY_STATUS_POL)
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }

    } while (isBusy);

    return status;
}

static status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr, bool enableOctal)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write enable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    if (enableOctal)
    {
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI;
    }
    else
    {
        flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;
    }

    status = FLEXSPI_TransferBlocking(MFLASH_FLEXSPI, &flashXfer);

    return status;
}

/* Internal - erase single sector */
static void mflash_drv_sector_erase(uint32_t sector_addr)
{
    __asm("cpsid i");

    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write neable */
    status = flexspi_nor_write_enable(MFLASH_FLEXSPI, 0, true);

    if (status != kStatus_Success)
    {
        return;// status;
    }

    flashXfer.deviceAddress = sector_addr;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
    status                  = FLEXSPI_TransferBlocking(MFLASH_FLEXSPI, &flashXfer);

    if (status != kStatus_Success)
    {
        return ;//status;
    }

    status = flexspi_nor_wait_bus_busy(MFLASH_FLEXSPI, true);

    /* Do software reset. */
//    FLEXSPI_SoftwareReset(base); //TODO: check

    DCACHE_InvalidateByRange(FlexSPI_AMBA_BASE + sector_addr, MFLASH_SECTOR_SIZE);

    __asm("cpsie i");
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ISB();
}

/* Program page into serial flash using QSPI polling way */
static int program_page(uint32_t dest_addr, uint32_t *src_addr)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    __asm("cpsid i");

    /* Write neable */
    status = flexspi_nor_write_enable(MFLASH_FLEXSPI, dest_addr, true);
    
    if (status != kStatus_Success)
    {
        return -1;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = dest_addr;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
    flashXfer.data          = (uint32_t *)src_addr;
    flashXfer.dataSize      = MFLASH_PAGE_SIZE;
    status                  = FLEXSPI_TransferBlocking(MFLASH_FLEXSPI, &flashXfer);

    if (status != kStatus_Success)
    {
        return -1;
    }

    status = flexspi_nor_wait_bus_busy(MFLASH_FLEXSPI, true);

    DCACHE_InvalidateByRange(FlexSPI_AMBA_BASE + dest_addr, MFLASH_PAGE_SIZE);

    __asm("cpsie i");
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ISB();
    
    return 0;
}

static status_t flexspi_nor_enable_octal_mode(FLEXSPI_Type *base)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    uint32_t writeValue = FLASH_ENABLE_OCTAL_CMD;

    /* Write enable */
    status = flexspi_nor_write_enable(base, 0, false);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Enable quad mode. */
    flashXfer.deviceAddress = 0;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ENTEROPI;
    flashXfer.data          = &writeValue;
    flashXfer.dataSize      = 1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base, true);

    return status;
}

#ifndef XIP_EXTERNAL_FLASH
static flexspi_device_config_t deviceconfig = {
    .flexspiRootClk       = 99000000,
    .flashSize            = FLASH_SIZE,
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 2,
    .columnspace          = 0,
    .enableWordAddress    = 0,
    .AWRSeqIndex          = 0,
    .AWRSeqNumber         = 0,
    .ARDSeqIndex          = NOR_CMD_LUT_SEQ_IDX_READ,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};
#endif

static const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {

    /*  OPI DDR read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xEE, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x11),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x29),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x0),

    /* Read status register */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

    /* Write Enable */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Read ID */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x9F, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x60),
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x16),
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x0),

    /*  Write Enable */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x06, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xF9),

    /*  Erase Sector */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x21, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xDE),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),

    /*  Erase Chip */
    [4 * NOR_CMD_LUT_SEQ_IDX_CHIPERASE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x60, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x9F),

    /*  Program */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x12, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xED),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04),

    /* Enter OPI mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x72, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /*  Read status register using Octal DDR read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xFA),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x0),
};

static int32_t mflash_drv_init_internal(void)
{
    uint64_t delay = 0;

    __asm("cpsid i");

    delay = USEC_TO_COUNT(20, SystemCoreClock);

#ifndef XIP_EXTERNAL_FLASH
    flexspi_config_t config;
    uint32_t localLUT[CUSTOM_LUT_LENGTH];

    memcpy(localLUT, customLUT, sizeof(customLUT));

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch   = true;
    config.rxSampleClock                 = kFLEXSPI_ReadSampleClkLoopbackInternally;
    config.enableCombination             = true;
    config.ahbConfig.enableAHBBufferable = true;
    config.ahbConfig.enableAHBCachable   = true;
    FLEXSPI_Init(MFLASH_FLEXSPI, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(MFLASH_FLEXSPI, &deviceconfig, FLASH_PORT);
#endif

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(MFLASH_FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    //FLEXSPI_SoftwareReset(MFLASH_FLEXSPI);

    /* Reset configuration */
#if 1
    do
    {
        GPIO->B[2][12] = 0;
        for (uint64_t i = 0; i < delay; i++)
        {
            __asm("nop");
        }
        GPIO->B[2][12] = 1;
        flexspi_nor_enable_octal_mode(MFLASH_FLEXSPI);
    } while (0);
#endif

//#if defined(EXAMPLE_INVALIDATE_FLEXSPI_CACHE)
//    EXAMPLE_INVALIDATE_FLEXSPI_CACHE();
//#endif

    __asm("cpsie i");

    return 0;
}

/* API - initialize 'mflash' */
int32_t mflash_drv_init(void)
{
    volatile int32_t result;
    /* Necessary to have double wrapper call in non_xip memory */
    result = mflash_drv_init_internal();

    return result;
}

/* Write data to flash */
int32_t mflash_drv_write(void *sector_addr, uint8_t *a_data, uint32_t a_len, uint8_t *b_data, uint32_t b_len)
{
    /* Address not aligned to sector boundary */
    if (false == mflash_drv_is_sector_aligned((uint32_t)sector_addr))
        return -1;

    uint32_t total_len, ceiling_len;
    uint32_t block_i, block_max;
    uint32_t tmp_a_len = a_len, tmp_b_len = b_len;
    uint8_t *tmp_a_data = a_data, *tmp_b_data = b_data;
    uint32_t tmp_addr = (uint32_t)sector_addr;

    total_len = a_len + b_len;
    ceiling_len = (total_len + (MFLASH_PAGE_MASK)) & (~((uint32_t)(MFLASH_PAGE_MASK)));
    block_max = ceiling_len / (MFLASH_PAGE_SIZE);

    for (block_i = 0; block_i < block_max; block_i++)
    {
        /* Erase sector */
        if ((tmp_addr & MFLASH_SECTOR_MASK) == 0)
        {
            mflash_drv_sector_erase(tmp_addr);
        }

        /* Process word-aligned a_data */
        if (tmp_a_len >= (MFLASH_PAGE_SIZE))
        {
            memcpy(g_tmp_data, tmp_a_data, MFLASH_PAGE_SIZE);
            program_page(tmp_addr, (uint32_t *)g_tmp_data);
            tmp_a_data += (MFLASH_PAGE_SIZE);
            tmp_a_len -= (MFLASH_PAGE_SIZE);
        }
        /* Process word-aligned b_data */
        else if (tmp_a_len == 0 && tmp_b_len >= (MFLASH_PAGE_SIZE))
        {
            memcpy(g_tmp_data, tmp_b_data, MFLASH_PAGE_SIZE);
            program_page(tmp_addr, (uint32_t *)g_tmp_data);
            tmp_b_data += (MFLASH_PAGE_SIZE);
            tmp_b_len -= (MFLASH_PAGE_SIZE);
        }
        /* Process word-unaligned a/b_data */
        else if ((tmp_a_len && tmp_a_len < (MFLASH_PAGE_SIZE)) || (tmp_b_len && tmp_b_len < (MFLASH_PAGE_SIZE)))
        {
            uint32_t tmp_i = 0, tmp_len = 0;
            /* Trailing a_data */
            if (tmp_a_len)
            {
                memcpy(&g_tmp_data[tmp_i], tmp_a_data, tmp_a_len);
                tmp_i = tmp_a_len;
                tmp_a_len = 0;
            }
            /* Leading or trailing b_data */
            if (tmp_b_len)
            {
                tmp_len = (MFLASH_PAGE_SIZE)-tmp_i;
                tmp_len = tmp_len > tmp_b_len ? tmp_b_len : tmp_len;
                memcpy(&g_tmp_data[tmp_i], tmp_b_data, tmp_len);
                tmp_b_data += tmp_len;
                tmp_b_len -= tmp_len;
            }
            program_page(tmp_addr, (uint32_t *)g_tmp_data);
        }
        else
        {
            /* Should not happen */
            assert(0);
            return -1;
        }
        tmp_addr += (MFLASH_PAGE_SIZE);
    }

    return 0;
}
