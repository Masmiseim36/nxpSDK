/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_FLASH_CONFIG_W25Q256JVS_H_
#define _SLN_FLASH_CONFIG_W25Q256JVS_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLEXSPI_CLOCK     BOARD_FLEXPSPI_CLOCK
#define FLEXSPI_AMBA_BASE BOARD_FLEXSPI_AMBA_BASE
#define FLASH_SIZE        BOARD_FLASH_SIZE
#define FLASH_PAGE_SIZE   256
#define FLASH_SECTOR_SIZE 0x1000
#define FLASH_VENDOR_ID   0xEF

#define FLASH_BLOCK_SIZE    0x10000
#define ERASE_BLOCK_SUPPORT 1

#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD         0
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD_4BYTE   1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE            2
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD       3
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD_4BYTE 4
#define NOR_CMD_LUT_SEQ_IDX_READID                 5
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG          6
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG2         7
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG3         8
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG         9
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR            10
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_4BYTE      11
#define NOR_CMD_LUT_SEQ_IDX_ENTER4BAADDRESS        12
#define NOR_CMD_LUT_SEQ_IDX_ENTERQUAD              13
#define NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK             14

#define NORFLASH_SIZE (FLASH_SIZE / 1024)

#define NORFLASH_ADDR_LENGTH   24
#define NORFLASH_ADDR_LENGTH4B 32

#define W25Q_WriteEnable  0x06
#define W25Q_WriteDisable 0x04

#define W25Q_ReadStatusReg1 0x05
#define W25Q_ReadStatusReg2 0x35
#define W25Q_ReadStatusReg3 0x15

#define W25Q_WriteStatusReg1 0x01
#define W25Q_WriteStatusReg2 0x31
#define W25Q_WriteStatusReg3 0x11

#define W25Q_ReadData           0x03
#define W25Q_ReadData4B         0x13
#define W25Q_FastReadData       0x0B
#define W25Q_FastReadData4B     0x0C
#define W25Q_FastReadDual       0x3B
#define W25Q_FastReadDual4B     0x3C
#define W25Q_FastReadQuad       0xEB
#define W25Q_FastReadQuad4B     0xEC
#define W25Q_Enter4BAddressMode 0xB7
#define W25Q_Exit4BAddressMode  0xE9

#define W25Q_PageProgram       0x02
#define W25Q_PageProgram4b     0x12
#define W25Q_PageProgramQuad   0x32
#define W25Q_PageProgramQuad4B 0x34

#define W25Q_BlockErase    0xD8
#define W25Q_SectorErase   0x20
#define W25Q_SectorErase4B 0x21

#define W25Q_ChipErase        0xC7
#define W25Q_PowerDown        0xB9
#define W25Q_ReleasePowerDown 0xAB
#define W25Q_DeviceID         0x4B
#define W25Q_ManufactDeviceID 0x90
#define W25Q_JedecDeviceID    0x9F

#define CUSTOM_LUT_LENGTH        64
#define FLASH_QUAD_ENABLE        0x2
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0

#endif /* _SLN_FLASH_CONFIG_W25Q256JVS_H_ */
