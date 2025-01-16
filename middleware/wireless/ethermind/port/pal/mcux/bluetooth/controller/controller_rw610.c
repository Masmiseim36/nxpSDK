/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fwk_platform_ble.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the platform */
void controller_init(void)
{
    PLATFORM_InitBle();
}
