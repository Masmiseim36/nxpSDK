/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MFLASH_DRV_H__
#define __MFLASH_DRV_H__

#include <stdbool.h>
#include <stdint.h>

/* Flash constants */
#ifndef MFLASH_SECTOR_SIZE
#define MFLASH_SECTOR_SIZE (4096U)
#endif

#ifndef MFLASH_SECTOR_MASK
#define MFLASH_SECTOR_MASK ((MFLASH_SECTOR_SIZE)-1)
#endif

#ifndef MFLASH_PAGE_SIZE
#define MFLASH_PAGE_SIZE (256U)
#endif

#ifndef MFLASH_PAGE_MASK
#define MFLASH_PAGE_MASK ((MFLASH_PAGE_SIZE)-1)
#endif

#define MFLASH_FLEXSPI FLEXSPI
#define FLASH_SIZE     0x04000000U

#define mflash_drv_is_page_aligned(x)   (((x) % (MFLASH_PAGE_SIZE)) == 0)
#define mflash_drv_is_sector_aligned(x) (((x) % (MFLASH_SECTOR_SIZE)) == 0)

int32_t mflash_drv_sector_erase(uint32_t sector_addr);
int32_t mflash_drv_page_program(uint32_t page_addr, uint32_t *data);

int32_t mflash_drv_init(void);
int32_t mflash_drv_write(uint32_t addr, const uint8_t *data, uint32_t data_len);
int32_t mflash_drv_read(uint32_t addr, uint32_t *buffer, uint32_t len);
void *mflash_drv_mmap(uint32_t addr, uint32_t len);

#endif
