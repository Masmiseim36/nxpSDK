/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_flexspi.h"
#include "fsl_flexspi_flr.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FOLLOWER_STATUS_AXI_ACCESS \
    (FLEXSPI_SLV_MODULE_STATUS_ALLOWAXIWR_MASK | FLEXSPI_SLV_MODULE_STATUS_ALLOWAXIRD_MASK)
#define FOLLOWER_STATUS_AXI_WRITE      (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_WIP_MASK)
#define FOLLOWER_STATUS_AXI_READ       (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_AXIREADIDLE_MASK)
#define FOLLOWER_STATUS_REG_ACCESS     (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_REGRWIDLE_MASK)
#define FOLLOWER_STATUS_AXI_WRITE_IDLE FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK

#ifndef FOLLOWER_READY_WAIT_100MS_COUNT
#define FOLLOWER_READY_WAIT_100MS_COUNT 100U
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t flexspi_ocram_send_mailbox(FLEXSPI_Type *base, uint8_t index, uint32_t value);
extern status_t flexspi_ocram_get_mailbox(FLEXSPI_Type *base, uint8_t index, uint32_t *value);
extern status_t flexspi_ocram_status_get(FLEXSPI_Type *base, uint32_t *value);
extern status_t flexspi_ocram_read_memory(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length);
extern status_t flexspi_ocram_write_memory(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length);
extern void flexspi_ocram_init(FLEXSPI_Type *base);
extern flexspi_device_config_t deviceconfig;
/*******************************************************************************
 * Variables
 ******************************************************************************/
#ifdef EXAMPLE_RUN_FLEXSPI_LEADER
/* Program data buffer should be 4-bytes alignment, which can avoid busfault due to this memory region is configured as
   Device Memory by MPU. */
SDK_ALIGN(static char s_follower_write_buffer[256], 4);
SDK_ALIGN(static char s_follower_read_buffer[256], 4);
#else
static char *io_mode[4] = {"SDRx4", "SDRx8", "DDRx4", "DDRx8"};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#ifndef EXAMPLE_RUN_FLEXSPI_LEADER
void flexspi_flr_callback(FLEXSPI_SLV_Type *base, flexspi_slv_handle_t *handle)
{
    uint32_t mailBox[FLEXSPI_SLV_SPIMAIL_COUNT] = {0};
    size_t rdCount, wrCount;

    if ((handle->intrMask & (uint32_t)kFLEXSPI_SLV_ErrorCommandFlag) != 0U)
    {
        /* Mask this log in case test reports error when there's glitch on bus. FLR ignores them. */
        //(void)PRINTF("[Follower](interrupt) Error command!\r\n");
    }
    if ((handle->intrMask & ((uint32_t)kFLEXSPI_SLV_WriteOverflowFlag | (uint32_t)kFLEXSPI_SLV_ReadUnderflowFlag)) !=
        0U)
    {
        FLEXSPI_SLV_GetOutOfRangeCounts(base, &rdCount, &wrCount);
        (void)PRINTF("[Follower](interrupt) Write error count = %u. Read error count = %u!\r\n", wrCount, rdCount);
    }
    if ((handle->intrMask & (uint32_t)kFLEXSPI_SLV_MailInterruptFlag) != 0U)
    {
        (void)PRINTF("[Follower](interrupt) Mailbox data: ");
        for (uint32_t i = 0; i < FLEXSPI_SLV_SPIMAIL_COUNT; i++)
        {
            mailBox[i] = FLEXSPI_SLV_GetMailboxData(base, i);
            (void)PRINTF("Box[%u] 0x%X. ", i, mailBox[i]);
        }
        (void)PRINTF("\r\n");
    }
}
#endif

int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

