/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SRAM_INIT_H__
#define __SRAM_INIT_H__

#include <stdint.h>
#include "memory/memory.h"

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

//! @brief SRAM size-related constants of KL series (CM0+ family)
enum _kl_serials_sram_constants
{
    kMaxRamIndex = 8u,
    kMinKlRamSize = (uint32_t)512u
};

#if defined(__CORE_CM4_H_GENERIC) || defined(__CORE_CM7_H_GENERIC)
//! @brief valid SRAM size index of K series (CM4 family)
enum _k_serials_sram_index_constants
{
    kRamSize16kIndex = 3u,
    kRamSize24kIndex = 4u,
    kRamSize32kIndex = 5u,
    kRamSize48kIndex = 6u,
    kRamSize64kIndex = 7u,
    kRamSize96kIndex = 8u,
    kRamSize128kIndex = 9u,
    kRamSize256kIndex = 11u,
    kRamSize512kIndex = 13u,
    kRamSize1024kIndex = 15u
};
#endif

#if defined(__CORE_CM0PLUS_H_GENERIC)
enum _k_serials_sram_index_constants
{
    kRamSize0p5kIndex = 0u,
    kRamSize1kIndex = 1u,
    kRamSize2kIndex = 2u,
    kRamSize4kIndex = 3u,
    kRamSize8kIndex = 4u,
    kRamSize16kIndex = 5u,
    kRamSize32kIndex = 6u,
    kRamSize64kIndex = 7u,
    kRamSize128kIndex = 9u
};
#endif

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

//! @brief Initialize sram interface.
status_t sram_init(void);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
