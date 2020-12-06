/*
 * Copyright 2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software
 */

#ifndef _SLN_HYPER_FLASH_H_
#define _SLN_HYPER_FLASH_H_

#include "board.h"

#define EXAMPLE_FLEXSPI FLEXSPI
#define FLASH_SIZE BOARD_FLASH_SIZE
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

#endif /* _SLN_HYPER_FLASH_H_ */