#ifndef EXAMPLE_RUN_FLEXSPI_LEADER
    flexspi_slv_config_t config;
    flexspi_slv_handle_t handle;

    PRINTF("\r\nFLEXSPI Follower(Root Clock: %dMHz, IO Mode: %s) example started!\r\n", EXAMPLE_FLEXSPI_SLV_ROOT_CLOCK,
           io_mode[EXAMPLE_FLEXSPI_SLV_MODE]);

    FLEXSPI_SLV_GetDefaultConfig(&config);
    config.ioMode             = EXAMPLE_FLEXSPI_SLV_MODE;
    config.baseAddr1          = OCRAM_BASEADDR;
    config.baseAddr2          = OCRAM_BASEADDR;
    config.addrRange1         = MEMORY_SIZE;
    config.addrRange2         = MEMORY_SIZE;
    config.rxWatermark        = 32;
    config.txWatermark        = kFLEXSPI_SLV_Write_Watermark_256Bytes;
    config.readRegCmd         = FLEXSPI_SLV_CMD_DDR(FOLLOWER_CMD_READ_STATUS);
    config.readRegDummyCycle  = EXAMPLE_FLEXSPI_SLV_DUMMY_CYCLE;
    config.writeRegCmd        = FLEXSPI_SLV_CMD_DDR(FOLLOWER_CMD_SEND_MAILBOX);
    config.readMemCmd1        = FLEXSPI_SLV_CMD_DDR(FOLLOWER_CMD_READ_MEMORY);
    config.readMemDummyCycle1 = EXAMPLE_FLEXSPI_SLV_DUMMY_CYCLE;
    config.writeMemCmd1       = FLEXSPI_SLV_CMD_DDR(FOLLOWER_CMD_WRITE_MEMORY);

    FLEXSPI_SLV_Init(EXAMPLE_FLEXSPI_SLV, &config);
    FLEXSPI_SLV_InterruptCreateHandle(EXAMPLE_FLEXSPI_SLV, &handle, flexspi_flr_callback,
                                      kFLEXSPI_SLV_AllInterruptFlags);
    PRINTF("FLEXSPI Follower is initialized!\r\n");
