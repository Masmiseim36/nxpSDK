/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_cop.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void COP_GetDefaultConfig(cop_config_t *config)
{
    assert(config);

    config->enableWindowMode = false;
#if defined(FSL_FEATURE_COP_HAS_LONGTIME_MODE) && FSL_FEATURE_COP_HAS_LONGTIME_MODE
    config->timeoutMode = kCOP_LongTimeoutMode;
    config->enableStop = false;
    config->enableDebug = false;
#endif /* FSL_FEATURE_COP_HAS_LONGTIME_MODE */
    config->clockSource = kCOP_LpoClock;
    config->timeoutCycles = kCOP_2Power10CyclesOr2Power18Cycles;
}

void COP_Init(SIM_Type *base, const cop_config_t *config)
{
    assert(config);

    uint32_t value = 0U;

#if defined(FSL_FEATURE_COP_HAS_LONGTIME_MODE) && FSL_FEATURE_COP_HAS_LONGTIME_MODE
    value = SIM_COPC_COPW(config->enableWindowMode) | SIM_COPC_COPCLKS(config->timeoutMode) |
            SIM_COPC_COPT(config->timeoutCycles) | SIM_COPC_COPSTPEN(config->enableStop) |
            SIM_COPC_COPDBGEN(config->enableDebug) | SIM_COPC_COPCLKSEL(config->clockSource);
#else
    value = SIM_COPC_COPW(config->enableWindowMode) | SIM_COPC_COPCLKS(config->clockSource) |
            SIM_COPC_COPT(config->timeoutCycles);
#endif /* FSL_FEATURE_COP_HAS_LONGTIME_MODE */
    base->COPC = value;
}

void COP_Refresh(SIM_Type *base)
{
    uint32_t primaskValue = 0U;

    /* Disable the global interrupt to protect refresh sequence */
    primaskValue = DisableGlobalIRQ();
    base->SRVCOP = COP_FIRST_BYTE_OF_REFRESH;
    base->SRVCOP = COP_SECOND_BYTE_OF_REFRESH;
    EnableGlobalIRQ(primaskValue);
}
