/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_FLASH_CONFIG_MT25QL256ABA_H_
#define _SLN_FLASH_CONFIG_MT25QL256ABA_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLEXSPI_CLOCK     BOARD_FLEXPSPI_CLOCK
#define FLEXSPI_AMBA_BASE BOARD_FLEXSPI_AMBA_BASE
#define FLASH_SIZE        BOARD_FLASH_SIZE
#define FLASH_PAGE_SIZE   256
#define FLASH_SECTOR_SIZE 0x1000
#define FLASH_VENDOR_ID   0x20

#define FLASH_BLOCK_SIZE    0x10000
#define ERASE_BLOCK_SUPPORT 1

#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD     0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS         1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE        2
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR        3
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD   4
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP          5
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 6
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL        7
#define NOR_CMD_LUT_SEQ_IDX_READID             8
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG     9
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI           10
#define NOR_CMD_LUT_SEQ_IDX_ENTER4BYTES        11
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG      12
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST          13
#define NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK         14

#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_SINGLE    1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_QUAD      2
#define NOR_CMD_LUT_SEQ_IDX_EXITQPI               10
#define NOR_CMD_LUT_SEQ_IDX_WRITE_EXTADDRREG      14
#define NOR_CMD_LUT_SEQ_IDX_WRITE_VOLATILECONFREG 15

#define CUSTOM_LUT_LENGTH        64
#define FLASH_QUAD_ENABLE        0x40
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0

#endif /* _SLN_FLASH_CONFIG_MT25QL256ABA_H_ */