#else
    status_t status;
    uint32_t slvStatus;
    uint8_t strLen;
    uint32_t data;

    deviceconfig.flexspiRootClk = FLEXSPI_GetRootClockFreqHz(EXAMPLE_FLEXSPI_ROOT_CLOCK_SRC);

    flexspi_ocram_init(EXAMPLE_FLEXSPI);
    PRINTF("FLEXSPI leader is initialized! Clock is %u\r\n", deviceconfig.flexspiRootClk);

    PRINTF("Wait for FLEXSPI Follower ready...\r\n");
    for (uint32_t timeout = 0; timeout < FOLLOWER_READY_WAIT_100MS_COUNT; timeout++)
    {
        /* Get FlexSPI Follower status. */
        status = flexspi_ocram_status_get(EXAMPLE_FLEXSPI, &slvStatus);
        if (status == kStatus_Success)
        {
            break;
        }

        SDK_DelayAtLeastUs(100000, CLOCK_GetFreq(kCLOCK_CpuClk));
    }
    if (status != kStatus_Success)
    {
        PRINTF("[%d] It's failed to get the follower status!\r\n", status);
        return status;
    }
    PRINTF("\r\n[Leader] The follower status: 0x%X\r\n", slvStatus);

    if ((slvStatus & FOLLOWER_STATUS_AXI_ACCESS) != FOLLOWER_STATUS_AXI_ACCESS)
    {
        PRINTF("[Leader] The follower AXI read/write access is not allowed!\r\n");
        return -2;
    }

    PRINTF("[Leader] Sending mailbox messages to the follower...\r\n");

    for (uint32_t i = FLEXSPI_SLV_SPIMAIL_COUNT - 1U; i < FLEXSPI_SLV_SPIMAIL_COUNT; i--)
    {
        do
        {
            status = flexspi_ocram_status_get(EXAMPLE_FLEXSPI, &slvStatus);
        } while (0U == (slvStatus & FLEXSPI_SLV_MODULE_STATUS_REGRWIDLE_MASK));

        /* Trigger interrupt to make follower read out the mail message. */
        data = (i == 0U) ? 1U : i;

        status = flexspi_ocram_send_mailbox(EXAMPLE_FLEXSPI, i, data);
        if (status != kStatus_Success)
        {
            PRINTF("[Leader] Send mailbox failure!\r\n");
            return status;
        }
    }

    /* Check whether the mail box interrupt is clear by follower. */
    do
    {
        status = flexspi_ocram_get_mailbox(EXAMPLE_FLEXSPI, 0, &slvStatus);
        if (status != kStatus_Success)
        {
            PRINTF("[Leader] Get mailbox failure!\r\n");
            return status;
        }
    } while ((slvStatus & 0x1U) != 0U);

    PRINTF("\r\n[Leader] Waiting the AXI bus write is idle...\r\n");
    do
    {
        status = flexspi_ocram_status_get(EXAMPLE_FLEXSPI, &slvStatus);
    } while (0U != (slvStatus & FLEXSPI_SLV_MODULE_STATUS_WIP_MASK));

    sprintf(s_follower_write_buffer, "[%s] This string is used to verify the memory write!", __TIME__);
    strLen = strlen(s_follower_write_buffer) + 1;
    PRINTF("[Leader] Original string: %s\r\n", s_follower_write_buffer);

    PRINTF("[Leader] Writing follower memory...\r\n");
    status =
        flexspi_ocram_write_memory(EXAMPLE_FLEXSPI, EXAMPLE_MEMORY_ADDR, (uint32_t *)s_follower_write_buffer, strLen);
    if (status != kStatus_Success)
    {
        PRINTF("[Leader] Write memory failure: %d!\r\n", status);
        return status;
    }
    PRINTF("[Leader] Write finished!\r\n");

    PRINTF("\r\n[Leader] Waiting that the AXI bus read is idle...\r\n");
    do
    {
        status = flexspi_ocram_status_get(EXAMPLE_FLEXSPI, &slvStatus);
    } while (0U == (slvStatus & FLEXSPI_SLV_MODULE_STATUS_AXIREADIDLE_MASK));

    PRINTF("[Leader] Reading the data from the follower memory...\r\n");
    memset(s_follower_read_buffer, 0, 256);
    status =
        flexspi_ocram_read_memory(EXAMPLE_FLEXSPI, EXAMPLE_MEMORY_ADDR, (uint32_t *)s_follower_read_buffer, strLen);
    if (status != kStatus_Success)
    {
        PRINTF("[Leader] Read memory failure!\r\n");
        return status;
    }
    PRINTF("[Leader] The read data: %s\r\n", s_follower_read_buffer);
    if (memcmp(&s_follower_read_buffer[0], &s_follower_write_buffer[0], 256) != 0)
    {
        PRINTF("[Leader] The read data doesn't match!\r\n");
        return -1;
    }

    /* AHB W/R case. */
    sprintf(s_follower_write_buffer, "[%s] Verified the memory write via the AHB bus!", __TIME__);
    strLen = strlen(s_follower_write_buffer) + 1;
    PRINTF("\r\n[Leader] Original string(AHB): %s\r\n", s_follower_write_buffer);
    PRINTF("[Leader] Writing follower memory via AHB bus...\r\n");
    memcpy((void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_MEMORY_ADDR_AHB), (void *)s_follower_write_buffer, strLen + 1);
    PRINTF("[Leader] Write finished(AHB)!\r\n");

    PRINTF("[Leader] Reading the data from the follower memory via AHB bus...\r\n");
    memset(s_follower_read_buffer, 0, 256);
    memcpy((void *)s_follower_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_MEMORY_ADDR_AHB), strLen + 1);
    PRINTF("[Leader] The read data(AHB): %s\r\n", s_follower_read_buffer);
    if (memcmp(&s_follower_read_buffer[0], &s_follower_write_buffer[0], strLen + 1) != 0)
    {
        PRINTF("[Leader] The read data doesn't match!\r\n");
        return -1;
    }
#endif
    while (1)
    {
    }
}
