/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_flexspi.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI FLEXSPI
#define FLASH_SIZE 0x2000000
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define FLASH_PAGE_SIZE 512
#define EXAMPLE_SECTOR 2
#define SECTOR_SIZE 0x40000
#define EXAMPLE_FLEXSPI_CLOCK kCLOCK_FlexSpi
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA 0
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS 2
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE 4
#define HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR 6
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITE_BUF_SEQ2 10
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITE_BUF_SEQ3 11
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITE_BUF_SEQ4 12
#define HYPERFLASH_CMD_LUT_SEQ_IDX_WRITE_BUF_SEQ5 13
#define HYPERFLASH_CMD_LUT_SEQ_IDX_READVCR 14
#if 0                                      // Currently unavailable
#define HYPERFLASH_CMD_LUT_SEQ_IDX_PROGVCR // TBD
#endif
#define CUSTOM_LUT_LENGTH 64
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
