// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2022 NXP
 */

#ifndef __SOEM_ENET_H___
#define __SOEM_ENET_H___
#include "fsl_enet.h"
#include "fsl_phy.h"
#include <stdint.h>

struct enet_if_port
{
    phy_handle_t phyHandle;
    enet_mii_mode_t mii_mode;
    phy_config_t phy_config;
    ENET_Type *base;
    enet_handle_t g_handle;
    enet_buffer_config_t *bufferConfig;
    uint32_t srcClock_Hz;
    uint32_t phy_autonego_timeout_count;
    uint32_t phy_stability_delay_us;
};
#endif
