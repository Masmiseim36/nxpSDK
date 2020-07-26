/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SPIFI_BOOT_H_
#define _SPIFI_BOOT_H_

#include <stdint.h>

#define BOOT_VERSION_STRING "1.0"
#define BOOT_PROMPT_STRING "[BOOT] "

/* Memory area reserverd for the bootloader itself at the beginning of the FLASH */
#define BOOT_RESERVED_AREA (0x20000)

/* Base address of the FLASH memory */
#define BOOT_FLASH_BASE (0x10000000)

/* Adress where XIP image is flashed and executed */
#define BOOT_EXEC_IMAGE_ADDR (BOOT_FLASH_BASE + BOOT_RESERVED_AREA)

/* Address of update controll block structure */
#define BOOT_UCB_ADDR (BOOT_FLASH_BASE + BOOT_RESERVED_AREA - MFLASH_SECTOR_SIZE)
#define BOOT_UCB_SIGNATURE 0x4243552A
#define BOOT_UCB_VERSION 0x00000010

#define BOOT_STATE_UNDEF 0xFFFFFFFF
#define BOOT_STATE_NEW 0xFFFFFF00
#define BOOT_STATE_PENDING_COMMIT 0xFFFF0000
#define BOOT_STATE_INVALID 0xFF000000
#define BOOT_STATE_VOID 0x00000000

/* Update control block structure */
struct boot_ucb
{
    uint32_t signature;
    uint32_t version;
    uint32_t flags; /* reserved, not used in the current version */
    uint32_t state;
    void *update_img;
    uint32_t update_img_size; /* reserved, not used in the current version */
    void *rollback_img;
    uint32_t rollback_img_size; /* reserved, not used in the current version */
};

/* Boot image structure */
#define BOOT_IMAGE_MARKER 0xEDDC94BD
#define BOOT_HEADER_MARKER 0xFEEDA5A5
#define BOOT_HEADER_MAX_OFFSET 0x19C

struct boot_image
{
    uint32_t vector_table_0x1c[7];
    uint32_t vector_table_chksum;
    uint32_t ecrp;
    uint32_t image_marker;
    uint32_t header_offset;
};

struct boot_image_header
{
    uint32_t header_marker;
    uint32_t image_type;
    uint32_t load_address;
    uint32_t image_length;
    uint32_t crc_value;
    uint32_t image_version;
    uint32_t emc_timings;
    uint32_t spi_clock_freq;
    uint32_t _reserved_0x20;
    uint32_t image_marker;
    uint32_t _reserved_0x28;
    uint32_t _reserved_0x2C;
};

extern int32_t boot_ucb_read(struct boot_ucb *ucbp);
extern int32_t boot_ucb_write(const struct boot_ucb *ucbp);
extern int32_t boot_ucb_erase(void);

extern int32_t boot_update_request(void *update_img, void *backup_storage);
extern void boot_cpureset(void);
extern void boot_wdtdis(void);

extern void boot_app_exec(const void *addr);
extern int32_t boot_run(void);

#endif
