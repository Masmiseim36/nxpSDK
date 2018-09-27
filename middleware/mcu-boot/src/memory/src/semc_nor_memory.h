/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SEMC_NOR_MEMORY_H__
#define __SEMC_NOR_MEMORY_H__

#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"
#include "semc/fsl_semc.h"
#include "semc_nor/semc_nor_flash.h"

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

#define SEMC_NOR_MAX_SIZE    (16U*1024*1024)
#define SEMC_NOR_BUFFER_PROG_CACHE_SIZE (512U)

typedef enum _semc_nor_property
{
    kSemcNorProperty_InitStatus = 0,
    kSemcNorProperty_StartAddress = 1,           //!< Tag used to retrieve start address
    kSemcNorProperty_TotalFlashSizeInKBytes = 2, //!< Tag used to retrieve total flash size in terms of KByte
    kSemcNorProperty_PageSize = 3,               //!< Tag used to retrieve page size in terms of byte
    kSemcNorProperty_BlockSize = 5,              //!< Tag used to retrieve block size in terms of byte

    kSemcNorProperty_TotalFlashSize = 0x10, //!< Tag used to retrieve total flash size in terms of byte
} semc_nor_property_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//! @name SEMC NOR memory
//@{

//! @brief Initialize SEMC NOR memory
status_t semc_nor_mem_init(void);

//! @brief Read SEMC NOR memory.
status_t semc_nor_mem_read(uint32_t address, uint32_t length, uint8_t *buffer);

//! @brief Write SEMC NOR memory.
status_t semc_nor_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer);

//! @brief Fill SEMC NOR memory with a word pattern.
status_t semc_nor_mem_fill(uint32_t address, uint32_t length, uint32_t pattern);

//! @brief Flush cached data into SEMC NOR memory.
status_t semc_nor_mem_flush(void);

//! @brief Erase SEMC NOR memory
status_t semc_nor_mem_erase(uint32_t address, uint32_t length);

status_t semc_nor_get_default_config_block(semc_nor_config_t *config);

//@}

#if defined(__cplusplus)
}
#endif
#endif /* #ifndef __SEMC_NOR_MEMORY_H__ */

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
