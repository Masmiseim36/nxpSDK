/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "rng.h"
#include "fsl_rng.h"

hal_rng_status_t HAL_RngInit()
{
    trng_config_t user_config;
    TRNG_GetDefaultConfig(&user_config);

    /* Initialise RNG */
    TRNG_Init(RNG, &user_config);
    return kStatus_HAL_RngSuccess;
}

void HAL_RngDeinit()
{
    TRNG_Deinit(RNG);
    return;
}

hal_rng_status_t HAL_RngGetData(void *pRandomNo, uint32_t dataSize)
{
    hal_rng_status_t status = kStatus_HAL_RngSuccess;

    if (!pRandomNo)
    {
        status = kStatus_HAL_RngNullPointer;
    }

    if (kStatus_Success != TRNG_GetRandomData(RNG, pRandomNo, dataSize))
        return kStatus_HAL_RngInternalError;

    return status;
}

hal_rng_status_t HAL_RngSetSeed(uint32_t seed)
{
    return KStatus_HAL_RngNotSupport;
}
