/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLEXSPI_HYPER_FLASH_OPS_H_
#define _FLEXSPI_HYPER_FLASH_OPS_H_

#include "sln_flash_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t flexspi_nor_hyperbus_read(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes);

status_t flexspi_nor_hyperbus_write(FLEXSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes);

status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr);

status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base);

status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);

status_t flexspi_nor_flash_page_program_with_buffer(FLEXSPI_Type *base, uint32_t address, const uint32_t *src);

status_t flexspi_nor_read_vcr(FLEXSPI_Type *base, uint32_t *vcr);

status_t flexspi_nor_write_vcr(FLEXSPI_Type *base, uint32_t *vcr);

status_t flexspi_nor_hyperflash_cfi(FLEXSPI_Type *base);

status_t flexspi_nor_hyperflash_id(FLEXSPI_Type *base, uint8_t *pid);

#endif /* _FLEXSPI_HYPER_FLASH_OPS_H_ */
