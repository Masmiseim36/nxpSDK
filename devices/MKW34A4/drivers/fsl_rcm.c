/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_rcm.h"

void RCM_ConfigureResetPinFilter(RCM_Type *base, const rcm_reset_pin_filter_config_t *config)
{
    assert(config);

#if (defined(FSL_FEATURE_RCM_REG_WIDTH) && (FSL_FEATURE_RCM_REG_WIDTH == 32))
    uint32_t reg;

    reg = (((uint32_t)config->enableFilterInStop << RCM_RPC_RSTFLTSS_SHIFT) | (uint32_t)config->filterInRunWait);
    if (config->filterInRunWait == kRCM_FilterBusClock)
    {
        reg |= ((uint32_t)config->busClockFilterCount << RCM_RPC_RSTFLTSEL_SHIFT);
    }
    base->RPC = reg;
#else
    base->RPFC = ((uint8_t)(config->enableFilterInStop << RCM_RPFC_RSTFLTSS_SHIFT) | (uint8_t)config->filterInRunWait);
    if (config->filterInRunWait == kRCM_FilterBusClock)
    {
        base->RPFW = config->busClockFilterCount;
    }
#endif /* FSL_FEATURE_RCM_REG_WIDTH */
}

#if (defined(FSL_FEATURE_RCM_HAS_BOOTROM) && FSL_FEATURE_RCM_HAS_BOOTROM)
void RCM_SetForceBootRomSource(RCM_Type *base, rcm_boot_rom_config_t config)
{
    uint32_t reg;

    reg = base->FM;
    reg &= ~RCM_FM_FORCEROM_MASK;
    reg |= ((uint32_t)config << RCM_FM_FORCEROM_SHIFT);
    base->FM = reg;
}
#endif /* #if FSL_FEATURE_RCM_HAS_BOOTROM */
