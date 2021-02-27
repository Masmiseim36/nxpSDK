/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __BL_API_H__
#define __BL_API_H__

#include "bl_flexspi.h"
#include "flexspi_nor_flash.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
//!@brief FLEXSPI ROOT CLOCK soruce related definitions
enum
{
    kFlexSpiClockSrc_MainClk = 0,
    kFlexSpiClockSrc_MainPllClk,
    kFlexSpiClockSrc_Aux0PllClk,
    kFlexSpiClockSrc_FRO192M_Clk,
    kFlexSpiClockSrc_Aux1PllClk,
};

enum
{
    kFlexSpiSerialClk_32MHz = 1,
    kFlexSpiSerialClk_48MHz = 2,
    kFlexSpiSerialClk_64MHz = 3,
    kFlexSpiSerialClk_96MHz = 4,
    kFlexSpiSerialClk_192MHz = 5,
};

#define FLEXSPI_NOR_DRIVER_VER_1_0_0 0x00010000
#define FLEXSPI_NOR_DRIVER_VER_1_0_1 0x00010001

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
void bootloader_user_entry(void *arg);

#endif // __BL_API_H__
