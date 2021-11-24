/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLEXSPI_NOR_FLASH_OPS_H_
#define _FLEXSPI_NOR_FLASH_OPS_H_

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

status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr);

status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base);

status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);

status_t flexspi_nor_flash_erase_block(FLEXSPI_Type *base, uint32_t address);

status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);

status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);

status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);

status_t flexspi_nor_enter_qpi_mode(FLEXSPI_Type *base);

status_t flexspi_nor_flash_init(FLEXSPI_Type *base);

#endif /* _FLEXSPI_NOR_FLASH_OPS_H_ */
