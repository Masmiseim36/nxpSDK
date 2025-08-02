/*
 * Copyright 2020, 2025 NXP
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
#if !defined(XIP_EXTERNAL_FLASH) && !(XIP_EXTERNAL_FLASH > 1)
#define USE_FLASH        1 /* Use FLASH */
#define FLASH_ERASE_CHIP 0 /* Do not erase whole FlexSPI chip data */
#endif                     /* XIP_EXTERNAL_FLASH */

#if USE_FLASH
#define EXAMPLE_FLEXSPI           FLEXSPI1
#define EXAMPLE_FLEXSPI           FLEXSPI1
#define FLASH_SIZE                0x10000 /* 64Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE           256
#define EXAMPLE_SECTOR            20
#define SECTOR_SIZE               0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK     kCLOCK_Flexspi1

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
#define FLASH_QUAD_ENABLE        0x02
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0
#endif                      /* USE_FLASH */

#define IEE IEE__IEE_RT1170 /* IEE defines */

/* Note: region is [bot:top), the end is open interval. So the bit[2:0] of the end address must be zero. */
#if !defined(USE_SDRAM)
#define StartAddr 0x202A0000
#define EndAddr   0x202AFFF8
#else
#define StartAddr 0x20360000
#define EndAddr   0x2036FFF8
#endif
/*${macro:end}*/

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
#if !(defined(XIP_EXTERNAL_FLASH))  /* If not XIP, set the clock divider to 1 */
    flexspi_transfer_t flashXfer;

    uint32_t ResetFlashCommandSeq[8] = {0U};

    
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
#endif /* XIP_EXTERNAL_FLASH */
}
/*${prototype:end}*/

#endif /* _APP_H_ */
