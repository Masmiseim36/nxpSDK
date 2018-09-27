/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/Crc.h"
#include "property/property.h"
#include "crc/crc32.h"

using namespace blfwk;

// See Crc.h for documentation of this method.
uint32_t Crc::calculate_application_crc32(const uint8_t *start, uint32_t length)
{
    uint32_t crc32;

    // Initialize the CRC32 information
    crc32_data_t crcInfo;
    crc32_init(&crcInfo);

    // Run CRC, Considering skip crcExpectedValue address
    const bootloader_configuration_data_t *config =
        NULL; // Just used for calculate the offset, must not change the region it points to.
    uint32_t bypassStartAddress = 0x3C0 + ((uint32_t)&config->crcExpectedValue - (uint32_t)&config->tag);
    uint32_t bypassEndAddress = bypassStartAddress + sizeof(config->crcExpectedValue);

    if (length <= bypassStartAddress)
    {
        crc32_update(&crcInfo, (uint8_t *)start, length);
    }
    else
    {
        // Assume that crcExpectedValue address (4 byte) resides in crc addresses completely
        crc32_update(&crcInfo, (uint8_t *)start, bypassStartAddress);
        crc32_update(&crcInfo, (uint8_t *)(start + bypassEndAddress), length - bypassEndAddress);
    }

    // Finalize the CRC calculations
    crc32_finalize(&crcInfo, &crc32);

    return crc32;
}
