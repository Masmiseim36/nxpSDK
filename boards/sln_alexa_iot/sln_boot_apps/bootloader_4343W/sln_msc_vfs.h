/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_MSC_VFS_H_
#define _SLN_MSC_VFS_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLASH_BYTE4_UPPER_NIBBLE 0xF0000000
#define FLASH_BYTE3_UPPER_NIBBLE 0x00F00000

typedef enum __transfer_state
{
    TRANSFER_IDLE,
    TRANSFER_START,
    TRANSFER_ACTIVE,
    TRANSFER_FINAL,
    TRANSFER_ERROR
} msc_vfs_state_t;

typedef struct __attribute__((packed)) __fat_file
{
    uint8_t name[11]; /*!< File name (0-7) and 3-byte extension (8-10) */
    uint8_t attributes;
    uint8_t reserved;
    uint8_t time_created_ms;
    uint16_t time_created;
    uint16_t date_created;
    uint16_t date_accessed;
    uint16_t first_cluser_upper;
    uint16_t time_modified;
    uint16_t date_modified;
    uint16_t first_cluster_lower;
    uint32_t size;
} fat_file_t;

typedef struct __attribute__((packed)) __fat_mbr
{
    uint8_t jump_instr[3];
    uint8_t oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t num_fats;
    uint16_t root_entries;
    uint16_t num_logical_sectors;
    uint8_t media_type;
    uint16_t logical_sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t large_sectors;
    uint8_t physical_drive_number;
    uint8_t not_used;
    uint8_t boot_record_signature;
    uint32_t volume_sn;
    char volume_label[11];
    char fat_type[8];
    uint8_t bootstrap[448];
    uint16_t signature;
} fat_mbr_t;

/*******************************************************************************
 * API
 ******************************************************************************/

status_t MSC_VFS_Init(uint8_t *storageDisk, TaskHandle_t *usbAppTask, uint32_t lbaLength);

status_t MSC_VFS_WriteResponse(uint32_t offset, uint32_t size, uint8_t *buffer);

msc_vfs_state_t MSC_VFS_GetTransferState(void);

void MSC_VFS_SetTransferState(msc_vfs_state_t transferState);

#endif /* _SLN_MSC_VFS_H_ */
