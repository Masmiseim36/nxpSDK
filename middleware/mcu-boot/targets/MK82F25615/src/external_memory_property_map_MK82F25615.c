/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "property/property.h"
#include "qspi.h"
#include "bootloader_common.h"

const external_memory_property_interface_t g_externalMemPropertyInterfaceMap[] = {
    { kMemoryQuadSpi0, quadspi_get_property }, { 0 }, // Terminator
};

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
