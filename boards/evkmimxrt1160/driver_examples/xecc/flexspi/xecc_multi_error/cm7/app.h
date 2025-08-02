/*
 * Copyright 2021, 2025 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_flexspi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* XECC */
#define EXAMPLE_XECC                  XECC_FLEXSPI1
#define EXAMPLE_XECC_IRQ              XECC_FLEXSPI1_FATAL_INT_IRQn
#define EXAMPLE_XECC_IRQ_HANDLER      XECC_FLEXSPI1_FATAL_INT_IRQHandler
#define EXAMPLE_XECC_BUSFAULT_HANDLER BusFault_Handler

/* FLEXSPI*/
#define EXAMPLE_FLEXSPI           FLEXSPI1
#define FLASH_SIZE                0x4000U
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE           256U
#define EXAMPLE_SECTOR            10U
#define SECTOR_SIZE               0x1000U
#define EXAMPLE_FLEXSPI_CLOCK     kCLOCK_Flexspi1
#define FLASH_PORT                      kFLEXSPI_PortA1
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkLoopbackFromDqsPad

#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL        7
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST          13
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD     0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS         1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE        2
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR        3
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD   4
#define NOR_CMD_LUT_SEQ_IDX_READID             8
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG     9
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI           10
#define NOR_CMD_LUT_SEQ_IDX_EXITQPI            11
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG      12
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP          5

#define CUSTOM_LUT_LENGTH        60
#define FLASH_QUAD_ENABLE        0x40
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0

/*
 * If cache is enabled, this example should maintain the cache to make sure
 * CPU core accesses the memory, not cache only.
 */
#define CACHE_MAINTAIN 1

/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
#if (defined CACHE_MAINTAIN) && (CACHE_MAINTAIN == 1)
typedef struct _flexspi_cache_status
{
    volatile bool DCacheEnableFlag;
    volatile bool ICacheEnableFlag;
} flexspi_cache_status_t;
#endif
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
static inline void flexspi_clock_init(void)
{
    /*Clock setting for flexspi1*/
    CLOCK_SetRootClockDiv(kCLOCK_Root_Flexspi1, 2);
    CLOCK_SetRootClockMux(kCLOCK_Root_Flexspi1, 0);

#if !(defined(XIP_EXTERNAL_FLASH))    
    flexspi_transfer_t flashXfer;

    uint32_t ResetFlashCommandSeq[8] = {0U};

    FLEXSPI_Enable(EXAMPLE_FLEXSPI, false);
    FLEXSPI_Enable(EXAMPLE_FLEXSPI, true);
    ResetFlashCommandSeq[0] = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x66, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00);
    ResetFlashCommandSeq[4] = FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x99, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00);
    FLEXSPI_UpdateLUT(EXAMPLE_FLEXSPI, 4 * 14, ResetFlashCommandSeq, 8);

    /* Reset */
    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;

    flashXfer.seqIndex = 14;

    (void)FLEXSPI_TransferBlocking(EXAMPLE_FLEXSPI, &flashXfer);
    flashXfer.seqIndex = 15;
    (void)FLEXSPI_TransferBlocking(EXAMPLE_FLEXSPI, &flashXfer);
#endif
}
/*${prototype:end}*/

#endif /* _APP_H_ */
